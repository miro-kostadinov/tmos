/*
 * tls_common.cpp
 *
 *  Created on: Jul 13, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <tls.h>
#include <tls_cache.h>
#include <x509.h>
#include <pem.h>
#include <asn1.h>
#include <oid.h>

RES_CODE tls_context_t::tls_parse_change_cipherspec(record_ctxt_t* rc)
{
	RES_CODE res;


	//Check the length of the ChangeCipherSpec message
	if (rc->msg_len < sizeof(tls_change_cipherspec_t))
		return RES_TLS_DECODING_FAILED;

   //TLS operates as a client or a server?
   if(entity == TLS_CONNECTION_END_CLIENT)
   {
	  //Check current state
	  if(tls_state != TLS_STATE_SERVER_CHANGE_CIPHER_SPEC)
		 return RES_TLS_UNEXPECTED_MESSAGE;
   }
   else
   {
	  //Check current state
	  if(tls_state != TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC)
		 return RES_TLS_UNEXPECTED_MESSAGE;
   }

   //Initialize decryption engine
   res = tls_init_decryption_engine();
   if(res == RES_OK)
   {
	   //Inform the record layer that subsequent records will be protected
	   //under the newly negotiated encryption algorithm
	   change_cipher_received = true;

	   //Prepare to receive a Finished message from the peer...
	   if(entity == TLS_CONNECTION_END_CLIENT)
		  tls_state = TLS_STATE_SERVER_FINISHED;
	   else
		  tls_state = TLS_STATE_CLIENT_FINISHED;
   }

   return res;
}

RES_CODE tls_context_t::tls_parse_alert(record_ctxt_t* rc)
{
	const tls_alert_t* message;

	//Check message length
	if (rc->msg_len != sizeof(tls_alert_t))
		return RES_TLS_DECODING_FAILED;

	message = (const tls_alert_t*)rc->msg_start();

	//Alert messages convey the severity of the message
	if (message->level == TLS_ALERT_LEVEL_WARNING)
	{
		//Closure alert received?
		if (message->description == TLS_ALERT_CLOSE_NOTIFY)
		{
			//A closure alert has been received
			close_notify_received = true;

			//Close down the connection immediately
			if (tls_state == TLS_STATE_APPLICATION_DATA)
				tls_state = TLS_STATE_CLOSING;
		}
	}
	else if (message->level == TLS_ALERT_LEVEL_FATAL)
	{
		//A fatal alert message has been received
		fatal_alert_received = true;

		//Any connection terminated with a fatal alert must not be resumed
		if (entity == TLS_CONNECTION_END_SERVER)
			tls_remove_from_cache(this);

		//Servers and clients must forget any session identifiers
		memclr(sessionId, 32);
		sessionIdLen = 0;

		//Alert messages with a level of fatal result in the immediate
		//termination of the connection
		tls_state = TLS_STATE_CLOSED;
	}
	else
	{
		//Report an error
		return RES_TLS_INVALID_PARAMETER;
	}

	//Successful processing
	return RES_OK;
}

#define LOAD24BE(p) ( ((p[0]) << 16) |  ((p[1]) << 8) |  ((p[2]) << 0) )

RES_CODE tls_context_t::tls_parse_certificate(record_ctxt_t* rc)
{
	RES_CODE res;
	const uint8_t *p;
	size_t n;
	const char *pemCert;
	size_t pemCertLength;
	uint8_t *derCert;
	size_t derCertSize;
	size_t derCertLength;
	const tls_certificate_msg_t *message;
	auto_ptr<X509CertificateInfo> certInfo;
	auto_ptr<X509CertificateInfo> issuerCertInfo;


	message = (const tls_certificate_msg_t*)rc->msg_start();

	//Check the length of the Certificate message
	if (rc->msg_len < sizeof(tls_certificate_msg_t))
		return RES_TLS_DECODING_FAILED;

	//TLS operates as a client or a server?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//Check current state
		if (tls_state != TLS_STATE_SERVER_CERTIFICATE)
			return RES_TLS_UNEXPECTED_MESSAGE;
	}
	else
	{
		//Check current state
		if (tls_state != TLS_STATE_CLIENT_CERTIFICATE)
			return RES_TLS_UNEXPECTED_MESSAGE;
	}

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//Get the size occupied by the certificate list
	n = LOAD24BE(message->certificate_list_Length);
	//Remaining bytes to process
	rc->msg_len -= sizeof(tls_certificate_msg_t);

	//Ensure that the chain of certificates is valid
	if (n > rc->msg_len)
		return RES_TLS_DECODING_FAILED;

	//Compute the length of the certificate list
	rc->msg_len = n;

	//The sender's certificate must come first in the list
	p = message->certificate_list;

	//Start of exception handling block
	do
	{
		//Assume an error...
		res = RES_OUT_OF_MEMORY;

		//Allocate a memory buffer to store X.509 certificate info
		certInfo = new X509CertificateInfo;
		if (certInfo.get() == nullptr)
			break;

		//Allocate a memory buffer to store the parent certificate
		issuerCertInfo = new X509CertificateInfo;
		if (issuerCertInfo.get() == nullptr)
			break;

		//TLS operates as a server?
		if (entity == TLS_CONNECTION_END_SERVER)
		{
			//Empty certificate list?
			if (!rc->msg_len)
			{
				//Check whether mutual authentication is required
				if (client_auth_mode == TLS_CLIENT_AUTH_REQUIRED)
				{
					//If client authentication is required by the server for the handshake
					//to continue, it may respond with a fatal handshake failure alert
					res = RES_TLS_HANDSHAKE_FAILED;
					break;
				}
				else
				{
					//Client authentication is optional
					peerCertType = TLS_CERT_NONE;
					res = RES_OK;
					break;
				}
			}
		}

		//Each certificate is preceded by a 3-byte length field
		if (rc->msg_len < 3)
		{
			//Report an error
			res = RES_TLS_DECODING_FAILED;
			break;
		}

		//Get the size occupied by the certificate
		n = LOAD24BE(p);
		//Jump to the beginning of the DER encoded certificate
		p += 3;
		rc->msg_len -= 3;

		//Make sure that the certificate is valid
		if (n > rc->msg_len)
		{
			//Report an error
			res = RES_TLS_DECODING_FAILED;
			break;
		}

		//Display ASN.1 structure
		res = asn1DumpObject(p, n, 0);
		//Any error to report?
		if (res != RES_OK)
			break;

		//Parse X.509 certificate
		res = certInfo->x509ParseCertificate(p, n);
		//Failed to parse the X.509 certificate?
		if (res != RES_OK)
			break;

#if TLS_CLIENT_SUPPORT
		//TLS operates as a client?
		if (entity == TLS_CONNECTION_END_CLIENT)
		{
			//Check if the hostname must be verified
			if (!server_name.empty())
			{
				int32_t i;
				int32_t j;

				//Point to the last character of the common name
				i = certInfo->subject.commonNameLen - 1;
				//Point to the last character of the hostname
				j = server_name.length() - 1;

				//Check the common name in the server certificate against
				//the actual hostname that is being requested
				while (i >= 0 && j >= 0)
				{
					//Wildcard certificate found?
					if (certInfo->subject.commonName[i] == '*' && i == 0)
					{
						//The CN is acceptable
						j = 0;
					}
					//Perform case insensitive character comparison
					else if (tolower((uint8_t) certInfo->subject.commonName[i])
							!= server_name[j])
					{
						break;
					}

					//Compare previous characters
					i--;
					j--;
				}

				//If the host names do not match, reject the certificate
				if (i >= 0 || j >= 0)
				{
					//Debug message
					TRACE_WARNING("Server name mismatch!\r\n");
					//Report an error
					res = RES_TLS_BAD_CERTIFICATE;
					break;
				}
			}
		}
#endif // TLS_CLIENT_SUPPORT

#if TLS_RSA_SIGN_SUPPORT || TLS_RSA_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
		//The certificate contains a valid RSA public key?
		if(!oidComp(certInfo->subjectPublicKeyInfo.oid, certInfo->subjectPublicKeyInfo.oidLen,
						RSA_ENCRYPTION_OID, sizeof(RSA_ENCRYPTION_OID)))
		{
			uint32_t k;

			//Retrieve the RSA public key
			res = certInfo->x509ReadRsaPublicKey(&peerRsaPublicKey);
			if(res != RES_OK)
				break;

			//Get the length of the modulus, in bits
			k = peerRsaPublicKey.n.mpiGetBitLength();

			//Make sure the modulus is acceptable
			if(k < TLS_MIN_RSA_MODULUS_SIZE || k > TLS_MAX_RSA_MODULUS_SIZE)
			{
				//Report an error
				res = RES_TLS_BAD_CERTIFICATE;
				break;
			}

			//Save the certificate type
			peerCertType = TLS_CERT_RSA_SIGN;
		}
		else
#endif
#if TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT
		//The certificate contains a valid DSA public key?
		if (!oidComp(certInfo->subjectPublicKeyInfo.oid,
				certInfo->subjectPublicKeyInfo.oidLen, DSA_OID,
				sizeof(DSA_OID)))
		{
			uint32_t k;

			//Retrieve the DSA public key
			res = certInfo->x509ReadDsaPublicKey(&peerDsaPublicKey);
			if (res != RES_OK)
				break;

			//Get the length of the prime modulus, in bits
			k = peerDsaPublicKey.p.mpiGetBitLength();

			//Make sure the prime modulus is acceptable
			if (k < TLS_MIN_DSA_MODULUS_SIZE || k > TLS_MAX_DSA_MODULUS_SIZE)
			{
				//Report an error
				res = RES_TLS_BAD_CERTIFICATE;
				break;
			}

			//Save the certificate type
			peerCertType = TLS_CERT_DSS_SIGN;
		}
		else
#endif
#if TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT
		//The certificate contains a valid EC public key?
		if(!oidComp(certInfo->subjectPublicKeyInfo.oid, certInfo->subjectPublicKeyInfo.oidLen,
						EC_PUBLIC_KEY_OID, sizeof(EC_PUBLIC_KEY_OID)))
		{
			const EcCurveInfo *curveInfo;

			//Retrieve EC domain parameters
			curveInfo = ecGetCurveInfo(certInfo->subjectPublicKeyInfo.ecParams.namedCurve,
					certInfo->subjectPublicKeyInfo.ecParams.namedCurveLen);

			//Make sure the specified elliptic curve is supported
			if(curveInfo == NULL)
			{
				//Report an error
				res = RES_TLS_BAD_CERTIFICATE;
				//Exit immediately
				break;
			}

			//Load EC domain parameters
			res = peerEcParams.ecLoadDomainParameters(curveInfo);
			if(res != RES_OK)
				break;

			//Retrieve the EC public key
			res = peerEcParams.ecImport(&peerEcPublicKey,
					certInfo->subjectPublicKeyInfo.ecPublicKey.q,
					certInfo->subjectPublicKeyInfo.ecPublicKey.qLen);
			if(res != RES_OK)
				break;

			//Save the certificate type
			peerCertType = TLS_CERT_ECDSA_SIGN;
		}
		else
#endif
		//The certificate does not contain any valid public key?
		{
			//Report an error
			res = RES_TLS_BAD_CERTIFICATE;
			break;
		}

		//Next certificate
		p += n;
		rc->msg_len -= n;

		//PKIX path validation
		while (rc->msg_len > 0)
		{
			//Each certificate is preceded by a 3-byte length field
			if (rc->msg_len < 3)
			{
				//Report an error
				res = RES_TLS_DECODING_FAILED;
				break;
			}

			//Get the size occupied by the certificate
			n = LOAD24BE(p);
			//Jump to the beginning of the DER encoded certificate
			p += 3;
			rc->msg_len -= 3;

			//Ensure that the certificate is valid
			if (n > rc->msg_len)
			{
				//Report an error
				res = RES_TLS_DECODING_FAILED;
				break;
			}

			//Display ASN.1 structure
			res = asn1DumpObject(p, n, 0);
			if (res != RES_OK)
				break;

			//Parse X.509 certificate
			res = issuerCertInfo->x509ParseCertificate(p, n);
			if (res != RES_OK)
				break;

			//Valid trusted CA list?
			if (trustedCaListLen > 0)
			{
				//Validate current certificate
				res = certInfo->x509ValidateCertificate(issuerCertInfo.get());
				if (res != RES_OK)
					break;
			}

			//Keep track of the issuer certificate
			memcpy(certInfo.get(), issuerCertInfo.get(), sizeof(X509CertificateInfo));

			//Next certificate
			p += n;
			rc->msg_len -= n;
		}

		//Propagate exception if necessary...
		if (res != RES_OK)
			break;

		//Point to the first trusted CA certificate
		pemCert = trustedCaList;
		//Get the total length, in bytes, of the trusted CA list
		pemCertLength = trustedCaListLen;

		//DER encoded certificate
		derCert = NULL;
		derCertSize = 0;
		derCertLength = 0;

		//Loop through the list
		while (pemCertLength > 0)
		{
			//Decode PEM certificate
			res = pemReadCertificate(&pemCert, &pemCertLength, &derCert,
					&derCertSize, &derCertLength);
			if (res != RES_OK)
				break;

			//Parse X.509 certificate
			res = issuerCertInfo->x509ParseCertificate(derCert, derCertLength);
			if (res != RES_OK)
				break;

			//Validate the certificate with the current trusted CA
			res = certInfo->x509ValidateCertificate(issuerCertInfo.get());
			if (res == RES_OK)
				break;
		}

		//The certificate could not be matched with a known, trusted CA?
		if (res == RES_EOF)
			res = RES_TLS_UNKNOWN_CA;

		//Free previously allocated memory
		tsk_free(derCert);

		//End of exception handling block
	} while (0);

	//Clean up side effects
	if (res != RES_OK)
	{
#if TLS_RSA_SIGN_SUPPORT || TLS_RSA_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
		//Release peer's RSA public key
		peerRsaPublicKey.rspk_free();
#endif
#if (TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT)
		//Release peer's DSA public key
		peerDsaPublicKey.dsapk_free();
#endif
#if TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT
		//Release peer's EC domain parameters
		peerEcParams.ecdp_free();
		//Release peer's EC public key
		peerEcPublicKey.ecp_free();
#endif
	}

	//TLS operates as a client or a server?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//Update FSM state
		if (cipher_info->suite_key_exch == TLS_KEY_EXCH_RSA)
			tls_state = TLS_STATE_CERTIFICATE_REQUEST;
		else
			tls_state = TLS_STATE_SERVER_KEY_EXCHANGE;
	}
	else
	{
		//Prepare to receive ClientKeyExchange message...
		tls_state = TLS_STATE_CLIENT_KEY_EXCHANGE;
	}

	//Return status code
	return res;
}

/**
 * @brief Parse Finished message
 * @param[in] context Pointer to the TLS context
 * @param[in] message Incoming Finished message to parse
 * @param[in] length Message length
 * @return Error code
 **/
