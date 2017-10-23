/*
 * ecdsa.h
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_ECDSA_H_
#define SERVICES_SECURITY_CRYPTO_ECDSA_H_
#include <tmos.h>
#include <ec.h>
#include <dsa.h>

//ECDSA related constants
extern const uint8_t ECDSA_WITH_SHA1_OID[7];
extern const uint8_t ECDSA_WITH_SHA224_OID[8];
extern const uint8_t ECDSA_WITH_SHA256_OID[8];
extern const uint8_t ECDSA_WITH_SHA384_OID[8];
extern const uint8_t ECDSA_WITH_SHA512_OID[8];
extern const uint8_t ECDSA_WITH_SHA3_224_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_256_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_384_OID[9];
extern const uint8_t ECDSA_WITH_SHA3_512_OID[9];


/// ECDSA signature

struct EcdsaSignature : public DsaSignature
{

	RES_CODE ecdsaVerifySignature(const EcDomainParameters* params, const EcPoint* publicKey,
	   const uint8_t* digest, size_t digestLength) const;

	RES_CODE ecdsaGenerateSignature(const EcDomainParameters* params,
			prng_algo_t* prngAlgo, const Mpi* privateKey, const uint8_t* digest,
			size_t digestLength);

	RES_CODE ecdsaWriteSignatureLen(size_t *length) const;
	RES_CODE ecdsaWriteSignature(uint8_t* data, size_t* length) const;

};

#endif /* SERVICES_SECURITY_CRYPTO_ECDSA_H_ */
