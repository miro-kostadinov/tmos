/*
 * mpi.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <mpi.h>
#include <asn1.h>


#if MPI_SUPPORT

#if (MPI_ASM_SUPPORT == DISABLED)

/**
 * @brief Multiply-accumulate operation
 * @param[out] r Resulting integer
 * @param[in] a First operand A
 * @param[in] m Size of A in words
 * @param[in] b Second operand B
 **/

void mpiMulAccCore(uint32_t *r, const uint32_t *a, int m, const uint32_t b)
{
   int i;
   uint32_t c;
   uint32_t u;
   uint32_t v;
   uint64_t p;

   //Clear variables
   c = 0;
   u = 0;
   v = 0;

   //Perform multiplication
   for(i = 0; i < m; i++)
   {
      p = (uint64_t) a[i] * b;
      u = (uint32_t) p;
      v = (uint32_t) (p >> 32);

      u += c;
      if(u < c) v++;

      u += r[i];
      if(u < r[i]) v++;

      r[i] = u;
      c = v;
   }

   //Propagate carry
   for(; c != 0; i++)
   {
      r[i] += c;
      c = (r[i] < c);
   }
}

#endif

void Mpi::mpi_free()
{
	if(mpi_refs  && mpi_data )
	{
		delete mpi_data;
		mpi_refs = 0;
	}
	mpi_size = 0;
	mpi_data = nullptr;
}


RES_CODE Mpi::mpiGrow(uint32_t size)
{
	uint32_t* data;

	//Ensure the parameter is valid
	size = max(size, 1u);

	//Check the current size
	if (mpi_size >= size)
		return RES_OK;

	if(size > MPI_MAX_SIZE)
		return RES_TLS_INVALID_LENGTH;

	//Allocate a memory buffer
	if(mpi_refs)
	{
		data = (uint32_t*)tsk_realloc(mpi_data, size * MPI_INT_SIZE);
		if (data == nullptr)
			return RES_OUT_OF_MEMORY;
	}
	else
	{
		data = (uint32_t*)tsk_malloc(size * MPI_INT_SIZE);
		if (data == nullptr)
			return RES_OUT_OF_MEMORY;

		mpi_refs = 1;
		if(mpi_size)
			memcpy(data, mpi_data, mpi_size * MPI_INT_SIZE);
	}
	mpi_data = data;

	while(mpi_size < size)
		mpi_data[mpi_size++] = 0;

	return RES_OK;
}

RES_CODE Mpi::mpiReadRaw(const uint8_t* data, uint32_t length)
{
	RES_CODE res;
	uint32_t i;

	//Skip leading zeroes
	while (length > 1 && *data == 0)
	{
		//Advance read pointer
		data++;
		length--;
	}

	//Ajust the size of the multiple precision integer
	res = mpiGrow((length + MPI_INT_SIZE - 1) / MPI_INT_SIZE);
	//Failed to adjust the size?
	if (res != RES_OK)
		return res;

	//Clear the contents of the multiple precision integer
	memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);
	//Set sign
	mpi_sign = 1;

	//Start from the least significant byte
	data += length - 1;

	//Copy data
	for (i = 0; i < length; i++, data--)
		mpi_data[i / MPI_INT_SIZE] |= *data << ((i % MPI_INT_SIZE) * 8);

	//The conversion succeeded
	return RES_OK;
}

RES_CODE Mpi::mpiWriteRaw(uint8_t *data, uint32_t length) const
{
	uint32_t i;

	//Get the actual length in bytes
	uint32_t n = mpiGetByteLength();

	//Make sure the output buffer is large enough
	if (n > length)
		return RES_TLS_INVALID_LENGTH;

	//Clear output buffer
	memset(data, 0, length);

	//Start from the least significant word
	data += length - 1;

	//Copy data
	for (i = 0; i < n; i++, data--)
		*data = mpi_data[i / MPI_INT_SIZE] >> ((i % MPI_INT_SIZE) * 8);

	//The conversion succeeded
	return RES_OK;
}

uint32_t Mpi::mpiGetLength() const
{
	int i;

	//Check whether the specified multiple precision integer is empty
	if (mpi_size == 0)
		return 0;

	//Start from the most significant word
	for (i = mpi_size - 1; i >= 0; i--)
	{
		//Loop as long as the current word is zero
		if (mpi_data[i] != 0)
			break;
	}

	//Return the actual length
	return i + 1;
}

uint32_t Mpi::mpiGetByteLength() const
{
	uint32_t n;
	uint32_t m;

	//Check whether the specified multiple precision integer is empty
	if (mpi_size == 0)
		return 0;

	//Start from the most significant word
	for (n = mpi_size-1; n > 0; n--)
	{
		//Loop as long as the current word is zero
		if (mpi_data[n] != 0)
			break;
	}

	//Get the current word
	m = mpi_data[n];
	//Convert the length to a byte count
	n *= MPI_INT_SIZE;

	//Adjust the byte count
	for (; m != 0; m >>= 8)
		n++;

	//Return the actual length in bytes
	return n;
}

