/*
 * asn1.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <asn1.h>
#include <oid.h>

RES_CODE Asn1Tag::asn1ReadTag(const uint8_t *data, size_t len)
{
	uint32_t i;
	uint32_t n;

	//Make sure the identifier octet is present
	if (!len)
		return RES_TLS_INVALID_TAG;

	//Save the class of the ASN.1 tag
	objClass = data[0] & ASN1_CLASS_MASK;
	//Primitive or constructed encoding?
	constructed = (data[0] & ASN1_ENCODING_CONSTRUCTED) ? true : false;

	//Check the tag number
	if ((data[0] & ASN1_TAG_NUMBER_MASK) < 31)
	{
		//Tag number is in the range 0 to 30
		objType = (Asn1Type)(data[0] & ASN1_TAG_NUMBER_MASK);
		//Point to the tag length field
		i = 1;
	}
	else
	{
		//If the tag number is greater than or equal to 31,
		//the subsequent octets will encode the tag number
		objType = (Asn1Type)0;

		//Decode the tag number
		for (i = 1;; i++)
		{
			//The field cannot exceed 5 bytes
			if (i > (sizeof(objType) + 1))
				return RES_TLS_INVALID_TAG;
			//Insufficient number of bytes to decode the tag number?
			if (!(len - i))
				return RES_TLS_INVALID_TAG;

			//Update the tag number with bits 7 to 1
			objType = (Asn1Type)((objType << 7) | (data[i] & 0x7F));

			//Bit 8 shall be set unless it is the last octet
			if (!(data[i] & 0x80))
				break;
		}
		//Point to the tag length field
		i++;
	}

	//Insufficient number of bytes to decode the tag length?
	if (!(len - i))
		return RES_TLS_INVALID_TAG;

	//Short form is used?
	if (data[i] < 128)
	{
		//Bits 7 to 1 encode the number of bytes in the contents
		length = data[i];
		//Point to the contents of the tag
		i++;
	}
	//Long form is used?
	else if (data[i] > 128 && data[i] < 255)
	{
		//Bits 7 to 1 encode the number of octets in the length field
		n = data[i] & 0x7F;

		//The field cannot exceed 4 bytes
		if (n > sizeof(length))
			return RES_TLS_INVALID_TAG;
		//Insufficient number of bytes to decode the tag length?
		if ((len - i) < n)
			return RES_TLS_INVALID_TAG;

		//Clear the tag length
		length = 0;
		//Read the subsequent octets
		for (i++; n > 0; n--)
			length = (length << 8) | data[i++];
	}
	//Indefinite form is used?
	else
	{
		//Indefinite form is not supported
		return RES_TLS_INVALID_TAG;
	}

	//Save the pointer to the tag contents
	value = data + i;
	//Check the length of tag
	if ((len - i) < length)
		return RES_TLS_INVALID_TAG;

	//Total length occupied by the ASN.1 tag in the input stream
	totalLength = i + length;
	//ASN.1 tag successfully decoded
	return RES_OK;
}

RES_CODE Asn1Tag::asn1ReadInt32(const uint8_t *data, size_t len, int32_t *val)
{
	RES_CODE res;
	size_t i;

	//Read ASN.1 tag
	res = asn1ReadTag(data, len);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	res = asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
	if (res != RES_OK)
		return res;

	//The contents shall consist of one or more octets
	if (length < 1 || length > 4)
		return RES_TLS_INVALID_TAG;

	//The contents octets shall be a two's complement binary
	//number equal to the integer value
	*val = (value[0] & 0x80) ? -1 : 0;

	//Process contents octets
	for (i = 0; i < length; i++)
	{
		//Rotate left operation
		*val <<= 8;
		//Reconstruct integer value
		*val |= value[i];
	}

	//ASN.1 tag successfully decoded
	return RES_OK;
}

/**
 * @brief Write an ASN.1 tag
 * @param[in] tag Structure describing the ASN.1 tag
 * @param[in] reverse Use reverse encoding
 * @param[out] data Output stream where to write the tag (optional parameter)
 * @param[out] written Number of bytes written to the output stream (optional parameter)
 * @return Error code
 **/
