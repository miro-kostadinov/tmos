/*
 * sha512.h
 *
 *  Created on: Jun 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_SHA512_H_
#define SERVICES_SECURITY_CRYPTO_SHA512_H_

#include <tmos.h>
#include <crypto.h>


#define SHA512_BLOCK_SIZE 128
#define SHA512_DIGEST_SIZE 64

extern const hash_info_t SHA512_HASH_ALGO[1];

struct sha512_algo_t: public hash_algo_t
{
	sha512_algo_t():	hash_algo_t(SHA512_HASH_ALGO)
	{
		Reset();
	};

	sha512_algo_t(const hash_info_t* info): hash_algo_t(info)
	{
		Reset();
	};

	void Reset() override;
	void Result(void* digest) override;
	void process_block(const uint32_t* block) override;

};

hash_algo_t* new_sha512_hash();





#endif /* SERVICES_SECURITY_CRYPTO_SHA512_H_ */