uint32_t Mpi::mpiGetBitLength() const
{
	uint32_t n;
	uint32_t m;

	//Check whether the specified multiple precision integer is empty
	if (mpi_size == 0)
		return 0;

	//Start from the most significant word
	for (n = mpi_size - 1; n > 0; n--)
	{
		//Loop as long as the current word is zero
		if (mpi_data[n] != 0)
			break;
	}

	//Get the current word
	m = mpi_data[n];
	//Convert the length to a bit count
	n *= MPI_INT_SIZE * 8;

	//Adjust the bit count
	for (; m != 0; m >>= 1)
		n++;

	//Return the actual length in bits
	return n;
}

RES_CODE Mpi::mpiSetBitValue(uint32_t index, uint32_t value)
{
	RES_CODE res;
	uint32_t n1;
	uint32_t n2;

	//Retrieve the position of the bit to be written
	n1 = index / (MPI_INT_SIZE * 8);
	n2 = index % (MPI_INT_SIZE * 8);

	//Ajust the size of the multiple precision integer if necessary
	res = mpiGrow((index + (MPI_INT_SIZE * 8) - 1) / (MPI_INT_SIZE * 8));
	if (res == RES_OK)
	{
		//Set bit value
		if (value)
			mpi_data[n1] |= (1 << n2);
		else
			mpi_data[n1] &= ~(1 << n2);

	}

	return res;
}

uint32_t Mpi::mpiGetBitValue(uint32_t index) const
{
	uint32_t n1;
	uint32_t n2;

	//Retrieve the position of the bit to be read
	n1 = index / (MPI_INT_SIZE * 8);
	n2 = index % (MPI_INT_SIZE * 8);

	//Index out of range?
	if (n1 >= mpi_size)
		return 0;

	//Return the actual bit value
	return (mpi_data[n1] >> n2) & 0x01;
}

int Mpi::mpiComp(const Mpi* b) const
{
	uint32_t m;
	uint32_t n;

	//Determine the actual length of A and B
	m = mpiGetLength();
	n = b->mpiGetLength();

	//Compare lengths
	if (!m && !n)
		return 0;
	else if (m > n)
		return mpi_sign;
	else if (m < n)
		return -b->mpi_sign;

	//Compare signs
	if (mpi_sign > 0 && b->mpi_sign < 0)
		return 1;
	else if (mpi_sign < 0 && b->mpi_sign > 0)
		return -1;

	//Then compare values
	while (n--)
	{
		if (mpi_data[n] > b->mpi_data[n])
			return mpi_sign;
		else if (mpi_data[n] < b->mpi_data[n])
			return -mpi_sign;
	}

	//Multiple precision integers are equals
	return 0;
}

int Mpi::mpiCompInt(int b) const
{
	int32_t sign;
	uint32_t m;

	sign = (b >= 0) ? 1 : -1;
	if (b < 0)
		b = -b;

	//Return comparison result
	//Determine the actual length of A and B
	m = mpiGetLength();

	//Compare lengths
	if (m > 1)
		return mpi_sign;
	else if (m < 1)
		return -sign;

	//Compare signs
	if (mpi_sign > 0 && sign < 0)
		return 1;
	else if (mpi_sign < 0 && sign > 0)
		return -1;

	//Then compare values
	if (mpi_data[0] > (unsigned)b)
		return mpi_sign;
	else if (mpi_data[0] < (unsigned)b)
		return -mpi_sign;

	//Multiple precision integers are equals
	return 0;
}

int Mpi::mpiCompAbs(const Mpi *b) const
{
	uint32_t m;
	uint32_t n;

	//Determine the actual length of A and B
	m = mpiGetLength();
	n = b->mpiGetLength();

	//Compare lengths
	if (!m && !n)
		return 0;
	else if (m > n)
		return 1;
	else if (m < n)
		return -1;

	//Then compare values
	while (n--)
	{
		if (mpi_data[n] > b->mpi_data[n])
			return 1;
		else if (mpi_data[n] < b->mpi_data[n])
			return -1;
	}

	//Operands are equals
	return 0;
}

RES_CODE Mpi::mpiCopy(const Mpi *a)
{
	RES_CODE res;
	uint32_t n;

	//R and A are the same instance?
	if (this == a)
		return RES_OK;

	//Determine the actual length of A
	n = a->mpiGetLength();

	//Ajust the size of the destination operand
	res = mpiGrow(n);
	if(res == RES_OK)
	{
		//Set the sign of R
		mpi_sign = a->mpi_sign;

		//Let R = A
		memcpy(mpi_data, a->mpi_data, n * MPI_INT_SIZE);
		memclr(mpi_data +n, (mpi_size -n) * MPI_INT_SIZE);
	}

	return res;
}

RES_CODE Mpi::mpiSetValue(int b)
{
	RES_CODE res;

	//Ajust the size of the destination operand
	res = mpiGrow(1);
	if (res == RES_OK)
	{
		//Clear the contents of the multiple precision integer
		memclr(mpi_data+1, (mpi_size-1) * MPI_INT_SIZE);
		//Set the value or R
		mpi_data[0] = (b >= 0) ? b : -b;
		//Set the sign of R
		mpi_sign = (b >= 0) ? 1 : -1;

	}

	return res;
}

/**
 * @brief Generate a random value
 * @param[out] r Pointer to a multiple precision integer
 * @param[in] length Desired length in bits
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @return Error code
 **/