RES_CODE Asn1Tag::asn1WriteTag(bool reverse, uint8_t* data, size_t* written)
{
	size_t i;
	size_t m;
	size_t n;

	//Compute the number of octets that are necessary to encode the tag number
	if (objType < 31)
		m = 0;
	else if (objType < 128)
		m = 1;
	else if (objType < 16384)
		m = 2;
	else if (objType < 2097152)
		m = 3;
	else if (objType < 268435456)
		m = 4;
	else
		m = 5;

	//Compute the number of octets that are necessary to encode the length field
	if (length < 128)
		n = 0;
	else if (length < 256)
		n = 1;
	else if (length < 65536)
		n = 2;
	else if (length < 16777216)
		n = 3;
	else
		n = 4;

	//Valid output stream?
	if (data != nullptr)
	{
		//Use reverse encoding?
		if (reverse)
		{
			//Any data to copy?
			if (value != nullptr && length > 0)
			{
				//Make room for the data
				data -= length;
				//Copy data
				memmove(data, value, length);
			}

			//Move backward
			data -= m + n + 2;
		}
		else
		{
			//Any data to copy?
			if (value != nullptr && length > 0)
			{
				//Copy data
				memmove(data + m + n + 2, value, length);
			}
		}

		//Save the class of the ASN.1 tag
		data[0] = objClass;

		//Primitive or constructed encoding?
		if (constructed)
			data[0] |= ASN1_ENCODING_CONSTRUCTED;

		//Encode the tag number
		if (m == 0)
		{
			//Tag number is in the range 0 to 30
			data[0] |= objType;
		}
		else
		{
			//The tag number is greater than or equal to 31
			data[0] |= ASN1_TAG_NUMBER_MASK;

			//The subsequent octets will encode the tag number
			for (i = 0; i < m; i++)
			{
				//Bits 7 to 1 encode the tag number
				data[m - i] = (objType >> (i * 7)) & 0x7F;

				//Bit 8 of each octet shall be set to one unless it is the
				//last octet of the identifier octets
				if (i != 0)
					data[m - i] |= 0x80;
			}
		}

		//Encode the length field
		if (n == 0)
		{
			//Use short form encoding
			data[1 + m] = length & 0x7F;
		}
		else
		{
			//Bits 7 to 1 encode the number of octets in the length field
			data[1 + m] = 0x80 | (n & 0x7F);

			//The subsequent octets will encode the length field
			for (i = 0; i < n; i++)
			{
				data[1 + m + n - i] = (length >> (i * 8)) & 0xFF;
			}
		}
	}

	//Total length occupied by the ASN.1 tag
	totalLength = length + m + n + 2;

	//The last parameter is optional
	if (written != nullptr)
	{
		//Number of bytes written to the output stream
		*written = m + n + 2;

		//Any data copied?
		if (value != nullptr)
			*written += length;
	}

	//Successful processing
	return RES_OK;
}

RES_CODE Asn1Tag::asn1CheckTag(bool cnstrd, uint32_t cls, uint32_t type) const
{
	//Check encoding
	if (constructed != cnstrd)
		return RES_TLS_WRONG_ENCODING;
	//Enforce class
	if (objClass != cls)
		return RES_TLS_INVALID_CLASS;
	//Enforce type
	if (objType != type)
		return RES_TLS_INVALID_TYPE;

	//The tag matches all the criteria
	return RES_OK;
}

RES_CODE Asn1Tag::asn1CheckOid(const uint8_t *oid, size_t len) const
{
	RES_CODE res;

	//Enforce encoding, class and type
	res = asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OBJECT_IDENTIFIER);
	if (res != RES_OK)
		return res;

	//Compare OID against the specified value
	if (oidComp(value, length, oid, len))
		return RES_TLS_WRONG_IDENTIFIER;

	//The tag matches all the criteria
	return RES_OK;
}

