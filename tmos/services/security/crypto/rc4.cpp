/*
 * rc4.cpp
 *
 *  Created on: Aug 28, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <rc4.h>


#if RC4_SUPPORT

const cipher_info_t rc4_cipher_info =
{
   "RC4",
   0,
   CIPHER_TYPE_STREAM
};

cipher_algo_t* new_cipher_rc4()
{
	cipher_algo_t* algo;

	algo = new rc4_cipher_algo_t(&rc4_cipher_info);
	return algo;
}

RES_CODE rc4_cipher_algo_t::init_key(const uint8_t* key, size_t key_size)
{
	uint32_t i;
	uint32_t j;
	uint8_t temp;

	//Clear context
	ii = 0;
	jj = 0;

	//Initialize the S array with identity permutation
	for (i = 0; i < 256; i++)
		s[i] = i;

	//S is then processed for 256 iterations
	for (i = 0, j = 0; i < 256; i++)
	{
		//Randomize the permutations using the supplied key
		j = (j + s[i] + key[i % key_size]) % 256;

		//Swap the values of S[i] and S[j]
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;
	}

	return RES_OK;
}

void rc4_cipher_algo_t::encrypt(const void* input, void* output, size_t blocks)
{
	uint8_t temp;

	//Restore context
	uint32_t i = ii;
	uint32_t j = jj;

	//Encryption loop
	while (blocks > 0)
	{
		//Adjust indices
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;

		//Swap the values of S[i] and S[j]
		temp = s[i];
		s[i] = s[j];
		s[j] = temp;

		//XOR the input data with the RC4 stream
		*(uint8_t*)output = *(const uint8_t*)input ^ s[(s[i] + s[j]) % 256];

		//Increment data pointers
		input = 1+ (uint8_t*)input;
		output = 1 + (uint8_t*)output;

		//Remaining bytes to process
		blocks--;
	}

	//Save context
	ii = i;
	jj = j;
}

void rc4_cipher_algo_t::decrypt(const void* input, void* output, size_t blocks)
{
	encrypt(input, output, blocks);
}



#endif // RC4_SUPPORT
