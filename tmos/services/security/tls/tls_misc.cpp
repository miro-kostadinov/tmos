/*
 * tls_misc.cpp
 *
 *  Created on: Jul 11, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <tls_misc.h>
#include <hmac.h>
#include <pem.h>
#include <x509.h>
#include <oid.h>

RES_CODE tls_context_t::tls_set_version(tls_version_t version)
{
	//Check TLS version
	if (version < REV16(TLS_MIN_VERSION) || version > REV16(TLS_MAX_VERSION))
	{
		return RES_TLS_INVALID_VERSION;
	}

	//Save the TLS protocol version to use
	tls_version = version;

	return RES_OK;
}


RES_CODE tls_context_t::tls_set_ciphersuite(tls_suite_id_t suite)
{
	RES_CODE res;
	bool acceptable;
	const tls_chipher_suite_info* info;
	uint32_t i;


	info = nullptr;

   //Restrict the use of certain cipher suites?
	if (num_ciphers > 0)
	{
		//This flag will be set if the specified cipher suite is acceptable
		acceptable = false;

		//Loop through allowed cipher suites
		for (i = 0; i < num_ciphers; i++)
		{
			//Compare cipher suite identifiers
			if (cipher_suites[i] == suite)
			{
				acceptable = true;
				break;
			}
		}
	}
	else
	{
		//The use of the cipher suite is not restricted
		acceptable = true;
	}

	//No restrictions exist concerning the use of the specified cipher suite?
	if (acceptable)
	{
		//This flag will be set if the specified cipher suite is acceptable
		acceptable = false;
		uint32_t n = tlsGetNumSupportedCipherSuites();;

		//Loop through the list of supported cipher suites
		for (i = 0; i < n; i++)
		{
			//Point to the current item
			info = &tls_supported_cipher_suites[i];

			//Compare cipher suite identifiers
			if (info->suite_identifier == suite)
			{
				acceptable = true;
				break;
			}
		}
	}

	//TLS 1.2 cipher suites must not be negotiated in older versions of TLS
	if (acceptable && tls_version < TLS_VER12)
	{
		if (info->new_prf_hash_algo != nullptr)
			acceptable = false;
	}

	//Ensure that the selected cipher suite matches all the criteria
	if (acceptable)
	{
		//Save cipher suite identifier
		cipherSuite = suite;

		res = RES_OK;

		//Set encryption algorithm and hash function
		if(info->new_cipher_algo)
		{
			wrc.cipher_algo = info->new_cipher_algo();
			if(!wrc.cipher_algo.get())
				res = RES_OUT_OF_MEMORY;
			rdc.cipher_algo = info->new_cipher_algo();
			if(!rdc.cipher_algo.get())
				res = RES_OUT_OF_MEMORY;
		} else
		{
			wrc.cipher_algo.reset();
			rdc.cipher_algo.reset();
		}

		if(info->new_hash_algo)
		{
			hash_algo = info->new_hash_algo();
			if(!hash_algo.get())
				res = RES_OUT_OF_MEMORY;
		}
		else
			hash_algo.reset();
		if(info->new_prf_hash_algo)
		{
			prf_hash_algo = info->new_prf_hash_algo();
			if(!prf_hash_algo.get())
				res = RES_OUT_OF_MEMORY;
		}
		else
			prf_hash_algo.reset();

		//Size of the verify data
		verify_data_len = info->verify_data_len;

		//PRF with the SHA-256 is used for all cipher suites published
		//prior than TLS 1.2 when TLS 1.2 is negotiated
		if(!prf_hash_algo.get())
		{
			prf_hash_algo = new_sha256_hash();
			if(!prf_hash_algo.get())
				res = RES_OUT_OF_MEMORY;
		}

		//The length of the verify data depends on the TLS version currently used
		if (tls_version == SSL_VER30)
			verify_data_len = 36;
		else if (tls_version <= TLS_VER11)
			verify_data_len = 12;

		cipher_info = info;

	}
	else
	{
		//Debug message
		TRACE_ERROR("Cipher suite not supported!\r\n");
		//The specified cipher suite is not supported
		res = RES_TLS_HANDSHAKE_FAILED;
	}

	return res;
}


RES_CODE tls_context_t::tls_set_compression_method(tls_compression_method_t method)
{
	//Check if the requested compression algorithm is supported
	if (method != TLS_COMPRESSION_METHOD_NULL)
		return RES_TLS_HANDSHAKE_FAILED;

	//Save compression method identifier
	compressionMethod = method;

	return RES_OK;
}

RES_CODE tls_context_t::tls_handshake_hash_init()
{
	//Allocate SHA-1 context
	handshake_sha1 = new_sha1_hash();
	if (handshake_sha1.get())
	{
		//SSL 3.0, TLS 1.0 or 1.1 currently selected?
		if (tls_version <= TLS_VER11)
		{
			//Allocate MD5 context
			handshake_hash = new_md5_hash();
		}
		//TLS 1.2 currently selected?
		else
		{
			if(cipher_info->new_prf_hash_algo)
				handshake_hash = cipher_info->new_prf_hash_algo();
			else
				handshake_hash = new_sha256_hash();
		}

		if(handshake_hash.get())
		{
			handshake_hash->Reset();
#if TLS_CLIENT_SUPPORT
			//TLS operates as a client?
			if (entity == TLS_CONNECTION_END_CLIENT)
			{
				if(last_txrc.tls_record.rec_type == TLS_TYPE_HANDSHAKE && last_txrc.data.get())
				{
					tls_client_hello_t* message;

					message = (tls_client_hello_t*)last_txrc.msg_start();
					tls_handshake_hash_update(message, last_txrc.msg_len);

				}
			}
#endif

			return RES_OK;
		}
	}
	return RES_OUT_OF_MEMORY;
}

void tls_context_t::tls_handshake_hash_update(const void *data, uint32_t len)
{
	hash_algo_t* hash;

	hash = handshake_sha1.get();
	if (hash)
		hash->Input(data, len);

	//SSL 3.0, TLS 1.0 or 1.1 currently selected?

	hash = handshake_hash.get();
	if (hash)
		hash->Input(data, len);
}

RES_CODE tls_context_t::tlsFinalizeHandshakeHash(const hash_algo_t* hash,
		const char *label, uint8_t *output)
{
	RES_CODE res;
	hash_algo_t* temp_hash;

	//Check parameters
	if (!hash || !label || !output)
		return RES_TLS_INVALID_PARAMETER;

	//Allocate a temporary hash context
	temp_hash = hash->hash_info->new_hash();
	if (temp_hash == nullptr)
		return RES_OUT_OF_MEMORY;

	//Original hash context must be preserved
	memcpy((void*)temp_hash, hash, sizeof(hash_algo_t) + hash->hash_info->digest_buf_size);

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
	//SSL 3.0 currently selected?
	if(tls_version == SSL_VER30)
	{
		size_t padLength;

		//The pad character is repeated 48 times for MD5 or 40 times for SHA-1
		padLength = (hash->hash_info == MD5_HASH_ALGO) ? 48 : 40;

		//hash(handshakeMessages + label + masterSecret + pad1)
		temp_hash->Input(label, strlen(label));
		temp_hash->Input(masterSecret, 48);
		temp_hash->Pad(0x36, padLength);
		temp_hash->Result(output);

		//hash(masterSecret + pad2 + hash(handshakeMessages + label + masterSecret + pad1))
		temp_hash->Reset();
		temp_hash->Input(masterSecret, 48);
		temp_hash->Pad(0x5C, padLength);
		temp_hash->Input(output, hash->hash_info->digest_size);
		temp_hash->Result(output);

		//Successful processing
		res = RES_OK;
	}
	else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_0 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.0, TLS 1.1 or TLS 1.2 currently selected?
	if (tls_version >= TLS_VER10 && tls_version <= TLS_VER12)
	{
		//Compute hash(handshakeMessages)
		temp_hash->Result(output);
		//Successful processing
		res = RES_OK;
	}
	else
#endif
	//The negotiated TLS version is not valid...
	{
		//Report an error
		res = RES_TLS_INVALID_VERSION;
	}

	//Release previously allocated resources
	delete temp_hash;

	return res;
}

RES_CODE sslExpandKey(const void* secret, size_t secretLength,
   const void* ran1, const void* ran2, uint8_t *output, size_t outputLength)
{
	RES_CODE res;
	uint32_t i, n;
	char pad[16];
	auto_ptr<sha1_algo_t> sha1;
	auto_ptr<md5_algo_t> md5;

	//Allocate a memory buffer to hold the MD5 context
	md5 = (md5_algo_t*) new_md5_hash();
	//Allocate a memory buffer to hold the SHA-1 context
	sha1 = (sha1_algo_t*) new_sha1_hash();

	//Failed to allocate memory?
	if (md5.get() && sha1.get())
	{
		//Loop until enough output has been generated
		for (i = 0; outputLength > 0; i++)
		{
			//Generate pad
			memset(pad, 'A' + i, i + 1);

			//Compute SHA(pad + secret + random)
			sha1->Reset();
			sha1->Input(pad, i + 1);
			sha1->Input(secret, secretLength);
			sha1->Input(ran1, 32);
			sha1->Input(ran2, 32);
			sha1->Result(nullptr);

			//Then compute MD5(secret + SHA(pad + secret + random))
			md5->Reset();
			md5->Input(secret, secretLength);
			md5->Input(sha1->digest8(), SHA1_DIGEST_SIZE);
			md5->Result(nullptr);

			//Calculate the number of bytes to copy
			n = min(outputLength, MD5_DIGEST_SIZE);
			//Copy the resulting hash value
			memcpy(output, md5->digest8(), n);

			//Advance data pointer
			output += n;
			//Decrement byte counter
			outputLength -= n;
		}
		res = RES_OK;
	} else
		res = RES_OUT_OF_MEMORY;

	return res;
}

RES_CODE tlsPrf(const uint8_t* secret, size_t secretLength, const char* label,
   const void* seed1, size_t seed1Length, const void* seed2, size_t seed2Length,
   uint8_t* output, size_t outputLength)
{
	RES_CODE res;
	auto_ptr<hmac_ctxt_t> hmac;
	auto_ptr<sha1_algo_t> sha1;
	auto_ptr<md5_algo_t> md5;
	size_t labelLength;
	size_t sLength;
	const uint8_t* s1;
	const uint8_t* s2;
	uint32_t i, j;
	uint8_t a[SHA1_DIGEST_SIZE];


	hmac = new hmac_ctxt_t;
	md5 = (md5_algo_t*) new_md5_hash();
	sha1 = (sha1_algo_t*) new_sha1_hash();

	if (hmac.get() && md5.get() && sha1.get())
	{
		//Compute the length of the label
		labelLength = strlen(label);

		//The secret is partitioned into two halves S1 and S2
		//with the possibility of one shared byte
		sLength = (secretLength + 1) / 2;
		//S1 is taken from the first half of the secret
		s1 = secret;
		//S2 is taken from the second half
		s2 = secret + secretLength - sLength;

	   //First compute A(1) = HMAC_MD5(S1, label + seed)
		TRACELN_TLS("  hash secret[%u]:", sLength);
		TRACE_TLS_ARRAY("    ", s1, sLength);

		TRACELN_TLS("  hash seed[%u]:", labelLength+seed1Length+seed2Length);
		TRACE_TLS_ARRAY("    ", label, labelLength);
		TRACE_TLS_ARRAY("    ", seed1, seed1Length);
		if(seed2Length)
			TRACE_TLS_ARRAY("    ", seed2, seed2Length);

		hmac->Reset(md5.get(), s1, sLength);
		hmac->Input(label, labelLength);
		hmac->Input(seed1, seed1Length);
		if(seed2Length)
			hmac->Input(seed2, seed2Length);
		hmac->Result(a);

		//Apply the data expansion function P_MD5
		for (i = 0; i < outputLength;)
		{
			//Compute HMAC_MD5(S1, A(i) + label + seed)
			hmac->Reset(md5.get(), s1, sLength);
			hmac->Input(a, MD5_DIGEST_SIZE);
			hmac->Input(label, labelLength);
			hmac->Input(seed1, seed1Length);
			if(seed2Length)
				hmac->Input(seed2, seed2Length);
			hmac->Result(nullptr);

			//Copy the resulting digest
			for (j = 0; i < outputLength && j < MD5_DIGEST_SIZE; i++, j++)
				output[i] = hmac->hmac_digest[j];

			//Compute A(i + 1) = HMAC_MD5(S1, A(i))
			hmac->Reset(md5.get(), s1, sLength);
			hmac->Input(a, MD5_DIGEST_SIZE);
			hmac->Result(a);
		}
		TRACELN1_TLS("  md5:");
		TRACE_TLS_ARRAY("    ", output, outputLength);

		//First compute A(1) = HMAC_SHA1(S2, label + seed)
		hmac->Reset(sha1.get(), s2, sLength);
		hmac->Input(label, labelLength);
		hmac->Input(seed1, seed1Length);
		if(seed2Length)
			hmac->Input(seed2, seed2Length);
		hmac->Result(a);

		//Apply the data expansion function P_SHA1
		for (i = 0; i < outputLength;)
		{
			//Compute HMAC_SHA1(S2, A(i) + label + seed)
			hmac->Reset(sha1.get(), s2, sLength);
			hmac->Input(a, SHA1_DIGEST_SIZE);
			hmac->Input(label, labelLength);
			hmac->Input(seed1, seed1Length);
			if(seed2Length)
				hmac->Input(seed2, seed2Length);
			hmac->Result(nullptr);

			//Copy the resulting digest
			for (j = 0; i < outputLength && j < SHA1_DIGEST_SIZE; i++, j++)
				output[i] ^= hmac->hmac_digest[j];

			//Compute A(i + 1) = HMAC_SHA1(S2, A(i))
			hmac->Reset(sha1.get(), s2, sLength);
			hmac->Input(a, SHA1_DIGEST_SIZE);
			hmac->Result(a);
		}
		TRACELN1_TLS("  sha:");
		TRACE_TLS_ARRAY("    ", output, outputLength);
		res = RES_OK;
	} else
		res = RES_OUT_OF_MEMORY;

	return res;

}

RES_CODE tlsPrf2(const hash_info_t* hash_info, const uint8_t* secret, size_t secretLength,
   const char* label, const void* seed1, size_t seed1Length, const void* seed2,
   size_t seed2Length, uint8_t* output, size_t outputLength)
{
	RES_CODE res;
	auto_ptr<hmac_ctxt_t> hmac;
	auto_ptr<hash_algo_t> hash;
	uint8_t a[MAX_HASH_DIGEST_SIZE];
	size_t labelLength;
	size_t n;

	hmac = new hmac_ctxt_t;
	hash = hash_info->new_hash();
	if (hmac.get() && hash.get())
	{
		//Compute the length of the label
		labelLength = strlen(label);

		//First compute A(1) = HMAC_hash(secret, label + seed)
		hmac->Reset(hash.get(), secret, secretLength);
		hmac->Input(label, labelLength);
		hmac->Input(seed1, seed1Length);
		if(seed2Length)
			hmac->Input(seed2, seed2Length);
		hmac->Result(a);

		//Apply the data expansion function P_hash
		while (outputLength > 0)
		{
			//Compute HMAC_hash(secret, A(i) + label + seed)
			hmac->Reset(hash.get(), secret, secretLength);
			hmac->Input(a, hash_info->digest_size);
			hmac->Input(label, labelLength);
			hmac->Input(seed1, seed1Length);
			if(seed2Length)
				hmac->Input(seed2, seed2Length);
			hmac->Result(nullptr);

			//Calculate the number of bytes to copy
			n = min(outputLength, hash_info->digest_size);
			//Copy the resulting digest
			memcpy(output, hmac->hmac_digest, n);

			//Compute A(i + 1) = HMAC_hash(secret, A(i))
			hmac->Reset(hash.get(), secret, secretLength);
			hmac->Input(a, hash_info->digest_size);
			hmac->Result(a);

			//Advance data pointer
			output += n;
			//Decrement byte counter
			outputLength -= n;
		}
		res = RES_OK;
	}
	else
		res = RES_OUT_OF_MEMORY;

	return res;
}


RES_CODE tls_context_t::tls_generate_keys()
{
	RES_CODE res;
	size_t n;

	//Length of necessary key material
	n = 2 * (cipher_info->mac_key_len + cipher_info->enc_key_len + cipher_info->fixedIV_len);

	//Make sure that the key block is large enough
	if (n > sizeof(keyBlock))
		return RES_ERROR;

	//Debug message
	TRACELN1_TLS("Generating keys...");
	TRACELN1_TLS("  Client random bytes:");
	TRACE_TLS_ARRAY("    ", &tls_randoms.client_random, 32);
	TRACELN1_TLS("  Server random bytes:");
	TRACE_TLS_ARRAY("    ", &tls_randoms.server_random, 32);

	//If a full handshake is being performed, the premaster secret
	//shall be first converted to the master secret
	if (!tls_resume)
	{
		//Debug message
		TRACELN1_TLS("  Premaster secret:");
		TRACE_TLS_ARRAY("    ", premasterSecret, premasterSecretLen);

		//Convert the premaster secret to the master secret
		if (tls_version == SSL_VER30)
		{
			//SSL 3.0 does not use a PRF, instead makes use abundantly of MD5
			res = sslExpandKey(premasterSecret, premasterSecretLen,
					&tls_randoms.client_random, &tls_randoms.server_random,
					masterSecret, 48);
		}
		else if (tls_version <= TLS_VER11)
		{
			//TLS 1.0 and 1.1 use a PRF that combines MD5 and SHA-1
			res = tlsPrf(premasterSecret, premasterSecretLen, "master secret",
					&tls_randoms.client_random, 32, &tls_randoms.server_random,
					32, masterSecret, 48);
		}
		else
		{
			//TLS 1.2 PRF uses SHA-256 or a stronger hash algorithm
			//as the core function in its construction
			res = tlsPrf2(prf_hash_algo->hash_info, premasterSecret, premasterSecretLen,
					"master secret", &tls_randoms.client_random, 32,
					&tls_randoms.server_random,	32, masterSecret, 48);
		}

		//Check the return status
		if (res != RES_OK)
			return res;

		//The premaster secret should be deleted from memory
		//once the master secret has been computed
		memclr(premasterSecret, 48);
	}

	//Debug message
	TRACELN1_TLS("  Master secret:");
	TRACE_TLS_ARRAY("    ", masterSecret, 48);

	//Perform key expansion
	if (tls_version == SSL_VER30)
	{
		//SSL 3.0 does not use a PRF, instead makes use abundantly of MD5
		res = sslExpandKey(masterSecret, 48, &tls_randoms.server_random,
				&tls_randoms.client_random, keyBlock, n);
	}
	else if (tls_version <= TLS_VER11)
	{
		//TLS 1.0 and 1.1 use a PRF that combines MD5 and SHA-1
		res = tlsPrf(masterSecret, 48, "key expansion",	&tls_randoms.server_random,
				32, &tls_randoms.client_random, 32, keyBlock, n);
	}
	else
	{
		//TLS 1.2 PRF uses SHA-256 or a stronger hash algorithm
		//as the core function in its construction
		res = tlsPrf2(prf_hash_algo->hash_info, masterSecret, 48, "key expansion",
				&tls_randoms.server_random,	32, &tls_randoms.client_random, 32,
				keyBlock, n);
	}

	//Any error while performing key expansion?
	if (res != RES_OK)
		return res;


	//Debug message
	TRACELN1_TLS("  Key block:");
	TRACE_TLS_ARRAY("    ", keyBlock, n);

	//TLS operates as a client?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//MAC keys
		wrc.mac_key = keyBlock;
		rdc.mac_key = wrc.mac_key + cipher_info->mac_key_len;
		//Encryption keys
		wrc.enc_key = rdc.mac_key + cipher_info->mac_key_len;
		rdc.enc_key = wrc.enc_key + cipher_info->enc_key_len;
		//Initialization vectors
		wrc.iv = rdc.enc_key + cipher_info->enc_key_len;
		rdc.iv = wrc.iv + cipher_info->fixedIV_len;
	}
	//TLS operates as a server?
	else
	{
		//MAC keys
		rdc.mac_key = keyBlock;
		wrc.mac_key = rdc.mac_key + cipher_info->mac_key_len;
		//Encryption keys
		rdc.enc_key = wrc.mac_key + cipher_info->mac_key_len;
		wrc.enc_key = rdc.enc_key + cipher_info->enc_key_len;
		//Initialization vectors
		rdc.iv = wrc.enc_key + cipher_info->enc_key_len;
		wrc.iv = rdc.iv + cipher_info->fixedIV_len;
	}

	//Dump MAC keys for debugging purpose
	if (cipher_info->mac_key_len > 0)
	{
		TRACELN1_TLS("  Write MAC key:");
		TRACE_TLS_ARRAY("    ", wrc.mac_key, cipher_info->mac_key_len);
		TRACELN1_TLS("  Read MAC key:");
		TRACE_TLS_ARRAY("    ", rdc.mac_key, cipher_info->mac_key_len);
	}

	//Dump encryption keys for debugging purpose
	TRACELN1_TLS("  Write encryption key:");
	TRACE_TLS_ARRAY("    ", wrc.enc_key, cipher_info->enc_key_len);
	TRACELN1_TLS("  Read encryption key:");
	TRACE_TLS_ARRAY("    ", rdc.enc_key, cipher_info->enc_key_len);

	//Dump initialization vectors for debugging purpose
	if (cipher_info->fixedIV_len > 0)
	{
		TRACELN1_TLS("  Write IV:");
		TRACE_TLS_ARRAY("    ", wrc.iv, cipher_info->fixedIV_len);
		TRACELN1_TLS("  Read IV:");
		TRACE_TLS_ARRAY("    ", rdc.iv, cipher_info->fixedIV_len);
	}

	return RES_OK;
}

RES_CODE tls_context_t::tls_init_decryption_engine()
{
	RES_CODE res = RES_ERROR;

	//Check cipher mode of operation
	if(cipher_info)
	{
		if (cipher_info->cipher_mode == CIPHER_MODE_STREAM
				|| cipher_info->cipher_mode == CIPHER_MODE_CBC
				|| cipher_info->cipher_mode == CIPHER_MODE_CCM
				|| cipher_info->cipher_mode == CIPHER_MODE_GCM)
		{
			//Sanity check
			if (rdc.cipher_algo.get())
			{
				//Configure the decryption engine with the read key
				res = rdc.cipher_algo->init_key(rdc.enc_key, cipher_info->enc_key_len);
			}
			else
			{
				//Report an error
				res = RES_ERROR;
			}

#if TLS_GCM_CIPHER_SUPPORT
			//GCM AEAD cipher?
			if (res == RES_OK && cipher_info->cipher_mode == CIPHER_MODE_GCM)
			{
				//Initialize GCM context
				res = rdc.cipher_algo->gcm_init();
			}
#endif
		}
		else if (cipher_info->cipher_mode == CIPHER_MODE_CHACHA20_POLY1305)
		{
			//We are done
			res = RES_OK;
		}
	}

	//Return status code
	return res;
}

/**
 * @brief Initialize encryption engine
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsInitEncryptionEngine()
{
	RES_CODE res;

	res = RES_TLS_FAIL;

	if(cipher_info)
	{
		//Check cipher mode of operation
		if (cipher_info->cipher_mode == CIPHER_MODE_STREAM
				|| cipher_info->cipher_mode == CIPHER_MODE_CBC
				|| cipher_info->cipher_mode == CIPHER_MODE_CCM
				|| cipher_info->cipher_mode == CIPHER_MODE_GCM)
		{
			//Sanity check
			if (wrc.cipher_algo.get())
			{
				//Configure the encryption engine with the write key
				res = wrc.cipher_algo->init_key(wrc.enc_key, cipher_info->enc_key_len);
			}

#if TLS_GCM_CIPHER_SUPPORT
			//GCM AEAD cipher?
			if(cipher_info->cipher_mode == CIPHER_MODE_GCM)
			{
				//Check status code
				if(res == RES_OK)
				{
					//Initialize GCM context
					res = wrc.cipher_algo->gcm_init();
				}
			}
#endif
		}
		else if (cipher_info->cipher_mode == CIPHER_MODE_CHACHA20_POLY1305)
		{
			//We are done
			res = RES_OK;
		}
	}

	//Return status code
	return res;
}

/**
 * @brief Verify ECDSA signature
 * @param[in] context Pointer to the TLS context
 * @param[in] digest Digest of the message whose signature is to be verified
 * @param[in] digestLength Length in octets of the digest
 * @param[in] signature Signature to be verified
 * @param[in] signatureLength Length of the signature to be verified
 * @return Error code
 **/
