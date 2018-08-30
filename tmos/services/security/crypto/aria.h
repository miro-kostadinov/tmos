/*
 * aria.h
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_ARIA_H_
#define SERVICES_SECURITY_CRYPTO_ARIA_H_

#include <tmos.h>
#include <crypto.h>


#define ARIA_BLOCK_SIZE 16

struct aria_cipher_algo_t: cipher_algo_t
{
	uint32_t nr;
	uint32_t k[16];
	uint32_t ek[68];
	uint32_t dk[68];

	aria_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info), nr(0)
	{
	};

	RES_CODE init_key(const uint8_t* key, size_t key_size) override;
	void encrypt(const void* input, void* output, size_t blocks) override;
	void decrypt(const void* input, void* output, size_t blocks) override;
};

cipher_algo_t* new_cipher_aria();





#endif /* SERVICES_SECURITY_CRYPTO_ARIA_H_ */
