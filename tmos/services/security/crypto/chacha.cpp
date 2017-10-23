/*
 * chacha.cpp
 *
 *  Created on: Jun 16, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <crypto.h>
#include <tls.h>
#include <chacha.h>

#if CHACHA_SUPPORT

#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))

#define CHACHA_QUARTER_ROUND(a, b, c, d) \
{ \
   a += b; d ^= a; d = ROL32(d, 16); \
   c += d; b ^= c; b = ROL32(b, 12); \
   a += b; d ^= a; d = ROL32(d, 8); \
   c += d; b ^= c; b = ROL32(b, 7); \
}

//Load unaligned 32-bit integer (little-endian encoding)
#define LOAD32LE(p) ( \
   ((uint32_t)(((uint8_t *)(p))[0]) << 0) | \
   ((uint32_t)(((uint8_t *)(p))[1]) << 8) | \
   ((uint32_t)(((uint8_t *)(p))[2]) << 16) | \
   ((uint32_t)(((uint8_t *)(p))[3]) << 24))

RES_CODE chacha_t::chacha_init(uint32_t nr, const uint8_t* key,
      size_t keyLength, const uint8_t* nonce, size_t nonceLen)
{

	//The number of rounds must be 8, 12 or 20
	if (nr != 8 && nr != 12 && nr != 20)
		return RES_TLS_INVALID_PARAMETER;

	//Save the number of rounds to be applied
	cha_nr = nr;

	//Check the length of the key
	if (keyLength == 16)
	{
		//The first four input words are constants
		state[0] = 0x61707865;
		state[1] = 0x3120646E;
		state[2] = 0x79622D36;
		state[3] = 0x6B206574;

		//Input words 4 through 7 are taken from the 128-bit key, by reading
		//the bytes in little-endian order, in 4-byte chunks
		state[4] = LOAD32LE(key);
		state[5] = LOAD32LE(key + 4);
		state[6] = LOAD32LE(key + 8);
		state[7] = LOAD32LE(key + 12);

		//Input words 8 through 11 are taken from the 128-bit key, again by
		//reading the bytes in little-endian order, in 4-byte chunks
		state[8] = LOAD32LE(key);
		state[9] = LOAD32LE(key + 4);
		state[10] = LOAD32LE(key + 8);
		state[11] = LOAD32LE(key + 12);
	}
	else if (keyLength == 32)
	{
		//The first four input words are constants
		state[0] = 0x61707865;
		state[1] = 0x3320646E;
		state[2] = 0x79622D32;
		state[3] = 0x6B206574;

		//Input words 4 through 11 are taken from the 256-bit key, by reading
		//the bytes in little-endian order, in 4-byte chunks
		state[4] = LOAD32LE(key);
		state[5] = LOAD32LE(key + 4);
		state[6] = LOAD32LE(key + 8);
		state[7] = LOAD32LE(key + 12);
		state[8] = LOAD32LE(key + 16);
		state[9] = LOAD32LE(key + 20);
		state[10] = LOAD32LE(key + 24);
		state[11] = LOAD32LE(key + 28);
	}
	else
	{
		//Invalid key length
		return RES_TLS_INVALID_PARAMETER;
	}

	//Check the length of the nonce
	if (nonceLen == 8)
	{
		//Input words 12 and 13 are a block counter, with word 12
		//overflowing into word 13
		state[12] = 0;
		state[13] = 0;

		//Input words 14 and 15 are taken from an 64-bit nonce, by reading
		//the bytes in little-endian order, in 4-byte chunks
		state[14] = LOAD32LE(nonce);
		state[15] = LOAD32LE(nonce + 4);
	}
	else if (nonceLen == 12)
	{
		//Input word 12 is a block counter
		state[12] = 0;

		//Input words 13 to 15 are taken from an 96-bit nonce, by reading
		//the bytes in little-endian order, in 4-byte chunks
		state[13] = LOAD32LE(nonce);
		state[14] = LOAD32LE(nonce + 4);
		state[15] = LOAD32LE(nonce + 8);
	}
	else
	{
		//Invalid nonce length
		return RES_TLS_INVALID_PARAMETER;
	}

	//The keystream block is empty
	pos = 0;

	//No error to report
	return RES_OK;
}

void chacha_t::chacha_cipher(const uint8_t *input, uint8_t *output, size_t len)
{
	uint32_t i;
	uint32_t n;
	uint8_t *k;

	//Encryption loop
	while (len > 0)
	{
		//Check whether a new keystream block must be generated
		if (pos == 0 || pos >= 64)
		{
			//ChaCha successively calls the ChaCha block function, with the same key
			//and nonce, and with successively increasing block counter parameters
			chacha_process_block();

			//Increment block counter
			if (++state[12] == 0)
				state[13]++;

			//Rewind to the beginning of the keystream block
			pos = 0;
		}

		//Compute the number of bytes to encrypt/decrypt at a time
		n = min(len, 64 - pos);

		//Valid output pointer?
		if (output != NULL)
		{
			//Point to the keystream
			k = (uint8_t *)block + pos;

			//Valid input pointer?
			if (input != NULL)
			{
				//XOR the input data with the keystream
				for (i = 0; i < n; i++)
					output[i] = input[i] ^ k[i];

				//Advance input pointer
				input += n;
			}
			else
			{
				//Output the keystream
				for (i = 0; i < n; i++)
					output[i] = k[i];
			}

			//Advance output pointer
			output += n;
		}

		//Current position in the keystream block
		pos += n;
		//Remaining bytes to process
		len -= n;
	}
}

void chacha_t::chacha_process_block()
{
	uint32_t i;

	//Copy the state to the working state
	for (i = 0; i < 16; i++)
		block[i] = state[i];

	//ChaCha runs 8, 12 or 20 rounds, alternating between column rounds
	//and diagonal rounds
	for (i = 0; i < cha_nr; i += 2)
	{
		//The column rounds apply the quarter-round function to the four
		//columns, from left to right
		CHACHA_QUARTER_ROUND(block[0], block[4], block[8], block[12]);
		CHACHA_QUARTER_ROUND(block[1], block[5], block[9], block[13]);
		CHACHA_QUARTER_ROUND(block[2], block[6], block[10], block[14]);
		CHACHA_QUARTER_ROUND(block[3], block[7], block[11], block[15]);

		//The diagonal rounds apply the quarter-round function to the top-left,
		//bottom-right diagonal, followed by the pattern shifted one place to
		//the right, for three more quarter-rounds
		CHACHA_QUARTER_ROUND(block[0], block[5], block[10], block[15]);
		CHACHA_QUARTER_ROUND(block[1], block[6], block[11], block[12]);
		CHACHA_QUARTER_ROUND(block[2], block[7], block[8], block[13]);
		CHACHA_QUARTER_ROUND(block[3], block[4], block[9], block[14]);
	}

	//Add the original input words to the output words
	for (i = 0; i < 16; i++)
		block[i] += state[i];

}


#endif // CHACHA_SUPPORT