RES_CODE Mpi::mpiRand(uint32_t length, prng_algo_t* prngAlgo)
{
	RES_CODE res;
	uint32_t m, n;

	//Compute the required length, in words
	n = (length + (MPI_INT_SIZE * 8) - 1) / (MPI_INT_SIZE * 8);
	//Number of bits in the most significant word
	m = length % (MPI_INT_SIZE * 8);

	//Ajust the size of the multiple precision integer if necessary
	res = mpiGrow( n);
	if (res != RES_OK)
		return res;

	//Clear the contents of the multiple precision integer
	memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);
	//Set the sign of R
	mpi_sign = 1;

	//Generate a random pattern
	res = prngAlgo->prng_read(mpi_data, n * MPI_INT_SIZE);
	if (res != RES_OK)
		return res;

	//Remove the meaningless bits in the most significant word
	if (n > 0 && m > 0)
		mpi_data[n - 1] &= (1 << m) - 1;

	//Successful operation
	return RES_OK;
}

/**
 * @brief Multiple precision addition
 * @param[out] r Resulting integer R = A + B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiAdd(const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	int sign;

	//Retrieve the sign of A
	sign = a->mpi_sign;

	//Both operands have the same sign?
	if (a->mpi_sign == b->mpi_sign)
	{
		//Perform addition
		res = mpiAddAbs(a, b);
		//Set the sign of the resulting number
		mpi_sign = sign;
	}
	//Operands have different signs?
	else
	{
		//Compare the absolute value of A and B
		if (a->mpiCompAbs(b) >= 0)
		{
			//Perform subtraction
			res = mpiSubAbs(a, b);
			//Set the sign of the resulting number
			mpi_sign = sign;
		}
		else
		{
			//Perform subtraction
			res = mpiSubAbs(b, a);
			//Set the sign of the resulting number
			mpi_sign = -sign;
		}
	}

	return res;
}

/**
 * @brief Add an integer to a multiple precision integer
 * @param[out] r Resulting integer R = A + B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiAddInt(const Mpi *a, int b)
{
	uint32_t value;
	Mpi t(b, &value);

	return mpiAdd(a, &t);
}

/**
 * @brief Multiple precision subtraction
 * @param[out] r Resulting integer R = A - B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiSub(const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	int sign;

	//Retrieve the sign of A
	sign = a->mpi_sign;

	//Both operands have the same sign?
	if (a->mpi_sign == b->mpi_sign)
	{
		//Compare the absolute value of A and B
		if (a->mpiCompAbs(b) >= 0)
		{
			//Perform subtraction
			res = mpiSubAbs(a, b);
			//Set the sign of the resulting number
			mpi_sign = sign;
		}
		else
		{
			//Perform subtraction
			res = mpiSubAbs(b, a);
			//Set the sign of the resulting number
			mpi_sign = -sign;
		}
	}
	//Operands have different signs?
	else
	{
		//Perform addition
		res = mpiAddAbs(a, b);
		//Set the sign of the resulting number
		mpi_sign = sign;
	}

	return res;
}

/**
 * @brief Subtract an integer from a multiple precision integer
 * @param[out] r Resulting integer R = A - B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiSubInt(const Mpi *a, int b)
{
	uint32_t value;
	Mpi t(b, &value);

	return mpiSub(a, &t);
}

/**
 * @brief Helper routine for multiple precision addition
 * @param[out] r Resulting integer R = |A + B|
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiAddAbs(const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	uint32_t n, c, d, i;

	//R and B are the same instance?
	if (this == b)
	{
		//Swap A and B
		const Mpi *t = a;
		a = b;
		b = t;
	}
	//R is neither A nor B?
	else if (this != a)
	{
		//Copy the first operand to R
		res = mpiCopy(a);
		if(res != RES_OK)
			return res;
	}

	//Determine the actual length of B
	n = b->mpiGetLength();
	//Extend the size of the destination register as needed
	res = mpiGrow(n);
	if(res != RES_OK)
		return res;

	//The result is always positive
	mpi_sign = 1;
	//Clear carry bit
	c = 0;

	//Add operands
	for (i = 0; i < n; i++)
	{
		//Add carry bit
		d = mpi_data[i] + c;
		//Update carry bit
		if (d != 0)
			c = 0;
		//Perform addition
		d += b->mpi_data[i];
		//Update carry bit
		if (d < b->mpi_data[i])
			c = 1;
		//Save result
		mpi_data[i] = d;
	}

	//Loop as long as the carry bit is set
	for (i = n; c && i < mpi_size; i++)
	{
		//Add carry bit
		mpi_data[i] += c;
		//Update carry bit
		if (mpi_data[i] != 0)
			c = 0;
	}

	//Check the final carry bit
	if (c && n >= mpi_size)
	{
		//Extend the size of the destination register
		res  = mpiGrow(n + 1);
		if(res != RES_OK)
			return res;
		//Add carry bit
		mpi_data[n] = 1;
	}

	return RES_OK;
}

/**
 * @brief Helper routine for multiple precision subtraction
 * @param[out] r Resulting integer R = |A - B|
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiSubAbs(const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	uint32_t c, d, i, m, n;

	//Check input parameters
	if (a->mpiCompAbs(b) < 0)
	{
		//Swap A and B if necessary
		const Mpi *t = b;
		a = b;
		b = t;
	}

	//Determine the actual length of A
	m = a->mpiGetLength();
	//Determine the actual length of B
	n = b->mpiGetLength();

	//Extend the size of the destination register as needed
	res= mpiGrow(m);
	if(res != RES_OK)
		return res;

	//The result is always positive
	mpi_sign = 1;
	//Clear carry bit
	c = 0;

	//Subtract operands
	for (i = 0; i < n; i++)
	{
		//Read first operand
		d = a->mpi_data[i];

		//Check the carry bit
		if (c)
		{
			//Update carry bit
			if (d != 0)
				c = 0;
			//Propagate carry bit
			d -= 1;
		}

		//Update carry bit
		if (d < b->mpi_data[i])
			c = 1;
		//Perform subtraction
		mpi_data[i] = d - b->mpi_data[i];
	}

	//Loop as long as the carry bit is set
	for (i = n; c && i < m; i++)
	{
		//Update carry bit
		if (a->mpi_data[i] != 0)
			c = 0;
		//Propagate carry bit
		mpi_data[i] = a->mpi_data[i] - 1;
	}

	//R and A are not the same instance?
	if (this != a)
	{
		//Copy the remaining words
		for (; i < m; i++)
			mpi_data[i] = a->mpi_data[i];

		//Zero the upper part of R
		for (; i < mpi_size; i++)
			mpi_data[i] = 0;
	}

	return RES_OK;
}

/**
 * @brief Left shift operation
 * @param[in,out] r The multiple precision integer to be shifted to the left
 * @param[in] n The number of bits to shift
 * @return Error code
 **/
