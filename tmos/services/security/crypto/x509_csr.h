/*
 * x509_csr.h
 *
 *  Created on: 15 Sept 2023
 *      Author: miro
 */

#ifndef SERVICES_SECURITY_CRYPTO_X509_CSR_H_
#define SERVICES_SECURITY_CRYPTO_X509_CSR_H_


#include <tmos.h>
#include <x509.h>


/**
 * @brief PKCS#9 ChallengePassword attribute
 **/

struct X509ChallengePassword
{
   const char*	value;
   size_t 		length;

   RES_CODE x509FormatChallengePassword(uint8_t *output, size_t *written) const;
};


///**
// * @brief CSR attribute
// **/
//
//struct X509Attribute
//{
//   const uint8_t*	oid;
//   size_t 			oidLen;
//   const uint8_t*	value;
//   size_t 			valueLen;
//};


/**
 * @brief CSR attributes
 **/

struct X509Attributes
{
   const uint8_t*	rawData;
   size_t 			rawDataLen;
   X509ChallengePassword challengePwd;
   X509Extensions   extensionReq;

   RES_CODE x509FormatAttributes(uint8_t *output, size_t *written) const;
};


/**
 * @brief RSASSA-PSS parameters
 **/

struct X509RsaPssParameters
{
   const uint8_t*	hashAlgo;
   size_t 			hashAlgoLen;
   const uint8_t*	maskGenAlgo;
   size_t 			maskGenAlgoLen;
   const uint8_t*	maskGenHashAlgo;
   size_t 			maskGenHashAlgoLen;
   size_t 			saltLen;

   RES_CODE x509FormatRsaPssParameters(uint8_t* output, size_t* written) const;
   RES_CODE x509FormatRsaPssHashAlgo(uint8_t* output, size_t* written) const;
   RES_CODE x509FormatRsaPssMaskGenAlgo(uint8_t* output, size_t* written) const;
   RES_CODE x509FormatRsaPssSaltLength(uint8_t* output, size_t* written) const;
   RES_CODE x509FormatRsaPssMaskGenHashAlgo(uint8_t* output, size_t* written) const;
};


/**
 * @brief Signature algorithm identifier
 **/

struct X509SignatureAlgoId
{
	const uint8_t *sa_oid;
	size_t sa_oidLen;
#if X509_RSA_PSS_SUPPORT && RSA_SUPPORT
	X509RsaPssParameters rsaPssParams;
#endif

	RES_CODE x509FormatSignatureAlgo(uint8_t *output, size_t *written) const;
	RES_CODE x509FormatSignatureValue(prng_algo_t *prngAlgo,
			const uint8_t *tbsCert, size_t tbsCertLen,
			const X509SubjectPublicKeyInfo *publicKeyInfo,
			const void *privateKey, uint8_t *output, size_t *written) const;
	RES_CODE x509GetSignHashAlgo(X509SignatureAlgo *signAlgo,
			const hash_info_t **hashAlgo) const;
	RES_CODE x509GenerateSignature(prng_algo_t *prngAlgo,
			const uint8_t *tbsCert, size_t tbsCertLen,
			const X509SubjectPublicKeyInfo *publicKeyInfo,
			const void *privateKey, uint8_t *output, size_t *written) const;

};

/**
 * @brief CertificationRequestInfo structure
 **/

struct X509CertRequestInfo
{
   uint8_t*		 	rawData;
   size_t 			rawDataLen;
   X509Version 		version;
   X509Name 		subject;
   X509SubjectPublicKeyInfo subjectPublicKeyInfo;
   X509Attributes 	attributes;

	RES_CODE x509CreateCsr_der(prng_algo_t* prngAlgo, const void* subjectPublicKey,
			const X509SignatureAlgoId* signatureAlgo, const void* signerPrivateKey);
	RES_CODE x509CreateCsr_pem(prng_algo_t* prngAlgo, const void* subjectPublicKey,
			const X509SignatureAlgoId* signatureAlgo, const void* signerPrivateKey);
	RES_CODE x509CreateCsr_data(prng_algo_t* prngAlgo, const void* subjectPublicKey,
			const X509SignatureAlgoId* signatureAlgo, const void* signerPrivateKey);

	RES_CODE x509FormatCertRequestInfo(const void* publicKey, uint8_t* output,
			size_t* written) const;
};



#endif /* SERVICES_SECURITY_CRYPTO_X509_CSR_H_ */
