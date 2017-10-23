/*
 * asn1.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_ASN1_H_
#define SERVICES_SECURITY_CRYPTO_ASN1_H_

#include <tmos.h>
#include <crypto.h>

//Tag number mask
#define ASN1_TAG_NUMBER_MASK        0x1F

//ASN.1 encoding
#define ASN1_ENCODING_MASK          0x20
#define ASN1_ENCODING_PRIMITIVE     0x00
#define ASN1_ENCODING_CONSTRUCTED   0x20

//ASN.1 class
#define ASN1_CLASS_MASK             0xC0
#define ASN1_CLASS_UNIVERSAL        0x00
#define ASN1_CLASS_APPLICATION      0x40
#define ASN1_CLASS_CONTEXT_SPECIFIC 0x80
#define ASN1_CLASS_PRIVATE          0xC0

enum Asn1Type : uint8_t
{
	ASN1_TYPE_BOOLEAN = 1,
	ASN1_TYPE_INTEGER = 2,
	ASN1_TYPE_BIT_STRING = 3,
	ASN1_TYPE_OCTET_STRING = 4,
	ASN1_TYPE_NULL = 5,
	ASN1_TYPE_OBJECT_IDENTIFIER = 6,
	ASN1_TYPE_OBJECT_DESCRIPTOR = 7,
	ASN1_TYPE_EXTERNAL = 8,
	ASN1_TYPE_REAL = 9,
	ASN1_TYPE_ENUMERATED = 10,
	ASN1_TYPE_UTF8_STRING = 12,
	ASN1_TYPE_SEQUENCE = 16,
	ASN1_TYPE_SET = 17,
	ASN1_TYPE_NUMERIC_STRING = 18,
	ASN1_TYPE_PRINTABLE_STRING = 19,
	ASN1_TYPE_TELETEX_STRING = 20,
	ASN1_TYPE_VIDEOTEX_STRING = 21,
	ASN1_TYPE_IA5_STRING = 22,
	ASN1_TYPE_UTC_TIME = 23,
	ASN1_TYPE_GENERALIZED_TIME = 24,
	ASN1_TYPE_GRAPHIC_STRING = 25,
	ASN1_TYPE_VISIBLE_STRING = 26,
	ASN1_TYPE_GENERAL_STRING = 27,
	ASN1_TYPE_UNIVERSAL_STRING = 28,
	ASN1_TYPE_BMP_STRING = 30,
};


struct Asn1Tag
{
   bool constructed;
   uint32_t objClass;
   Asn1Type objType;
   size_t length;
   const uint8_t* value;
   size_t totalLength;

   RES_CODE asn1ReadTag(const uint8_t *data, size_t len);
   RES_CODE asn1ReadInt32(const uint8_t *data, size_t len, int32_t *val);

   RES_CODE asn1WriteTag(bool reverse, uint8_t* data, size_t* written);

   RES_CODE asn1CheckTag(bool cnstrd, uint32_t cls, uint32_t type) const;
   RES_CODE asn1CheckOid(const uint8_t *oid, size_t len) const;
};

uint32_t asn1WriteTagLen(Asn1Type objType, size_t len);

RES_CODE asn1DumpObject(const uint8_t* data, size_t length, uint32_t level);

#endif /* SERVICES_SECURITY_CRYPTO_ASN1_H_ */
