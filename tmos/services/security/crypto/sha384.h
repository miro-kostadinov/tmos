/*
 * sha384.h
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_SHA384_H_
#define SERVICES_SECURITY_CRYPTO_SHA384_H_

#include <tmos.h>
#include <sha512.h>


#define SHA384_BLOCK_SIZE 128
#define SHA384_DIGEST_SIZE 48

extern const hash_info_t SHA384_HASH_ALGO[1];


struct sha384_algo_t: public sha512_algo_t
{
	sha384_algo_t() : sha512_algo_t(SHA384_HASH_ALGO)
	{
		Reset();
	}


	void Reset() override;
};

hash_algo_t* new_sha384_hash();


#endif /* SERVICES_SECURITY_CRYPTO_SHA384_H_ */