RES_CODE tls_context_t::tlsVerifyEcdsaSignature(const uint8_t* digest, size_t digestLength,
		const TlsDigitalSignature* signature)
{
#if TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT
	RES_CODE res;
	EcdsaSignature ecdsaSignature;

	//Read the ASN.1 encoded ECDSA signature
	res = ecdsaSignature.dsaReadSignature(signature->value, __REV16(signature->length));
	if(res == RES_OK)
	{
#if TLS_ECC_CALLBACK_SUPPORT
		//Any registered callback?
		if(ecdsaVerifyCallback != nullptr)
		{
			//Invoke user callback function
			res = ecdsaVerifyCallback(this, digest, digestLength, &ecdsaSignature);
		}
		else
#endif
		{
			//No callback function defined
			res = RES_TLS_UNSUPPORTED_ELLIPTIC_CURVE;
		}

		//Check status code
		if (res == RES_TLS_UNSUPPORTED_ELLIPTIC_CURVE
				|| res == RES_TLS_UNSUPPORTED_HASH_ALGO)
		{
			//ECDSA signature verification
			res = ecdsaSignature.ecdsaVerifySignature(&peerEcParams,
					&peerEcPublicKey, digest, digestLength);
		}
	}

	return res;
#else
	//ECDSA signature verification is not supported
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}


