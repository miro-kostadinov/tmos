/*
 * aes.h
 *
 *  Created on: Aug 29, 2017
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_CRYPTO_AES_H_
#define SERVICES_SECURITY_CRYPTO_AES_H_

#include <tmos.h>
#include <crypto.h>
#include <aes_tmos.h>


#define AES_BLOCK_SIZE 16u


struct aes_cipher_algo_t: cipher_algo_t
{
	aes_context en_ctx;
	aes_context de_ctx;

	aes_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info)
	{
	};

	RES_CODE init_key(const uint8_t* key, size_t key_size) override;
	void encrypt(const void* input, void* output, size_t blocks) override;
	void decrypt(const void* input, void* output, size_t blocks) override;
};

cipher_algo_t* new_cipher_aes();

extern const cipher_info_t aes_cipher_info;





#endif /* SERVICES_SECURITY_CRYPTO_AES_H_ */