RES_CODE Mpi::mpiShiftLeft(uint32_t n)
{
	RES_CODE res;
	uint32_t i;

	//Number of 32-bit words to shift
	uint32_t n1 = n / (MPI_INT_SIZE * 8);
	//Number of bits to shift
	uint32_t n2 = n % (MPI_INT_SIZE * 8);

	//Check parameters
	if (!mpi_size || !n)
		return RES_OK;

	//Increase the size of the multiple-precision number
	res = mpiGrow(mpi_size + (n + 31) / 32);
	if(res != RES_OK)
		return res;

	//First, shift words
	if (n1 > 0)
	{
		//Process the most significant words
		for (i = mpi_size - 1; i >= n1; i--)
			mpi_data[i] = mpi_data[i - n1];
		//Fill the rest with zeroes
		for (i = 0; i < n1; i++)
			mpi_data[i] = 0;
	}
	//Then shift bits
	if (n2 > 0)
	{
		//Process the most significant words
		for (i = mpi_size - 1; i >= 1; i--)
			mpi_data[i] = (mpi_data[i] << n2) | (mpi_data[i - 1] >> (32 - n2));
		//The least significant word requires a special handling
		mpi_data[0] <<= n2;
	}

	//Shift operation is complete
	return RES_OK;
}

/**
 * @brief Right shift operation
 * @param[in,out] r The multiple precision integer to be shifted to the right
 * @param[in] n The number of bits to shift
 * @return Error code
 **/
RES_CODE Mpi::mpiShiftRight(uint32_t n)
{
	uint32_t i, m;

	//Number of 32-bit words to shift
	uint32_t n1 = n / (MPI_INT_SIZE * 8);
	//Number of bits to shift
	uint32_t n2 = n % (MPI_INT_SIZE * 8);

	//Check parameters
	if (n1 >= mpi_size)
	{
		memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);
		return RES_OK;
	}

	//First, shift words
	if (n1 > 0)
	{
		//Process the least significant words
		for (m = mpi_size - n1, i = 0; i < m; i++)
			mpi_data[i] = mpi_data[i + n1];
		//Fill the rest with zeroes
		for (i = m; i < mpi_size; i++)
			mpi_data[i] = 0;
	}
	//Then shift bits
	if (n2 > 0)
	{
		//Process the least significant words
		for (m = mpi_size - n1 - 1, i = 0; i < m; i++)
			mpi_data[i] = (mpi_data[i] >> n2) | (mpi_data[i + 1] << (32 - n2));
		//The most significant word requires a special handling
		mpi_data[m] >>= n2;
	}

	//Shift operation is complete
	return RES_OK;
}

/**
 * @brief Multiple precision multiplication
 * @param[out] r Resulting integer R = A * B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiMul(const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	int i;
	int m;
	int n;
	Mpi ta;
	Mpi tb;

	//R and A are the same instance?
	if (this == a)
	{
		//Copy A to TA
		res = ta.mpiCopy(a);
		if(res != RES_OK)
			return res;
		//Use TA instead of A
		a = &ta;
	}

	//R and B are the same instance?
	if (this == b)
	{
		//Copy B to TB
		res = tb.mpiCopy(b);
		if(res != RES_OK)
			return res;
		//Use TB instead of B
		b = &tb;
	}

	//Determine the actual length of A and B
	m = a->mpiGetLength();
	n = b->mpiGetLength();

	//Adjust the size of R
	res = mpiGrow(m + n);
	if(res != RES_OK)
		return res;
	//Set the sign of R
	mpi_sign = (a->mpi_sign == b->mpi_sign) ? 1 : -1;

	//Clear the contents of the destination integer
	memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);

	//Perform multiplication
	if (m < n)
	{
		for (i = 0; i < m; i++)
			mpiMulAccCore(&mpi_data[i], b->mpi_data, n, a->mpi_data[i]);
	}
	else
	{
		for (i = 0; i < n; i++)
			mpiMulAccCore(&mpi_data[i], a->mpi_data, m, b->mpi_data[i]);
	}

	return RES_OK;
}

/**
 * @brief Multiply a multiple precision integer by an integer
 * @param[out] r Resulting integer R = A * B
 * @param[in] a First operand A
 * @param[in] b Second operand B
 * @return Error code
 **/