/**
 * @brief Select the hash algorithm to be used when generating signatures
 * @param[in] context Pointer to the TLS context
 * @param[in] signAlgo Desired signature algorithm (RSA, DSA or ECDSA)
 * @param[in] supportedSignAlgos List of supported signature/hash algorithm pairs
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSelectSignHashAlgo(tls_sign_algo_t signAlgo,
		const tls_sign_algos_t* supportedSignAlgos)
{
	uint32_t i;
	uint32_t n;

	//Check whether the peer has provided a list of supported
	//hash/signature algorithm pairs?
	if (supportedSignAlgos != nullptr)
	{
		//Process the list and select the relevant hash algorithm...
		signHashAlgo = TLS_HASH_ALGO_NONE;
		//Get the number of hash/signature algorithm pairs present in the list
		n = __REV16(supportedSignAlgos->length) / sizeof(tls_signature_algo_t);

		//The hash algorithm to be used when generating signatures must be
		//one of those present in the list
		for (i = 0; i < n; i++)
		{
			//Acceptable signature algorithm found?
			if (supportedSignAlgos->value[i].signature == signAlgo)
			{
				//TLS operates as a client?
				if (entity == TLS_CONNECTION_END_CLIENT)
				{
					//Check whether the associated hash algorithm is supported
					if (tlsGetHashAlgo(supportedSignAlgos->value[i].hash)
							== prf_hash_algo->hash_info)
						signHashAlgo = supportedSignAlgos->value[i].hash;
#if TLS_SHA1_SUPPORT
					else if(supportedSignAlgos->value[i].hash == TLS_HASH_ALGO_SHA1)
					signHashAlgo =  supportedSignAlgos->value[i].hash;
#endif
				}
				//TLS operates as a server?
				else
				{
					//Check whether the associated hash algorithm is supported
					switch (supportedSignAlgos->value[i].hash)
					{
#if TLS_MD5_SUPPORT
					//MD5 hash identifier?
					case TLS_HASH_ALGO_MD5:
#endif
#if TLS_SHA1_SUPPORT
						//SHA-1 hash identifier?
						case TLS_HASH_ALGO_SHA1:
#endif
#if TLS_SHA224_SUPPORT
						//SHA-224 hash identifier?
						case TLS_HASH_ALGO_SHA224:
#endif
#if TLS_SHA256_SUPPORT
						//SHA-256 hash identifier?
						case TLS_HASH_ALGO_SHA256:
#endif
#if TLS_SHA384_SUPPORT
						//SHA-384 hash identifier?
						case TLS_HASH_ALGO_SHA384:
#endif
#if TLS_SHA512_SUPPORT
						//SHA-512 hash identifier?
					case TLS_HASH_ALGO_SHA512:
#endif
						//Select the hash algorithm to be used for signing
						signHashAlgo = supportedSignAlgos->value[i].hash;
						break;
					default:
						break;
					}
				}

				//Acceptable hash algorithm found?
				if (signHashAlgo != TLS_HASH_ALGO_NONE)
					break;
			}
		}
	}
	else
	{
		//Use default hash algorithm when generating RSA, DSA or ECDSA signatures
#if TLS_SHA1_SUPPORT
		signHashAlgo = TLS_HASH_ALGO_SHA1;
#elif (TLS_SHA224_SUPPORT)
		signHashAlgo = TLS_HASH_ALGO_SHA224;
#elif (TLS_SHA256_SUPPORT)
		signHashAlgo = TLS_HASH_ALGO_SHA256;
#elif (TLS_SHA384_SUPPORT)
		signHashAlgo = TLS_HASH_ALGO_SHA384;
#elif (TLS_SHA512_SUPPORT)
		signHashAlgo = TLS_HASH_ALGO_SHA512;
#endif
	}

	//If no acceptable choices are presented, return an error
	if (signHashAlgo == TLS_HASH_ALGO_NONE)
		return RES_TLS_FAIL;

	//Successful processing
	return RES_OK;
}

/**
 * @brief Compute verify data from previous handshake messages
 * @param[in] context Pointer to the TLS context
 * @param[in] entity Specifies whether the computation is performed at client or server side
 * @return Error code
 **/
