/*
 * ec_curves.h
 *
 *  Created on: Jul 26, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_EC_CURVES_H_
#define SERVICES_SECURITY_CRYPTO_EC_CURVES_H_

#include <tmos.h>
#include <crypto.h>
#include <mpi.h>

#ifndef ECC112_SUPPORT
#define ECC112_SUPPORT 1
#endif
#ifndef ECC128_SUPPORT
#define ECC128_SUPPORT 1
#endif
#ifndef ECC160_SUPPORT
#define ECC160_SUPPORT 1
#endif
#ifndef ECC192_SUPPORT
#define ECC192_SUPPORT 1
#endif
#ifndef ECC224_SUPPORT
#define ECC224_SUPPORT 1
#endif
#ifndef ECC239_SUPPORT
#define ECC239_SUPPORT 0
#endif
#ifndef ECC256_SUPPORT
#define ECC256_SUPPORT 1
#endif
#ifndef ECC320_SUPPORT
#define ECC320_SUPPORT 1
#endif
#ifndef ECC384_SUPPORT
#define ECC384_SUPPORT 1
#endif
#ifndef ECC512_SUPPORT
#define ECC512_SUPPORT 1
#endif
#ifndef ECC521_SUPPORT
#define ECC521_SUPPORT 1
#endif

/* Determine max ECC bits based on enabled curves */
#if ECC521_SUPPORT
    #define MAX_ECC_BITS    521
#elif ECC512_SUPPORT
    #define MAX_ECC_BITS    512
#elif ECC384_SUPPORT
    #define MAX_ECC_BITS    384
#elif ECC320_SUPPORT
    #define MAX_ECC_BITS    320
#elif ECC256_SUPPORT
    #define MAX_ECC_BITS    256
#elif ECC239_SUPPORT
    #define MAX_ECC_BITS    239
#elif ECC224_SUPPORT
    #define MAX_ECC_BITS    224
#elif ECC192_SUPPORT
    #define MAX_ECC_BITS    192
#elif ECC160_SUPPORT
    #define MAX_ECC_BITS    160
#elif ECC128_SUPPORT
    #define MAX_ECC_BITS    128
#elif ECC112_SUPPORT
    #define MAX_ECC_BITS    112
#endif

/* calculate max ECC bytes */
#if ((MAX_ECC_BITS * 2) % 8) == 0
    #define MAX_ECC_BYTES     (MAX_ECC_BITS / 8)
#else
    /* add byte if not aligned */
    #define MAX_ECC_BYTES     ((MAX_ECC_BITS / 8) + 1)
#endif

//Constants
extern const uint8_t SECP112R1_OID[5];
extern const uint8_t SECP112R2_OID[5];
extern const uint8_t SECP128R1_OID[5];
extern const uint8_t SECP128R2_OID[5];
extern const uint8_t SECP160K1_OID[5];
extern const uint8_t SECP160R1_OID[5];
extern const uint8_t SECP160R2_OID[5];
extern const uint8_t SECP192K1_OID[5];
extern const uint8_t SECP192R1_OID[8];
extern const uint8_t SECP224K1_OID[5];
extern const uint8_t SECP224R1_OID[5];
extern const uint8_t SECP256K1_OID[5];
extern const uint8_t SECP256R1_OID[8];
extern const uint8_t SECP384R1_OID[5];
extern const uint8_t SECP521R1_OID[5];
extern const uint8_t BRAINPOOLP160R1_OID[10];
extern const uint8_t BRAINPOOLP192R1_OID[10];
extern const uint8_t BRAINPOOLP224R1_OID[10];
extern const uint8_t BRAINPOOLP256R1_OID[10];
extern const uint8_t BRAINPOOLP320R1_OID[10];
extern const uint8_t BRAINPOOLP384R1_OID[10];
extern const uint8_t BRAINPOOLP512R1_OID[10];


/// Fast modular reduction
typedef RES_CODE (*EcFastModAlgo)(Mpi *a, const Mpi *p);

//Fast modular reduction
RES_CODE secp128r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp128r2Mod(Mpi *a, const Mpi *p);
RES_CODE secp160k1Mod(Mpi *a, const Mpi *p);
RES_CODE secp160r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp160r2Mod(Mpi *a, const Mpi *p);
RES_CODE secp192k1Mod(Mpi *a, const Mpi *p);
RES_CODE secp192r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp224k1Mod(Mpi *a, const Mpi *p);
RES_CODE secp224r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp256k1Mod(Mpi *a, const Mpi *p);
RES_CODE secp256r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp384r1Mod(Mpi *a, const Mpi *p);
RES_CODE secp521r1Mod(Mpi *a, const Mpi *p);



