/*
 * hmac.cpp
 *
 *  Created on: Jun 14, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <hmac.h>

void hmac_ctxt_t::Reset(hash_algo_t* hash, const void *key, uint32_t key_len)
{
	uint32_t ds, bs;
	//Hash algorithm used to compute HMAC
	hash_algo = hash;

	//The key is longer than the block size?
	ds = hash->hash_info->digest_size;
	bs = hash->hash_info->algo_info.block_size;
	if(key_len > bs)
	{
		hash_algo->Reset();
		hash_algo->Input(key, key_len);
		hash_algo->Result(hmac_key);
		memclr(hmac_key + ds, bs - ds);
	}
	else
	{
		memcpy(hmac_key, key, key_len);
		memclr(hmac_key + key_len, bs - key_len);
	}

	for(uint32_t i = 0; i < bs; i++)
		hmac_key[i] ^= HMAC_IPAD;

	//Initialize context for the first pass
	hash_algo->Reset();
	hash_algo->Input(hmac_key, bs);
}

void hmac_ctxt_t::Input(const void* data, size_t len)
{
	hash_algo->Input(data, len);
}

void hmac_ctxt_t::Result(void* digest)
{
	//Finish the first pass
	hash_algo->Result(hmac_digest);

	//XOR the original key with opad
	for (uint32_t i = 0; i < hash_algo->hash_info->algo_info.block_size; i++)
		hmac_key[i] ^= HMAC_IPAD ^ HMAC_OPAD;

	//Initialize context for the second pass
	hash_algo->Reset();
	hash_algo->Input(hmac_key, hash_algo->hash_info->algo_info.block_size);
	hash_algo->Input(hmac_digest, hash_algo->hash_info->digest_size);
	hash_algo->Result(hmac_digest);

	//Copy the resulting HMAC value
	if (digest != NULL)
		memcpy(digest, hmac_digest, hash_algo->hash_info->digest_size);
}






