/*
 * sha512_224.h
 *
 *  Created on: 19 Sept 2023
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_CRYPTO_SHA512_224_H_
#define SERVICES_SECURITY_CRYPTO_SHA512_224_H_

#include <tmos.h>
#include <sha512.h>


#define SHA512_224_BLOCK_SIZE 128
#define SHA512_224_DIGEST_SIZE 28

extern const hash_info_t SHA512_224_HASH_ALGO[1];

struct sha512_224_algo_t: public sha512_algo_t
{
	sha512_224_algo_t():	sha512_algo_t(SHA512_224_HASH_ALGO)
	{
		Reset();
	};

	sha512_224_algo_t(const hash_info_t* info): sha512_algo_t(info)
	{
		Reset();
	};

	void Reset() override;
};

hash_algo_t* new_sha512_224_hash();






#endif /* SERVICES_SECURITY_CRYPTO_SHA512_224_H_ */
