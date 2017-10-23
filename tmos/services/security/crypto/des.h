/*
 * des.h
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_DES_H_
#define SERVICES_SECURITY_CRYPTO_DES_H_

#include <tmos.h>
#include <crypto.h>


#define DES_BLOCK_SIZE 8

struct des_cipher_algo_t: cipher_algo_t
{
	uint32_t ks[32];

	des_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info)
	{
	};

	virtual RES_CODE init_key(const uint8_t* key, size_t key_size);
	virtual void encrypt(const void* input, void* output, size_t blocks);
	virtual void decrypt(const void* input, void* output, size_t blocks);
};

cipher_algo_t* new_cipher_des();

void des_init_key(uint32_t* ks, const uint8_t* key);
void des_crypt_block(uint32_t* ks, int dir, const uint32_t* input, uint32_t* output);


#endif /* SERVICES_SECURITY_CRYPTO_DES_H_ */