RES_CODE tls_context_t::tlsComputeVerifyData(tls_connection_end_t ce)
{
	RES_CODE res;
	const char* label;

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
	//SSL 3.0 currently selected?
	if(tls_version == SSL_VER30)
	{
		//Computation is performed at client or server side?
		label = (ce == TLS_CONNECTION_END_CLIENT) ? "CLNT" : "SRVR";

		//Compute MD5(masterSecret + pad2 + MD5(handshakeMessages + label + masterSecret + pad1))
		res = tlsFinalizeHandshakeHash(handshake_hash.get(), label, verifyData);
		if(res != RES_OK)
			return res;

		//Compute SHA(masterSecret + pad2 + SHA(handshakeMessages + label + masterSecret + pad1))
		res = tlsFinalizeHandshakeHash(handshake_sha1.get(), label, verifyData + MD5_DIGEST_SIZE);
		if(res != RES_OK)
			return res;
	}
	else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_0 && TLS_MIN_VERSION <= TLS_VERSION_1_1)
	//TLS 1.0 or 1.1 currently selected?
	if (tls_version == TLS_VER10 || tls_version == TLS_VER11)
	{
		//A temporary buffer is needed to concatenate MD5
		//and SHA-1 hash values before computing PRF
		uint8_t buffer[MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE];

		//Finalize MD5 hash computation
		res = tlsFinalizeHandshakeHash(handshake_hash.get(), "", buffer);
		if (res != RES_OK)
			return res;

		//Finalize SHA-1 hash computation
		res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", buffer + MD5_DIGEST_SIZE);
		if (res != RES_OK)
			return res;

		//Computation is performed at client or server side?
		label = (ce == TLS_CONNECTION_END_CLIENT) ?
				"client finished" : "server finished";

		//Verify data is always 12-byte long for TLS 1.0 and 1.1
		res = tlsPrf(masterSecret, 48, label, buffer,
		MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE, nullptr, 0, verifyData, 12);
		if (res != RES_OK)
			return res;
	}
	else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.2 currently selected?
	if (tls_version == TLS_VER12)
	{
		//Allocate a memory buffer to hold the hash algorithm context
		hash_algo_t* hash = prf_hash_algo->hash_info->new_hash();
		if (hash == nullptr)
			return RES_OUT_OF_MEMORY;

		//The original hash context must be preserved
		memcpy((void*)hash, handshake_hash.get(), sizeof(hash_algo_t)
				+ hash->hash_info->digest_buf_size);
		//Finalize hash computation
		hash->Result(nullptr);

		//Computation is performed at client or server side?
		label = (ce == TLS_CONNECTION_END_CLIENT) ?
				"client finished" : "server finished";

		//Generate the verify data
		res = tlsPrf2(hash->hash_info, masterSecret, 48, label, hash->digest8(),
				hash->hash_info->digest_size, nullptr, 0, verifyData,
				verify_data_len);

		//Release previously allocated memory
		delete hash;

		if (res != RES_OK)
			return res;
	}
	else
#endif
	//The negotiated TLS version is not valid...
	{
		//Report an error
		return RES_TLS_INVALID_VERSION;
	}

	//Debug message
	TRACELN1_TLS("Verify data:");
	TRACE_TLS_ARRAY("  ", verifyData, verify_data_len);

	//Successful processing
	return RES_OK;
}


