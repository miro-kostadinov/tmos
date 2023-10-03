/*
 * pem.cpp
 *
 *  Created on: Aug 1, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <pem.h>
#include <base64.h>
#include <asn1.h>

#if PEM_SUPPORT

/**
 * @brief Search a string for a given tag
 * @param[in] s String to search
 * @param[in] sLen Length of the string to search
 * @param[in] tag String containing the tag to search for
 * @param[in] tagLen Length of the tag
 * @return The index of the first occurrence of the tag in the string,
 *   or -1 if the tag does not appear in the string
 **/

static int pemSearchTag(const char *s, size_t sLen, const char* tag, size_t tagLen)
{
	size_t i;
	size_t j;

	//Loop through input string
	for (i = 0; (i + tagLen) <= sLen; i++)
	{
		//Compare current substring with the given tag
		for (j = 0; j < tagLen; j++)
		{
			if (s[i + j] != tag[j])
				break;
		}

		//Check whether the tag has been found
		if (j == tagLen)
			return i;
	}

	//The tag does not appear in the string
	return -1;
}

/**
 * @brief Decode a PEM file containing a certificate
 * @param[in,out] input Pointer to the PEM structure
 * @param[in,out] inputLength Length of the PEM structure
 * @param[in,out] output Pointer to the DER encoded certificate
 * @param[in,out] outputSize Size of the memory block that holds the DER certificate
 * @param[out] outputLength Length of the DER encoded certificate
 * @return Error code
 **/
