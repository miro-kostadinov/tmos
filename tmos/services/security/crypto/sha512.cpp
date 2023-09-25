/*
 * sha512.cpp
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <sha512.h>


#if SHA384_SUPPORT || SHA512_SUPPORT ||  SHA512_224_SUPPORT || SHA512_256_SUPPORT


//SHA-512 object identifier (2.16.840.1.101.3.4.2.3)
const uint8_t SHA512_OID[] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03};

const hash_info_t SHA512_HASH_ALGO[1] =
{
	"SHA-512",
	SHA512_BLOCK_SIZE,
	SHA512_DIGEST_SIZE,
	SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE,
	sizeof(SHA512_OID),
	SHA512_OID,
	new_sha512_hash
};


//Macro to access the workspace as a circular buffer
#define W(t) buf64[(t) & 0x0F]

//SHA-512 auxiliary functions
#define CH(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z)))
#define SIGMA1(x) (ROR64(x, 28) ^ ROR64(x, 34) ^ ROR64(x, 39))
#define SIGMA2(x) (ROR64(x, 14) ^ ROR64(x, 18) ^ ROR64(x, 41))
#define SIGMA3(x) (ROR64(x, 1) ^ ROR64(x, 8) ^ SHR64(x, 7))
#define SIGMA4(x) (ROR64(x, 19) ^ ROR64(x, 61) ^ SHR64(x, 6))

//SHA-512 constants
const uint64_t k[80] =
{
   0x428A2F98D728AE22, 0x7137449123EF65CD, 0xB5C0FBCFEC4D3B2F, 0xE9B5DBA58189DBBC,
   0x3956C25BF348B538, 0x59F111F1B605D019, 0x923F82A4AF194F9B, 0xAB1C5ED5DA6D8118,
   0xD807AA98A3030242, 0x12835B0145706FBE, 0x243185BE4EE4B28C, 0x550C7DC3D5FFB4E2,
   0x72BE5D74F27B896F, 0x80DEB1FE3B1696B1, 0x9BDC06A725C71235, 0xC19BF174CF692694,
   0xE49B69C19EF14AD2, 0xEFBE4786384F25E3, 0x0FC19DC68B8CD5B5, 0x240CA1CC77AC9C65,
   0x2DE92C6F592B0275, 0x4A7484AA6EA6E483, 0x5CB0A9DCBD41FBD4, 0x76F988DA831153B5,
   0x983E5152EE66DFAB, 0xA831C66D2DB43210, 0xB00327C898FB213F, 0xBF597FC7BEEF0EE4,
   0xC6E00BF33DA88FC2, 0xD5A79147930AA725, 0x06CA6351E003826F, 0x142929670A0E6E70,
   0x27B70A8546D22FFC, 0x2E1B21385C26C926, 0x4D2C6DFC5AC42AED, 0x53380D139D95B3DF,
   0x650A73548BAF63DE, 0x766A0ABB3C77B2A8, 0x81C2C92E47EDAEE6, 0x92722C851482353B,
   0xA2BFE8A14CF10364, 0xA81A664BBC423001, 0xC24B8B70D0F89791, 0xC76C51A30654BE30,
   0xD192E819D6EF5218, 0xD69906245565A910, 0xF40E35855771202A, 0x106AA07032BBD1B8,
   0x19A4C116B8D2D0C8, 0x1E376C085141AB53, 0x2748774CDF8EEB99, 0x34B0BCB5E19B48A8,
   0x391C0CB3C5C95A63, 0x4ED8AA4AE3418ACB, 0x5B9CCA4F7763E373, 0x682E6FF3D6B2B8A3,
   0x748F82EE5DEFB2FC, 0x78A5636F43172F60, 0x84C87814A1F0AB72, 0x8CC702081A6439EC,
   0x90BEFFFA23631E28, 0xA4506CEBDE82BDE9, 0xBEF9A3F7B2C67915, 0xC67178F2E372532B,
   0xCA273ECEEA26619C, 0xD186B8C721C0C207, 0xEADA7DD6CDE0EB1E, 0xF57D4F7FEE6ED178,
   0x06F067AA72176FBA, 0x0A637DC5A2C898A6, 0x113F9804BEF90DAE, 0x1B710B35131C471B,
   0x28DB77F523047D84, 0x32CAAB7B40C72493, 0x3C9EBE0A15C9BEBC, 0x431D67C49C100D4C,
   0x4CC5D4BECB3E42B6, 0x597F299CFC657E2A, 0x5FCB6FAB3AD6FAEC, 0x6C44198C4A475817
};


void sha512_algo_t::Reset()
{
	uint64_t* h = digest64();

	//Set initial hash value
	h[0] = 0x6A09E667F3BCC908;
	h[1] = 0xBB67AE8584CAA73B;
	h[2] = 0x3C6EF372FE94F82B;
	h[3] = 0xA54FF53A5F1D36F1;
	h[4] = 0x510E527FADE682D1;
	h[5] = 0x9B05688C2B3E6C1F;
	h[6] = 0x1F83D9ABFB41BD6B;
	h[7] = 0x5BE0CD19137E2179;

	indx = 0;
	total_blocks = 0;
	Computed = false;
}

void sha512_algo_t::Result(void* digest)
{
	uint64_t total_bits;
	uint64_t* h = digest64();

	//Length of the original message (before padding)
	total_bits = total_blocks;
	total_bits *= SHA512_BLOCK_SIZE;
	total_bits += indx;
	total_bits *= 8;

	buf8[indx++] = 0x80;
	if (indx > 112)
	{
		while (indx < 128)
		{
			buf8[indx++] = 0;
		}

		process_block(buf32);
		indx = 0;
	}
	while (indx < 112)
	{
		buf8[indx++] = 0;
	}

	buf64[14] = 0;
	buf64[15] = __REV64(total_bits);

	process_block(buf32);

	//Convert from host byte order to big-endian byte order
	for (int i = 0; i < 8; i++)
		h[i] = __REV64(h[i]);

	//Copy the resulting digest
	if (digest != NULL)
		memcpy(digest, h, hash_info->digest_size);
}

void sha512_algo_t::process_block(const uint32_t* block)
{
	uint32_t t, temp32;
	uint64_t temp1;
	uint64_t temp2;
	uint64_t* h = digest64();

	//Convert from big-endian byte order to host byte order
	for (t = 0; t < 32; t+=2)
	{
		temp32 = __REV(block[t]);
		buf32[t] = __REV(block[t+1]);
		buf32[t+1] = temp32;
	}

	//Initialize the 8 working registers
	uint64_t a = h[0];
	uint64_t b = h[1];
	uint64_t c = h[2];
	uint64_t d = h[3];
	uint64_t e = h[4];
	uint64_t f = h[5];
	uint64_t g = h[6];
	uint64_t hh = h[7];

	//SHA-512 hash computation (alternate method)
	for (t = 0; t < 80; t++)
	{
		//Prepare the message schedule
		if (t >= 16)
			W(t) += SIGMA4(W(t + 14)) + W(t + 9) + SIGMA3(W(t + 1));

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

	total_blocks ++;
}


hash_algo_t* new_sha512_hash()
{
	void * ptr = tsk_malloc(sizeof(hash_algo_t) + SHA512_BLOCK_SIZE + SHA512_DIGEST_SIZE);

	return new(ptr) sha512_algo_t();
}

#endif
