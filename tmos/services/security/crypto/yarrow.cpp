/*
 * yarrow.cpp
 *
 *  Created on: Sep 28, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <yarrow.h>

#if YARROW_SUPPORT

//Common interface for PRNG algorithms
const algo_info_t yarrow_prng_info =
{
   "Yarrow",
   0
};

RES_CODE prng_yarrow_algo_t::prng_init()
{
	memclr(key, sizeof(key));
	memclr(counter, sizeof(counter));
	memclr(currentPool, sizeof(currentPool));
	memclr(fastPoolEntropy, sizeof(fastPoolEntropy));
	memclr(slowPoolEntropy, sizeof(slowPoolEntropy));
	return RES_OK;
}

RES_CODE prng_yarrow_algo_t::prng_seed(const void* input, size_t length)
{
	//Check parameters
	if (length < sizeof(key))
		return RES_TLS_INVALID_PARAMETER;

	//Add entropy to the fast pool
	fastPool.Input(input, length);

	//Reseed from the fast pool
	yarrowFastReseed();

	//Successful processing
	return RES_OK;
}

RES_CODE prng_yarrow_algo_t::prng_read(void* output, size_t length)
{
	size_t n;
	uint8_t buffer[AES_BLOCK_SIZE];

	//Make sure that the PRNG has been properly seeded
	if (!ready)
		return RES_TLS_PRNG_NOT_READY;

	//Generate random data in a block-by-block fashion
	while (length > 0)
	{
		//Number of bytes to process at a time
		n = min(length, AES_BLOCK_SIZE);

		//Generate a random block
		yarrowGenerateBlock(buffer);
		//Copy data to the output buffer
		memcpy(output, buffer, n);

		//We keep track of how many blocks we have output
		blockCount++;

		//Next block
		output = (char*)output + n;
		length -= n;
	}

	//Apply generator gate?
	if (blockCount >= YARROW_PG)
	{
		//Generate some random bytes
		yarrowGenerateBlock(key);
		//Use them as the new key
		cipherContext.init_key(key, sizeof(key));

		//Reset block counter
		blockCount = 0;
	}

	//Successful processing
	return RES_OK;
}

void prng_yarrow_algo_t::yarrowFastReseed()
{
	size_t i;

	//Reseeding from the fast pool use the current key and the hash of all
	//inputs to the fast pool since the last reseed, to generate a new key
	fastPool.Input(key, sizeof(key));
	fastPool.Result(key);

	//Set the new key
	cipherContext.init_key(key, sizeof(key));

	//Define the new value of the counter
	memclr(counter, sizeof(counter));
	cipherContext.encrypt(counter, counter, 1);

	//Reset the hash context
	fastPool.Reset();

	//The entropy estimates for the fast pool are all reset to zero
	for (i = 0; i < YARROW_N; i++)
		fastPoolEntropy[i] = 0;

	//The PRNG is ready to generate random data
	ready = true;
}

void prng_yarrow_algo_t::yarrowGenerateBlock(uint8_t *output)
{
	//Encrypt counter block
	cipherContext.encrypt(counter, output, 1);

	//Increment counter value
	for (int i = AES_BLOCK_SIZE - 1; i >= 0; i--)
	{
		//Increment the current byte and propagate the carry if necessary
		if (++(counter[i]) != 0)
			break;
	}
}

#endif