/**
 * @brief Premaster secret generation (for PSK cipher suites)
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsGeneratePskPremasterSecret()
{
	RES_CODE res;

#if TLS_PSK_SUPPORT
	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK)
	{
		//Check whether the output buffer is large enough to hold the premaster secret
		if ((pskLen * 2 + 4) <= TLS_MAX_PREMASTER_SECRET_SIZE)
		{
			//The premaster secret is formed as follows: if the PSK is N octets
			//long, concatenate a uint16 with the value N, N zero octets, a second
			//uint16 with the value N, and the PSK itself
			premasterSecret[0] = pskLen >>8;
			premasterSecret[1] = pskLen;
			memclr(premasterSecret + 2, pskLen);
			premasterSecret[pskLen + 2] = pskLen >>8;
			premasterSecret[pskLen + 3] = pskLen;
			memcpy(premasterSecret + pskLen + 4, psk, pskLen);

			//Save the length of the premaster secret
			premasterSecretLen = pskLen * 2 + 4;

			//Premaster secret successfully generated
			res = RES_OK;
		}
		else
		{
			//Report an error
			res = RES_TLS_BUFFER_OVERFLOW;
		}
	}
	else
#endif
#if (TLS_RSA_PSK_SUPPORT || TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//RSA_PSK, DHE_PSK or ECDHE_PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{

		//Check whether the output buffer is large enough to hold the premaster secret
		if ((premasterSecretLen + pskLen + 4) <= TLS_MAX_PREMASTER_SECRET_SIZE)
		{
			//The "other_secret" field comes from the Diffie-Hellman, ECDH or
			//RSA exchange (DHE_PSK, ECDH_PSK and RSA_PSK, respectively)
			memmove(premasterSecret + 2, premasterSecret, premasterSecretLen);

			//The "other_secret" field is preceded by a 2-byte length field
			premasterSecret[0] = premasterSecretLen >> 8;
			premasterSecret[1] = premasterSecretLen;

			//if the PSK is N octets long, concatenate a uint16 with the value N
			premasterSecret[premasterSecretLen + 2] = pskLen >> 8;
			premasterSecret[premasterSecretLen + 3] = pskLen ;

			//Concatenate the PSK itself
			memcpy(premasterSecret + premasterSecretLen + 4, psk, pskLen);

			//Adjust the length of the premaster secret
			premasterSecretLen += pskLen + 4;

			//Premaster secret successfully generated
			res = RES_OK;
		}
		else
		{
			//Report an error
			res = RES_TLS_BUFFER_OVERFLOW;
		}
	}
	else
#endif
	//Invalid key exchange method?
	{
		//The specified key exchange method is not supported
		res = RES_TLS_UNSUPPORTED_KEY_EXCH_METHOD;
	}

	//Return status code
	return res;
}

/**
 * @brief Generate DSA signature
 * @param[in] context Pointer to the TLS context
 * @param[in] digest Digest of the message to be signed
 * @param[in] digestLength Length in octets of the digest
 * @param[out] signature Resulting signature
 * @param[out] signatureLength Length of the resulting signature
 * @return Error code
 **/
RES_CODE tls_context_t::tlsGenerateDsaSignature(const uint8_t* digest,
   size_t digestLength, uint8_t** signature, size_t* signatureLength)
{
#if (TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT)
	RES_CODE res;
	DsaPrivateKey privateKey;
	DsaSignature dsaSignature;

	//Decode the PEM structure that holds the DSA private key
	res = pemReadDsaPrivateKey(cert->privateKey, cert->privateKeyLength,
			&privateKey);

	//Check status code
	if (res == RES_OK)
	{
		//Generate DSA signature
		res = dsaSignature.dsaGenerateSignature(tls_randoms.prng_algo.get(),
				&privateKey, digest, digestLength);
	}

	//Check status code
	if (res == RES_OK)
	{
		size_t len;

		// calculate the size and allocate memory
		res = dsaSignature.dsaWriteSignatureLen(&len);
		if(res == RES_OK)
		{
			uint8_t* mem;

			mem = (uint8_t*)tsk_realloc(*signature, len + *signatureLength);
			if(mem != nullptr)
			{
				*signature = mem;

				//Encode the resulting (R, S) integer pair using ASN.1
				res = dsaSignature.dsaWriteSignature(mem + *signatureLength, &len);
				*signatureLength += len;
			} else
				res = RES_OUT_OF_MEMORY;
		}

	}

	return res;
#else
	//DSA signature generation is not supported
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Generate ECDSA signature
 * @param[in] context Pointer to the TLS context
 * @param[in] digest Digest of the message to be signed
 * @param[in] digestLength Length in octets of the digest
 * @param[out] signature Resulting signature
 * @param[out] signatureLength Length of the resulting signature
 * @return Error code
 **/
RES_CODE tls_context_t::tlsGenerateEcdsaSignature(const uint8_t* digest,
   size_t digestLength, uint8_t** signature, size_t* signatureLength)
{
#if TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT
	RES_CODE res;
	EcdsaSignature ecdsaSignature;

#if TLS_ECC_CALLBACK_SUPPORT
	//Any registered callback?
	if(ecdsaSignCallback != nullptr)
	{
		//Invoke user callback function
		res = ecdsaSignCallback(this, digest, digestLength, &ecdsaSignature);
	}
	else
#endif
	{
		EcDomainParameters params;
		Mpi privateKey;

		//Decode the PEM structure that holds the EC domain parameters
		res = pemReadEcParameters(cert->privateKey.c_str(), cert->privateKeyLength, &params);

		//Check status code
		if(res == RES_OK)
		{
			//Decode the PEM structure that holds the EC private key
			res = pemReadEcPrivateKey(cert->privateKey.c_str(), cert->privateKeyLength,
					&privateKey);
		}

		//Check status code
		if(res == RES_OK)
		{
			//Generate ECDSA signature
			res = ecdsaSignature.ecdsaGenerateSignature(&params,
					tls_randoms.prng_algo.get(), &privateKey, digest,
					digestLength);
		}

	}

	//Check status code
	if(res == RES_OK)
	{
		size_t len;

		// calculate the size and allocate memory
		res = ecdsaSignature.ecdsaWriteSignatureLen(&len);
		if(res == RES_OK)
		{
			*signature = (uint8_t*)tsk_malloc(len + *signatureLength);
			if(*signature != nullptr)
			{
				//Encode the resulting (R, S) integer pair using ASN.1
				res = ecdsaSignature.ecdsaWriteSignature((*signature) +*signatureLength, &len);
				*signatureLength += len;
			} else
				res = RES_OUT_OF_MEMORY;
		}
	}

	//Return status code
	return res;
#else
	//ECDSA signature generation is not supported
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Translate an error code to an alert message
 * @param[in] context Pointer to the TLS context
 * @param[in] errorCode Internal error code
 * @return Error code
 **/
void tls_context_t::tlsProcessError(RES_CODE errorCode)
{
	//Check current state
	if (tls_state != TLS_STATE_CLOSED)
	{
		//Check status code
		switch (errorCode)
		{
		//The timeout interval has elapsed
		case RES_IDLE:
			break;
			//The read/write operation has failed
		case RES_TLS_WRITE_FAILED:
		case RES_TLS_READ_FAILED:
			break;
			//An inappropriate message was received
		case RES_TLS_UNEXPECTED_MESSAGE:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_UNEXPECTED_MESSAGE);
			break;
			//A record is received with an incorrect MAC
		case RES_TLS_BAD_RECORD_MAC:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_BAD_RECORD_MAC);
			break;
			//Invalid record length
		case RES_TLS_RECORD_OVERFLOW:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL, TLS_ALERT_RECORD_OVERFLOW);
			break;
			//Unable to negotiate an acceptable set of security parameters
		case RES_TLS_HANDSHAKE_FAILED:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_HANDSHAKE_FAILURE);
			break;
			//A certificate was corrupt
		case RES_TLS_BAD_CERTIFICATE:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL, TLS_ALERT_BAD_CERTIFICATE);
			break;
			//A certificate has expired or is not currently valid
		case RES_TLS_CERTIFICATE_EXPIRED:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_CERTIFICATE_EXPIRED);
			break;
			//A field in the handshake was out of range or inconsistent with other fields
		case RES_TLS_ILLEGAL_PARAMETER:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_ILLEGAL_PARAMETER);
			break;
			//The certificate could not be matched with a known, trusted CA
		case RES_TLS_UNKNOWN_CA:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL, TLS_ALERT_UNKNOWN_CA);
			break;
			//A message could not be decoded because some field was incorrect
		case RES_TLS_DECODING_FAILED:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_DECODE_ERROR);
			break;
			//A handshake cryptographic operation failed
		case RES_TLS_INVALID_SIGNATURE:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_DECRYPT_ERROR);
			break;
			//The protocol version the client has attempted to negotiate is not supported
		case RES_TLS_VERSION_NOT_SUPPORTED:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_PROTOCOL_VERSION);
			break;
			//Internal error
		default:
			tlsSendAlert(TLS_ALERT_LEVEL_FATAL,	TLS_ALERT_INTERNAL_ERROR);
			break;
		}
	}
}

RES_CODE tls_context_t::tlsSelectNamedCurve(const tls_elliptic_curve_list_t* curveList)
{
	uint32_t i, n;

	//Check whether a list of elliptic curves has been provided
	if (curveList != nullptr)
	{
		//Process the list and select the relevant elliptic curve...
		named_curve = TLS_EC_CURVE_NONE;
		//Get the number of named curves present in the list
		n = __REV16(curveList->length) / sizeof(uint16_t);

		//The named curve to be used when performing ECDH key exchange must be
		//one of those present in the list
		for (i = 0; i < n; i++)
		{
			//Acceptable elliptic curve found?
			if (tlsGetCurveInfo(curveList->value[i]) != nullptr)
			{
				//Save the named curve
				named_curve = curveList->value[i];
				//We are done
				break;
			}
		}
	}
	else
	{
		//A client that proposes ECC cipher suites may choose not to include
		//the EllipticCurves extension. In this case, the server is free to
		//choose any one of the elliptic curves it supports
#if TLS_SECP256R1_SUPPORT
		named_curve = TLS_EC_CURVE_SECP256R1;
#else
		named_curve = TLS_EC_CURVE_NONE;
#endif
	}

	//If no acceptable choices are presented, return an error
	if (named_curve == TLS_EC_CURVE_NONE)
		return RES_TLS_FAIL;

	//Successful processing
	return RES_OK;
}

