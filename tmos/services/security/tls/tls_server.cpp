/*
 * tls_server.cpp
 *
 *  Created on: Aug 18, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <tls.h>
#include <tls_cipher_suites.h>
#include <tls_misc.h>
#include <tls_cache.h>
#include <hmac.h>
#include <pem.h>
#include <x509.h>

#if TLS_SERVER_SUPPORT

/**
 * @brief TLS server handshake
 *
 * TLS handshake protocol is responsible for the authentication
 * and key exchange necessary to establish a secure session
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsServerHandshake()
{
	RES_CODE res;

	//Initialize status code
	res = RES_OK;

	//Wait for the handshake to complete
	while(tls_state != TLS_STATE_APPLICATION_DATA && res == RES_OK)
	{

			//The TLS handshake is implemented as a state machine
		//representing the current location in the protocol
		switch (tls_state)
		{
		//Default state?
		case TLS_STATE_INIT:
			//The client initiates the TLS handshake by sending a ClientHello
			//message to the server
			tls_state = TLS_STATE_CLIENT_HELLO;
			break;
			//Sending ServerHello message?
		case TLS_STATE_SERVER_HELLO:
			//The server will send this message in response to a ClientHello
			//message when it was able to find an acceptable set of algorithms
			res = tlsSendServerHello();
			break;
			//Sending Certificate message?
		case TLS_STATE_SERVER_CERTIFICATE:
			//The server must send a Certificate message whenever the agreed-
			//upon key exchange method uses certificates for authentication. This
			//message will always immediately follow the ServerHello message
			res = tlsSendCertificate();
			break;
			//Sending ServerKeyExchange message?
		case TLS_STATE_SERVER_KEY_EXCHANGE:
			//The ServerKeyExchange message is sent by the server only when the
			//server Certificate message (if sent) does not contain enough data
			//to allow the client to exchange a premaster secret
			res = tlsSendServerKeyExchange();
			break;
			//Sending Certificate message?
		case TLS_STATE_CERTIFICATE_REQUEST:
			//A non-anonymous server can optionally request a certificate from the
			//client, if appropriate for the selected cipher suite. This message,
			//if sent, will immediately follow the ServerKeyExchange message
			res = tlsSendCertificateRequest();
			break;
			//Sending ServerHelloDone message?
		case TLS_STATE_SERVER_HELLO_DONE:
			//The ServerHelloDone message is sent by the server to indicate the
			//end of the ServerHello and associated messages
			res = tlsSendServerHelloDone();
			break;
			//Sending ChangeCipherSpec message?
		case TLS_STATE_SERVER_CHANGE_CIPHER_SPEC:
			//The ChangeCipherSpec message is sent by the server and to notify the
			//client that subsequent records will be protected under the newly
			//negotiated CipherSpec and keys
			res = tlsSendChangeCipherSpec();
			break;
			//Sending Finished message?
		case TLS_STATE_SERVER_FINISHED:
			//A Finished message is always sent immediately after a changeCipherSpec
			//message to verify that the key exchange and authentication processes
			//were successful
			res = tlsSendFinished();
			break;
			//Waiting for a message from the client?
		case TLS_STATE_CLIENT_HELLO:
		case TLS_STATE_CLIENT_CERTIFICATE:
		case TLS_STATE_CLIENT_KEY_EXCHANGE:
		case TLS_STATE_CERTIFICATE_VERIFY:
		case TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC:
		case TLS_STATE_CLIENT_FINISHED:
			//Parse incoming handshake message
			res = tlsParseClientMessage();
			break;
			//Sending Alert message?
		case TLS_STATE_CLOSING:
			//Mark the TLS connection as closed
			tls_state = TLS_STATE_CLOSED;
			break;
			//TLS connection closed?
		case TLS_STATE_CLOSED:
			//Debug message
			TRACE1_TLS("TLS handshake failure!\r\n");
			//Report an error
			res = RES_TLS_HANDSHAKE_FAILED;
			break;
			//Invalid state?
		default:
			//Report an error
			res = RES_TLS_UNEXPECTED_STATE;
			break;
		}

	};

	//Successful TLS handshake?
	if (res == RES_OK)
	{
		//Save current session in the session cache for further reuse
		tlsSaveToCache(this);
	}
	else
	{
		//Send an alert message to the client, if applicable
		tlsProcessError(res);
	}

	//Return status code
	return res;
}

/**
 * @brief Send ServerHello message
 *
 * The server will send this message in response to a ClientHello
 * message when it was able to find an acceptable set of algorithms.
 * If it cannot find such a match, it will respond with a handshake
 * failure alert
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendServerHello()
{
	RES_CODE res;

	//Generate the server random value using a cryptographically-safe
	//pseudorandom number generator
	res = tls_randoms.tls_random_generate(tls_randoms.server_random);

	//Check status code
	if (res == RES_OK)
	{
		//Format ServerHello message
		res = tls_server_hello_msg_len(&last_txrc);
		if (res == RES_OK)
			res = tls_record_get_lens(&last_txrc);
	}

	//Check status code
	if (res == RES_OK)
	{
		tls_server_hello_t* message;

		last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
		message = (tls_server_hello_t*)last_txrc.msg_start();
		tls_server_make_hello(message, last_txrc.msg_len);
		tls_handshake_hash_update(message, last_txrc.msg_len);

		//Debug message
		TRACE_TLS("Sending ServerHello message (%u bytes)...\r\n", last_txrc.msg_len);
		TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

		res = tls_record_write(&last_txrc);

	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
#if TLS_SESSION_RESUME_SUPPORT
		//Use abbreviated handshake?
		if(tls_resume)
		{
			//Derive session keys from the master secret
			res = tls_generate_keys();

			//Key material successfully generated?
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
			tls_state = TLS_STATE_SERVER_CERTIFICATE;
		}
	}

	//Return status code
	return res;
}

/**
 * @brief Send ServerKeyExchange message
 *
 * The ServerKeyExchange message is sent by the server only when the
 * server Certificate message does not contain enough data to allow
 * the client to exchange a premaster secret
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendServerKeyExchange()
{
	RES_CODE res;

	//Initialize status code
	res = RES_OK;


	//The ServerKeyExchange message is sent by the server only when the server
	//Certificate message (if sent) does not contain enough data to allow the
	//client to exchange a premaster secret
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//Format ServerKeyExchange message
		res = RES_IDLE;
	}
	else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
	{
#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		//If no PSK identity hint is provided by the server, the
		//ServerKeyExchange message is omitted...
		if (!psk_identity_hint.empty())
		{
			//Format ServerKeyExchange message
			res = RES_IDLE;
		}
#endif
	}

	if(res == RES_IDLE)
	{
		uint8_t* message = nullptr;

		res = tls_make_server_key_exchange_msg(message, last_txrc.msg_len);
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
				TRACE_TLS("Sending ServerKeyExchange message (%u bytes)...\r\n", last_txrc.msg_len);
				TRACE_TLS_ARRAY("  ", last_txrc.msg_start(), last_txrc.msg_len);

				res = tls_record_write(&last_txrc);
			}

		}

		if(message)
			tsk_free(message);

	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//Prepare to send a CertificateRequest message...
		tls_state = TLS_STATE_CERTIFICATE_REQUEST;
	}

	//Return status code
	return res;
}

/**
 * @brief Send CertificateRequest message
 *
 * A server can optionally request a certificate from the client, if
 * appropriate for the selected cipher suite. This message will
 * immediately follow the ServerKeyExchange message
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendCertificateRequest()
{
	RES_CODE res = RES_OK;

#if (TLS_RSA_SIGN_SUPPORT || TLS_DSA_SIGN_SUPPORT || TLS_ECDSA_SIGN_SUPPORT)
	//A server can optionally request a certificate from the client
	if (client_auth_mode != TLS_CLIENT_AUTH_NONE)
	{
		//Non-anonymous key exchange?
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
		{
			//Format CertificateRequest message
			res = tls_certificate_request_msg_len(&last_txrc);
			if(res == RES_OK)
				res = tls_record_get_lens(&last_txrc);

			if(res == RES_OK)
			{
				tls_certificate_request_t* message;

				last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
				message = (tls_certificate_request_t*)last_txrc.msg_start();
				res = tls_certificate_request_msg(message, last_txrc.msg_len);
				if (res == RES_OK)
				{
					//Debug message
					TRACE_TLS("Sending CertificateRequest message (%u bytes)...\r\n", last_txrc.msg_len);
					TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

					tls_handshake_hash_update(message, last_txrc.msg_len);

					res = tls_record_write(&last_txrc);

				}
			}
		}
	}
#endif

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//Prepare to send a ServerHelloDone message...
		tls_state = TLS_STATE_SERVER_HELLO_DONE;
	}

	//Return status code
	return res;
}

/**
 * @brief Send ServerHelloDone message
 *
 * The ServerHelloDone message is sent by the server to indicate the
 * end of the ServerHello and associated messages. After sending this
 * message, the server will wait for a client response
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendServerHelloDone()
{
	RES_CODE res;
	tls_server_hellodone_t* message;

	//The ServerHelloDone message does not contain any data
	last_txrc.msg_len =  sizeof(tls_handshake_t);
	res = tls_record_get_lens(&last_txrc);
	if (res == RES_OK)
	{
		//Point to the buffer where to format the message
		message = (tls_server_hellodone_t*) last_txrc.msg_start();
		message->header.set(TLS_TYPE_SERVER_HELLO_DONE, 0);

		//Debug message
		TRACE_TLS("Sending ServerHelloDone message (%u bytes)...\r\n", last_txrc.msg_len);
		TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

		//Send handshake message
		last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
		tls_handshake_hash_update(message, last_txrc.msg_len);

		res = tls_record_write(&last_txrc);

	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//The client must send a Certificate message if the server requests it
		if (client_auth_mode != TLS_CLIENT_AUTH_NONE)
			tls_state = TLS_STATE_CLIENT_CERTIFICATE;
		else
			tls_state = TLS_STATE_CLIENT_KEY_EXCHANGE;
	}

	//Return status code
	return res;
}

/**
 * @brief Parse incoming handshake message
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseClientMessage()
{
	RES_CODE res;
	record_ctxt_t rc;

	//A message can be fragmented across several records...
	res = tls_message_read(&rc);

	//Check status code
	if (res == RES_OK)
	{
		switch(rc.tls_record.rec_type)
		{
		   case TLS_TYPE_HANDSHAKE:
			   switch(static_cast<tls_handshake_t*>((void*)rc.data.get())->msgType)
			   {
				//ClientHello message received?
				case TLS_TYPE_CLIENT_HELLO:
					//When a client first connects to a server, it is required to send
					//the ClientHello as its first message
					res = tlsParseClientHello(&rc);
					break;
					//Certificate message received?
				case TLS_TYPE_CERTIFICATE:
					//This is the first message the client can send after receiving a
					//ServerHelloDone message. This message is only sent if the server
					//requests a certificate
					res = tls_parse_certificate(&rc);
					break;
					//ClientKeyExchange message received?
				case TLS_TYPE_CLIENT_KEY_EXCHANGE:
					//This message is always sent by the client. It must immediately
					//follow the client certificate message, if it is sent. Otherwise,
					//it must be the first message sent by the client after it receives
					//the ServerHelloDone message
					res = tlsParseClientKeyExchange(&rc);
					break;
					//CertificateVerify message received?
				case TLS_TYPE_CERTIFICATE_VERIFY:
					//This message is used to provide explicit verification of a client
					//certificate. This message is only sent following a client certificate
					//that has signing capability. When sent, it must immediately follow
					//the clientKeyExchange message
					res = tlsParseCertificateVerify(&rc);
					break;
					//Finished message received?
				case TLS_TYPE_FINISHED:
					//A Finished message is always sent immediately after a changeCipherSpec
					//message to verify that the key exchange and authentication processes
					//were successful
					res = tlsParseFinished(&rc);
					break;
					//Invalid handshake message received?
				default:
					//Report an error
					res = RES_TLS_UNEXPECTED_MESSAGE;
					break;
			   }
			   break;
		case TLS_TYPE_CHANGE_CIPHER_SPEC:
			//The ChangeCipherSpec message is sent by the client and to notify the
			//server that subsequent records will be protected under the newly
			//negotiated CipherSpec and keys
			res = tls_parse_change_cipherspec(&rc);
			break;

		case TLS_TYPE_ALERT:
			//Parse Alert message
			res = tls_parse_alert(&rc);
			break;

		default:
			//The client cannot transmit application data
			//before the handshake is completed
			res = RES_TLS_UNEXPECTED_MESSAGE;
			break;
		}

	}
	return res;
}

RES_CODE tls_context_t::tls_server_hello_msg_len(record_ctxt_t* rc)
{
	uint32_t len;


	len = sizeof(tls_server_hello_t);

#if TLS_SESSION_RESUME_SUPPORT
	len += sessionIdLen;
#endif

	len += sizeof(tls_suite_id_t) + sizeof(tls_compression_method_t);


#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
	   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//A server that selects an ECC cipher suite in response to a ClientHello
	//message including an EcPointFormats extension appends this extension
	//to its ServerHello message
	if (is_ecc_cipher_suite(get_cipher_suite_info(cipherSuite)) && ecPointFormatExtFound)
	{
		len += 1 + sizeof(tls_ecpoint_format_list_t) + sizeof(tls_extension_t)
				+ sizeof(tls_extensions_t);
	}
#endif

	rc->msg_len = len;
	return RES_OK;
}

RES_CODE tls_context_t::tls_server_make_hello(tls_server_hello_t* message, uint32_t len)
{
	uint8_t *p;

	//Handshake message type and length
	message->header.set(TLS_TYPE_SERVER_HELLO, len - sizeof(tls_handshake_t));

	//This field contains the lower of the version suggested by the client
	//in the ClientHello and the highest supported by the server
	message->serverVersion = tls_version;

	//Server random value
	message->random = tls_randoms.server_random;

	//Point to the session ID
	p = message->sessionId;

#if TLS_SESSION_RESUME_SUPPORT
	//The SessionID uniquely identifies the current session
	message->sessionIdLength = sessionIdLen;
	memcpy(message->sessionId, sessionId, sessionIdLen);
#else
	//The server may return an empty session ID to indicate that the session
	//will not be cached and therefore cannot be resumed
	message->sessionIdLength = 0;
#endif

	//Debug message
	TRACE_TLS("Session ID (%u bytes):\r\n", message->sessionIdLength);
	TRACE_TLS_ARRAY("  ", message->sessionId, message->sessionIdLength);

	//Advance data pointer
	p += message->sessionIdLength;

	//The single cipher suite selected by the server
	p[0] = cipherSuite >> 8;
	p[1] = cipherSuite;

	//Advance data pointer
	p += sizeof(tls_suite_id_t);

	//The single compression algorithm selected by the server
	*p = compressionMethod;

#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
	   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//A server that selects an ECC cipher suite in response to a ClientHello
	//message including an EcPointFormats extension appends this extension
	//to its ServerHello message
	if (is_ecc_cipher_suite(get_cipher_suite_info(cipherSuite)) && ecPointFormatExtFound)
	{
		tls_extensions_t* extensionList;
		tls_ecpoint_format_list_t* ecPointFormatList;

		//Only extensions offered by the client can appear in the server's list
		extensionList = (tls_extensions_t *) (p + sizeof(tls_compression_method_t));

		//Type of the extension
		extensionList->value[0].type = TLS_EXT_EC_POINT_FORMATS;

		//Point to the list of supported EC point formats
		ecPointFormatList = (tls_ecpoint_format_list_t *) extensionList->value[0].value;

		//The server can parse only the uncompressed point format...
		ecPointFormatList->value[0] = TLS_EC_POINT_FORMAT_UNCOMPRESSED;
		ecPointFormatList->length = sizeof(tls_ecpoint_format_t);

		//Consider the 2-byte length field that precedes the list
		len = sizeof(tls_ecpoint_format_t) + sizeof(tls_ecpoint_format_list_t);
		extensionList->value[0].length = __REV16(len);

		//Compute the length, in bytes, of the EcPointFormats extension
		len += sizeof(tls_extension_t);

		//Convert the length of the extension list to network byte order
		extensionList->length = __REV16(len);
	}
#endif


	return RES_OK;
}

RES_CODE tls_context_t::tls_make_server_key_exchange_msg(uint8_t*& msg, size_t& len)
{
	RES_CODE res = RES_OK;
	uint32_t n, n2, n3;
	size_t params, paramsLen;

	len = sizeof(tls_handshake_t);

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		TlsPskIdentityHint* pskIdentityHint;

		n = sizeof(TlsPskIdentityHint);
		//To help the client in selecting which identity to use, the server
		//can provide a PSK identity hint in the ServerKeyExchange message
#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
		n += psk_identity_hint.length();
#endif
		msg = (uint8_t*)tsk_realloc(msg, n + len);
		if(!msg)
			return RES_OUT_OF_MEMORY;
		pskIdentityHint = (TlsPskIdentityHint*)(msg + len);
		pskIdentityHint->length = __REV16(n);
		len += n;
#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	      //Copy PSK identity hint
	      memcpy(pskIdentityHint->value, psk_identity_hint.c_str(), n);
#endif

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
		params = len;

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

	         //Debug message
			TRACE1_TLS("Diffie-Hellman parameters:\r\n");
			TRACE1_TLS("  Prime modulus:\r\n");
			TRACE_MPI("    ", &dhContext.params.p);
			TRACE1_TLS("  Generator:\r\n");
			TRACE_MPI("    ", &dhContext.params.g);
			TRACE1_TLS("  Server public value:\r\n");
			TRACE_MPI("    ", &dhContext.ya);

			n = dhContext.params.p.mpiGetByteLength();
			n2 = dhContext.params.g.mpiGetByteLength();
			n3 = dhContext.ya.mpiGetByteLength();
			msg = (uint8_t*)tsk_realloc(msg, len + n + n2 + n3 + 6);
			if(!msg)
				return RES_OUT_OF_MEMORY;

			msg[len] = n >> 8;
			msg[len+1] = n;
			dhContext.params.p.mpiWriteRaw(msg + len + 2, n);
			len += n + 2;

			msg[len] = n2 >> 8;
			msg[len+1] = n2;
			dhContext.params.g.mpiWriteRaw(msg + len + 2, n2);
			len += n2 + 2;

			msg[len] = n3 >> 8;
			msg[len+1] = n3;
			dhContext.ya.mpiWriteRaw(msg + len + 2, n3);
			len += n3 + 2;

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
			const EcCurveInfo *curveInfo;

			//Retrieve the elliptic curve to be used
			curveInfo = tlsGetCurveInfo(named_curve);

			//Make sure the elliptic curve is supported
			if (curveInfo !=nullptr)
			{
				//Load EC domain parameters
				res = ecdhContext.params.ecLoadDomainParameters(curveInfo);
				if(res != RES_OK)
					return res;

				//Generate an ephemeral key pair
				res = ecdhContext.ecdhGenerateKeyPair(tls_randoms.prng_algo.get());
				if(res != RES_OK)
					return res;

	            //Debug message
	            TRACE1_TLS("  Server public key X:\r\n");
	            TRACE_MPI("    ", &ecdhContext.qa.x);
	            TRACE1_TLS("  Server public key Y:\r\n");
	            TRACE_MPI("    ", &ecdhContext.qa.y);

				n= sizeof(TlsEcCurveType) + sizeof(tls_ecnamedcurve_t);

				n += ecdhContext.params.p.mpiGetByteLength() *2 + 1 + 1;

				msg = (uint8_t*)tsk_realloc(msg, n + len);
				if(!msg)
					return RES_OUT_OF_MEMORY;

	            //Set the type of the elliptic curve domain parameters
	            msg[len++] = TLS_EC_CURVE_TYPE_NAMED_CURVE;


	            //Write elliptic curve identifier
	            msg[len++] = named_curve >> 8;
	            msg[len++] = named_curve;

	            //Write server's public key
	            res = ecdhContext.tlsWriteEcPoint(msg+len, &n);
	            len += n;
			}
			else
			{
				//The specified elliptic curve is not supported
				res = RES_TLS_FAIL;
			}
		}
		else
#endif
		//Any other exchange method?
		{
			//It is not legal to send the ServerKeyExchange message when a key
			//exchange method other than DHE_DSS, DHE_RSA, DH_anon, ECDHE_RSA,
			//ECDHE_ECDSA or ECDH_anon is selected
			res = RES_TLS_FAIL;
		}
	}
	if(res != RES_OK)
		return res;

	//For non-anonymous Diffie-Hellman and ECDH key exchanges, a signature
	//over the server's key exchange parameters shall be generated
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
	{
		//Sign server's key exchange parameters

		paramsLen = len - params;

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= TLS_VERSION_1_1)
		//SSL 3.0, TLS 1.0 or TLS 1.1 currently selected?
		if (tls_version <= TLS_VER11)
		{
			TlsDigitalSignature *signature;


			len += sizeof(TlsDigitalSignature);

#if TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
			//Check whether DHE_RSA or ECDHE_RSA key exchange method is currently used
			if(cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA ||
					cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA)
			{
				hash_algo_t* algo;
				RsaPrivateKey privateKey;


				//Allocate a memory buffer to hold the MD5 context
				algo = new_md5_hash();
				if(algo == nullptr)
					return RES_OUT_OF_MEMORY;

				//Compute MD5(ClientHello.random + ServerHello.random + ServerDhParams)
				algo->Input(&tls_randoms, 64);
				algo->Input(msg + params, paramsLen);
				algo->Result(verifyData);

				//Release previously allocated memory
				delete algo;

				//Allocate a memory buffer to hold the SHA-1 context
				algo = new_sha1_hash();
				if(algo == nullptr)
					return RES_OUT_OF_MEMORY;

				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				algo->Input(&tls_randoms, 64);
				algo->Input(msg+params, paramsLen);
				algo->Result(verifyData + MD5_DIGEST_SIZE);

				//Release previously allocated memory
				delete algo;


				//Decode the PEM structure that holds the RSA private key
				res = pemReadRsaPrivateKey(cert->privateKey,
						cert->privateKeyLength, &privateKey);
				if(res != RES_OK)
					return res;

				//Sign the key exchange parameters using RSA
				n = privateKey.n.mpiGetByteLength();
				msg = (uint8_t*)tsk_realloc(msg, n + len);
				if(msg == nullptr)
					return RES_OUT_OF_MEMORY;


				//Generate a RSA signature using the client's private key
				res = privateKey.tlsGenerateRsaSignature(verifyData,
						msg+len, n);
				len += n;

			}
			else
#endif
#if TLS_DHE_DSS_SUPPORT
			//Check whether DHE_DSS key exchange method is currently used
			if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS)
			{
				hash_algo_t* algo;

				algo = new_sha1_hash();
				if(algo == nullptr)
					return RES_OUT_OF_MEMORY;

				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				algo->Input(&tls_randoms, 64);
				algo->Input(msg+params, paramsLen);
				algo->Result(verifyData);

				delete algo;

				//Sign the key exchange parameters using DSA
				res = tlsGenerateDsaSignature(verifyData, SHA1_DIGEST_SIZE, &msg, &len);
			}
			else
#endif
#if TLS_ECDHE_ECDSA_SUPPORT
			//Check whether ECDHE_ECDSA key exchange method is currently used
			if(cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
			{
				hash_algo_t* algo;

				algo = new_sha1_hash();
				if(algo == nullptr)
					return RES_OUT_OF_MEMORY;

				//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
				algo->Input(&tls_randoms, 64);
				algo->Input(msg+params, paramsLen);
				algo->Result(verifyData);

				delete algo;


				//Sign the key exchange parameters using ECDSA
				res = tlsGenerateEcdsaSignature(verifyData,	SHA1_DIGEST_SIZE, &msg, &len);
			}
			else
#endif
			//Invalid signature algorithm?
			{
				//Report an error
				res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
			}

			if(res != RES_OK)
				return res;

			//Fix the length of the digitally-signed element
			params += paramsLen;
			signature = (TlsDigitalSignature*)(msg + params);
			signature->length = __REV16(len - params - sizeof(TlsDigitalSignature));
		}
		else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
		//TLS 1.2 currently selected?
		if (tls_version == TLS_VER12)
		{
			TlsDigitalSignature2* signature = nullptr;
			const hash_info_t* hash_info;
			hash_algo_t* algo;

			len += sizeof(TlsDigitalSignature2);

			//Retrieve the hash algorithm used for signing
			hash_info = tlsGetHashAlgo(signHashAlgo);

			//Make sure the hash algorithm is supported
			if (hash_info != nullptr)
			{
				//Allocate a memory buffer to hold the hash context
				algo = hash_info->new_hash();

				//Successful memory allocation?
				if (algo != nullptr)
				{
					//Compute SHA(ClientHello.random + ServerHello.random + ServerDhParams)
					algo->Input(&tls_randoms, 64);
					algo->Input(msg+params, paramsLen);
					algo->Result(nullptr);
					params += paramsLen;

#if TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
					//Check whether DHE_RSA or ECDHE_RSA key exchange method is currently used
					if(cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA ||
							cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA)
					{
						RsaPrivateKey privateKey;

						//Decode the PEM structure that holds the RSA private key
						res = pemReadRsaPrivateKey(cert->privateKey,
								cert->privateKeyLength, &privateKey);

						n = privateKey.n.mpiGetByteLength();
						len += n;
						msg = (uint8_t*)tsk_realloc(msg, len);

						if (msg)
						{
							//Point to the digitally-signed element
							signature = (TlsDigitalSignature2 *) (msg + params);

							//Set the relevant signature algorithm
							signature->algorithm.signature = TLS_SIGN_ALGO_RSA;
							signature->algorithm.hash = signHashAlgo;

							//Use the signature algorithm defined in PKCS #1 v1.5
							res = privateKey.rsassaPkcs1v15Sign(hash_info,
									algo->digest8(), signature->signature.value, n);
						}
						else
							res = RES_OUT_OF_MEMORY;

					}
					else
#endif
#if TLS_DHE_DSS_SUPPORT
					//Check whether DHE_DSS key exchange method is currently used
					if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS)
					{
						//Sign the key exchange parameters using DSA
						res = tlsGenerateDsaSignature(algo->digest8(),
								hash_info->digest_size, &msg, &len);
						if(res == RES_OK)
						{
							signature = (TlsDigitalSignature2 *) (msg + params);
							signature->algorithm.signature = TLS_SIGN_ALGO_DSA;
							signature->algorithm.hash = signHashAlgo;
						}
					}
					else
#endif
#if TLS_ECDHE_ECDSA_SUPPORT
					//Check whether ECDHE_ECDSA key exchange method is currently used
					if(cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
					{
						//Sign the key exchange parameters using ECDSA
						res = tlsGenerateEcdsaSignature(algo->digest8(),
								hash_info->digest_size, &msg, &len);
						if(res == RES_OK)
						{
							//Set the relevant signature algorithm
							signature = (TlsDigitalSignature2 *) (msg + params);
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

					//Release previously allocated memory
					delete algo;
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

			//Fix the length of the digitally-signed element
			if(signature)
			{
				//Length of the signature
				signature->signature.length = __REV16(len - params - sizeof(TlsDigitalSignature2));
			}
		}
		else
#endif
		{
			//The negotiated TLS version is not valid
			res = RES_TLS_INVALID_VERSION;
		}


		if(res != RES_OK)
			return res;
	}

	if(msg)
		static_cast<tls_handshake_t*>((void*)msg)->set(TLS_TYPE_SERVER_KEY_EXCHANGE,
				len - sizeof(tls_handshake_t));
	return res;
}

RES_CODE tls_context_t::tls_certificate_request_msg_len(record_ctxt_t* rc)
{
	RES_CODE res = RES_OK;
	uint32_t len;
	const char* pemCert;
	size_t pemCertLength;
	uint8_t* derCert;
	size_t derCertSize;
	size_t derCertLength;
	X509CertificateInfo* certInfo;

	len = sizeof(tls_certificate_request_t);

	//Enumerate the types of certificate types that the client may offer
#if TLS_RSA_SIGN_SUPPORT
	//Accept certificates that contain a RSA public key
	len++;
#endif
#if TLS_DSA_SIGN_SUPPORT
	//Accept certificates that contain a DSA public key
	len++;
#endif
#if TLS_ECDSA_SIGN_SUPPORT
	//Accept certificates that contain an ECDSA public key
	len++;
#endif


#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//Check whether TLS 1.2 is currently used
	if (tls_version == TLS_VER12)
	{

#if TLS_RSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//SHA-1 with RSA is supported
		len += sizeof(tls_signature_algo_t);
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//SHA-256 with RSA is supported
			len += sizeof(tls_signature_algo_t);
		}
#endif
#if TLS_SHA384_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA384_HASH_ALGO)
		{
			//SHA-384 with RSA is supported
			len += sizeof(tls_signature_algo_t);
		}
#endif
#endif

#if TLS_DSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//DSA with SHA-1 is supported
		len += sizeof(tls_signature_algo_t);
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//DSA with SHA-256 is supported
			len += sizeof(tls_signature_algo_t);
		}
#endif
#endif

#if TLS_ECDSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//ECDSA with SHA-1 is supported
		len += sizeof(tls_signature_algo_t);
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//ECDSA with SHA-256 is supported
			len += sizeof(tls_signature_algo_t);
		}
#endif
#if TLS_SHA384_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA384_HASH_ALGO)
		{
			//ECDSA with SHA-384 is supported
			len += sizeof(tls_signature_algo_t);
		}
#endif
#endif

		len += sizeof(tls_sign_algos_t);
	}
#endif

	//Total length of the message
	len += sizeof(TlsCertAuthorities);

	//Point to the first trusted CA certificate
	pemCert = trustedCaList;
	//Get the total length, in bytes, of the trusted CA list
	pemCertLength = trustedCaListLen;

	//DER encoded certificate
	derCert = nullptr;
	derCertSize = 0;
	derCertLength = 0;

	//Allocate a memory buffer to store X.509 certificate info
	certInfo = (X509CertificateInfo*)tsk_malloc(sizeof(X509CertificateInfo));

	//Successful memory allocation?
	if (certInfo != nullptr)
	{
		//Loop through the list of trusted CA certificates
		while (pemCertLength > 0)
		{
			//Decode PEM certificate
			res = pemReadCertificate(&pemCert, &pemCertLength, &derCert,
					&derCertSize, &derCertLength);
			if (res != RES_OK)
			{
				//End of file detected
				res = RES_OK;
				break;
			}

			//Parse X.509 certificate
			res = certInfo->x509ParseCertificate(derCert, derCertLength);
			if (res != RES_OK)
				break;

			//Total length of the message
			len += certInfo->subject.rawDataLen + 2;

			//Prevent the buffer from overflowing
			if (len > TLS_MAX_RECORD_LENGTH)
			{
				//Report an error
				res = RES_TLS_MESSAGE_TOO_LONG;
				break;
			}

		}

		//Free previously allocated memory
		tsk_free(derCert);
		tsk_free(certInfo);

	}
	else
	{
		//Report an error
		res = RES_OUT_OF_MEMORY;
	}

	rc->msg_len = len;
	return res;
}

RES_CODE tls_context_t::tls_certificate_request_msg(tls_certificate_request_t* msg, size_t len)
{
	RES_CODE res = RES_OK;
	size_t n;
	uint8_t* p;
	const char* pemCert;
	size_t pemCertLength;
	uint8_t* derCert;
	size_t derCertSize;
	size_t derCertLength;
	X509CertificateInfo* certInfo;
	TlsCertAuthorities* certAuthorities;

	//Handshake message type
	msg->header.set(TLS_TYPE_CERTIFICATE_REQUEST, len - sizeof(tls_handshake_t));

	//Enumerate the types of certificate types that the client may offer
	n = 0;

#if TLS_RSA_SIGN_SUPPORT
	//Accept certificates that contain a RSA public key
	msg->certificateTypes[n++] = TLS_CERT_RSA_SIGN;
#endif
#if TLS_DSA_SIGN_SUPPORT
	//Accept certificates that contain a DSA public key
	msg->certificateTypes[n++] = TLS_CERT_DSS_SIGN;
#endif
#if TLS_ECDSA_SIGN_SUPPORT
	//Accept certificates that contain an ECDSA public key
	msg->certificateTypes[n++] = TLS_CERT_ECDSA_SIGN;
#endif

	//Set the length of the list
	msg->certificateTypesLength = n;
	p = &msg->certificateTypes[n];

#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//Check whether TLS 1.2 is currently used
	if (tls_version == TLS_VER12)
	{
		tls_sign_algos_t* supportedSignAlgos;

		//Point to the list of the hash/signature algorithm pairs that
		//the server is able to verify
		supportedSignAlgos = (tls_sign_algos_t*)p;

		//Enumerate the hash/signature algorithm pairs in descending
		//order of preference
		n = 0;

#if TLS_RSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//SHA-1 with RSA is supported
		supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_RSA;
		supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA1;
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//SHA-256 with RSA is supported
			supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_RSA;
			supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA256;
		}
#endif
#if TLS_SHA384_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA384_HASH_ALGO)
		{
			//SHA-384 with RSA is supported
			supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_RSA;
			supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA384;
		}
#endif
#endif

#if TLS_DSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//DSA with SHA-1 is supported
		supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_DSA;
		supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA1;
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//DSA with SHA-256 is supported
			supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_DSA;
			supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA256;
		}
#endif
#endif

#if TLS_ECDSA_SIGN_SUPPORT
#if TLS_SHA1_SUPPORT
		//ECDSA with SHA-1 is supported
		supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_ECDSA;
		supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA1;
#endif
#if TLS_SHA256_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA256_HASH_ALGO)
		{
			//ECDSA with SHA-256 is supported
			supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_ECDSA;
			supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA256;
		}
#endif
#if TLS_SHA384_SUPPORT
		//The hash algorithm used for PRF operations can also be used for signing
		if(prf_hash_algo.get() && prf_hash_algo->hash_info == SHA384_HASH_ALGO)
		{
			//ECDSA with SHA-384 is supported
			supportedSignAlgos->value[n].signature = TLS_SIGN_ALGO_ECDSA;
			supportedSignAlgos->value[n++].hash = TLS_HASH_ALGO_SHA384;
		}
#endif
#endif

		//Fix the length of the list
		n *= sizeof(tls_signature_algo_t);
		supportedSignAlgos->length = __REV16(n);
		p += sizeof(tls_sign_algos_t) + n;
	}
#endif

	//Point to the list of the distinguished names of acceptable
	//certificate authorities
	certAuthorities = (TlsCertAuthorities*)p;

	//Point to the first certificate authority
	p = certAuthorities->value;
	//Length of the list in bytes
	n = 0;

	//Point to the first trusted CA certificate
	pemCert = trustedCaList;
	//Get the total length, in bytes, of the trusted CA list
	pemCertLength = trustedCaListLen;

	//DER encoded certificate
	derCert = nullptr;
	derCertSize = 0;
	derCertLength = 0;

	//Allocate a memory buffer to store X.509 certificate info
	certInfo = (X509CertificateInfo*)tsk_malloc(sizeof(X509CertificateInfo));

	//Successful memory allocation?
	if (certInfo != nullptr)
	{
		//Loop through the list of trusted CA certificates
		while (pemCertLength > 0)
		{
			//Decode PEM certificate
			res = pemReadCertificate(&pemCert, &pemCertLength, &derCert,
					&derCertSize, &derCertLength);
			if (res != RES_OK)
			{
				//End of file detected
				res = RES_OK;
				break;
			}

			//Parse X.509 certificate
			res = certInfo->x509ParseCertificate(derCert, derCertLength);
			if (res != RES_OK)
				break;


			//Each distinguished name is preceded by a 2-byte length field
			*(uint16_t*)(void*)p = __REV16(certInfo->subject.rawDataLen);
			//The distinguished name shall be DER encoded
			memcpy(p + 2, certInfo->subject.rawData,
					certInfo->subject.rawDataLen);

			//Advance data pointer
			p += certInfo->subject.rawDataLen + 2;
			//Adjust the length of the list
			n += certInfo->subject.rawDataLen + 2;
		}

		//Free previously allocated memory
		tsk_free(derCert);
		tsk_free(certInfo);

		//Fix the length of the list
		certAuthorities->length = __REV16(n);
	}
	else
	{
		//Report an error
		res = RES_OUT_OF_MEMORY;
	}

	//Return status code
	return res;
}

RES_CODE tls_context_t::tlsParseClientHello(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t i;
	size_t j;
	size_t k;
	size_t n;
	bool acceptable;
	uint8_t certType;
	const uint8_t *p;
	const tls_cipher_suites_t* cipherSuites;
	const tls_compression_methods_t* compressionMethods;
	const tls_extension_t* extension;
	const tls_sign_algos_t* supportedSignAlgos;
	const tls_elliptic_curve_list_t* curveList;
	const tls_client_hello_t* message;

	message = (tls_client_hello_t*)rc->msg_start();

	//Debug message
	TRACE_TLS("ClientHello message received (%u bytes)...\r\n", rc->msg_len);
	TRACE_TLS_ARRAY("  ", message, rc->msg_len);

	//Check the length of the ClientHello message
	if (rc->msg_len < sizeof(tls_client_hello_t))
		return RES_TLS_DECODING_FAILED;

	//Check current state
	if (tls_state != TLS_STATE_CLIENT_HELLO)
		return RES_TLS_UNEXPECTED_MESSAGE;

	//Remaining bytes to process
	n = rc->msg_len - sizeof(tls_client_hello_t);

	//Check the length of the session ID
	if (message->sessionIdLen > n)
		return RES_TLS_DECODING_FAILED;
	if (message->sessionIdLen > 32)
		return RES_TLS_ILLEGAL_PARAMETER;

	//Debug message
	TRACE_TLS("Session ID (%u bytes):\r\n", message->sessionIdLen);
	TRACE_TLS_ARRAY("  ", message->sessionId, message->sessionIdLen);

	//Point to the next field
	p = message->sessionId + message->sessionIdLen;
	//Remaining bytes to process
	n -= message->sessionIdLen;

	//Malformed ClientHello message?
	if (n < sizeof(tls_cipher_suites_t))
		return RES_TLS_DECODING_FAILED;

	//List of cryptographic algorithms supported by the client
	cipherSuites = (tls_cipher_suites_t *) p;
	//Remaining bytes to process
	n -= sizeof(tls_cipher_suites_t);

	//Check the length of the list
	k = __REV16(cipherSuites->length);
	if (k < 2)
		return RES_TLS_ILLEGAL_PARAMETER;
	if (k > n)
		return RES_TLS_DECODING_FAILED;

	//Point to the next field
	p += sizeof(tls_cipher_suites_t) + k;
	//Remaining bytes to process
	n -= k;

	//Get the number of cipher suite identifiers present in the list
	k  /= sizeof(tls_suite_id_t);

	//Debug message
	TRACE1_TLS("Cipher suites:\r\n");

	//Dump the list of cipher suites
	for (i = 0; i < k; i++)
	{
		//Debug message
		TRACE_TLS("  0x%04u (%s)\r\n", __REV16(cipherSuites->value[i]),
				tlsGetCipherSuiteName(__REV16(cipherSuites->value[i])));
	}

	//Malformed ClientHello message?
	if (n < sizeof(tls_compression_methods_t))
		return RES_TLS_DECODING_FAILED;

	//List of compression algorithms supported by the client
	compressionMethods = (tls_compression_methods_t *)p;
	//Remaining bytes to process
	n -= sizeof(tls_compression_methods_t);

	//Check the length of the list
	if (compressionMethods->length < 1)
		return RES_TLS_ILLEGAL_PARAMETER;
	if (compressionMethods->length > n)
		return RES_TLS_DECODING_FAILED;

	//Point to the next field
	p += sizeof(tls_compression_methods_t) + compressionMethods->length;
	//Remaining bytes to process
	n -= compressionMethods->length;

	//Parse the list of extensions offered by the client
	extension = tlsGetExtension(p, n, TLS_EXT_ELLIPTIC_CURVES);

	//The EllipticCurves extension was found?
	if (extension)
	{
		//This extension allows a client to enumerate the elliptic curves it supports
		curveList = (tls_elliptic_curve_list_t *)extension->value;

		//Check the length of the list
		if (__REV16(extension->length) < sizeof(tls_elliptic_curve_list_t))
			return RES_TLS_DECODING_FAILED;
		if (__REV16(extension->length) <
				(sizeof(tls_elliptic_curve_list_t) + __REV16(curveList->length)))
			return RES_TLS_DECODING_FAILED;
	}
	else
	{
		//The client may omit the SignatureAlgorithms extension
		curveList = nullptr;
	}

	//Parse the list of extensions offered by the client
	extension = tlsGetExtension(p, n, TLS_EXT_EC_POINT_FORMATS);

	//The EcPointFormats extension was found?
	if (extension)
		ecPointFormatExtFound = true;
	else
		ecPointFormatExtFound = false;

	//Parse the list of extensions offered by the client
	extension = tlsGetExtension(p, n, TLS_EXT_SIGNATURE_ALGORITHMS);

	//The SignatureAlgorithms extension was found?
	if (extension)
	{
		//Point to the list of supported hash/signature algorithm pairs
		supportedSignAlgos = (tls_sign_algos_t *) extension->value;

		//Check the length of the list
		if (__REV16(extension->length) < sizeof(tls_sign_algos_t))
			return RES_TLS_DECODING_FAILED;
		if (__REV16(extension->length)
				< (sizeof(tls_sign_algos_t) + __REV16(supportedSignAlgos->length)))
			return RES_TLS_DECODING_FAILED;
	}
	else
	{
		//The client may omit the SignatureAlgorithms extension
		supportedSignAlgos = nullptr;
	}

	//Get the version the client wishes to use during this session
	client_version = message->clientVersion;

	//If a TLS server receives a ClientHello containing a version number
	//greater than the highest version supported by the server, it must
	//reply according to the highest version supported by the server
   res = tls_set_version((tls_version_t)min(client_version, REV16(TLS_MAX_VERSION)));
   if (res != RES_OK)
		return res;

	//Save client random value
	tls_randoms.client_random = message->random;

#if TLS_SESSION_RESUME_SUPPORT
	//Check whether session caching is supported
	if (true)
	{
		//Check whether a matching entry has been found in the cache
		if (tlsFindAndRestoreCache(this, message->sessionId,
				message->sessionIdLen) == RES_OK)
		{
			//Perform abbreviated handshake
			tls_resume = true;
		}
		else
		{
			//Generate a new random ID
			res = tls_randoms.prng_algo->prng_read(sessionId, 32);
			if (res != RES_OK)
				return res;

			//Session ID is limited to 32 bytes
			sessionIdLen = 32;
			//Perform a full handshake
			tls_resume = false;
		}
	}
	else
#endif
	{
		//This session cannot be resumed
		sessionIdLen = 0;
		//Perform a full handshake
		tls_resume = false;
	}

	//Full handshake?
	if (!tls_resume)
	{
		//Get the size of the cipher suite list
		k = __REV16(cipherSuites->length) / 2;

		//The cipher suite list contains the combinations of cryptographic algorithms
		//supported by the client in order of the client's preference
		for (i = 0; i < k; i++)
		{
			//Check whether the current cipher suite is supported
			res = tls_set_ciphersuite((tls_suite_id_t)__REV16(cipherSuites->value[i]));

			//Successful processing?
			if (res == RES_OK)
			{
				//ECC cipher suite?
				if (cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDH_ANON
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
				{
					//One of the proposed ECC cipher suites must be negotiated only
					//if the server can successfully complete the handshake while
					//using the curves and point formats supported by the client
					res = tlsSelectNamedCurve(curveList);
				}
			}

			//Successful processing?
			if (res == RES_OK)
			{
				//The server requires a valid certificate whenever the agreed-upon
				//key exchange method uses certificates for authentication
				if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
						|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
				{
					//RSA, DHE_RSA, ECDHE_RSA and RAS_PSK key exchange methods
					//require a RSA certificate
					certType = TLS_CERT_RSA_SIGN;
				}
				else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS)
				{
					//DHE_DSS key exchange method requires a DSA certificate
					certType = TLS_CERT_DSS_SIGN;
				}
				else if (cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA)
				{
					//ECDHE_ECDSA key exchange method requires an ECDSA certificate
					certType = TLS_CERT_ECDSA_SIGN;
				}
				else
				{
					//DH_anon and ECDH_anon key exchange methods do not require any certificate
					certType = TLS_CERT_NONE;
				}

				//Check whether a certificate is required
				if (certType != TLS_CERT_NONE)
				{
					//Do not accept the specified cipher suite unless a suitable
					//certificate has been previously loaded by the user
					res = RES_TLS_NO_CERTIFICATE;

					//Loop through the list of available certificates
					for (j = 0; j < numCerts; j++)
					{
						//Check whether the current certificate is acceptable
						acceptable = tlsIsCertificateAcceptable(&certs[j],
								&certType, 1, supportedSignAlgos, curveList,
								nullptr);

						//Is the certificate suitable for the selected cipher suite?
						if (acceptable)
						{
							//The hash algorithm to be used when generating signatures must be
							//one of those present in the SignatureAlgorithms extension
							res = tlsSelectSignHashAlgo(certs[j].signAlgo,
									supportedSignAlgos);

							//If all the requirements were met, the certificate can be
							//used in conjunction with the selected cipher suite
							if (res == RES_OK)
							{
								cert = &certs[j];
								break;
							}
						}
					}
				}
			}

			//If the list contains cipher suites the server does not recognize,
			//support, or wish to use, the server must ignore those cipher
			//suites, and process the remaining ones as usual
			if (res == RES_OK)
				break;
		}

		//If no acceptable choices are presented, return a handshake failure
		//alert and close the connection
		if (res != RES_OK)
			return RES_TLS_HANDSHAKE_FAILED;

		//The list of the compression methods supported by the client
		//is sorted by client preference
		for (i = 0; i < compressionMethods->length; i++)
		{
			//Check whether the algorithm to be used for data compression is supported
			res = tls_set_compression_method(compressionMethods->value[i]);

			//If the compression method is not supported, process the remaining ones
			if (res == RES_OK)
				break;
		}

		//If no compression algorithm is acceptable, return a handshake failure
		//alert and close the connection
		if (res != RES_OK)
			return RES_TLS_HANDSHAKE_FAILED;
	}

	//Initialize handshake message hashing
	res = tls_handshake_hash_init();
	if (res != RES_OK)
		return res;

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//Prepare to send ServerHello message...
	tls_state = TLS_STATE_SERVER_HELLO;
	//Successful processing
	return RES_OK;
}

RES_CODE tls_context_t::tlsParseClientKeyExchange(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t n;
	const uint8_t *p;
	const tls_client_key_exchange_t* message;

	message = (tls_client_key_exchange_t*)rc->msg_start();

	//Debug message
	TRACE_TLS("ClientKeyExchange message received (%u bytes)...\r\n", rc->msg_len);
	TRACE_TLS_ARRAY("  ", message, rc->msg_len);

	//Check the length of the ClientKeyExchange message
	if (rc->msg_len < sizeof(tls_client_key_exchange_t))
		return RES_TLS_DECODING_FAILED;

	//Check current state
	if (tls_state == TLS_STATE_CLIENT_CERTIFICATE)
	{
		//A an non-anonymous server can optionally request a certificate from the client
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_DSS
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_RSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_ECDSA
				|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
		{
			//If client authentication is required by the server for the handshake
			//to continue, it may respond with a fatal handshake failure alert
			if (client_auth_mode == TLS_CLIENT_AUTH_REQUIRED)
				return RES_TLS_HANDSHAKE_FAILED;
		}
	}
	else if (tls_state != TLS_STATE_CLIENT_KEY_EXCHANGE)
	{
		//Send a fatal alert to the client
		return RES_TLS_UNEXPECTED_MESSAGE;
	}

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//Point to the body of the handshake message
	p = message->data;
	//Remaining bytes to process
	rc->msg_len -= sizeof(tls_client_key_exchange_t);

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//The PSK identity is sent in cleartext
		res = tlsParsePskIdentity(p, rc->msg_len, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Point to the next field
		p += n;
		//Remaining bytes to process
		rc->msg_len -= n;
	}

	//RSA, Diffie-Hellman or ECDH key exchange method?
	if (cipher_info->suite_key_exch != TLS_KEY_EXCH_PSK)
	{
		//Parse client's key exchange parameters
		res = tlsParseClientKeyParams(p, rc->msg_len, &n);
		//Any error to report?
		if (res != RES_OK)
			return res;

		//Point to the next field
		p += n;
		//Remaining bytes to process
		rc->msg_len -= n;
	}

	//If the amount of data in the message does not precisely match the format
	//of the ClientKeyExchange message, then send a fatal alert
	if (rc->msg_len != 0)
		return RES_TLS_DECODING_FAILED;

	//PSK key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_DHE_PSK
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_ECDHE_PSK)
	{
		//Generate premaster secret
		res = tlsGeneratePskPremasterSecret();
		//Any error to report?
		if (res != RES_OK)
			return res;
	}

	//Derive session keys from the premaster secret
	res = tls_generate_keys();
	//Unable to generate key material?
	if (res != RES_OK)
		return res;

	//Update FSM state
	if (peerCertType != TLS_CERT_NONE)
		tls_state = TLS_STATE_CERTIFICATE_VERIFY;
	else
		tls_state = TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC;

	//Successful processing
	return RES_OK;
}

/**
 * @brief Parse CertificateVerify message
 *
 * The CertificateVerify message is used to provide explicit verification
 * of a client certificate. This message is only sent following a client
 * certificate that has signing capability
 *
 * @param[in] context Pointer to the TLS context
 * @param[in] message Incoming CertificateVerify message to parse
 * @param[in] length Message length
 * @return Error code
 **/

