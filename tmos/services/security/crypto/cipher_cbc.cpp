/*
 * cipher_cbc.cpp
 *
 *  Created on: Jun 15, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <crypto.h>
#include <tls.h>


#if CBC_SUPPORT

RES_CODE cipher_algo_t::cbc_encrypt(uint8_t* iv, uint8_t* data, size_t len)
{
   size_t i;
   uint8_t* old_data = iv;
   uint32_t block_size = algo_info->block_size;

   //CBC mode operates in a block-by-block fashion
   while(len >= block_size)
   {
	  for(i = 0; i < block_size; i++)
		  data[i] ^= old_data[i];

	  encrypt(data, data, 1);

	  old_data = data;
	  data += block_size;
	  len -= block_size;
   }

   if(len != 0)
	  return RES_TLS_INVALID_PARAMETER;

   memcpy(iv, old_data, block_size);

   return RES_OK;
}

RES_CODE cipher_algo_t::cbc_decrypt(uint8_t* iv, const uint8_t* c, uint8_t* p, size_t len)
{
	size_t i, temp;
	uint8_t t[16];
	uint32_t block_size = algo_info->block_size;

	while (len >= block_size)
	{
		decrypt(c, t, 1);

		for (i = 0; i < block_size; i++)
		{
			temp = iv[i];
			iv[i] = c[i];
			p[i] = t[i] ^ temp;
		}

		c += block_size;
		p += block_size;
		len -= block_size;
	}

	if (len != 0)
		return RES_TLS_INVALID_PARAMETER;

	return RES_OK;
}



#endif