RES_CODE tls_context_t::tlsParseFinished(record_ctxt_t* rc)
{
	RES_CODE res;
	const tls_finished_t* message = (tls_finished_t*)rc->msg_start();

	//Debug message
	TRACE_TLS("Finished message received (%u bytes)...\r\n", rc->msg_len);
	TRACE_TLS_ARRAY("  ", message, rc->msg_len);

	//Check the length of the Finished message
	if (rc->msg_len < sizeof(tls_finished_t))
		return RES_TLS_DECODING_FAILED;

	//TLS operates as a client or a server?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//Check current state
		if (tls_state != TLS_STATE_SERVER_FINISHED)
			return RES_TLS_UNEXPECTED_MESSAGE;

		//The verify data is generated from all messages in this
		//handshake up to but not including the Finished message
		res = tlsComputeVerifyData(TLS_CONNECTION_END_SERVER);
	}
	else
	{
		//Check current state
		if (tls_state != TLS_STATE_CLIENT_FINISHED)
			return RES_TLS_UNEXPECTED_MESSAGE;

		//The verify data is generated from all messages in this
		//handshake up to but not including the Finished message
		res = tlsComputeVerifyData(TLS_CONNECTION_END_CLIENT);
	}

	//Unable to generate the verify data?
	if (res != RES_OK)
		return res;

	//Check message length
	if (message->header.get() != verify_data_len)
		return RES_TLS_DECODING_FAILED;

	//Check the resulting verify data
	if (memcmp(message->verifyData, verifyData,	verify_data_len))
		return RES_TLS_INVALID_SIGNATURE;

	//Update the hash value with the incoming handshake message
	tls_handshake_hash_update(message, rc->msg_len);

	//TLS operates as a client or a server?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//Use abbreviated or full handshake?
		if (tls_resume)
			tls_state = TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC;
		else
			tls_state = TLS_STATE_APPLICATION_DATA;
	}
	else
	{
		//Use abbreviated or full handshake?
		if (tls_resume)
			tls_state = TLS_STATE_APPLICATION_DATA;
		else
			tls_state = TLS_STATE_SERVER_CHANGE_CIPHER_SPEC;
	}

	//Successful processing
	return RES_OK;
}

