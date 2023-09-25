/*
 * md4.cpp
 *
 *  Created on: 19 Sept 2023
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <md4.h>


#if MD4_SUPPORT

//MD4 auxiliary functions
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))

#define FF(a, b, c, d, x, s) a += F(b, c, d) + (x), a = ROL32(a, s)
#define GG(a, b, c, d, x, s) a += G(b, c, d) + (x) + 0x5A827999, a = ROL32(a, s)
#define HH(a, b, c, d, x, s) a += H(b, c, d) + (x) + 0x6ED9EBA1, a = ROL32(a, s)

//MD4 object identifier (1.2.840.113549.2.4)
const uint8_t md4Oid[8] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x02, 0x04};

const hash_info_t MD4_HASH_ALGO[1] =
{
	"MD4",
	MD4_BLOCK_SIZE,
	MD4_DIGEST_SIZE,
	MD4_BLOCK_SIZE + MD4_DIGEST_SIZE,
	sizeof(md4Oid),
	md4Oid,
	new_md4_hash
};

void md4_algo_t::Reset()
{
	uint32_t* h = digest32();

	//Set initial hash value
	h[0] = 0x67452301;
	h[1] = 0xEFCDAB89;
	h[2] = 0x98BADCFE;
	h[3] = 0x10325476;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

void md4_algo_t::Result(void* digest)
{
	uint64_t total_bits;
	uint32_t* h = digest32();

	//Length of the original message (before padding)
	total_bits = total_blocks;
	total_bits *= MD4_BLOCK_SIZE;
	total_bits += indx;
	total_bits *= 8;

	buf8[indx++] = 0x80;
	if (indx > (MD4_BLOCK_SIZE - 8))
	{
		while (indx < MD4_BLOCK_SIZE)
		{
			buf8[indx++] = 0;
		}

		process_block(buf32);
		indx = 0;
	}
	while (indx < (MD4_BLOCK_SIZE - 8))
	{
		buf8[indx++] = 0;
	}

	buf32[14] = total_bits;
	buf32[15] = total_bits >> 32;

	process_block(buf32);

	//Copy the resulting digest
	if (digest)
		memcpy(digest, h, MD4_DIGEST_SIZE);
}



void md4_algo_t::process_block(const uint32_t* block)
{
	uint32_t a, b, c, d;
	uint32_t* h = digest32();

	//Initialize the 4 working registers
	a = h[0];
	b = h[1];
	c = h[2];
	d = h[3];

	//Round 1
	FF(a, b, c, d, block[ 0], 3);
	FF(d, a, b, c, block[ 1], 7);
	FF(c, d, a, b, block[ 2], 11);
	FF(b, c, d, a, block[ 3], 19);
	FF(a, b, c, d, block[ 4], 3);
	FF(d, a, b, c, block[ 5], 7);
	FF(c, d, a, b, block[ 6], 11);
	FF(b, c, d, a, block[ 7], 19);
	FF(a, b, c, d, block[ 8], 3);
	FF(d, a, b, c, block[ 9], 7);
	FF(c, d, a, b, block[10], 11);
	FF(b, c, d, a, block[11], 19);
	FF(a, b, c, d, block[12], 3);
	FF(d, a, b, c, block[13], 7);
	FF(c, d, a, b, block[14], 11);
	FF(b, c, d, a, block[15], 19);

	//Round 2
	GG(a, b, c, d, block[ 0], 3);
	GG(d, a, b, c, block[ 4], 5);
	GG(c, d, a, b, block[ 8], 9);
	GG(b, c, d, a, block[12], 13);
	GG(a, b, c, d, block[ 1], 3);
	GG(d, a, b, c, block[ 5], 5);
	GG(c, d, a, b, block[ 9], 9);
	GG(b, c, d, a, block[13], 13);
	GG(a, b, c, d, block[ 2], 3);
	GG(d, a, b, c, block[ 6], 5);
	GG(c, d, a, b, block[10], 9);
	GG(b, c, d, a, block[14], 13);
	GG(a, b, c, d, block[ 3], 3);
	GG(d, a, b, c, block[ 7], 5);
	GG(c, d, a, b, block[11], 9);
	GG(b, c, d, a, block[15], 13);

	//Round 3
	HH(a, b, c, d, block[ 0], 3);
	HH(d, a, b, c, block[ 8], 9);
	HH(c, d, a, b, block[ 4], 11);
	HH(b, c, d, a, block[12], 15);
	HH(a, b, c, d, block[ 2], 3);
	HH(d, a, b, c, block[10], 9);
	HH(c, d, a, b, block[ 6], 11);
	HH(b, c, d, a, block[14], 15);
	HH(a, b, c, d, block[ 1], 3);
	HH(d, a, b, c, block[ 9], 9);
	HH(c, d, a, b, block[ 5], 11);
	HH(b, c, d, a, block[13], 15);
	HH(a, b, c, d, block[ 3], 3);
	HH(d, a, b, c, block[11], 9);
	HH(c, d, a, b, block[ 7], 11);
	HH(b, c, d, a, block[15], 15);

	//Update the hash value
	h[0] += a;
	h[1] += b;
	h[2] += c;
	h[3] += d;
	total_blocks ++;
}

hash_algo_t* new_md4_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + MD4_BLOCK_SIZE + MD4_DIGEST_SIZE);

	return new (ptr) md4_algo_t();
}


#endif // MD2_SUPPORT



