/*
 * chacha.h
 *
 *  Created on: Jun 16, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_CHACHA_H_
#define SERVICES_SECURITY_CRYPTO_CHACHA_H_

#include <tmos.h>
#include <crypto.h>


struct chacha_t
{
   uint32_t cha_nr;
   uint32_t state[16];
   uint32_t block[16];
   size_t pos;

   //ChaCha related functions
   RES_CODE chacha_init(uint32_t nr, const uint8_t* key,
      size_t keyLength, const uint8_t* nonce, size_t nonceLen);

   void chacha_cipher(const uint8_t *input, uint8_t *output, size_t len);

   void chacha_process_block();
};



#endif /* SERVICES_SECURITY_CRYPTO_CHACHA_H_ */
