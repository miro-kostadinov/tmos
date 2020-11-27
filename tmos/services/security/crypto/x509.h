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

enum X509Version : uint8_t
{
   X509_VERSION_1 = 0x00,
   X509_VERSION_2 = 0x01,
   X509_VERSION_3 = 0x02,
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
};

struct X509DsaParameters
{
   const uint8_t *p;
   size_t pLen;
   const uint8_t *q;
   size_t qLen;
   const uint8_t *g;
   size_t gLen;
};

struct X509DsaPublicKey
{
   const uint8_t *y;
   size_t yLen;
};

struct X509EcParameters
{
   const uint8_t *namedCurve;
   size_t namedCurveLen;
};


struct X509EcPublicKey
{
   const uint8_t *q;
   size_t qLen;
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
};

struct X509BasicContraints
{
   bool ca;
   uint32_t pathLenConstraint;
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
};

struct X509Extensions
{
   X509BasicContraints basicConstraints;
   uint16_t keyUsage;
   X509SubjectAltName subjectAltName;
   const uint8_t *subjectKeyId;
   size_t subjectKeyIdLen;
   const uint8_t *authorityKeyId;
   size_t authorityKeyIdLen;
   uint8_t nsCertType;
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
   RES_CODE x509ParseDsaParameters(const uint8_t *data, size_t length);
   RES_CODE x509ParseEcParameters(const uint8_t *data, size_t length);
   RES_CODE x509ParseAlgorithmIdentifier(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseRsaPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseDsaPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseEcPublicKey(const uint8_t *data, size_t length);
   RES_CODE x509ParseSubjectPublicKeyInfo(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseIssuerUniqueId(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseSubjectUniqueId(const uint8_t *data, size_t length, size_t *totalLength);
   RES_CODE x509ParseBasicConstraints(const uint8_t *data, size_t length);
   RES_CODE x509ParseKeyUsage(const uint8_t *data, size_t length);
   RES_CODE x509ParseExtendedKeyUsage(const uint8_t *data, size_t length);
   RES_CODE x509ParseSubjectKeyId(const uint8_t *data, size_t length);
   RES_CODE x509ParseAuthorityKeyId(const uint8_t *data, size_t length);
   RES_CODE x509ParseNsCertType(const uint8_t *data, size_t length);
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
