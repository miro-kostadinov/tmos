/*
 * sha256.cpp
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha256.h>


#if SHA224_SUPPORT || SHA256_SUPPORT

//SHA-256 object identifier (2.16.840.1.101.3.4.2.1)
const uint8_t sha256Oid[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01};

const hash_info_t SHA256_HASH_ALGO[1] =
{
	"SHA-256",
	SHA256_BLOCK_SIZE,
	SHA256_DIGEST_SIZE,
	sizeof(sha256Oid),
	sha256Oid,
	new_sha256_hash
};

//Macro to access the workspace as a circular buffer
#define W(t) buf32[(t) & 0x0F]

//SHA-256 auxiliary functions
#define CH(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define SIGMA1(x) (ROR32(x, 2) ^ ROR32(x, 13) ^ ROR32(x, 22))
#define SIGMA2(x) (ROR32(x, 6) ^ ROR32(x, 11) ^ ROR32(x, 25))
#define SIGMA3(x) (ROR32(x, 7) ^ ROR32(x, 18) ^ SHR32(x, 3))
#define SIGMA4(x) (ROR32(x, 17) ^ ROR32(x, 19) ^ SHR32(x, 10))

//SHA-256 constants
static const uint32_t k[64] =
{
   0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
   0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
   0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
   0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
   0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
   0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
   0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
   0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
};


void sha256_algo_t::Reset()
{
	uint32_t* h = digest32();

	h[0] = 0x6A09E667;
	h[1] = 0xBB67AE85;
	h[2] = 0x3C6EF372;
	h[3] = 0xA54FF53A;
	h[4] = 0x510E527F;
	h[5] = 0x9B05688C;
	h[6] = 0x1F83D9AB;
	h[7] = 0x5BE0CD19;

	indx = 0;
	total_blocks = 0;
}

void sha256_algo_t::Result(void* digest)
{
	uint64_t total_bits;
	uint32_t* h = digest32();

	//Length of the original message (before padding)
	total_bits = total_blocks;
	total_bits *= SHA256_BLOCK_SIZE;
	total_bits += indx;
	total_bits *= 8;

	buf8[indx++] = 0x80;
	if (indx > (SHA256_BLOCK_SIZE - 8))
	{
		while (indx < SHA256_BLOCK_SIZE)
		{
			buf8[indx++] = 0;
		}

		process_block(buf32);
		indx = 0;
	}
	while (indx < (SHA256_BLOCK_SIZE - 8))
	{
		buf8[indx++] = 0;
	}

	buf32[14] = __REV(total_bits >> 32);
	buf32[15] = __REV(total_bits);

	process_block(buf32);

	//Convert from host byte order to big-endian byte order
	for (uint32_t i = 0; i < 8; i++)
		h[i] = __REV(h[i]);

	//Copy the resulting digest
	if (digest != NULL)
		memcpy(digest, h, hash_info->digest_size);
}


void sha256_algo_t::process_block(const uint32_t* block)
{
	uint32_t t;
	uint32_t temp1;
	uint32_t temp2;
	uint32_t* h = digest32();

	//Convert from big-endian byte order to host byte order
	for (t = 0; t < 16; t++)
		buf32[t] = __REV(block[t]);

	//Initialize the 8 working registers
	uint32_t a = h[0];
	uint32_t b = h[1];
	uint32_t c = h[2];
	uint32_t d = h[3];
	uint32_t e = h[4];
	uint32_t f = h[5];
	uint32_t g = h[6];
	uint32_t hh = h[7];

	//SHA-256 hash computation (alternate method)
	for (t = 0; t < 64; t++)
	{
		//Prepare the message schedule
		if (t >= 16)
			W(t)+= SIGMA4(W(t + 14)) + W(t + 9) + SIGMA3(W(t + 1));

			//Calculate T1 and T2
			temp1 = hh + SIGMA2(e) + CH(e, f, g) + k[t] + W(t);
			temp2 = SIGMA1(a) + MAJ(a, b, c);

			//Update the working registers
			hh = g;
			g = f;
			f = e;
			e = d + temp1;
			d = c;
			c = b;
			b = a;
			a = temp1 + temp2;
		}

		//Update the hash value
	h[0] += a;
	h[1] += b;
	h[2] += c;
	h[3] += d;
	h[4] += e;
	h[5] += f;
	h[6] += g;
	h[7] += hh;

	total_blocks++;
}

hash_algo_t* new_sha256_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA256_BLOCK_SIZE + SHA256_DIGEST_SIZE);

	return new(ptr) sha256_algo_t();
}

#endif

