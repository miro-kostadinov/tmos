/*
 * md5.cpp

 *
 *  Created on: Jun 13, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <md5.h>


#if MD5_SUPPORT

//MD5 object identifier (1.2.840.113549.2.5)
const uint8_t md5Oid[] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x02, 0x05};

const hash_info_t MD5_HASH_ALGO[1] =
{
	"MD5",
	MD5_BLOCK_SIZE,
	MD5_DIGEST_SIZE,
	MD5_BLOCK_SIZE + MD5_DIGEST_SIZE,
	sizeof(md5Oid),
	md5Oid,
	new_md5_hash
};

void md5_algo_t::Reset()
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

void md5_algo_t::Result(void* digest)
{
	uint64_t total_bits;
	uint32_t* h = digest32();

	//Length of the original message (before padding)
	total_bits = total_blocks;
	total_bits *= MD5_BLOCK_SIZE;
	total_bits += indx;
	total_bits *= 8;

	buf8[indx++] = 0x80;
	if (indx > (MD5_BLOCK_SIZE - 8))
	{
		while (indx < MD5_BLOCK_SIZE)
		{
			buf8[indx++] = 0;
		}

		process_block(buf32);
		indx = 0;
	}
	while (indx < (MD5_BLOCK_SIZE - 8))
	{
		buf8[indx++] = 0;
	}

	buf32[14] = total_bits;
	buf32[15] = total_bits >> 32;

	process_block(buf32);

	//Copy the resulting digest
	if (digest)
		memcpy(digest, h, MD5_DIGEST_SIZE);
}


/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) ((x & y) | ((~x) & z))
#define G(x, y, z) ((x & z) | (y & (~z)))
#define H(x, y, z) (x ^ y ^ z)
#define I(x, y, z) (y ^ (x | (~z)))

/* ROTATE_LEFT rotates x left n bits.  */
#define ROTATE_LEFT(x, n) ((x << n) | (x >> (32-n)))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
   Rotation is separate from addition to prevent recomputation.  */
#define FF(a, b, c, d, x, s, ac) { \
    a += F (b, c, d) + x + ac; \
    a = ROTATE_LEFT (a, s) + b; \
  }
#define GG(a, b, c, d, x, s, ac) { \
    a += G (b, c, d) + x + ac; \
    a = ROTATE_LEFT (a, s) + b; \
  }
#define HH(a, b, c, d, x, s, ac) { \
    a += H (b, c, d) + x + ac; \
    a = ROTATE_LEFT (a, s) + b; \
  }
#define II(a, b, c, d, x, s, ac) { \
    a += I (b, c, d) + x + ac; \
    a = ROTATE_LEFT (a, s) + b; \
  }

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