RES_CODE Mpi::mpiMulInt(const Mpi *a, int b)
{
	uint32_t value;
	Mpi t(b, &value);

	//Perform multiplication
	return mpiMul(a, &t);
}

/**
 * @brief Multiple precision division
 * @param[out] q The quotient Q = A / B
 * @param[out] r The remainder R = A mod B
 * @param[in] a The dividend A
 * @param[in] b The divisor B
 * @return Error code
 **/
RES_CODE Mpi::mpiDiv(Mpi *r, const Mpi *a, const Mpi *b)
{
	RES_CODE res;
	uint32_t m, n;
	Mpi c;
	Mpi d;

	//Check whether the divisor is equal to zero
	if (!b->mpiCompInt(0))
		return RES_TLS_INVALID_PARAMETER;

	res = c.mpiCopy(a);
	if(res != RES_OK)
		return res;
	res = d.mpiCopy(b);
	if(res != RES_OK)
		return res;
	res = mpiSetValue(0);
	if(res != RES_OK)
		return res;

	m = c.mpiGetBitLength();
	n = d.mpiGetBitLength();

	if (m > n)
	{
		res = d.mpiShiftLeft(m - n);
		if(res != RES_OK)
			return res;
	}

	while (n++ <= m)
	{
		res = mpiShiftLeft(1);
		if(res != RES_OK)
			return res;

		if (c.mpiComp(&d) >= 0)
		{
			res = mpiSetBitValue(0, 1);
			if(res != RES_OK)
				return res;
			res = c.mpiSub(&c, &d);
			if(res != RES_OK)
				return res;
		}

		res = d.mpiShiftRight(1);
		if(res != RES_OK)
			return res;
	}

	if (r != NULL)
		res = r->mpiCopy(&c);

	return res;
}

/**
 * @brief Divide a multiple precision integer by an integer
 * @param[out] q The quotient Q = A / B
 * @param[out] r The remainder R = A mod B
 * @param[in] a The dividend A
 * @param[in] b The divisor B
 * @return Error code
 **/
RES_CODE Mpi::mpiDivInt(Mpi *r, const Mpi *a, int b)
{
	uint32_t value;
	Mpi t(b, &value);

	//Convert the divisor to a multiple precision integer

	//Perform division
	return mpiDiv(r, a, &t);
}

/**
 * @brief Modulo operation
 * @param[out] r Resulting integer R = A mod P
 * @param[in] a The multiple precision integer to be reduced
 * @param[in] p The modulus P
 * @return Error code
 **/

RES_CODE Mpi::mpiMod(const Mpi *a, const Mpi *p)
{
	RES_CODE res;
	int sign;
	uint32_t m, n;
	Mpi c;

	//Make sure the modulus is positive
	if (p->mpiCompInt(0) <= 0)
		return RES_TLS_INVALID_PARAMETER;

	//Save the sign of A
	sign = a->mpi_sign;
	//Determine the actual length of A
	m = a->mpiGetBitLength();
	//Determine the actual length of P
	n = p->mpiGetBitLength();

	//Let R = A
	res = mpiCopy(a);
	if (res == RES_OK)
	{
		if (m >= n)
		{
			res = c.mpiCopy(p);
			if(res == RES_OK)
			{
				res = c.mpiShiftLeft(m - n);

				while (mpiCompAbs(p) >= 0)
				{
					if (mpiCompAbs(&c) >= 0)
					{
						res = mpiSubAbs(this, &c);
						if(res != RES_OK)
							return res;
					}

					res = c.mpiShiftRight(1);
					if(res != RES_OK)
						return res;
				}
			}
		}

		if (sign < 0 && res == RES_OK)
		{
			res = mpiSubAbs(p, this);
		}
	}

	return res;
}

/**
 * @brief Modular addition
 * @param[out] r Resulting integer R = A + B mod P
 * @param[in] a The first operand A
 * @param[in] b The second operand B
 * @param[in] p The modulus P
 * @return Error code
 **/
RES_CODE Mpi::mpiAddMod(const Mpi *a, const Mpi *b, const Mpi *p)
{
	RES_CODE res;

	//Perform modular addition
	res = mpiAdd(a, b);
	if(res == RES_OK)
		res = mpiMod(this, p);

	return res;
}

/**
 * @brief Modular subtraction
 * @param[out] r Resulting integer R = A - B mod P
 * @param[in] a The first operand A
 * @param[in] b The second operand B
 * @param[in] p The modulus P
 * @return Error code
 **/
RES_CODE Mpi::mpiSubMod(const Mpi *a, const Mpi *b, const Mpi *p)
{
	RES_CODE res;

	//Perform modular subtraction
	res = mpiSub(a, b);
	if(res == RES_OK)
		res = mpiMod(this, p);

	return res;
}

/**
 * @brief Modular multiplication
 * @param[out] r Resulting integer R = A * B mod P
 * @param[in] a The first operand A
 * @param[in] b The second operand B
 * @param[in] p The modulus P
 * @return Error code
 **/
