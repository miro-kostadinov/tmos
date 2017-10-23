/*
 * md5.h
 *
 *  Created on: Jun 13, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_MD5_H_
#define SERVICES_SECURITY_CRYPTO_MD5_H_

#include <tmos.h>
#include <crypto.h>

#define MD5_BLOCK_SIZE 64u
#define MD5_DIGEST_SIZE 16u

extern const hash_info_t MD5_HASH_ALGO[1];

struct md5_algo_t: public hash_algo_t
{

	md5_algo_t():	hash_algo_t(MD5_HASH_ALGO)
	{
		Reset();
	};


	virtual void Reset();
	virtual void Result(void* digest);
	virtual void process_block(const uint32_t* block);

};

hash_algo_t* new_md5_hash();




#endif /* SERVICES_SECURITY_CRYPTO_MD5_H_ */
