/*
 * crypto.h
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_CRYPTO_H_
#define SERVICES_SECURITY_CRYPTO_CRYPTO_H_

#include <tmos.h>
#include <crypto_opt.h>
#include <common.h>
#include <memory.h>

//Rotate left operation
#define ROL8(a, n) (((a) << (n)) | ((a) >> (8 - (n))))
#define ROL16(a, n) (((a) << (n)) | ((a) >> (16 - (n))))
#define ROL32(a, n) (((a) << (n)) | ((a) >> (32 - (n))))
#define ROL64(a, n) (((a) << (n)) | ((a) >> (64 - (n))))

//Rotate right operation
#define ROR8(a, n) (((a) >> (n)) | ((a) << (8 - (n))))
#define ROR16(a, n) (((a) >> (n)) | ((a) << (16 - (n))))
#define ROR32(a, n) (((a) >> (n)) | ((a) << (32 - (n))))
#define ROR64(a, n) (((a) >> (n)) | ((a) << (64 - (n))))

//Shift left operation
#define SHL8(a, n) ((a) << (n))
#define SHL16(a, n) ((a) << (n))
#define SHL32(a, n) ((a) << (n))
#define SHL64(a, n) ((a) << (n))

//Shift right operation
#define SHR8(a, n) ((a) >> (n))
#define SHR16(a, n) ((a) >> (n))
#define SHR32(a, n) ((a) >> (n))
#define SHR64(a, n) ((a) >> (n))

/// Encryption algorithm type
enum cipher_type_t : char
{
   CIPHER_TYPE_STREAM = 0,
   CIPHER_TYPE_BLOCK  = 1
};


/// Cipher operation modes
enum cipher_mode_t : char
{
   CIPHER_MODE_NULL              = 0,
   CIPHER_MODE_STREAM            = 1,
   CIPHER_MODE_ECB               = 2,
   CIPHER_MODE_CBC               = 3,
   CIPHER_MODE_CFB               = 4,
   CIPHER_MODE_OFB               = 5,
   CIPHER_MODE_CTR               = 6,
   CIPHER_MODE_CCM               = 7,
   CIPHER_MODE_GCM               = 8,
   CIPHER_MODE_CHACHA20_POLY1305 = 9
};


struct algo_info_t
{
	const char*	algo_name;
	uint8_t 	block_size;
};

struct hash_algo_t;
typedef hash_algo_t* (*hash_allocator_t)();

struct hash_info_t
{
	algo_info_t		algo_info;
	uint8_t 		digest_size;
	uint8_t 		oid_size;
	const uint8_t*	oid;
	hash_allocator_t new_hash;
};

struct cipher_info_t
{
	algo_info_t		algo_info;
	cipher_type_t 	cipher_type;
};


/// Base class for algorithms
struct algo_base_t
{
	union
	{
		const algo_info_t* algo_info;
		const hash_info_t* hash_info;		//!< for hash algorithms only!
		const cipher_info_t* cipher_info;	//!< for cipher algorithms only!
	};

	algo_base_t(const algo_info_t* info): algo_info(info) {};
	virtual ~algo_base_t()
	{
	};
};

/// Base class for hash algorithms
struct hash_algo_t: public algo_base_t
{
	uint8_t 	indx;
	uint32_t 	total_blocks;

	hash_algo_t(const hash_info_t* info): algo_base_t(&info->algo_info)
	{
		hash_algo_t::Reset();
	};

	union
	{
		uint8_t  buf8[0];				//!< buf[], digest[]
		uint32_t buf32[0];				//!<
		uint64_t buf64[0];				//!<
	};

	virtual void Reset()
	{

	};
	virtual void Input(const void* data, size_t len);
	virtual void Result(void* digest)=0;
    virtual void process_block(const uint32_t* block)=0;


	void Input(const CSTRING& message)
	{
		Input(message.c_str(), message.length());
	}

	void Result(CSTRING& message_digest)
	{
		uint8_t* ptr;

		message_digest.clear();
		Result(nullptr);
		ptr = digest8();
		for(uint32_t i=0; i < hash_info->digest_size; i++)
		{
			message_digest.appendf("%02X", ptr[i]);
		}
	}

    uint8_t* digest8()
    {
    	return buf8 + hash_info->algo_info.block_size;
    }

    uint32_t* digest32()
    {
    	return static_cast<uint32_t*> ((void*)(buf8 + hash_info->algo_info.block_size));
    }

    uint64_t* digest64()
    {
    	return static_cast<uint64_t*> ((void*)(buf8 + hash_info->algo_info.block_size));
    }

	void Pad(uint8_t data, size_t len);
};

typedef hash_algo_t* (*new_hash_algo_f)();

#if GCM_SUPPORT
struct gcm_context_t
{
   uint32_t m[16][4];            ///<Precalculated table
};
#endif


/// Base class for encryption algorithms
struct cipher_algo_t: public algo_base_t
{
#if GCM_SUPPORT
	auto_ptr<gcm_context_t> gcm_ctxt;
#endif

	cipher_algo_t(const cipher_info_t* info): algo_base_t(&info->algo_info) {};

	virtual RES_CODE init_key(const uint8_t* key, size_t key_size)=0;

	virtual void encrypt(const void* input, void* output, size_t blocks)=0;
	virtual void decrypt(const void* input, void* output, size_t blocks)=0;

#if CBC_SUPPORT
	// in place encrypt/decrypt
	RES_CODE cbc_encrypt(uint8_t* iv, uint8_t* data, size_t len);
	RES_CODE cbc_decrypt(uint8_t* iv, const uint8_t* c, uint8_t* p, size_t len);
#endif
#if CCM_SUPPORT
	RES_CODE ccm_encrypt(const uint8_t *n, size_t nLen, const uint8_t *a,
			size_t aLen, uint8_t *data, size_t len, uint8_t *t, size_t tLen);
	RES_CODE ccm_decrypt(const uint8_t *n, size_t nLen, const uint8_t *a,
			size_t aLen, const uint8_t* c, uint8_t* p, size_t len, uint8_t *t, size_t tLen);
#endif
#if GCM_SUPPORT
	RES_CODE gcm_init();
	RES_CODE gcm_encrypt(const uint8_t *iv, size_t ivLen, const uint8_t *a,
			size_t aLen, uint8_t *data, size_t len, uint8_t *t, size_t tLen);
	RES_CODE gcm_decrypt(const uint8_t *iv, size_t ivLen, const uint8_t *a,
			size_t aLen, const uint8_t* c, uint8_t* p, size_t len, uint8_t *t, size_t tLen);
#endif
};

typedef cipher_algo_t* (*new_cipher_algo_f)();


/// Base class for pseudo-random number generators
struct prng_algo_t: public algo_base_t
{
	prng_algo_t(const algo_info_t* info): algo_base_t(info) {};

	virtual RES_CODE prng_init()=0;
	virtual RES_CODE prng_seed(const void* input, size_t length)=0;
	virtual RES_CODE prng_read(void* output, size_t length)=0;
};

uint32_t get_gmt_seconds();

#endif /* SERVICES_SECURITY_CRYPTO_CRYPTO_H_ */