RES_CODE tls_context_t::tlsParseCertificateVerify(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t n;
	const tls_certificate_verify_t* message;

	message = (tls_certificate_verify_t*)rc->msg_start();


	//Debug message
	TRACE_TLS("CertificateVerify message received (%u bytes)...\r\n", rc->msg_len);
	TRACE_TLS_ARRAY("  ", message, rc->msg_len);

	//Check the length of the CertificateVerify message
	if (rc->msg_len < sizeof(tls_certificate_verify_t))
		return RES_TLS_DECODING_FAILED;

	//Check current state
	if (tls_state != TLS_STATE_CERTIFICATE_VERIFY)
		return RES_TLS_UNEXPECTED_MESSAGE;

	//Remaining bytes to process
	n = rc->msg_len - sizeof(tls_certificate_verify_t);

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= TLS_VERSION_1_1)
	//SSL 3.0, TLS 1.0 or TLS 1.1 currently selected?
	if (tls_version <= TLS_VER11)
	{
		//Point to the digitally-signed element
		TlsDigitalSignature *signature =
				(TlsDigitalSignature *) message->signature;

		//Check the length of the digitally-signed element
		if (n < sizeof(TlsDigitalSignature))
			return RES_TLS_DECODING_FAILED;
		if (n < (sizeof(TlsDigitalSignature) + __REV16(signature->length)))
			return RES_TLS_DECODING_FAILED;

#if TLS_RSA_SIGN_SUPPORT
		//The client's certificate contains a valid RSA public key?
		if(peerCertType == TLS_CERT_RSA_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello (using MD5)
			res = tlsFinalizeHandshakeHash(handshake_hash.get(), "",
					verifyData);
			//Any error to report?
			if (res != RES_OK)
				return res;

			//Digest all the handshake messages starting at ClientHello (using SHA-1)
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "",
					verifyData + MD5_DIGEST_SIZE);
			//Any error to report?
			if (res != RES_OK)
				return res;

			//Verify RSA signature using client's public key
			res = tlsVerifyRsaSignature(&peerRsaPublicKey, verifyData,
					signature);
		}
		else
