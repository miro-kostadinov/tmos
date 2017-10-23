/*
 * camellia.h
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_CAMELLIA_H_
#define SERVICES_SECURITY_CRYPTO_CAMELLIA_H_

#include <tmos.h>
#include <crypto.h>


#define CAMELLIA_BLOCK_SIZE 16

struct camellia_subkey_t
{
   uint8_t index;
   uint8_t key;
   uint8_t shift;
   uint8_t position;
};

struct camellia_cipher_algo_t: cipher_algo_t
{
	uint32_t nr;
	uint32_t kk[16];
	uint32_t ks[68];

	camellia_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info), nr(0)
	{
	};

	virtual RES_CODE init_key(const uint8_t* key, size_t key_size);
	virtual void encrypt(const void* input, void* output, size_t blocks);
	virtual void decrypt(const void* input, void* output, size_t blocks);
};

cipher_algo_t* new_cipher_camellia();




#endif /* SERVICES_SECURITY_CRYPTO_CAMELLIA_H_ */
