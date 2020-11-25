/*
 * sha384.cpp
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha384.h>

#if SHA384_SUPPORT

//SHA-384 object identifier (2.16.840.1.101.3.4.2.2)
const uint8_t sha384Oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02};

const hash_info_t SHA384_HASH_ALGO[1] =
{
	"SHA-384",
	SHA384_BLOCK_SIZE,
	SHA384_DIGEST_SIZE,
	sizeof(sha384Oid),
	sha384Oid,
	new_sha384_hash
};


void sha384_algo_t::Reset()
{
	uint64_t* h = digest64();

	h[0] = 0xCBBB9D5DC1059ED8;
	h[1] = 0x629A292A367CD507;
	h[2] = 0x9159015A3070DD17;
	h[3] = 0x152FECD8F70E5939;
	h[4] = 0x67332667FFC00B31;
	h[5] = 0x8EB44A8768581511;
	h[6] = 0xDB0C2E0D64F98FA7;
	h[7] = 0x47B5481DBEFA4FA4;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

hash_algo_t* new_sha384_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE);

	return new(ptr) sha384_algo_t();
}


#endif // SHA384_SUPPORT




