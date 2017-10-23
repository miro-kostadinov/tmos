/*
 * ecdsa.cpp
 *
 *  Created on: Jul 17, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <ecdsa.h>

#if ECDSA_SUPPORT

//ECDSA with SHA-1 OID (1.2.840.10045.4.1)
const uint8_t ECDSA_WITH_SHA1_OID[7] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x01};
//ECDSA with SHA-224 OID (1.2.840.10045.4.3.1)
const uint8_t ECDSA_WITH_SHA224_OID[8] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x01};
//ECDSA with SHA-256 OID (1.2.840.10045.4.3.2)
const uint8_t ECDSA_WITH_SHA256_OID[8] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02};
//ECDSA with SHA-384 OID (1.2.840.10045.4.3.3)
const uint8_t ECDSA_WITH_SHA384_OID[8] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x03};
//ECDSA with SHA-512 OID (1.2.840.10045.4.3.4)
const uint8_t ECDSA_WITH_SHA512_OID[8] = {0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x04};
//ECDSA with SHA-3-224 OID (2.16.840.1.101.3.4.3.9)
const uint8_t ECDSA_WITH_SHA3_224_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x09};
//ECDSA with SHA-3-256 OID (2.16.840.1.101.3.4.3.10)
const uint8_t ECDSA_WITH_SHA3_256_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0A};
//ECDSA with SHA-3-384 OID (2.16.840.1.101.3.4.3.11)
const uint8_t ECDSA_WITH_SHA3_384_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0B};
//ECDSA with SHA-3-512 OID (2.16.840.1.101.3.4.3.12)
const uint8_t ECDSA_WITH_SHA3_512_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x0C};


RES_CODE EcdsaSignature::ecdsaVerifySignature(const EcDomainParameters* params, const EcPoint* publicKey,
	   const uint8_t* digest, size_t digestLength) const
{
	RES_CODE res;
	uint32_t n;
	Mpi w;
	Mpi z;
	Mpi u1;
	Mpi u2;
	Mpi v;
	EcPoint v0;
	EcPoint v1;

	//Check parameters
	if (params == nullptr || publicKey == nullptr || digest == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("ECDSA signature verification...\r\n"); TRACE1_TLS("  public key X:\r\n");
	TRACE_MPI("    ", &publicKey->x);
	TRACE1_TLS("  public key Y:\r\n");
	TRACE_MPI("    ", &publicKey->y);
	TRACE1_TLS("  digest:\r\n");
	TRACE_TLS_ARRAY("    ", digest, digestLength);
	TRACE1_TLS("  r:\r\n");
	TRACE_MPI("    ", &r);
	TRACE1_TLS("  s:\r\n");
	TRACE_MPI("    ", &s);

	//The verifier shall check that 0 < r < q and 0 < s < q. If either
	//condition is violated, the signature shall be rejected as invalid
	if (r.mpiCompInt(0) <= 0 || r.mpiComp(&params->q) >= 0)
		return RES_TLS_INVALID_SIGNATURE;
	if (s.mpiCompInt(0) <= 0 || s.mpiComp(&params->q) >= 0)
		return RES_TLS_INVALID_SIGNATURE;

	//Let N be the bit length of q
	n = params->q.mpiGetBitLength();
	//Compute N = MIN(N, outlen)
	n = min(n, digestLength * 8);

	//Convert the digest to a multiple precision integer
	res = z.mpiReadRaw(digest, (n + 7) / 8);
	if(res == RES_OK)
	{
		//Keep the leftmost N bits of the hash value
		if ((n % 8) != 0)
		{
			res = z.mpiShiftRight(8 - (n % 8));
		}
	}

	if(res == RES_OK)
	{
		//Compute w = s ^ -1 mod q
		res = w.mpiInvMod(&s, &params->q);
		if(res == RES_OK)
		{
			//Compute u1 = z * w mod q
			res = u1.mpiMulMod(&z, &w, &params->q);
			if(res == RES_OK)
			{
				//Compute u2 = r * w mod q
				res = u2.mpiMulMod(&r, &w, &params->q);
				if(res == RES_OK)
				{
					//Compute V0 = (x0, y0) = u1.G + u2.Q
					res = v1.ecProjectify(publicKey);
					if(res == RES_OK)
					{
						res = params->ecTwinMult(&v0, &u1, &params->g, &u2, &v1);
						if(res == RES_OK)
						{
							res = params->ecAffinify(&v0, &v0);
							if(res == RES_OK)
							{
								//Debug message
								TRACE1_TLS("  x0:\r\n");
								TRACE_MPI("    ", &v0.x);
								TRACE1_TLS("  y0:\r\n");
								TRACE_MPI("    ", &v0.y);

								//Compute v = x0 mod q
								res = v.mpiMod(&v0.x, &params->q);
								if(res == RES_OK)
								{
									//Debug message
									TRACE1_TLS("  v:\r\n");
									TRACE_MPI("    ", &v);

									//If v = r, then the signature is verified. If v does not equal r,
									//then the message or the signature may have been modified
									if (v.mpiComp(&r))
										res = RES_TLS_INVALID_SIGNATURE;
								}
							}

						}
					}
				}

			}
		}
	}
	return res;
}

/**
 * @brief ECDSA signature generation
 * @param[in] params EC domain parameters
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] privateKey Signer's ECDSA private key
 * @param[in] digest Digest of the message to be signed
 * @param[in] digestLength Length in octets of the digest
 * @param[out] signature (R, S) integer pair
 * @return Error code
 **/