/// Elliptic curve type
enum EcCurveType: uint8_t
{
	EC_CURVE_TYPE_NONE = 0,
	EC_CURVE_TYPE_SECT_K1 = 1,
	EC_CURVE_TYPE_SECT_R1 = 2,
	EC_CURVE_TYPE_SECT_R2 = 3,
	EC_CURVE_TYPE_SECP_K1 = 4,
	EC_CURVE_TYPE_SECP_R1 = 5,
	EC_CURVE_TYPE_SECP_R2 = 6,
	EC_CURVE_TYPE_BRAINPOOLP_R1 = 7
};

/// Elliptic curve parameters
struct EcCurveInfo
{
   const char * name;    ///<Curve name
   const uint8_t *oid;   ///<Object identifier
   size_t oidSize;       ///<OID size
   EcCurveType type;     ///<Curve type
   const uint8_t p[66];  ///<Prime modulus p
   size_t pLen;          ///<Length of p
   const uint8_t a[66];  ///<Curve parameter a
   size_t aLen;          ///<Length of a
   const uint8_t b[66];  ///<Curve parameter b
   size_t bLen;          ///<Length of b
   const uint8_t gx[66]; ///<x-coordinate of the base point G
   size_t gxLen;         ///<Length of Gx
   const uint8_t gy[66]; ///<y-coordinate of the base point G
   size_t gyLen;         ///<Length of Gy
   const uint8_t q[66];  ///<Order of the base point G
   size_t qLen;          ///<Length of q
   uint32_t h;           ///<Cofactor h
   EcFastModAlgo mod;    ///<Fast modular reduction
};

extern const EcCurveInfo secp112r1Curve;
extern const EcCurveInfo secp112r2Curve;
extern const EcCurveInfo secp128r1Curve;
extern const EcCurveInfo secp128r2Curve;
extern const EcCurveInfo secp160k1Curve;
extern const EcCurveInfo secp160r1Curve;
extern const EcCurveInfo secp160r2Curve;
extern const EcCurveInfo secp192k1Curve;
extern const EcCurveInfo secp192r1Curve;
extern const EcCurveInfo secp224k1Curve;
extern const EcCurveInfo secp224r1Curve;
extern const EcCurveInfo secp256k1Curve;
extern const EcCurveInfo secp256r1Curve;
extern const EcCurveInfo secp384r1Curve;
extern const EcCurveInfo secp521r1Curve;
extern const EcCurveInfo brainpoolP160r1Curve;
extern const EcCurveInfo brainpoolP192r1Curve;
extern const EcCurveInfo brainpoolP224r1Curve;
extern const EcCurveInfo brainpoolP256r1Curve;
extern const EcCurveInfo brainpoolP320r1Curve;
extern const EcCurveInfo brainpoolP384r1Curve;
extern const EcCurveInfo brainpoolP512r1Curve;

//SECG curves
#define SECP112R1_CURVE (&secp112r1Curve)
#define SECP112R2_CURVE (&secp112r2Curve)
#define SECP128R1_CURVE (&secp128r1Curve)
#define SECP128R2_CURVE (&secp128r2Curve)
#define SECP160K1_CURVE (&secp160k1Curve)
#define SECP160R1_CURVE (&secp160r1Curve)
#define SECP160R2_CURVE (&secp160r2Curve)
#define SECP192K1_CURVE (&secp192k1Curve)
#define SECP192R1_CURVE (&secp192r1Curve)
#define SECP224K1_CURVE (&secp224k1Curve)
#define SECP224R1_CURVE (&secp224r1Curve)
#define SECP256K1_CURVE (&secp256k1Curve)
#define SECP256R1_CURVE (&secp256r1Curve)
#define SECP384R1_CURVE (&secp384r1Curve)
#define SECP521R1_CURVE (&secp521r1Curve)

//Brainpool curves
#define BRAINPOOLP160R1_CURVE (&brainpoolP160r1Curve)
#define BRAINPOOLP192R1_CURVE (&brainpoolP192r1Curve)
#define BRAINPOOLP224R1_CURVE (&brainpoolP224r1Curve)
#define BRAINPOOLP256R1_CURVE (&brainpoolP256r1Curve)
#define BRAINPOOLP320R1_CURVE (&brainpoolP320r1Curve)
#define BRAINPOOLP384R1_CURVE (&brainpoolP384r1Curve)
#define BRAINPOOLP512R1_CURVE (&brainpoolP512r1Curve)


const EcCurveInfo *ecGetCurveInfo(const uint8_t *oid, size_t length);

#endif /* SERVICES_SECURITY_CRYPTO_EC_CURVES_H_ */
