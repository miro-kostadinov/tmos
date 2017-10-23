/*
 * cipher_gcm.cpp
 *
 *  Created on: Jun 16, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <crypto.h>
#include <tls.h>


#if GCM_SUPPORT
union cbc_buf_t
{
	uint8_t as_byte[16];
	uint32_t as_word[4];
};

static void gcmXorBlock(uint8_t *x, const uint8_t *a, const uint8_t *b, size_t n)
{
   size_t i;

   //Perform XOR operation
   for(i = 0; i < n; i++)
   {
      x[i] = a[i] ^ b[i];
   }
}

static void gcmXorBlock(cbc_buf_t& x, const cbc_buf_t& a, const cbc_buf_t& b)
{
    x.as_word[0] = a.as_word[0] ^ b.as_word[0];
    x.as_word[1] = a.as_word[1] ^ b.as_word[1];
    x.as_word[2] = a.as_word[2] ^ b.as_word[2];
    x.as_word[3] = a.as_word[3] ^ b.as_word[3];
}

//Reduction table
const uint32_t reductuction_table[16] =
{
   0x00000000,
   0x1C200000,
   0x38400000,
   0x24600000,
   0x70800000,
   0x6CA00000,
   0x48C00000,
   0x54E00000,
   0xE1000000,
   0xFD200000,
   0xD9400000,
   0xC5600000,
   0x91800000,
   0x8DA00000,
   0xA9C00000,
   0xB5E00000
};

static void gcm_mul(gcm_context_t* context, cbc_buf_t& x)
{
	int i;
	uint32_t b;
	uint32_t c;
	uint32_t z[4];

	//Let Z = 0
	z[0] = 0;
	z[1] = 0;
	z[2] = 0;
	z[3] = 0;

	//Fast table-driven implementation
	for (i = 15; i >= 0; i--)
	{
		//Process the lower nibble
		b = x.as_byte[i] & 0x0F;

		c = z[0] & 0x0F;
		z[0] = (z[0] >> 4) | (z[1] << 28);
		z[1] = (z[1] >> 4) | (z[2] << 28);
		z[2] = (z[2] >> 4) | (z[3] << 28);
		z[3] >>= 4;

		z[3] ^= reductuction_table[c];

		z[0] ^= context->m[b][0];
		z[1] ^= context->m[b][1];
		z[2] ^= context->m[b][2];
		z[3] ^= context->m[b][3];

		//Process the upper nibble
		b = (x.as_byte[i] >> 4) & 0x0F;

		c = z[0] & 0x0F;
		z[0] = (z[0] >> 4) | (z[1] << 28);
		z[1] = (z[1] >> 4) | (z[2] << 28);
		z[2] = (z[2] >> 4) | (z[3] << 28);
		z[3] >>= 4;

		z[3] ^= reductuction_table[c];

		z[0] ^= context->m[b][0];
		z[1] ^= context->m[b][1];
		z[2] ^= context->m[b][2];
		z[3] ^= context->m[b][3];
	}

	//Save the result
	x.as_word[0] = __REV(z[3]);
	x.as_word[1] = __REV(z[2]);
	x.as_word[2] = __REV(z[1]);
	x.as_word[3] = __REV(z[0]);
}

uint8_t reverseInt4(uint8_t value)
{
   value = ((value & 0x0C) >> 2) | ((value & 0x03) << 2);
   value = ((value & 0x0A) >> 1) | ((value & 0x05) << 1);

   return value;
}

RES_CODE cipher_algo_t::gcm_init()
{
	RES_CODE res;

	gcm_ctxt = new gcm_context_t;

	//Successful memory allocation?
	if (gcm_ctxt.get())
	{
		uint32_t i;
		uint32_t j;
		uint8_t c;
		uint32_t h[4];

		//GCM supports only symmetric block ciphers whose block size is 128 bits
		if (cipher_info->cipher_type != CIPHER_TYPE_BLOCK ||
				cipher_info->algo_info.block_size != 16)
			return RES_TLS_INVALID_PARAMETER;

		//Let H = 0
		h[0] = 0;
		h[1] = 0;
		h[2] = 0;
		h[3] = 0;

		//Generate the hash subkey H
		encrypt(h,	h, 1);

		//Pre-compute M(0) = H * 0
		j = reverseInt4(0);
		gcm_ctxt->m[j][0] = 0;
		gcm_ctxt->m[j][1] = 0;
		gcm_ctxt->m[j][2] = 0;
		gcm_ctxt->m[j][3] = 0;

		//Pre-compute M(1) = H * 1
		j = reverseInt4(1);
		gcm_ctxt->m[j][0] = __REV(h[3]);
		gcm_ctxt->m[j][1] = __REV(h[2]);
		gcm_ctxt->m[j][2] = __REV(h[1]);
		gcm_ctxt->m[j][3] = __REV(h[0]);

		//Pre-compute all 4-bit multiples of H
		for (i = 2; i < 16; i++)
		{
			//Odd value?
			if (i & 1)
			{
				//Compute M(i) = M(i - 1) + H
				j = reverseInt4(i - 1);
				h[0] = gcm_ctxt->m[j][0];
				h[1] = gcm_ctxt->m[j][1];
				h[2] = gcm_ctxt->m[j][2];
				h[3] = gcm_ctxt->m[j][3];

				//An addition in GF(2^128) is identical to a bitwise
				//exclusive-OR operation
				j = reverseInt4(1);
				h[0] ^= gcm_ctxt->m[j][0];
				h[1] ^= gcm_ctxt->m[j][1];
				h[2] ^= gcm_ctxt->m[j][2];
				h[3] ^= gcm_ctxt->m[j][3];
			}
			//Even value?
			else
			{
				//Compute M(i) = M(i / 2) * x
				j = reverseInt4(i / 2);
				h[0] = gcm_ctxt->m[j][0];
				h[1] = gcm_ctxt->m[j][1];
				h[2] = gcm_ctxt->m[j][2];
				h[3] = gcm_ctxt->m[j][3];

				//The multiplication of a polynomial by x in GF(2^128)
				//corresponds to a shift of indices
				c = h[0] & 0x01;
				h[0] = (h[0] >> 1) | (h[1] << 31);
				h[1] = (h[1] >> 1) | (h[2] << 31);
				h[2] = (h[2] >> 1) | (h[3] << 31);
				h[3] >>= 1;

				//If the highest term of the result is equal to one,
				//then perform reduction
				if (c != 0)
				{
					h[3] ^= reductuction_table[reverseInt4(1)];
				}
			}

			//Save M(i)
			j = reverseInt4(i);
			gcm_ctxt->m[j][0] = h[0];
			gcm_ctxt->m[j][1] = h[1];
			gcm_ctxt->m[j][2] = h[2];
			gcm_ctxt->m[j][3] = h[3];
		}
		res = RES_OK;
	}
	else
	{
		res = RES_OUT_OF_MEMORY;
	}

	return res;
}

RES_CODE cipher_algo_t::gcm_encrypt(const uint8_t *iv, size_t ivLen, const uint8_t *a,
		size_t aLen, uint8_t *data, size_t len, uint8_t *t,	size_t tLen)
{
	size_t k;
	size_t n;
	cbc_buf_t b;
	cbc_buf_t j;
	cbc_buf_t s;

	//Make sure the GCM context is valid
	if (gcm_ctxt.get() == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//The length of the IV shall meet SP 800-38D requirements
	if (ivLen < 1)
		return RES_TLS_INVALID_PARAMETER;

	//Check the length of the authentication tag
	if (tLen < 4 || tLen > 16)
		return RES_TLS_INVALID_PARAMETER;

	//Check whether the length of the IV is 96 bits
	if (ivLen == 12)
	{
		//When the length of the IV is 96 bits, the padding string is
		//appended to the IV to form the pre-counter block
		memcpy(j.as_byte, iv, 12);
		j.as_word[3] = __REV(1);
	}
	else
	{
		//Initialize GHASH calculation
		j.as_word[0] = 0;
		j.as_word[1] = 0;
		j.as_word[2] = 0;
		j.as_word[3] = 0;

		//Length of the IV
		n = ivLen;

		//Process the initialization vector
		while (n > 0)
		{
			//The IV processed in a block-by-block fashion
			k = min(n, 16u);

			//Apply GHASH function
			gcmXorBlock(j.as_byte, j.as_byte, iv, k);
			gcm_mul(gcm_ctxt.get(), j);

			//Next block
			iv += k;
			n -= k;
		}

		//The string is appended with 64 additional 0 bits, followed by the
		//64-bit representation of the length of the IV
		b.as_word[0] = 0;
		b.as_word[1] = 0;
		b.as_word[2] = 0;
		b.as_word[3] = __REV(ivLen * 8);

		//The GHASH function is applied to the resulting string to form the
		//pre-counter block
		gcmXorBlock(j, j, b);
		gcm_mul(gcm_ctxt.get(), j);
	}

	//Compute MSB(CIPH(J(0)))
	encrypt(j.as_byte, b.as_byte, 1);
	memcpy(t, b.as_byte, tLen);

	//Initialize GHASH calculation
	s.as_word[0] = 0;
	s.as_word[1] = 0;
	s.as_word[2] = 0;
	s.as_word[3] = 0;
	//Length of the AAD
	n = aLen;

	//Process AAD
	while (n > 0)
	{
		//Additional data are processed in a block-by-block fashion
		k = min(n, 16u);

		//Apply GHASH function
		gcmXorBlock(s.as_byte, s.as_byte, a, k);
		gcm_mul(gcm_ctxt.get(), s);

		//Next block
		a += k;
		n -= k;
	}

	//Length of the plaintext
	n = len;

	//Process plaintext
	while (n > 0)
	{
		//The encryption operates in a block-by-block fashion
        for( k = 15; k > 11; k-- )
        {
            if( ++(j.as_byte[k]) != 0 )
                break;
        }
		k = min(n, 16u);


		//Encrypt plaintext
		encrypt(j.as_byte, b.as_byte, 1);
		gcmXorBlock(data, data, b.as_byte, k);

		//Apply GHASH function
		gcmXorBlock(s.as_byte, s.as_byte, data, k);
		gcm_mul(gcm_ctxt.get(), s);

		//Next block
		data += k;
		n -= k;
	}

	//Append the 64-bit representation of the length of the AAD and the ciphertext
	b.as_word[0] = 0;
	b.as_word[1] = __REV(aLen * 8);
	b.as_word[2] = 0;
	b.as_word[3] = __REV(len * 8);

	//The GHASH function is applied to the result to produce a single output block S
	gcmXorBlock(s, s, b);
	gcm_mul(gcm_ctxt.get(), s);

	//Let T = MSB(GCTR(J(0), S)
	gcmXorBlock(t, t, s.as_byte, tLen);

	//Successful encryption
	return RES_OK;
}

RES_CODE cipher_algo_t::gcm_decrypt(const uint8_t *iv, size_t ivLen, const uint8_t *a,
		size_t aLen, const uint8_t* c, uint8_t* p, size_t len, uint8_t *t,	size_t tLen)
{
	size_t k;
	size_t n;
	cbc_buf_t b;
	cbc_buf_t j;
	cbc_buf_t r;
	cbc_buf_t s;

	///Make sure the GCM context is valid
	if (gcm_ctxt.get() == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//The length of the IV shall meet SP 800-38D requirements
	if (ivLen < 1)
		return RES_TLS_INVALID_PARAMETER;

	//Check the length of the authentication tag
	if (tLen < 4 || tLen > 16)
		return RES_TLS_INVALID_PARAMETER;

	//Check whether the length of the IV is 96 bits
	if (ivLen == 12)
	{
		//When the length of the IV is 96 bits, the padding string is
		//appended to the IV to form the pre-counter block
		memcpy(j.as_byte, iv, 12);
		j.as_word[3] = __REV(1);
	}
	else
	{
		//Initialize GHASH calculation
		j.as_word[0] = 0;
		j.as_word[1] = 0;
		j.as_word[2] = 0;
		j.as_word[3] = 0;

		//Length of the IV
		n = ivLen;

		//Process the initialization vector
		while (n > 0)
		{
			//The IV processed in a block-by-block fashion
			k = min(n, 16u);

			//Apply GHASH function
			gcmXorBlock(j.as_byte, j.as_byte, iv, k);
			gcm_mul(gcm_ctxt.get(), j);

			//Next block
			iv += k;
			n -= k;
		}

		//The string is appended with 64 additional 0 bits, followed by the
		//64-bit representation of the length of the IV
		b.as_word[0] = 0;
		b.as_word[1] = 0;
		b.as_word[2] = 0;
		b.as_word[3] = __REV(ivLen * 8);

		//The GHASH function is applied to the resulting string to form the
		//pre-counter block
		gcmXorBlock(j, j, b);
		gcm_mul(gcm_ctxt.get(), j);
	}

	//Compute MSB(CIPH(J(0)))
	encrypt(j.as_byte, b.as_byte, 1);
	memcpy(r.as_byte, b.as_byte, tLen);

	//Initialize GHASH calculation
	s.as_word[0] = 0;
	s.as_word[1] = 0;
	s.as_word[2] = 0;
	s.as_word[3] = 0;
	//Length of the AAD
	n = aLen;

	//Process AAD
	while (n > 0)
	{
		//Additional data are processed in a block-by-block fashion
		k = min(n, 16u);

		//Apply GHASH function
		gcmXorBlock(s.as_byte, s.as_byte, a, k);
		gcm_mul(gcm_ctxt.get(), s);

		//Next block
		a += k;
		n -= k;
	}

	//Length of the ciphertext
	n = len;

	//Process ciphertext
	while (n > 0)
	{
		//The decryption operates in a block-by-block fashion
		for (k = 15; k > 11; k--)
			if (++(j.as_byte[k]) != 0)
				break;

		k = min(n, 16u);

		//Apply GHASH function
		gcmXorBlock(s.as_byte, s.as_byte, c, k);
		gcm_mul(gcm_ctxt.get(), s);

		//Decrypt ciphertext
		encrypt(j.as_byte, b.as_byte, 1);
		gcmXorBlock(p, c, b.as_byte, k);

		//Next block
		c += k;
		p += k;
		n -= k;
	}

	//Append the 64-bit representation of the length of the AAD and the ciphertext
	b.as_word[0] = 0;
	b.as_word[1] = __REV(aLen * 8);
	b.as_word[2] = 0;
	b.as_word[3] = __REV(len * 8);

	//The GHASH function is applied to the result to produce a single output block S
	gcmXorBlock(s, s, b);
	gcm_mul(gcm_ctxt.get(), s);

	//Let R = MSB(GCTR(J(0), S)
	gcmXorBlock(r.as_byte, r.as_byte, s.as_byte, tLen);

	//The calculated tag is bitwise compared to the received tag. The
	//message is authenticated if and only if the tags match
	if (memcmp(r.as_byte, t, tLen))
		return RES_TLS_FAIL;

	//Successful decryption
	return RES_OK;
}
#endif