RES_CODE EcdsaSignature::ecdsaGenerateSignature(const EcDomainParameters* params,
		prng_algo_t* prngAlgo, const Mpi* privateKey, const uint8_t* digest,
		size_t digestLength)
{
	RES_CODE res;
	uint32_t n;
	Mpi k;
	Mpi z;
	EcPoint r1;

	//Check parameters
	if (params == nullptr || privateKey == nullptr || digest == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("ECDSA signature generation...\r\n"); TRACE1_TLS("  private key:\r\n");
	TRACE_MPI("    ", privateKey);
	TRACE1_TLS("  digest:\r\n");
	TRACE_TLS_ARRAY("    ", digest, digestLength);

	//Let N be the bit length of q
	n = params->q.mpiGetBitLength();

	//Generated a pseudorandom number
	MPI_CHECK(k.mpiRand(n, prngAlgo));

	//Make sure that 0 < k < q
	if (k.mpiComp(&params->q) >= 0)
		k.mpiShiftRight(1);

	//Debug message
	TRACE1_TLS("  k:\r\n");
	TRACE_MPI("    ", &k);

	//Compute N = MIN(N, outlen)
	n = min(n, digestLength * 8);

	//Convert the digest to a multiple precision integer
	MPI_CHECK(z.mpiReadRaw(digest, (n + 7) / 8));

	//Keep the leftmost N bits of the hash value
	if ((n % 8) != 0)
	{
		MPI_CHECK(z.mpiShiftRight(8 - (n % 8)));
	}

	//Debug message
	TRACE1_TLS("  z:\r\n");
	TRACE_MPI("    ", &z);

	//Compute R1 = (x1, y1) = k.G
	EC_CHECK(params->ecMult(&r1, &k, &params->g));
	EC_CHECK(params->ecAffinify(&r1, &r1));

	//Debug message
	TRACE1_TLS("  x1:\r\n");
	TRACE_MPI("    ", &r1.x);
	TRACE1_TLS("  y1:\r\n");
	TRACE_MPI("    ", &r1.y);

	//Compute r = x1 mod q
	MPI_CHECK(r.mpiMod(&r1.x, &params->q));

	//Compute k ^ -1 mod q
	MPI_CHECK(k.mpiInvMod(&k, &params->q));

	//Compute s = k ^ -1 * (z + x * r) mod q
	MPI_CHECK(s.mpiMul(privateKey, &r));
	MPI_CHECK(s.mpiAdd(&s, &z));
	MPI_CHECK(s.mpiMod(&s, &params->q));
	MPI_CHECK(s.mpiMulMod(&s, &k, &params->q));

	//Dump ECDSA signature
	TRACE1_TLS("  r:\r\n");
	TRACE_MPI("    ", &r);
	TRACE1_TLS("  s:\r\n");
	TRACE_MPI("    ", &s);

end:
	return res;
}

RES_CODE EcdsaSignature::ecdsaWriteSignatureLen(size_t *length) const
{
	return dsaWriteSignatureLen(length);
}

RES_CODE EcdsaSignature::ecdsaWriteSignature(uint8_t* data, size_t* length) const
{
	return dsaWriteSignature(data, length);
}


#endif
