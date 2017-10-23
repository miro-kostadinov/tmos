/*
 * dsa.cpp
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <dsa.h>
#include <asn1.h>

#if DSA_SUPPORT

//DSA OID (1.2.840.10040.4.1)
const uint8_t DSA_OID[7] = {0x2A, 0x86, 0x48, 0xCE, 0x38, 0x04, 0x01};
//DSA with SHA-1 OID (1.2.840.10040.4.3)
const uint8_t DSA_WITH_SHA1_OID[7] = {0x2A, 0x86, 0x48, 0xCE, 0x38, 0x04, 0x03};
//DSA with SHA-224 OID (2.16.840.1.101.3.4.3.1)
const uint8_t DSA_WITH_SHA224_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x01};
//DSA with SHA-256 OID (2.16.840.1.101.3.4.3.2)
const uint8_t DSA_WITH_SHA256_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x02};
//DSA with SHA-384 OID (2.16.840.1.101.3.4.3.3)
const uint8_t DSA_WITH_SHA384_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x03};
//DSA with SHA-512 OID (2.16.840.1.101.3.4.3.4)
const uint8_t DSA_WITH_SHA512_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x04};
//DSA with SHA-3-224 OID (2.16.840.1.101.3.4.3.5)
const uint8_t DSA_WITH_SHA3_224_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x05};
//DSA with SHA-3-256 OID (2.16.840.1.101.3.4.3.6)
const uint8_t DSA_WITH_SHA3_256_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x06};
//DSA with SHA-3-384 OID (2.16.840.1.101.3.4.3.7)
const uint8_t DSA_WITH_SHA3_384_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x07};
//DSA with SHA-3-512 OID (2.16.840.1.101.3.4.3.8)
const uint8_t DSA_WITH_SHA3_512_OID[9] = {0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x03, 0x08};



void DsaPublicKey::dsapk_free()
{
	p.mpi_free();
	q.mpi_free();
	g.mpi_free();
	y.mpi_free();
};

void DsaPrivateKey::dsapk_free()
{
	p.mpi_free();
	q.mpi_free();
	g.mpi_free();
	x.mpi_free();
}

RES_CODE DsaSignature::dsaWriteSignatureLen(size_t *length) const
{
	size_t n;
	size_t rLen;
	size_t sLen;


	//Calculate the length of R
	rLen = r.mpiGetByteLength();
	//Calculate the length of S
	sLen = s.mpiGetByteLength();

	//Make sure the (R, S) integer pair is valid
	if (rLen == 0 || sLen == 0)
		return RES_TLS_INVALID_LENGTH;

	//R and S are always encoded in the smallest possible number of octets
	if (r.mpiGetBitValue((rLen * 8) - 1))
		rLen++;
	if (s.mpiGetBitValue((sLen * 8) - 1))
		sLen++;

	//Update the length of the ASN.1 sequence
	n = asn1WriteTagLen(ASN1_TYPE_INTEGER, rLen);

	//Update the length of the ASN.1 sequence
	n += asn1WriteTagLen(ASN1_TYPE_INTEGER, sLen);

	n = asn1WriteTagLen(ASN1_TYPE_SEQUENCE, n) -n;

	n += asn1WriteTagLen(ASN1_TYPE_INTEGER, rLen);
	n += asn1WriteTagLen(ASN1_TYPE_INTEGER, sLen);


	//Total length of the ASN.1 structure
	*length = n;

	return RES_OK;
}

RES_CODE DsaSignature::dsaReadSignature(const uint8_t *data, size_t length)
{
	RES_CODE res;
	Asn1Tag tag;

	//Debug message
	TRACE1_TLS("Reading DSA signature...\r\n");

	//Dump DSA signature
	TRACE1_TLS("  signature:\r\n");
	TRACE_TLS_ARRAY("    ", data, length);

	//Start of exception handling block
	do
	{
		//Display ASN.1 structure
		res = asn1DumpObject(data, length, 0);
		if (res != RES_OK)
			break;

		//Read the contents of the ASN.1 structure
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(true, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_SEQUENCE);
		if (res != RES_OK)
			break;

		//Point to the first field
		data = tag.value;
		length = tag.length;

		//Read the parameter R
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL, ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the octet string to a multiple precision integer
		res = r.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Point to the next field
		data += tag.totalLength;
		length -= tag.totalLength;

		//Read the parameter S
		res = tag.asn1ReadTag(data, length);
		if (res != RES_OK)
			break;

		//Enforce encoding, class and type
		res = tag.asn1CheckTag(false, ASN1_CLASS_UNIVERSAL,	ASN1_TYPE_INTEGER);
		if (res != RES_OK)
			break;

		//Convert the octet string to a multiple precision integer
		res = s.mpiReadRaw(tag.value, tag.length);
		if (res != RES_OK)
			break;

		//Dump (R, S) integer pair
		TRACE1_TLS("  r:\r\n");
		TRACE_MPI("    ", &r);
		TRACE1_TLS("  s:\r\n");
		TRACE_MPI("    ", &s);

		//End of exception handling block
	} while (0);

	return res;
}

/**
 * @brief Encode DSA signature using ASN.1
 * @param[in] signature (R, S) integer pair
 * @param[out] data Pointer to the buffer where to store the resulting ASN.1 structure
 * @param[out] length Length of the ASN.1 structure
 * @return Error code
 **/
