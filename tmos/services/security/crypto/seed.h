/*
 * seed.h
 *
 *  Created on: Aug 30, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_SEED_H_
#define SERVICES_SECURITY_CRYPTO_SEED_H_

#include <tmos.h>
#include <crypto.h>


#define SEED_BLOCK_SIZE 16

struct seed_cipher_algo_t: cipher_algo_t
{
	uint32_t ks[32];

	seed_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info)
	{
	};

	RES_CODE init_key(const uint8_t* key, size_t key_size) override;
	void encrypt(const void* input, void* output, size_t blocks) override;
	void decrypt(const void* input, void* output, size_t blocks) override;
};

cipher_algo_t* new_cipher_seed();


#endif /* SERVICES_SECURITY_CRYPTO_SEED_H_ */
