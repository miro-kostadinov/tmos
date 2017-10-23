/*
 * aes.cpp
 *
 *  Created on: Aug 29, 2017
 *      Author: miro
 */

#include <tmos.h>
#include <aes.h>


#if AES_SUPPORT

const cipher_info_t aes_cipher_info =
{
   "AES",
   AES_BLOCK_SIZE,
   CIPHER_TYPE_BLOCK
};


cipher_algo_t* new_cipher_aes()
{
	cipher_algo_t* algo;

	algo = new aes_cipher_algo_t(&aes_cipher_info);
	return algo;
}


RES_CODE aes_cipher_algo_t::init_key(const uint8_t* key, size_t key_size)
{
	//Check key length
	switch (key_size)
	{
	case 16:	// 128-bit key
	case 24:	// 192-bit key
	case 32:	// 256-bit key
		break;

	default:
		return RES_TLS_INVALID_KEY_LENGTH;
	}

	en_ctx.Sb = aes_ForwardSBox;
	en_ctx.FT = aes_ForwardTable;
	en_ctx.RSb= aes_ReverseSBox;
	en_ctx.RT = aes_ReverseTable;
	de_ctx.Sb = aes_ForwardSBox;
	de_ctx.FT = aes_ForwardTable;
	de_ctx.RSb= aes_ReverseSBox;
	de_ctx.RT = aes_ReverseTable;

	aes_tmos_en_key(&en_ctx, key, key_size/4);
	aes_tmos_de_key(&de_ctx, key, key_size/4);
	return RES_OK;
}

void aes_cipher_algo_t::encrypt(const void* input, void* output, size_t blocks)
{
	aes_tmos_en_ecb(&en_ctx, (const unsigned char*)input, (unsigned char*)output, blocks);
}
void aes_cipher_algo_t::decrypt(const void* input, void* output, size_t blocks)
{
	aes_tmos_de_ecb(&de_ctx, (const unsigned char*)input, (unsigned char*)output, blocks);
};

#endif // AES_SUPPORT