RES_CODE DsaSignature::dsaWriteSignature(uint8_t* data, size_t* length) const
{
	RES_CODE res;
	size_t k;
	size_t n;
	size_t rLen;
	size_t sLen;
	Asn1Tag tag;

	//Debug message
	TRACE1_TLS("Writing DSA signature...\r\n");

	//Dump (R, S) integer pair
	TRACE1_TLS("  r:\r\n");
	TRACE_MPI("    ", &r);
	TRACE1_TLS("  s:\r\n");
	TRACE_MPI("    ", &s);

	//Calculate the length of R
	rLen = r.mpiGetByteLength();
	//Calculate the length of S
	sLen = s.mpiGetByteLength();

	//Make sure the (R, S) integer pair is valid
	if (rLen == 0 || sLen == 0)
		return RES_TLS_INVALID_LENGTH;

	//R and S are always encoded in the smallest possible number of octets
	if (r.mpiGetBitValue((rLen * 8) - 1))
		rLen++;
	if (s.mpiGetBitValue((sLen * 8) - 1))
		sLen++;

	//The first pass computes the length of the ASN.1 sequence

	n = asn1WriteTagLen(ASN1_TYPE_INTEGER, rLen);

	n += asn1WriteTagLen(ASN1_TYPE_INTEGER, sLen);

	//The second pass encodes the ASN.1 structure
	k = 0;

	//The (R, S) integer pair is encapsulated within a sequence
	tag.constructed = true;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_SEQUENCE;
	tag.length = n;
	tag.value = nullptr;

	res = tag.asn1WriteTag(false, data + k, &n);
	if (res != RES_OK)
		return res;

	//Advance write pointer
	k += n;

	//Encode the parameter R using ASN.1
	tag.constructed = false;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_INTEGER;
	tag.length = rLen;
	tag.value = nullptr;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, data + k, &n);
	if (res != RES_OK)
		return res;

	//Advance write pointer
	k += n;

	//Convert R to an octet string
	res = r.mpiWriteRaw(data + k, rLen);
	if (res != RES_OK)
		return res;

	//Advance write pointer
	k += rLen;

	//Encode the parameter S using ASN.1
	tag.constructed = false;
	tag.objClass = ASN1_CLASS_UNIVERSAL;
	tag.objType = ASN1_TYPE_INTEGER;
	tag.length = sLen;
	tag.value = nullptr;

	//Write the corresponding ASN.1 tag
	res = tag.asn1WriteTag(false, data + k, &n);
	if (res != RES_OK)
		return res;

	//Advance write pointer
	k += n;

	//Convert S to an octet string
	res = s.mpiWriteRaw(data + k, sLen);
	if (res != RES_OK)
		return res;

	//Advance write pointer
	k += sLen;

	//Dump DSA signature
	TRACE1_TLS("  signature:\r\n");
	TRACE_TLS_ARRAY("    ", data, k);

	//Total length of the ASN.1 structure
	*length = k;

	//Successful processing
	return res;
}

/**
 * @brief DSA signature generation
 * @param[in] prngAlgo PRNG algorithm
 * @param[in] prngContext Pointer to the PRNG context
 * @param[in] key Signer's DSA private key
 * @param[in] digest Digest of the message to be signed
 * @param[in] digestLength Length in octets of the digest
 * @param[out] signature (R, S) integer pair
 * @return Error code
 **/