/**
 * @brief Send Certificate message
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendCertificate()
{
	RES_CODE res = RES_OK;

#if TLS_CLIENT_SUPPORT
	//TLS operates as a client?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//The client must send a Certificate message if the server requests it
		if (clientCertRequested)
		{
#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
			//No suitable certificate available?
			if(cert == nullptr && tls_version == SSL_VER30)
			{
				//The client should send a no_certificate alert instead
				last_txrc.msg_len = tls_get_alert_len();
				res = tls_record_get_lens(&last_txrc);

				if(res == RES_OK)
				{
					tls_alert_t* message;

					last_txrc.tls_record.rec_type = TLS_TYPE_ALERT;
					message = (tls_alert_t*)last_txrc.msg_start();
					message->level = TLS_ALERT_LEVEL_WARNING;
					message->description = TLS_ALERT_NO_CERTIFICATE;

					TRACE_TLS("Sending Alert message (%u bytes)...\r\n", last_txrc.msg_len);
					TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

					//Send Alert message
					res = tls_record_write(&last_txrc);
				}
			}
			else
#endif
			{
				//Format Certificate message
				res = tls_certificate_msg_len(&last_txrc);
				if(res == RES_OK)
					res = tls_record_get_lens(&last_txrc);

				// Prepare record
				if(res == RES_OK)
				{
					tls_certificate_msg_t* message;

					last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
					message = (tls_certificate_msg_t*)last_txrc.msg_start();
					res = tls_make_certificate_msg(message, last_txrc.msg_len);
					if (res == RES_OK)
					{
						//Debug message
						TRACE_TLS("Sending Certificate message (%u bytes)...\r\n", last_txrc.msg_len);
						TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

						tls_handshake_hash_update(message, last_txrc.msg_len);

						res = tls_record_write(&last_txrc);
					}

				}
			}
		}
	}
	else
#endif // TLS_CLIENT_SUPPORT
#if TLS_SERVER_SUPPORT
	//TLS operates as a server?
	if (entity == TLS_CONNECTION_END_SERVER)
	{
		//The server must send a Certificate message whenever the agreed-upon
		//key exchange method uses certificates for authentication
		if (cert != nullptr)
		{
			//Format Certificate message
			res = tls_certificate_msg_len(&last_txrc);
			if(res == RES_OK)
				res = tls_record_get_lens(&last_txrc);

			// Prepare record
			if(res == RES_OK)
			{
				tls_certificate_msg_t* message;

				last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
				message = (tls_certificate_msg_t*)last_txrc.msg_start();
				res = tls_make_certificate_msg(message, last_txrc.msg_len);
				if (res == RES_OK)
				{
					//Debug message
					TRACE_TLS("Sending Certificate message (%u bytes)...\r\n", last_txrc.msg_len);
					TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

					tls_handshake_hash_update(message, last_txrc.msg_len);

					res = tls_record_write(&last_txrc);
				}
			}
		}
	}
	else
#endif
	//Unsupported mode of operation?
	{
		//Report an error
		res = RES_TLS_FAIL;
	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//Update FSM state
		if (entity == TLS_CONNECTION_END_CLIENT)
			tls_state = TLS_STATE_CLIENT_KEY_EXCHANGE;
		else
			tls_state = TLS_STATE_SERVER_KEY_EXCHANGE;
	}

	return res;
}

uint32_t tls_context_t::tls_get_alert_len()
{
	uint32_t len;

	len = sizeof(tls_alert_t);

	return len;
}

RES_CODE tls_context_t::tls_certificate_msg_len(record_ctxt_t* rc)
{
	uint32_t len;

	len = 0;
	if (cert != NULL)
	{
		const char* pemCert;
		size_t pemCertLength;
		uint8_t* derCert;
		size_t derCertSize;
		size_t derCertLength;

		//Point to the certificate chain
		pemCert = cert->certChain;
		//Get the total length, in bytes, of the certificate chain
		pemCertLength = cert->certChainLength;

		//DER encoded certificate
		derCert = nullptr;
		derCertSize = 0;
		derCertLength = 0;

		//Parse the certificate chain
		while (pemCertLength > 0)
		{
			//Decode PEM certificate
			if(pemReadCertificate(&pemCert, &pemCertLength, &derCert,
					&derCertSize, &derCertLength) != RES_OK)
				break;

			//Total length of the certificate list
			len += derCertLength + 3;

		}
	   //Free previously allocated memory
		tsk_free(derCert);
	}

	len += 3 + sizeof(tls_handshake_t);
	rc->msg_len = len;
	return RES_OK;
}


RES_CODE tls_context_t::tls_make_certificate_msg(tls_certificate_msg_t* message, uint32_t len)
{
	RES_CODE res;
	uint8_t *p;

	//Initialize status code
	res = RES_OK;

	//Handshake message type
	//Length of the complete handshake message
	len -= sizeof(tls_handshake_t);
	message->header.set(TLS_TYPE_CERTIFICATE, len);

	//Consider the 3-byte length field
	len -= 3;

	//A 3-byte length field shall precede the certificate list
	message->certificate_list_Length[0] = len >> 16;
	message->certificate_list_Length[1] = len >> 8;
	message->certificate_list_Length[2] = len;


	//Point to the first certificate of the list
	p = message->certificate_list;

	//Check whether a certificate is available
	if (cert != nullptr)
	{
		const char *pemCert;
		size_t pemCertLength;
		uint8_t *derCert;
		size_t derCertSize;
		size_t derCertLength;

		//Point to the certificate chain
		pemCert = cert->certChain;
		//Get the total length, in bytes, of the certificate chain
		pemCertLength = cert->certChainLength;

		//DER encoded certificate
		derCert = NULL;
		derCertSize = 0;
		derCertLength = 0;

		//Parse the certificate chain
		while (pemCertLength > 0)
		{
			//Decode PEM certificate
			res = pemReadCertificate(&pemCert, &pemCertLength, &derCert,
					&derCertSize, &derCertLength);

			//Any error to report?
			if (res != RES_OK)
			{
				//End of file detected
				res = RES_OK;
				break;
			}

			//Each certificate is preceded by a 3-byte length field
			p[0] = derCertLength >> 16;
			p[1] = derCertLength >> 8;
			p[2] = derCertLength;

			//Copy the current certificate
			memcpy(p + 3, derCert, derCertLength);

			//Advance data pointer
			p += derCertLength + 3;
		}

		//Free previously allocated memory
		tsk_free(derCert);

	}


	return res;
}

/**
 * @brief Send ChangeCipherSpec message
 *
 * The change cipher spec message is sent by both the client and the
 * server to notify the receiving party that subsequent records will be
 * protected under the newly negotiated CipherSpec and keys
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendChangeCipherSpec()
{
	RES_CODE res;

	   //The ChangeCipherSpec message consists of a single byte of value 1
	last_txrc.msg_len = sizeof(tls_change_cipherspec_t);
	res = tls_record_get_lens(&last_txrc);

	// Prepare record
	if(res == RES_OK)
	{
		tls_change_cipherspec_t* message;

		//Point to the buffer where to format the message
		message = (tls_change_cipherspec_t*) last_txrc.msg_start();

		//Format ChangeCipherSpec message
		message->type = 1;

		//Debug message
		TRACE1_TLS("Sending ChangeCipherSpec message (1 bytes)...\r\n");
		TRACE_TLS_ARRAY("  ", message, 1);

		//Send ChangeCipherSpec message
		last_txrc.tls_record.rec_type = TLS_TYPE_CHANGE_CIPHER_SPEC;

		res = tls_record_write(&last_txrc);
	}


	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//Initialize encryption engine
		res = tlsInitEncryptionEngine();
	}

	//Check status code
	if (res == RES_OK)
	{
		//Inform the record layer that subsequent records will be protected
		//under the newly negotiated encryption algorithm
		change_cipher_sent = true;

		//Prepare to send a Finished message to the peer...
		if (entity == TLS_CONNECTION_END_CLIENT)
			tls_state = TLS_STATE_CLIENT_FINISHED;
		else
			tls_state = TLS_STATE_SERVER_FINISHED;
	}

	//Return status code
	return res;
}

/**
 * @brief Send Finished message
 *
 * A Finished message is always sent immediately after a change
 * cipher spec message to verify that the key exchange and
 * authentication processes were successful. It is essential that a
 * change cipher spec message be received between the other handshake
 * messages and the Finished message
 *
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendFinished()
{
	RES_CODE res;
	tls_finished_t* message;


	//The verify data is generated from all messages in this handshake
	//up to but not including the Finished message
	res = tlsComputeVerifyData(entity);

	//Check status code
	if (res == RES_OK)
	{
		//Format Finished message
		last_txrc.msg_len = verify_data_len + sizeof(tls_handshake_t);
		res = tls_record_get_lens(&last_txrc);
		if (res == RES_OK)
		{
			message = (tls_finished_t *) last_txrc.msg_start();

			message->header.set(TLS_TYPE_FINISHED, verify_data_len);

			//Copy the verify data
			memcpy(message->verifyData, verifyData, verify_data_len);
		}
	}

	//Check status code
	if (res == RES_OK)
	{
		//Debug message
		TRACE_TLS("Sending Finished message (%u bytes)...\r\n", last_txrc.msg_len);
		TRACE_TLS_ARRAY("  ", message, last_txrc.msg_len);

		//Send handshake message
		last_txrc.tls_record.rec_type = TLS_TYPE_HANDSHAKE;
		tls_handshake_hash_update(message, last_txrc.msg_len);

		res = tls_record_write(&last_txrc);
	}

	//Check status code
	if (res == RES_OK || res == RES_IDLE)
	{
		//TLS operates as a client or a server?
		if (entity == TLS_CONNECTION_END_CLIENT)
		{
			//Use abbreviated or full handshake?
			if (tls_resume)
				tls_state = TLS_STATE_APPLICATION_DATA;
			else
				tls_state = TLS_STATE_SERVER_CHANGE_CIPHER_SPEC;
		}
		else
		{
			//Use abbreviated or full handshake?
			if (tls_resume)
				tls_state = TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC;
			else
				tls_state = TLS_STATE_APPLICATION_DATA;
		}
	}

	//Return status code
	return res;
}

/**
 * @brief Send Alert message
 * @param[in] context Pointer to the TLS context
 * @param[in] level Severity of the message (warning or fatal)
 * @param[in] description Description of the alert
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSendAlert(tls_alert_level_t level, tls_alert_description_t description)
{
	RES_CODE res;
	tls_alert_t* message;

	last_txrc.msg_len = sizeof(tls_alert_t);
	res = tls_record_get_lens(&last_txrc);
	if (res == RES_OK)
	{
		//Point to the buffer where to format the message
		message = (tls_alert_t*) last_txrc.msg_start();

		//Format Alert message
		message->level = level;
		message->description = description;

		//Debug message
		TRACE_TLS("Sending Alert message (%u bytes)...\r\n", sizeof(tls_alert_t));
		TRACE_TLS_ARRAY("  ", message, sizeof(tls_alert_t));

		//Send Alert message
		last_txrc.tls_record.rec_type = TLS_TYPE_ALERT;
		res = tls_record_write(&last_txrc);

		//Alert messages convey the severity of the message
		if (level == TLS_ALERT_LEVEL_WARNING)
		{
			//If an alert with a level of warning is sent, generally the
			//connection can continue normally
			if (description == TLS_ALERT_CLOSE_NOTIFY)
			{
				//Either party may initiate a close by sending a close_notify alert
				close_notify_sent = true;

				//Update FSM state
				tls_state = TLS_STATE_CLOSING;
			}
		}
		else if (level == TLS_ALERT_LEVEL_FATAL)
		{
			//Alert messages with a level of fatal result in the immediate
			//termination of the connection
			fatal_alert_sent = true;

			//Any connection terminated with a fatal alert must not be resumed
			if (entity == TLS_CONNECTION_END_SERVER)
				tls_remove_from_cache(this);

			//Servers and clients must forget any session identifiers
			memset(sessionId, 0, 32);
			sessionIdLen = 0;

			//Update FSM state
			tls_state = TLS_STATE_CLOSING;
		}
	}

	//Return status code
	return res;
}

/**
 * @brief Perform TLS handshake
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tlsHandshake()
{
	RES_CODE res;

#if TLS_CLIENT_SUPPORT
	//TLS operates as a client?
	if (entity == TLS_CONNECTION_END_CLIENT)
	{
		//Initiate TLS handshake with the remote server
		res = tls_clent_handshake();
	}
	else
#endif
#if TLS_SERVER_SUPPORT
	//TLS operates as a server?
	if (entity == TLS_CONNECTION_END_SERVER)
	{
		//Initiate TLS handshake with the remote client
		res = tlsServerHandshake();
	}
	else
#endif
	//Unsupported mode of operation?
	{
		//Cannot establish a secure session between the server and the client
		res = RES_TLS_INVALID_PARAMETER;
	}

	//Return status code
	return res;
}

void tls_dump_record(const record_ctxt_t* rc)
{
	const tls_record_t* rec = &rc->tls_record;

	TRACELN1_TLS("------ TLS ---------");
	TRACELN_TLS("type=%u ver=%x len=%u", rec->rec_type, rec->rec_version, __REV16(rec->rec_length));
	switch(rec->rec_type)
	{
	case TLS_TYPE_CHANGE_CIPHER_SPEC:
		TRACELN1_TLS("CHANGE_CIPHER_SPEC");
		{
			const tls_change_cipherspec_t* message;

			//Point to the buffer where to format the message
			message = (tls_change_cipherspec_t*) rc->msg_start();
			TRACELN_TLS("type = %u", message->type);
		}
		break;

	case TLS_TYPE_ALERT:
		TRACELN1_TLS("ALERT");
		{
			const tls_alert_t* message;

			message = (tls_alert_t*) rc->msg_start();
			TRACELN_TLS("level=%u description=%u", message->level, message->description);

		}
		break;

	case TLS_TYPE_HANDSHAKE:
		TRACELN1_TLS("HANDSHAKE");
		{
			const tls_handshake_t* header;

			header = (tls_handshake_t*) rc->msg_start();
			TRACELN_TLS("header msgtype=%u len=%u", header->msgType, header->get());
			switch(header->msgType)
			{
			   case TLS_TYPE_HELLO_REQUEST        :
					TRACELN1_TLS("HELLO_REQUEST");
					break;

			   case TLS_TYPE_CLIENT_HELLO         :
					TRACELN1_TLS("CLIENT_HELLO");
					{
						const tls_client_hello_t* message;

						message = (tls_client_hello_t*)rc->msg_start();
						TRACELN_TLS("cver=%x, slen=%u", message->clientVersion, message->sessionIdLen);
						if (rc->msg_len < sizeof(tls_client_hello_t))
						{
							TRACELN1_TLS("bad len");
						} else
						{
							const uint8_t *p;
							size_t n, k;

							p = message->sessionId + message->sessionIdLen;
							n = rc->msg_len - sizeof(tls_client_hello_t);
							if (message->sessionIdLen > n || message->sessionIdLen > 32)
							{
								TRACELN1_TLS("bad len");
							} else
							{
								n -= message->sessionIdLen;

								//Malformed ClientHello message?
								if (n < sizeof(tls_cipher_suites_t))
									TRACELN1_TLS("bad len");
								else
								{
									const tls_cipher_suites_t* cipherSuites;

									cipherSuites = (tls_cipher_suites_t *) p;
									//Remaining bytes to process
									n -= sizeof(tls_cipher_suites_t);

									k = __REV16(cipherSuites->length);
									if (k < 2 || k > n)
										TRACELN1_TLS("bad chipsuite");
									else
									{
										//Point to the next field
										p += sizeof(tls_cipher_suites_t) + k;
										//Remaining bytes to process
										n -= k;

										//Get the number of cipher suite identifiers present in the list
										k  /= sizeof(tls_suite_id_t);

										//Debug message
										TRACELN1_TLS("Cipher suites:\r\n");

										//Dump the list of cipher suites
										for (uint32_t i = 0; i < k; i++)
										{
											//Debug message
											TRACE_TLS("  0x%04x (%s)\r\n", __REV16(cipherSuites->value[i]),
													tlsGetCipherSuiteName(__REV16(cipherSuites->value[i])));
											tsk_sleep(3);
										}

										//Malformed ClientHello message?
										if (n < sizeof(tls_compression_methods_t))
											TRACELN1_TLS("bad compression");
										else
										{
											//Check the length of the list
											k = ((tls_compression_methods_t *)p)->length;
											if (k < 1 || k > n)
												TRACELN1_TLS("bad compression");
											else
											{
												tls_extensions_t* extensions;

												//Remaining bytes to process
												n -= sizeof(tls_compression_methods_t) +k;
												//Point to the next field
												p += sizeof(tls_compression_methods_t) + k;

												extensions = (tls_extensions_t*)p;

												k = __REV16(extensions->length);
												TRACELN_TLS("extensions len=%u", k);
												if(k)
												{
													if((k + sizeof(tls_extensions_t)) > n || k < sizeof(tls_extensions_t))
														TRACE1_TLS(" bad ext list!!");
													else
													{
														while(k)
														{
															n = __REV16(extensions->value[0].length);
															TRACELN_TLS("ext len=%u type=%u", n, __REV16(extensions->value[0].type));

															n += sizeof(tls_extension_t);
															if( n > k )
															{
																TRACE1_TLS(" bad ext!!");
																break;
															}
															else
															{
																p += n;
																k -= n;
																extensions = (tls_extensions_t*)p;
															}
														}

													}
												}


											}

										}


									}


								}

							}
						}

					}
					break;

			   case TLS_TYPE_SERVER_HELLO         :
					TRACELN1_TLS("SERVER_HELLO");
					break;

			   case TLS_TYPE_HELLO_VERIFY_REQUEST :
					TRACELN1_TLS("VERIFY_REQUEST");
					break;

			   case TLS_TYPE_NEW_SESSION_TICKET   :
					TRACELN1_TLS("SESSION_TICKET");
					break;

			   case TLS_TYPE_CERTIFICATE          :
					TRACELN1_TLS("CERTIFICATE");
					break;

			   case TLS_TYPE_SERVER_KEY_EXCHANGE  :
					TRACELN1_TLS("KEY_EXCHANGE");
					break;

			   case TLS_TYPE_CERTIFICATE_REQUEST  :
					TRACELN1_TLS("CERTIFICATE_REQUEST");
					break;

			   case TLS_TYPE_SERVER_HELLO_DONE    :
					TRACELN1_TLS("HELLO_DONE");
					break;

			   case TLS_TYPE_CERTIFICATE_VERIFY   :
					TRACELN1_TLS("CERTIFICATE_VERIFY");
					break;

			   case TLS_TYPE_CLIENT_KEY_EXCHANGE  :
					TRACELN1_TLS("CLIENT_KEY_EXCHANGE");
					break;

			   case TLS_TYPE_FINISHED             :
					TRACELN1_TLS("FINISHED");
					break;

			   case TLS_TYPE_CERTIFICATE_URL      :
					TRACELN1_TLS("CERTIFICATE_URL");
					break;

			   case TLS_TYPE_CERTIFICATE_STATUS   :
					TRACELN1_TLS("CERTIFICATE_STATUS");
					break;

			   case TLS_TYPE_SUPPLEMENTAL_DATA    :
					TRACELN1_TLS("SUPPLEMENTAL_DATA");
					break;

			   default:
					TRACELN1_TLS("UNKNOWN msgType!!!");
					break;


			}
		}
		break;

	case TLS_TYPE_APPLICATION_DATA:
		TRACELN1_TLS("APPLICATION_DATA");
		break;

	default:
		TRACELN1_TLS("UNKNOWN recType!!!");
		break;

	}
	TRACELN1_TLS("--------------------");
}
