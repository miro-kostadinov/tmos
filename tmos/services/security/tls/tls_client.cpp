/*
 * tls_client.cpp
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <tls.h>
#include <tls_misc.h>
#include <tls_cipher_suites.h>
#include <hmac.h>
#include <pem.h>

RES_CODE tls_context_t::tls_client_hello_msg_len(record_ctxt_t* rc)
{
	uint32_t len, ext_len;
	uint32_t i;
	bool use_ecc = false;

	// 1. hello message header
	len = sizeof(tls_client_hello_t);					// max 0 + 37 = 37

	// 2. Session ID
	len += sessionIdLen;									// max 37 + 32 = 69

	// 3. Cipher suites
	if (num_ciphers)
	{
		//User preferred cipher suite?
		for (i = num_ciphers; i-- > 0 ; )
		{
			const tls_chipher_suite_info*  suite;

			suite = get_cipher_suite_info(cipher_suites[i]);
			if (suite)
			{
				len += 2;

				if(!use_ecc)
					use_ecc = is_ecc_cipher_suite(suite);
			}
		}
	} else
	{
		// Default cipher suite
		ext_len = tlsGetNumSupportedCipherSuites();
		len += 2 * ext_len;					// max 69 + 400 = 469
		//Parse cipher suites

		for (i = 0; i < ext_len; i++)
		{
			//ECC cipher suite?
			if (is_ecc_cipher_suite(&tls_supported_cipher_suites[i]))
			{
				use_ecc = true;
				break;
			}
		}
	}
	len += sizeof(tls_cipher_suites_t);						// max 469 + 2 = 471

	// 4. Compression methods
	len += sizeof(tls_compression_methods_t) + 1;			// max +2 = 473


	// 5. Various extensions
	ext_len = 0;

	// 5.1 Server Name Indication extension (RFC 3546)
#if TLS_SNI_SUPPORT
	if (!server_name.empty())
	{
		ext_len += sizeof(tls_extension_t) + sizeof(tls_servername_list_t);
		ext_len += sizeof(tls_server_name_t) + server_name.length();
	}
#endif

	// 5.2 Application-Layer Protocol Negotiation Extension (RFC 7301)
#if TLS_ALPN_SUPPORT
	if (!protocol_list.empty())
	{
		const char* str = protocol_list.c_str();
		uint32_t j=0, proto_len=0;

		i = 0;
		do
		{
			if (str[i] == ','|| str[i] == '\0')
			{
				if ( i != j)
					proto_len += sizeof(tls_protocol_name_t) + i - j;

				j = i + 1;
			}
		} while (str[i++]);

		if(proto_len)
		{
			ext_len += sizeof(tls_extension_t) + sizeof(tls_protocol_name_list_t);
			ext_len += proto_len;

		}
	}
#endif // TLS_ALPN_SUPPORT

	// 5.3 Elliptic Curves Extension (RFC 4492)
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
	   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//A client that proposes ECC cipher suites in its ClientHello message
	//should send the EllipticCurves extension
	if (use_ecc)
	{
		i = 2 * (TLS_SECP160K1_SUPPORT + TLS_SECP160R1_SUPPORT
				+ TLS_SECP160R2_SUPPORT	+ TLS_SECP192K1_SUPPORT
				+ TLS_SECP192R1_SUPPORT	+ TLS_SECP224K1_SUPPORT
				+ TLS_SECP224R1_SUPPORT	+ TLS_SECP256K1_SUPPORT
				+ TLS_SECP256R1_SUPPORT	+ TLS_SECP384R1_SUPPORT
				+ TLS_SECP521R1_SUPPORT	+ TLS_BRAINPOOLP256R1_SUPPORT
				+ TLS_BRAINPOOLP384R1_SUPPORT + TLS_BRAINPOOLP512R1_SUPPORT);
		if(i)
			ext_len += sizeof(tls_extension_t) + sizeof(tls_elliptic_curve_list_t) + i;

		// 5.4 EC Point Formats Extension (RFC 4492)
		ext_len += sizeof(tls_extension_t) + sizeof(tls_ecpoint_format_list_t) + 1;
	}
#endif

	// 5.5 Signature Algorithms Extension (RFC 5246)
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	{
		i = 0;

#if TLS_RSA_SIGN_SUPPORT
		i = TLS_MD5_SUPPORT + TLS_SHA1_SUPPORT + TLS_SHA224_SUPPORT
				+ TLS_SHA256_SUPPORT + TLS_SHA384_SUPPORT + TLS_SHA512_SUPPORT;
#endif

#if TLS_DSA_SIGN_SUPPORT
		i += TLS_SHA1_SUPPORT + TLS_SHA224_SUPPORT + TLS_SHA256_SUPPORT;
#endif

#if TLS_ECDSA_SIGN_SUPPORT
		//Any ECC cipher suite proposed by the client?
		if (use_ecc)
		{
			i += TLS_SHA1_SUPPORT + TLS_SHA224_SUPPORT + TLS_SHA256_SUPPORT
					+ TLS_SHA384_SUPPORT + TLS_SHA512_SUPPORT;
		}
#endif

		if(i)
		{
			ext_len += sizeof(tls_extension_t) + sizeof(tls_sign_algos_t);
			ext_len +=  sizeof(tls_signature_algo_t) * i;
		}
	}
#endif

	// Finally, sum all extensions
	if (ext_len)
	{
		len += sizeof(tls_extensions_t) + ext_len;
	}

	rc->msg_len = len;
	return RES_OK;
}

void tls_context_t::tls_client_make_hello(tls_client_hello_t* message, uint32_t len)
{
	bool use_ecc = false;
	uint8_t *p;
	uint32_t i, n;
	const tls_chipher_suite_info* suite;
	tls_extensions_t* extensions;
	tls_extension_t* extension;

	// 1. hello message header
	message->header.set(TLS_TYPE_CLIENT_HELLO, len - sizeof(tls_handshake_t));
	message->clientVersion = (tls_version_t)__REV16(TLS_MAX_VERSION);
	message->random = tls_randoms.client_random;

	// 2. Session ID
#if TLS_SESSION_RESUME_SUPPORT
	message->sessionIdLen = sessionIdLen;
	memcpy(message->sessionId, sessionId, sessionIdLen);
	p = message->sessionId + message->sessionIdLen;
#else
	//Session resumption is not supported
	message->sessionIdLen = 0;
	p = message->sessionId;
#endif


	// 3. Cipher suites
	if (num_ciphers)
	{
		// User preferred cipher suite?
		n = 0;

		for (i = 0; i < num_ciphers; i++)
		{
			suite = get_cipher_suite_info(cipher_suites[i]);
			if (suite)
			{
				static_cast<tls_cipher_suites_t*>((void*) p)->value[n++] =
						(tls_suite_id_t) __REV16(cipher_suites[i]);
				if(!use_ecc)
					use_ecc = is_ecc_cipher_suite(suite);
			}
		}
	}else
	{
		//Default cipher suite
		n = tlsGetNumSupportedCipherSuites();
		suite = tls_supported_cipher_suites;
		for (i = 0; i < n; i++, suite++)
		{
			static_cast<tls_cipher_suites_t*>((void*)p)->value[i] =
					(tls_suite_id_t) __REV16(suite->suite_identifier);
			if(!use_ecc)
				use_ecc = is_ecc_cipher_suite(suite);
		}
	}
	static_cast<tls_cipher_suites_t*>((void*)p)->length = __REV16(n * 2);
	p += sizeof(tls_cipher_suites_t) + n * 2;

	// 4. Compression methods
	static_cast<tls_compression_methods_t*>((void*)p)->length = 1;
	static_cast<tls_compression_methods_t*>((void*)p)->value[0] = TLS_COMPRESSION_METHOD_NULL;
	p += sizeof(tls_compression_methods_t) + 1;

	// 5. Various extensions
	extensions = (tls_extensions_t*)p;
	extensions->length = 0;
	p += sizeof(tls_extensions_t);

	// 5.1 Server Name Indication extension (RFC 3546)
#if TLS_SNI_SUPPORT
	if (!server_name.empty())
	{
		tls_servername_list_t* server_list;

		extension = (tls_extension_t*)p;
		server_list = (tls_servername_list_t*)extension->value;
		n = server_name.length();

		extension->type = TLS_EXT_SERVER_NAME;
		extension->length = __REV16(sizeof(tls_servername_list_t) + sizeof(tls_server_name_t) + n);

		server_list->length = __REV16(sizeof(tls_server_name_t) + n);
		server_list->value[0].type = TLS_NAME_TYPE_HOSTNAME;
		server_list->value[0].length = __REV16(n);
		memcpy(server_list->value[0].hostname, server_name.c_str(), n);

		n += sizeof(tls_extension_t) + sizeof(tls_servername_list_t) + sizeof(tls_server_name_t);
		extensions->length += n;
		p += n;
	}
#endif

	// 5.2 Application-Layer Protocol Negotiation Extension (RFC 7301)
#if TLS_ALPN_SUPPORT
	if (!protocol_list.empty())
	{
		const char* str = protocol_list.c_str();
		uint32_t j=0;
		tls_protocol_name_list_t* proto_list;

		i = 0;
		n = 0;
		extension = (tls_extension_t*)p;
		proto_list = (tls_protocol_name_list_t*)extension->value;
		do
		{
			if (str[i] == ','|| str[i] == '\0')
			{
				if ( i != j)
				{
					proto_list[n].length = i - j;
					memcpy(proto_list[n].value, str + j, i - j);
					n += sizeof(tls_server_name_t) + i -j;
				}

				j = i + 1;
			}
		} while (str[i++]);

		if(n)
		{
			proto_list->length = __REV16(n);
			n += sizeof(tls_protocol_name_list_t);

			extension->type = TLS_EXT_ALPN;
			extension->length = __REV16(n);

			n += sizeof(tls_extension_t);
			extensions->length += n ;
			p += n;
		}
	}
#endif // TLS_ALPN_SUPPORT

	// 5.3 Elliptic Curves Extension (RFC 4492)
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
	   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//A client that proposes ECC cipher suites in its ClientHello message
	//should send the EllipticCurves extension
	if (use_ecc)
	{
		n = 2* (TLS_SECP160K1_SUPPORT + TLS_SECP160R1_SUPPORT
				+ TLS_SECP160R2_SUPPORT	+ TLS_SECP192K1_SUPPORT
				+ TLS_SECP192R1_SUPPORT	+ TLS_SECP224K1_SUPPORT
				+ TLS_SECP224R1_SUPPORT	+ TLS_SECP256K1_SUPPORT
				+ TLS_SECP256R1_SUPPORT	+ TLS_SECP384R1_SUPPORT
				+ TLS_SECP521R1_SUPPORT	+ TLS_BRAINPOOLP256R1_SUPPORT
				+ TLS_BRAINPOOLP384R1_SUPPORT + TLS_BRAINPOOLP512R1_SUPPORT);
		if(n)
		{
			tls_elliptic_curve_list_t* curve_list;

			extension = (tls_extension_t*)p;
			curve_list = (tls_elliptic_curve_list_t*) extension->value;
			i = 0;

#if TLS_SECP160K1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP160K1;
#endif
#if TLS_SECP160R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP160R1;
#endif
#if TLS_SECP160R2_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP160R2;
#endif
#if TLS_SECP192K1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP192K1;
#endif
#if TLS_SECP192R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP192R1;
#endif
#if TLS_SECP224K1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP224K1;
#endif
#if TLS_SECP224R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP224R1;
#endif
#if TLS_SECP256K1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP256K1;
#endif
#if TLS_SECP256R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP256R1;
#endif
#if TLS_SECP384R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP384R1;
#endif
#if TLS_SECP521R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_SECP521R1;
#endif
#if TLS_BRAINPOOLP256R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_BRAINPOOLP256R1;
#endif
#if TLS_BRAINPOOLP384R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_BRAINPOOLP384R1;
#endif
#if TLS_BRAINPOOLP512R1_SUPPORT
			curve_list->value[i++] = TLS_EC_CURVE_BRAINPOOLP512R1;
#endif

			curve_list->length = __REV16(n);
			n += sizeof(tls_elliptic_curve_list_t);

			extension->type = TLS_EXT_ELLIPTIC_CURVES;
			extension->length = __REV16( n );
			n += sizeof(tls_extension_t);

			extensions->length += n ;
			p += n;
		}

		// 5.4 EC Point Formats Extension (RFC 4492)
		extension = (tls_extension_t*)p;
		extension->type = TLS_EXT_EC_POINT_FORMATS;
		extension->length = __REV16( sizeof(tls_ecpoint_format_list_t) + 1);
		static_cast<tls_ecpoint_format_list_t*>((void*)extension->value)->length = 1;
		static_cast<tls_ecpoint_format_list_t*>((void*)extension->value)->value[0] =
				TLS_EC_POINT_FORMAT_UNCOMPRESSED;

		extensions->length += sizeof(tls_extension_t) + sizeof(tls_ecpoint_format_list_t) + 1;
		p += sizeof(tls_extension_t) + sizeof(tls_ecpoint_format_list_t) + 1;
	}

#endif

	// 5.5 Signature Algorithms Extension (RFC 5246)
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	{
		tls_sign_algos_t* sign_algos;

		extension = (tls_extension_t*)p;
		sign_algos = (tls_sign_algos_t*) extension->value;
		n = 0;

#if TLS_RSA_SIGN_SUPPORT
#if TLS_MD5_SUPPORT
		//MD5 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_MD5;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#if TLS_SHA1_SUPPORT
		//SHA-1 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA1;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#if TLS_SHA224_SUPPORT
		//SHA-224 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA224;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#if TLS_SHA256_SUPPORT
		//SHA-256 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA256;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#if TLS_SHA384_SUPPORT
		//SHA-384 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA384;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#if TLS_SHA512_SUPPORT
		//SHA-512 with RSA support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA512;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_RSA;
#endif
#endif

#if TLS_DSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//DSA with SHA-1 support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA1;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_DSA;
#endif
#if TLS_SHA224_SUPPORT
		//DSA with SHA-224 support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA224;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_DSA;
#endif
#if TLS_SHA256_SUPPORT
		//DSA with SHA-256 support
		sign_algos->value[n].hash = TLS_HASH_ALGO_SHA256;
		sign_algos->value[n++].signature = TLS_SIGN_ALGO_DSA;
#endif
#endif

#if TLS_ECDSA_SIGN_SUPPORT
		//Any ECC cipher suite proposed by the client?
		if(use_ecc)
		{
#if TLS_SHA1_SUPPORT
			//ECDSA with SHA-1 support
			sign_algos->value[n].hash = TLS_HASH_ALGO_SHA1;
			sign_algos->value[n++].signature = TLS_SIGN_ALGO_ECDSA;
#endif
#if TLS_SHA224_SUPPORT
			//ECDSA with SHA-224 support
			sign_algos->value[n].hash = TLS_HASH_ALGO_SHA224;
			sign_algos->value[n++].signature = TLS_SIGN_ALGO_ECDSA;
#endif
#if TLS_SHA256_SUPPORT
			//ECDSA with SHA-256 support
			sign_algos->value[n].hash = TLS_HASH_ALGO_SHA256;
			sign_algos->value[n++].signature = TLS_SIGN_ALGO_ECDSA;
#endif
#if TLS_SHA384_SUPPORT
			//ECDSA with SHA-384 support
			sign_algos->value[n].hash = TLS_HASH_ALGO_SHA384;
			sign_algos->value[n++].signature = TLS_SIGN_ALGO_ECDSA;
#endif
#if TLS_SHA512_SUPPORT
			//ECDSA with SHA-512 support
			sign_algos->value[n].hash = TLS_HASH_ALGO_SHA512;
			sign_algos->value[n++].signature = TLS_SIGN_ALGO_ECDSA;
#endif
		}
#endif

		if(n)
		{
			n *= sizeof(tls_signature_algo_t);
			sign_algos->length = __REV16(n);
			n += sizeof(tls_sign_algos_t);

			extension->type = TLS_EXT_SIGNATURE_ALGORITHMS;
			extension->length = __REV16(n);
			n += sizeof(tls_extension_t);

			extensions->length += n ;
			p += n;
		}
	}
#endif


	// Finally, sum all extensions
	extensions->length = __REV16(extensions->length);
}


RES_CODE tls_context_t::tls_client_send_hello()
{
	RES_CODE res;

    TRACELN1_TLS("TLS send hello");
	// Generate the client random value
	res = tls_randoms.tls_random_generate(tls_randoms.client_random);

	// figure out required message/record size
	if (res == RES_OK)
	{
		res = tls_client_hello_msg_len(&last_txrc);
	    TRACELN_TLS("TLS hello res=%x len=%u", res, last_txrc.msg_len);
		if (res == RES_OK)
			res = tls_record_get_lens(&last_txrc);
	}

	// Prepare record
	if(res == RES_OK)
	{
		tls_client_hello_t* message;

		last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
		message = (tls_client_hello_t*)last_txrc.msg_start();
		tls_client_make_hello(message, last_txrc.msg_len);
		tls_handshake_hash_update(message, last_txrc.msg_len);

		res = tls_record_write(&last_txrc);

		if (res == RES_OK)
		{
			//Prepare to receive ServerHello message...
			tls_state = TLS_STATE_SERVER_HELLO;
		}

	}
    TRACELN_TLS("TLS send hello res %u", res);
	return res;

}

RES_CODE tls_context_t::tls_clent_handshake()
{
	RES_CODE res = RES_TLS_HANDSHAKE_FAILED;
	record_ctxt_t rc;

	if(tls_state != TLS_STATE_INIT)
		return res;


	tls_state = TLS_STATE_CLIENT_HELLO;
    TRACELN1_TLS("TLS handshake");

	while (tls_state != TLS_STATE_APPLICATION_DATA)
	{
		//RFC 5246 TLS handshake state machine
		switch (tls_state)
		{

		// When a client first connects to a server, it is required to send
	    // the ClientHello as its first message
		case TLS_STATE_CLIENT_HELLO:
			res = tls_client_send_hello();
			break;

			//Send Certificate message?
		case TLS_STATE_CLIENT_CERTIFICATE:
			//This is the first message the client can send after receiving a
			//ServerHelloDone message. This message is only sent if the server
			//requests a certificate
			res = tlsSendCertificate();
			break;
			//Send ClientKeyExchange message?
		case TLS_STATE_CLIENT_KEY_EXCHANGE:
			//This message is always sent by the client. It must immediately
			//follow the client certificate message, if it is sent. Otherwise,
			//it must be the first message sent by the client after it receives
			//the ServerHelloDone message
			res = tlsSendClientKeyExchange();
			break;
			//Send CertificateVerify message?
		case TLS_STATE_CERTIFICATE_VERIFY:
			//This message is used to provide explicit verification of a client
			//certificate. This message is only sent following a client certificate
			//that has signing capability. When sent, it must immediately follow
			//the clientKeyExchange message
			res = tlsSendCertificateVerify();
			break;
			//Send ChangeCipherSpec message?
		case TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC:
			//The ChangeCipherSpec message is sent by the client and to notify the
			//server that subsequent records will be protected under the newly
			//negotiated CipherSpec and keys
			res = tlsSendChangeCipherSpec();
			break;
			//Send Finished message?
		case TLS_STATE_CLIENT_FINISHED:
			//A Finished message is always sent immediately after a changeCipherSpec
			//message to verify that the key exchange and authentication processes
			//were successful
			res = tlsSendFinished();
			break;
			//Wait for a message from the server?
		case TLS_STATE_SERVER_HELLO:
		case TLS_STATE_SERVER_CERTIFICATE:
		case TLS_STATE_SERVER_KEY_EXCHANGE:
		case TLS_STATE_CERTIFICATE_REQUEST:
		case TLS_STATE_SERVER_HELLO_DONE:
		case TLS_STATE_SERVER_CHANGE_CIPHER_SPEC:
		case TLS_STATE_SERVER_FINISHED:
            //Parse incoming handshake message
			res = tls_parse_server_message(rc);
            break;
         //Sending Alert message?
         case TLS_STATE_CLOSING:
            //Mark the TLS connection as closed
            tls_state = TLS_STATE_CLOSED;
            break;
         //TLS connection closed?
         case TLS_STATE_CLOSED:
            //Debug message
            TRACELN1_TLS("TLS handshake failure!");
            //Report an error
            res = RES_TLS_HANDSHAKE_FAILED;
            break;
         //Invalid state?
         default:
            //Report an error
            res = RES_TLS_UNEXPECTED_STATE;
            break;

		}

		//Abort TLS handshake if an error was encountered
		if (res != RES_OK)
		{
			//Send an alert message to the server
			tlsProcessError(res);
			//Exit immediately
			break;
		}
	}

	//Return status code
	return res;

}


/**
 * @brief Parse PSK identity hint
 * @param[in] context Pointer to the TLS context
 * @param[in] p Input stream where to read the PSK identity hint
 * @param[in] length Number of bytes available in the input stream
 * @param[out] consumed Total number of bytes that have been consumed
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParsePskIdentityHint(const uint8_t* p, size_t length, size_t* consumed)
{
	size_t n;
	TlsPskIdentityHint *pskIdentityHint;

	//Malformed ServerKeyExchange message?
	if (length < sizeof(TlsPskIdentityHint))
		return RES_TLS_DECODING_FAILED;

	//Point to the PSK identity hint
	pskIdentityHint = (TlsPskIdentityHint *) p;

	//Retrieve the length of the PSK identity hint
	n = __REV16(pskIdentityHint->length);

	//Make sure the length field is valid
	if (length < (sizeof(TlsPskIdentityHint) + n))
		return RES_TLS_DECODING_FAILED;

#if TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT

	//The PSK identity hint is optional
	psk_identity_hint.clear();
	if (n > 0)
	{
		//Save the PSK identity hint
		psk_identity_hint.assign(pskIdentityHint->value, n);
		if (psk_identity_hint.empty())
			return RES_OUT_OF_MEMORY;
	}
#endif

	//Total number of bytes that have been consumed
	*consumed = sizeof(TlsPskIdentityHint) + n;

	//Successful processing
	return RES_OK;
}

/**
 * @brief Parse server's key exchange parameters
 * @param[in] context Pointer to the TLS context
 * @param[in] p Input stream where to read the server's key exchange parameters
 * @param[in] length Number of bytes available in the input stream
 * @param[out] consumed Total number of bytes that have been consumed
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseServerKeyParams(const uint8_t* p, size_t length, size_t* consumed)
{
	RES_CODE res= RES_OK;
	const uint8_t *params;


	//Point to the server's key exchange parameters
	params = p;

#if (TLS_DH_ANON_SUPPORT || TLS_DHE_RSA_SUPPORT || \
	   TLS_DHE_DSS_SUPPORT || TLS_DHE_PSK_SUPPORT)
	//Diffie-Hellman key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK)
	{
		uint32_t k;
		size_t n;

		//Convert the prime modulus to a multiple precision integer
		res = dhContext.params.p.tlsReadMpi(p, length, &n);

		if (res == RES_OK)
		{
			//Get the length of the prime modulus, in bits
			k = dhContext.params.p.mpiGetBitLength();

			//Make sure the prime modulus is acceptable
			if (k < TLS_MIN_DH_MODULUS_SIZE || k > TLS_MAX_DH_MODULUS_SIZE)
				res = RES_TLS_ILLEGAL_PARAMETER;
		}

		if (res == RES_OK)
		{
			//Advance data pointer
			p += n;
			//Remaining bytes to process
			length -= n;

			//Convert the generator to a multiple precision integer
			res = dhContext.params.g.tlsReadMpi(p, length, &n);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Advance data pointer
			p += n;
			//Remaining bytes to process
			length -= n;

			//Convert the server's public value to a multiple precision integer
			res = dhContext.yb.tlsReadMpi(p, length, &n);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Advance data pointer
			p += n;
			//Remaining bytes to process
			length -= n;

			//Verify peer's public value
			res = dhContext.params.dhCheckPublicKey(&dhContext.yb);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Debug message
			TRACELN_TLS("Diffie-Hellman parameters:");
//			TRACELN_TLS("  Prime modulus: ");
//			TRACE_MPI("    ", &dhContext.params.p);
//			TRACE_TLS("  Generator:\r\n");
//			TRACE_MPI("    ", &dhContext.params.g);
//			TRACE_TLS("  Server public value:\r\n");
//			TRACE_MPI("    ", &dhContext.yb);
		}
	}
	else
#endif
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
	   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//ECDH key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		size_t n;
		uint8_t curveType;
		tls_ecnamedcurve_t namedCurve;
		const EcCurveInfo *curveInfo=nullptr;

		//Malformed ServerKeyExchange message?
		if (length < sizeof(curveType))
			res = RES_TLS_DECODING_FAILED;

		//Check status code
		if (res == RES_OK)
		{
			//Retrieve the type of the elliptic curve domain parameters
			curveType = *p;

			//Advance data pointer
			p += sizeof(curveType);
			//Remaining bytes to process
			length -= sizeof(curveType);

			//Only named curves are supported
			if (curveType != TLS_EC_CURVE_TYPE_NAMED_CURVE)
				res = RES_TLS_ILLEGAL_PARAMETER;
		}

		//Check status code
		if (res == RES_OK)
		{
			//Malformed ServerKeyExchange message?
			if (length < sizeof(namedCurve))
				res = RES_TLS_DECODING_FAILED;
		}

		//Check status code
		if (res == RES_OK)
		{
			//Get elliptic curve identifier
			namedCurve = (tls_ecnamedcurve_t)((p[1] << 8) + p[0]);

			//Advance data pointer
			p += sizeof(namedCurve);
			//Remaining bytes to process
			length -= sizeof(namedCurve);

			//Retrieve the corresponding EC domain parameters
			curveInfo = tlsGetCurveInfo(namedCurve);

			//Make sure the elliptic curve is supported
			if (curveInfo == nullptr)
				res = RES_TLS_ILLEGAL_PARAMETER;
		}

		//Check status code
		if (res == RES_OK)
		{
			//Load EC domain parameters
			res = ecdhContext.params.ecLoadDomainParameters(curveInfo);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Read server's public key
			res = ecdhContext.tlsReadEcPoint(p, length, &n);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Advance data pointer
			p += n;
			//Remaining bytes to process
			length -= n;

			//Verify peer's public key
			res = ecdhContext.ecdhCheckPublicKey();
		}

		//Check status code
/*
		if (res == RES_OK)
		{
			//Debug message
			TRACE_TLS("  Server public key X:\r\n");
			TRACE_MPI("    ", &ecdhContext.qb.x);
			TRACE_TLS("  Server public key Y:\r\n");
			TRACE_MPI("    ", &ecdhContext.qb.y);
		}
*/
	}
	else