uint32_t asn1WriteTagLen(Asn1Type objType, size_t len)
{
	//Compute the number of octets that are necessary to encode the length field
	if (len> 127)
	{
		if (len < 256)
			len += 1;
		else if (len < 65536)
			len += 2;
		else if (len < 16777216)
			len += 3;
		else
			len += 4;
	}

	//Compute the number of octets that are necessary to encode the tag number
	if (objType > 30)
	{
		if (objType < 128)
			len += 1;
		else if (objType < 16384)
			len += 2;
		else if (objType < 2097152)
			len += 3;
		else if (objType < 268435456)
			len += 4;
		else
			len += 5;
	}
	return len + 2;
}

#ifndef TRACE_TLS_LEVEL
#define TRACE_TLS_LEVEL TRACE_LEVEL_NONE
#endif

#if (TRACE_AND_LEVEL >= TRACE_LEVEL_DEBUG)
static void oidDumpAsString(const uint8_t *oid, size_t oidLen)
{
   size_t i;
   uint32_t value;

   //Check the length of the OID
   if(oidLen > 0)
   {
      //Convert the first 2 bytes
      TRACE_TLS("%u.%u", oid[0] / 40, oid[0] % 40);

      //Initialize the value of the sub-identifier
      value = 0;

      //Convert the rest of the OID
      for(i = 1; i < oidLen; i++)
      {
         //Shift the value to the left
         value <<= 7;
         //Update the current value
         value |= oid[i] & OID_VALUE_MASK;

         //Bit b8 is set to zero to indicate the last byte
         if(!(oid[i] & OID_MORE_FLAG))
         {
            //Dump current value
            TRACE_TLS(".%u" , value);

            //Prepare to decode the next value
            value = 0;
         }
      }
   }

}
//ASN.1 universal types
const char* label[32] =
{
	"[0]",							//  0
	"BOOLEAN",                      //  1
	"INTEGER",                      //  2
	"BIT STRING",                   //  3
	"OCTET STRING",                 //  4
	"NULL",                         //  5
	"OBJECT IDENTIFIER",            //  6
	"OBJECT DESCRIPTOR",            //  7
	"EXTERNAL",                     //  8
	"REAL",                         //  9
	"ENUMERATED",                   // 10
	"[11]",                         // 11
	"UTF8 STRING",                  // 12
	"[13]",                         // 13
	"[14]",                         // 14
	"[15]",                         // 15
	"SEQUENCE",                     // 16
	"SET",                          // 17
	"NUMERIC STRING",               // 18
	"PRINTABLE STRING",             // 19
	"TELETEX STRING",               // 20
	"VIDEOTEX STRING",              // 21
	"IA5 STRING",                   // 22
	"UTC TIME",                     // 23
	"GENERALIZED TIME",             // 24
	"GRAPHIC STRING",               // 25
	"VISIBLE STRING",               // 26
	"GENERAL STRING",               // 27
	"UNIVERSAL STRING",             // 28
	"[29]",                         // 29
	"BMP STRING",                   // 30
	"[31]"                          // 31
};

//Prefix used to format the structure
const char* prefix[8] =
{ "", "  ", "    ", "      ", "        ", "          ", "            ",
		"              " };

void trace_tag_header(Asn1Tag& tag, uint32_t level)
{
	//Dump tag number, tag class, and contents length fields
	if (tag.objType	< 32 && (tag.objClass & ASN1_CLASS_MASK) == ASN1_CLASS_UNIVERSAL)
		TRACELN_TLS("%s%s (%u bytes) ", prefix[level],	label[tag.objType], tag.length);
	else
		TRACELN_TLS("%s[%u] (%u bytes) ", prefix[level], tag.objType, tag.length);
	tsk_sleep(1);
}

