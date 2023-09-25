/*
 * sha1.h
 *
 *  Created on: Jun 12, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_SHA1_H_
#define SERVICES_SECURITY_CRYPTO_SHA1_H_

#include <tmos.h>
#include <crypto.h>

#define SHA1_BLOCK_SIZE 64
#define SHA1_DIGEST_SIZE 20

extern const uint8_t SHA1_OID[5];

extern const hash_info_t SHA1_HASH_ALGO[1];

struct sha1_algo_t: public hash_algo_t
{
	using hash_algo_t::Result;

	sha1_algo_t():	hash_algo_t(SHA1_HASH_ALGO)
	{
		sha1_algo_t::Reset();
	};


	void Reset() override;
	void Result(void* digest) override;
	void process_block(const uint32_t* block) override;

};

struct static_sha1_t: public sha1_algo_t
{
	uint8_t		static_buf[SHA1_DIGEST_SIZE + SHA1_BLOCK_SIZE];
};

hash_algo_t* new_sha1_hash();

#endif /* SERVICES_SECURITY_CRYPTO_SHA1_H_ */
