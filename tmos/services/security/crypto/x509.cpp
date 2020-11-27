/*
 * x509.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <x509.h>
#include <asn1.h>
#include <oid.h>
#include <ec.h>
#include <hmac.h>
#include <ecdsa.h>

//Check crypto library configuration
#if X509_SUPPORT

//Common Name OID (2.5.4.3)
const uint8_t X509_COMMON_NAME_OID[3] = {0x55, 0x04, 0x03};
//Surname OID (2.5.4.4)
const uint8_t X509_SURNAME_OID[3] = {0x55, 0x04, 0x04};
//Serial Number OID (2.5.4.5)
const uint8_t X509_SERIAL_NUMBER_OID[3] = {0x55, 0x04, 0x05};
//Country Name OID (2.5.4.6)
const uint8_t X509_COUNTRY_NAME_OID[3] = {0x55, 0x04, 0x06};
//Locality Name OID (2.5.4.7)
const uint8_t X509_LOCALITY_NAME_OID[3] = {0x55, 0x04, 0x07};
//State Or Province Name OID (2.5.4.8)
const uint8_t X509_STATE_OR_PROVINCE_NAME_OID[] = {0x55, 0x04, 0x08};
//Organization Name OID (2.5.4.10)
const uint8_t X509_ORGANIZATION_NAME_OID[3] = {0x55, 0x04, 0x0A};
//Organizational Unit Name OID (2.5.4.11)
const uint8_t X509_ORGANIZATIONAL_UNIT_NAME_OID[3] = {0x55, 0x04, 0x0B};
//Title OID (2.5.4.12)
const uint8_t X509_TITLE_OID[3] = {0x55, 0x04, 0x0C};
//Name OID (2.5.4.41)
const uint8_t X509_NAME_OID[3] = {0x55, 0x04, 0x29};
//Given Name OID (2.5.4.42)
const uint8_t X509_GIVEN_NAME_OID[3] = {0x55, 0x04, 0x2A};
//Initials OID (2.5.4.43)
const uint8_t X509_INITIALS_OID[3] = {0x55, 0x04, 0x2B};
//Generation Qualifier OID (2.5.4.44)
const uint8_t X509_GENERATION_QUALIFIER_OID[3] = {0x55, 0x04, 0x2C};
//DN Qualifier OID (2.5.4.46)
const uint8_t X509_DN_QUALIFIER_OID[3] = {0x55, 0x04, 0x2E};
//Pseudonym OID (2.5.4.65)
const uint8_t X509_PSEUDONYM_OID[3] = {0x55, 0x04, 0x41};

//Subject Directory Attributes OID (2.5.29.9)
const uint8_t X509_SUBJECT_DIRECTORY_ATTR_OID[3] = {0x55, 0x1D, 0x09};
//Subject Key Identifier OID (2.5.29.14)
const uint8_t X509_SUBJECT_KEY_ID_OID[3] = {0x55, 0x1D, 0x0E};
//Key Usage OID (2.5.29.15)
const uint8_t X509_KEY_USAGE_OID[3] = {0x55, 0x1D, 0x0F};
//Subject Alternative Name OID (2.5.29.17)
const uint8_t X509_SUBJECT_ALT_NAME_OID[3] = {0x55, 0x1D, 0x11};
//Issuer Alternative Name OID (2.5.29.18)
const uint8_t X509_ISSUER_ALT_NAME_OID[3] = {0x55, 0x1D, 0x12};
//Basic Constraints OID (2.5.29.19)
const uint8_t X509_BASIC_CONSTRAINTS_OID[3] = {0x55, 0x1D, 0x13};
//Name Constraints OID (2.5.29.30)
const uint8_t X509_NAME_CONSTRAINTS_OID[3] = {0x55, 0x1D, 0x1E};
//CRL Distribution Points OID (2.5.29.31)
const uint8_t X509_CRL_DISTR_POINTS_OID[3] = {0x55, 0x1D, 0x1F};
//Certificate Policies OID (2.5.29.32)
const uint8_t X509_CERTIFICATE_POLICIES_OID[3] = {0x55, 0x1D, 0x20};
//Policy Mappings OID (2.5.29.33)
const uint8_t X509_POLICY_MAPPINGS_OID[3] = {0x55, 0x1D, 0x21};
//Authority Key Identifier OID (2.5.29.35)
const uint8_t X509_AUTHORITY_KEY_ID_OID[3] = {0x55, 0x1D, 0x23};
//Policy Constraints OID (2.5.29.36)
const uint8_t X509_POLICY_CONSTRAINTS_OID[3] = {0x55, 0x1D, 0x24};
//Extended Key Usage OID (2.5.29.37)
const uint8_t X509_EXTENDED_KEY_USAGE_OID[3] = {0x55, 0x1D, 0x25};
//Freshest CRL OID (2.5.29.46)
const uint8_t X509_FRESHEST_CRL_OID[3] = {0x55, 0x1D, 0x2E};
//Inhibit Any-Policy OID (2.5.29.54)
const uint8_t X509_INHIBIT_ANY_POLICY_OID[3] = {0x55, 0x1D, 0x36};

//Netscape Certificate Type OID (2.16.840.1.113730.1.1)
const uint8_t X509_NS_CERT_TYPE_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x86, 0xF8, 0x42, 0x01, 0x01};


RES_CODE X509Name::x509ParseName(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;
	Asn1Tag attrType;
	Asn1Tag attrValue;

	//Debug message
	TRACELN1_TLS("    Parsing Name...");

	//Read the contents of the Name structure
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Raw ASN.1 sequence
	rawData = data;
	rawDataLen = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//The Name describes a hierarchical name composed of attributes
	data = tag.value;
	length = tag.length;

	//Loop through all the attributes
	while (length > 0)
	{
		//Read current attribute
		res = tag.asn1ReadTag(data, length);
		//Failed to decode ASN.1 tag?
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SET);
		//The tag does not match the criteria?
		if (res != RES_OK)
			return res;

		//Point to the next attribute
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the inner tag
		res = tag.asn1ReadTag(tag.value, tag.length);
		//Failed to decode ASN.1 tag?
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
		//The tag does not match the criteria?
		if (res != RES_OK)
			return res;

		//Read attribute type
		res = attrType.asn1ReadTag(tag.value, tag.length);
		//Failed to decode ASN.1 tag?
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = attrType.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OBJECT_IDENTIFIER);
		//The tag does not match the criteria?
		if (res != RES_OK)
			return res;

		//Read attribute value
		res = attrValue. asn1ReadTag(tag.value + attrType.totalLength, tag.length - attrType.totalLength);
		//Failed to decode ASN.1 tag?
		if (res != RES_OK)
			return res;

		//Common Name attribute found?
		if (!oidComp(attrType.value, attrType.length, X509_COMMON_NAME_OID,
				sizeof(X509_COMMON_NAME_OID)))
		{
			commonName = (const char *) attrValue.value;
			commonNameLen = attrValue.length;
		}
		//Surname attribute found?
		else if (!oidComp(attrType.value, attrType.length, X509_SURNAME_OID,
				sizeof(X509_SURNAME_OID)))
		{
			surname = (const char *) attrValue.value;
			surnameLen = attrValue.length;
		}
		//Serial Number attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_SERIAL_NUMBER_OID, sizeof(X509_SERIAL_NUMBER_OID)))
		{
			serialNumber = (const char *) attrValue.value;
			serialNumberLen = attrValue.length;
		}
		//Country Name attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_COUNTRY_NAME_OID, sizeof(X509_COUNTRY_NAME_OID)))
		{
			countryName = (const char *) attrValue.value;
			countryNameLen = attrValue.length;
		}
		//Locality Name attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_LOCALITY_NAME_OID, sizeof(X509_LOCALITY_NAME_OID)))
		{
			localityName = (const char *) attrValue.value;
			localityNameLen = attrValue.length;
		}
		//State Or Province Name attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_STATE_OR_PROVINCE_NAME_OID,
				sizeof(X509_STATE_OR_PROVINCE_NAME_OID)))
		{
			stateOrProvinceName = (const char *) attrValue.value;
			stateOrProvinceNameLen = attrValue.length;
		}
		//Organization Name attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_ORGANIZATION_NAME_OID, sizeof(X509_ORGANIZATION_NAME_OID)))
		{
			organizationName = (const char *) attrValue.value;
			organizationNameLen = attrValue.length;
		}
		//Organizational Unit Name attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_ORGANIZATIONAL_UNIT_NAME_OID,
				sizeof(X509_ORGANIZATIONAL_UNIT_NAME_OID)))
		{
			organizationalUnitName = (const char *) attrValue.value;
			organizationalUnitNameLen = attrValue.length;
		}
		//Title attribute found?
		else if (!oidComp(attrType.value, attrType.length, X509_TITLE_OID,
				sizeof(X509_TITLE_OID)))
		{
			title = (const char *) attrValue.value;
			titleLen = attrValue.length;
		}
		//Name attribute found?
		else if (!oidComp(attrType.value, attrType.length, X509_NAME_OID,
				sizeof(X509_NAME_OID)))
		{
			name = (const char *) attrValue.value;
			nameLen = attrValue.length;
		}
		//Given Name attribute found?
		else if (!oidComp(attrType.value, attrType.length, X509_GIVEN_NAME_OID,
				sizeof(X509_GIVEN_NAME_OID)))
		{
			givenName = (const char *) attrValue.value;
			givenNameLen = attrValue.length;
		}
		//Initials attribute OID (2.5.4.43)
		else if (!oidComp(attrType.value, attrType.length, X509_INITIALS_OID,
				sizeof(X509_INITIALS_OID)))
		{
			initials = (const char *) attrValue.value;
			initialsLen = attrValue.length;
		}
		//Generation Qualifier attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_GENERATION_QUALIFIER_OID,
				sizeof(X509_GENERATION_QUALIFIER_OID)))
		{
			generationQualifier = (const char *) attrValue.value;
			generationQualifierLen = attrValue.length;
		}
		//DN Qualifier attribute found?
		else if (!oidComp(attrType.value, attrType.length,
				X509_DN_QUALIFIER_OID, sizeof(X509_DN_QUALIFIER_OID)))
		{
			dnQualifier = (const char *) attrValue.value;
			dnQualifierLen = attrValue.length;
		}
		//Pseudonym attribute found?
		else if (!oidComp(attrType.value, attrType.length, X509_PSEUDONYM_OID,
				sizeof(X509_PSEUDONYM_OID)))
		{
			pseudonym = (const char *) attrValue.value;
			pseudonymLen = attrValue.length;
		}
	}

	//Name field successfully parsed
	return RES_OK;
}

RES_CODE X509GeneralName::x509ParseGeneralName(const uint8_t *data, size_t len, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACE_DEBUG("        Parsing GeneralName...\r\n");

	//Read current item
	res = tag.asn1ReadTag(data, len);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Explicit tagging shall be used to encode the subject alternative name
	if (tag.objClass != ASN1_CLASS_CONTEXT_SPECIFIC)
		return RES_TLS_INVALID_CLASS;

	//Conforming CAs must not issue certificates with subjectAltNames containing
	//empty GeneralName fields (refer to RFC 5280, section 4.2.1.6)
	if (tag.length == 0)
		return RES_TLS_INVALID_SYNTAX;

	//Save subject alternative name
	type = (X509GeneralNameType) tag.objType;
	value = (char *) tag.value;
	length = tag.length;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Successful processing
	return res;
}

RES_CODE X509SubjectAltName::x509ParseSubjectAltName(const uint8_t *data, size_t length)
{
	RES_CODE res;
	uint32_t i;
	size_t n;
	Asn1Tag tag;
	X509GeneralName generalName;

	//Debug message
	TRACE_DEBUG("      Parsing SubjectAltName...\r\n");


	//The SubjectAltName structure shall contain a valid sequence
	res = tag.asn1ReadSequence(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Raw contents of the ASN.1 sequence
	rawData = tag.value;
	rawDataLen = tag.length;

	//Point to the first item of the sequence
	data = tag.value;
	length = tag.length;

	//The subject alternative name extension allows identities to be bound to the
	//subject of the certificate. These identities may be included in addition
	//to or in place of the identity in the subject field of the certificate
	for (i = 0; length > 0; i++)
	{
		//Parse GeneralName field
		res = generalName.x509ParseGeneralName(data, length, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Sanity check
		if (i < X509_MAX_SUBJECT_ALT_NAMES)
		{
			//Save subject alternative name
			generalNames[i] = generalName;
		}

		//Next item
		data += n;
		length -= n;
	}

	//If the SubjectAltName extension is present, the sequence must contain at
	//least one entry (refer to RFC 5280, section 4.2.1.6)
	if (i == 0)
		return RES_TLS_INVALID_SYNTAX;

	//Save the number of subject alternative names
	numGeneralNames = min(i, X509_MAX_SUBJECT_ALT_NAMES);

	//Successful processing
	return res;
}


RES_CODE X509CertificateInfo::x509ParseVersion(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("    Parsing Version...");

	//Explicit tagging shall be used to encode version
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_CONTEXT_SPECIFIC, 0);

	//The tag does not match the criteria?
	if (res != RES_OK)
	{
		//Assume X.509v1 format
		version = X509_VERSION_1;
		//Skip the current field
		*totalLength = 0;
		//Exit immediately
		return RES_OK;
	}

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Read the inner tag
	res = tag.asn1ReadTag(tag.value, tag.length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//Check length field
	if (tag.length != 1)
		return RES_TLS_INVALID_LENGTH;
	//Check version field
	if (tag.value[0] > X509_VERSION_3)
		return RES_TLS_INVALID_VERSION;

	//Save certificate version
	version = (X509Version)tag.value[0];
	//No error to report
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseSerialNumber(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("    Parsing SerialNumber...");

	//Read the contents of the SerialNumber structure
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//Get the signature value
	serialNumber = tag.value;
	serialNumberLen = tag.length;

	//No error to report
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseSignature(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("    Parsing Signature...");

	//Read the contents of the Signature structure
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//Read the inner tag
	res = tag.asn1ReadTag(tag.value, tag.length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,
			ASN1_TYPE_OBJECT_IDENTIFIER);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//Get the signature algorithm identifier
	signatureAlgo = tag.value;
	signatureAlgoLen = tag.length;

	//Validity field successfully parsed
	return RES_OK;
}

RES_CODE x509ParseInt(const uint8_t *data, size_t length, uint32_t *value)
{
   //Initialize integer value
   *value = 0;

   //Parse the string
   while(length > 0)
   {
      //Check whether the character is decimal digit
      if(!IS_DIGIT(*data))
         return RES_ERROR;

      //Convert the string to integer
      *value = *value * 10 + (*data - '0');

      //Next character
      data++;
      length--;
   }

   //Successful processing
   return RES_OK;
}

RES_CODE x509ParseTime(const uint8_t *data, size_t length, size_t *totalLength, time_t *time)
{
	RES_CODE res;
	uint32_t value;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing Time...");

	//Read current ASN.1 tag
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//The date may be encoded as UTCTime or GeneralizedTime
	if (tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_UTC_TIME) == RES_OK)
	{
		//Check the length of the UTCTime field
		if (tag.length != 13)
			return RES_TLS_INVALID_SYNTAX;

		//The UTCTime uses a 2-digit representation of the year
		res = x509ParseInt(tag.value, 2, &value);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//If YY is greater than or equal to 50, the year shall be interpreted
		//as 19YY. If YY is less than 50, the year shall be interpreted as 20YY
		if (value >= 50)
			time->year = 1900 + value;
		else
			time->year = 2000 + value;

		//Point to the next field
		data = tag.value + 2;
	}
	else if (tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_GENERALIZED_TIME) == RES_OK)
	{
		//Check the length of the GeneralizedTime field
		if (tag.length != 15)
			return RES_TLS_INVALID_SYNTAX;

		//The GeneralizedTime uses a 4-digit representation of the year
		res = x509ParseInt(tag.value, 4, &value);
		if (res != RES_OK)
			return res;

		//Point to the next field
		data = tag.value + 4;
	}
	else
	{
		//The tag does not contain a valid date
		return RES_ERROR;
	}

	//Month
	res = x509ParseInt(data, 2, &value);
	if (res != RES_OK)
		return res;

	//Save the resulting value
	time->mon = value;

	//Day
	res = x509ParseInt(data + 2, 2, &value);
	if (res != RES_OK)
		return res;

	//Save the resulting value
	time->mday = value;

	//Hours
	res = x509ParseInt(data + 4, 2, &value);
	if (res != RES_OK)
		return res;

	//Save the resulting value
	time->hour = value;

	//Minutes
	res = x509ParseInt(data + 6, 2, &value);
	if (res != RES_OK)
		return res;

	//Save the resulting value
	time->minute = value;

	//Seconds
	res = x509ParseInt(data + 8, 2, &value);
	if (res != RES_OK)
		return res;

	//The encoding shall terminate with a "Z"
	if (data[10] != 'Z')
		return RES_TLS_INVALID_SYNTAX;

	//Save the resulting value
	time->second = value;

	//Milliseconds
	time->subsec = 0;

	//UTCTime or GeneralizedTime field successfully parsed
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseValidity(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("    Parsing Validity...");

	//Read the contents of the Validity structure
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	//The tag does not match the criteria?
	if (res != RES_OK)
		return res;

	//Point to the very first field of the sequence
	data = tag.value;
	length = tag.length;

	//The NotBefore field may be encoded as UTCTime or GeneralizedTime
	res = x509ParseTime(data, length, &n, &validity.notBefore);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//The NotAfter field may be encoded as UTCTime or GeneralizedTime
	res = x509ParseTime(data, length, &n, &validity.notAfter);

	return res;
}

RES_CODE X509CertificateInfo::x509ParseDsaParameters(const uint8_t *data, size_t length)
{
#if DSA_SUPPORT
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("        Parsing DSAParameters...");

	//Read DSAParameters structure
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Point to the first field
	data = tag.value;
	length = tag.length;

	//Read the parameter p
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if (res != RES_OK)
		return res;

	//Save the parameter p
	subjectPublicKeyInfo.dsaParams.p = tag.value;
	subjectPublicKeyInfo.dsaParams.pLen = tag.length;

	//Point to the next field
	data += tag.totalLength;
	length -= tag.totalLength;

	//Read the parameter q
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if (res != RES_OK)
		return res;

	//Save the parameter q
	subjectPublicKeyInfo.dsaParams.q = tag.value;
	subjectPublicKeyInfo.dsaParams.qLen = tag.length;

	//Point to the next field
	data += tag.totalLength;
	length -= tag.totalLength;

	//Read the parameter g
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if (res != RES_OK)
		return res;

	//Save the parameter g
	subjectPublicKeyInfo.dsaParams.g = tag.value;
	subjectPublicKeyInfo.dsaParams.gLen = tag.length;

	//Successful processing
	return RES_OK;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ParseEcParameters(const uint8_t *data, size_t length)
{
#if EC_SUPPORT
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("        Parsing ECParameters...");

	//Read namedCurve field
	res = tag.asn1ReadTag(data, length);
	if(res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OBJECT_IDENTIFIER);
	if(res != RES_OK)
		return res;

	//The namedCurve field identifies all the required values for a particular
	//set of elliptic curve domain parameters to be represented by an object
	//identifier
	subjectPublicKeyInfo.ecParams.namedCurve = tag.value;
	subjectPublicKeyInfo.ecParams.namedCurveLen = tag.length;

	//Successful processing
	return RES_OK;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ParseAlgorithmIdentifier(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing AlgorithmIdentifier...");

	//Read AlgorithmIdentifier field
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Point to the first field
	data = tag.value;
	length = tag.length;

	//Read algorithm identifier (OID)
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_OBJECT_IDENTIFIER);
	if (res != RES_OK)
		return res;

	//Save the algorithm identifier
	subjectPublicKeyInfo.oid = tag.value;
	subjectPublicKeyInfo.oidLen = tag.length;

	//Point to the next field (if any)
	data += tag.totalLength;
	length -= tag.totalLength;

#if RSA_SUPPORT
	//RSA algorithm identifier?
	if(tag.asn1CheckOid(RSA_ENCRYPTION_OID, sizeof(RSA_ENCRYPTION_OID)) == RES_OK)
	{
		//RSA does not require any additional parameters
		res = RES_OK;
	}
	else
#endif
#if DSA_SUPPORT
	//DSA algorithm identifier?
	if(tag.asn1CheckOid(DSA_OID, sizeof(DSA_OID)) == RES_OK)
	{
		//Read DsaParameters structure
		res = x509ParseDsaParameters(data, length);
	}
	else
#endif
#if EC_SUPPORT
	//EC public key identifier?
	if(tag.asn1CheckOid(EC_PUBLIC_KEY_OID, sizeof(EC_PUBLIC_KEY_OID)) == RES_OK)
	{
		//Read ECParameters structure
		res = x509ParseEcParameters(data, length);
	}
	else
#endif
	//The certificate does not contain any valid public key...
	{
		//Report an error
		res = RES_TLS_BAD_CERTIFICATE;
	}

	//Return status code
	return res;
}

RES_CODE X509CertificateInfo::x509ParseRsaPublicKey(const uint8_t *data, size_t length)
{
#if RSA_SUPPORT
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing RSAPublicKey...");

	//Read RSAPublicKey structure
	res = tag.asn1ReadTag(data, length);
	if(res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if(res != RES_OK)
		return res;

	//Point to the first field
	data = tag.value;
	length = tag.length;

	//Read Modulus field
	res = tag.asn1ReadTag(data, length);
	if(res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if(res != RES_OK)
		return res;

	//Get the modulus
	subjectPublicKeyInfo.rsaPublicKey.n = tag.value;
	subjectPublicKeyInfo.rsaPublicKey.nLen = tag.length;

	//Point to the next field
	data += tag.totalLength;
	length -= tag.totalLength;

	//Read PublicExponent field
	res = tag.asn1ReadTag(data, length);
	if(res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if(res != RES_OK)
		return res;

	//Get the public exponent
	subjectPublicKeyInfo.rsaPublicKey.e = tag.value;
	subjectPublicKeyInfo.rsaPublicKey.eLen = tag.length;

	//Successful processing
	return RES_OK;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ParseDsaPublicKey(const uint8_t *data, size_t length)
{
#if DSA_SUPPORT
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing DSAPublicKey...");

	//Read DSAPublicKey structure
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if(res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if(res != RES_OK)
		return res;

	//Save the DSA public value
	subjectPublicKeyInfo.dsaPublicKey.y = tag.value;
	subjectPublicKeyInfo.dsaPublicKey.yLen = tag.length;

	//Successful processing
	return RES_OK;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ParseEcPublicKey(const uint8_t *data, size_t length)
{
#if EC_SUPPORT
   //Debug message
   TRACELN1_TLS("      Parsing ECPublicKey...");

   //Make sure the EC public key is valid
   if(!length)
      return RES_TLS_BAD_CERTIFICATE;

   //Save the EC public key
   subjectPublicKeyInfo.ecPublicKey.q = data;
   subjectPublicKeyInfo.ecPublicKey.qLen = length;

   //Successful processing
   return RES_OK;
#else
   //Not implemented
   return RES_TLS_NOT_IMPLEMENTED;
#endif
}


RES_CODE X509CertificateInfo::x509ParseSubjectPublicKeyInfo(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("    Parsing SubjectPublicKeyInfo...");

	//Read SubjectPublicKeyInfo field
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Point to the first field
	data = tag.value;
	length = tag.length;

	//Read AlgorithmIdentifier field
	res = x509ParseAlgorithmIdentifier(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//The SubjectPublicKey structure is encapsulated within a bit string
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_BIT_STRING);
	if (res != RES_OK)
		return res;

	//The bit string shall contain an initial octet which encodes the number
	//of unused bits in the final subsequent octet
	if (tag.length < 1 || tag.value[0] != 0x00)
		return RES_ERROR;

#if RSA_SUPPORT
	//RSA algorithm identifier?
	if(!oidComp(subjectPublicKeyInfo.oid, subjectPublicKeyInfo.oidLen,
					RSA_ENCRYPTION_OID, sizeof(RSA_ENCRYPTION_OID)))
	{
		//Read RSAPublicKey structure
		res = x509ParseRsaPublicKey(tag.value + 1, tag.length - 1);
	}
	else
#endif
#if DSA_SUPPORT
	//DSA algorithm identifier?
	if(!oidComp(subjectPublicKeyInfo.oid, subjectPublicKeyInfo.oidLen,
					DSA_OID, sizeof(DSA_OID)))
	{
		//Read DSAPublicKey structure
		res = x509ParseDsaPublicKey(tag.value + 1, tag.length - 1);
	}
	else
#endif
#if EC_SUPPORT
	//EC public key identifier?
	if(!oidComp(subjectPublicKeyInfo.oid, subjectPublicKeyInfo.oidLen,
					EC_PUBLIC_KEY_OID, sizeof(EC_PUBLIC_KEY_OID)))
	{
		//Read ECPublicKey structure
		res = x509ParseEcPublicKey(tag.value + 1, tag.length - 1);
	}
	else
#endif
	//The certificate does not contain any valid public key...
	{
		//Report an error
		res = RES_TLS_BAD_CERTIFICATE;
	}

	//Return status code
	return res;
}

RES_CODE X509CertificateInfo::x509ParseIssuerUniqueId(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//No more data to process?
	if (!length)
	{
		//The IssuerUniqueID field is optional
		*totalLength = 0;
		//Exit immediately
		return RES_OK;
	}

	//Explicit tagging is used to encode the IssuerUniqueID field
	res = tag.asn1ReadTag(data, length);
	//Failed to decode ASN.1 tag?
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_CONTEXT_SPECIFIC, 1);
	//The tag does not match the criteria?
	if (res != RES_OK)
	{
		//The IssuerUniqueID field is optional
		*totalLength = 0;
		//Exit immediately
		return RES_OK;
	}

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Debug message
	TRACELN1_TLS("    Parsing IssuerUniqueID...");

	//This field must only appear if the version is 2 or 3
	if (version < X509_VERSION_2)
		return RES_TLS_INVALID_VERSION;

	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseSubjectUniqueId(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//No more data to process?
	if (!length)
	{
		//The SubjectUniqueID field is optional
		*totalLength = 0;
		return RES_OK;
	}

	//Explicit tagging is used to encode the SubjectUniqueID field
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_CONTEXT_SPECIFIC, 2);
	if (res != RES_OK)
	{
		//The SubjectUniqueID field is optional
		*totalLength = 0;
		return RES_OK;
	}

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Debug message
	TRACELN1_TLS("    Parsing SubjectUniqueID...");

	//This field must only appear if the version is 2 or 3
	if (version < X509_VERSION_2)
		return RES_TLS_INVALID_VERSION;

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseBasicConstraints(const uint8_t *data, size_t length)
{
	RES_CODE res;
	int32_t value;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing BasicConstraints...");

	//The BasicConstraints structure shall contain a valid sequence
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Point to the first item of the sequence
	data = tag.value;
	length = tag.length;

	//The cA field is optional
	if (length > 0)
	{
		//The cA boolean indicates whether the certified public key may be used
		//to verify certificate signatures
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,
				ASN1_TYPE_BOOLEAN);

		//Check status code
		if (res == RES_OK)
		{
			//Make sure the length of the boolean is valid
			if (tag.length != 1)
				return RES_TLS_INVALID_LENGTH;

			//Get boolean value
			extensions.basicConstraints.ca = tag.value[0] ? true : false;

			//Point to the next item
			data += tag.totalLength;
			length -= tag.totalLength;
		}
	}

	//The pathLenConstraint field is optional
	if (length > 0)
	{
		//Read the pathLenConstraint field
		res = tag.asn1ReadInt32(data, length, &value);
		if (res != RES_OK)
			return res;

		//The pathLenConstraint field gives the maximum number of non-self-issued
		//intermediate certificates that may follow this certificate in a valid
		//certification path
		extensions.basicConstraints.pathLenConstraint = value;
	}

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseKeyUsage(const uint8_t *data, size_t length)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing KeyUsage...");

	//The key usage extension defines the purpose of the key contained in the
	//certificate
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING);
	if (res != RES_OK)
		return res;

	//The bit string shall contain an initial octet which encodes the number
	//of unused bits in the final subsequent octet
	if (tag.length < 1)
		return RES_TLS_INVALID_SYNTAX;

	//Sanity check
	if (tag.value[0] >= 8)
		return RES_TLS_INVALID_SYNTAX;

	//Clear bit string
	extensions.keyUsage = 0;

	//Read bits b0 to b7
	if (tag.length >= 2)
		extensions.keyUsage |= __RBIT8(tag.value[1]);

	//Read bits b8 to b15
	if (tag.length >= 3)
		extensions.keyUsage |= __RBIT8(tag.value[2]) << 8;

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseExtendedKeyUsage(const uint8_t *data, size_t length)
{
	//Debug message
	TRACELN1_TLS("      Parsing ExtendedKeyUsage...");

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseSubjectKeyId(const uint8_t *data, size_t length)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing SubjectKeyIdentifier...");

	//The subject key identifier extension provides a means of identifying
	//certificates that contain a particular public key
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING);
	if (res != RES_OK)
		return res;

	//Save the subject key identifier
	extensions.subjectKeyId = tag.value;
	extensions.subjectKeyIdLen = tag.length;

	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseAuthorityKeyId(const uint8_t *data, size_t length)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing AuthorityKeyIdentifier...");

	//The AuthorityKeyIdentifier structure shall contain a valid sequence
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Point to the first item of the sequence
	data = tag.value;
	length = tag.length;

	//Parse the content of the sequence
	while (length > 0)
	{
		//Read current item
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			return res;

		//Enforce encoding and class
//		if (tag.constructed)
//			return RES_TLS_WRONG_ENCODING;
		// there are constructed tags like authorityCertIssuer - skip them
		if (!tag.constructed)
		{
			if (tag.objClass != ASN1_CLASS_CONTEXT_SPECIFIC)
				return RES_TLS_INVALID_CLASS;

			//keyIdentifier object found?
			if (tag.objType == 0)
			{
				//Save the authority key identifier
				extensions.authorityKeyId = tag.value;
				extensions.authorityKeyIdLen = tag.length;
			}

		}
		//Next item
		data += tag.totalLength;
		length -= tag.totalLength;
	}

	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseNsCertType(const uint8_t *data, size_t length)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("      Parsing NetscapeCertType...");

	//The NetscapeCertType extension limit the use of a certificate
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_BIT_STRING);
	if (res != RES_OK)
		return res;

	//The bit string shall contain an initial octet which encodes the number
	//of unused bits in the final subsequent octet
	if (tag.length < 1)
		return RES_TLS_INVALID_SYNTAX;

	//Sanity check
	if (tag.value[0] >= 8)
		return RES_TLS_INVALID_SYNTAX;

	//Clear bit string
	extensions.nsCertType = 0;

	//Read bits b0 to b7
	if (tag.length >= 2)
		extensions.nsCertType |= __RBIT8(tag.value[1]);

	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseExtensions(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	bool critical;
	const uint8_t *extensionData;
	size_t extensionLength;
	Asn1Tag tag;
	Asn1Tag oidTag;

	//No more data to process?
	if (!length)
	{
		//The Extensions field is optional
		*totalLength = 0;
		return RES_OK;
	}

	//Explicit tagging is used to encode the Extensions field
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_CONTEXT_SPECIFIC, 3);
	if (res != RES_OK)
	{
		//The Extensions field is optional
		*totalLength = 0;
		return RES_OK;
	}

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Debug message
	TRACELN1_TLS("    Parsing Extensions...");

	//This field must only appear if the version is 3
	if (version < X509_VERSION_3)
		return RES_TLS_INVALID_VERSION;

	//Read inner tag
	res = tag.asn1ReadTag(tag.value, tag.length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//This field is a sequence of one or more certificate extensions
	data = tag.value;
	length = tag.length;

	//Loop through the extensions
	while (length > 0)
	{
		//Read current extension
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
		if (res != RES_OK)
			return res;

		//Point to the next extension
		data += tag.totalLength;
		length -= tag.totalLength;

		//Contents of the current extension
		extensionData = tag.value;
		extensionLength = tag.length;

		//Read the object identifier
		res = oidTag.asn1ReadTag(extensionData, extensionLength);
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = oidTag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OBJECT_IDENTIFIER);
		if (res != RES_OK)
			return res;

		//Next item
		extensionData += oidTag.totalLength;
		extensionLength -= oidTag.totalLength;

		//Read the Critical flag (if present)
		res = tag.asn1ReadTag(extensionData, extensionLength);
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_BOOLEAN);

		//Check whether the Critical field is present
		if (res == RES_OK)
		{
			//Make sure the length of the boolean is valid
			if (tag.length != 1)
				return RES_TLS_INVALID_LENGTH;

			//Each extension in a certificate is designated as either critical
			//or non-critical
			critical = tag.value[0] ? true : false;

			//Next item
			extensionData += tag.totalLength;
			extensionLength -= tag.totalLength;
		}
		else
		{
			//The extension is considered as non-critical
			critical = false;
		}

		//The extension itself is encapsulated in an octet string
		res = tag.asn1ReadTag(extensionData, extensionLength);
		if (res != RES_OK)
			return res;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_OCTET_STRING);
		if (res != RES_OK)
			return res;

		//BasicConstraints extension found?
		if (!oidComp(oidTag.value, oidTag.length, X509_BASIC_CONSTRAINTS_OID,
				sizeof(X509_BASIC_CONSTRAINTS_OID)))
		{
			//Parse BasicConstraints extension
			res = x509ParseBasicConstraints(tag.value, tag.length);
		}
		//KeyUsage extension found?
		else if (!oidComp(oidTag.value, oidTag.length, X509_KEY_USAGE_OID,
				sizeof(X509_KEY_USAGE_OID)))
		{
			//Parse KeyUsage extension
			res = x509ParseKeyUsage(tag.value, tag.length);
		}
		//ExtendedKeyUsage extension found?
		else if (!oidComp(oidTag.value, oidTag.length,
				X509_EXTENDED_KEY_USAGE_OID,
				sizeof(X509_EXTENDED_KEY_USAGE_OID)))
		{
			//Parse ExtendedKeyUsage extension
			res = x509ParseExtendedKeyUsage(tag.value, tag.length);
		}
	    //SubjectAltName extension found?
		else if (!oidComp(oidTag.value, oidTag.length,
				X509_SUBJECT_ALT_NAME_OID, sizeof(X509_SUBJECT_ALT_NAME_OID)))
		{
			//Parse SubjectAltName extension
			res = extensions.subjectAltName.x509ParseSubjectAltName(
					tag.value, tag.length);
		}
		//SubjectKeyIdentifier extension found?
		else if (!oidComp(oidTag.value, oidTag.length, X509_SUBJECT_KEY_ID_OID,
				sizeof(X509_SUBJECT_KEY_ID_OID)))
		{
			//Parse SubjectKeyIdentifier extension
			res = x509ParseSubjectKeyId(tag.value, tag.length);
		}
		//AuthorityKeyIdentifier extension found?
		else if (!oidComp(oidTag.value, oidTag.length,
				X509_AUTHORITY_KEY_ID_OID, sizeof(X509_AUTHORITY_KEY_ID_OID)))
		{
			//Parse AuthorityKeyIdentifier extension
			res = x509ParseAuthorityKeyId(tag.value, tag.length);
		}
		//NetscapeCertType extension found?
		else if (!oidComp(oidTag.value, oidTag.length, X509_NS_CERT_TYPE_OID,
				sizeof(X509_NS_CERT_TYPE_OID)))
		{
			//Parse NetscapeCertType extension
			res = x509ParseNsCertType(tag.value, tag.length);
		}
		//The current extension is marked as critical?
		else if (critical)
		{
			//A certificate-using system must reject the certificate if it encounters
			//a critical extension it does not recognize or a critical extension that
			//contains information that it cannot process
			res = RES_TLS_NOT_IMPLEMENTED;
		}

		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Successful processing
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseTbsCertificate(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	size_t n;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("  Parsing TBSCertificate...");

	//Read the contents of the TBSCertificate structure
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//The ASN.1 DER encoded TBSCertificate is used as the input to the
	//signature function
	tbsCertificate = data;
	tbsCertificateLen = tag.totalLength;

	//Point to the very first field of the TBSCertificate
	data = tag.value;
	length = tag.length;

	//Parse Version field
	res = x509ParseVersion(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read SerialNumber field
	res = x509ParseSerialNumber(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read Signature field
	res = x509ParseSignature(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read Issuer field
	res = issuer.x509ParseName(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read Validity field
	res = x509ParseValidity(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read Subject field
	res = subject.x509ParseName(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read SubjectPublicKeyInfo field
	res = x509ParseSubjectPublicKeyInfo(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read IssuerUniqueID field (optional)
	res = x509ParseIssuerUniqueId(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read SubjectUniqueID field (optional)
	res = x509ParseSubjectUniqueId(data, length, &n);
	if (res != RES_OK)
		return res;

	//Point to the next field
	data += n;
	length -= n;

	//Read Extensions field (optional)
	res = x509ParseExtensions(data, length, &n);

	return res;
}

RES_CODE X509CertificateInfo::x509ParseSignatureAlgo(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("  Parsing SignatureAlgorithm...");

	//Read the contents of the SignatureAlgorithm field
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
	if (res != RES_OK)
		return res;

	//Read the inner tag
	res = tag.asn1ReadTag(tag.value, tag.length);
	if (res != RES_OK)
		return res;

	//This field must contain the same algorithm identifier as the signature
	//field in the TBSCertificate sequence
	res = tag.asn1CheckOid(signatureAlgo, signatureAlgoLen);

	return res;
}

RES_CODE X509CertificateInfo::x509ParseSignatureValue(const uint8_t *data, size_t length, size_t *totalLength)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACELN1_TLS("  Parsing SignatureValue...");

	//Read the contents of the SignatureValue structure
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Save the total length of the field
	*totalLength = tag.totalLength;

	//Enforce encoding, class and type
	res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_BIT_STRING);
	if (res != RES_OK)
		return res;

	//The bit string shall contain an initial octet which encodes
	//the number of unused bits in the final subsequent octet
	if (tag.length < 1 || tag.value[0] != 0x00)
		return RES_ERROR;

	//Get the signature value
	signatureValue = tag.value + 1;
	signatureValueLen = tag.length - 1;

	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ParseCertificate(const uint8_t *data, size_t length)
{
	RES_CODE res;
	size_t totalLength;
	Asn1Tag tag;

	//Clear the certificate information structure
	memclr(this, sizeof(X509CertificateInfo));

	//Read the contents of the certificate
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return RES_TLS_BAD_CERTIFICATE;

	//Point to the very first field
	data = tag.value;
	length = tag.length;

	//Parse TBSCertificate structure
	res = x509ParseTbsCertificate(data, length, &totalLength);
	if (res != RES_OK)
		return RES_TLS_BAD_CERTIFICATE;

	//Point to the next field
	data += totalLength;
	length -= totalLength;

	//Parse SignatureAlgorithm structure
	res = x509ParseSignatureAlgo(data, length, &totalLength);
	if (res != RES_OK)
		return RES_TLS_BAD_CERTIFICATE;

	//Point to the next field
	data += totalLength;
	length -= totalLength;

	//Parse SignatureValue structure
	res = x509ParseSignatureValue(data, length, &totalLength);
	if (res != RES_OK)
		return RES_TLS_BAD_CERTIFICATE;

	//Certificate successfully parsed
	return RES_OK;
}

RES_CODE X509CertificateInfo::x509ReadRsaPublicKey(RsaPublicKey* key) const
{
#if RSA_SUPPORT
	RES_CODE res;

	//The certificate shall contain a valid RSA public key
	if(!subjectPublicKeyInfo.rsaPublicKey.n || !subjectPublicKeyInfo.rsaPublicKey.e)
	{
		//Report an error
		return RES_TLS_INVALID_KEY;
	}

	//Convert the modulus to a big number
	res = key->n.mpiReadRaw(subjectPublicKeyInfo.rsaPublicKey.n,
			subjectPublicKeyInfo.rsaPublicKey.nLen);
	if(res != RES_OK)
		return res;

	//Convert the public exponent to a big number
	res = key->e.mpiReadRaw(subjectPublicKeyInfo.rsaPublicKey.e,
			subjectPublicKeyInfo.rsaPublicKey.eLen);
	if(res != RES_OK)
		return res;

	return RES_OK;
#else
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ReadDsaPublicKey(DsaPublicKey *key) const
{
#if DSA_SUPPORT
	RES_CODE res;

	//The certificate shall contain a valid DSA public key
	if(!subjectPublicKeyInfo.dsaParams.p ||
			!subjectPublicKeyInfo.dsaParams.q ||
			!subjectPublicKeyInfo.dsaParams.g ||
			!subjectPublicKeyInfo.dsaPublicKey.y)
	{
		//Report an error
		return RES_TLS_INVALID_KEY;
	}

	//Convert the parameter p to a big number
	res = key->p.mpiReadRaw(subjectPublicKeyInfo.dsaParams.p,
			subjectPublicKeyInfo.dsaParams.pLen);
	if(res != RES_OK)
		return res;

	//Convert the parameter q to a big number
	res = key->q.mpiReadRaw(subjectPublicKeyInfo.dsaParams.q,
			subjectPublicKeyInfo.dsaParams.qLen);
	if(res != RES_OK)
		return res;

	//Convert the parameter g to a big number
	res = key->g.mpiReadRaw(subjectPublicKeyInfo.dsaParams.g,
			subjectPublicKeyInfo.dsaParams.gLen);
	if(res != RES_OK)
		return res;

	//Convert the public value to a big number
	res = key->y.mpiReadRaw(subjectPublicKeyInfo.dsaPublicKey.y,
			subjectPublicKeyInfo.dsaPublicKey.yLen);
	if(res != RES_OK)
		return res;

	return RES_OK;
#else
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE X509CertificateInfo::x509ValidateCertificate(const X509CertificateInfo *issuerCertInfo) const
{
	RES_CODE res;
	auto_ptr<hash_algo_t> hashAlgo;

	//Use RSA, DSA or ECDSA signature algorithm?
#if RSA_SUPPORT
	bool rsaSignAlgo = false;
#endif
#if DSA_SUPPORT
	bool dsaSignAlgo = false;
#endif
#if ECDSA_SUPPORT
	bool ecdsaSignAlgo = false;
#endif

	//Retrieve current time
	res = get_gmt_seconds();
	//Any real-time clock implemented?
	if (res != 0)
	{
		time_t currentTime;

		currentTime = res;
		//Check the certificate validity period
		if (currentTime < validity.notBefore || currentTime > validity.notAfter)
			return RES_TLS_CERTIFICATE_EXPIRED;
	}

	//Make sure that the subject and issuer names chain correctly
	if (issuer.rawDataLen != issuerCertInfo->subject.rawDataLen)
		return RES_TLS_BAD_CERTIFICATE;
	if (memcmp(issuer.rawData, issuerCertInfo->subject.rawData,	issuer.rawDataLen))
		return RES_TLS_BAD_CERTIFICATE;

	//X.509 version 3 certificate?
	if (issuerCertInfo->version >= X509_VERSION_3)
	{
		//Ensure that the issuer certificate is a CA certificate
		if (!issuerCertInfo->extensions.basicConstraints.ca)
			return RES_TLS_BAD_CERTIFICATE;
	}

	//Retrieve the signature algorithm that has been used to sign the certificate
#if RSA_SUPPORT && MD5_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, MD5_WITH_RSA_ENCRYPTION_OID,
			sizeof(MD5_WITH_RSA_ENCRYPTION_OID)))
	{
		//MD5 with RSA signature algorithm
		rsaSignAlgo = true;
		hashAlgo = new_md5_hash();
	}
	else
#endif
#if RSA_SUPPORT && SHA1_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, SHA1_WITH_RSA_ENCRYPTION_OID,
			sizeof(SHA1_WITH_RSA_ENCRYPTION_OID)))
	{
		//SHA-1 with RSA signature algorithm
		rsaSignAlgo = true;
		hashAlgo = new_sha1_hash();
	}
	else
#endif
#if RSA_SUPPORT && SHA256_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, SHA256_WITH_RSA_ENCRYPTION_OID,
			sizeof(SHA256_WITH_RSA_ENCRYPTION_OID)))
	{
		//SHA-256 with RSA signature algorithm
		rsaSignAlgo = true;
		hashAlgo = new_sha256_hash();
	}
	else
#endif
#if RSA_SUPPORT && SHA384_SUPPORT
	if(!oidComp(signatureAlgo, signatureAlgoLen, SHA384_WITH_RSA_ENCRYPTION_OID,
			sizeof(SHA384_WITH_RSA_ENCRYPTION_OID)))
	{
		//SHA-384 with RSA signature algorithm
		rsaSignAlgo = true;
		hashAlgo = new_sha384_hash();
	}
	else
#endif
#if RSA_SUPPORT && SHA512_SUPPORT
	if(!oidComp(signatureAlgo, signatureAlgoLen, SHA512_WITH_RSA_ENCRYPTION_OID,
			sizeof(SHA512_WITH_RSA_ENCRYPTION_OID)))
	{
		//SHA-512 with RSA signature algorithm
		rsaSignAlgo = true;
		hashAlgo = new_sha512_hash();
	}
	else
#endif
#if DSA_SUPPORT && SHA1_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, DSA_WITH_SHA1_OID,
			sizeof(DSA_WITH_SHA1_OID)))
	{
		//DSA with SHA-1 signature algorithm
		dsaSignAlgo = true;
		hashAlgo = new_sha1_hash();
	}
	else
#endif
#if DSA_SUPPORT && SHA224_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, DSA_WITH_SHA224_OID,
			sizeof(DSA_WITH_SHA224_OID)))
	{
		//DSA with SHA-224 signature algorithm
		dsaSignAlgo = true;
		hashAlgo = new_sha224_hash();
	}
	else
#endif
#if DSA_SUPPORT && SHA256_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, DSA_WITH_SHA256_OID,
			sizeof(DSA_WITH_SHA256_OID)))
	{
		//DSA with SHA-256 signature algorithm
		dsaSignAlgo = true;
		hashAlgo = new_sha256_hash();
	}
	else
#endif
#if ECDSA_SUPPORT && SHA1_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, ECDSA_WITH_SHA1_OID,
			sizeof(ECDSA_WITH_SHA1_OID)))
	{
		//ECDSA with SHA-1 signature algorithm
		ecdsaSignAlgo = true;
		hashAlgo = new_sha1_hash();
	}
	else
#endif
#if ECDSA_SUPPORT && SHA224_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, ECDSA_WITH_SHA224_OID,
			sizeof(ECDSA_WITH_SHA224_OID)))
	{
		//ECDSA with SHA-224 signature algorithm
		ecdsaSignAlgo = true;
		hashAlgo = new_sha224_hash();
	}
	else
#endif
#if ECDSA_SUPPORT && SHA256_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, ECDSA_WITH_SHA256_OID,
			sizeof(ECDSA_WITH_SHA256_OID)))
	{
		//ECDSA with SHA-256 signature algorithm
		ecdsaSignAlgo = true;
		hashAlgo = new_sha256_hash();
	}
	else
#endif
#if ECDSA_SUPPORT && SHA384_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, ECDSA_WITH_SHA384_OID,
			sizeof(ECDSA_WITH_SHA384_OID)))
	{
		//ECDSA with SHA-384 signature algorithm
		ecdsaSignAlgo = true;
		hashAlgo = new_sha384_hash();
	}
	else
#endif
#if ECDSA_SUPPORT && SHA512_SUPPORT
	if (!oidComp(signatureAlgo, signatureAlgoLen, ECDSA_WITH_SHA512_OID,
			sizeof(ECDSA_WITH_SHA512_OID)))
	{
		//ECDSA with SHA-512 signature algorithm
		ecdsaSignAlgo = true;
		hashAlgo = new_sha512_hash();
	}
	else
#endif
	{
		//The specified signature algorithm is not supported
		return RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
	}

	//Failed to allocate memory?
	if (hashAlgo.get() == nullptr)
		return RES_OUT_OF_MEMORY;

	//Digest the TBSCertificate structure using the specified hash algorithm
	hashAlgo->Input(tbsCertificate, tbsCertificateLen);
	hashAlgo->Result(nullptr);

	//Check signature algorithm
#if RSA_SUPPORT
	if(rsaSignAlgo)
	{
		RsaPublicKey publicKey;

		//Get the RSA public key
		res = issuerCertInfo->x509ReadRsaPublicKey(&publicKey);

		//Check status code
		if(res == RES_OK)
		{
			//Verify RSA signature
			res = publicKey.rsassaPkcs1v15Verify(hashAlgo->hash_info, hashAlgo->digest8(),
					signatureValue, signatureValueLen);
		}

	}
	else
#endif
#if DSA_SUPPORT
	if(dsaSignAlgo)
	{
		DsaPublicKey publicKey;
		DsaSignature signature;

		//Get the DSA public key
		res = issuerCertInfo->x509ReadDsaPublicKey(&publicKey);

		//Check status code
		if(res == RES_OK)
		{
			//Read the ASN.1 encoded signature
			res = signature.dsaReadSignature(signatureValue, signatureValueLen);
		}

		//Check status code
		if(res == RES_OK)
		{
			//Verify DSA signature
			res = signature.dsaVerifySignature(&publicKey, hashAlgo->digest8(),
					hashAlgo->hash_info->digest_size);
		}

	}
	else
#endif
#if ECDSA_SUPPORT
	if(ecdsaSignAlgo)
	{
		const EcCurveInfo *curveInfo;
		EcDomainParameters params;
		EcPoint publicKey;
		EcdsaSignature signature;


		//Retrieve EC domain parameters
		curveInfo = ecGetCurveInfo(issuerCertInfo->subjectPublicKeyInfo.ecParams.namedCurve,
				issuerCertInfo->subjectPublicKeyInfo.ecParams.namedCurveLen);

		//Make sure the specified elliptic curve is supported
		if(curveInfo != nullptr)
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
		if(res == RES_OK)
		{
			//Retrieve the EC public key
			res = params.ecImport(&publicKey, issuerCertInfo->subjectPublicKeyInfo.ecPublicKey.q,
					issuerCertInfo->subjectPublicKeyInfo.ecPublicKey.qLen);
		}

		//Check status code
		if(res == RES_OK)
		{
			//Read the ASN.1 encoded signature
			res = signature.dsaReadSignature(signatureValue, signatureValueLen);
		}

		//Check status code
		if(res == RES_OK)
		{
			//Verify ECDSA signature
			res = signature.ecdsaVerifySignature(&params, &publicKey,
					hashAlgo->digest8(), hashAlgo->hash_info->digest_size);
		}

	}
	else
#endif
	{
		//The signature algorithm is not supported...
		res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
	}

	return res;
}

RES_CODE x509CompareSubjectName(const char *subjectName, size_t subjectNameLen,
		const char *fqdn)
{
	size_t i;
	size_t j;
	size_t fqdnLen;

	//Retrieve the length of the FQDN
	fqdnLen = strlen(fqdn);

	//Initialize variables
	i = 0;
	j = 0;

	//Parse the subject name
	while (i < subjectNameLen && j < fqdnLen)
	{
		//Wildcard name found?
		if (subjectName[i] == '*')
		{
			//The implementation should not attempt to match a presented
			//identifier in which the wildcard character comprises a label other
			//than the left-most label (refer to RFC 6125, section 6.4.3)
			if (i != 0)
			{
				break;
			}

			//The implementation should not compare against anything but the
			//left-most label of the reference identifier
			if (fqdn[j] == '.')
			{
				i++;
			}
			else
			{
				j++;
			}
		}
		else
		{
			//Perform case insensitive character comparison
			if (tolower(subjectName[i]) != fqdn[j])
			{
				break;
			}

			//Compare next characters
			i++;
			j++;
		}
	}

	//Check whether the subject name matches the specified FQDN
	if (i == subjectNameLen && j == fqdnLen)
	{
		return RES_OK;
	}

	return RES_IDLE;
}

RES_CODE X509CertificateInfo::x509CheckSubjectName(const char *fqdn) const
{
	RES_CODE res;
	uint32_t i;
	size_t n;
	size_t length;
	const uint8_t *data;
	const X509Extensions *extension;
	X509GeneralName generalName;

	//Point to the X.509 extension of the CA certificate
	extension = &extensions;

	//Valid FQDN name provided?
	if (fqdn)
	{
		//Initialize flag
		res = RES_IDLE;

		//Total number of valid DNS names found in the SubjectAltName extension
		i = 0;

		//Valid SubjectAltName extension?
		if (extension->subjectAltName.rawDataLen > 0)
		{
			//The subject alternative name extension allows identities to be bound
			//to the subject of the certificate. These identities may be included
			//in addition to or in place of the identity in the subject field of
			//the certificate
			data = extension->subjectAltName.rawData;
			length = extension->subjectAltName.rawDataLen;

			//Loop through the list of subject alternative names
			while (res == RES_IDLE && length > 0)
			{
				//Parse GeneralName field
				res = generalName.x509ParseGeneralName(data, length, &n);
				//Failed to decode ASN.1 tag?
				if (res != RES_OK)
					return res;
				res = RES_IDLE;

				//DNS name found?
				if (generalName.type == X509_GENERAL_NAME_TYPE_DNS)
				{
					//Check whether the alternative name matches the specified FQDN
					res = x509CompareSubjectName(generalName.value,
							generalName.length, fqdn);

					//Increment counter
					i++;
				}

				//Next item
				data += n;
				length -= n;
			}
		}

		//No match?
		if (res == RES_IDLE)
		{
			//The implementation must not seek a match for a reference identifier
			//of CN-ID if the presented identifiers include a DNS-ID, SRV-ID or
			//URI-ID (refer to RFC 6125, section 6.4.4)
			if (i == 0 && subject.commonNameLen > 0)
			{
				//The implementation may as a last resort check the CN-ID for a match
				res = x509CompareSubjectName(subject.commonName, subject.commonNameLen, fqdn);
			}
		}

		//Check whether the subject name matches the specified FQDN
		if(res == RES_IDLE)
			res = RES_TLS_UNKNOWN_IDENTITY;
	}
	else
	{
		//If no valid FQDN name is provided, then the subject name of the
		//certificate is not verified
		res = RES_OK;
	}

	//Return status code
	return res;

}


#endif
