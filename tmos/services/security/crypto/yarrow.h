/*
 * yarrow.h
 *
 *  Created on: Sep 28, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_YARROW_H_
#define SERVICES_SECURITY_CRYPTO_YARROW_H_

#include <tmos.h>
#include <crypto.h>
#include <hmac.h>
#include <aes.h>

//Yarrow PRNG parameters
#define YARROW_N 3
#define YARROW_K 2
#define YARROW_PG 10
#define YARROW_FAST_THRESHOLD 100
#define YARROW_SLOW_THRESHOLD 160

extern const algo_info_t yarrow_prng_info;

struct prng_yarrow_algo_t: public prng_algo_t
{
	//OsMutex mutex; 						//Mutex to prevent simultaneous access to the PRNG state
	bool 	ready; 						//This flag tells whether the PRNG has been properly seeded
	uint8_t key[32]; 					//Current key
	uint8_t counter[16]; 				//Counter block
	size_t blockCount; 					//Number of blocks that have been generated
	uint32_t currentPool[YARROW_N]; 	//Current pool identifier
	size_t fastPoolEntropy[YARROW_N]; 	//Entropy estimation (fast pool)
	size_t slowPoolEntropy[YARROW_N]; 	//Entropy estimation (slow pool)
	static_sha256_t fastPool; //Fast pool
	static_sha256_t slowPool; //Slow pool
	aes_cipher_algo_t cipherContext; //Cipher context

	prng_yarrow_algo_t():
		prng_algo_t(&yarrow_prng_info),
		ready(false),
		blockCount(0),
		cipherContext(&aes_cipher_info)
	{};

	RES_CODE prng_init() override;
	RES_CODE prng_seed(const void* input, size_t length) override;
	RES_CODE prng_read(void* output, size_t length) override;

	void yarrowFastReseed();
	void yarrowGenerateBlock(uint8_t *output);
};


#endif /* SERVICES_SECURITY_CRYPTO_YARROW_H_ */
