/*
 * poly1305.h
 *
 *  Created on: Jun 19, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_POLY1305_H_
#define SERVICES_SECURITY_CRYPTO_POLY1305_H_

#include <tmos.h>
#include <crypto.h>


#if POLY1305_SUPPORT

struct poly1305_ctxt_t
{
   uint32_t acc[5];
   uint32_t r[4];
   uint32_t s[4];

   size_t size;
   uint8_t buffer[16];


   void Reset(const uint32_t *key);
   void Input(const void *data, size_t len);
   void Result(uint8_t *tag);

};



#endif // POLY1305_SUPPORT


#endif /* SERVICES_SECURITY_CRYPTO_POLY1305_H_ */
