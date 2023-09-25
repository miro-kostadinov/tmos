/*
 * x509_csr.cpp
 *
 *  Created on: 15 Sept 2023
 *      Author: miro
 */

#include <tmos.h>
#include <x509_csr.h>
#include <asn1.h>
#include <hmac.h>
#include <oid.h>
#include <ecdsa.h>


/**
 * @brief Format ChallengePassword attribute
 * @param[in] challengePwd Value of the attribute
 * @param[out] output Buffer where to format the ASN.1 structure
 * @param[out] written Length of the resulting ASN.1 structure
 * @return Error code
 **/

RES_CODE X509ChallengePassword::x509FormatChallengePassword(uint8_t *output, size_t *written) const
{
	RES_CODE res;
	size_t n;
	size_t len;
	uint8_t *p;
	Asn1Tag tag;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	len = 0;

	//Valid challenge password?
	if (value != nullptr && length > 0)
	{
		//Format attribute identifier
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_OBJECT_IDENTIFIER;
		tag.length = sizeof(X509_CHALLENGE_PASSWORD_OID);
		tag.value = X509_CHALLENGE_PASSWORD_OID;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, p, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Advance data pointer
		p += n;
		len += n;

		//Format challenge password
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_UTF8_STRING;
		tag.length = length;
		tag.value = (uint8_t*) value;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, p, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Attribute value is encapsulated within a set
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_SET;
		tag.length = n;
		tag.value = p;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, p, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//The attribute is encapsulated within a sequence
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_SEQUENCE;
		tag.length = len + n;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &len);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Total number of bytes that have been written
	*written = len;

	//Successful processing
	return RES_OK;
}

