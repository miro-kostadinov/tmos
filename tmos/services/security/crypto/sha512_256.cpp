/*
 * sha512_256.cpp
 *
 *  Created on: 19 Sept 2023
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha512_256.h>


#if SHA512_256_SUPPORT


//SHA-512/256 object identifier (2.16.840.1.101.3.4.2.6)
const uint8_t sha512_256Oid[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x06};

const hash_info_t SHA512_HASH_ALGO[1] =
{
	"SHA-512/256",
	SHA512_256_BLOCK_SIZE,
	SHA512_256_DIGEST_SIZE,
	SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE,
	sizeof(sha512_256Oid),
	sha512_256Oid,
	new_sha512_256_hash
};



void sha512_256_algo_t::Reset()
{
	uint64_t* h = digest64();

	//Set initial hash value
	h[0] = 0x22312194FC2BF72C;
	h[1] = 0x9F555FA3C84C64C2;
	h[2] = 0x2393B86B6F53B151;
	h[3] = 0x963877195940EABD;
	h[4] = 0x96283EE2A88EFFE3;
	h[5] = 0xBE5E1E2553863992;
	h[6] = 0x2B0199FC2C85B8AA;
	h[7] = 0x0EB72DDC81C52CA2;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

hash_algo_t* new_sha512_256_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE);

	return new(ptr) sha512_256_algo_t();
}

#endif