RES_CODE Mpi::mpiMulMod(const Mpi *a, const Mpi *b, const Mpi *p)
{
	RES_CODE res;

	//Perform modular multiplication
	res = mpiMul(a, b);
	if(res == RES_OK)
		res = mpiMod(this, p);

	return res;
}

/**
 * @brief Modular inverse
 * @param[out] r Resulting integer R = A^-1 mod P
 * @param[in] a The multiple precision integer A
 * @param[in] p The modulus P
 * @return Error code
 **/

RES_CODE Mpi::mpiInvMod(const Mpi *a, const Mpi *p)
{
	RES_CODE res;
	Mpi b;
	Mpi c;
	Mpi q0;
	Mpi r0;
	Mpi t;
	Mpi u;
	Mpi v;

	res = b.mpiCopy(p);					// b = p
	if(res != RES_OK)
		return res;
	res = c.mpiCopy(a);					// c = a
	if(res != RES_OK)
		return res;
	res = u.mpiSetValue(0);				// u = 0
	if(res != RES_OK)
		return res;
	res = v.mpiSetValue(1);				// v = 1
	if(res != RES_OK)
		return res;

	while (c.mpiCompInt(0) > 0)			// while (c > 0)
	{
		res = q0.mpiDiv(&r0, &b, &c);	// q0,r0 = b/c
		if(res != RES_OK)
			return res;

		res = b.mpiCopy(&c);			// b = c
		if(res != RES_OK)
			return res;

		res = c.mpiCopy(&r0);			// c = r0
		if(res != RES_OK)
			return res;

		res = t.mpiCopy(&v);			// t = v
		if(res != RES_OK)
			return res;

		res = q0.mpiMul(&q0, &v);		// q0 *= v
		if(res != RES_OK)
			return res;

		res = v.mpiSub(&u, &q0);		// v = u - q0
		if(res != RES_OK)
			return res;

		res = u.mpiCopy(&t);			// u - t
		if(res != RES_OK)
			return res;
	}

	if (b.mpiCompInt(1))
		return RES_ERROR;

	if (u.mpiCompInt(0) > 0)
		res = mpiCopy(&u);
	else
		res = mpiAdd(&u, p);

	return res;
}

/**
 * @brief Modular exponentiation
 * @param[out] r Resulting integer R = A ^ E mod P
 * @param[in] a Pointer to a multiple precision integer
 * @param[in] e Exponent
 * @param[in] p Modulus
 * @return Error code
 **/
RES_CODE Mpi::mpiExpMod(const Mpi *a, const Mpi *e, const Mpi *p)
{
	RES_CODE res;
	int i, j, n;
	uint32_t d, k, u;
	Mpi b;
	Mpi c2;
	Mpi t;
	Mpi s[8];

	//Very small exponents are often selected with low Hamming weight.
	//The sliding window mechanism should be disabled in that case
	d = (e->mpiGetBitLength() <= 32) ? 1 : 4;

	//Even modulus?
	if (!p->mpiGetBitValue(0))
	{
		//Let B = A^2
		res = b.mpiMulMod(a, a, p);
		if(res != RES_OK)
			return res;

		//Let S[0] = A
		res = s[0].mpiCopy(a);
		if(res != RES_OK)
			return res;

		//Precompute S[i] = A^(2 * i + 1)
		for (i = 1; i < (1 << (d - 1)); i++)
		{
			res = s[i].mpiMulMod(&s[i - 1], &b, p);
			if(res != RES_OK)
				return res;
		}

		//Let R = 1
		res = mpiSetValue(1);
		if(res != RES_OK)
			return res;

		//The exponent is processed in a right-to-left fashion
		i = e->mpiGetBitLength() - 1;

		//Perform sliding window exponentiation
		while (i >= 0)
		{
			//The sliding window exponentiation algorithm decomposes E
			//into zero and nonzero windows
			if (!e->mpiGetBitValue(i))
			{
				//Compute R = R^2
				res = mpiMulMod(this, this, p);
				if(res != RES_OK)
					return res;
				//Next bit to be processed
				i--;
			}
			else
			{
				//Find the longest window
				n = max(i - d + 1, 0u);

				//The least significant bit of the window must be equal to 1
				while (!e->mpiGetBitValue(n))
					n++;

				//The algorithm processes more than one bit per iteration
				for (u = 0, j = i; j >= n; j--)
				{
					//Compute R = R^2
					res = mpiMulMod(this, this, p);
					if(res != RES_OK)
						return res;
					//Compute the relevant index to be used in the precomputed table
					u = (u << 1) | e->mpiGetBitValue(j);
				}

				//Perform a single multiplication per iteration
				res = mpiMulMod(this, &s[u >> 1], p);
				if(res != RES_OK)
					return res;
				//Next bit to be processed
				i = n - 1;
			}
		}
	}
	else
	{
		//Compute the smaller C = (2^32)^k such as C > P
		k = p->mpiGetLength();

		//Compute C^2 mod P
		res = c2.mpiSetValue(1);
		if(res != RES_OK)
			return res;
		res =  c2.mpiShiftLeft(2 * k * (MPI_INT_SIZE * 8));
		if(res != RES_OK)
			return res;
		res = c2.mpiMod(&c2, p);

		//Let B = A * C mod P
		if (a->mpiComp(p) >= 0)
		{
			res = b.mpiMod(a, p);
			if(res != RES_OK)
				return res;
			res = b.mpiMontgomeryMul(&b, &c2, k, p, &t);
		}
		else
		{
			res = b.mpiMontgomeryMul(a, &c2, k, p, &t);
		}
		if(res != RES_OK)
			return res;

		//Let R = B^2 * C^-1 mod P
		res = mpiMontgomeryMul(&b, &b, k, p, &t);
		if(res != RES_OK)
			return res;
		//Let S[0] = B
		res = s[0].mpiCopy(&b);
		if(res != RES_OK)
			return res;

		//Precompute S[i] = B^(2 * i + 1) * C^-1 mod P
		for (i = 1; i < (1 << (d - 1)); i++)
		{
			res = s[i].mpiMontgomeryMul(&s[i - 1], this, k, p, &t);
			if(res != RES_OK)
				return res;
		}

		//Let R = C mod P
		res = mpiCopy(&c2);
		if(res != RES_OK)
			return res;
		res = mpiMontgomeryRed(this, k, p, &t);
		if(res != RES_OK)
			return res;

		//The exponent is processed in a right-to-left fashion
		i = e->mpiGetBitLength() - 1;

		//Perform sliding window exponentiation
		while (i >= 0)
		{
			//The sliding window exponentiation algorithm decomposes E
			//into zero and nonzero windows
			if (!e->mpiGetBitValue(i))
			{
				//Compute R = R^2 * C^-1 mod P
				res = mpiMontgomeryMul(this, this, k, p, &t);
				if(res != RES_OK)
					return res;
				//Next bit to be processed
				i--;
			}
			else
			{
				//Find the longest window
				n = max(i - d + 1, 0u);

				//The least significant bit of the window must be equal to 1
				while (!e->mpiGetBitValue(n))
					n++;

				//The algorithm processes more than one bit per iteration
				for (u = 0, j = i; j >= n; j--)
				{
					//Compute R = R^2 * C^-1 mod P
					res = mpiMontgomeryMul(this, this, k, p, &t);
					if(res != RES_OK)
						return res;
					//Compute the relevant index to be used in the precomputed table
					u = (u << 1) | e->mpiGetBitValue(j);
				}

				//Compute R = R * T[u/2] * C^-1 mod P
				res = mpiMontgomeryMul(this, &s[u >> 1], k, p, &t);
				if(res != RES_OK)
					return res;
				//Next bit to be processed
				i = n - 1;
			}
		}

		//Compute R = R * C^-1 mod P
		res = mpiMontgomeryRed(this, k, p, &t);
	}

	return res;
}