void md5_algo_t::process_block(const uint32_t* block)
{
	uint32_t a, b, c, d;
	uint32_t* h = digest32();

	//Initialize the 5 working registers
	a = h[0];
	b = h[1];
	c = h[2];
	d = h[3];


    /* Round 1 */
    FF (a, b, c, d, block[ 0], S11, 0xd76aa478); /* 1 */
    FF (d, a, b, c, block[ 1], S12, 0xe8c7b756); /* 2 */
    FF (c, d, a, b, block[ 2], S13, 0x242070db); /* 3 */
    FF (b, c, d, a, block[ 3], S14, 0xc1bdceee); /* 4 */
    FF (a, b, c, d, block[ 4], S11, 0xf57c0faf); /* 5 */
    FF (d, a, b, c, block[ 5], S12, 0x4787c62a); /* 6 */
    FF (c, d, a, b, block[ 6], S13, 0xa8304613); /* 7 */
    FF (b, c, d, a, block[ 7], S14, 0xfd469501); /* 8 */
    FF (a, b, c, d, block[ 8], S11, 0x698098d8); /* 9 */
    FF (d, a, b, c, block[ 9], S12, 0x8b44f7af); /* 10 */
    FF (c, d, a, b, block[10], S13, 0xffff5bb1); /* 11 */
    FF (b, c, d, a, block[11], S14, 0x895cd7be); /* 12 */
    FF (a, b, c, d, block[12], S11, 0x6b901122); /* 13 */
    FF (d, a, b, c, block[13], S12, 0xfd987193); /* 14 */
    FF (c, d, a, b, block[14], S13, 0xa679438e); /* 15 */
    FF (b, c, d, a, block[15], S14, 0x49b40821); /* 16 */

    /* Round 2 */
    GG (a, b, c, d, block[ 1], S21, 0xf61e2562); /* 17 */
    GG (d, a, b, c, block[ 6], S22, 0xc040b340); /* 18 */
    GG (c, d, a, b, block[11], S23, 0x265e5a51); /* 19 */
    GG (b, c, d, a, block[ 0], S24, 0xe9b6c7aa); /* 20 */
    GG (a, b, c, d, block[ 5], S21, 0xd62f105d); /* 21 */
    GG (d, a, b, c, block[10], S22, 0x02441453); /* 22 */
    GG (c, d, a, b, block[15], S23, 0xd8a1e681); /* 23 */
    GG (b, c, d, a, block[ 4], S24, 0xe7d3fbc8); /* 24 */
    GG (a, b, c, d, block[ 9], S21, 0x21e1cde6); /* 25 */
    GG (d, a, b, c, block[14], S22, 0xc33707d6); /* 26 */
    GG (c, d, a, b, block[ 3], S23, 0xf4d50d87); /* 27 */
    GG (b, c, d, a, block[ 8], S24, 0x455a14ed); /* 28 */
    GG (a, b, c, d, block[13], S21, 0xa9e3e905); /* 29 */
    GG (d, a, b, c, block[ 2], S22, 0xfcefa3f8); /* 30 */
    GG (c, d, a, b, block[ 7], S23, 0x676f02d9); /* 31 */
    GG (b, c, d, a, block[12], S24, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH (a, b, c, d, block[ 5], S31, 0xfffa3942); /* 33 */
    HH (d, a, b, c, block[ 8], S32, 0x8771f681); /* 34 */
    HH (c, d, a, b, block[11], S33, 0x6d9d6122); /* 35 */
    HH (b, c, d, a, block[14], S34, 0xfde5380c); /* 36 */
    HH (a, b, c, d, block[ 1], S31, 0xa4beea44); /* 37 */
    HH (d, a, b, c, block[ 4], S32, 0x4bdecfa9); /* 38 */
    HH (c, d, a, b, block[ 7], S33, 0xf6bb4b60); /* 39 */
    HH (b, c, d, a, block[10], S34, 0xbebfbc70); /* 40 */
    HH (a, b, c, d, block[13], S31, 0x289b7ec6); /* 41 */
    HH (d, a, b, c, block[ 0], S32, 0xeaa127fa); /* 42 */
    HH (c, d, a, b, block[ 3], S33, 0xd4ef3085); /* 43 */
    HH (b, c, d, a, block[ 6], S34, 0x04881d05); /* 44 */
    HH (a, b, c, d, block[ 9], S31, 0xd9d4d039); /* 45 */
    HH (d, a, b, c, block[12], S32, 0xe6db99e5); /* 46 */
    HH (c, d, a, b, block[15], S33, 0x1fa27cf8); /* 47 */
    HH (b, c, d, a, block[ 2], S34, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II (a, b, c, d, block[ 0], S41, 0xf4292244); /* 49 */
    II (d, a, b, c, block[ 7], S42, 0x432aff97); /* 50 */
    II (c, d, a, b, block[14], S43, 0xab9423a7); /* 51 */
    II (b, c, d, a, block[ 5], S44, 0xfc93a039); /* 52 */
    II (a, b, c, d, block[12], S41, 0x655b59c3); /* 53 */
    II (d, a, b, c, block[ 3], S42, 0x8f0ccc92); /* 54 */
    II (c, d, a, b, block[10], S43, 0xffeff47d); /* 55 */
    II (b, c, d, a, block[ 1], S44, 0x85845dd1); /* 56 */
    II (a, b, c, d, block[ 8], S41, 0x6fa87e4f); /* 57 */
    II (d, a, b, c, block[15], S42, 0xfe2ce6e0); /* 58 */
    II (c, d, a, b, block[ 6], S43, 0xa3014314); /* 59 */
    II (b, c, d, a, block[13], S44, 0x4e0811a1); /* 60 */
    II (a, b, c, d, block[ 4], S41, 0xf7537e82); /* 61 */
    II (d, a, b, c, block[11], S42, 0xbd3af235); /* 62 */
    II (c, d, a, b, block[ 2], S43, 0x2ad7d2bb); /* 63 */
    II (b, c, d, a, block[ 9], S44, 0xeb86d391); /* 64 */


	//Update the hash value
	h[0] += a;
	h[1] += b;
	h[2] += c;
	h[3] += d;
	total_blocks ++;
}

hash_algo_t* new_md5_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + MD5_BLOCK_SIZE + MD5_DIGEST_SIZE);

	return new (ptr) md5_algo_t();
}


#endif // MD5_SUPPORT



