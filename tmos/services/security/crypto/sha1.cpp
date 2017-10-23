/*
 * sha1.cpp
 *
 *  Created on: Jun 12, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha1.h>

#if SHA1_SUPPORT

//SHA-1 object identifier (1.3.14.3.2.26)
const uint8_t sha1Oid[] = {0x2B, 0x0E, 0x03, 0x02, 0x1A};

const hash_info_t SHA1_HASH_ALGO[1] =
{
	"SHA-1",
	SHA1_BLOCK_SIZE,
	SHA1_DIGEST_SIZE,
	sizeof(sha1Oid),
	sha1Oid,
	new_sha1_hash
};

void sha1_algo_t::Reset()
{
	uint32_t* h = digest32();

	//Set initial hash value
	h[0] = 0x67452301;
	h[1] = 0xEFCDAB89;
	h[2] = 0x98BADCFE;
	h[3] = 0x10325476;
	h[4] = 0xC3D2E1F0;

	indx = 0;
	total_blocks = 0;
}

void sha1_algo_t::Result(void* digest)
{
	uint64_t total_bits;
	uint32_t* h = digest32();

	//Length of the original message (before padding)
	total_bits = total_blocks;
	total_bits *= SHA1_BLOCK_SIZE;
	total_bits += indx;
	total_bits *= 8;

	buf8[indx++] = 0x80;
	if (indx > (SHA1_BLOCK_SIZE - 8))
	{
		while (indx < SHA1_BLOCK_SIZE)
		{
			buf8[indx++] = 0;
		}

		process_block(buf32);
		indx = 0;
	}
	while (indx < (SHA1_BLOCK_SIZE - 8))
	{
		buf8[indx++] = 0;
	}

	buf32[14] = __REV(total_bits >> 32);
	buf32[15] = __REV(total_bits);

	process_block(buf32);

	//Convert from host byte order to big-endian byte order
	for (uint32_t i = 0; i < 5; i++)
		h[i] = __REV(h[i]);

	//Copy the resulting digest
	if (digest)
		memcpy(digest, h, SHA1_DIGEST_SIZE);
}


#define W(x) buf32[(x) & 0x0F]
#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))

#define CH(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define PARITY(x, y, z) ((x) ^ (y) ^ (z))
#define MAJ(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))

//SHA-1 constants
static const uint32_t k[4] =
{
   0x5A827999,
   0x6ED9EBA1,
   0x8F1BBCDC,
   0xCA62C1D6
};


void sha1_algo_t::process_block(const uint32_t* block)
{
	uint32_t i, a, b, c, d, e;
	uint32_t temp;
	uint32_t* h = digest32();

	//Convert from big-endian byte order to host byte order
	for (i = 0; i < 16; i++)
		buf32[i] = __REV(block[i]);

	//Initialize the 5 working registers
	a = h[0];
	b = h[1];
	c = h[2];
	d = h[3];
	e = h[4];

	//SHA-1 hash computation (alternate method)
	for (i = 0; i < 80; i++)
	{
		//Prepare the message schedule
		if (i >= 16)
			W(i) = ROL32(W(i + 13) ^ W(i + 8) ^ W(i + 2) ^ W(i), 1);

		//Calculate T
		if (i < 20)
			temp = ROL32(a, 5) + CH(b, c, d) + e + W(i) + k[0];
		else if (i < 40)
			temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(i) + k[1];
		else if (i < 60)
			temp = ROL32(a, 5) + MAJ(b, c, d) + e + W(i) + k[2];
		else
			temp = ROL32(a, 5) + PARITY(b, c, d) + e + W(i) + k[3];

		//Update the working registers
		e = d;
		d = c;
		c = ROL32(b, 30);
		b = a;
		a = temp;
	}

	//Update the hash value
	h[0] += a;
	h[1] += b;
	h[2] += c;
	h[3] += d;
	h[4] += e;
	total_blocks ++;
}

hash_algo_t* new_sha1_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA1_BLOCK_SIZE + SHA1_DIGEST_SIZE);

	return new(ptr) sha1_algo_t();
}


#endif // SHA1_SUPPORT
