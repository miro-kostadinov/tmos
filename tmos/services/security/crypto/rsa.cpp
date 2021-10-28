/*
 * rsa.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <rsa.h>
#include <hmac.h>
#include <asn1.h>
#include <oid.h>

#if RSA_SUPPORT

//PKCS #1 OID (1.2.840.113549.1.1)
const uint8_t PKCS1_OID[8] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01};
//RSA encryption OID (1.2.840.113549.1.1.1)
const uint8_t RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01};
//MD5 with RSA encryption OID (1.2.840.113549.1.1.4)
const uint8_t MD5_WITH_RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x04};
//SHA-1 with RSA encryption OID (1.2.840.113549.1.1.5)
const uint8_t SHA1_WITH_RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x05};
//SHA-256 with RSA encryption OID (1.2.840.113549.1.1.11)
const uint8_t SHA256_WITH_RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0B};
//SHA-384 with RSA encryption OID (1.2.840.113549.1.1.12)
const uint8_t SHA384_WITH_RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0C};
//SHA-512 with RSA encryption OID (1.2.840.113549.1.1.13)
const uint8_t SHA512_WITH_RSA_ENCRYPTION_OID[9] = {0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x0D};
//RSA PKCS #1 v1.5 signature with SHA-3-224 OID (2.16.840.1.101.3.4.3.13)
const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_224_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0D};
//RSA PKCS #1 v1.5 signature with SHA-3-256 OID (2.16.840.1.101.3.4.3.14)
const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_256_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0E};
//RSA PKCS #1 v1.5 signature with SHA-3-384 OID (2.16.840.1.101.3.4.3.15)
const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_384_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0F};
//RSA PKCS #1 v1.5 signature with SHA-3-512 OID (2.16.840.1.101.3.4.3.16)
const uint8_t RSASSA_PKCS1_v1_5_WITH_SHA3_512_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x10};


void RsaPublicKey::rspk_free()
{
	n.mpi_free();
	e.mpi_free();
}

RES_CODE RsaPublicKey::rsaep(const Mpi* m, Mpi* c) const
{
	//Ensure the RSA public key is valid
	if (!n.mpi_size || !e.mpi_size)
		return RES_TLS_INVALID_PARAMETER;

	//The message representative m shall be between 0 and n - 1
	if (m->mpiCompInt(0) < 0 || m->mpiComp(&n) >= 0)
		return RES_OUT_OF_RANGE;

	//Perform modular exponentiation (c = m ^ e mod n)
	return c->mpiExpMod(m, &e, &n);
}

RES_CODE RsaPublicKey::rsavp1(const Mpi* s, Mpi* m) const
{
	//RSAVP1 primitive is the same as RSAEP except for the names of its
	//input and output arguments. They are distinguished as they are
	//intended for different purposes
	return rsaep(s, m);
}

RES_CODE RsaPublicKey::rsassaPkcs1v15Verify(const hash_info_t* hinfo, const uint8_t* digest,
		const uint8_t* signature, size_t signatureLength) const
{
	RES_CODE res;
	uint32_t k;
	uint8_t *em;
	const uint8_t *oid;
	size_t oidLength;
	const uint8_t *d;
	size_t dLength;
	Mpi s;
	Mpi m;

	//Check parameters
	if (hinfo == nullptr || digest == nullptr || signature == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Get the length in octets of the modulus n
	k = n.mpiGetByteLength();

	//Check the length of the signature
	if (signatureLength != k)
		return RES_TLS_INVALID_LENGTH;

	//Allocate a memory buffer to hold the encoded message
	em = (uint8_t*)tsk_malloc(k);
	if (em == nullptr)
		return RES_OUT_OF_MEMORY;

	//Convert the signature to an integer signature representative s
	res = s.mpiReadRaw(signature, signatureLength);
	if (res == RES_OK)
	{
		//Apply the RSAVP1 verification primitive
		res = rsavp1(&s, &m);
		if (res == RES_OK)
		{
			//Convert the message representative m to an encoded message EM of length k octets
			res = m.mpiWriteRaw(em, k);
			if (res == RES_OK)
			{
				//Parse the encoded message EM
				res = emsaPkcs1v15Decode(em, k, &oid, &oidLength, &d, &dLength);
				if (res == RES_OK)
				{
					//Assume an error...
					res = RES_TLS_INVALID_SIGNATURE_ALGO;
					//Ensure the hash algorithm identifier matches the OID
					if (dLength == hinfo->digest_size &&
							!oidComp(oid, oidLength, hinfo->oid, hinfo->oid_size))
					{
						//Compare the message digest
						res = memcmp(digest, d, dLength) ? 	RES_TLS_INVALID_SIGNATURE : RES_OK;
					}
				}
			}
		}
	}



	//Free previously allocated memory
	tsk_free(em);

	//Return status code
	return res;
}

/**
 * @brief PKCS #1 v1.5 encryption operation
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] key Recipient's RSA public key
 * @param[in] message Message to be encrypted
 * @param[in] messageLength Length of the message to be encrypted
 * @param[out] ciphertext Ciphertext resulting from the encryption operation
 * @param[out] ciphertextLength Length of the resulting ciphertext
 * @return Error code
 **/
