/*
 * sha512_224.cpp
 *
 *  Created on: 19 Sept 2023
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha512_224.h>


#if SHA512_224_SUPPORT


//SHA-512/224 object identifier (2.16.840.1.101.3.4.2.5)
const uint8_t sha512_224Oid[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x05};

const hash_info_t SHA512_HASH_ALGO[1] =
{
	"SHA-512/224",
	SHA512_224_BLOCK_SIZE,
	SHA512_224_DIGEST_SIZE,
	SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE,
	sizeof(sha512_224Oid),
	sha512_224Oid,
	new_sha512_224_hash
};



void sha512_224_algo_t::Reset()
{
	uint64_t* h = digest64();

	//Set initial hash value
	h[0] = 0x8C3D37C819544DA2;
	h[1] = 0x73E1996689DCD4D6;
	h[2] = 0x1DFAB7AE32FF9C82;
	h[3] = 0x679DD514582F9FCF;
	h[4] = 0x0F6D2B697BD44DA8;
	h[5] = 0x77E36F7304C48942;
	h[6] = 0x3F9D85A86A1D36C8;
	h[7] = 0x1112E6AD91D692A1;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

hash_algo_t* new_sha512_224_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE);

	return new(ptr) sha512_224_algo_t();
}

#endif




