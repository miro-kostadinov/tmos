/*
 * des3.cpp
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <des3.h>

#if DES3_SUPPORT

const cipher_info_t des3_cipher_info =
{
   "3DES",
   DES3_BLOCK_SIZE,
   CIPHER_TYPE_BLOCK
};


cipher_algo_t* new_cipher_des3()
{
	cipher_algo_t* algo;

	algo = new des_cipher_algo_t(&des3_cipher_info);
	return algo;
}

RES_CODE des3_cipher_algo_t::init_key(const uint8_t* key, size_t key_size)
{
	switch(key_size)
	{
	case 8:
	      //This option provides backward compatibility with DES, because the
	      //first and second DES operations cancel out
	      des_init_key(ks1, key);
	      des_init_key(ks2, key);
	      des_init_key(ks3, key);
		break;

	case 16:
	      //If the key length is 128 bits including parity, the first 8 bytes of the
	      //encoding represent the key used for the two outer DES operations, and
	      //the second 8 bytes represent the key used for the inner DES operation
	      des_init_key(ks1, key);
	      des_init_key(ks2, key + 8);
	      des_init_key(ks3, key);
		break;

	case 24:
	      //If the key length is 192 bits including parity, then three independent DES
	      //keys are represented, in the order in which they are used for encryption
	      des_init_key(ks1, key);
	      des_init_key(ks2, key + 8);
	      des_init_key(ks3, key + 16);
		break;

	default:
		return RES_TLS_INVALID_KEY_LENGTH;
	}

	return RES_OK;
}

void des3_cipher_algo_t::encrypt(const void* input, void* output, size_t blocks)
{
	while(blocks--)
	{
		//The first pass is a DES encryption
		des_crypt_block(ks1, 2, (const uint32_t*)input, (uint32_t*)output);
		//The second pass is a DES decryption of the first ciphertext result
		des_crypt_block(ks2, -2, (uint32_t*)output, (uint32_t*)output);
		//The third pass is a DES encryption of the second pass result
		des_crypt_block(ks3, 2, (uint32_t*)output, (uint32_t*)output);

		input = (const uint8_t*)input + DES3_BLOCK_SIZE;
		output = (uint8_t*)output + DES3_BLOCK_SIZE;
	}
}
void des3_cipher_algo_t::decrypt(const void* input, void* output, size_t blocks)
{
	while(blocks--)
	{
		//The first pass is a DES decryption
		des_crypt_block(ks3, -2, (const uint32_t*)input, (uint32_t*)output);
		//The second pass is a DES encryption of the first pass result
		des_crypt_block(ks2, 2, (uint32_t*)output, (uint32_t*)output);
		//The third pass is a DES decryption of the second ciphertext result
		des_crypt_block(ks1, -2, (uint32_t*)output, (uint32_t*)output);

		input = (const uint8_t*)input + DES3_BLOCK_SIZE;
		output = (uint8_t*)output + DES3_BLOCK_SIZE;
	}
}

#endif // DES3_SUPPORT