/**
 * @brief Verify RSA signature (SSL 3.0, TLS 1.0 and TLS 1.1)
 * @param[in] key Signer's RSA public key
 * @param[in] digest Digest of the message whose signature is to be verified
 * @param[in] signature Signature to be verified
 * @param[in] signatureLength Length of the signature to be verified
 * @return Error code
 **/
RES_CODE tlsVerifyRsaSignature(const RsaPublicKey* key, const uint8_t* digest,
		const TlsDigitalSignature* signature)
{
#if TLS_RSA_SIGN_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
	RES_CODE res;
	uint32_t i, k, signatureLength;
	uint8_t* em;
	Mpi s;
	Mpi m;

	signatureLength = __REV16(signature->length);

	//Debug message
	TRACE_TLS("RSA signature verification...");
//	TRACE_TLS("  Modulus:\r\n");
//	TRACE_MPI("    ", &key->n);
//	TRACE_TLS("  Public exponent:\r\n");
//	TRACE_MPI("    ", &key->e);
//	TRACE_TLS("  Message digest:\r\n");
//	TRACE_TLS_ARRAY("    ", digest, MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE);
//	TRACE_TLS("  Signature:\r\n");
//	TRACE_TLS_ARRAY("    ", signature, signatureLength);

	//Get the length in octets of the modulus n
	k = key->n.mpiGetByteLength();


	//Check the length of the signature
	if(signatureLength != k)
		return RES_TLS_INVALID_SIGNATURE;

	//Allocate a memory buffer to hold the encoded message
	em = (uint8_t*)tsk_malloc(k);
	if(em == nullptr)
		return RES_OUT_OF_MEMORY;

	//Start of exception handling block
	do
	{
		//Convert the signature to an integer signature representative s
		res = s.mpiReadRaw(signature->value, signatureLength);
		if(res != RES_OK)
			break;

		//Apply the RSAVP1 verification primitive
		res = key->rsavp1(&s, &m);
		if(res != RES_OK)
			break;

		//Convert the message representative m to an encoded message EM of length k octets
		res = m.mpiWriteRaw(em, k);
		if(res != RES_OK)
			break;

		//Debug message
//		TRACE_TLS("  Encoded message\r\n");
//		TRACE_TLS_ARRAY("    ", em, k);

		//Assume an error...
		res = RES_TLS_INVALID_SIGNATURE;

		//The first octet of EM must have a value of 0x00
		if(em[0] != 0x00)
			break;
		//The block type BT shall be 0x01
		if(em[1] != 0x01)
			break;

		//Check the padding string PS
		for(i = 2; i < k; i++)
		{
			//A 0x00 octet indicates the end of the padding string
			if(em[i] == 0x00)
				break;

			//Each byte of PS must be set to 0xFF when the block type is 0x01
			if(em[i] != 0xFF)
				break;
		}

		//Check whether the padding string is properly terminated
		if (i >= k || em[i] != 0x00)
			break;

		//The length of PS cannot be less than 8 octets
		if (i < 10)
			break;

		//Check the length of the digest
		if ((k - i - 1) != (MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE))
			break;
		//Check the digest value
		if (memcmp(digest, em + i + 1, MD5_DIGEST_SIZE + SHA1_DIGEST_SIZE))
			break;

		//The RSA signature is valid
		res = RES_OK;

		//End of exception handling block
	}while(0);

	//Release previously allocated memory
	tsk_free(em);

	//Return status code
	return res;
#else
	//RSA signature verification is not supported
	return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Verify DSA signature
 * @param[in] context Pointer to the TLS context
 * @param[in] digest Digest of the message whose signature is to be verified
 * @param[in] digestLength Length in octets of the digest
 * @param[in] signature Signature to be verified
 * @param[in] signatureLength Length of the signature to be verified
 * @return Error code
 **/
RES_CODE tlsVerifyDsaSignature(DsaPublicKey* key, const uint8_t* digest,
		size_t digestLength, const TlsDigitalSignature* signature)
{
#if (TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT)
   RES_CODE res;
   DsaSignature dsaSignature;


   //Read the ASN.1 encoded DSA signature
   res = dsaSignature.dsaReadSignature(signature->value, __REV16(signature->length));
   if(res == RES_OK)
   {
	   //DSA signature verification
	   res = dsaSignature.dsaVerifySignature(key, digest, digestLength);
   }

   //Return status code
   return res;
#else
   //DSA signature verification is not supported
   return RES_TLS_NOT_IMPLEMENTED;
#endif
}

/**
 * @brief Check whether a certificate is acceptable
 * @param[in] cert End entity certificate
 * @param[in] certTypes List of supported certificate types
 * @param[in] numCertTypes Size of the list that contains the supported certificate types
 * @param[in] signHashAlgos List of supported signature algorithms
 * @param[in] curveList List of supported elliptic curves
 * @param[in] certAuthorities List of trusted CA
 * @return TRUE if the specified certificate conforms to the requirements, else FALSE
 **/
bool tlsIsCertificateAcceptable(const TlsCertDesc *cert,
		const uint8_t* certTypes, size_t numCertTypes,
		const tls_sign_algos_t* signHashAlgos,
		const tls_elliptic_curve_list_t* curveList,
		const TlsCertAuthorities* certAuthorities)
{
	size_t i;
	size_t n;
	size_t length;
	bool acceptable;

	//Make sure that a valid certificate has been loaded
	if (!cert->certChain.length())
		return false;

	//This flag tells whether the certificate is acceptable
	acceptable = true;

	//Filter out certificates with unsupported type
	if (numCertTypes > 0)
	{
		//Loop through the list of supported certificate types
		for (acceptable = false, i = 0; i < numCertTypes; i++)
		{
			//Check whether the certificate type is acceptable
			if (certTypes[i] == cert->type)
			{
				acceptable = true;
				break;
			}
		}
	}

	//Filter out certificates that are signed with an unsupported
	//hash/signature algorithm
	if (acceptable && signHashAlgos != nullptr)
	{
		//Retrieve the number of items in the list
		n = __REV16(signHashAlgos->length) / sizeof(tls_signature_algo_t);

		//Loop through the list of supported hash/signature algorithm pairs
		for (acceptable = false, i = 0; i < n; i++)
		{
			//The certificate must be signed using a valid hash/signature algorithm pair
			if (signHashAlgos->value[i].signature == cert->signAlgo
					&& signHashAlgos->value[i].hash == cert->hashAlgo)
			{
				acceptable = true;
				break;
			}
		}
	}

	//Check whether the certificate contains an ECDSA public key
	if (cert->type == TLS_CERT_ECDSA_SIGN)
	{
		//Filter out ECDSA certificates that use an unsupported elliptic curve
		if (acceptable && curveList != nullptr)
		{
			//Retrieve the number of items in the list
			n = __REV16(curveList->length) / sizeof(uint16_t);

			//Loop through the list of supported elliptic curves
			for (acceptable = false, i = 0; i < n; i++)
			{
				//Check whether the elliptic curve is supported
				if (curveList->value[i] == cert->namedCurve)
				{
					acceptable = true;
					break;
				}
			}
		}
	}

	//Filter out certificates that are issued by a non trusted CA
	if (acceptable && certAuthorities != nullptr)
	{
		//Retrieve the length of the list
		length = __REV16(certAuthorities->length);

		//If the certificate authorities list is empty, then the client
		//may send any certificate of the appropriate type
		if (length > 0)
		{
			RES_CODE res;
			const uint8_t *p;
			const char* pemCert;
			size_t pemCertLength;
			size_t derCertSize;
			size_t derCertLength;
			uint8_t* derCert=nullptr;
			auto_ptr<X509CertificateInfo> certInfo;

			//The list of acceptable certificate authorities describes the known roots CA
			acceptable = false;

			//Point to the first distinguished name
			p = certAuthorities->value;

			//Point to the end entity certificate
			pemCert = cert->certChain.c_str();
			//Get the total length, in bytes, of the certificate chain
			pemCertLength = cert->certChain.length();

			//DER encoded certificate
			derCertSize = 0;
			derCertLength = 0;

			//Start of exception handling block
			do
			{
				//Allocate a memory buffer to store X.509 certificate info
				certInfo = new X509CertificateInfo;
				//Failed to allocate memory?
				if (certInfo.get() == nullptr)
					break;

				//Point to the last certificate of the chain
				do
				{
					//Read PEM certificates, one by one
					res = pemReadCertificate(&pemCert, &pemCertLength,
							&derCert, &derCertSize, &derCertLength);

					//Loop as long as necessary
				} while (res == RES_OK);

				//Any error to report?
				if (res != RES_EOF)
					break;

				//Parse the last certificate of the chain
				res = certInfo->x509ParseCertificate(derCert, derCertLength);
				//Failed to parse the X.509 certificate?
				if (res != RES_OK)
					break;

				//Parse each distinguished name of the list
				while (length > 0)
				{
					//Sanity check
					if (length < 2)
						break;

					//Each distinguished name is preceded by a 2-byte length field
					n = (p[0] << 8) + p[1];

					//Make sure the length field is valid
					if (length < (n + 2))
						break;

					//Check if the distinguished name matches the root CA
					if (n == certInfo->issuer.rawDataLen
							&& !memcmp(p + 2, certInfo->issuer.rawData, n))
					{
						acceptable = true;
						break;
					}

					//Check if the distinguished name matches the root CA
					if (n == certInfo->subject.rawDataLen
							&& !memcmp(p + 2, certInfo->subject.rawData, n))
					{
						acceptable = true;
						break;
					}

					//Advance data pointer
					p += n + 2;
					//Number of bytes left in the list
					length -= n + 2;
				}

				//End of exception handling block
			} while (0);

			//Release previously allocated memory
			tsk_free(derCert);
		}
	}

	//The return value specifies whether all the criteria were matched
	return acceptable;
}

/**
 * @brief Get the EC domain parameters that match the specified named curve
 * @param[in] namedCurve Elliptic curve identifier
 * @return Elliptic curve domain parameters
 **/
const EcCurveInfo* tlsGetCurveInfo(tls_ecnamedcurve_t namedCurve)
{
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT )
	//Explicit prime elliptic curves are not supported
	if (namedCurve == TLS_EC_CURVE_ARBITRARY_EXPLICIT_PRIME)
		return nullptr;
	//Explicit characteristic-2 elliptic curves are not supported
	else if (namedCurve == TLS_EC_CURVE_ARBITRARY_EXPLICIT_CHAR2)
		return nullptr;
#if TLS_SECP160K1_SUPPORT
	//secp160k1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP160K1)
		return SECP160K1_CURVE;
#endif
#if TLS_SECP160R1_SUPPORT
	//secp160r1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP160R1)
		return SECP160R1_CURVE;
#endif
#if TLS_SECP160R2_SUPPORT
	//secp160r2 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP160R2)
		return SECP160R2_CURVE;
#endif
#if TLS_SECP192K1_SUPPORT
	//secp192k1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP192K1)
		return SECP192K1_CURVE;
#endif
#if TLS_SECP192R1_SUPPORT
	//secp192r1 elliptic curve?
	else if(namedCurve == TLS_EC_CURVE_SECP192R1)
	return SECP192R1_CURVE;
#endif
#if TLS_SECP224K1_SUPPORT
	//secp224k1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP224K1)
		return SECP224K1_CURVE;
#endif
#if TLS_SECP224R1_SUPPORT
	//secp224r1 elliptic curve?
	else if(namedCurve == TLS_EC_CURVE_SECP224R1)
	return SECP224R1_CURVE;
#endif
#if TLS_SECP256K1_SUPPORT
	//secp256k1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_SECP256K1)
		return SECP256K1_CURVE;
#endif
#if TLS_SECP256R1_SUPPORT
	//secp256r1 elliptic curve?
	else if(namedCurve == TLS_EC_CURVE_SECP256R1)
	return SECP256R1_CURVE;
#endif
#if TLS_SECP384R1_SUPPORT
	//secp384r1 elliptic curve?
	else if(namedCurve == TLS_EC_CURVE_SECP384R1)
	return SECP384R1_CURVE;
#endif
#if TLS_SECP521R1_SUPPORT
	//secp521r1 elliptic curve?
	else if(namedCurve == TLS_EC_CURVE_SECP521R1)
	return SECP521R1_CURVE;
#endif
#if TLS_BRAINPOOLP256R1_SUPPORT
	//brainpoolP256r1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_BRAINPOOLP256R1)
		return BRAINPOOLP256R1_CURVE;
#endif
#if TLS_BRAINPOOLP384R1_SUPPORT
	//brainpoolP384r1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_BRAINPOOLP384R1)
		return BRAINPOOLP384R1_CURVE;
#endif
#if TLS_BRAINPOOLP512R1_SUPPORT
	//brainpoolP512r1 elliptic curve?
	else if (namedCurve == TLS_EC_CURVE_BRAINPOOLP512R1)
		return BRAINPOOLP512R1_CURVE;
#endif
	//Unknown identifier?
	else
		return nullptr;
#else
	//ECC not supported
	return nullptr;
#endif
}

