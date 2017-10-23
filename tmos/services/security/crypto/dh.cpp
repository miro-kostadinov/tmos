/*
 * dh.cpp
 *
 *  Created on: Aug 3, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <dh.h>

#if DH_SUPPORT

/**
 * @brief Check Diffie-Hellman public value
 * @param[in] params Pointer to the Diffie-Hellman parameters
 * @param[in] publicKey Public value to be checked
 * @return Error code
 **/
RES_CODE DhParameters::dhCheckPublicKey(const Mpi* publicKey)
{
	RES_CODE res;
	Mpi a;

	//Precompute p - 1
	res = a.mpiSubInt(&p, 1);

	//Check status
	if (res == RES_OK)
	{
		//Reject weak public values 1 and p - 1
		if (publicKey->mpiCompInt(1) <= 0)
			res = RES_TLS_ILLEGAL_PARAMETER;
		else if (publicKey->mpiComp(&a) >= 0)
			res = RES_TLS_ILLEGAL_PARAMETER;
	}

	return res;
}


/**
 * @brief Diffie-Hellman key pair generation
 * @param[in] context Pointer to the Diffie-Hellman context
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @return Error code
 **/
RES_CODE DhContext::dhGenerateKeyPair(prng_algo_t* prngAlgo)
{
	RES_CODE res;
	uint32_t k;

	//Debug message
	TRACE1_TLS("Generating Diffie-Hellman key pair...\r\n");

	//Get the length in bits of the prime p
	k = params.p.mpiGetBitLength();

	//Ensure the length is valid
	if (k == 0)
		return RES_TLS_INVALID_PARAMETER;

	//The private value shall be randomly generated
	res = xa.mpiRand(k, prngAlgo);
	if (res != RES_OK)
		return res;

	//The private value shall be less than p
	if (xa.mpiComp(&params.p) >= 0)
	{
		//Shift value to the right
		res = xa.mpiShiftRight(1);
		if (res != RES_OK)
			return res;
	}

	//Debug message
	TRACE1_TLS("  Private value:\r\n");
	TRACE_MPI("    ", &xa);

	//Calculate the corresponding public value (ya = g ^ xa mod p)
	res = ya.mpiExpMod(&params.g, &xa, &params.p);
	if (res != RES_OK)
		return res;

	//Debug message
	TRACE1_TLS("  Public value:\r\n");
	TRACE_MPI("    ", &ya);

	//Check public value
	res = params.dhCheckPublicKey(&ya);

	return res;
}

/**
 * @brief Compute Diffie-Hellman shared secret
 * @param[in] context Pointer to the Diffie-Hellman context
 * @param[out] output Buffer where to store the shared secret
 * @param[in] outputSize Size of the buffer in bytes
 * @param[out] outputLength Length of the resulting shared secret
 * @return Error code
 **/
RES_CODE DhContext::dhComputeSharedSecret(uint8_t* output, size_t outputSize, size_t* outputLength)
{
	RES_CODE res;
	size_t k;
	Mpi z;

	//Debug message
	TRACE1_TLS("Computing Diffie-Hellman shared secret...\r\n");

	//Get the length in octets of the prime modulus
	k = params.p.mpiGetByteLength();

	//Make sure that the output buffer is large enough
	if (outputSize < k)
		return RES_TLS_INVALID_LENGTH;

	//Start of exception handling block
	do
	{
		//Calculate the shared secret key (k = yb ^ xa mod p)
		res = z.mpiExpMod(&yb, &xa, &params.p);
		if (res != RES_OK)
			return res;

		//Convert the resulting integer to an octet string
		res = z.mpiWriteRaw(output, k);
		if (res != RES_OK)
			return res;

		//Length of the resulting shared secret
		*outputLength = k;

		//Debug message
		TRACE_TLS("  Shared secret (%u bytes):\r\n", *outputLength);
		TRACE_TLS_ARRAY("    ", output, *outputLength);

		//End of exception handling block
	} while (0);

	return res;
}

#endif // DH_SUPPORT
