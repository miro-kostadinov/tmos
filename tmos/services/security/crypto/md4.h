/*
 * md4.h
 *
 *  Created on: 19 Sept 2023
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_CRYPTO_MD4_H_
#define SERVICES_SECURITY_CRYPTO_MD4_H_

#include <tmos.h>
#include <crypto.h>

#define MD4_BLOCK_SIZE 64u
#define MD4_DIGEST_SIZE 16u

extern const hash_info_t MD4_HASH_ALGO[1];

struct md4_algo_t: public hash_algo_t
{

	md4_algo_t():	hash_algo_t(MD4_HASH_ALGO)
	{
		md4_algo_t::Reset();
	};


	void Reset() override;
	void Result(void* digest) override;
	void process_block(const uint32_t* block) override;

};

hash_algo_t* new_md4_hash();




#endif /* SERVICES_SECURITY_CRYPTO_MD4_H_ */