#endif
	//Invalid key exchange method?
	{
		//It is not legal to send the ServerKeyExchange message when a key
		//exchange method other than DHE_DSS, DHE_RSA, DH_anon, ECDHE_RSA,
		//ECDHE_ECDSA or ECDH_anon is selected
		res = RES_TLS_UNEXPECTED_MESSAGE;
	}

	//Total number of bytes that have been consumed
	*consumed = p - params;

	return res;
}

/**
 * @brief Verify signature over the server's key exchange parameters
 * @param[in] context Pointer to the TLS context
 * @param[in] p Input stream where to read the signature
 * @param[in] length Number of bytes available in the input stream
 * @param[in] params Pointer to the server's key exchange parameters
 * @param[in] paramsLen Length of the server's key exchange parameters
 * @param[out] consumed Total number of bytes that have been consumed
 * @return Error code
 **/
RES_CODE tls_context_t::tlsVerifyServerKeySignature(const uint8_t* p,
		size_t length, const uint8_t* params, size_t paramsLen,
		size_t* consumed)
{
	RES_CODE res = RES_OK;


#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= TLS_VERSION_1_1)
	//SSL 3.0, TLS 1.0 or TLS 1.1 currently selected?
	if (tls_version <= TLS_VER11)
	{
		TlsDigitalSignature *signature;

		//Point to the digitally-signed element
		signature = (TlsDigitalSignature *) p;

		//Check the length of the digitally-signed element
		if (length < sizeof(TlsDigitalSignature))
			return RES_TLS_DECODING_FAILED;
		if (length < (sizeof(TlsDigitalSignature) + __REV16(signature->length)))
			return RES_TLS_DECODING_FAILED;

#if TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
		//Check whether DHE_RSA or ECDHE_RSA key exchange method is currently used
		if(cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA ||
				cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA)
		{
			hash_algo_t* algo;

			//Allocate a memory buffer to hold the MD5 context
			algo = new_md5_hash();

			//Successful memory allocation?
			if(algo != nullptr)
			{
				//Compute MD5(ClientHello.random + ServerHello.random + ServerDhParams)
				algo->Input(&tls_randoms, 64);
				algo->Input(params, paramsLen);
				algo->Result(verifyData);

				//Release previously allocated memory
				delete algo;
			}
			else
			{
				//Failed to allocate memory
				res = RES_OUT_OF_MEMORY;
			}

			//Check status code
			if(res == RES_OK)
			{
				//Allocate a memory buffer to hold the SHA-1 context
				algo = new_sha1_hash();

				//Successful memory allocation?
				if(algo != nullptr)
				{
					//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
					algo->Input(&tls_randoms, 64);
					algo->Input(params, paramsLen);
					algo->Result(verifyData + MD5_DIGEST_SIZE);

					//Release previously allocated memory
					delete algo;
				}
				else
				{
					//Failed to allocate memory
					res = RES_OUT_OF_MEMORY;
				}
			}

			//Check status code
			if(res == RES_OK)
			{
				//RSA signature verification
				res = tlsVerifyRsaSignature(&peerRsaPublicKey, verifyData, signature);
			}
		}
		else
#endif
#if TLS_DHE_DSS_SUPPORT
		//Check whether DHE_DSS key exchange method is currently used
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS)
		{
			hash_algo_t* sha1Context;

			//Allocate a memory buffer to hold the SHA-1 context
			sha1Context = new_sha1_hash();

			//Successful memory allocation?
			if (sha1Context != nullptr)
			{
				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				sha1Context->Input(&tls_randoms, 64);
				sha1Context->Input(params, paramsLen);
				sha1Context->Result(verifyData);

				//Release previously allocated memory
				delete  sha1Context;
			}
			else
			{
				//Failed to allocate memory
				res = RES_OUT_OF_MEMORY;
			}

			//Check status code
			if (res == RES_OK)
			{
				//DSA signature verification
				res = tlsVerifyDsaSignature(&peerDsaPublicKey, verifyData,
				SHA1_DIGEST_SIZE, signature);
			}
		}
		else
#endif
#if TLS_ECDHE_ECDSA_SUPPORT
		//Check whether ECDHE_ECDSA key exchange method is currently used
		if(cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
		{
			hash_algo_t* sha1Context;

			//Allocate a memory buffer to hold the SHA-1 context
			sha1Context = new_sha1_hash();

			//Successful memory allocation?
			if(sha1Context != nullptr)
			{
				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				sha1Context->Input(&tls_randoms, 64);
				sha1Context->Input(params, paramsLen);
				sha1Context->Result(verifyData);

				delete  sha1Context;
			}

			//Check status code
			if(res == RES_OK)
			{
				//ECDSA signature verification
				res = tlsVerifyEcdsaSignature(verifyData, SHA1_DIGEST_SIZE, signature);
			}
		}
		else
#endif
		//Invalid signature algorithm?
		{
			//Report an error
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}

		//Total number of bytes that have been consumed
		*consumed = sizeof(TlsDigitalSignature) + __REV16(signature->length);
	}
	else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.2 currently selected?
	if (tls_version == TLS_VER12)
	{
		TlsDigitalSignature2* signature;
		auto_ptr<hash_algo_t> hashAlgo;
		const hash_info_t* hash_info;

		//Point to the digitally-signed element
		signature = (TlsDigitalSignature2 *) p;

		//Check the length of the digitally-signed element
		if (length < sizeof(TlsDigitalSignature2))
			return RES_TLS_DECODING_FAILED;
		if (length < (sizeof(TlsDigitalSignature2) + __REV16(signature->signature.length)))
			return RES_TLS_DECODING_FAILED;

		//Retrieve the hash algorithm used for signing
		hash_info = tlsGetHashAlgo(signature->algorithm.hash);

		//Make sure the hash algorithm is supported
		if (hash_info != nullptr)
		{
			hashAlgo = hash_info->new_hash();

			//Successful memory allocation?
			if (hashAlgo.get() != nullptr)
			{
				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				hashAlgo->Input(&tls_randoms, 64);
				hashAlgo->Input(params, paramsLen);
				hashAlgo->Result(nullptr);

#if TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
				//Check whether DHE_RSA or ECDHE_RSA key exchange method is currently used
				if((cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA ||
								cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA) &&
						signature->algorithm.signature == TLS_SIGN_ALGO_RSA)
				{
					//Use the signature verification algorithm defined in PKCS #1 v1.5
					res = peerRsaPublicKey.rsassaPkcs1v15Verify(hash_info,
							hashAlgo->digest8(), signature->signature.value,
							__REV16(signature->signature.length));
				}
				else
#endif
#if TLS_DHE_DSS_SUPPORT
				//Check whether DHE_DSS key exchange method is currently used
				if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
						&& signature->algorithm.signature == TLS_SIGN_ALGO_DSA)
				{
					//DSA signature verification
					res = tlsVerifyDsaSignature(&peerDsaPublicKey,
							hashAlgo->digest8(), hash_info->digest_size,
							&signature->signature);
				}
				else
#endif
#if TLS_ECDHE_ECDSA_SUPPORT
				//Check whether DHE_ECDSA key exchange method is currently used
				if(cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA &&
						signature->algorithm.signature == TLS_SIGN_ALGO_ECDSA)
				{
					//ECDSA signature verification
					res = tlsVerifyEcdsaSignature(hashAlgo->digest8(),
							hash_info->digest_size, &signature->signature);
				}
				else
#endif
				//Invalid signature algorithm?
				{
					//Report an error
					res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
				}

			}
			else
			{
				//Failed to allocate memory
				res = RES_OUT_OF_MEMORY;
			}
		}
		else
		{
			//Hash algorithm not supported
			res = RES_TLS_INVALID_SIGNATURE;
		}

		//Total number of bytes that have been consumed
		*consumed = sizeof(TlsDigitalSignature2) + __REV16(signature->signature.length);
	}
	else