#endif
#if TLS_DSA_SIGN_SUPPORT
		//The client's certificate contains a valid DSA public key?
		if (peerCertType == TLS_CERT_DSS_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", verifyData);
			//Any error to report?
			if (res != RES_OK)
				return res;

			//Verify DSA signature using client's public key
			res = tlsVerifyDsaSignature(&peerDsaPublicKey, verifyData,
					SHA1_DIGEST_SIZE, signature);
		}
		else
#endif
#if TLS_ECDSA_SIGN_SUPPORT
		//The client's certificate contains a valid ECDSA public key?
		if(peerCertType == TLS_CERT_ECDSA_SIGN)
		{
			//Digest all the handshake messages starting at ClientHello
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "",
					verifyData);
			//Any error to report?
			if (res != RES_OK)
				return res;

			//Verify ECDSA signature using client's public key
			res = tlsVerifyEcdsaSignature(verifyData,
					SHA1_DIGEST_SIZE, signature);
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
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_2 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
	//TLS 1.2 currently selected?
	if (tls_version == TLS_VER12)
	{
		const hash_info_t* hash_info;

		//Point to the digitally-signed element
		TlsDigitalSignature2* signature = (TlsDigitalSignature2 *)message->signature;
		uint32_t signature_len = __REV16(signature->signature.length);

		//Check the length of the digitally-signed element
		if (n < sizeof(TlsDigitalSignature2))
			return RES_TLS_DECODING_FAILED;
		if (n < (sizeof(TlsDigitalSignature2) + signature_len))
			return RES_TLS_DECODING_FAILED;

		//Retrieve the hash algorithm used for signing
		hash_info = tlsGetHashAlgo(signature->algorithm.hash);

		//Digest all the handshake messages starting at ClientHello
		if (hash_info == SHA1_HASH_ALGO)
		{
			//Use SHA-1 hash algorithm
			res = tlsFinalizeHandshakeHash(handshake_sha1.get(), "", verifyData);
		}
		else if (prf_hash_algo.get() && (hash_info == prf_hash_algo->hash_info))
		{
			//Use PRF hash algorithm (SHA-256 or SHA-384)
			res = tlsFinalizeHandshakeHash(prf_hash_algo.get(), "", verifyData);
		}
		else
		{
			//The specified hash algorithm is not supported
			res = RES_TLS_UNSUPPORTED_SIGNATURE_ALGO;
		}

		//Any error to report?
		if (res != RES_OK)
			return res;

#if TLS_RSA_SIGN_SUPPORT
		//The client's certificate contains a valid RSA public key?
		if(signature->algorithm.signature == TLS_SIGN_ALGO_RSA)
		{
			//Use the signature verification algorithm defined in PKCS #1 v1.5
			res = peerRsaPublicKey.rsassaPkcs1v15Verify(hash_info,
					verifyData, signature->signature.value, signature_len);
		}
		else
#endif
#if TLS_DSA_SIGN_SUPPORT
		//The client's certificate contains a valid DSA public key?
		if (signature->algorithm.signature == TLS_SIGN_ALGO_DSA)
		{
			//Verify DSA signature using client's public key
			res = tlsVerifyDsaSignature(&peerDsaPublicKey, verifyData,
					hash_info->digest_size, &signature->signature);
		}
		else
#endif
#if TLS_ECDSA_SIGN_SUPPORT
		//The client's certificate contains a valid ECDSA public key?
		if(signature->algorithm.signature == TLS_SIGN_ALGO_ECDSA)
		{
			//Verify ECDSA signature using client's public key
			res = tlsVerifyEcdsaSignature(verifyData,
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
#endif
	{
		//The negotiated TLS version is not valid
		res = RES_TLS_INVALID_VERSION;
	}

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//Prepare to receive a ChangeCipherSpec message...
	tls_state = TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC;
	//Return status code
	return res;
}

RES_CODE tls_context_t::tlsParsePskIdentity(const uint8_t* p, size_t length, size_t* consumed)
{
	size_t n;
	TlsPskIdentity* psk_identity;

	//Malformed ClientKeyExchange message?
	if (length < sizeof(TlsPskIdentity))
		return RES_TLS_DECODING_FAILED;

	//Point to the PSK identity
	psk_identity = (TlsPskIdentity *) p;

	//Retrieve the length of the PSK identity
	n = __REV16(psk_identity->length);

	//Make sure the length field is valid
	if (length < (sizeof(TlsPskIdentity) + n))
		return RES_TLS_DECODING_FAILED;

#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
	   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	//Check whether the PSK identity has already been configured
	pskIdentity.clear();

	//Valid PSK identity?
	if (n > 0)
	{
		//Save the PSK identity
		pskIdentity.assign((char*)psk_identity->value, n);
	}

	//Any registered callback?
	if (pskCallback != nullptr)
	{
		RES_CODE res;

		//Invoke user callback function
		if (!pskIdentity.empty())
			res = pskCallback(this, pskIdentity.c_str());
		else
			res = pskCallback(this, "");

		//Any error to report?
		if (res != RES_OK)
			return RES_TLS_UNKNOWN_IDENTITY;
	}
#endif

	//Total number of bytes that have been consumed
	*consumed = sizeof(TlsPskIdentity) + n;

	//Successful processing
	return RES_OK;
}

RES_CODE tls_context_t::tlsParseClientKeyParams(const uint8_t* p, size_t length, size_t* consumed)
{
	RES_CODE res;

#if (TLS_RSA_SUPPORT || TLS_RSA_PSK_SUPPORT)
	//RSA key exchange method?
	if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA
			|| cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA_PSK)
	{
		size_t n;
		uint16_t version;
		RsaPrivateKey privateKey;

		//The RSA-encrypted premaster secret in a ClientKeyExchange is preceded by
		//two length bytes. SSL 3.0 implementations do not include these bytes
		if (tls_version > SSL_VER30)
		{
			//Malformed ClientKeyExchange message?
			if (length < 2)
				return RES_TLS_DECODING_FAILED;

			//Decode the length field
			n = __REV16(*(uint16_t*)(void*)p);

			//Check the length of the RSA-encrypted premaster secret
			if (n > (length - 2))
				return RES_TLS_DECODING_FAILED;

			//Save the length of the RSA-encrypted premaster secret
			length = n;
			//Advance the pointer over the length field
			p += 2;
			//Total number of bytes that have been consumed
			*consumed = length + 2;
		}
		else
		{
			//Total number of bytes that have been consumed
			*consumed = length;
		}

		//Decode the PEM structure that holds the RSA private key
		res = pemReadRsaPrivateKey(cert->privateKey, cert->privateKeyLength,
			&privateKey);

		//Check status code
		if (res == RES_OK)
		{
			//Decrypt the premaster secret using the server private key
			res = privateKey.rsaesPkcs1v15Decrypt(p, length, premasterSecret,
					TLS_MAX_PREMASTER_SECRET_SIZE, &premasterSecretLen);
		}

		//Retrieve the latest version supported by the client. This is used
		//to detect version roll-back attacks
		version = *(uint16_t*)(void*)premasterSecret;

		//The best way to avoid vulnerability to the Bleichenbacher attack is to
		//treat incorrectly formatted messages in a manner indistinguishable from
		//correctly formatted RSA blocks
		if (res != RES_OK || premasterSecretLen != 48
				|| version != client_version)
		{
			//When it receives an incorrectly formatted RSA block, the server
			//should generate a random 48-byte value and proceed using it as
			//the premaster secret
			res = tls_randoms.prng_algo->prng_read(premasterSecret, 48);

			//Fix the length of the premaster secret
			premasterSecretLen = 48;
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
		size_t n;

		//Convert the client's public value to a multiple precision integer
		res = dhContext.yb.tlsReadMpi(p, length, &n);

		//Check status code
		if (res == RES_OK)
		{
			//Total number of bytes that have been consumed
			*consumed = n;

			//Verify client's public value
			res = dhContext.params.dhCheckPublicKey(&dhContext.yb);
		}

		//Check status code
		if (res == RES_OK)
		{
			//Calculate the negotiated key Z
			res = dhContext.dhComputeSharedSecret(premasterSecret,
					TLS_MAX_PREMASTER_SECRET_SIZE, &premasterSecretLen);
		}

		//Check status code
		if (res == RES_OK)
		{
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
				memmove(premasterSecret, premasterSecret + n,
						premasterSecretLen - n);

				//Adjust the length of the premaster secret
				premasterSecretLen -= n;
			}
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

		//Decode client's public key
		res = ecdhContext.tlsReadEcPoint(p, length, &n);

		//Check status code
		if (res == RES_OK)
		{
			//Total number of bytes that have been consumed
			*consumed = n;

			//Verify client's public key and make sure that it is on the same
			//elliptic curve as the server's ECDH key
			res = ecdhContext.ecdhCheckPublicKey();
		}

		//Check status code
		if (res == RES_OK)
		{
			//Calculate the shared secret Z. Leading zeros found in this octet
			//string must not be truncated (see RFC 4492, section 5.10)
			res = ecdhContext.ecdhComputeSharedSecret(premasterSecret,
					TLS_MAX_PREMASTER_SECRET_SIZE, &premasterSecretLen);
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
#endif


