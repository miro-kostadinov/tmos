/*
 * pem.h
 *
 *  Created on: Aug 1, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_PEM_H_
#define SERVICES_SECURITY_CRYPTO_PEM_H_

#include <tmos.h>
#include <crypto.h>
#include <ec.h>
#include <rsa.h>
#include <dsa.h>

RES_CODE pemReadCertificate(const char** input, size_t* inputLength,
   uint8_t** output, size_t* outputSize, size_t* outputLength);

RES_CODE pemReadRsaPrivateKey(const char* input, size_t length, RsaPrivateKey* key);
RES_CODE pemReadDsaPrivateKey(const char* input, size_t length, DsaPrivateKey* key);

RES_CODE pemReadEcParameters(const char* input, size_t length, EcDomainParameters* params);
RES_CODE pemReadEcPrivateKey(const char* input, size_t length, Mpi* key);

RES_CODE pemEncodeFile(const void* input, size_t inputLen, const char* label,
   char* output, size_t* outputLen);

#endif /* SERVICES_SECURITY_CRYPTO_PEM_H_ */