#endif
	{
		//The negotiated TLS version is not valid
		res = RES_TLS_INVALID_VERSION;
	}

	//Return status code
	return res;
}

/**
 * @brief Parse ServerKeyExchange message
 *
 * The ServerKeyExchange message is sent by the server only when the
 * server Certificate message does not contain enough data to allow
 * the client to exchange a premaster secret
 *
 * @param[in] context Pointer to the TLS context
 * @param[in] message Incoming ServerKeyExchange message to parse
 * @param[in] length Message length
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseServerKeyExchange(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t n, msg_len;
	size_t paramsLen;
	const uint8_t *p;
	const uint8_t *params=nullptr;
	const tls_server_keyexchange_t* message = (const tls_server_keyexchange_t*)rc->msg_start();;

	//Debug message
	msg_len = rc->msg_len;
	TRACELN_TLS("ServerKeyExchange message received (%u bytes)...", msg_len);
//	TRACE_BUF(message, msg_len);

	//Check the length of the ServerKeyExchange message
	if (msg_len < sizeof(tls_server_keyexchange_t))
		return RES_TLS_DECODING_FAILED;

	//Check current state
	if (tls_state != TLS_STATE_SERVER_KEY_EXCHANGE)
		return RES_TLS_UNEXPECTED_MESSAGE;

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, msg_len);

	//Point to the body of the handshake message
	p = message->data;
	//Remaining bytes to process
	msg_len -= sizeof(tls_server_keyexchange_t);

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//To help the client in selecting which identity to use, the server
		//can provide a PSK identity hint in the ServerKeyExchange message
		res = tlsParsePskIdentityHint(p, msg_len, &n);
		if (res != RES_OK)
			return res;

		//Point to the next field
		p += n;
		//Remaining bytes to process
		msg_len -= n;
	}

	//Diffie-Hellman or ECDH key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//Point to the server's key exchange parameters
		params = p;

		//Parse server's key exchange parameters
		res = tlsParseServerKeyParams(p, msg_len, &paramsLen);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Point to the next field
		p += paramsLen;
		//Remaining bytes to process
		msg_len -= paramsLen;
	}

	//Non-anonymous Diffie-Hellman and ECDH key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
	{
		//For non-anonymous Diffie-Hellman and ECDH key exchanges, the signature
		//over the server's key exchange parameters shall be verified
		res = tlsVerifyServerKeySignature(p, msg_len, params, paramsLen, &n);
		if (res != RES_OK)
			return res;

		//Remaining bytes to process
		msg_len -= n;
	}

	//If the amount of data in the message does not precisely match the format
	//of the ServerKeyExchange message, then send a fatal alert
	if (msg_len != 0)
		return RES_TLS_DECODING_FAILED;

	//Anomynous server?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//An anonymous server cannot request client authentication
		tls_state = TLS_STATE_SERVER_HELLO_DONE;
	}
	else
	{
		//A non-anonymous server can optionally request a certificate from
		//the client, if appropriate for the selected cipher suite
		tls_state = TLS_STATE_CERTIFICATE_REQUEST;
	}

	//Successful processing
	return RES_OK;
}

/**
 * @brief Parse CertificateRequest message
 *
 * A server can optionally request a certificate from the client, if
 * appropriate for the selected cipher suite. This message will
 * immediately follow the ServerKeyExchange message
 *
 * @param[in] context Pointer to the TLS context
 * @param[in] message Incoming CertificateRequest message to parse
 * @param[in] length Message length
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseCertificateRequest(record_ctxt_t* rc)
{
	uint32_t i;
	size_t n, msg_len;
	uint8_t *p;
	bool acceptable;
	tls_sign_algos_t* supportedSignAlgos;
	TlsCertAuthorities *certAuthorities;
	const tls_certificate_request_t* message;

	//Debug message
	msg_len = rc->msg_len;
	TRACELN_TLS("CertificateRequest message received (%u bytes)...", msg_len);
//	TRACE_TLS_ARRAY("  ", rc->msg_start(), msg_len);

	//Check the length of the ServerKeyExchange message
	if (msg_len < sizeof(tls_certificate_request_t))
		return RES_TLS_DECODING_FAILED;

	//Check key exchange method
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//It is a fatal handshake failure alert for an anonymous
		//server to request client authentication
		return RES_TLS_HANDSHAKE_FAILED;
	}
	else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
	{
		//If no PSK identity hint is provided by the server, the
		//ServerKeyExchange message is omitted...
		if (tls_state != TLS_STATE_SERVER_KEY_EXCHANGE
				&& tls_state != TLS_STATE_CERTIFICATE_REQUEST)
		{
			//Handshake failure
			return RES_TLS_UNEXPECTED_MESSAGE;
		}
	}
	else
	{
		//Check current state
		if (tls_state != TLS_STATE_CERTIFICATE_REQUEST)
			return RES_TLS_UNEXPECTED_MESSAGE;
	}

	message = (tls_certificate_request_t*)rc->msg_start();

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(rc->msg_start(), msg_len);

	//The server requests a certificate from the client, so that
	//the connection can be mutually authenticated
	clientCertRequested = true;

	//Point to the beginning of the message
	p = rc->msg_start();
	//Remaining bytes to process
	msg_len -= sizeof(tls_certificate_request_t);

	//Retrieve the size of the list of supported certificate types
	n = message->certificateTypesLength;
	//Make sure the length field is valid
	if (n > msg_len)
		return RES_TLS_DECODING_FAILED;

	//Point to the next field
	p += sizeof(tls_certificate_request_t) + n;
	//Remaining bytes to process
	msg_len -= n;

#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.2 currently selected?
	if (tls_version == TLS_VER12)
	{
		//Malformed CertificateRequest message?
		if (msg_len < sizeof(tls_sign_algos_t))
			return RES_TLS_DECODING_FAILED;

		//Point to the list of the hash/signature algorithm pairs that
		//the server is able to verify
		supportedSignAlgos = (tls_sign_algos_t *) p;
		//Remaining bytes to process
		msg_len -= sizeof(tls_sign_algos_t);

		//Get the size of the list
		n = __REV16(supportedSignAlgos->length);
		//Make sure the length field is valid
		if (n > msg_len)
			return RES_TLS_DECODING_FAILED;

		//Point to the next field
		p += sizeof(tls_sign_algos_t) + n;
		//Remaining bytes to process
		msg_len -= n;
	}
	//SSL 3.0, TLS 1.0 or TLS 1.1 currently selected?
	else
#endif
	{
		//Implementations prior to TLS 1.2 do not include a
		//list of supported hash/signature algorithm pairs
		supportedSignAlgos = nullptr;
	}

	//Malformed CertificateRequest message?
	if (msg_len < sizeof(TlsCertAuthorities))
		return RES_TLS_DECODING_FAILED;

	//Point to the list of the distinguished names of acceptable
	//certificate authorities
	certAuthorities = (TlsCertAuthorities *) p;
	//Remaining bytes to process
	msg_len -= sizeof(TlsCertAuthorities);

	//Get the size of the list
	n = __REV16(certAuthorities->length);
	//Make sure the length field is valid
	if (n > msg_len)
		return RES_TLS_DECODING_FAILED;

	//No suitable certificate has been found for the moment
	cert = nullptr;

	//Loop through the list of available certificates
	for (i = 0; i < numCerts; i++)
	{
		//Check whether the current certificate is suitable
		acceptable = tlsIsCertificateAcceptable(&certs[i],
				message->certificateTypes, message->certificateTypesLength,
				supportedSignAlgos, nullptr, certAuthorities);

#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
		//TLS 1.2 requires additional examinations
		if (acceptable && tls_version == TLS_VER12)
		{
			//The hash and signature algorithms used in the signature of the CertificateVerify
			//message must be one of those present in the SupportedSignatureAlgorithms field
			if (tlsSelectSignHashAlgo(certs[i].signAlgo, supportedSignAlgos) != RES_OK)
				acceptable = false;
		}
#endif

		//If all the requirements were met, the certificate can be
		//used to authenticate the client
		if (acceptable)
		{
			cert = &certs[i];
			break;
		}
	}

	//Prepare to receive ServerHelloDone message...
	tls_state = TLS_STATE_SERVER_HELLO_DONE;
	//Successful processing
	return RES_OK;
}

/**
 * @brief Parse ServerHelloDone message
 *
 * The ServerHelloDone message is sent by the server to indicate the
 * end of the ServerHello and associated messages. After sending this
 * message, the server will wait for a client response
 *
 * @param[in] context Pointer to the TLS context
 * @param[in] message Incoming ServerHelloDone message to parse
 * @param[in] length Message length
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseServerHelloDone(record_ctxt_t* rc)
{
	const tls_server_hellodone_t* message = (const tls_server_hellodone_t*)rc->msg_start();

	//Debug message
	TRACELN_TLS("ServerHelloDone message received (%u bytes)...", rc->msg_len);
//	TRACE_TLS_ARRAY("  ", message, rc->msg_len);

	//Check the length of the ServerHelloDone message
	if (rc->msg_len != sizeof(tls_server_hellodone_t))
		return RES_TLS_DECODING_FAILED;

	//Check key exchange method
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
	{
		//The server may omit the CertificateRequest message and go
		//directly to the ServerHelloDone message
		if (tls_state != TLS_STATE_CERTIFICATE_REQUEST
				&& tls_state != TLS_STATE_SERVER_HELLO_DONE)
		{
			//Handshake failure
			return RES_TLS_UNEXPECTED_MESSAGE;
		}
	}
	else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK)
	{
		//If no PSK identity hint is provided by the server, the
		//ServerKeyExchange message is omitted
		if (tls_state != TLS_STATE_SERVER_KEY_EXCHANGE
				&& tls_state != TLS_STATE_SERVER_HELLO_DONE)
		{
			//Handshake failure
			return RES_TLS_UNEXPECTED_MESSAGE;
		}
	}
	else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
	{
		//The server may omit the ServerKeyExchange message and/or
		//the CertificateRequest message
		if (tls_state != TLS_STATE_SERVER_KEY_EXCHANGE
				&& tls_state != TLS_STATE_CERTIFICATE_REQUEST
				&& tls_state != TLS_STATE_SERVER_HELLO_DONE)
		{
			//Handshake failure
			return RES_TLS_UNEXPECTED_MESSAGE;
		}
	}
	else
	{
		//Check current state
		if (tls_state != TLS_STATE_SERVER_HELLO_DONE)
			return RES_TLS_UNEXPECTED_MESSAGE;
	}

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//Prepare to send client Certificate message...
	tls_state = TLS_STATE_CLIENT_CERTIFICATE;
	//Successful processing
	return RES_OK;
}

RES_CODE tls_context_t::tls_parse_server_message(record_ctxt_t& rc)
{
	RES_CODE res;

    TRACELN1_TLS("TLS parse server message");

    //A message can be fragmented across several records...
	res = tls_message_read(&rc);

	//Check status code
	if (res == RES_OK)
	{
		switch(rc.tls_record.rec_type)
		{
		   case TLS_TYPE_CHANGE_CIPHER_SPEC:
				//The ChangeCipherSpec message is sent by the server and to notify the
				//client that subsequent records will be protected under the newly
				//negotiated CipherSpec and keys
				res = tls_parse_change_cipherspec(&rc);
				break;

		   case TLS_TYPE_ALERT:
				//Parse Alert message
				res = tls_parse_alert(&rc);
				break;

		   case TLS_TYPE_HANDSHAKE:
			   switch(static_cast<tls_handshake_t*>((void*)rc.data.get())->msgType)
			   {
			   case TLS_TYPE_HELLO_REQUEST:
					//The HelloRequest message can be sent at any time but it should be
					//ignored by the client if it arrives in the middle of a handshake
				   break;

			   case TLS_TYPE_SERVER_HELLO:
					//The server will send this message in response to a ClientHello
					//message when it was able to find an acceptable set of algorithms
					res = tls_parse_server_hello(&rc);
					break;

			   case TLS_TYPE_CERTIFICATE:
					//The server must send a Certificate message whenever the agreed-
					//upon key exchange method uses certificates for authentication. This
					//message will always immediately follow the ServerHello message
					res = tls_parse_certificate(&rc);
					break;

			   case TLS_TYPE_SERVER_KEY_EXCHANGE:
					//The ServerKeyExchange message is sent by the server only when the
					//server Certificate message (if sent) does not contain enough data
					//to allow the client to exchange a premaster secret
					res = tlsParseServerKeyExchange(&rc);
					break;

			   case TLS_TYPE_CERTIFICATE_REQUEST:
					//A non-anonymous server can optionally request a certificate from the
					//client, if appropriate for the selected cipher suite. This message,
					//if sent, will immediately follow the ServerKeyExchange message
					res = tlsParseCertificateRequest(&rc);
					break;

			   case TLS_TYPE_SERVER_HELLO_DONE:
					//The ServerHelloDone message is sent by the server to indicate the
					//end of the ServerHello and associated messages
					res = tlsParseServerHelloDone(&rc);
					break;

			   case TLS_TYPE_FINISHED:
					//A Finished message is always sent immediately after a changeCipherSpec
					//message to verify that the key exchange and authentication processes
					//were successful
					res = tlsParseFinished(&rc);
					break;

//			   case TLS_TYPE_CLIENT_HELLO:
//			   case TLS_TYPE_HELLO_VERIFY_REQUEST:
//			   case TLS_TYPE_NEW_SESSION_TICKET:
//			   case TLS_TYPE_CERTIFICATE_VERIFY:
//			   case TLS_TYPE_CLIENT_KEY_EXCHANGE:
//			   case TLS_TYPE_CERTIFICATE_URL:
//			   case TLS_TYPE_CERTIFICATE_STATUS:
//			   case TLS_TYPE_SUPPLEMENTAL_DATA:
			   default:
				   res = RES_TLS_UNEXPECTED_MESSAGE;
				   break;
			   }
			   break;

//		   case TLS_TYPE_APPLICATION_DATA:
//				The server cannot transmit application data
//				before the handshake is completed
		   default:
			   res = RES_TLS_UNEXPECTED_MESSAGE;
			   break;

		}
	}

    TRACELN_TLS("TLS parse server message res %u", res);
    if(rc.msg_len < rc.rec_len && rc.msg_len && rc.data.get())
    {
    	memmove(rc.data.get(), rc.data.get()+rc.msg_len, rc.rec_len - rc.msg_len);
    	rc.rec_len -= rc.msg_len;
    } else
    {
    	rc.rec_len = 0;
    }

	return res;
}

RES_CODE tls_context_t::tls_parse_server_hello(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t n;
	const tls_server_hello_t* message;
	const uint8_t *p;
	tls_suite_id_t suite;
	tls_compression_method_t compression;

	//Check the length of the ServerHello message
	if (rc->msg_len < sizeof(tls_server_hello_t))
		return RES_TLS_DECODING_FAILED;
	n = rc->msg_len - sizeof(tls_server_hello_t);
	message = static_cast<const tls_server_hello_t*>((void*)rc->data.get());
	TRACELN_TLS("Server ver %04X", message->serverVersion);

	//Check current state
	if (tls_state != TLS_STATE_SERVER_HELLO)
		return RES_TLS_UNEXPECTED_MESSAGE;

   //Check the length of the session ID
	if (message->sessionIdLength > 32)
		return RES_TLS_INVALID_PARAMETER;
	if (message->sessionIdLength > n)
		return RES_TLS_DECODING_FAILED;
	n -= message->sessionIdLength;

   //Point to the first field
	p = message->sessionId + message->sessionIdLength;

   //Malformed ServerHello message?
	if (n < (sizeof(tls_suite_id_t) + sizeof(tls_compression_method_t)))
		return RES_TLS_DECODING_FAILED;
	n -= sizeof(tls_suite_id_t) + sizeof(tls_compression_method_t);

	//Get the negotiated cipher suite
	suite = (tls_suite_id_t)__REV16(*(tls_suite_id_t*)(void*)p);
	p += sizeof(tls_suite_id_t);
	TRACELN_TLS("cipher suite %04X", suite);

	//Get the negotiated compression method
	compression = (tls_compression_method_t)*p;

#if TLS_SESSION_RESUME_SUPPORT
   //Check whether the session ID matches the value that was supplied by the client
   if(message->sessionIdLength > 0 && message->sessionIdLength == sessionIdLen &&
      !memcmp(message->sessionId, sessionId, sessionIdLen))
   {
      //For resumed sessions, the selected cipher suite and compression
      //method shall be the same as the session being resumed
      if(suite != cipherSuite || compression != compressionMethod)
      {
         //The session ID is no more valid
         sessionIdLen = 0;
         //When renegotiating, if the server tries to use another
         //version or compression method than previously, abort
         return RES_TLS_HANDSHAKE_FAILED;
      }

      //Perform abbreviated handshake
	   tls_resume = true;
   }
   else
#endif
   {
      //Perform a full handshake
	   tls_resume = false;
   }


   //Save server random value
   memcpy(&tls_randoms.server_random, &message->random, sizeof(tls_random_t));

   //Save session identifier
   memcpy(sessionId, message->sessionId, message->sessionIdLength);
   sessionIdLen = message->sessionIdLength;

   //Set the TLS version to use
   res = tls_set_version(message->serverVersion);
   if(res == RES_OK)
   {
	   //Set cipher suite
	   res = tls_set_ciphersuite(suite);
	   if(res == RES_OK)
	   {
		   TRACELN1_TLS("suite name: ");
		   TRACE1_TLS(cipher_info->suite_name);

		   //Set compression method
		   res = tls_set_compression_method(compression);
		   if(res == RES_OK)
		   {
			   //Initialize handshake message hashing
			   res = tls_handshake_hash_init();
			   if(res == RES_OK)
			   {
				   //Update the hash value with the incoming handshake message
				   tls_handshake_hash_update(message, rc->msg_len);

#if TLS_SESSION_RESUME_SUPPORT
					//Use abbreviated handshake?
					if(tls_resume)
					{
						//Derive session keys from the master secret
						res = tls_generate_keys();
						if(res == RES_OK)
						{
							//At this point, both client and server must send ChangeCipherSpec
							//messages and proceed directly to Finished messages
							tls_state = TLS_STATE_SERVER_CHANGE_CIPHER_SPEC;
						}
					}
					else
#endif
					{
						//Perform a full handshake
						if (cipher_info->suite_key_exch    == TLS_KEY_EXCH_PSK
							|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
							|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
							|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
							|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
						{
							//The Certificate message is omitted from the server's response
							tls_state = TLS_STATE_SERVER_KEY_EXCHANGE;
						}
						else
						{
							//The server is required to send a Certificate message
							tls_state = TLS_STATE_SERVER_CERTIFICATE;
						}
					}
			   }

		   }
	   }
   }
   return res;
}


/**
 * @brief Send ClientKeyExchange message
 *
 * This message is always sent by the client. It must immediately
 * follow the client Certificate message, if it is sent. Otherwise,
 * it must be the first message sent by the client after it receives
 * the ServerHelloDone message
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendClientKeyExchange()
{
	RES_CODE res;

	//Format Key Exchange message
	res = tls_client_key_exchange_msg_len(&last_txrc);
	if(res == RES_OK)
		res = tls_record_get_lens(&last_txrc);

	// Prepare record
	if(res == RES_OK)
	{
		tls_client_key_exchange_t* message;

		last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
		message = (tls_client_key_exchange_t*)last_txrc.msg_start();
		res = tls_client_key_exchange_msg(message, last_txrc.msg_len);
		if (res == RES_OK)
		{
			//Debug message
			TRACELN_TLS("Sending ClientKeyExchange message (%u bytes)...", last_txrc.msg_len);
//			TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

			tls_handshake_hash_update(message, last_txrc.msg_len);

			res = tls_record_write(&last_txrc);

			//Check status code
			if (res == RES_OK || res == RES_IDLE)
			{
				//Derive session keys from the premaster secret
				res = tls_generate_keys();

				//Key material successfully generated?
				if (res == RES_OK)
				{
					//Prepare to send CertificateVerify message...
					tls_state = TLS_STATE_CERTIFICATE_VERIFY;
				}
			}
		}
	}

	return res;
}

RES_CODE tls_context_t::tls_client_key_exchange_msg_len(record_ctxt_t* rc)
{
	RES_CODE res;
	uint32_t len = 0;

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		len = pskIdentity.length() ;
#endif
		len += sizeof(TlsPskIdentity);
	}


   //RSA, Diffie-Hellman or ECDH key exchange method?
	if (cipher_info->suite_key_exch != TLS_KEY_EXCH_PSK)
	{
		//Format client's key exchange parameters
#if (TLS_RSA_SUPPORT || TLS_RSA_PSK_SUPPORT)
		//RSA key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
		{
			//The RSA-encrypted premaster secret in a ClientKeyExchange is preceded by
			//two length bytes. SSL 3.0 implementations do not include these bytes
			len += peerRsaPublicKey.n.mpiGetByteLength();
			if (tls_version > SSL_VER30)
			{
				len +=  2;
			}

		}
		else
#endif
#if (TLS_DH_ANON_SUPPORT || TLS_DHE_RSA_SUPPORT || \
   TLS_DHE_DSS_SUPPORT || TLS_DHE_PSK_SUPPORT)
		//Diffie-Hellman key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK)
		{

			//Generate an ephemeral key pair
			res = dhContext.dhGenerateKeyPair(tls_randoms.prng_algo.get());
			if (res != RES_OK)
				return res;

			len += dhContext.ya.mpiGetByteLength() + 2;
		}
		else
#endif
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		//ECDH key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
		{

#if TLS_ECC_CALLBACK_SUPPORT
			//Any registered callback?
			if (ecdhCallback != NULL)
			{
				//Invoke user callback function
				res = ecdhCallback(this);
			}
			else
#endif
			{
				//No callback function defined
				res = RES_TLS_UNSUPPORTED_ELLIPTIC_CURVE;
			}

			//Check status code
			if (res == RES_TLS_UNSUPPORTED_ELLIPTIC_CURVE)
			{
				//Generate an ephemeral key pair
				res = ecdhContext.ecdhGenerateKeyPair(tls_randoms.prng_algo.get());
				//Any error to report?
				if (res != RES_OK)
					return res;

				//Calculate the negotiated key Z
				res = ecdhContext.ecdhComputeSharedSecret(premasterSecret,
						TLS_MAX_PREMASTER_SECRET_SIZE, &premasterSecretLen);
			}

			if (res != RES_OK)
				return res;

#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)

			len += ecdhContext.params.p.mpiGetByteLength() *2 + 1 + 1;

#else
			return ERROR_NOT_IMPLEMENTED;
#endif
		}
		else
#endif
		//Invalid key exchange method?
		{
			//The specified key exchange method is not supported
			return RES_TLS_UNSUPPORTED_KEY_EXCH_METHOD;
		}
	}

	//Length of the complete handshake message
	len += sizeof(tls_handshake_t);
	rc->msg_len = len;
	return RES_OK;
}

RES_CODE tls_context_t::tls_client_key_exchange_msg(tls_client_key_exchange_t* message, uint32_t len)
{
	RES_CODE res;
	size_t n;
	uint8_t* p;

	//Handshake message type
	message->header.set(TLS_TYPE_CLIENT_KEY_EXCHANGE, len - sizeof(tls_handshake_t));

	//Point to the body of the handshake message
	p = message->data;

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//The client indicates which key to use by including a PSK identity
		//in the ClientKeyExchange message
		TlsPskIdentity *pIdentity = (TlsPskIdentity *) p;

		//Initialize length field
		n = 0;

#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
		   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		//Any registered callback?
		if (pskCallback != NULL)
		{
			//Invoke user callback function
			if (psk_identity_hint.c_str() != nullptr)
				res = pskCallback(this, psk_identity_hint.c_str());
			else
				res = pskCallback(this, "");

			//Any error to report?
			if (res != RES_OK)
				return RES_TLS_UNKNOWN_IDENTITY;
		}

		//Any PSK identity defined?
		n = pskIdentity.length();
		if (n)
		{
			//Copy PSK identity
			memcpy(pIdentity->value, pskIdentity.c_str(), n);
		}
#endif

		//The PSK identity is preceded by a 2-byte length field
		pIdentity->length = __REV16(n);


		//Advance data pointer
		p += n;
	}

	//RSA, Diffie-Hellman or ECDH key exchange method?
	if (cipher_info->suite_key_exch != TLS_KEY_EXCH_PSK)
	{
		//Format client's key exchange parameters
#if (TLS_RSA_SUPPORT || TLS_RSA_PSK_SUPPORT)
		//RSA key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
		{

			//Sanity check
			if (TLS_MAX_PREMASTER_SECRET_SIZE < 48)
				return RES_TLS_BUFFER_OVERFLOW;

			//If RSA is being used for key agreement and authentication, the
			//client generates a 48-byte premaster secret
			premasterSecretLen = 48;

			//The first 2 bytes code the latest version supported by the client
			premasterSecret[0] = TLS_MAX_VERSION >> 8;
			premasterSecret[1] = (uint8_t)TLS_MAX_VERSION;

			//The last 46 bytes contain securely-generated random bytes
			res = tls_randoms.prng_algo->prng_read(premasterSecret + 2, 46);
			if (res != RES_OK)
				return res;

			//The RSA-encrypted premaster secret in a ClientKeyExchange is preceded by
			//two length bytes. SSL 3.0 implementations do not include these bytes
			if (tls_version > SSL_VER30)
			{
				//Encrypt the premaster secret using the server public key
				res = peerRsaPublicKey.rsaesPkcs1v15Encrypt(tls_randoms.prng_algo.get(),
						premasterSecret, 48, p + 2, &n);
				if (res != RES_OK)
					return res;

				//Write the length field
				p[0] = n >> 8;
				p[1] = n;

				//Length of the resulting octet string
				n += 2;
			}
			else
			{
				//Encrypt the premaster secret using the server public key
				res = peerRsaPublicKey.rsaesPkcs1v15Encrypt(tls_randoms.prng_algo.get(),
						premasterSecret, 48, p, &n);
				if (res != RES_OK)
					return res;
			}

		}
		else
#endif
#if (TLS_DH_ANON_SUPPORT || TLS_DHE_RSA_SUPPORT || \
   TLS_DHE_DSS_SUPPORT || TLS_DHE_PSK_SUPPORT)
		//Diffie-Hellman key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK)
		{

			//Encode the client's public value to an opaque vector
			res = dhContext.ya.tlsWriteMpi(p, &n);
			if (res != RES_OK)
				return res;

			//Calculate the negotiated key Z
			res = dhContext.dhComputeSharedSecret(premasterSecret,
					TLS_MAX_PREMASTER_SECRET_SIZE, &premasterSecretLen);
			if (res != RES_OK)
				return res;

			//Leading bytes of Z that contain all zero bits are stripped before
			//it is used as the premaster secret (RFC 4346, section 8.2.1)
			for (n = 0; n < premasterSecretLen; n++)
			{
				if (premasterSecret[n] != 0x00)
					break;
			}

			//Any leading zero bytes?
			if (n > 0)
			{
				//Strip leading zero bytes from the negotiated key
				memmove(premasterSecret, premasterSecret + n, premasterSecretLen - n);
				premasterSecretLen -= n;
			}
		}
		else
#endif
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		//ECDH key exchange method?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
		{

			//Encode the client's public key to an opaque vector
			res = ecdhContext.tlsWriteEcPoint(p, &n);
			if (res != RES_OK)
				return res;
		}
		else
#endif
		//Invalid key exchange method?
		{
			//The specified key exchange method is not supported
			return RES_TLS_UNSUPPORTED_KEY_EXCH_METHOD;
		}
	}

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//Generate premaster secret
		res = tlsGeneratePskPremasterSecret();
		if (res != RES_OK)
			return res;
	}

	return RES_OK;
}

/**
 * @brief Send CertificateVerify message
 *
 * The CertificateVerify message is used to provide explicit verification
 * of a client certificate. This message is only sent following a client
 * certificate that has signing capability
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendCertificateVerify()
{
	RES_CODE res;

	//Initialize status code
	res = RES_OK;

	//The CertificateVerify message is only sent following a client
	//certificate that has signing capability
	if (cert != NULL)
	{
		//Check certificate type
		if (cert->type == TLS_CERT_RSA_SIGN
				|| cert->type == TLS_CERT_DSS_SIGN
				|| cert->type == TLS_CERT_ECDSA_SIGN)
		{
			tls_certificate_verify_t* message=nullptr;

			res = tls_make_Certificate_verify(&message, &last_txrc.msg_len);
			if(res == RES_OK)
			{
				res = tls_record_get_lens(&last_txrc);
				if(res == RES_OK)
				{
					memcpy(last_txrc.msg_start(), message, last_txrc.msg_len);
					tsk_free(message);
					message = nullptr;
					last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
					tls_handshake_hash_update(last_txrc.msg_start(), last_txrc.msg_len);

					//Debug message
					TRACELN_TLS("Sending CertificateVerify message (%u bytes)...n", last_txrc.msg_len);
//					TRACE_TLS_ARRAY("  ", last_txrc.msg_start(), last_txrc.msg_len);

					res = tls_record_write(&last_txrc);
				}

			}

			if(message)
				tsk_free(message);

		}
	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//Prepare to send ChangeCipherSpec message...
		tls_state = TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC;
	}

	//Return status code
	return res;
}

WEAK RES_CODE private_rsa_signature(tls_context_t* context, tls_certificate_verify_t** msg, size_t* len)
{
	return RES_ERROR;
}

RES_CODE tls_context_t::tls_make_Certificate_verify(tls_certificate_verify_t** msg, size_t* len)
{
	RES_CODE res;

	//Initialize message length
	*len = sizeof(tls_certificate_verify_t);

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= TLS_VERSION_1_1)
	//SSL 3.0, TLS 1.0 or TLS 1.1 currently selected?
	if (tls_version <= TLS_VER11)
	{
		TlsDigitalSignature *signature = nullptr;

		*len += sizeof(TlsDigitalSignature);
#if TLS_RSA_SIGN_SUPPORT
		//The client's certificate contains a valid RSA public key?
		if (cert->type == TLS_CERT_RSA_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello (using MD5)
			res = tlsFinalizeHandshakeHash(handshake_hash.get(), "",
					verifyData);

			if (res == RES_OK)
			{
				//Digest all the handshake messages starting at ClientHello (using SHA-1)
				res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "",
						verifyData + MD5_DIGEST_SIZE);
			}

			//Check status code
			if (res == RES_OK)
			{
				if(cert->privateKeyAvailable)
				{
					RsaPrivateKey privateKey;

					//Decode the PEM structure that holds the RSA private key
					res = pemReadRsaPrivateKey(cert->privateKey,
							cert->privateKeyLength, &privateKey);

					//Check status code
					if (res == RES_OK)
					{
						uint32_t length;

						length = privateKey.n.mpiGetByteLength();
						*len += length;
						*msg = (tls_certificate_verify_t*) tsk_malloc(*len);

						if (*msg)
						{
							//Point to the digitally-signed element
							signature = (TlsDigitalSignature *) ((*msg)->signature);

							//Generate a RSA signature using the client's private key
							res = privateKey.tlsGenerateRsaSignature(verifyData,
									signature->value, length);
						}
						else
							res = RES_OUT_OF_MEMORY;
					}
				} else
				{
					res = private_rsa_signature(this, msg, len);
				}
			}

		}
		else
#endif
#if TLS_DSA_SIGN_SUPPORT
		//The client's certificate contains a valid DSA public key?
		if (cert->type == TLS_CERT_DSS_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", verifyData);

			//Check status code
			if (res == RES_OK)
			{
				//Generate a DSA signature using the client's private key
				res = tlsGenerateDsaSignature(verifyData, SHA1_DIGEST_SIZE,
						(uint8_t**) msg, len);
				signature = (TlsDigitalSignature *) ((*msg)->signature);
			}
		}
		else
#endif
#if TLS_ECDSA_SIGN_SUPPORT
		//The client's certificate contains a valid ECDSA public key?
		if (cert->type == TLS_CERT_ECDSA_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", verifyData);

			//Check status code
			if (res == RES_OK)
			{
				//Generate an ECDSA signature using the client's private key
				res = tlsGenerateEcdsaSignature(verifyData, SHA1_DIGEST_SIZE,
						(uint8_t**) msg, len);
				signature = (TlsDigitalSignature *) ((*msg)->signature);
			}
		}
		else
#endif
		//Invalid signature algorithm?
		{
			//Report an error
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}

		if(res == RES_OK && signature)
		{
			//Length of the signature
			signature->length = __REV16(
					*len - sizeof(TlsDigitalSignature)
							- sizeof(tls_certificate_verify_t));
		}
	}
	else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.2 currently selected?
	if (tls_version == TLS_VER12)
	{
		TlsDigitalSignature2* signature = nullptr;
		const hash_info_t* hash_info;

		*len += sizeof(TlsDigitalSignature2);

		//Retrieve the hash algorithm to be used for signing
		hash_info = tlsGetHashAlgo(signHashAlgo);

		//Digest all the handshake messages starting at ClientHello
		if (hash_info == SHA1_HASH_ALGO)
		{
			//Use SHA-1 hash algorithm
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", verifyData);
		}
		else if (prf_hash_algo.get() && (hash_info == prf_hash_algo->hash_info))
		{
			//Use PRF hash algorithm (SHA-256 or SHA-384)
			res = tlsFinalizeHandshakeHash(handshake_hash.get(), "", verifyData);
		}
		else
		{
			//The specified hash algorithm is not supported
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}

		//Handshake message hash successfully computed?
		if (res == RES_OK)
		{
#if TLS_RSA_SIGN_SUPPORT
			//The client's certificate contains a valid RSA public key?
			if(cert->type == TLS_CERT_RSA_SIGN)
			{
				if(cert->privateKeyAvailable)
				{
					RsaPrivateKey privateKey;


					//Decode the PEM structure that holds the RSA private key
					res = pemReadRsaPrivateKey(cert->privateKey,
							cert->privateKeyLength, &privateKey);

					//Check status code
					if(res == RES_OK)
					{
						uint32_t length;

						length = privateKey.n.mpiGetByteLength();
						*len += length;
						*msg = (tls_certificate_verify_t*) tsk_malloc(*len);

						if (*msg)
						{
							//Point to the digitally-signed element
							signature = (TlsDigitalSignature2 *) ((*msg)->signature);

							//Set the relevant signature algorithm
							signature->algorithm.signature = TLS_SIGN_ALGO_RSA;
							signature->algorithm.hash = signHashAlgo;

							//Use the signature algorithm defined in PKCS #1 v1.5
							res = privateKey.rsassaPkcs1v15Sign(hash_info,
									verifyData, signature->signature.value, length);
						}
						else
							res = RES_OUT_OF_MEMORY;
					}
				} else
				{
					res = private_rsa_signature(this, msg, len);
					if(msg)
						signature = (TlsDigitalSignature2 *) ((*msg)->signature);

				}

			}
			else
#endif
#if TLS_DSA_SIGN_SUPPORT
			//The client's certificate contains a valid DSA public key?
			if (cert->type == TLS_CERT_DSS_SIGN)
			{

				//Generate a DSA signature using the client's private key
				res = tlsGenerateDsaSignature(verifyData,
						hash_info->digest_size, (uint8_t**) msg, len);
				if(res == RES_OK)
				{
					signature = (TlsDigitalSignature2 *) ((*msg)->signature);
					signature->algorithm.signature = TLS_SIGN_ALGO_DSA;
					signature->algorithm.hash = signHashAlgo;
				}
			}
			else
#endif
#if TLS_ECDSA_SIGN_SUPPORT
			//The client's certificate contains a valid ECDSA public key?
			if(cert->type == TLS_CERT_ECDSA_SIGN)
			{
				//Generate an ECDSA signature using the client's private key
				res = tlsGenerateEcdsaSignature(verifyData,
						hash_info->digest_size, (uint8_t**) msg, len);
				if(res == RES_OK)
				{
					//Set the relevant signature algorithm
					signature = (TlsDigitalSignature2 *) ((*msg)->signature);
					signature->algorithm.signature = TLS_SIGN_ALGO_ECDSA;
					signature->algorithm.hash = signHashAlgo;
				}
			}
			else
#endif
			//Invalid signature algorithm?
			{
				//Report an error
				res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
			}
		}

		//Length of the signature
		if(res == RES_OK && signature)
		{
			//Length of the signature
			signature->signature.length = __REV16(
					*len - sizeof(TlsDigitalSignature2)
							- sizeof(tls_certificate_verify_t));
		}
	}
	else
#endif
	{
		//The negotiated TLS version is not valid
		res = RES_TLS_INVALID_VERSION;
	}

	//Fix the length field
	if(res == RES_OK && *msg)
	{
		(*msg)->header.set(TLS_TYPE_CERTIFICATE_VERIFY,
				*len - sizeof(tls_certificate_verify_t));
	}

	//Return status code
	return res;
}
