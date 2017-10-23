/*
 * dsa.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_DSA_H_
#define SERVICES_SECURITY_CRYPTO_DSA_H_

#include <tmos.h>
#include <crypto.h>
#include <mpi.h>

//DSA related constants
extern const uint8_t DSA_OID[7];
extern const uint8_t DSA_WITH_SHA1_OID[7];
extern const uint8_t DSA_WITH_SHA224_OID[9];
extern const uint8_t DSA_WITH_SHA256_OID[9];
extern const uint8_t DSA_WITH_SHA384_OID[9];
extern const uint8_t DSA_WITH_SHA512_OID[9];
extern const uint8_t DSA_WITH_SHA3_224_OID[9];
extern const uint8_t DSA_WITH_SHA3_256_OID[9];
extern const uint8_t DSA_WITH_SHA3_384_OID[9];
extern const uint8_t DSA_WITH_SHA3_512_OID[9];


/// DSA public key
struct DsaPublicKey
{
   Mpi p; ///<Prime modulus
   Mpi q; ///<Prime divisor
   Mpi g; ///<Generator of the subgroup
   Mpi y; ///<Public key

   void dsapk_free();
};

/// DSA private key
struct DsaPrivateKey
{
   Mpi p; ///<Prime modulus
   Mpi q; ///<<Prime divisor
   Mpi g; ///<Generator of the subgroup
   Mpi x; ///<Private key
   void dsapk_free();
};

/// DSA signature
struct DsaSignature
{
   Mpi r;
   Mpi s;

   RES_CODE dsaWriteSignatureLen(size_t *length) const;

   RES_CODE dsaReadSignature(const uint8_t *data, size_t length);
   RES_CODE dsaWriteSignature(uint8_t* data, size_t* length) const;

   RES_CODE dsaGenerateSignature(prng_algo_t* prngAlgo,
      const DsaPrivateKey* key, const uint8_t* digest, size_t digestLength);

   RES_CODE dsaVerifySignature(const DsaPublicKey *key, const uint8_t *digest,
		   size_t digestLength) const;
};


#endif /* SERVICES_SECURITY_CRYPTO_DSA_H_ */
