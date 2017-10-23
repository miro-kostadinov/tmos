/*
 * idea.cpp
 *
 *  Created on: Aug 29, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <idea.h>

#if IDEA_SUPPORT

const cipher_info_t idea_cipher_info =
{
   "IDEA",
   IDEA_BLOCK_SIZE,
   CIPHER_TYPE_BLOCK
};

cipher_algo_t* new_cipher_idea()
{
	cipher_algo_t* algo;

	algo = new idea_cipher_algo_t(&idea_cipher_info);
	return algo;
}

static uint16_t ideaInv(uint16_t a)
{
   uint32_t b;
   uint32_t q;
   uint32_t r;
   int32_t t;
   int32_t u;
   int32_t v;

   b = 0x10001;
   u = 0;
   v = 1;

   while(a > 0)
   {
      q = b / a;
      r = b % a;

      b = a;
      a = r;

      t = v;
      v = u - q * v;
      u = t;
   }

   if(u < 0)
      u += 0x10001;

   return u;
}

static uint16_t ideaMul(uint16_t a, uint16_t b)
{
   uint32_t c = a * b;

   if(c)
   {
      c = (ROL32(c, 16) - c) >> 16;
      return (c + 1) & 0xFFFF;
   }
   else
   {
      return (1 - a - b) & 0xFFFF;
   }
}

RES_CODE idea_cipher_algo_t::init_key(const uint8_t* key, size_t key_size)
{
	uint32_t i;

	//Invalid key length?
	if (key_size != 16)
		return RES_TLS_INVALID_KEY_LENGTH;


	//First, the 128-bit key is partitioned into eight 16-bit sub-blocks
	for (i = 0; i < 8; i++)
		ek[i] = __REV16(*(uint16_t*)(void*)(key + i * 2));

	//Expand encryption subkeys
	for (i = 8; i < 52; i++)
	{
		if ((i % 8) == 6)
			ek[i] = (ek[i - 7] << 9) | (ek[i - 14] >> 7);
		else if ((i % 8) == 7)
			ek[i] = (ek[i - 15] << 9) | (ek[i - 14] >> 7);
		else
			ek[i] = (ek[i - 7] << 9) | (ek[i - 6] >> 7);
	}

	//Generate subkeys for decryption
	for (i = 0; i < 52; i += 6)
	{
		dk[i] = ideaInv(ek[48 - i]);

		if (i == 0 || i == 48)
		{
			dk[i + 1] = -ek[49 - i];
			dk[i + 2] = -ek[50 - i];
		}
		else
		{
			dk[i + 1] = -ek[50 - i];
			dk[i + 2] = -ek[49 - i];
		}

		dk[i + 3] = ideaInv(ek[51 - i]);

		if (i < 48)
		{
			dk[i + 4] = ek[46 - i];
			dk[i + 5] = ek[47 - i];
		}
	}

	return RES_OK;
};

static void crypt_block(uint16_t* k, const uint16_t* input, uint16_t* output)
{
	uint32_t i;
	uint16_t e;
	uint16_t f;

	//The plaintext is divided into four 16-bit registers
	uint16_t a = __REV16(input[0]);
	uint16_t b = __REV16(input[1]);
	uint16_t c = __REV16(input[2]);
	uint16_t d = __REV16(input[3]);


	//The process consists of eight identical encryption steps
	for (i = 0; i < 8; i++)
	{
		//Apply a round
		a = ideaMul(a, k[0]);
		b += k[1];
		c += k[2];
		d = ideaMul(d, k[3]);

		e = a ^ c;
		f = b ^ d;

		e = ideaMul(e, k[4]);
		f += e;
		f = ideaMul(f, k[5]);
		e += f;

		a ^= f;
		d ^= e;
		e ^= b;
		f ^= c;

		b = f;
		c = e;

		//Advance current location in key schedule
		k += 6;
	}

	//The four 16-bit values produced at the end of the 8th encryption
	//round are combined with the last four of the 52 key sub-blocks
	a = ideaMul(a, k[0]);
	c += k[1];
	b += k[2];
	d = ideaMul(d, k[3]);

	//The resulting value is the ciphertext
	output[0] = __REV16(a);
	output[1] = __REV16(c);
	output[2] = __REV16(b);
	output[3] = __REV16(d);
}

void idea_cipher_algo_t::encrypt(const void* input, void* output, size_t blocks)
{
	while(blocks--)
	{
		crypt_block(ek, (const uint16_t*)input, (uint16_t*)output);
		input = (const uint8_t*)input + IDEA_BLOCK_SIZE;
		output = (uint8_t*)output + IDEA_BLOCK_SIZE;
	}
}

void idea_cipher_algo_t::decrypt(const void* input, void* output, size_t blocks)
{
	while(blocks--)
	{
		crypt_block(dk, (const uint16_t*)input, (uint16_t*)output);
		input = (const uint8_t*)input + IDEA_BLOCK_SIZE;
		output = (uint8_t*)output + IDEA_BLOCK_SIZE;
	}
}



#endif // IDEA_SUPPORT
