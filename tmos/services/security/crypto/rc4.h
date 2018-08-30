/*
 * rc4.h
 *
 *  Created on: Aug 25, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_RC4_H_
#define SERVICES_SECURITY_CRYPTO_RC4_H_

#include <tmos.h>
#include <crypto.h>


struct rc4_cipher_algo_t: cipher_algo_t
{
	uint32_t ii;
	uint32_t jj;
	uint8_t s[256];

	rc4_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info), ii(0), jj(0)
	{
	};

	RES_CODE init_key(const uint8_t* key, size_t key_size) override;
	void encrypt(const void* input, void* output, size_t blocks) override;
	void decrypt(const void* input, void* output, size_t blocks) override;
};

cipher_algo_t* new_cipher_rc4();


#endif /* SERVICES_SECURITY_CRYPTO_RC4_H_ */
