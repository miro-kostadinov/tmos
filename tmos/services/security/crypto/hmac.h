/*
 * hmac.h
 *
 *  Created on: Jun 14, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_HMAC_H_
#define SERVICES_SECURITY_CRYPTO_HMAC_H_

#include <tmos.h>
#include <crypto.h>

//MD2 hash support?
#if MD2_SUPPORT
   #include <md2.h>
#endif

//MD4 hash support?
#if MD4_SUPPORT
   #include <md4.h>
#endif

//MD5 hash support?
#if MD5_SUPPORT
   #include <md5.h>
#endif

//RIPEMD-128 hash support?
#if RIPEMD128_SUPPORT
   #include <ripemd128.h>
#endif

//RIPEMD-160 hash support?
#if RIPEMD160_SUPPORT
   #include <ripemd160.h>
#endif

//SHA-1 hash support?
#if SHA1_SUPPORT
   #include <sha1.h>
#endif

//SHA-224 hash support?
#if SHA224_SUPPORT
   #include <sha224.h>
#endif

//SHA-256 hash support?
#if SHA256_SUPPORT
   #include <sha256.h>
#endif

//SHA-384 hash support?
#if SHA384_SUPPORT
   #include <sha384.h>
#endif

//SHA-512 hash support?
#if SHA512_SUPPORT
   #include <sha512.h>
#endif

//SHA-512/224 hash support?
#if SHA512_224_SUPPORT
   #include <sha512_224.h>
#endif

//SHA-512/256 hash support?
#if SHA512_256_SUPPORT
   #include <sha512_256.h>
#endif

//SHA3-224 hash support?
#if SHA3_224_SUPPORT
   #include <sha3_224.h>
#endif

//SHA3-256 hash support?
#if SHA3_256_SUPPORT
   #include <sha3_256.h>
#endif

//SHA3-384 hash support?
#if SHA3_384_SUPPORT
   #include <sha3_384.h>
#endif

//SHA3-512 hash support?
#if SHA3_512_SUPPORT
   #include <sha3_512.h>
#endif

//Tiger hash support?
#if TIGER_SUPPORT
   #include <tiger.h>
#endif

//Whirlpool hash support?
#if WHIRLPOOL_SUPPORT
   #include <whirlpool.h>
#endif

//Maximum block size (hash algorithms)
#if SHA3_224_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA3_224_BLOCK_SIZE
#elif SHA3_256_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA3_256_BLOCK_SIZE
#elif SHA512_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA512_BLOCK_SIZE
#elif SHA384_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA384_BLOCK_SIZE
#elif SHA512_256_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA512_256_BLOCK_SIZE
#elif SHA512_224_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA512_224_BLOCK_SIZE
#elif SHA3_384_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA3_384_BLOCK_SIZE
#elif SHA3_512_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA3_512_BLOCK_SIZE
#elif SHA256_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA256_BLOCK_SIZE
#elif SHA224_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA224_BLOCK_SIZE
#elif SHA1_SUPPORT
   #define MAX_HASH_BLOCK_SIZE SHA1_BLOCK_SIZE
#elif WHIRLPOOL_SUPPORT
   #define MAX_HASH_BLOCK_SIZE WHIRLPOOL_BLOCK_SIZE
#elif TIGER_SUPPORT
   #define MAX_HASH_BLOCK_SIZE TIGER_BLOCK_SIZE
#elif RIPEMD160_SUPPORT
   #define MAX_HASH_BLOCK_SIZE RIPEMD160_BLOCK_SIZE
#elif RIPEMD128_SUPPORT
   #define MAX_HASH_BLOCK_SIZE RIPEMD128_BLOCK_SIZE
#elif MD5_SUPPORT
   #define MAX_HASH_BLOCK_SIZE MD5_BLOCK_SIZE
#elif MD4_SUPPORT
   #define MAX_HASH_BLOCK_SIZE MD4_BLOCK_SIZE
#elif MD2_SUPPORT
   #define MAX_HASH_BLOCK_SIZE MD2_BLOCK_SIZE
#endif

//Maximum digest size (hash algorithms)
#if WHIRLPOOL_SUPPORT
   #define MAX_HASH_DIGEST_SIZE WHIRLPOOL_DIGEST_SIZE
#elif SHA3_512_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA3_512_DIGEST_SIZE
#elif SHA512_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA512_DIGEST_SIZE
#elif SHA3_384_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA3_384_DIGEST_SIZE
#elif SHA384_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA384_DIGEST_SIZE
#elif SHA3_256_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA3_256_DIGEST_SIZE
#elif SHA512_256_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA512_256_DIGEST_SIZE
#elif SHA256_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA256_DIGEST_SIZE
#elif SHA3_224_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA3_224_DIGEST_SIZE
#elif SHA512_224_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA512_224_DIGEST_SIZE
#elif SHA224_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA224_DIGEST_SIZE
#elif TIGER_SUPPORT
   #define MAX_HASH_DIGEST_SIZE TIGER_DIGEST_SIZE
#elif SHA1_SUPPORT
   #define MAX_HASH_DIGEST_SIZE SHA1_DIGEST_SIZE
#elif RIPEMD160_SUPPORT
   #define MAX_HASH_DIGEST_SIZE RIPEMD160_DIGEST_SIZE
#elif RIPEMD128_SUPPORT
   #define MAX_HASH_DIGEST_SIZE RIPEMD128_DIGEST_SIZE
#elif MD5_SUPPORT
   #define MAX_HASH_DIGEST_SIZE MD5_DIGEST_SIZE
#elif MD4_SUPPORT
   #define MAX_HASH_DIGEST_SIZE MD4_DIGEST_SIZE
#elif MD2_SUPPORT
   #define MAX_HASH_DIGEST_SIZE MD2_DIGEST_SIZE
#endif


//Inner padding (ipad)
#define HMAC_IPAD 0x36
//Outer padding (opad)
#define HMAC_OPAD 0x5C


struct hmac_ctxt_t
{
	hash_algo_t* hash_algo;
	uint8_t hmac_key[MAX_HASH_BLOCK_SIZE];
	uint8_t hmac_digest[MAX_HASH_DIGEST_SIZE];

	void Reset(hash_algo_t* hash, const void *key, uint32_t key_len);
	void Input(const void* data, size_t len);
	void Result(void* digest);

};



#endif /* SERVICES_SECURITY_CRYPTO_HMAC_H_ */