/**
 * @brief Montgomery multiplication
 * @param[out] r Resulting integer R = A * B / 2^k mod P
 * @param[in] a An integer A such as 0 <= A < 2^k
 * @param[in] b An integer B such as 0 <= B < 2^k
 * @param[in] k An integer k such as P < 2^k
 * @param[in] p Modulus P
 * @param[in] t An preallocated integer T (for internal operation)
 * @return Error code
 **/
RES_CODE Mpi::mpiMontgomeryMul(const Mpi *a, const Mpi *b, uint32_t k, const Mpi *p, Mpi *t)
{
	RES_CODE res;
	uint32_t i, m, n, q;

	//Use Newton's method to compute the inverse of P[0] mod 2^32
	for (m = 2 - p->mpi_data[0], i = 0; i < 4; i++)
		m = m * (2 - m * p->mpi_data[0]);

	//Precompute -1/P[0] mod 2^32;
	m = ~m + 1;

	//We assume that B is always less than 2^k
	n = min(b->mpi_size, k);

	//Make sure T is large enough
	res = t->mpiGrow(2 * k + 1);
	if(res != RES_OK)
		return res;
	//Let T = 0
	t->mpiSetValue(0);

	//Perform Montgomery multiplication
	for (i = 0; i < k; i++)
	{
		//Check current index
		if (i < a->mpi_size)
		{
			//Compute q = ((T[i] + A[i] * B[0]) * m) mod 2^32
			q = (t->mpi_data[i] + a->mpi_data[i] * b->mpi_data[0]) * m;
			//Compute T = T + A[i] * B
			mpiMulAccCore(t->mpi_data + i, b->mpi_data, n, a->mpi_data[i]);
		}
		else
		{
			//Compute q = (T[i] * m) mod 2^32
			q = t->mpi_data[i] * m;
		}

		//Compute T = T + q * P
		mpiMulAccCore(t->mpi_data + i, p->mpi_data, k, q);
	}

	//Compute R = T / 2^(32 * k)
	res = t->mpiShiftRight(k * (MPI_INT_SIZE * 8));
	if(res != RES_OK)
		return res;

	//A final subtraction is required
	if (t->mpiComp(p) >= 0)
	{
		res = mpiSub(t, p);
	} else
		res = mpiCopy(t);

	return res;
}

/**
 * @brief Montgomery reduction
 * @param[out] r Resulting integer R = A / 2^k mod P
 * @param[in] a An integer A such as 0 <= A < 2^k
 * @param[in] k An integer k such as P < 2^k
 * @param[in] p Modulus P
 * @param[in] t An preallocated integer T (for internal operation)
 * @return Error code
 **/
RES_CODE Mpi::mpiMontgomeryRed(const Mpi *a, uint32_t k, const Mpi *p, Mpi *t)
{
	uint32_t value;
	Mpi b(1, &value);

	//Compute R = A / 2^k mod P
	return mpiMontgomeryMul(a, &b, k, p, t);
}

