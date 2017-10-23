/*
 * idea.h
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_IDEA_H_
#define SERVICES_SECURITY_CRYPTO_IDEA_H_

#include <tmos.h>
#include <crypto.h>

#define IDEA_BLOCK_SIZE 8

struct idea_cipher_algo_t: cipher_algo_t
{
	uint16_t ek[52];
	uint16_t dk[52];

	idea_cipher_algo_t(const cipher_info_t* info) :
			cipher_algo_t(info)
	{
	};

	virtual RES_CODE init_key(const uint8_t* key, size_t key_size);
	virtual void encrypt(const void* input, void* output, size_t blocks);
	virtual void decrypt(const void* input, void* output, size_t blocks);
};

cipher_algo_t* new_cipher_idea();



#endif /* SERVICES_SECURITY_CRYPTO_IDEA_H_ */
