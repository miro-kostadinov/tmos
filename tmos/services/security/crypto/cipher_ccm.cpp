/*
 * cipher_ccm.cpp
 *
 *  Created on: Jun 15, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <crypto.h>
#include <tls.h>


#if CCM_SUPPORT

static void ccmXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n)
{
   size_t i;

   //Perform XOR operation
   for(i = 0; i < n; i++)
      x[i] = a[i] ^ b[i];
}

static void ccmIncCounter(uint8_t *x, size_t n)
{
   size_t i;

   //The function increments the right-most bytes of the block. The remaining
   //left-most bytes remain unchanged
   for(i = 0; i < n; i++)
   {
      //Increment the current byte and propagate the carry if necessary
      if(++(x[15 - i]) != 0)
         break;
   }
}

RES_CODE cipher_algo_t::ccm_encrypt(const uint8_t *n, size_t nLen, const uint8_t *a,
		size_t aLen, uint8_t *data, size_t len, uint8_t *t, size_t tLen)
{
	size_t m;
	size_t q;
	size_t qLen;
	uint8_t b[16];
	uint8_t y[16];
	uint8_t s[16];

	//CCM supports only symmetric block ciphers whose block size is 128 bits
	if (cipher_info->cipher_type != CIPHER_TYPE_BLOCK ||
			cipher_info->algo_info.block_size != 16)
		return RES_TLS_INVALID_PARAMETER;

	//Check the length of the nonce
	if (nLen < 7 || nLen > 13)
		return RES_TLS_INVALID_PARAMETER;
	//Check the length of the MAC
	if (tLen < 4 || tLen > 16 || (tLen % 2) != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Q is the bit string representation of the octet length of P
	q = len;
	//Compute the octet length of Q
	qLen = 15 - nLen;

	//Format the leading octet of the first block
	b[0] = (aLen > 0) ? 0x40 : 0x00;
	//Encode the octet length of T
	b[0] |= ((tLen - 2) / 2) << 3;
	//Encode the octet length of Q
	b[0] |= qLen - 1;

	//Copy the nonce
	memcpy(b + 1, n, nLen);

	//Encode the length field Q
	for (m = 0; m < qLen; m++, q >>= 8)
		b[15 - m] = q & 0xFF;

	//Invalid length?
	if (q != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Set Y(0) = CIPH(B(0))
	encrypt(b, y, 1);

	//Any additional data?
	if (aLen > 0)
	{
		//Format the associated data
		memset(b, 0, 16);

		//Check the length of the associated data string
		if (aLen < 0xFF00)
		{
			//The length is encoded as 2 octets
			b[0] = aLen >> 8;
			b[1] = aLen;
			//Number of bytes to copy
			m = min(aLen, 16u - 2u);
			//Concatenate the associated data A
			memcpy(b + 2, a, m);
		}
		else
		{
			//The length is encoded as 6 octets
			b[0] = 0xFF;
			b[1] = 0xFE;
			b[2] = aLen >> 24;
			b[3] = aLen >> 16;
			b[4] = aLen >> 8;
			b[5] = aLen;
			//Number of bytes to copy
			m = min(aLen, 16u - 6u);
			//Concatenate the associated data A
			memcpy(b + 6, a, m);
		}

		//XOR B(1) with Y(0)
		ccmXorBlock(y, b, y, 16);
		//Compute Y(1) = CIPH(B(1) ^ Y(0))
		encrypt(y, y, 1);

		//Number of remaining data bytes
		aLen -= m;
		a += m;

		//Process the remaining data bytes
		while (aLen > 0)
		{
			//Associated data are processed in a block-by-block fashion
			m = min(aLen, 16u);

			//XOR B(i) with Y(i-1)
			ccmXorBlock(y, a, y, m);
			//Compute Y(i) = CIPH(B(i) ^ Y(i-1))
			encrypt(y, y, 1);

			//Next block
			aLen -= m;
			a += m;
		}
	}

	//Format CTR(0)
	b[0] = (uint8_t) (qLen - 1);
	//Copy the nonce
	memcpy(b + 1, n, nLen);
	//Initialize counter value
	memset(b + 1 + nLen, 0, qLen);

	//Compute S(0) = CIPH(CTR(0))
	encrypt(b, s, 1);
	//Save MSB(S(0))
	memcpy(t, s, tLen);

	//Encrypt plaintext
	while (len > 0)
	{
		//The encryption operates in a block-by-block fashion
		m = min(len, 16u);

		//XOR B(i) with Y(i-1)
		ccmXorBlock(y, data, y, m);
		//Compute Y(i) = CIPH(B(i) ^ Y(i-1))
		encrypt(y, y, 1);

		//Increment counter
		ccmIncCounter(b, qLen);
		//Compute S(i) = CIPH(CTR(i))
		encrypt(b, s, 1);
		//Compute C(i) = B(i) XOR S(i)
		ccmXorBlock(data, data, s, m);

		//Next block
		len -= m;
		data += m;
	}

	//Compute MAC
	ccmXorBlock(t, t, y, tLen);

	return RES_OK;
}

RES_CODE cipher_algo_t::ccm_decrypt(const uint8_t *n, size_t nLen, const uint8_t *a,
		size_t aLen, const uint8_t* c, uint8_t* p, size_t len, uint8_t *t, size_t tLen)
{
	size_t m;
	size_t q;
	size_t qLen;
	uint8_t b[16];
	uint8_t y[16];
	uint8_t r[16];
	uint8_t s[16];

	//CCM supports only symmetric block ciphers whose block size is 128 bits
	if (cipher_info->cipher_type != CIPHER_TYPE_BLOCK ||
			cipher_info->algo_info.block_size != 16)
		return RES_TLS_INVALID_PARAMETER;

	//Check the length of the nonce
	if (nLen < 7 || nLen > 13)
		return RES_TLS_INVALID_PARAMETER;
	//Check the length of the MAC
	if (tLen < 4 || tLen > 16 || (tLen % 2) != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Q is the bit string representation of the octet length of C
	q = len;
	//Compute the octet length of Q
	qLen = 15 - nLen;

	//Format the leading octet of the first block
	b[0] = (aLen > 0) ? 0x40 : 0x00;
	//Encode the octet length of T
	b[0] |= ((tLen - 2) / 2) << 3;
	//Encode the octet length of Q
	b[0] |= qLen - 1;

	//Copy the nonce
	memcpy(b + 1, n, nLen);

	//Encode the length field Q
	for (m = 0; m < qLen; m++, q >>= 8)
		b[15 - m] = q & 0xFF;

	//Invalid length?
	if (q != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Set Y(0) = CIPH(B(0))
	encrypt(b, y, 1);

	//Any additional data?
	if (aLen > 0)
	{
		//Format the associated data
		memset(b, 0, 16);

		//Check the length of the associated data string
		if (aLen < 0xFF00)
		{
			//The length is encoded as 2 octets
			b[0] = aLen >> 8;
			b[1] = aLen;
			//Number of bytes to copy
			m = min(aLen, 16u - 2u);
			//Concatenate the associated data A
			memcpy(b + 2, a, m);
		}
		else
		{
			//The length is encoded as 6 octets
			b[0] = 0xFF;
			b[1] = 0xFE;
			b[2] = aLen >> 24;
			b[3] = aLen >> 16;
			b[4] = aLen >> 8;
			b[5] = aLen;
			//Number of bytes to copy
			m = min(aLen, 16u - 6u);
			//Concatenate the associated data A
			memcpy(b + 6, a, m);
		}

		//XOR B(1) with Y(0)
		ccmXorBlock(y, b, y, 16);
		//Compute Y(1) = CIPH(B(1) ^ Y(0))
		encrypt(y, y, 1);

		//Number of remaining data bytes
		aLen -= m;
		a += m;

		//Process the remaining data bytes
		while (aLen > 0)
		{
			//Associated data are processed in a block-by-block fashion
			m = min(aLen, 16u);

			//XOR B(i) with Y(i-1)
			ccmXorBlock(y, a, y, m);
			//Compute Y(i) = CIPH(B(i) ^ Y(i-1))
			encrypt(y, y, 1);

			//Next block
			aLen -= m;
			a += m;
		}
	}

	//Format CTR(0)
	b[0] = (uint8_t) (qLen - 1);
	//Copy the nonce
	memcpy(b + 1, n, nLen);
	//Initialize counter value
	memset(b + 1 + nLen, 0, qLen);

	//Compute S(0) = CIPH(CTR(0))
	encrypt(b, s, 1);
	//Save MSB(S(0))
	memcpy(r, s, tLen);

	//Decrypt ciphertext
	while (len > 0)
	{
		//The decryption operates in a block-by-block fashion
		m = min(len, 16u);

		//Increment counter
		ccmIncCounter(b, qLen);
		//Compute S(i) = CIPH(CTR(i))
		encrypt(b, s, 1);
		//Compute B(i) = C(i) XOR S(i)
		ccmXorBlock(p, c, s, m);

		//XOR B(i) with Y(i-1)
		ccmXorBlock(y, p, y, m);
		//Compute Y(i) = CIPH(B(i) ^ Y(i-1))
		encrypt(y, y, 1);

		//Next block
		len -= m;
		p += m;
		c += m;
	}

	//Compute MAC
	ccmXorBlock(r, r, y, tLen);

	//The calculated tag is bitwise compared to the received tag. The
	//message is authenticated if and only if the tags match
	if (memcmp(r, t, tLen))
		return RES_TLS_FAIL;
	return RES_OK;
}

#endif