RES_CODE RsaPublicKey::rsaesPkcs1v15Encrypt(prng_algo_t* prngAlgo, const uint8_t* message,
		size_t messageLength, uint8_t* ciphertext,
		size_t* ciphertextLength) const
{
	RES_CODE res;
	uint32_t i, j, k, nn;
	uint8_t *p;
	Mpi m;
	Mpi c;

	//Check parameters
	if (message == nullptr || ciphertext == nullptr || ciphertextLength == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACELN1_TLS("RSA PKCS #1 v1.5 encryption...");
//	TRACE_MPI("    ", &n);
//	TRACE1_TLS("  Public exponent:\r\n");
//	TRACE_MPI("    ", &e);
//	TRACE1_TLS("  Message:\r\n");
//	TRACE_TLS_ARRAY("    ", message, messageLength);


	//Get the length in octets of the modulus n
	k = n.mpiGetByteLength();

	//Check the length of the message
	if ((messageLength + 11) > k)
		return RES_TLS_INVALID_LENGTH;

	//Point to the buffer where the encoded message EM will be formatted
	p = ciphertext;

	//The leading 0x00 octet ensures that the encoded message,
	//converted to an integer, is less than the modulus
	*(p++) = 0x00;
	//For a public-key operation, the block type BT shall be 0x02
	*(p++) = 0x02;

	//Length of the padding string PS
	nn = k - messageLength - 3;

	//Generate the padding string (pseudo-randomly generated non-zero octets)
	while (nn > 0)
	{
		//Generate random data
		res = prngAlgo->prng_read(p, nn);
		if (res != RES_OK)
			return res;

		//Parse the resulting octet string
		for (i = 0, j = 0; j < nn; j++)
		{
			//Strip any byte with a value of zero
			if (p[j] != 0)
				p[i++] = p[j];
		}

		//Advance data pointer
		p += i;
		nn -= i;
	}

	//Append a 0x00 octet to the padding string
	*(p++) = 0x00;
	//Copy the message to be encrypted
	memcpy(p, message, messageLength);

	//Rewind to the beginning of the encoded message
	p = ciphertext;

	//Debug message
//	TRACE1_TLS("  Encoded message\r\n");
//	TRACE_TLS_ARRAY("    ", p, k);

	//Start of exception handling block
	do
	{
		//Convert the encoded message EM to an integer message representative m
		res = m.mpiReadRaw(p, k);
		if (res != RES_OK)
			break;

		//Apply the RSAEP encryption primitive
		res = rsaep(&m, &c);
		if (res != RES_OK)
			break;

		//Convert the ciphertext representative c to a ciphertext of length k octets
		res = c.mpiWriteRaw(ciphertext, k);
		if (res != RES_OK)
			break;

		//Length of the resulting ciphertext
		*ciphertextLength = k;

		//Debug message
//		TRACE1_TLS("  Ciphertext:\r\n");
//		TRACE_TLS_ARRAY("    ", ciphertext, k);

		//End of exception handling block
	} while (0);


	//Return status code
	return res;
}

/**
 * @brief RSA decryption primitive
 *
 * The RSA decryption primitive recovers the message representative from
 * the ciphertext representative under the control of a private key
 *
 * @param[in] key RSA private key
 * @param[in] c Ciphertext representative
 * @param[out] m Message representative
 * @return Error code
 **/
RES_CODE RsaPrivateKey::rsadp(const Mpi* c, Mpi* m) const
{
	RES_CODE res;
	Mpi m1;
	Mpi m2;
	Mpi h;

	//The ciphertext representative c shall be between 0 and n - 1
	if (c->mpiCompInt(0) < 0 || c->mpiComp(&n) >= 0)
		return RES_OUT_OF_RANGE;

	//Use the Chinese remainder algorithm?
	if (n.mpi_size && p.mpi_size && q.mpi_size && dp.mpi_size
			&& dq.mpi_size && qinv.mpi_size)
	{
		//Compute m1 = c ^ dP mod p
		MPI_CHECK(m1.mpiExpMod(c, &dp, &p));
		//Compute m2 = c ^ dQ mod q
		MPI_CHECK(m2.mpiExpMod(c, &dq, &q));
		//Let h = (m1 - m2) * qInv mod p
		MPI_CHECK(h.mpiSub(&m1, &m2));
		MPI_CHECK(h.mpiMulMod(&h, &qinv, &p));
		//Let m = m2 + q * h
		MPI_CHECK(m->mpiMul(&q, &h));
		MPI_CHECK(m->mpiAdd(m, &m2));
	}
	//Use modular exponentiation?
	else if (n.mpi_size && d.mpi_size)
	{
		//Let m = c ^ d mod n
		res = m->mpiExpMod(c, &d, &n);
	}
	//Invalid parameters?
	else
	{
		//Report an error
		res = RES_TLS_INVALID_PARAMETER;
	}

end:
	return res;
}

/**
 * @brief RSA signature primitive
 *
 * The RSA signature primitive produces a signature representative from
 * a message representative under the control of a private key
 *
 * @param[in] key RSA private key
 * @param[in] m Message representative
 * @param[out] s Signature representative
 * @return Error code
 **/
RES_CODE RsaPrivateKey::rsasp1(const Mpi* m, Mpi* s) const
{
	//RSASP1 primitive is the same as RSADP except for the names of its
	//input and output arguments. They are distinguished as they are
	//intended for different purposes
	return rsadp(m, s);
}

/**
 * @brief Generate RSA signature (SSL 3.0, TLS 1.0 and TLS 1.1)
 * @param[in] key Signer's RSA private key
 * @param[in] digest Digest of the message to be signed
 * @param[out] signature Resulting signature
 * @param[out] signatureLength Length of the resulting signature
 * @return Error code
 **/
RES_CODE RsaPrivateKey::tlsGenerateRsaSignature(const uint8_t* digest,
		uint8_t* signature, size_t signatureLength) const
{
#if (TLS_RSA_SIGN_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT)
   RES_CODE res;
   size_t paddingLength;
   uint8_t *em;
   Mpi m;
   Mpi s;

   //Debug message
   TRACE1_TLS("RSA signature generation...\r\n");
   TRACE1_TLS("  Modulus:\r\n");
   TRACE_MPI("    ", &n);
   TRACE1_TLS("  Public exponent:\r\n");
   TRACE_MPI("    ", &e);
   TRACE1_TLS("  Private exponent:\r\n");
   TRACE_MPI("    ", &d);
   TRACE1_TLS("  Prime 1:\r\n");
   TRACE_MPI("    ", &p);
   TRACE1_TLS("  Prime 2:\r\n");
   TRACE_MPI("    ", &q);
   TRACE1_TLS("  Prime exponent 1:\r\n");
   TRACE_MPI("    ", &dp);
   TRACE1_TLS("  Prime exponent 2:\r\n");
   TRACE_MPI("    ", &dq);
   TRACE1_TLS("  Coefficient:\r\n");
   TRACE_MPI("    ", &qinv);
   TRACE1_TLS("  Message digest:\r\n");
   TRACE_TLS_ARRAY("    ", digest, MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE);


   //Check the length of the modulus
   if(signatureLength < (MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE + 11))
      return RES_TLS_INVALID_KEY;

   //Point to the buffer where the encoded message EM will be generated
   em = signature;

   //The leading 0x00 octet ensures that the encoded message,
   //converted to an integer, is less than the modulus
   em[0] = 0x00;
   //Block type 0x01 is used for private-key operations
   em[1] = 0x01;

   //Compute the length of the padding string PS
   paddingLength = signatureLength - (MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE + 3);
   //Fill the padding string with 0xFF
   memset(em + 2, 0xFF, paddingLength);
   //Append a 0x00 octet to PS
   em[paddingLength + 2] = 0x00;

   //Append the digest value
   memcpy(em + paddingLength + 3, digest, MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE);

   //Debug message
   TRACE1_TLS("  Encoded message\r\n");
   TRACE_TLS_ARRAY("    ", em, signatureLength);

   //Start of exception handling block
   do
   {
      //Convert the encoded message EM to an integer message representative m
      res = m.mpiReadRaw(em, signatureLength);
      if(res != RES_OK)
         break;

      //Apply the RSASP1 signature primitive
      res = rsasp1(&m, &s);
      if(res != RES_OK)
         break;

      //Convert the signature representative s to a signature of length k octets
      res = s.mpiWriteRaw(signature, signatureLength);
      //Conversion failed?
      if(res != RES_OK)
         break;

      //Debug message
      TRACE1_TLS("  Signature:\r\n");
      TRACE_TLS_ARRAY("    ", signature, signatureLength);

      //End of exception handling block
   } while(0);


   return res;
#else
   //RSA signature generation is not supported
   return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief PKCS #1 v1.5 signature generation operation
 * @param[in] key Signer's RSA private key
 * @param[in] hash Hash function used to digest the message
 * @param[in] digest Digest of the message to be signed
 * @param[out] signature Resulting signature
 * @param[out] signatureLength Length of the resulting signature
 * @return Error code
 **/
RES_CODE RsaPrivateKey::rsassaPkcs1v15Sign(const hash_info_t* hash, const uint8_t* digest,
		uint8_t* signature, size_t signatureLength) const
{
	RES_CODE res;
	Mpi m;
	Mpi s;

	//Check parameters
	if (hash == nullptr || digest == nullptr)
		return RES_TLS_INVALID_PARAMETER;
	if (signature == nullptr || signatureLength == 0)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("RSA PKCS #1 v1.5 signature generation...\r\n"); TRACE1_TLS("  Modulus:\r\n");
	TRACE_MPI("    ", &n);
	TRACE1_TLS("  Public exponent:\r\n");
	TRACE_MPI("    ", &e);
	TRACE1_TLS("  Private exponent:\r\n");
	TRACE_MPI("    ", &d);
	TRACE1_TLS("  Prime 1:\r\n");
	TRACE_MPI("    ", &p);
	TRACE1_TLS("  Prime 2:\r\n");
	TRACE_MPI("    ", &q);
	TRACE1_TLS("  Prime exponent 1:\r\n");
	TRACE_MPI("    ", &dp);
	TRACE1_TLS("  Prime exponent 2:\r\n");
	TRACE_MPI("    ", &dq);
	TRACE1_TLS("  Coefficient:\r\n");
	TRACE_MPI("    ", &qinv);
	TRACE1_TLS("  Message digest:\r\n");
	TRACE_TLS_ARRAY("    ", digest, hash->digest_size);


	//Apply the EMSA-PKCS1-v1.5 encoding operation
	res = emsaPkcs1v15Encode(hash, digest, signature, signatureLength);
	if (res != RES_OK)
		return res;

	//Debug message
	TRACE1_TLS("  Encoded message\r\n");
	TRACE_TLS_ARRAY("    ", signature, signatureLength);

	//Start of exception handling block
	do
	{
		//Convert the encoded message EM to an integer message representative m
		res = m.mpiReadRaw(signature, signatureLength);
		if (res != RES_OK)
			break;

		//Apply the RSASP1 signature primitive
		res = rsasp1(&m, &s);
		if (res != RES_OK)
			break;

		//Convert the signature representative s to a signature of length k octets
		res = s.mpiWriteRaw(signature, signatureLength);
		if (res != RES_OK)
			break;

		//Debug message
		TRACE1_TLS("  Signature:\r\n");
		TRACE_TLS_ARRAY("    ", signature, signatureLength);

		//End of exception handling block
	} while (0);

	return res;
}

/**
 * @brief PKCS #1 v1.5 decryption operation
 * @param[in] key Recipient's RSA private key
 * @param[in] ciphertext Ciphertext to be decrypted
 * @param[in] ciphertextLength Length of the ciphertext to be decrypted
 * @param[out] message Output buffer where to store the decrypted message
 * @param[in] messageSize Size of the output buffer
 * @param[out] messageLength Length of the decrypted message
 * @return Error code
 **/
RES_CODE RsaPrivateKey::rsaesPkcs1v15Decrypt(const uint8_t* ciphertext,
		size_t ciphertextLength, uint8_t* message, size_t messageSize,
		size_t* messageLength) const
{
	RES_CODE res;
	uint32_t i;
	uint32_t k;
	uint8_t* em;
	Mpi c;
	Mpi m;

	//Check parameters
	if (ciphertext == nullptr || message == nullptr || messageLength == 0)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("RSA PKCS #1 v1.5 decryption...\r\n  Modulus:\r\n");
	TRACE_MPI("    ", &n);
	TRACE1_TLS("  Public exponent:\r\n");
	TRACE_MPI("    ", &e);
	TRACE1_TLS("  Private exponent:\r\n");
	TRACE_MPI("    ", &d);
	TRACE1_TLS("  Prime 1:\r\n");
	TRACE_MPI("    ", &p);
	TRACE1_TLS("  Prime 2:\r\n");
	TRACE_MPI("    ", &q);
	TRACE1_TLS("  Prime exponent 1:\r\n");
	TRACE_MPI("    ", &dp);
	TRACE1_TLS("  Prime exponent 2:\r\n");
	TRACE_MPI("    ", &dq);
	TRACE1_TLS("  Coefficient:\r\n");
	TRACE_MPI("    ", &qinv);
	TRACE1_TLS("  Ciphertext:\r\n");
	TRACE_TLS_ARRAY("    ", ciphertext, ciphertextLength);

	//Get the length in octets of the modulus n
	k = n.mpiGetByteLength();

	//Check the length of the ciphertext
	if (ciphertextLength != k || ciphertextLength < 11)
		return RES_TLS_INVALID_LENGTH;

	//Allocate a buffer to store the encoded message EM
	em = (uint8_t*)tsk_malloc(k);
	//Failed to allocate memory?
	if (em == nullptr)
		return RES_OUT_OF_MEMORY;

	//Start of exception handling block
	do
	{
		//Convert the ciphertext to an integer ciphertext representative c
		res = c.mpiReadRaw(ciphertext, ciphertextLength);
		if (res != RES_OK)
			break;

		//Apply the RSADP decryption primitive
		res = rsadp(&c, &m);
		if (res != RES_OK)
			break;

		//Convert the message representative m to an encoded message EM of length k octets
		res = m.mpiWriteRaw(em, k);
		//Conversion failed?
		if (res != RES_OK)
			break;

		//Debug message
		TRACE1_TLS("  Encoded message\r\n");
		TRACE_TLS_ARRAY("    ", em, k);

		//The first octet of EM must have a value of 0x00
		//and the block type BT shall be 0x02
		if (em[0] != 0x00 || em[1] != 0x02)
		{
			//Report an error
			res = RES_TLS_UNEXPECTED_VALUE;
			break;
		}

		//An octet with hexadecimal value 0x00 is used to separate PS from M
		for (i = 2; i < k && em[i] != 0x00; i++)
			;

		//Check whether the padding string is valid
		if (i < 10 || i >= k)
		{
			//Report an error
			res = RES_TLS_INVALID_PADDING;
			break;
		}

		//Ensure that the output buffer is large enough
		if (messageSize < (k - i - 1))
		{
			//Report an error
			res = RES_TLS_INVALID_LENGTH;
			break;
		}

		//Recover the length of the message
		*messageLength = k - i - 1;
		//Copy the message contents
		memcpy(message, em + i + 1, *messageLength);

		//Debug message
		TRACE1_TLS("  Message:\r\n");
		TRACE_TLS_ARRAY("    ", message, *messageLength);

		//End of exception handling block
	} while (0);

	//Free previously allocated memory
	tsk_free(em);

	//Return status code
	return res;
}

RES_CODE emsaPkcs1v15Decode(const uint8_t *em, size_t emLength, const uint8_t **oid,
   size_t *oidLength, const uint8_t **digest, size_t *digestLength)
{
	RES_CODE res;
	uint32_t i;
	size_t length;
	const uint8_t *data;
	Asn1Tag tag;

	//Check the length of the encoded message EM
	if (emLength < 11)
		return RES_TLS_INVALID_LENGTH;

	//The first octet of EM must have a value of 0x00
	if (em[0] != 0x00)
		return RES_TLS_UNEXPECTED_VALUE;
	//The block type BT shall be 0x01
	if (em[1] != 0x01)
		return RES_TLS_UNEXPECTED_VALUE;

	//Check the padding string PS
	for (i = 2; i < emLength; i++)
	{
		//A 0x00 octet indicates the end of the padding string
		if (em[i] == 0x00)
			break;
		//Each byte of PS must be set to 0xFF when the block type is 0x01
		if (em[i] != 0xFF)
			return RES_TLS_INVALID_PADDING;
	}

	//Check whether the padding string is properly terminated
	if (i >= emLength)
		return RES_TLS_INVALID_PADDING;
	//The length of PS cannot be less than 8 octets
	if (i < 10)
		return RES_TLS_INVALID_PADDING;

	//Point to the DigestInfo structure
	data = em + i + 1;
	length = emLength - i - 1;

	//Read the contents of the DigestInfo structure
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	if (!tag.constructed || tag.objType != ASN1_TYPE_SEQUENCE)
		return RES_TLS_INVALID_TAG;

	//Point to the DigestAlgorithm structure
	data = tag.value;
	length = tag.length;

	//Decode the DigestAlgorithm tag
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	if (!tag.constructed || tag.objType != ASN1_TYPE_SEQUENCE)
		return RES_TLS_INVALID_TAG;

	//Save the location of the next tag
	data += tag.totalLength;
	length -= tag.totalLength;

	//Decode the AlgorithmIdentifier tag
	res = tag.asn1ReadTag(tag.value, tag.length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	if (tag.constructed || tag.objType != ASN1_TYPE_OBJECT_IDENTIFIER)
		return RES_TLS_INVALID_TAG;

	//Save the hash algorithm OID
	*oid = tag.value;
	*oidLength = tag.length;

	//Decode the DigestValue tag
	res = tag.asn1ReadTag(data, length);
	if (res != RES_OK)
		return res;

	//Enforce encoding, class and type
	if (tag.constructed || tag.objType != ASN1_TYPE_OCTET_STRING)
		return RES_TLS_INVALID_TAG;

	//Save the hash value
	*digest = tag.value;
	*digestLength = tag.length;

	//EM successfully decoded
	return RES_OK;
}

/**
 * @brief PKCS #1 v1.5 encoding method
 * @param[in] hash Hash function used to digest the message
 * @param[in] digest Digest of the message to be signed
 * @param[out] em Encoded message
 * @param[in] emLength Intended length of the encoded message
 * @return Error code
 **/
RES_CODE emsaPkcs1v15Encode(const hash_info_t* hash, const uint8_t* digest,
		uint8_t* em, size_t emLength)
{
	uint32_t i;
	size_t paddingLength;

	//Ensure the length of the digest is valid
	if ((hash->oid_size + hash->digest_size + 21u) > emLength)
		return RES_TLS_INVALID_LENGTH;

	//The leading 0x00 octet ensures that the encoded message,
	//converted to an integer, is less than the modulus
	em[0] = 0x00;
	//Block type 0x01 is used for private-key operations
	em[1] = 0x01;

	//Compute the length of the padding string PS
	paddingLength = emLength - hash->oid_size - hash->digest_size - 13;
	//Fill the padding string with 0xFF
	memset(em + 2, 0xFF, paddingLength);

	//Point to the byte that follows PS
	i = paddingLength + 2;
	//Append a 0x00 octet to PS
	em[i++] = 0x00;

	//Encode the DigestInfo using ASN.1
	em[i++] = ASN1_ENCODING_CONSTRUCTED | ASN1_TYPE_SEQUENCE;
	em[i++] = (uint8_t) (hash->oid_size + hash->digest_size + 8);
	em[i++] = ASN1_ENCODING_CONSTRUCTED | ASN1_TYPE_SEQUENCE;
	em[i++] = (uint8_t) (hash->oid_size + 4);
	em[i++] = ASN1_TYPE_OBJECT_IDENTIFIER;
	em[i++] = (uint8_t) hash->oid_size;

	//Copy the hash algorithm OID
	memcpy(em + i, hash->oid, hash->oid_size);
	i += hash->oid_size;

	//Encode the rest of the ASN.1 structure
	em[i++] = ASN1_TYPE_NULL;
	em[i++] = 0;
	em[i++] = ASN1_TYPE_OCTET_STRING;
	em[i++] = hash->digest_size;

	//Append the hash value
	memcpy(em + i, digest, hash->digest_size);

	//Successful processing
	return RES_OK;
}

/**
 * @brief MGF1 mask generation function
 * @param[in] hash Hash function
 * @param[in] hashContext Hash function context
 * @param[in] seed Seed from which the mask is generated
 * @param[in] seedLen Length of the seed in bytes
 * @param[in,out] data Data block to be masked
 * @param[in] dataLen Length of the data block in bytes
 **/

void mgf1(hash_algo_t* hash, const uint8_t *seed, size_t seedLen, uint8_t *data, size_t dataLen)
{
   size_t i;
   size_t n;
   uint32_t counter;
   uint32_t c;

   //The data is processed block by block
   for(counter = 0; dataLen > 0; counter++)
   {
      //Limit the number of bytes to process at a time
      n = min(dataLen, hash->hash_info->digest_size);

      //Convert counter to an octet string C of length 4 octets
      c = __REV(counter);

      //Calculate Hash(mgfSeed || C)
      hash->Reset();
      hash->Input(seed, seedLen);
      hash->Input(&c, sizeof(c));
      hash->Result(nullptr);

      //Apply the mask
      for(i = 0; i < n; i++)
      {
         data[i] ^= hash->digest8()[i];
      }

      //Advance data pointer
      data += n;
      dataLen -= n;
   }
}

/**
 * @brief EME-OAEP encoding operation
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] hash Underlying hash function
 * @param[in] label Optional label to be associated with the message
 * @param[in] message Message to be encrypted
 * @param[in] messageLen Length of the message to be encrypted
 * @param[out] em Encoded message
 * @param[in] k Length of the encoded message
 * @return Error code
 **/
/*
 *                           +----------+---------+-------+
                        DB = |  lHash   |    PS   |   M   |
                             +----------+---------+-------+
                                            |
                  +----------+              V
                  |   seed   |--> MGF ---> xor
                  +----------+              |
                        |                   |
               +--+     V                   |
               |00|    xor <----- MGF <-----|
               +--+     |                   |
                 |      |                   |
                 V      V                   V
               +--+----------+----------------------------+
         EM =  |00|maskedSeed|          maskedDB          |
               +--+----------+----------------------------+
               |00|          |sha-256 		| PS |01|   M |
 */
RES_CODE emeOaepEncode(prng_algo_t* prngAlgo, const hash_info_t* oaep_hinfo,
		const hash_info_t* mfg_hinfo, const char* label, const uint8_t* message,
		size_t messageLen, uint8_t *em, size_t k)
{
   RES_CODE res;
   size_t n;
   uint8_t *db;
   uint8_t *seed;
   auto_ptr<hash_algo_t> oaep_hash;
   auto_ptr<hash_algo_t> mfg_hash;

   //Check the length of the message
   if(messageLen > (k - 2 * oaep_hinfo->digest_size - 2))
      return RES_TLS_INVALID_LENGTH;

   //Point to the buffer where to format the seed
   seed = em + 1;
   //Point to the buffer where to format the data block
   db = em + oaep_hinfo->digest_size + 1;

   //Generate a random octet string seed of length hLen
   res = prngAlgo->prng_read(seed, oaep_hinfo->digest_size);
   //Any error to report?
   if(res != RES_OK)
      return res;

   //Allocate a memory buffer to hold the hash context
   oaep_hash = oaep_hinfo->new_hash();
   if(!oaep_hash.get())
      return RES_OUT_OF_MEMORY;
   mfg_hash = mfg_hinfo->new_hash();
   if(!mfg_hash.get())
      return RES_OUT_OF_MEMORY;

   //If the label L is not provided, let L be the empty string
   if(label == NULL)
      label = "";

   //Let lHash = Hash(L)
   oaep_hash->Input(label, strlen(label));
   oaep_hash->Result(db);

   //The padding string PS consists of k - mLen - 2hLen - 2 zero octets
   n = k - messageLen - 2 * oaep_hinfo->digest_size - 2;
   //Generate the padding string
   memset(db + oaep_hinfo->digest_size, 0, n);

   //Concatenate lHash, PS, a single octet with hexadecimal value 0x01, and
   //the message M to form a data block DB of length k - hLen - 1 octets
   db[oaep_hinfo->digest_size + n] = 0x01;
   memcpy(db + oaep_hinfo->digest_size + n + 1, message, messageLen);

   //Calculate the length of the data block
   n = k - oaep_hinfo->digest_size - 1;

   //Let maskedDB = DB xor MGF(seed, k - hLen - 1)
   mgf1(mfg_hash.get(), seed, oaep_hinfo->digest_size, db, n);
   //Let maskedSeed = seed xor MGF(maskedDB, hLen)
   mgf1(mfg_hash.get(), db, n, seed, oaep_hinfo->digest_size);

   //Concatenate a single octet with hexadecimal value 0x00, maskedSeed, and
   //maskedDB to form an encoded message EM of length k octets
   em[0] = 0x00;


   //Successful processing
   return RES_OK;
}

RES_CODE rsaesOaepEncrypt(prng_algo_t* prngAlgo, const RsaPublicKey* key,
		const hash_info_t* oaep_hinfo, const hash_info_t* mfg_hinfo, const char* label,
		const uint8_t* message, size_t messageLen, uint8_t* ciphertext,
		size_t* ciphertextLen)
{
	RES_CODE res;
	uint32_t k;
	uint8_t *em;
	Mpi m;
	Mpi c;

	//Check parameters
	if (prngAlgo == NULL || key == NULL || message == NULL)
		return RES_TLS_INVALID_PARAMETER;
	if (ciphertext == NULL || ciphertextLen == NULL)
		return RES_TLS_INVALID_PARAMETER;

	//Get the length in octets of the modulus n
	k = key->n.mpiGetByteLength();

	//Make sure the modulus is valid
	if (k == 0)
		return RES_TLS_INVALID_PARAMETER;

	//Point to the buffer where the encoded message EM will be formatted
	em = ciphertext;

	//EME-OAEP encoding
	res = emeOaepEncode(prngAlgo, oaep_hinfo, mfg_hinfo, label, message,
			messageLen, em, k);
	//Any error to report?
	if (res != RES_OK)
		return res;

	//Debug message
	TRACELN1_TLS("  Encoded message:\r\n");
	TRACE_TLS_ARRAY("    ", em, k);

	//Start of exception handling block
	do
	{
		//Convert the encoded message EM to an integer message representative m
		res = m.mpiReadRaw(em, k);
		//Conversion failed?
		if (res != RES_OK)
			break;

		//Apply the RSAEP encryption primitive
		res = key->rsaep(&m, &c);
		//Any error to report?
		if (res != RES_OK)
			break;

		//Convert the ciphertext representative c to a ciphertext of length k octets
		res = c.mpiWriteRaw(ciphertext, k);
		//Conversion failed?
		if (res != RES_OK)
			break;

		//Length of the resulting ciphertext
		*ciphertextLen = k;

		//Debug message
		TRACELN1_TLS("  Ciphertext:");
		TRACE_TLS_ARRAY("    ", ciphertext, *ciphertextLen);

		//End of exception handling block
	} while (0);

	//Return status code
	return res;
}

#endif // RSA_SUPPORT

