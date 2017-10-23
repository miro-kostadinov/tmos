/*
 * chacha20_poly1305.h
 *
 *  Created on: Jun 16, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_CHACHA20_POLY1305_H_
#define SERVICES_SECURITY_CRYPTO_CHACHA20_POLY1305_H_

#include <tmos.h>
#include <crypto.h>

#if TLS_CHACHA20_POLY1305_SUPPORT

//ChaCha20Poly1305 related functions
RES_CODE chacha20Poly1305Encrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   uint8_t* data, size_t len, uint8_t *t, size_t tLen);

RES_CODE chacha20Poly1305Decrypt(const uint8_t *k, size_t kLen,
   const uint8_t *n, size_t nLen, const uint8_t *a, size_t aLen,
   uint8_t* data, size_t len, const uint8_t *t, size_t tLen);

#endif // TLS_CHACHA20_POLY1305_SUPPORT


#endif /* SERVICES_SECURITY_CRYPTO_CHACHA20_POLY1305_H_ */
