/*
 * sha224.cpp
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha224.h>

#if SHA224_SUPPORT

//SHA-224 object identifier (2.16.840.1.101.3.4.2.4)
const uint8_t SHA224_OID[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04};

const hash_info_t SHA224_HASH_ALGO[1] =
{
	"SHA-224",
	SHA224_BLOCK_SIZE,
	SHA224_DIGEST_SIZE,
	SHA224_BLOCK_SIZE + SHA256_DIGEST_SIZE,
	sizeof(SHA224_OID),
	SHA224_OID,
	new_sha224_hash
};



void sha224_algo_t::Reset()
{
	uint32_t* h = digest32();

	h[0] = 0xC1059ED8;
	h[1] = 0x367CD507;
	h[2] = 0x3070DD17;
	h[3] = 0xF70E5939;
	h[4] = 0xFFC00B31;
	h[5] = 0x68581511;
	h[6] = 0x64F98FA7;
	h[7] = 0xBEFA4FA4;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

hash_algo_t* new_sha224_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA256_BLOCK_SIZE + SHA256_DIGEST_SIZE);

	return new(ptr) sha224_algo_t();
}

#endif // SHA224_SUPPORT


