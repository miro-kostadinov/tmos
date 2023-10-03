/*
 * ec.h
 *
 *  Created on: Jul 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_EC_H_
#define SERVICES_SECURITY_CRYPTO_EC_H_

#include <tmos.h>
#include <crypto.h>
#include <ec_curves.h>

//EC related constants
extern const uint8_t EC_PUBLIC_KEY_OID[7];

//Error code checking
#define EC_CHECK(f) if((res = f) != RES_OK) goto end

/// Elliptic curve point
struct EcPoint
{
   Mpi x; ///<x-coordinate
   Mpi y; ///<y-coordinate
   Mpi z; ///<z-coordinate

   void ecp_free();
   RES_CODE ecCopy(const EcPoint* s);
   RES_CODE ecProjectify(const EcPoint* s);
};


/// EC domain parameters

struct EcDomainParameters
{
	EcCurveType type; 	///<Curve type
	Mpi p; 				///<Prime
	Mpi pa;				///<Curve parameter a
	Mpi pb;				///<Curve parameter b
	EcPoint g; 			///<Base point G
	Mpi q; 				///<Order of the point G
	EcFastModAlgo mod;	///<Fast modular reduction

	EcDomainParameters() : type(EC_CURVE_TYPE_NONE), mod(nullptr) {};

	void ecdp_free();

	RES_CODE ecLoadDomainParameters(const EcCurveInfo* curveInfo);

	RES_CODE ecDouble(EcPoint* r, const EcPoint* s) const;

	RES_CODE ecAdd(EcPoint* r, const EcPoint* s, const EcPoint* t) const;
	RES_CODE ecFullAdd(EcPoint* r, const EcPoint* s, const EcPoint* t) const;
	RES_CODE ecFullSub(EcPoint* r, const EcPoint* s, const EcPoint* t) const;

	RES_CODE ecMult(EcPoint* r, const Mpi* d, const EcPoint* s) const;

	RES_CODE ecTwinMult(EcPoint* r, const Mpi* d0, const EcPoint* s,
			const Mpi* d1, const EcPoint* t) const;

	RES_CODE ecAddMod(Mpi *r, const Mpi *a, const Mpi *b) const;
	RES_CODE ecSubMod(Mpi *r, const Mpi *a, const Mpi *b) const;
	RES_CODE ecMulMod(Mpi *r, const Mpi *a, const Mpi *b) const;
	RES_CODE ecSqrMod(Mpi *r, const Mpi *a) const;

	RES_CODE ecImport(EcPoint* r, const uint8_t* data, size_t length) const;
	RES_CODE ecExport(const EcPoint* a, uint8_t* data, size_t* length) const;

	RES_CODE ecProjectify(EcPoint* r, const EcPoint* s) const;
	RES_CODE ecAffinify(EcPoint* r, const EcPoint* s) const;

	RES_CODE ecIsPointAffine(const EcPoint *s) const;

	RES_CODE ecGeneratePrivateKey(prng_algo_t* prngAlgo, Mpi* privateKey) const;
	RES_CODE ecGeneratePublicKey(const Mpi* privateKey, EcPoint* publicKey) const;
	RES_CODE ecGenerateKeyPair(prng_algo_t* prngAlgo, Mpi* privateKey,
			EcPoint* publicKey) const;
};

RES_CODE pkcs8FormatEcPrivateKey(const EcCurveInfo* curveInfo,
   const Mpi* privateKey, const EcPoint* publicKey, uint8_t* output,
   size_t* written);
RES_CODE pkcs8FormatEcPublicKey(const EcCurveInfo* curveInfo,
   const EcPoint* publicKey, uint8_t* output, size_t* written);
RES_CODE pemExportEcPrivateKey(const EcCurveInfo* curveInfo,
   const Mpi* privateKey, const EcPoint* publicKey, char *output,
   size_t* written);




#endif /* SERVICES_SECURITY_CRYPTO_EC_H_ */