RES_CODE DsaSignature::dsaGenerateSignature(prng_algo_t* prngAlgo,
   const DsaPrivateKey* key, const uint8_t* digest, size_t digestLength)
{
	RES_CODE res;
	uint32_t n;
	Mpi k;
	Mpi z;

	//Check parameters
	if (key == nullptr || digest == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("DSA signature generation...\r\n"); TRACE1_TLS("  p:\r\n");
	TRACE_MPI("    ", &key->p);
	TRACE1_TLS("  q:\r\n");
	TRACE_MPI("    ", &key->q);
	TRACE1_TLS("  g:\r\n");
	TRACE_MPI("    ", &key->g);
	TRACE1_TLS("  x:\r\n");
	TRACE_MPI("    ", &key->x);
	TRACE1_TLS("  digest:\r\n");
	TRACE_TLS_ARRAY("    ", digest, digestLength);


	//Let N be the bit length of q
	n = key->q.mpiGetBitLength();

	//Generated a pseudorandom number
	MPI_CHECK(k.mpiRand(n, prngAlgo));

	//Make sure that 0 < k < q
	if (k.mpiComp(&key->q) >= 0)
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

	//Compute r = (g ^ k mod p) mod q
	MPI_CHECK(r.mpiExpMod(&key->g, &k, &key->p));
	MPI_CHECK(r.mpiMod(&r, &key->q));

	//Compute k ^ -1 mod q
	MPI_CHECK(k.mpiInvMod(&k, &key->q));

	//Compute s = k ^ -1 * (z + x * r) mod q
	MPI_CHECK(s.mpiMul(&key->x, &r));
	MPI_CHECK(s.mpiAdd(&s, &z));
	MPI_CHECK(s.mpiMod(&s, &key->q));
	MPI_CHECK(s.mpiMulMod(&s, &k, &key->q));

	//Dump DSA signature
	TRACE1_TLS("  r:\r\n");
	TRACE_MPI("    ", &r);
	TRACE1_TLS("  s:\r\n");
	TRACE_MPI("    ", &s);

end:

	//Clean up side effects if necessary
	if (res != RES_OK)
	{
		//Release (R, S) integer pair
		r.mpi_free();
		s.mpi_free();
	}

	return res;
}

RES_CODE DsaSignature::dsaVerifySignature(const DsaPublicKey *key, const uint8_t *digest,
		   size_t digestLength) const
{
	RES_CODE res;
	uint32_t n;
	Mpi w;
	Mpi z;
	Mpi u1;
	Mpi u2;
	Mpi v;

	//Check parameters
	if (key == nullptr || digest == nullptr )
		return RES_TLS_INVALID_PARAMETER;

	//Debug message
	TRACE1_TLS("DSA signature verification...\r\n"); TRACE1_TLS("  p:\r\n");
	TRACE_MPI("    ", &key->p);
	TRACE1_TLS("  q:\r\n");
	TRACE_MPI("    ", &key->q);
	TRACE1_TLS("  g:\r\n");
	TRACE_MPI("    ", &key->g);
	TRACE1_TLS("  y:\r\n");
	TRACE_MPI("    ", &key->y);
	TRACE1_TLS("  digest:\r\n");
	TRACE_TLS_ARRAY("    ", digest, digestLength);
	TRACE1_TLS("  r:\r\n");
	TRACE_MPI("    ", &r);
	TRACE1_TLS("  s:\r\n");
	TRACE_MPI("    ", &s);

	//The verifier shall check that 0 < r < q and 0 < s < q. If either
	//condition is violated, the signature shall be rejected as invalid
	if (r.mpiCompInt(0) <= 0	|| r.mpiComp(&key->q) >= 0)
		return RES_TLS_INVALID_SIGNATURE;
	if (s.mpiCompInt(0) <= 0	|| s.mpiComp(&key->q) >= 0)
		return RES_TLS_INVALID_SIGNATURE;

	//Let N be the bit length of q
	n = key->q.mpiGetBitLength();
	//Compute N = MIN(N, outlen)
	n = min(n, digestLength * 8);

	//Convert the digest to a multiple precision integer
	res = z.mpiReadRaw(digest, (n + 7) / 8);
	if(res != RES_OK)
		return res;

	//Keep the leftmost N bits of the hash value
	if ((n % 8) != 0)
	{
		res = z.mpiShiftRight(8 - (n % 8));
		if(res != RES_OK)
			return res;
	}

	//Compute w = s ^ -1 mod q
	res = w.mpiInvMod(&s, &key->q);
	if(res != RES_OK)
		return res;
	//Compute u1 = z * w mod q
	res = u1.mpiMulMod(&z, &w, &key->q);
	if(res != RES_OK)
		return res;
	//Compute u2 = r * w mod q
	res = u2.mpiMulMod(&r, &w, &key->q);
	if(res != RES_OK)
		return res;


	//Compute v = ((g ^ u1) * (y ^ u2) mod p) mod q
	res = v.mpiExpMod(&key->g, &u1, &key->p);
	if(res != RES_OK)
		return res;
	res = w.mpiExpMod(&key->y, &u2, &key->p);
	if(res != RES_OK)
		return res;
	res = v.mpiMulMod(&v, &w, &key->p);
	if(res != RES_OK)
		return res;
	res = v.mpiMod(&v, &key->q);
	if(res != RES_OK)
		return res;

	//Debug message
	TRACE1_TLS("  v:\r\n");
	TRACE_MPI("    ", &v);

	//If v = r, then the signature is verified. If v does not equal r,
	//then the message or the signature may have been modified
	if (!v.mpiComp(&r))
		res = RES_OK;
	else
		res = RES_TLS_INVALID_SIGNATURE;

	return res;
}

#endif
