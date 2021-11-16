/*
 * rsa.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_RSA_H_
#define SERVICES_SECURITY_CRYPTO_RSA_H_

#include <tmos.h>
#include <crypto.h>
#include <mpi.h>

//RSA related constants
extern const uint8_t PKCS1_OID[8];
extern const uint8_t RSA_ENCRYPTION_OID[9];
extern const uint8_t MD5_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA1_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA256_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA384_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t SHA512_WITH_RSA_ENCRYPTION_OID[9];
extern const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_224_OID[9];
extern const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_256_OID[9];
extern const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_384_OID[9];
extern const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_512_OID[9];


struct RsaPublicKey
{
	Mpi n; //!< Modulus
	Mpi e; //!< Public exponent

	void rspk_free();

	RES_CODE rsaep(const Mpi* m, Mpi* c) const;

	RES_CODE rsavp1(const Mpi* s, Mpi* m) const;

	RES_CODE rsassaPkcs1v15Verify(const hash_info_t* hinfo, const uint8_t* digest,
			const uint8_t* signature, size_t signatureLength) const;

	RES_CODE rsaesPkcs1v15Encrypt(prng_algo_t* prngAlgo, const uint8_t* message,
			size_t messageLength, uint8_t* ciphertext,
			size_t* ciphertextLength) const;
};

struct RsaPrivateKey
{
	Mpi n;    //!< Modulus
	Mpi e;    //!< Public exponent
	Mpi d;    //!< Private exponent
	Mpi p;    //!< First factor
	Mpi q;    //!< Second factor
	Mpi dp;   //!< First factor's CRT exponent
	Mpi dq;   //!< second factor's CRT exponent
	Mpi qinv; //!< CRT coefficient

	RES_CODE rsadp(const Mpi* c, Mpi* m) const;

	RES_CODE rsasp1(const Mpi* m, Mpi* s) const;

	RES_CODE tlsGenerateRsaSignature(const uint8_t* digest, uint8_t* signature,
			size_t signatureLength)	const;

	RES_CODE rsassaPkcs1v15Sign(const hash_info_t* hash, const uint8_t* digest,
			uint8_t* signature, size_t signatureLength) const;

	RES_CODE rsaesPkcs1v15Decrypt(const uint8_t* ciphertext,
			size_t ciphertextLength, uint8_t* message, size_t messageSize,
			size_t* messageLength) const;

};

RES_CODE emsaPkcs1v15Decode(const uint8_t *em, size_t emLength, const uint8_t **oid,
   size_t *oidLength, const uint8_t **digest, size_t *digestLength);

RES_CODE emsaPkcs1v15Encode(const hash_info_t* hash, const uint8_t* digest,
		uint8_t* em, size_t emLength);

RES_CODE rsaesOaepEncrypt(prng_algo_t* prngAlgo, const RsaPublicKey* key,
		const hash_info_t* oaep_hinfo, const hash_info_t* mfg_hinfo, const char* label,
		const uint8_t* message, size_t messageLen, uint8_t* ciphertext,
		size_t* ciphertextLen);

RES_CODE rsaesOaepDecrypt(const RsaPrivateKey *key,
		const hash_info_t *oaep_hinfo, const hash_info_t *mfg_hinfo,
		const char *label, const uint8_t *ciphertext, size_t ciphertextLen,
		uint8_t *message, size_t messageSize, size_t *messageLen);

#endif /* SERVICES_SECURITY_CRYPTO_RSA_H_ */