/**
 * @brief Read a multiple precision integer from an opaque vector
 * @param[out] a Resulting multiple precision integer
 * @param[in] data Buffer where to read the opaque vector
 * @param[in] size Total number of bytes available in the buffer
 * @param[out] length Total number of bytes that have been read
 * @return Error code
 **/
RES_CODE Mpi::tlsReadMpi(const uint8_t* data, size_t size, size_t* length)
{
	RES_CODE res;
	size_t n;

	//Buffer underrun?
	if (size < 2)
		return RES_TLS_DECODING_FAILED;

	//Decode the length field
	n = (data[0] << 8) + data[1];

	//Buffer underrun?
	if (size < (n + 2))
		return RES_TLS_DECODING_FAILED;

	//Convert the octet string to a multiple precision integer
	res = mpiReadRaw(data + 2, n);
	if (res == RES_OK)
	{
		//Return the total number of bytes that have been read
		*length = n + 2;
	}
	return res;
}

/**
 * @brief Encode a multiple precision integer to an opaque vector
 * @param[in] a Pointer to a multiple precision integer
 * @param[out] data Buffer where to store the opaque vector
 * @param[out] length Total number of bytes that have been written
 * @return Error code
 **/
RES_CODE Mpi::tlsWriteMpi(uint8_t* data, size_t* length) const
{
	RES_CODE res;
	size_t n;

	//Retrieve the actual size of the integer
	n = mpiGetByteLength();

	//The data is preceded by a 2-byte length field
	data[0] = n >> 8;
	data[1] = n;

	//Convert the integer to an octet string
	res = mpiWriteRaw(data + 2, n);
	if (res == RES_OK)
	{
		//Return the total number of bytes that have been written
		*length = n + 2;
	}
	return res;
}

RES_CODE Mpi::mpiImport(const uint8_t *data, size_t length, MpiFormat format)
{
	RES_CODE res;
	uint32_t i;

	//Check input format
	if (format == MPI_FORMAT_LITTLE_ENDIAN)
	{
		//Skip trailing zeroes
		while (length > 0 && data[length - 1] == 0)
		{
			length--;
		}

		//Ajust the size of the multiple precision integer
		res = mpiGrow((length + MPI_INT_SIZE - 1) / MPI_INT_SIZE);

		//Check status code
		if (res == RES_OK)
		{
			//Clear the contents of the multiple precision integer
			memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);
			//Set sign
			mpi_sign = 1;

			//Import data
			for (i = 0; i < length; i++, data++)
			{
				mpi_data[i / MPI_INT_SIZE] |= *data << ((i % MPI_INT_SIZE) * 8);
			}
		}
	}
	else if (format == MPI_FORMAT_BIG_ENDIAN)
	{
		//Skip leading zeroes
		while (length > 1 && *data == 0)
		{
			data++;
			length--;
		}

		//Ajust the size of the multiple precision integer
		res = mpiGrow((length + MPI_INT_SIZE - 1) / MPI_INT_SIZE);

		//Check status code
		if (res == RES_OK)
		{
			//Clear the contents of the multiple precision integer
			memset(mpi_data, 0, mpi_size * MPI_INT_SIZE);
			//Set sign
			mpi_sign = 1;

			//Start from the least significant byte
			data += length - 1;

			//Import data
			for (i = 0; i < length; i++, data--)
			{
				mpi_data[i / MPI_INT_SIZE] |= *data << ((i % MPI_INT_SIZE) * 8);
			}
		}
	}
	else
	{
		//Report an error
		res = RES_TLS_INVALID_PARAMETER;
	}

	//Return status code
	return res;
}

RES_CODE Mpi::asn1WriteMpi(bool reverse, uint8_t* data, size_t* written) const
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Retrieve the length of the multiple precision integer
	n = mpiGetBitLength();

	//An integer value is always encoded in the smallest possible number of
	//octets
	n = (n / 8) + 1;

	//Valid output stream?
	if (data != NULL)
	{
		//Use reverse encoding?
		if (reverse)
			data -= n;

		//The value of the multiple precision integer is encoded MSB first
		res = mpiWriteRaw(data, n);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//The integer is encapsulated within an ASN.1 structure
	tag.constructed = false;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_INTEGER;
	tag.length = n;
	tag.value = data;

	//Compute the length of the corresponding ASN.1 structure
	res = tag.asn1WriteTag(false, data, NULL);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Number of bytes written to the output stream
	if (written != NULL)
		*written = tag.totalLength;

	//Successful processing
	return RES_OK;
}

RES_CODE Mpi::mpiCheckProbablePrime() const
{
	return RES_TLS_NOT_IMPLEMENTED;
}

void mpiDump(const char* prepend, const Mpi* a)
{
	uint32_t i;

	//Process each word
	for (i = 0; i < a->mpi_size; i++)
	{
		//Beginning of a new line?
		if (i == 0 || ((a->mpi_size - i - 1) % 8) == 7)
			TRACE1(prepend);

		//Display current data
		TRACE("%08X ", a->mpi_data[a->mpi_size - 1 - i]);

		//End of current line?
		if (((a->mpi_size - i - 1) % 8) == 0 || i == (unsigned)(a->mpi_size - 1))
			TRACE1("\r\n");
	}
}

#endif //MPI_SUPPORT
