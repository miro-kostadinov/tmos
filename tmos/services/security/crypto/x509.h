/*
 * x509.h
 *
 *  Created on: Jul 13, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_X509_H_
#define SERVICES_SECURITY_CRYPTO_X509_H_

#include <tmos.h>
#include <crypto.h>
#include <tmos_time.h>
#include <rsa.h>
#include <dsa.h>
#if EC_SUPPORT
#include <ec.h>
#endif

//RSA certificate support
#ifndef X509_RSA_SUPPORT
   #define X509_RSA_SUPPORT 1
#endif

//RSA-PSS certificate support
#ifndef X509_RSA_PSS_SUPPORT
   #define X509_RSA_PSS_SUPPORT 0
#endif

//DSA certificate support
#ifndef X509_DSA_SUPPORT
   #define X509_DSA_SUPPORT 0
#endif

//ECDSA certificate support
#ifndef X509_ECDSA_SUPPORT
   #define X509_ECDSA_SUPPORT 1
#endif

//MD5 hash support (insecure)
#ifndef X509_MD5_SUPPORT
   #define X509_MD5_SUPPORT 0
#endif

//SHA-1 hash support (weak)
#ifndef X509_SHA1_SUPPORT
   #define X509_SHA1_SUPPORT 0
#endif

//SHA-224 hash support (weak)
#ifndef X509_SHA224_SUPPORT
   #define X509_SHA224_SUPPORT 0
#endif

//SHA-256 hash support
#ifndef X509_SHA256_SUPPORT
   #define X509_SHA256_SUPPORT 1
#endif

//SHA-384 hash support
#ifndef X509_SHA384_SUPPORT
   #define X509_SHA384_SUPPORT 1
#endif

//SHA-512 hash support
#ifndef X509_SHA512_SUPPORT
   #define X509_SHA512_SUPPORT 1
#endif

//SHA3-224 hash support
#ifndef X509_SHA3_224_SUPPORT
   #define X509_SHA3_224_SUPPORT 0
#endif

//SHA3-256 hash support
#ifndef X509_SHA3_256_SUPPORT
   #define X509_SHA3_256_SUPPORT 0
#endif

//SHA3-384 hash support
#ifndef X509_SHA3_384_SUPPORT
   #define X509_SHA3_384_SUPPORT 0
#endif

//SHA3-512 hash support
#ifndef X509_SHA3_512_SUPPORT
   #define X509_SHA3_512_SUPPORT 0
#endif

//secp112r1 elliptic curve support (weak)
#ifndef X509_SECP112R1_SUPPORT
   #define X509_SECP112R1_SUPPORT 0
#endif

//secp112r2 elliptic curve support (weak)
#ifndef X509_SECP112R2_SUPPORT
   #define X509_SECP112R2_SUPPORT 0
#endif

//secp128r1 elliptic curve support (weak)
#ifndef X509_SECP128R1_SUPPORT
   #define X509_SECP128R1_SUPPORT 0
#endif

//secp128r2 elliptic curve support (weak)
#ifndef X509_SECP128R2_SUPPORT
   #define X509_SECP128R2_SUPPORT 0
#endif

//secp160k1 elliptic curve support (weak)
#ifndef X509_SECP160K1_SUPPORT
   #define X509_SECP160K1_SUPPORT 0
#endif

//secp160r1 elliptic curve support (weak)
#ifndef X509_SECP160R1_SUPPORT
   #define X509_SECP160R1_SUPPORT 0
#endif

//secp160r2 elliptic curve support (weak)
#ifndef X509_SECP160R2_SUPPORT
   #define X509_SECP160R2_SUPPORT 0
#endif

//secp192k1 elliptic curve support
#ifndef X509_SECP192K1_SUPPORT
   #define X509_SECP192K1_SUPPORT 0
#endif

//secp192r1 elliptic curve support (NIST P-192)
#ifndef X509_SECP192R1_SUPPORT
   #define X509_SECP192R1_SUPPORT 0
#endif

//secp224k1 elliptic curve support
#ifndef X509_SECP224K1_SUPPORT
   #define X509_SECP224K1_SUPPORT 0
#endif

//secp224r1 elliptic curve support (NIST P-224)
#ifndef X509_SECP224R1_SUPPORT
   #define X509_SECP224R1_SUPPORT 0
#endif

//secp256k1 elliptic curve support
#ifndef X509_SECP256K1_SUPPORT
   #define X509_SECP256K1_SUPPORT 0
#endif

//secp256r1 elliptic curve support (NIST P-256)
#ifndef X509_SECP256R1_SUPPORT
   #define X509_SECP256R1_SUPPORT 1
#endif

//secp384r1 elliptic curve support (NIST P-384)
#ifndef X509_SECP384R1_SUPPORT
   #define X509_SECP384R1_SUPPORT 1
#endif

//secp521r1 elliptic curve support (NIST P-521)
#ifndef X509_SECP521R1_SUPPORT
   #define X509_SECP521R1_SUPPORT 1
#endif

//brainpoolP160r1 elliptic curve support
#ifndef X509_BRAINPOOLP160R1_SUPPORT
   #define X509_BRAINPOOLP160R1_SUPPORT 0
#endif

//brainpoolP192r1 elliptic curve support
#ifndef X509_BRAINPOOLP192R1_SUPPORT
   #define X509_BRAINPOOLP192R1_SUPPORT 0
#endif

//brainpoolP224r1 elliptic curve support
#ifndef X509_BRAINPOOLP224R1_SUPPORT
   #define X509_BRAINPOOLP224R1_SUPPORT 0
#endif

//brainpoolP256r1 elliptic curve support
#ifndef X509_BRAINPOOLP256R1_SUPPORT
   #define X509_BRAINPOOLP256R1_SUPPORT 0
#endif

//brainpoolP320r1 elliptic curve support
#ifndef X509_BRAINPOOLP320R1_SUPPORT
   #define X509_BRAINPOOLP320R1_SUPPORT 0
#endif

//brainpoolP384r1 elliptic curve support
#ifndef X509_BRAINPOOLP384R1_SUPPORT
   #define X509_BRAINPOOLP384R1_SUPPORT 0
#endif

//brainpoolP512r1 elliptic curve support
#ifndef X509_BRAINPOOLP512R1_SUPPORT
   #define X509_BRAINPOOLP512R1_SUPPORT 0
#endif

//Ed25519 elliptic curve support
#ifndef X509_ED25519_SUPPORT
   #define X509_ED25519_SUPPORT 0
#endif

//Ed448 elliptic curve support
#ifndef X509_ED448_SUPPORT
   #define X509_ED448_SUPPORT 0
#endif

//Minimum acceptable size for RSA modulus
#ifndef X509_MIN_RSA_MODULUS_SIZE
   #define X509_MIN_RSA_MODULUS_SIZE 1024u
#elif (X509_MIN_RSA_MODULUS_SIZE < 512)
   #error X509_MIN_RSA_MODULUS_SIZE parameter is not valid
#endif

//Maximum acceptable size for RSA modulus
#ifndef X509_MAX_RSA_MODULUS_SIZE
   #define X509_MAX_RSA_MODULUS_SIZE 4096u
#elif (X509_MAX_RSA_MODULUS_SIZE < X509_MIN_RSA_MODULUS_SIZE)
   #error X509_MAX_RSA_MODULUS_SIZE parameter is not valid
#endif

//Minimum acceptable size for DSA prime modulus
#ifndef X509_MIN_DSA_MODULUS_SIZE
   #define X509_MIN_DSA_MODULUS_SIZE 1024u
#elif (X509_MIN_DSA_MODULUS_SIZE < 512)
   #error X509_MIN_DSA_MODULUS_SIZE parameter is not valid
#endif

//Maximum acceptable size for DSA prime modulus
#ifndef X509_MAX_DSA_MODULUS_SIZE
   #define X509_MAX_DSA_MODULUS_SIZE 4096u
#elif (X509_MAX_DSA_MODULUS_SIZE < X509_MIN_DSA_MODULUS_SIZE)
   #error X509_MAX_DSA_MODULUS_SIZE parameter is not valid
#endif

//Default size of serial numbers
#ifndef X509_SERIAL_NUMBER_SIZE
   #define X509_SERIAL_NUMBER_SIZE 20u
#elif (X509_SERIAL_NUMBER_SIZE < 1)
   #error X509_SERIAL_NUMBER_SIZE parameter is not valid
#endif

//Maximum number of subject alternative names
#ifndef X509_MAX_SUBJECT_ALT_NAMES
   #define X509_MAX_SUBJECT_ALT_NAMES 4u
#elif (X509_MAX_SUBJECT_ALT_NAMES < 1)
   #error X509_MAX_SUBJECT_ALT_NAMES parameter is not valid
#endif

//Maximum number of certificate issuer names
#ifndef X509_MAX_CERT_ISSUER_NAMES
   #define X509_MAX_CERT_ISSUER_NAMES 4u
#elif (X509_MAX_CERT_ISSUER_NAMES < 1)
   #error X509_MAX_CERT_ISSUER_NAMES parameter is not valid
#endif

//Maximum number of custom extensions
#ifndef X509_MAX_CUSTOM_EXTENSIONS
   #define X509_MAX_CUSTOM_EXTENSIONS 2u
#elif (X509_MAX_CUSTOM_EXTENSIONS < 1)
   #error X509_MAX_CUSTOM_EXTENSIONS parameter is not valid
#endif

//Maximum digest size
#if (X509_SHA3_512_SUPPORT && SHA3_512_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 64
#elif (X509_SHA512_SUPPORT && SHA512_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 64
#elif (X509_SHA3_384_SUPPORT && SHA3_384_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 48
#elif (X509_SHA384_SUPPORT && SHA384_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 48
#elif (X509_SHA3_256_SUPPORT && SHA3_256_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 32
#elif (X509_SHA256_SUPPORT && SHA256_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 32
#elif (X509_SHA3_224_SUPPORT && SHA3_224_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 28
#elif (X509_SHA224_SUPPORT && SHA224_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 28
#elif (X509_SHA1_SUPPORT && SHA1_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 20
#elif (X509_MD5_SUPPORT && MD5_SUPPORT)
   #define X509_MAX_HASH_DIGEST_SIZE 16
#endif

//X.509 related constants
extern const uint8_t X509_COMMON_NAME_OID[3];
extern const uint8_t X509_SURNAME_OID[3];
extern const uint8_t X509_SERIAL_NUMBER_OID[3];
extern const uint8_t X509_COUNTRY_NAME_OID[3];
extern const uint8_t X509_LOCALITY_NAME_OID[3];
extern const uint8_t X509_STATE_OR_PROVINCE_NAME_OID[3];
extern const uint8_t X509_ORGANIZATION_NAME_OID[3];
extern const uint8_t X509_ORGANIZATIONAL_UNIT_NAME_OID[3];
extern const uint8_t X509_TITLE_OID[3];
extern const uint8_t X509_NAME_OID[3];
extern const uint8_t X509_GIVEN_NAME_OID[3];
extern const uint8_t X509_INITIALS_OID[3];
extern const uint8_t X509_GENERATION_QUALIFIER_OID[3];
extern const uint8_t X509_DN_QUALIFIER_OID[3];
extern const uint8_t X509_PSEUDONYM_OID[3];

extern const uint8_t X509_SUBJECT_DIRECTORY_ATTR_OID[3];
extern const uint8_t X509_SUBJECT_KEY_ID_OID[3];
extern const uint8_t X509_KEY_USAGE_OID[3];
extern const uint8_t X509_SUBJECT_ALT_NAME_OID[3];
extern const uint8_t X509_ISSUER_ALT_NAME_OID[3];
extern const uint8_t X509_BASIC_CONSTRAINTS_OID[3];
extern const uint8_t X509_NAME_CONSTRAINTS_OID[3];
extern const uint8_t X509_CRL_DISTR_POINTS_OID[3];
extern const uint8_t X509_CERTIFICATE_POLICIES_OID[3];
extern const uint8_t X509_POLICY_MAPPINGS_OID[3];
extern const uint8_t X509_AUTHORITY_KEY_ID_OID[3];
extern const uint8_t X509_POLICY_CONSTRAINTS_OID[3];
extern const uint8_t X509_EXTENDED_KEY_USAGE_OID[3];
extern const uint8_t X509_FRESHEST_CRL_OID[3];
extern const uint8_t X509_INHIBIT_ANY_POLICY_OID[3];

extern const uint8_t X509_NS_CERT_TYPE_OID[9];

extern const uint8_t X509_CHALLENGE_PASSWORD_OID[9];
extern const uint8_t X509_EXTENSION_REQUEST_OID[9];

enum X509Version : uint8_t
{
   X509_VERSION_1 = 0x00,
   X509_VERSION_2 = 0x01,
   X509_VERSION_3 = 0x02,
};

enum X509SignatureAlgo : uint8_t
{
   X509_SIGN_ALGO_NONE    = 0,
   X509_SIGN_ALGO_RSA     = 1,
   X509_SIGN_ALGO_RSA_PSS = 2,
   X509_SIGN_ALGO_DSA     = 3,
   X509_SIGN_ALGO_ECDSA   = 4,
   X509_SIGN_ALGO_ED25519 = 5,
   X509_SIGN_ALGO_ED448   = 6
};

struct X509Name
{
   const uint8_t*	rawData;
   size_t 			rawDataLen;
   const char*		commonName;
   size_t 			commonNameLen;
   const char*		surname;
   size_t 			surnameLen;
   const char*		serialNumber;
   size_t 			serialNumberLen;
   const char*		countryName;
   size_t 			countryNameLen;
   const char*		localityName;
   size_t 			localityNameLen;
   const char*		stateOrProvinceName;
   size_t 			stateOrProvinceNameLen;
   const char*		organizationName;
   size_t 			organizationNameLen;
   const char*		organizationalUnitName;
   size_t 			organizationalUnitNameLen;
   const char*		title;
   size_t 			titleLen;
   const char*		name;
   size_t 			nameLen;
   const char*		givenName;
   size_t 			givenNameLen;
   const char*		initials;
   size_t 			initialsLen;
   const char*		generationQualifier;
   size_t 			generationQualifierLen;
   const char*		dnQualifier;
   size_t 			dnQualifierLen;
   const char*		pseudonym;
   size_t 			pseudonymLen;

   RES_CODE x509ParseName(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509FormatName(uint8_t* output, size_t* written) const;
};

struct X509Validity
{
   time_t notBefore;
   time_t notAfter;
};

struct X509RsaPublicKey
{
   const uint8_t *n;
   size_t nLen;
   const uint8_t *e;
   size_t eLen;

   RES_CODE x509FormatRsaPublicKey(uint8_t *output, size_t *written) const;

};

struct X509DsaParameters
{
   const uint8_t *p;
   size_t pLen;
   const uint8_t *q;
   size_t qLen;
   const uint8_t *g;
   size_t gLen;

   RES_CODE x509FormatDsaParameters(uint8_t* output, size_t* written) const;
};

struct X509DsaPublicKey
{
   const uint8_t *y;
   size_t yLen;

   RES_CODE x509FormatDsaPublicKey(uint8_t *output, size_t *written) const;
};

struct X509EcParameters
{
   const uint8_t *namedCurve;
   size_t namedCurveLen;

   RES_CODE x509FormatEcParameters(uint8_t* output, size_t* written) const;
};


struct X509EcPublicKey
{
   const uint8_t *q;
   size_t qLen;

   RES_CODE x509FormatEcPublicKey(uint8_t* output, size_t* written) const;
};

struct X509SubjectPublicKeyInfo
{
   const uint8_t *oid;
   size_t oidLen;
#if RSA_SUPPORT
   X509RsaPublicKey rsaPublicKey;
#endif
#if DSA_SUPPORT
   X509DsaParameters dsaParams;
   X509DsaPublicKey dsaPublicKey;
#endif
#if EC_SUPPORT
   X509EcParameters ecParams;
   X509EcPublicKey ecPublicKey;
#endif

   RES_CODE x509ParseDsaParameters(const uint8_t *data, size_t length);
   RES_CODE x509ParseEcParameters(const uint8_t *data, size_t length);
   RES_CODE x509ParseAlgorithmIdentifier(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseRsaPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseDsaPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseEcPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseSubjectPublicKeyInfo(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509FormatSubjectPublicKeyInfo(const void* publicKey, uint8_t* keyId,
		   uint8_t* output, size_t* written) const;
   RES_CODE x509FormatAlgorithmIdentifier(const void* params, uint8_t* output,
		   size_t* written) const;
#if EC_SUPPORT
   RES_CODE x509ExportEcPublicKey(const EcPoint* publicKey, uint8_t* output, size_t* written) const;
#endif
};

struct X509BasicConstraints
{
   bool critical;
   bool cA;
   uint32_t pathLenConstraint;

   RES_CODE x509FormatBasicConstraints(uint8_t* output, size_t* written) const;
};

struct X509KeyUsage
{
   bool	    critical;
   uint16_t bitmap;

   RES_CODE x509FormatKeyUsage(uint8_t* output, size_t* written) const;

};

//Maximum number of subject alternative names
#ifndef X509_MAX_SUBJECT_ALT_NAMES
   #define X509_MAX_SUBJECT_ALT_NAMES 4u
#elif (X509_MAX_SUBJECT_ALT_NAMES < 1)
   #error X509_MAX_SUBJECT_ALT_NAMES parameter is not valid
#endif

enum X509GeneralNameType
{
   X509_GENERAL_NAME_TYPE_OTHER         = 0,
   X509_GENERAL_NAME_TYPE_RFC822        = 1,
   X509_GENERAL_NAME_TYPE_DNS           = 2,
   X509_GENERAL_NAME_TYPE_X400_ADDRESS  = 3,
   X509_GENERAL_NAME_TYPE_DIRECTORY     = 4,
   X509_GENERAL_NAME_TYPE_EDI_PARTY     = 5,
   X509_GENERAL_NAME_TYPE_URI           = 6,
   X509_GENERAL_NAME_TYPE_IP_ADDRESS    = 7,
   X509_GENERAL_NAME_TYPE_REGISTERED_ID = 8
};

struct X509GeneralName
{
   X509GeneralNameType type;
   const char *value;
   size_t length;

   RES_CODE x509ParseGeneralName(const uint8_t *data, size_t len, size_t *totalLength);
};

struct X509SubjectAltName
{
   const uint8_t *rawData;
   size_t rawDataLen;
   uint32_t numGeneralNames;
   X509GeneralName generalNames[X509_MAX_SUBJECT_ALT_NAMES];

   RES_CODE x509ParseSubjectAltName(const uint8_t *data, size_t length);
   RES_CODE x509FormatSubjectAltName(uint8_t* output, size_t* written) const;
};

struct X509SubjectKeyId
{
   bool   critical;
   const uint8_t* value;
   size_t length;

   RES_CODE x509FormatSubjectKeyId(uint8_t* output, size_t* written) const;
};

struct X509AuthorityKeyId
{
   bool critical;
   const uint8_t* keyId;
   size_t keyIdLen;

   RES_CODE x509FormatAuthorityKeyId(uint8_t* output, size_t* written) const;
};

struct X509NsCertType
{
   bool	   critical;
   uint8_t bitmap;

   RES_CODE x509FormatNsCertType(uint8_t* output, size_t* written) const;
};


struct X509Extensions
{
   X509BasicConstraints basicConstraints;
   X509KeyUsage keyUsage;
   X509SubjectAltName subjectAltName;
   X509SubjectKeyId subjectKeyId;
   X509AuthorityKeyId authKeyId;
   X509NsCertType nsCertType;

   RES_CODE x509FormatExtensionRequest(uint8_t* output, size_t* written) const;
   RES_CODE x509ParseNsCertType(bool critical, const uint8_t *data, size_t length);
};


struct X509CertificateInfo
{
   const uint8_t*	tbsCertificate;
   size_t 			tbsCertificateLen;
   X509Version 		version;
   const uint8_t*	serialNumber;
   size_t 			serialNumberLen;
   X509Name 		issuer;
   X509Validity 	validity;
   X509Name 		subject;
   X509SubjectPublicKeyInfo subjectPublicKeyInfo;
   X509Extensions 	extensions;
   const uint8_t*	signatureAlgo;
   size_t 			signatureAlgoLen;
   const uint8_t*	signatureValue;
   size_t 			signatureValueLen;

   RES_CODE x509ParseVersion(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSerialNumber(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSignature(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseValidity(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseIssuerUniqueId(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSubjectUniqueId(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseBasicConstraints(const uint8_t *data, size_t length);
   RES_CODE x509ParseKeyUsage(const uint8_t *data, size_t length);
   RES_CODE x509ParseExtendedKeyUsage(const uint8_t *data, size_t length);
   RES_CODE x509ParseSubjectKeyId(const uint8_t *data, size_t length);
   RES_CODE x509ParseAuthorityKeyId(const uint8_t *data, size_t length);
   RES_CODE x509ParseExtensions(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseTbsCertificate(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSignatureAlgo(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSignatureValue(const uint8_t *data, size_t length, size_t *totalLength);

   RES_CODE x509ParseCertificate(const uint8_t *data, size_t length);

   RES_CODE x509ReadRsaPublicKey(RsaPublicKey* key) const;
   RES_CODE x509ReadDsaPublicKey(DsaPublicKey* key) const;
   RES_CODE x509ValidateCertificate(const X509CertificateInfo *issuerCertInfo) const;
   RES_CODE x509CheckSubjectName(const char *fqdn) const;

};




#endif /* SERVICES_SECURITY_CRYPTO_X509_H_ */