const hash_info_t* tlsGetHashAlgo(tls_hash_algo_t hash_id)
{
	switch(hash_id)
	{
#if TLS_MD5_SUPPORT
	//MD5 hash identifier?
	case TLS_HASH_ALGO_MD5:
		return MD5_HASH_ALGO;
#endif
#if TLS_SHA1_SUPPORT
	//SHA-1 hash identifier?
	case TLS_HASH_ALGO_SHA1:
		return SHA1_HASH_ALGO;
#endif
#if TLS_SHA224_SUPPORT
	//SHA-224 hash identifier?
	case TLS_HASH_ALGO_SHA224:
		return SHA224_HASH_ALGO;
#endif
#if TLS_SHA256_SUPPORT
	//SHA-256 hash identifier?
	case TLS_HASH_ALGO_SHA256:
		return SHA256_HASH_ALGO;
#endif
#if TLS_SHA384_SUPPORT
	//SHA-384 hash identifier?
	case TLS_HASH_ALGO_SHA384:
		return SHA384_HASH_ALGO;
#endif
#if TLS_SHA512_SUPPORT
	//SHA-512 hash identifier?
	case TLS_HASH_ALGO_SHA512:
		return SHA512_HASH_ALGO;
#endif
	default:
		break;
	}

	return nullptr;
}

/**
 * @brief Get the named curve that matches the specified OID
 * @param[in] oid Object identifier
 * @param[in] length OID length
 * @return Named curve
 **/

tls_ecnamedcurve_t tlsGetNamedCurve(const uint8_t *oid, size_t length)
{
#if (TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT)
   //Make sure the object identifier is valid
   if(oid == NULL)
      return TLS_EC_CURVE_NONE;
#if (TLS_SECP160K1_SUPPORT)
   //secp160k1 elliptic curve?
   else if(!oidComp(oid, length, SECP160K1_OID, sizeof(SECP160K1_OID)))
      return TLS_EC_CURVE_SECP160K1;
#endif
#if (TLS_SECP160R1_SUPPORT)
   //secp160r1 elliptic curve?
   else if(!oidComp(oid, length, SECP160R1_OID, sizeof(SECP160R1_OID)))
      return TLS_EC_CURVE_SECP160R1;
#endif
#if (TLS_SECP160R2_SUPPORT)
   //secp160r2 elliptic curve?
   else if(!oidComp(oid, length, SECP160R2_OID, sizeof(SECP160R2_OID)))
      return TLS_EC_CURVE_SECP160R2;
#endif
#if (TLS_SECP192K1_SUPPORT)
   //secp192k1 elliptic curve?
   else if(!oidComp(oid, length, SECP192K1_OID, sizeof(SECP192K1_OID)))
      return TLS_EC_CURVE_SECP192K1;
#endif
#if (TLS_SECP192R1_SUPPORT)
   //secp192r1 elliptic curve?
   else if(!oidComp(oid, length, SECP192R1_OID, sizeof(SECP192R1_OID)))
      return TLS_EC_CURVE_SECP192R1;
#endif
#if (TLS_SECP224K1_SUPPORT)
   //secp224k1 elliptic curve?
   else if(!oidComp(oid, length, SECP224K1_OID, sizeof(SECP224K1_OID)))
      return TLS_EC_CURVE_SECP224K1;
#endif
#if (TLS_SECP224R1_SUPPORT)
   //secp224r1 elliptic curve?
   else if(!oidComp(oid, length, SECP224R1_OID, sizeof(SECP224R1_OID)))
      return TLS_EC_CURVE_SECP224R1;
#endif
#if (TLS_SECP256K1_SUPPORT)
   //secp256k1 elliptic curve?
   else if(!oidComp(oid, length, SECP256K1_OID, sizeof(SECP256K1_OID)))
      return TLS_EC_CURVE_SECP256K1;
#endif
#if (TLS_SECP256R1_SUPPORT)
   //secp256r1 elliptic curve?
   else if(!oidComp(oid, length, SECP256R1_OID, sizeof(SECP256R1_OID)))
      return TLS_EC_CURVE_SECP256R1;
#endif
#if (TLS_SECP384R1_SUPPORT)
   //secp384r1 elliptic curve?
   else if(!oidComp(oid, length, SECP384R1_OID, sizeof(SECP384R1_OID)))
      return TLS_EC_CURVE_SECP384R1;
#endif
#if (TLS_SECP521R1_SUPPORT)
   //secp521r1 elliptic curve?
   else if(!oidComp(oid, length, SECP521R1_OID, sizeof(SECP521R1_OID)))
      return TLS_EC_CURVE_SECP521R1;
#endif
#if (TLS_BRAINPOOLP256R1_SUPPORT)
   //brainpoolP256r1 elliptic curve?
   else if(!oidComp(oid, length, BRAINPOOLP256R1_OID, sizeof(BRAINPOOLP256R1_OID)))
      return TLS_EC_CURVE_BRAINPOOLP256R1;
#endif
#if (TLS_BRAINPOOLP384R1_SUPPORT)
   //brainpoolP384r1 elliptic curve?
   else if(!oidComp(oid, length, BRAINPOOLP384R1_OID, sizeof(BRAINPOOLP384R1_OID)))
      return TLS_EC_CURVE_BRAINPOOLP384R1;
#endif
#if (TLS_BRAINPOOLP512R1_SUPPORT)
   //brainpoolP512r1 elliptic curve?
   else if(!oidComp(oid, length, BRAINPOOLP512R1_OID, sizeof(BRAINPOOLP512R1_OID)))
      return TLS_EC_CURVE_BRAINPOOLP512R1;
#endif
   //Unknown identifier?
   else
      return TLS_EC_CURVE_NONE;
#else
   //ECC not supported
   return TLS_EC_CURVE_NONE;
#endif
}

