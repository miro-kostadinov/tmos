/*
 * ecdh.cpp
 *
 *  Created on: Aug 4, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <ecdh.h>
#include <tls.h>


#if ECDH_SUPPORT

/**
 * @brief Read an EC point from an opaque vector
 * @param[in] params EC domain parameters
 * @param[out] a Resulting EC point
 * @param[in] data Buffer where to read the opaque vector
 * @param[in] size Total number of bytes available in the buffer
 * @param[out] length Total number of bytes that have been read
 * @return Error code
 **/
RES_CODE EcdhContext::tlsReadEcPoint(const uint8_t* data, size_t size, size_t* length)
{
#if TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT
	RES_CODE res;
	size_t n;

	//Buffer underrun?
	if (size < 1)
		return RES_TLS_DECODING_FAILED;

	//The EC point representation is preceded by a length field
	n = data[0];

	//Valid EC point representation?
	if (size < (n + 1))
		return RES_TLS_DECODING_FAILED;

	//Convert the octet string to an EC point
	res = params.ecImport(&qb, data + 1, n);
	if (res != RES_OK)
		return res;

	//Return the total number of bytes that have been read
	*length = n + 1;

	return RES_OK;

#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Encode an EC point to an opaque vector
 * @param[in] params EC domain parameters
 * @param[in] a Pointer to an EC point
 * @param[out] data Buffer where to store the opaque vector
 * @param[out] length Total number of bytes that have been written
 * @return Error code
 **/
RES_CODE EcdhContext::tlsWriteEcPoint(uint8_t *data, size_t *length) const
{
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	RES_CODE res;

	//Convert the EC point to an octet string
	res = params.ecExport(&qa, data + 1, length);
	if (res != RES_OK)
		return res;

	//Set the length of the opaque vector
	data[0] = (uint8_t) (*length);

	//Return the total number of bytes that have been written
	*length += 1;
	//Successful processing
	return RES_OK;
#else
	//Not implemented
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Check ECDH public key
 * @param[in] params EC domain parameters
 * @param[in] publicKey Public key to be checked
 * @return Error code
 **/
RES_CODE EcdhContext::ecdhCheckPublicKey()
{
	RES_CODE res;

	//Verify that 0 <= Qx < p
	if (qb.x.mpiCompInt(0) < 0)
		return RES_TLS_ILLEGAL_PARAMETER;
	else if (qb.x.mpiComp(&params.p) >= 0)
		return RES_TLS_ILLEGAL_PARAMETER;

	//Verify that 0 <= Q y < p
	if (qb.y.mpiCompInt(0) < 0)
		return RES_TLS_ILLEGAL_PARAMETER;
	else if (qb.y.mpiComp(&params.p) >= 0)
		return RES_TLS_ILLEGAL_PARAMETER;

	//Check whether the point is on the curve
	res = params.ecIsPointAffine(&qb);

	return res;
}

/**
 * @brief ECDH key pair generation
 * @param[in] context Pointer to the ECDH context
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @return Error code
 **/
RES_CODE EcdhContext::ecdhGenerateKeyPair(prng_algo_t* prngAlgo)
{
	RES_CODE res;
	uint32_t n;

	//Debug message
	TRACE1_TLS("Generating ECDH key pair...\r\n");

	//Let N be the bit length of q
	n = params.q.mpiGetBitLength();

	//Generated a pseudorandom number
	MPI_CHECK(da.mpiRand(n, prngAlgo));

	//Make sure that 0 < da < q
	if (da.mpiComp(&params.q) >= 0)
		da.mpiShiftRight(1);

	//Debug message
	TRACE1_TLS("  Private key:\r\n");
	TRACE_MPI("    ", &da);

	//Compute Qa = da.G
	EC_CHECK( params.ecMult(&qa, &da, &params.g) );
	EC_CHECK( params.ecAffinify(&qa, &qa) );

	//Debug message
	TRACE1_TLS("  Public key X:\r\n");
	TRACE_MPI("    ", &qa.x);
	TRACE1_TLS("  Public key Y:\r\n");
	TRACE_MPI("    ", &qa.y);

end:
	return res;
}

/**
 * @brief Compute ECDH shared secret
 * @param[in] context Pointer to the ECDH context
 * @param[out] output Buffer where to store the shared secret
 * @param[in] outputSize Size of the buffer in bytes
 * @param[out] outputLength Length of the resulting shared secret
 * @return Error code
 **/
RES_CODE EcdhContext::ecdhComputeSharedSecret(uint8_t* output, size_t outputSize, size_t* outputLength)
{
	RES_CODE res;
	size_t k;
	EcPoint z;

	//Debug message
	TRACELN1_TLS("Computing Diffie-Hellman shared secret...");

	//Get the length in octets of the prime modulus
	k = params.p.mpiGetByteLength();

	//Make sure that the output buffer is large enough
	if (outputSize < k)
		return RES_TLS_INVALID_LENGTH;

	//Compute Z = da.Qb
	EC_CHECK( params.ecProjectify(&qb, &qb) );
	EC_CHECK( params.ecMult(&z, &da, &qb) );
	EC_CHECK( params.ecAffinify(&z, &z) );

	//Convert the x-coordinate of Z to an octet string
	MPI_CHECK( z.x.mpiWriteRaw(output, k) );

	//Length of the resulting shared secret
	*outputLength = k;

	//Debug message
	TRACELN_TLS("  Shared secret (%u bytes)", *outputLength);
//	TRACE_TLS_ARRAY("    ", output, *outputLength);

end:
	return res;
}


#endif // ECDH_SUPPORT