RES_CODE pemReadCertificate(const char** input, size_t* inputLength,
   uint8_t** output, size_t* outputSize, size_t* outputLength)
{
	RES_CODE res;
	size_t length;
	size_t i;
	size_t j;
	int k;

	//Check parameters
	if (input == nullptr || inputLength == nullptr)
		return RES_TLS_INVALID_PARAMETER;
	if (output == nullptr || outputSize == nullptr || outputLength == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Search for the beginning tag
	k = pemSearchTag(*input, *inputLength, "-----BEGIN CERTIFICATE-----", 27);
	//Failed to find the specified tag?
	if (k < 0)
		return RES_EOF;

	//Advance the input pointer over the tag
	*input += k + 27;
	*inputLength -= k + 27;

	//Search for the end tag
	k = pemSearchTag(*input, *inputLength, "-----END CERTIFICATE-----", 25);
	//Invalid PEM file?
	if (k <= 0)
		return RES_TLS_INVALID_SYNTAX;

	//Length of the PEM structure
	length = k;

	//Increase buffer size?
	if (length > *outputSize)
	{
		//Release previously allocated buffer if necessary
		if (*output != nullptr)
		{
			tsk_free(*output);
			*output = nullptr;
			*outputSize = 0;
		}

		//Allocate a memory buffer to hold the decoded data
		*output = (uint8_t*)tsk_malloc(length);
		//Failed to allocate memory?
		if (*output == nullptr)
			return RES_OUT_OF_MEMORY;

		//Record the size of the buffer
		*outputSize = length;
	}

	//Copy the contents of the PEM structure
	memcpy(*output, *input, length);

	//Advance the input pointer over the certificate
	*input += length + 25;
	*inputLength -= length + 25;

	//Remove carriage returns and line feeds
	for (i = 0, j = 0; i < length; i++)
	{
		if ((*output)[i] != '\r' && (*output)[i] != '\n')
			(*output)[j++] = (*output)[i];
	}

	//Start of exception handling block
	do
	{
		//The PEM file is Base64 encoded...
		res = base64Decode((char *) *output, j, *output, &length);
		//Failed to decode the file?
		if (res != RES_OK)
			break;

		//Display ASN.1 structure
		res = asn1DumpObject(*output, length, 0);
		//Any res to report?
		if (res != RES_OK)
			break;

		//End of exception handling block
	} while (0);

	//Clean up side effects
	if (res != RES_OK)
	{
		//Release previously allocated memory
		tsk_free(*output);
		*output = nullptr;
		*outputSize = 0;
	}

	//Size of the decoded certificate
	*outputLength = length;
	//Return status code
	return res;
}

/**
 * @brief Decode a PEM file containing a RSA private key
 * @param[in] input Pointer to the PEM structure
 * @param[in] length Length of the PEM structure
 * @param[out] key RSA private key resulting from the parsing process
 * @return Error code
 **/
RES_CODE pemReadRsaPrivateKey(const char* input, size_t length, RsaPrivateKey* key)
{
#if RSA_SUPPORT
	RES_CODE res;
	size_t i;
	size_t j;
	int32_t k;
	char* buffer;
	const uint8_t* data;
	Asn1Tag tag;

	//Check parameters
	if (input == nullptr && length != 0)
		return RES_TLS_INVALID_PARAMETER;
	if (key == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Search for the beginning tag
	k = pemSearchTag(input, length, "-----BEGIN RSA PRIVATE KEY-----", 31);
	//Failed to find the specified tag?
	if (k < 0)
		return RES_TLS_INVALID_SYNTAX;

	//Advance the pointer over the tag
	input += k + 31;
	length -= k + 31;

	//Search for the end tag
	k = pemSearchTag(input, length, "-----END RSA PRIVATE KEY-----", 29);
	//Invalid PEM file?
	if (k <= 0)
		return RES_TLS_INVALID_SYNTAX;

	//Length of the PEM structure
	length = k;

	//Allocate a memory buffer to hold the decoded data
	buffer = (char*)tsk_malloc(length);
	//Failed to allocate memory?
	if (buffer == nullptr)
		return RES_OUT_OF_MEMORY;

	//Copy the contents of the PEM structure
	memcpy(buffer, input, length);

	//Remove carriage returns and line feeds
	for (i = 0, j = 0; i < length; i++)
	{
		if (buffer[i] != '\r' && buffer[i] != '\n')
			buffer[j++] = buffer[i];
	}

	//Start of exception handling block
	do
	{
		//The PEM file is Base64 encoded...
		res = base64Decode(buffer, j, buffer, &length);
		if (res != RES_OK)
			break;

		//Point to the resulting ASN.1 structure
		data = (uint8_t *) buffer;

		//Display ASN.1 structure
		res = asn1DumpObject(data, length, 0);
		if (res != RES_OK)
			break;

		//The RSA private key is encapsulated within a sequence
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
		if (res != RES_OK)
			break;

		//Point to the first field of the sequence
		data = tag.value;
		length = tag.length;

		//Read the version
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Skip the version field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the modulus
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the modulus to a multiple precision integer
		res = key->n.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the public exponent
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the public exponent to a multiple precision integer
		res = key->e.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the private exponent
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the private exponent to a multiple precision integer
		res = key->d.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the first factor
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the first factor to a multiple precision integer
		res = key->p.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the second factor
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the second factor to a multiple precision integer
		res = key->q.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the first exponent
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the first exponent to a multiple precision integer
		res = key->dp.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the second exponent
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the second exponent to a multiple precision integer
		res = key->dq.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the coefficient
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the coefficient to a multiple precision integer
		res = key->qinv.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Debug message
		TRACE1_TLS("RSA private key:\r\n  Modulus:\r\n");
		TRACE_MPI("    ", &key->n);
		TRACE1_TLS("  Public exponent:\r\n");
		TRACE_MPI("    ", &key->e);
		TRACE1_TLS("  Private exponent:\r\n");
		TRACE_MPI("    ", &key->d);
		TRACE1_TLS("  Prime 1:\r\n");
		TRACE_MPI("    ", &key->p);
		TRACE1_TLS("  Prime 2:\r\n");
		TRACE_MPI("    ", &key->q);
		TRACE1_TLS("  Prime exponent 1:\r\n");
		TRACE_MPI("    ", &key->dp);
		TRACE1_TLS("  Prime exponent 2:\r\n");
		TRACE_MPI("    ", &key->dq);
		TRACE1_TLS("  Coefficient:\r\n");
		TRACE_MPI("    ", &key->qinv);

		//End of exception handling block
	} while (0);

	//Release previously allocated memory
	tsk_free(buffer);

	return res;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Decode a PEM file containing a DSA private key
 * @param[in] input Pointer to the PEM structure
 * @param[in] length Length of the PEM structure
 * @param[out] key DSA private key resulting from the parsing process
 * @return Error code
 **/
RES_CODE pemReadDsaPrivateKey(const char* input, size_t length, DsaPrivateKey* key)
{
#if DSA_SUPPORT
	RES_CODE res;
	size_t i, j;
	int32_t k;
	char* buffer;
	const uint8_t* data;
	Asn1Tag tag;

	//Check parameters
	if (input == nullptr && length != 0)
		return RES_TLS_INVALID_PARAMETER;
	if (key == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Search for the beginning tag
	k = pemSearchTag(input, length, "-----BEGIN DSA PRIVATE KEY-----", 31);
	//Failed to find the specified tag?
	if (k < 0)
		return RES_TLS_INVALID_SYNTAX;

	//Advance the pointer over the tag
	input += k + 31;
	length -= k + 31;

	//Search for the end tag
	k = pemSearchTag(input, length, "-----END DSA PRIVATE KEY-----", 29);
	//Invalid PEM file?
	if (k <= 0)
		return RES_TLS_INVALID_SYNTAX;

	//Length of the PEM structure
	length = k;

	//Allocate a memory buffer to hold the decoded data
	buffer = (char*)tsk_malloc(length);
	if (buffer == nullptr)
		return RES_OUT_OF_MEMORY;

	//Copy the contents of the PEM structure
	memcpy(buffer, input, length);

	//Remove carriage returns and line feeds
	for (i = 0, j = 0; i < length; i++)
	{
		if (buffer[i] != '\r' && buffer[i] != '\n')
			buffer[j++] = buffer[i];
	}

	//Start of exception handling block
	do
	{
		//The PEM file is Base64 encoded...
		res = base64Decode(buffer, j, buffer, &length);
		if (res != RES_OK)
			break;

		//Point to the resulting ASN.1 structure
		data = (uint8_t *) buffer;

		//Display ASN.1 structure
		res = asn1DumpObject(data, length, 0);
		if (res != RES_OK)
			break;

		//The DSA private key is encapsulated within a sequence
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
		if (res != RES_OK)
			break;

		//Point to the first field of the sequence
		data = tag.value;
		length = tag.length;

		//Read the version
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Skip the version field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read p
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert p to a multiple precision integer
		res = key->p.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read q
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert q to a multiple precision integer
		res = key->q.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read g
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert g to a multiple precision integer
		res = key->g.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the public value
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the private value
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the private value to a multiple precision integer
		res = key->x.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Debug message
		TRACE1_TLS("DSA private key:\r\n"); TRACE1_TLS("  p:\r\n");
		TRACE_MPI("    ", &key->p);
		TRACE1_TLS("  q:\r\n");
		TRACE_MPI("    ", &key->q);
		TRACE1_TLS("  g:\r\n");
		TRACE_MPI("    ", &key->g);
		TRACE1_TLS("  x:\r\n");
		TRACE_MPI("    ", &key->x);

		//End of exception handling block
	} while (0);

	//Release previously allocated memory
	tsk_free(buffer);


	return res;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Decode a PEM file containing EC domain parameters
 * @param[in] input Pointer to the PEM structure
 * @param[in] length Length of the PEM structure
 * @param[out] params EC domain parameters
 * @return Error code
 **/
RES_CODE pemReadEcParameters(const char* input, size_t length, EcDomainParameters* params)
{
#if EC_SUPPORT
	RES_CODE res;
	size_t i;
	size_t j;
	int32_t k;
	char* buffer;
	const uint8_t* data;
	Asn1Tag tag;
	const EcCurveInfo* curveInfo;

	//Check parameters
	if (input == nullptr && length != 0)
		return RES_TLS_INVALID_PARAMETER;
	if (params == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Check the format of the PEM file
	if (pemSearchTag(input, length, "-----BEGIN EC PARAMETERS-----", 29) >= 0)
	{
		//Search for the beginning tag
		k = pemSearchTag(input, length, "-----BEGIN EC PARAMETERS-----", 29);
		//Failed to find the specified tag?
		if (k < 0)
			return RES_TLS_INVALID_SYNTAX;

		//Advance the pointer over the tag
		input += k + 29;
		length -= k + 29;

		//Search for the end tag
		k = pemSearchTag(input, length, "-----END EC PARAMETERS-----", 27);
		//Invalid PEM file?
		if (k <= 0)
			return RES_TLS_INVALID_SYNTAX;

		//Length of the PEM structure
		length = k;

		//Allocate a memory buffer to hold the decoded data
		buffer = (char*)tsk_malloc(length);
		//Failed to allocate memory?
		if (buffer == nullptr)
			return RES_OUT_OF_MEMORY;

		//Copy the contents of the PEM structure
		memcpy(buffer, input, length);

		//Remove carriage returns and line feeds
		for (i = 0, j = 0; i < length; i++)
		{
			if (buffer[i] != '\r' && buffer[i] != '\n')
				buffer[j++] = buffer[i];
		}

		//Start of exception handling block
		do
		{
			//The PEM file is Base64 encoded...
			res = base64Decode(buffer, j, buffer, &length);
			if (res != RES_OK)
				break;

			//Point to the resulting ASN.1 structure
			data = (uint8_t *) buffer;

			//Display ASN.1 structure
			res = asn1DumpObject(data, length, 0);
			if (res != RES_OK)
				break;

			//Read the curve identifier
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_OBJECT_IDENTIFIER);
			if (res != RES_OK)
				break;

			//Retrieve EC domain parameters
			curveInfo = ecGetCurveInfo(tag.value, tag.length);
			//Make sure the specified elliptic curve is supported
			if (curveInfo == nullptr)
			{
				//Report an error
				res = RES_TLS_ILLEGAL_PARAMETER;
				break;
			}

			//Load EC domain parameters
			res = params->ecLoadDomainParameters(curveInfo);
			if (res != RES_OK)
				break;

			//End of exception handling block
		} while (0);
	}
	else
	{
		//Search for the beginning tag
		k = pemSearchTag(input, length, "-----BEGIN PRIVATE KEY-----", 27);
		//Failed to find the specified tag?
		if (k < 0)
			return RES_TLS_INVALID_SYNTAX;

		//Advance the pointer over the tag
		input += k + 27;
		length -= k + 27;

		//Search for the end tag
		k = pemSearchTag(input, length, "-----END PRIVATE KEY-----", 25);
		//Invalid PEM file?
		if (k <= 0)
			return RES_TLS_INVALID_SYNTAX;

		//Length of the PEM structure
		length = k;

		//Allocate a memory buffer to hold the decoded data
		buffer = (char*)tsk_malloc(length);
		if (buffer == nullptr)
			return RES_OUT_OF_MEMORY;

		//Copy the contents of the PEM structure
		memcpy(buffer, input, length);

		//Remove carriage returns and line feeds
		for (i = 0, j = 0; i < length; i++)
		{
			if (buffer[i] != '\r' && buffer[i] != '\n')
				buffer[j++] = buffer[i];
		}

		//Start of exception handling block
		do
		{
			//The PEM file is Base64 encoded...
			res = base64Decode(buffer, j, buffer, &length);
			if (res != RES_OK)
				break;

			//Point to the resulting ASN.1 structure
			data = (uint8_t *) buffer;

			//Display ASN.1 structure
			res = asn1DumpObject(data, length, 0);
			if (res != RES_OK)
				break;

			//The private key is encapsulated within a sequence
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL,
					ASN1_TYPE_SEQUENCE);
			//The tag does not match the criteria?
			if (res != RES_OK)
				break;

			//Point to the first field of the sequence
			data = tag.value;
			length = tag.length;

			//Read the Version field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
			if (res != RES_OK)
				break;

			//Skip the Version field
			data += tag.totalLength;
			length -= tag.totalLength;

			//Read the PrivateKeyAlgorithmIdentifier field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
			if (res != RES_OK)
				break;

			//Point to the first field of the sequence
			data = tag.value;
			length = tag.length;

			//Read the algorithm identifier (OID)
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Check algorithm identifier
			res = tag.asn1CheckOid(EC_PUBLIC_KEY_OID, sizeof(EC_PUBLIC_KEY_OID));
			if (res != RES_OK)
				break;

			//Point to the next field
			data += tag.totalLength;
			length += tag.totalLength;

			//Read namedCurve field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_OBJECT_IDENTIFIER);
			if (res != RES_OK)
				break;

			//Retrieve EC domain parameters
			curveInfo = ecGetCurveInfo(tag.value, tag.length);
			//Make sure the specified elliptic curve is supported
			if (curveInfo == nullptr)
			{
				//Report an error
				res = RES_TLS_ILLEGAL_PARAMETER;
				//Exit immediately
				break;
			}

			//Load EC domain parameters
			res = params->ecLoadDomainParameters(curveInfo);
			if (res != RES_OK)
				break;

			//End of exception handling block
		} while (0);
	}

	//Release previously allocated memory
	tsk_free(buffer);

	//Return status code
	return res;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Decode a PEM file containing an EC private key
 * @param[in] input Pointer to the PEM structure
 * @param[in] length Length of the PEM structure
 * @param[out] key EC private key resulting from the parsing process
 * @return Error code
 **/
RES_CODE pemReadEcPrivateKey(const char* input, size_t length, Mpi* key)
{
#if EC_SUPPORT
	RES_CODE res;
	size_t i;
	size_t j;
	int32_t k;
	char* buffer;
	const uint8_t* data;
	Asn1Tag tag;

	//Check parameters
	if (input == nullptr && length != 0)
		return RES_TLS_INVALID_PARAMETER;
	if (key == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Check the format of the PEM file
	if (pemSearchTag(input, length, "-----BEGIN EC PRIVATE KEY-----", 30) >= 0)
	{
		//Search for the beginning tag
		k = pemSearchTag(input, length, "-----BEGIN EC PRIVATE KEY-----", 30);
		//Failed to find the specified tag?
		if (k < 0)
			return RES_TLS_INVALID_SYNTAX;

		//Advance the pointer over the tag
		input += k + 30;
		length -= k + 30;

		//Search for the end tag
		k = pemSearchTag(input, length, "-----END EC PRIVATE KEY-----", 28);
		//Invalid PEM file?
		if (k <= 0)
			return RES_TLS_INVALID_SYNTAX;

		//Length of the PEM structure
		length = k;

		//Allocate a memory buffer to hold the decoded data
		buffer = (char*)tsk_malloc(length);
		//Failed to allocate memory?
		if (buffer == nullptr)
			return RES_OUT_OF_MEMORY;

		//Copy the contents of the PEM structure
		memcpy(buffer, input, length);

		//Remove carriage returns and line feeds
		for (i = 0, j = 0; i < length; i++)
		{
			if (buffer[i] != '\r' && buffer[i] != '\n')
				buffer[j++] = buffer[i];
		}

		//Start of exception handling block
		do
		{
			//The PEM file is Base64 encoded...
			res = base64Decode(buffer, j, buffer, &length);
			if (res != RES_OK)
				break;

			//Point to the resulting ASN.1 structure
			data = (uint8_t *) buffer;

			//Display ASN.1 structure
			res = asn1DumpObject(data, length, 0);
			if (res != RES_OK)
				break;

			//The private key is encapsulated within a sequence
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
			if (res != RES_OK)
				break;

			//Point to the first field of the sequence
			data = tag.value;
			length = tag.length;

			//Read the Version field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
			if (res != RES_OK)
				break;

			//Skip the Version field
			data += tag.totalLength;
			length -= tag.totalLength;

			//Read the PrivateKey field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING);
			if (res != RES_OK)
				break;

			//Read the EC private key
			res = key->mpiReadRaw(tag.value, tag.length);
			if (res != RES_OK)
				break;

			//Debug message
			TRACE1_TLS("EC private key:\r\n");
			TRACE_MPI("  ", key);

			//End of exception handling block
		} while (0);
	}
	else
	{
		//Search for the beginning tag
		k = pemSearchTag(input, length, "-----BEGIN PRIVATE KEY-----", 27);
		//Failed to find the specified tag?
		if (k < 0)
			return RES_TLS_INVALID_SYNTAX;

		//Advance the pointer over the tag
		input += k + 27;
		length -= k + 27;

		//Search for the end tag
		k = pemSearchTag(input, length, "-----END PRIVATE KEY-----", 25);
		//Invalid PEM file?
		if (k <= 0)
			return RES_TLS_INVALID_SYNTAX;

		//Length of the PEM structure
		length = k;

		//Allocate a memory buffer to hold the decoded data
		buffer = (char*)tsk_malloc(length);
		if (buffer == nullptr)
			return RES_OUT_OF_MEMORY;

		//Copy the contents of the PEM structure
		memcpy(buffer, input, length);

		//Remove carriage returns and line feeds
		for (i = 0, j = 0; i < length; i++)
		{
			if (buffer[i] != '\r' && buffer[i] != '\n')
				buffer[j++] = buffer[i];
		}

		//Start of exception handling block
		do
		{
			//The PEM file is Base64 encoded...
			res = base64Decode(buffer, j, buffer, &length);
			if (res != RES_OK)
				break;

			//Point to the resulting ASN.1 structure
			data = (uint8_t *) buffer;

			//Display ASN.1 structure
			res = asn1DumpObject(data, length, 0);
			if (res != RES_OK)
				break;

			//The private key is encapsulated within a sequence
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
			if (res != RES_OK)
				break;

			//Point to the first field of the sequence
			data = tag.value;
			length = tag.length;

			//Read the Version field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
			if (res != RES_OK)
				break;

			//Skip the Version field
			data += tag.totalLength;
			length -= tag.totalLength;

			//Read the PrivateKeyAlgorithmIdentifier field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
			if (res != RES_OK)
				break;

			//Skip the PrivateKeyAlgorithmIdentifier field
			data += tag.totalLength;
			length -= tag.totalLength;

			//The PrivateKey field is encapsulated within an octet string
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_OCTET_STRING);
			if (res != RES_OK)
				break;

			//Display ASN.1 structure
			res = asn1DumpObject(tag.value, tag.length, 0);
			if (res != RES_OK)
				break;

			//Read the contents of the PrivateKey structure
			res = tag.asn1ReadTag(tag.value, tag.length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
			if (res != RES_OK)
				break;

			//Point to the first field of the sequence
			data = tag.value;
			length = tag.length;

			//Read the Version field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
			if (res != RES_OK)
				break;

			//Skip the Version field
			data += tag.totalLength;
			length -= tag.totalLength;

			//Read the PrivateKey field
			res = tag.asn1ReadTag(data, length);
			if (res != RES_OK)
				break;

			//Enforce encoding, class and type
			res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_OCTET_STRING);
			if (res != RES_OK)
				break;

			//Read the EC private key
			res = key->mpiReadRaw(tag.value, tag.length);
			if (res != RES_OK)
				break;

			//Debug message
			TRACE1_TLS("EC private key:\r\n");
			TRACE_MPI("  ", key);

			//End of exception handling block
		} while (0);
	}

	//Release previously allocated memory
	tsk_free(buffer);

	//Return status code
	return res;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

RES_CODE pemEncodeFile(const void* input, size_t inputLen, const char* label,
   char* output, size_t* outputLen)
{
   size_t n;
   size_t labelLen;
   char *p;

   //Check parameters
   if(label == nullptr || outputLen == nullptr)
      return RES_TLS_INVALID_PARAMETER;

   //Calculate the length of the label
   labelLen = strlen(label);

   //Encode the ASN.1 data using Base64
   base64Encode(input, inputLen, output, &n);

   //If the output parameter is NULL, then the function calculates the length
   //of the resulting PEM file without copying any data
   if(output != nullptr)
   {
      //A PEM file starts with a beginning tag
      p = output + strlen("-----BEGIN -----\r\n") + labelLen;

      //Make room for the beginning tag
      memmove(p, output, n);

      //The type of data encoded is labeled depending on the type label in
      //the "-----BEGIN " line (refer to RFC 7468, section 2)
      strcpy(output, "-----BEGIN ");
      strcpy(output + 11, label);
      memcpy(p - 7, "-----\r\n", 7);

      //Generators must put the same label on the "-----END " line as the
      //corresponding "-----BEGIN " line
      strcpy(p + n, "\r\n-----END ");
      strcpy(p + n + 11, label);
      strcpy(p + n + labelLen + 11, "-----\r\n");
   }

   //Consider the length of the PEM tags
   n += strlen("-----BEGIN -----\r\n") + labelLen;
   n += strlen("\r\n-----END -----\r\n") + labelLen;

   //Total number of bytes that have been written
   *outputLen = n;

   //Successful processing
   return RES_OK;
}


#endif // PEM_SUPPORT
