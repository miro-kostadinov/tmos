/*
 * chacha20_poly1305.cpp
 *
 *  Created on: Jun 16, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <crypto.h>
#include <tls.h>
#include <poly1305.h>
#include <chacha20_poly1305.h>
#include <chacha.h>

#if TLS_CHACHA20_POLY1305_SUPPORT

RES_CODE chacha20Poly1305Encrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   uint8_t* data, size_t len, uint8_t *t, size_t tLen)
{
	RES_CODE res;
	size_t paddingLen;
	chacha_t chacha;
	poly1305_ctxt_t poly1305;
	uint32_t temp[8];

	//Check the length of the message-authentication code
	if (tLen != 16)
		return RES_TLS_INVALID_PARAMETER;

	//Initialize ChaCha20 context
	res = chacha.chacha_init(20, k, kLen, n, nLen);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//First, a Poly1305 one-time key is generated from the 256-bit key
	//and nonce
	chacha.chacha_cipher(NULL, (uint8_t *)temp, 32);

	//The other 256 bits of the Chacha20 block are discarded
	chacha.chacha_cipher(NULL, NULL, 32);

	//Next, the ChaCha20 encryption function is called to encrypt the
	//plaintext, using the same key and nonce
	chacha.chacha_cipher(data, data, len);

	//Initialize the Poly1305 function with the key calculated above
	poly1305.Reset((const uint32_t *)temp);

	//Compute MAC over the AAD
	poly1305.Input(a, aLen);

	//If the length of the AAD is not an integral multiple of 16 bytes,
	//then padding is required
	if ((aLen % 16) != 0)
	{
		//Compute the number of padding bytes
		paddingLen = 16 - (aLen % 16);

		//The padding is up to 15 zero bytes, and it brings the total
		//length so far to an integral multiple of 16
		memset(temp, 0, paddingLen);

		//Compute MAC over the padding
		poly1305.Input(temp, paddingLen);
	}

	//Compute MAC over the ciphertext
	poly1305.Input(data, len);

	//If the length of the ciphertext is not an integral multiple of 16 bytes,
	//then padding is required
	if ((len % 16) != 0)
	{
		//Compute the number of padding bytes
		paddingLen = 16 - (len % 16);

		//The padding is up to 15 zero bytes, and it brings the total
		//length so far to an integral multiple of 16
		memset(temp, 0, paddingLen);

		//Compute MAC over the padding
		poly1305.Input(temp, paddingLen);
	}

	//Encode the length of the AAD as a 64-bit little-endian integer
	temp[0] = aLen;
	temp[1] = 0;
	//Compute MAC over the length field
	poly1305.Input(temp, sizeof(uint64_t));

	//Encode the length of the ciphertext as a 64-bit little-endian integer
	temp[0] = len;
	//Compute MAC over the length field
	poly1305.Input(temp, sizeof(uint64_t));

	//Compute message-authentication code
	poly1305.Result(t);

	//Successful encryption
	return RES_OK;
}

RES_CODE chacha20Poly1305Decrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   uint8_t* data, size_t len, const uint8_t *t, size_t tLen)
{
	RES_CODE res;
	size_t paddingLen;
	chacha_t chacha;
	poly1305_ctxt_t poly1305;
	uint32_t temp[8];

	//Check the length of the message-authentication code
	if (tLen != 16)
		return RES_TLS_INVALID_PARAMETER;

	//Initialize ChaCha20 context
	res = chacha.chacha_init(20, k, kLen, n, nLen);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//First, a Poly1305 one-time key is generated from the 256-bit key
	//and nonce
	chacha.chacha_cipher(NULL, (uint8_t *)temp, 32);

	//The other 256 bits of the Chacha20 block are discarded
	chacha.chacha_cipher(NULL, NULL, 32);

	//Initialize the Poly1305 function with the key calculated above
	poly1305.Reset((const uint32_t *)temp);

	//Compute MAC over the AAD
	poly1305.Input(a, aLen);

	//If the length of the AAD is not an integral multiple of 16 bytes,
	//then padding is required
	if ((aLen % 16) != 0)
	{
		//Compute the number of padding bytes
		paddingLen = 16 - (aLen % 16);

		//The padding is up to 15 zero bytes, and it brings the total
		//length so far to an integral multiple of 16
		memset(temp, 0, paddingLen);

		//Compute MAC over the padding
		poly1305.Input(temp, paddingLen);
	}

	//Compute MAC over the ciphertext
	poly1305.Input(data, len);

	//If the length of the ciphertext is not an integral multiple of 16 bytes,
	//then padding is required
	if ((len % 16) != 0)
	{
		//Compute the number of padding bytes
		paddingLen = 16 - (len % 16);

		//The padding is up to 15 zero bytes, and it brings the total
		//length so far to an integral multiple of 16
		memset(temp, 0, paddingLen);

		//Compute MAC over the padding
		poly1305.Input(temp, paddingLen);
	}

	//Encode the length of the AAD as a 64-bit little-endian integer
	temp[0] = aLen;
	temp[1] = 0;
	//Compute MAC over the length field
	poly1305.Input(temp, sizeof(uint64_t));

	//Encode the length of the ciphertext as a 64-bit little-endian integer
	temp[0] = len;
	//Compute MAC over the length field
	poly1305.Input(temp, sizeof(uint64_t));

	//Compute message-authentication code
	poly1305.Result((uint8_t *)temp);

	//Finally, we decrypt the ciphertext
	chacha.chacha_cipher(data, data, len);

	//The calculated tag is bitwise compared to the received tag. The
	//message is authenticated if and only if the tags match
	if (memcmp(temp, t, tLen))
		return RES_TLS_FAIL;

	//Successful encryption
	return RES_OK;
}

#endif // TLS_CHACHA20_POLY1305_SUPPORT
