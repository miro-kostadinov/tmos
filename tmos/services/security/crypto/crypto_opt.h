/*
 * crypto_opt.h
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_CRYPTO_OPT_H_
#define SERVICES_SECURITY_CRYPTO_CRYPTO_OPT_H_

#include <tmos.h>

//Multiple precision integer support
#ifndef MPI_SUPPORT
   #define MPI_SUPPORT 1
#endif

//Assembly optimizations for time-critical routines
#ifndef MPI_ASM_SUPPORT
   #define MPI_ASM_SUPPORT 0
#endif

//Base64 encoding support
#ifndef BASE64_SUPPORT
   #define BASE64_SUPPORT 1
#endif

//MD2 hash support
#ifndef MD2_SUPPORT
   #define MD2_SUPPORT 0 //
#endif

//MD4 hash support
#ifndef MD4_SUPPORT
   #define MD4_SUPPORT 0 //
#endif

//MD5 hash support
#ifndef MD5_SUPPORT
   #define MD5_SUPPORT 1
#endif

//RIPEMD-128 hash support
#ifndef RIPEMD128_SUPPORT
   #define RIPEMD128_SUPPORT 0 //
#endif

//RIPEMD-160 hash support
#ifndef RIPEMD160_SUPPORT
   #define RIPEMD160_SUPPORT 0 //
#endif

//SHA-1 hash support
#ifndef SHA1_SUPPORT
   #define SHA1_SUPPORT 1
#endif

//SHA-224 hash support
#ifndef SHA224_SUPPORT
   #define SHA224_SUPPORT 1
#endif

//SHA-256 hash support
#ifndef SHA256_SUPPORT
   #define SHA256_SUPPORT 1
#endif

//SHA-384 hash support
#ifndef SHA384_SUPPORT
   #define SHA384_SUPPORT 1
#endif

//SHA-512 hash support
#ifndef SHA512_SUPPORT
   #define SHA512_SUPPORT 1
#endif

//SHA-512/224 hash support
#ifndef SHA512_224_SUPPORT
   #define SHA512_224_SUPPORT 0 //
#endif

//SHA-512/256 hash support
#ifndef SHA512_256_SUPPORT
   #define SHA512_256_SUPPORT 0 //
#endif

//SHA3-224 hash support
#ifndef SHA3_224_SUPPORT
   #define SHA3_224_SUPPORT 0
#endif

//SHA3-256 hash support
#ifndef SHA3_256_SUPPORT
   #define SHA3_256_SUPPORT 0
#endif

//SHA3-384 hash support
#ifndef SHA3_384_SUPPORT
   #define SHA3_384_SUPPORT 0
#endif

//SHA3-512 hash support
#ifndef SHA3_512_SUPPORT
   #define SHA3_512_SUPPORT 0
#endif

//SHAKE128 support
#ifndef SHAKE128_SUPPORT
   #define SHAKE128_SUPPORT 0
#endif

//SHAKE256 support
#ifndef SHAKE256_SUPPORT
   #define SHAKE256_SUPPORT 0
#endif

//Keccak support
#ifndef KECCAK_SUPPORT
   #define KECCAK_SUPPORT 0
#endif

//Tiger hash support
#ifndef TIGER_SUPPORT
   #define TIGER_SUPPORT 0 //
#endif

//Whirlpool hash support
#ifndef WHIRLPOOL_SUPPORT
   #define WHIRLPOOL_SUPPORT 1
#endif

//HMAC support
#ifndef HMAC_SUPPORT
   #define HMAC_SUPPORT 1
#endif

//RC4 encryption support
#ifndef RC4_SUPPORT
   #define RC4_SUPPORT 1
#endif

//RC6 encryption support
#ifndef RC6_SUPPORT
   #define RC6_SUPPORT 1
#endif

//IDEA encryption support
#ifndef IDEA_SUPPORT
   #define IDEA_SUPPORT 1
#endif

//DES encryption support
#ifndef DES_SUPPORT
   #define DES_SUPPORT 1
#endif

//Triple DES encryption support
#ifndef DES3_SUPPORT
   #define DES3_SUPPORT 1
#endif

//AES encryption support
#ifndef AES_SUPPORT
   #define AES_SUPPORT 1
#endif

//Camellia encryption support
#ifndef CAMELLIA_SUPPORT
   #define CAMELLIA_SUPPORT 1
#endif

//SEED encryption support
#ifndef SEED_SUPPORT
   #define SEED_SUPPORT 1
#endif

//ARIA encryption support
#ifndef ARIA_SUPPORT
   #define ARIA_SUPPORT 1
#endif

//ECB mode support
#ifndef ECB_SUPPORT
   #define ECB_SUPPORT 1
#endif

//CBC mode support
#ifndef CBC_SUPPORT
   #define CBC_SUPPORT 1
#endif

//CFB mode support
#ifndef CFB_SUPPORT
   #define CFB_SUPPORT 1
#endif

//OFB mode support
#ifndef OFB_SUPPORT
   #define OFB_SUPPORT 1
#endif

//CTR mode support
#ifndef CTR_SUPPORT
   #define CTR_SUPPORT 1
#endif

//CCM mode support
#ifndef CCM_SUPPORT
   #define CCM_SUPPORT 1
#endif

//GCM mode support
#ifndef GCM_SUPPORT
   #define GCM_SUPPORT 1
#endif

//Chacha support
#ifndef CHACHA_SUPPORT
   #define CHACHA_SUPPORT 1
#endif

//Poly1305 support
#ifndef POLY1305_SUPPORT
   #define POLY1305_SUPPORT 1
#endif

//Chacha20Poly1305 support
#ifndef CHACHA20_POLY1305_SUPPORT
   #define CHACHA20_POLY1305_SUPPORT 1
#endif

//Diffie-Hellman support
#ifndef DH_SUPPORT
   #define DH_SUPPORT 1
#endif

//RSA support
#ifndef RSA_SUPPORT
   #define RSA_SUPPORT 1
#endif

//DSA support
#ifndef DSA_SUPPORT
   #define DSA_SUPPORT 1
#endif

//Elliptic curve cryptography support
#ifndef EC_SUPPORT
   #define EC_SUPPORT 1
#endif

//ECDH support
#ifndef ECDH_SUPPORT
   #define ECDH_SUPPORT 1
#endif

//ECDSA support
#ifndef ECDSA_SUPPORT
   #define ECDSA_SUPPORT 1
#endif

//PKCS #5 support
#ifndef PKCS5_SUPPORT
   #define PKCS5_SUPPORT 1
#endif

//Yarrow PRNG support
#ifndef YARROW_SUPPORT
   #define YARROW_SUPPORT 1
#endif

//Object identifier support
#ifndef OID_SUPPORT
   #define OID_SUPPORT 1
#endif

//ASN.1 syntax support
#ifndef ASN1_SUPPORT
   #define ASN1_SUPPORT 1
#endif

//PEM file support
#ifndef PEM_SUPPORT
   #define PEM_SUPPORT 1
#endif

//X.509 certificate support
#ifndef X509_SUPPORT
   #define X509_SUPPORT 1
#endif

//Maximum context size (hash algorithms)
#if SHA3_512_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha3_512Context)
#elif SHA3_384_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha3_384Context)
#elif SHA3_256_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha3_256Context)
#elif SHA3_224_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha3_224Context)
#elif WHIRLPOOL_SUPPORT == 1
   #define MAX_HASH_CONTEXT_SIZE sizeof(WhirlpoolContext)
#elif SHA512_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha512Context)
#elif SHA384_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha384Context)
#elif SHA512_256_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha512_256Context)
#elif SHA512_224_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha512_224Context)
#elif SHA256_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha256Context)
#elif SHA224_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha224Context)
#elif TIGER_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(TigerContext)
#elif SHA1_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Sha1Context)
#elif RIPEMD160_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Ripemd160Context)
#elif RIPEMD128_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Ripemd128Context)
#elif MD5_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Md5Context)
#elif MD4_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Md4Context)
#elif MD2_SUPPORT
   #define MAX_HASH_CONTEXT_SIZE sizeof(Md2Context)
#endif

//Maximum context size (cipher algorithms)
#if ARIA_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(AriaContext)
#elif AES_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(AesContext)
#elif DES3_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(Des3Context)
#elif CAMELLIA_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(CamelliaContext)
#elif RC4_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(Rc4Context)
#elif RC6_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(Rc6Context)
#elif IDEA_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(IdeaContext)
#elif DES_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(DesContext)
#elif SEED_SUPPORT
   #define MAX_CIPHER_CONTEXT_SIZE sizeof(SeedContext)
#endif

//Maximum block size (cipher algorithms)
#if CAMELLIA_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE CAMELLIA_BLOCK_SIZE
#elif AES_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE AES_BLOCK_SIZE
#elif ARIA_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE ARIA_BLOCK_SIZE
#elif SEED_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE SEED_BLOCK_SIZE
#elif RC6_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE RC6_BLOCK_SIZE
#elif DES3_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE DES3_BLOCK_SIZE
#elif DES_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE DES_BLOCK_SIZE
#elif IDEA_SUPPORT
   #define MAX_CIPHER_BLOCK_SIZE IDEA_BLOCK_SIZE
#endif

//*----------------------------------------------------------------------------
//*			Trace
//*----------------------------------------------------------------------------
#ifndef TRACE_TLS_LEVEL
#define TRACE_TLS_LEVEL TRACE_LEVEL_NONE
#endif

#ifndef TRACE_MPI_LEVEL
#define TRACE_MPI_LEVEL TRACE_LEVEL_NONE
#endif

#define TRACE_TLS(...) 			TRACE_LEVEL(TRACE_TLS_LEVEL, __VA_ARGS__)
#define TRACE1_TLS(str)			TRACE1_LEVEL(TRACE_TLS_LEVEL, str)

#if TRACE_TLS_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACELN_TLS(...)			TRACELN(__VA_ARGS__)
#	define TRACELN1_TLS(...)		TRACELN1(__VA_ARGS__)
#	define TRACE_TLS_ARRAY(str, ptr, len) {TRACELN1(""); TRACE1(str); TRACE_BUF(ptr, len);}
#else
#	define TRACELN_TLS(str,...)
#	define TRACELN1_TLS(str)
#	define TRACE_TLS_ARRAY(str, ptr, len)
#endif

#if TRACE_MPI_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACE_MPI(p, a)			mpiDump(p, a)
#else
#	define TRACE_MPI(p, a)
#endif






#endif /* SERVICES_SECURITY_CRYPTO_CRYPTO_OPT_H_ */
