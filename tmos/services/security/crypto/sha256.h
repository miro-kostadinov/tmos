/*
 * sha256.h
 *
 *  Created on: Jul 12, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_SHA256_H_
#define SERVICES_SECURITY_CRYPTO_SHA256_H_

#include <tmos.h>
#include <crypto.h>


#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

extern const hash_info_t SHA256_HASH_ALGO[1];

struct sha256_algo_t: public hash_algo_t
{

	sha256_algo_t():	hash_algo_t(SHA256_HASH_ALGO)
	{
		sha256_algo_t::Reset();
	};

	sha256_algo_t(const hash_info_t* info): hash_algo_t(info)
	{
		sha256_algo_t::Reset();
	};

	void Reset() override;
	void Result(void* digest) override;
	void process_block(const uint32_t* block) override;
};

struct static_sha256_t: public sha256_algo_t
{
	uint8_t		static_buf[SHA256_DIGEST_SIZE + SHA256_BLOCK_SIZE];

};

hash_algo_t* new_sha256_hash();


#endif /* SERVICES_SECURITY_CRYPTO_SHA256_H_ */
