/*
 * md2.h
 *
 *  Created on: 19 Sept 2023
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_CRYPTO_MD2_H_
#define SERVICES_SECURITY_CRYPTO_MD2_H_

#include <tmos.h>
#include <crypto.h>

#define MD2_BLOCK_SIZE 16u
#define MD2_DIGEST_SIZE 16u

extern const hash_info_t MD2_HASH_ALGO[1];

struct md2_algo_t: public hash_algo_t
{

	md2_algo_t():	hash_algo_t(MD2_HASH_ALGO)
	{
		md2_algo_t::Reset();
	};


	void Reset() override;
	void Result(void* digest) override;
	void process_block(const uint32_t* block) override;

};

hash_algo_t* new_md2_hash();




#endif /* SERVICES_SECURITY_CRYPTO_MD2_H_ */