RES_CODE X509Attributes::x509FormatAttributes(uint8_t *output, size_t *written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t* p;
	Asn1Tag tag;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	length = 0;

	//Format PKCS#9 Challenge Password attribute
	res = challengePwd.x509FormatChallengePassword(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format PKCS#9 Extension Request attribute
	res = extensionReq.x509FormatExtensionRequest(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Explicit tagging shall be used to encode the Extensions structure
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_CONTEXT_SPECIFIC;
	tag.objType = (Asn1Type)0;
	tag.length = length;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &length);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = length;

	//Successful processing
	return RES_OK;
}

RES_CODE X509RsaPssParameters::x509FormatRsaPssParameters(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t *p;
	Asn1Tag tag;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	length = 0;

	//Format hashAlgorithm parameter
	res = x509FormatRsaPssHashAlgo(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format maskGenAlgorithm parameter
	res = x509FormatRsaPssMaskGenAlgo(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format saltLength parameter
	res = x509FormatRsaPssSaltLength(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//The RSASSA-PSS parameters are encapsulated within a sequence
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_SEQUENCE;
	tag.length = length;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}

RES_CODE X509RsaPssParameters::x509FormatRsaPssHashAlgo(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Length of the ASN.1 structure
	n = 0;

	//The default hash algorithm is SHA-1
	if (hashAlgo != nullptr && hashAlgoLen > 0)
	{
		//Write the hash algorithm identifier
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_OBJECT_IDENTIFIER;
		tag.length = hashAlgoLen;
		tag.value = hashAlgo;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//The hashAlgorithm parameter is encapsulated within a sequence
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_SEQUENCE;
		tag.length = n;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Explicit tagging shall be used to encode each parameter
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_CONTEXT_SPECIFIC;
		tag.objType = (Asn1Type)0;
		tag.length = n;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}

RES_CODE X509RsaPssParameters::x509FormatRsaPssMaskGenAlgo(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t *p;
	Asn1Tag tag;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	length = 0;

	//The default mask generation function is MGF1
	if (maskGenAlgo != nullptr && maskGenAlgoLen > 0)
	{
		//Write the mask generation algorithm identifier
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_OBJECT_IDENTIFIER;
		tag.length = maskGenAlgoLen;
		tag.value = maskGenAlgo;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, p, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Advance data pointer
		p += n;
		length += n;

		//Write the algorithm identifier of the one-way hash function employed
		//with the mask generation function
		res = x509FormatRsaPssMaskGenHashAlgo(p, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Advance data pointer
		p += n;
		length += n;

		//The maskGenAlgorithm parameter is encapsulated within a sequence
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_SEQUENCE;
		tag.length = length;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &length);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Explicit tagging shall be used to encode each parameter
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_CONTEXT_SPECIFIC;
		tag.objType = (Asn1Type)1;
		tag.length = length;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &length);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Total number of bytes that have been written
	*written = length;

	//Successful processing
	return RES_OK;
}

RES_CODE X509RsaPssParameters::x509FormatRsaPssSaltLength(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Length of the ASN.1 structure
	n = 0;

	//The default length of the salt is 20
	if (saltLen != 20)
	{
		//Write the length of the salt
		res = asn1WriteInt32((int32_t) saltLen, false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Explicit tagging shall be used to encode the saltLength parameter
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_CONTEXT_SPECIFIC;
		tag.objType = (Asn1Type)2;
		tag.length = n;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}

RES_CODE X509RsaPssParameters::x509FormatRsaPssMaskGenHashAlgo(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Length of the ASN.1 structure
	n = 0;

	//The default hash algorithm is SHA-1
	if (maskGenHashAlgo != nullptr	&& maskGenHashAlgoLen > 0)
	{
		//Write the algorithm identifier of the one-way hash function employed
		//with the mask generation function
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_OBJECT_IDENTIFIER;
		tag.length = maskGenHashAlgoLen;
		tag.value = maskGenHashAlgo;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//The hash algorithm identifier is encapsulated within a sequence
		tag.constructed = true;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_SEQUENCE;
		tag.length = n;
		tag.value = output;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, output, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}


RES_CODE X509SignatureAlgoId::x509FormatSignatureAlgo(uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t *p;
	Asn1Tag tag;
	X509SignatureAlgo signAlgo;
	const hash_info_t *hashAlgo;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	length = 0;

	//Retrieve the signature algorithm that will be used to sign the certificate
	res = x509GetSignHashAlgo(&signAlgo, &hashAlgo);
	//Unsupported signature algorithm?
	if (res != RES_OK)
		return res;

	//The Algorithm field contains the OID for the algorithm used by the CA
	//to sign the certificate
	tag.constructed = false;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_OBJECT_IDENTIFIER;
	tag.length = sa_oidLen;
	tag.value = sa_oid;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

#if (X509_RSA_SUPPORT && RSA_SUPPORT)
	//RSA signature algorithm?
	if (signAlgo == X509_SIGN_ALGO_RSA)
	{
		//For RSA signature algorithm, the parameters component of that type
		//shall be the ASN.1 type NULL (refer to RFC 3279, section 2.2.1)
		tag.constructed = false;
		tag.objClass = ASN1_CLASS_UNIVERSAL;
		tag.objType = ASN1_TYPE_NULL;
		tag.length = 0;
		tag.value = nullptr;

		//Write the corresponding ASN.1 tag
		res = tag.asn1WriteTag(false, p, &n);
	}
	else
#endif
#if (X509_RSA_PSS_SUPPORT && RSA_SUPPORT)
	//RSA-PSS signature algorithm?
	if (signAlgo == X509_SIGN_ALGO_RSA_PSS)
	{
		//The parameters must be present when used in the algorithm identifier
		//associated with a signature value (refer to RFC 4055, section 3.1)
		res = rsaPssParams.x509FormatRsaPssParameters(p, &n);
	}
	else
#endif
#if (X509_DSA_SUPPORT && DSA_SUPPORT)
	   //DSA signature algorithm?
	   if(signAlgo == X509_SIGN_ALGO_DSA)
	   {
	      //For DSA signature algorithm, the encoding shall omit the parameters
	      //field (refer to RFC 3279, section 2.2.2)
	      n = 0;
	   }
	   else
#endif
#if (X509_ECDSA_SUPPORT && ECDSA_SUPPORT)
	//ECDSA signature algorithm?
	if (signAlgo == X509_SIGN_ALGO_ECDSA)
	{
		//For ECDSA signature algorithm, the encoding must omit the parameters
		//field (refer to RFC 3279, section 2.2.3)
		n = 0;
	}
	else
#endif
#if (X509_ED25519_SUPPORT && ED25519_SUPPORT)
	//Ed25519 signature algorithm?
	if (signAlgo == X509_SIGN_ALGO_ED25519)
	{
		//The parameters must be absent (refer to RFC 8410, section 6)
		n = 0;
	}
	else
#endif
#if (X509_ED448_SUPPORT && ED448_SUPPORT)
	//Ed448 signature algorithm?
	if (signAlgo == X509_SIGN_ALGO_ED448)
	{
		//The parameters must be absent (refer to RFC 8410, section 6)
		n = 0;
	}
	else
#endif
	//Invalid signature algorithm?
	{
		//Report an error
		res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
	}

	//Check status code
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//The Algorithm and Parameters fields are encapsulated within a sequence
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_SEQUENCE;
	tag.length = length;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &length);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = length;

	//Successful processing
	return RES_OK;
}

RES_CODE X509SignatureAlgoId::x509FormatSignatureValue(prng_algo_t *prngAlgo,
			const uint8_t *tbsCert, size_t tbsCertLen,
			const X509SubjectPublicKeyInfo *publicKeyInfo,
			const void *privateKey, uint8_t *output, size_t *written) const
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//The bit string shall contain an initial octet which encodes the number
	//of unused bits in the final subsequent octet
	output[0] = 0;

	//The ASN.1 DER-encoded tbsCertificate is used as the input to the signature
	//function
	res = x509GenerateSignature(prngAlgo, tbsCert, tbsCertLen, publicKeyInfo,
			privateKey, output + 1, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//The signature is encapsulated within a bit string
	tag.constructed = false;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_BIT_STRING;
	tag.length = n + 1;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}

RES_CODE X509SignatureAlgoId::x509GetSignHashAlgo(X509SignatureAlgo* signAlgo, const hash_info_t** hashAlgo) const
{
	RES_CODE res;
	size_t oidLen;
	const uint8_t *oid;

	//Initialize status code
	res = RES_OK;

	//Point to the object identifier
	oid = sa_oid;
	oidLen = sa_oidLen;

#if (X509_RSA_SUPPORT && RSA_SUPPORT)
	#if (X509_MD5_SUPPORT && MD5_SUPPORT)
	   //RSA with MD5 signature algorithm?
	   if(!oidComp(oid, oidLen, MD5_WITH_RSA_ENCRYPTION_OID,
	      sizeof(MD5_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = MD5_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA1_SUPPORT && SHA1_SUPPORT)
	   //RSA with SHA-1 signature algorithm?
	   if(!oidComp(oid, oidLen, SHA1_WITH_RSA_ENCRYPTION_OID,
	      sizeof(SHA1_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA1_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA224_SUPPORT && SHA224_SUPPORT)
	   //RSA with SHA-224 signature algorithm?
	   if(!oidComp(oid, oidLen, SHA224_WITH_RSA_ENCRYPTION_OID,
	      sizeof(SHA224_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA256_SUPPORT && SHA256_SUPPORT)
	   //RSA with SHA-256 signature algorithm?
	   if(!oidComp(oid, oidLen, SHA256_WITH_RSA_ENCRYPTION_OID,
	      sizeof(SHA256_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA384_SUPPORT && SHA384_SUPPORT)
	   //RSA with SHA-384 signature algorithm?
	   if(!oidComp(oid, oidLen, SHA384_WITH_RSA_ENCRYPTION_OID,
	      sizeof(SHA384_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA512_SUPPORT && SHA512_SUPPORT)
	   //RSA with SHA-512 signature algorithm?
	   if(!oidComp(oid, oidLen, SHA512_WITH_RSA_ENCRYPTION_OID,
	      sizeof(SHA512_WITH_RSA_ENCRYPTION_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA512_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_224_SUPPORT && SHA3_224_SUPPORT)
	   //RSA with SHA3-224 signature algorithm?
	   if(!oidComp(oid, oidLen, RSASSA_PKCS1_V1_5_WITH_SHA3_224_OID,
	      sizeof(RSASSA_PKCS1_V1_5_WITH_SHA3_224_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA3_224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_256_SUPPORT && SHA3_256_SUPPORT)
	   //RSA with SHA3-256 signature algorithm?
	   if(!oidComp(oid, oidLen, RSASSA_PKCS1_V1_5_WITH_SHA3_256_OID,
	      sizeof(RSASSA_PKCS1_V1_5_WITH_SHA3_256_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA3_256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_384_SUPPORT && SHA3_384_SUPPORT)
	   //RSA with SHA3-384 signature algorithm?
	   if(!oidComp(oid, oidLen, RSASSA_PKCS1_V1_5_WITH_SHA3_384_OID,
	      sizeof(RSASSA_PKCS1_V1_5_WITH_SHA3_384_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA3_384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_512_SUPPORT && SHA3_512_SUPPORT)
	   //RSA with SHA3-512 signature algorithm?
	   if(!oidComp(oid, oidLen, RSASSA_PKCS1_V1_5_WITH_SHA3_512_OID,
	      sizeof(RSASSA_PKCS1_V1_5_WITH_SHA3_512_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_RSA;
	      *hashAlgo = SHA3_512_HASH_ALGO;
	   }
	   else
	#endif
	#endif
#if (X509_RSA_PSS_SUPPORT && RSA_SUPPORT)
	   //RSA-PSS signature algorithm
	   if(!oidComp(oid, oidLen, RSASSA_PSS_OID,
	      sizeof(RSASSA_PSS_OID)))
	   {
	      //Get the OID of the hash algorithm
	      oid = rsaPssParams.hashAlgo;
	      oidLen = rsaPssParams.hashAlgoLen;

	#if (X509_SHA1_SUPPORT && SHA1_SUPPORT)
	      //SHA-1 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA1_OID, sizeof(SHA1_OID)))
	      {
	         //RSA-PSS with SHA-1 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA1_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA224_SUPPORT && SHA224_SUPPORT)
	      //SHA-224 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA224_OID, sizeof(SHA224_OID)))
	      {
	         //RSA-PSS with SHA-224 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA224_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA256_SUPPORT && SHA256_SUPPORT)
	      //SHA-256 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA256_OID, sizeof(SHA256_OID)))
	      {
	         //RSA-PSS with SHA-256 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA256_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA384_SUPPORT && SHA384_SUPPORT)
	      //SHA-384 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA384_OID, sizeof(SHA384_OID)))
	      {
	         //RSA-PSS with SHA-384 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA384_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA512_SUPPORT && SHA512_SUPPORT)
	      //SHA-512 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA512_OID, sizeof(SHA512_OID)))
	      {
	         //RSA-PSS with SHA-512 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA512_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA3_224_SUPPORT && SHA3_224_SUPPORT)
	      //SHA3-224 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA3_224_OID, sizeof(SHA3_224_OID)))
	      {
	         //RSA-PSS with SHA3-224 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA3_224_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA3_256_SUPPORT && SHA3_256_SUPPORT)
	      //SHA3-256 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA3_256_OID, sizeof(SHA3_256_OID)))
	      {
	         //RSA-PSS with SHA3-256 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA3_256_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA3_384_SUPPORT && SHA3_384_SUPPORT)
	      //SHA3-384 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA3_384_OID, sizeof(SHA3_384_OID)))
	      {
	         //RSA-PSS with SHA3-384 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA3_384_HASH_ALGO;
	      }
	      else
	#endif
	#if (X509_SHA3_512_SUPPORT && SHA3_512_SUPPORT)
	      //SHA3-512 hash algorithm identifier?
	      if(!oidComp(oid, oidLen, SHA3_512_OID, sizeof(SHA3_512_OID)))
	      {
	         //RSA-PSS with SHA3-512 signature algorithm
	         *signAlgo = X509_SIGN_ALGO_RSA_PSS;
	         *hashAlgo = SHA3_512_HASH_ALGO;
	      }
	      else
	#endif
	      //Unknown hash algorithm identifier?
	      {
	         //The specified signature algorithm is not supported
	         res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
	      }
	   }
	   else
	#endif
#if (X509_DSA_SUPPORT && DSA_SUPPORT)
	#if (X509_SHA1_SUPPORT && SHA1_SUPPORT)
	   //DSA with SHA-1 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA1_OID,
	      sizeof(DSA_WITH_SHA1_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA1_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA224_SUPPORT && SHA224_SUPPORT)
	   //DSA with SHA-224 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA224_OID,
	      sizeof(DSA_WITH_SHA224_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA256_SUPPORT && SHA256_SUPPORT)
	   //DSA with SHA-256 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA256_OID,
	      sizeof(DSA_WITH_SHA256_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA384_SUPPORT && SHA384_SUPPORT)
	   //DSA with SHA-384 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA384_OID,
	      sizeof(DSA_WITH_SHA384_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA512_SUPPORT && SHA512_SUPPORT)
	   //DSA with SHA-512 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA512_OID,
	      sizeof(DSA_WITH_SHA512_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA512_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_224_SUPPORT && SHA3_224_SUPPORT)
	   //DSA with SHA3-224 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA3_224_OID,
	      sizeof(DSA_WITH_SHA3_224_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA3_224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_256_SUPPORT && SHA3_256_SUPPORT)
	   //DSA with SHA3-256 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA3_256_OID,
	      sizeof(DSA_WITH_SHA3_256_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA3_256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_384_SUPPORT && SHA3_384_SUPPORT)
	   //DSA with SHA3-384 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA3_384_OID,
	      sizeof(DSA_WITH_SHA3_384_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA3_384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_512_SUPPORT && SHA3_512_SUPPORT)
	   //DSA with SHA3-512 signature algorithm?
	   if(!oidComp(oid, oidLen, DSA_WITH_SHA3_512_OID,
	      sizeof(DSA_WITH_SHA3_512_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_DSA;
	      *hashAlgo = SHA3_512_HASH_ALGO;
	   }
	   else
	#endif
	#endif
#if (X509_ECDSA_SUPPORT && ECDSA_SUPPORT)
	#if (X509_SHA1_SUPPORT && SHA1_SUPPORT)
	   //ECDSA with SHA-1 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA1_OID,
	      sizeof(ECDSA_WITH_SHA1_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA1_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA224_SUPPORT && SHA224_SUPPORT)
	   //ECDSA with SHA-224 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA224_OID,
	      sizeof(ECDSA_WITH_SHA224_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA256_SUPPORT && SHA256_SUPPORT)
	   //ECDSA with SHA-256 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA256_OID,
	      sizeof(ECDSA_WITH_SHA256_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA384_SUPPORT && SHA384_SUPPORT)
	   //ECDSA with SHA-384 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA384_OID,
	      sizeof(ECDSA_WITH_SHA384_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA512_SUPPORT && SHA512_SUPPORT)
	   //ECDSA with SHA-512 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA512_OID,
	      sizeof(ECDSA_WITH_SHA512_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA512_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_224_SUPPORT && SHA3_224_SUPPORT)
	   //ECDSA with SHA3-224 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA3_224_OID,
	      sizeof(ECDSA_WITH_SHA3_224_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA3_224_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_256_SUPPORT && SHA3_256_SUPPORT)
	   //ECDSA with SHA3-256 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA3_256_OID,
	      sizeof(ECDSA_WITH_SHA3_256_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA3_256_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_384_SUPPORT && SHA3_384_SUPPORT)
	   //ECDSA with SHA3-384 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA3_384_OID,
	      sizeof(ECDSA_WITH_SHA3_384_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA3_384_HASH_ALGO;
	   }
	   else
	#endif
	#if (X509_SHA3_512_SUPPORT && SHA3_512_SUPPORT)
	   //ECDSA with SHA3-512 signature algorithm?
	   if(!oidComp(oid, oidLen, ECDSA_WITH_SHA3_512_OID,
	      sizeof(ECDSA_WITH_SHA3_512_OID)))
	   {
	      *signAlgo = X509_SIGN_ALGO_ECDSA;
	      *hashAlgo = SHA3_512_HASH_ALGO;
	   }
	   else
	#endif
	#endif
#if (X509_ED25519_SUPPORT && ED25519_SUPPORT)
	//Ed25519 signature algorithm?
	if (!oidComp(oid, oidLen, ED25519_OID, sizeof(ED25519_OID)))
	{
		*signAlgo = X509_SIGN_ALGO_ED25519;
		*hashAlgo = nullptr;
	}
	else
#endif
#if (X509_ED448_SUPPORT && ED448_SUPPORT)
	//Ed448 signature algorithm?
	if (!oidComp(oid, oidLen, ED448_OID, sizeof(ED448_OID)))
	{
		*signAlgo = X509_SIGN_ALGO_ED448;
		*hashAlgo = nullptr;
	}
	else
#endif
	//Unknown signature algorithm?
	{
		res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
	}

	//Return status code
	return res;
}

RES_CODE X509SignatureAlgoId::x509GenerateSignature(prng_algo_t *prngAlgo,
		const uint8_t *tbsCert, size_t tbsCertLen,
		const X509SubjectPublicKeyInfo *publicKeyInfo, const void *privateKey,
		uint8_t *output, size_t *written) const
{
	RES_CODE res;
	X509SignatureAlgo signAlgo;
	const hash_info_t *hashAlgo;
	uint8_t digest[X509_MAX_HASH_DIGEST_SIZE];

	//Retrieve the signature algorithm that will be used to sign the certificate
	res = x509GetSignHashAlgo(&signAlgo, &hashAlgo);
	//Unsupported signature algorithm?
	if (res != RES_OK)
		return res;

	//Ed25519 and Ed448 are used in PureEdDSA mode, without pre-hashing
	if (hashAlgo != NULL)
	{
		hash_algo_t* temp_hash;

		temp_hash = hashAlgo->new_hash();
		if (temp_hash == nullptr)
			return RES_OUT_OF_MEMORY;

		//Digest the TBSCertificate structure using the specified hash algorithm
		temp_hash->Input(tbsCert, tbsCertLen);
		temp_hash->Result(digest);
		delete temp_hash;

#if (X509_RSA_SUPPORT && RSA_SUPPORT)
		//RSA signature algorithm?
		if (signAlgo == X509_SIGN_ALGO_RSA)
		{
			//Generate RSA signature
			*written = ((const RsaPrivateKey*) privateKey)->n.mpiGetByteLength();
			res = ((const RsaPrivateKey*) privateKey)->rsassaPkcs1v15Sign(
					hashAlgo, digest, output, *written);
		}
		else
#endif
#if (X509_RSA_PSS_SUPPORT && RSA_SUPPORT)
		//RSA-PSS signature algorithm?
		if (signAlgo == X509_SIGN_ALGO_RSA_PSS)
		{
			//Generate RSA-PSS signature
			res = ((const RsaPrivateKey*) privateKey)->rsassaPssSign(prngAlgo,
					hashAlgo, rsaPssParams.saltLen, digest, output, written);
		}
		else
#endif
#if (X509_DSA_SUPPORT && DSA_SUPPORT)
	      //DSA signature algorithm?
	      if(signAlgo == X509_SIGN_ALGO_DSA)
	      {
	         DsaSignature signature;

	         //Initialize DSA signature
	         dsaInitSignature(&signature);

	         //Generate DSA signature
	         res = dsaGenerateSignature(prngAlgo, prngContext, privateKey,
	            digest, hashAlgo->digestSize, &signature);

	         //Check status code
	         if(res == RES_OK)
	         {
	            //Encode DSA signature using ASN.1
	            res = dsaWriteSignature(&signature, output, written);
	         }

	         //Release previously allocated resources
	         dsaFreeSignature(&signature);
	      }
	      else
	#endif
#if (X509_ECDSA_SUPPORT && ECDSA_SUPPORT)
		//ECDSA signature algorithm?
		if (signAlgo == X509_SIGN_ALGO_ECDSA)
		{
			const EcCurveInfo *curveInfo;
			EcDomainParameters params;
			EcdsaSignature signature;

			//Retrieve EC domain parameters
			curveInfo = ecGetCurveInfo(publicKeyInfo->ecParams.namedCurve,
					publicKeyInfo->ecParams.namedCurveLen);

			//Make sure the specified elliptic curve is supported
			if (curveInfo != NULL)
			{
				//Load EC domain parameters
				res = params.ecLoadDomainParameters(curveInfo);
			}
			else
			{
				//Invalid EC domain parameters
				res = RES_TLS_BAD_CERTIFICATE;
			}

			//Check status code
			if (res == RES_OK)
			{
				//Generate ECDSA signature
				res = signature.ecdsaGenerateSignature(&params, prngAlgo,
						(const Mpi*) privateKey, digest, hashAlgo->digest_size);
			}

			//Check status code
			if (res == RES_OK)
			{
				//Encode ECDSA signature using ASN.1
				res = signature.ecdsaWriteSignature(output, written);
			}
		}
		else
#endif
		//Invalid signature algorithm?
		{
			//Report an error
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}
	}
	else
	{
#if (X509_ED25519_SUPPORT && ED25519_SUPPORT)
		//Ed25519 signature algorithm?
		if (signAlgo == X509_SIGN_ALGO_ED25519)
		{
			const EddsaPrivateKey *eddsaPrivateKey;

			//Point to the EdDSA private key
			eddsaPrivateKey = (const EddsaPrivateKey*) privateKey;

			//Check the length of the EdDSA private key
			if (mpiGetByteLength(&eddsaPrivateKey->d)
					== ED25519_PRIVATE_KEY_LEN)
			{
				uint8_t d[ED25519_PRIVATE_KEY_LEN];

				//Retrieve private key
				res = mpiExport(&eddsaPrivateKey->d, d,
						ED25519_PRIVATE_KEY_LEN, MPI_FORMAT_LITTLE_ENDIAN);

				//Check status code
				if (res == RES_OK)
				{
					//Generate Ed25519 signature (PureEdDSA mode)
					res = ed25519GenerateSignature(d, NULL, tbsCert,
							tbsCertLen,
							NULL, 0, 0, output);
				}

				//Length of the resulting EdDSA signature
				*written = ED25519_SIGNATURE_LEN;
			}
			else
			{
				//The length of the EdDSA private key is not valid
				res = RES_TLS_INVALID_KEY;
			}
		}
		else
#endif
#if (X509_ED448_SUPPORT && ED448_SUPPORT)
		//Ed448 signature algorithm?
		if (signAlgo == X509_SIGN_ALGO_ED448)
		{
			const EddsaPrivateKey *eddsaPrivateKey;

			//Point to the EdDSA private key
			eddsaPrivateKey = (const EddsaPrivateKey*) privateKey;

			//Check the length of the EdDSA private key
			if (mpiGetByteLength(&eddsaPrivateKey->d) == ED448_PRIVATE_KEY_LEN)
			{
				uint8_t d[ED448_PRIVATE_KEY_LEN];

				//Retrieve private key
				res = mpiExport(&eddsaPrivateKey->d, d, ED448_PRIVATE_KEY_LEN,
						MPI_FORMAT_LITTLE_ENDIAN);

				//Check status code
				if (res == RES_OK)
				{
					//Generate Ed448 signature (PureEdDSA mode)
					res = ed448GenerateSignature(d, NULL, tbsCert, tbsCertLen,
					NULL, 0, 0, output);
				}

				//Length of the resulting EdDSA signature
				*written = ED448_SIGNATURE_LEN;
			}
			else
			{
				//The length of the EdDSA private key is not valid
				res = RES_TLS_INVALID_KEY;
			}
		}
		else
#endif
		//Invalid signature algorithm?
		{
			//Report an error
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}
	}

	//Return status code
	return res;
}

RES_CODE X509CertRequestInfo::x509CreateCsr(prng_algo_t* prngAlgo, const void* subjectPublicKey,
		const X509SignatureAlgoId* signatureAlgo,
		const void* signerPrivateKey, uint8_t* output, size_t* written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t *p;
	uint8_t *input;
	size_t inputLen;
	Asn1Tag tag;

	//Check parameters
	if (subjectPublicKey == nullptr || signatureAlgo == nullptr
			|| signerPrivateKey == nullptr || written == nullptr)
	{
		return RES_TLS_INVALID_PARAMETER;
	}

	//Point to the buffer where to write the CSR
	p = output;
	//Length of the CSR
	length = 0;

	//Format CertificationRequestInfo structure
	res = x509FormatCertRequestInfo(subjectPublicKey, p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//The ASN.1 DER-encoded CertificationRequestInfo is used as the input to
	//the signature function
	input = p;
	inputLen = n;

	//Advance data pointer
	p += n;
	length += n;

	//Format SignatureAlgorithm structure
	res = signatureAlgo->x509FormatSignatureAlgo(p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format Signature structure
	res = signatureAlgo->x509FormatSignatureValue(prngAlgo, input, inputLen,
			&subjectPublicKeyInfo, signerPrivateKey, p, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//The CSR is encapsulated within a sequence
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_SEQUENCE;
	tag.length = length;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &n);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertRequestInfo::x509FormatCertRequestInfo(const void *publicKey,
		uint8_t *output, size_t *written) const
{
	RES_CODE res;
	size_t n;
	size_t length;
	uint8_t *p;
	Asn1Tag tag;

	//Point to the buffer where to write the ASN.1 structure
	p = output;
	//Length of the ASN.1 structure
	length = 0;

	//Format Version field
	res = asn1WriteInt32(version, false, p, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format Subject field
	res = subject.x509FormatName(p, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format SubjectPublicKeyInfo field
	res = subjectPublicKeyInfo.x509FormatSubjectPublicKeyInfo(publicKey, nullptr, p, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//Format Attributes field
	res = attributes.x509FormatAttributes(p, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Advance data pointer
	p += n;
	length += n;

	//The CertificationRequestInfo structure is encapsulated within a sequence
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_SEQUENCE;
	tag.length = length;
	tag.value = output;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, output, &n);
	//Any error to report?
	if(res != RES_OK)
		return res;

	//Total number of bytes that have been written
	*written = n;

	//Successful processing
	return RES_OK;
}