RES_CODE trace_tag_body(Asn1Tag& tag, uint32_t level)
{
	uint32_t i;

	//Check the type of the current tag
	switch (tag.objType)
	{
	//OID?
	case ASN1_TYPE_OBJECT_IDENTIFIER:
		//Append prefix
		TRACE_TLS(prefix[level + 1]);
		//Print OID
		oidDumpAsString(tag.value, tag.length);
		//Add a line feed
		TRACE_TLS("\r\n");
		break;
		//String?
	case ASN1_TYPE_UTF8_STRING:
	case ASN1_TYPE_NUMERIC_STRING:
	case ASN1_TYPE_PRINTABLE_STRING:
	case ASN1_TYPE_TELETEX_STRING:
	case ASN1_TYPE_VIDEOTEX_STRING:
	case ASN1_TYPE_IA5_STRING:
	case ASN1_TYPE_GRAPHIC_STRING:
	case ASN1_TYPE_VISIBLE_STRING:
	case ASN1_TYPE_GENERAL_STRING:
	case ASN1_TYPE_UNIVERSAL_STRING:
	case ASN1_TYPE_BMP_STRING:
		//Append prefix
		TRACE1_TLS(prefix[level + 1]);
		//Dump the entire string
		for (i = 0; i < tag.length; i++)
			TRACE_TLS("%c", tag.value[i]);
		//Add a line feed
		TRACE_TLS("\r\n");
		break;
		//UTC time?
	case ASN1_TYPE_UTC_TIME:
		//Check length
		if (tag.length != 13)
			return RES_TLS_WRONG_ENCODING;
		//The encoding shall terminate with a "Z"
		if (tag.value[tag.length - 1] != 'Z')
			return RES_TLS_WRONG_ENCODING;

		//Append prefix
		TRACE1_TLS(prefix[level + 1]);
		//Display date
		TRACE_TLS("%c%c/%c%c/%c%c ", tag.value[0], tag.value[1],
				tag.value[2], tag.value[3], tag.value[4], tag.value[5]);
		//Display time
		TRACE_TLS("%c%c:%c%c:%c%c", tag.value[6], tag.value[7],
				tag.value[8], tag.value[9], tag.value[10],
				tag.value[11]);
		//Add a line feed
		TRACE_TLS("\r\n");
		break;
		//Generalized time?
	case ASN1_TYPE_GENERALIZED_TIME:
		//Check length
		if (tag.length != 15)
			return RES_TLS_WRONG_ENCODING;
		//The encoding shall terminate with a "Z"
		if (tag.value[tag.length - 1] != 'Z')
			return RES_TLS_WRONG_ENCODING;

		//Append prefix
		TRACE1_TLS(prefix[level + 1]);
		//Display date
		TRACE_TLS("%c%c%c%c/%c%c/%c%c ", tag.value[0], tag.value[1],
				tag.value[2], tag.value[3], tag.value[4], tag.value[5],
				tag.value[6], tag.value[7]);
		//Display time
		TRACE_TLS("%c%c:%c%c:%c%c", tag.value[8], tag.value[9],
				tag.value[10], tag.value[11], tag.value[12],
				tag.value[13]);
		//Add a line feed
		TRACE_TLS("\r\n");
		break;
		//Any other type?
	default:
		//Dump the contents of the tag
		TRACE_TLS_ARRAY(prefix[level + 1], tag.value, tag.length);
		break;
	}
	tsk_sleep(2);
	return RES_OK;
}
#endif


RES_CODE asn1DumpObject(const uint8_t* data, size_t length, uint32_t level)
{
	//Check debugging level
#if (TRACE_AND_LEVEL >= TRACE_LEVEL_DEBUG)
	RES_CODE res;
	Asn1Tag tag;


	//Parse ASN.1 object
	while (length > 0)
	{
		//Decode current ASN.1 tag
		res = tag.asn1ReadTag(data, length);
		//Decoding failed?
		if (res != RES_OK)
			return res;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		trace_tag_header(tag, level);

		//Constructed type?
		if (tag.constructed)
		{
			//Check whether the maximum level of recursion is reached
			if (level < 7)
			{
				//Recursive decoding of the ASN.1 tag
				res = asn1DumpObject(tag.value, tag.length, level + 1);
				if (res != RES_OK)
					return res;
			}
			else
			{
				//If the maximum level of recursion is reached, then dump contents
				TRACE_TLS_ARRAY(prefix[level + 1], tag.value, tag.length);
			}
		}
		//Primitive type?
		else
		{
			res = trace_tag_body(tag, level);
			if (res != RES_OK)
				return res;
		}
	}
#endif

	//ASN.1 object successfully decoded
	return RES_OK;
}