/**
 * @brief Retrieve the certificate type
 * @param[in] certInfo X.509 certificate
 * @param[out] certType Certificate type
 * @param[out] certSignAlgo Signature algorithm that has been used to sign the certificate
 * @param[out] certHashAlgo Hash algorithm that has been used to sign the certificate
 * @param[out] namedCurve Elliptic curve (only for ECDSA certificates)
 * @return Error code
 **/

RES_CODE tlsGetCertificateType(const X509CertificateInfo *certInfo, TlsCertificateType *certType,
		tls_sign_algo_t *certSignAlgo, tls_hash_algo_t *certHashAlgo, tls_ecnamedcurve_t *namedCurve)
{
   //Check parameters
   if(certInfo == nullptr || certType == nullptr || certSignAlgo == nullptr ||
      certHashAlgo == nullptr || namedCurve == nullptr)
   {
      //Report an error
      return RES_TLS_INVALID_PARAMETER;
   }

#if (TLS_RSA_SIGN_SUPPORT  || TLS_RSA_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT)
   //A valid RSA public key has been found?
   if(!oidComp(certInfo->subjectPublicKeyInfo.oid, certInfo->subjectPublicKeyInfo.oidLen,
      RSA_ENCRYPTION_OID, sizeof(RSA_ENCRYPTION_OID)))
   {
      //Save certificate type
      *certType = TLS_CERT_RSA_SIGN;
      //Elliptic curve cryptography is not used
      *namedCurve = TLS_EC_CURVE_NONE;
   }
   else
#endif
#if (TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT)
   //A valid DSA public key has been found?
   if(!oidComp(certInfo->subjectPublicKeyInfo.oid, certInfo->subjectPublicKeyInfo.oidLen,
      DSA_OID, sizeof(DSA_OID)))
   {
      //Save certificate type
      *certType = TLS_CERT_DSS_SIGN;
      //Elliptic curve cryptography is not used
      *namedCurve = TLS_EC_CURVE_NONE;
   }
   else
#endif
#if (TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT)
   //A valid EC public key has been found?
   if(!oidComp(certInfo->subjectPublicKeyInfo.oid, certInfo->subjectPublicKeyInfo.oidLen,
      EC_PUBLIC_KEY_OID, sizeof(EC_PUBLIC_KEY_OID)))
   {
      //Save certificate type
      *certType = TLS_CERT_ECDSA_SIGN;

      //Retrieve the named curve that has been used to generate the EC public key
      *namedCurve = tlsGetNamedCurve(certInfo->subjectPublicKeyInfo.ecParams.namedCurve,
         certInfo->subjectPublicKeyInfo.ecParams.namedCurveLen);
   }
   else
#endif
   //The certificate does not contain any valid public key...
   {
      //Report an error
      return RES_TLS_BAD_CERTIFICATE;
   }

   //Retrieve the signature algorithm that has been used to sign the certificate
   if(certInfo->signatureAlgo == nullptr)
   {
      //Invalid certificate
      return RES_TLS_BAD_CERTIFICATE;
   }
#if (TLS_RSA_SIGN_SUPPORT)
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      MD5_WITH_RSA_ENCRYPTION_OID, sizeof(MD5_WITH_RSA_ENCRYPTION_OID)))
   {
      //MD5 with RSA signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_RSA;
      *certHashAlgo = TLS_HASH_ALGO_MD5;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      SHA1_WITH_RSA_ENCRYPTION_OID, sizeof(SHA1_WITH_RSA_ENCRYPTION_OID)))
   {
      //SHA-1 with RSA signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_RSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA1;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      SHA256_WITH_RSA_ENCRYPTION_OID, sizeof(SHA256_WITH_RSA_ENCRYPTION_OID)))
   {
      //SHA-256 with RSA signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_RSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA256;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      SHA384_WITH_RSA_ENCRYPTION_OID, sizeof(SHA384_WITH_RSA_ENCRYPTION_OID)))
   {
      //SHA-384 with RSA signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_RSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA384;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      SHA512_WITH_RSA_ENCRYPTION_OID, sizeof(SHA512_WITH_RSA_ENCRYPTION_OID)))
   {
      //SHA-512 with RSA signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_RSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA512;
   }
#endif
#if (TLS_DSA_SIGN_SUPPORT)
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      DSA_WITH_SHA1_OID, sizeof(DSA_WITH_SHA1_OID)))
   {
      //DSA with SHA-1 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_DSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA1;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      DSA_WITH_SHA224_OID, sizeof(DSA_WITH_SHA224_OID)))
   {
      //DSA with SHA-224 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_DSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA224;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      DSA_WITH_SHA256_OID, sizeof(DSA_WITH_SHA256_OID)))
   {
      //DSA with SHA-256 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_DSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA256;
   }
#endif
#if (TLS_ECDSA_SIGN_SUPPORT)
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      ECDSA_WITH_SHA1_OID, sizeof(ECDSA_WITH_SHA1_OID)))
   {
      //ECDSA with SHA-1 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_ECDSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA1;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      ECDSA_WITH_SHA224_OID, sizeof(ECDSA_WITH_SHA224_OID)))
   {
      //ECDSA with SHA-224 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_ECDSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA224;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      ECDSA_WITH_SHA256_OID, sizeof(ECDSA_WITH_SHA256_OID)))
   {
      //ECDSA with SHA-256 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_ECDSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA256;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      ECDSA_WITH_SHA384_OID, sizeof(ECDSA_WITH_SHA384_OID)))
   {
      //ECDSA with SHA-384 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_ECDSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA384;
   }
   else if(!oidComp(certInfo->signatureAlgo, certInfo->signatureAlgoLen,
      ECDSA_WITH_SHA512_OID, sizeof(ECDSA_WITH_SHA512_OID)))
   {
      //ECDSA with SHA-512 signature algorithm
      *certSignAlgo = TLS_SIGN_ALGO_ECDSA;
      *certHashAlgo = TLS_HASH_ALGO_SHA512;
   }
#endif
   else
   {
      //The signature algorithm is not supported...
      return RES_TLS_BAD_CERTIFICATE;
   }

   //X.509 certificate successfully parsed
   return RES_OK;
}

/**
 * @brief Find the specified extension
 * @param[in] data Pointer to the list of extensions
 * @param[in] length Length in bytes of the list
 * @param[in] type Expected extension type
 * @return If the specified extension was found, a pointer to the corresponding
 *   extension is returned. Otherwise nullptr pointer is returned
 **/
const tls_extension_t* tlsGetExtension(const uint8_t* data, size_t length, uint16_t type)
{
	size_t n;
	const tls_extension_t *extension;

	//Sanity check
	if (length < 2)
		return nullptr;

	//Retrieve the length of the list
	n = __REV16(*(const uint16_t*)(void*)data);

	//Sanity check
	if (length < (n + 2))
		return nullptr;

	//Point to the first extension of the list
	data += 2;

	//Parse the list of extensions offered by the peer
	while (n >= sizeof(tls_extension_t))
	{
		//Point to the current extension
		extension = (tls_extension_t *) data;

		//Check the length of the extension
		length = sizeof(tls_extension_t) + __REV16(extension->length);
		if (n < length)
			break;

		//Check whether the extension type matches the specified one
		if (extension->type == type)
			return extension;

		//Jump to the next extension
		data += length;
		//Remaining bytes to process
		n -= length;
	}

	//The specified extension type was not found
	return nullptr;
}
