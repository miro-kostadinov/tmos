/*
 * tls.cpp
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */


#include <tmos.h>
#include <tls.h>



//------------------------------------------------------------------------------
//	tls_randoms_t
//------------------------------------------------------------------------------
RES_CODE tls_randoms_t::tls_random_generate(tls_random_t& random)
{
	RES_CODE res;
	uint32_t time;

	if (prng_algo.get())
	{
		time = get_gmt_seconds();

		if (time != 0)
		{
			random.gmt_unix_time = __REV(time);

			res = prng_algo->prng_read(random.random_bytes, 28);
		}
		else
		{
			res = prng_algo->prng_read(&random, 32);
		}

	}
	else
		res = RES_TLS_NOT_CONFIGURED;

	return res;
}


void tls_seq_num_t::increment()
{
	for (int i = 7; i >= 0; i--)
	{
		if (++data[i] != 0)
			break;
	}
}


tls_context_t::tls_context_t()
{
	tls_socket = nullptr;
	tls_state = TLS_STATE_INIT;
    entity = TLS_CONNECTION_END_CLIENT;
	cipherSuite = TLS_NULL_WITH_NULL_NULL;
	compressionMethod = TLS_COMPRESSION_METHOD_NULL;
	named_curve = TLS_EC_CURVE_NONE;
    sessionIdLen = 0;
	client_version = (tls_version_t)REV16(TLS_MIN_VERSION);
	tls_version = (tls_version_t)REV16(TLS_MIN_VERSION);
	num_ciphers = 0;
	cipher_suites = nullptr;
	change_cipher_sent = false;
	change_cipher_received = false;
	tls_resume = false;
	fatal_alert_sent = false;
	fatal_alert_received = false;
	close_notify_sent = false;
	close_notify_received = false;
	ecPointFormatExtFound = false;

	cipher_info = nullptr;

#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	psk = nullptr;
	pskLen = 0;
	pskCallback = nullptr;
#endif

	numCerts = 0;
	cert = nullptr;

	signHashAlgo = TLS_HASH_ALGO_NONE;
	peerCertType = TLS_CERT_NONE;
	client_auth_mode = TLS_CLIENT_AUTH_NONE;
	clientCertRequested = false;
	verify_data_len = 0;
	premasterSecretLen = 0;

#if TLS_ECC_CALLBACK_SUPPORT
   ecdhCallback = nullptr;
   ecdsaSignCallback = nullptr;
   ecdsaVerifyCallback = nullptr;
#endif

	trustedCaList = nullptr;
	trustedCaListLen = 0;
}

/**
 * @brief Set operation mode (client or server)
 * @param[in] context Pointer to the TLS context
 * @param[in] entity Specifies whether this entity is considered a client or a server
 * @return Error code
 **/
RES_CODE tls_context_t::tlsSetConnectionEnd(tls_connection_end_t ce)
{
	if (ce != TLS_CONNECTION_END_CLIENT && ce != TLS_CONNECTION_END_SERVER)
		return RES_TLS_INVALID_PARAMETER;

	//Check whether TLS operates as a client or a server
	entity = ce;

	return RES_OK;
}

RES_CODE tls_context_t::tlsSetPrng(prng_algo_t* prngAlgo)
{
	if (prngAlgo == nullptr )
		return RES_TLS_INVALID_PARAMETER;

	tls_randoms.prng_algo = prngAlgo;

	return RES_OK;
}

/**
 * @brief Initiate the TLS handshake
 * @param[in] context Pointer to the TLS context
 * @return Error code
 **/
RES_CODE tls_context_t::tls_connect(CSocket* sock)
{
	RES_CODE res;

	//Ensure the I/O callback functions are properly registered
	if (sock == nullptr || sock->res >= RES_CLOSED)
		return RES_TLS_INVALID_PARAMETER;
	tls_socket = sock;

	//Verify that the PRNG is properly set
	if (tls_randoms.prng_algo.get() == nullptr)
		return RES_TLS_NOT_CONFIGURED;

	//Perform TLS handshake
	res = tlsHandshake();
	//Return status code
	return res;
}

/**
 * @brief Send application data to the remote host using TLS
 * @param[in] context Pointer to the TLS context
 * @param[in] data Pointer to a buffer containing the data to be transmitted
 * @param[in] length Number of bytes to be transmitted
 * @param[out] written Actual number of bytes written (optional parameter)
 * @return Error code
 **/
RES_CODE tls_context_t::tls_write(const void* data, size_t length, size_t* written)
{
	RES_CODE res;
	size_t n;
	size_t totalLength;

	//Check parameters
	if (data == nullptr && length != 0)
		return RES_TLS_INVALID_PARAMETER;

	//Ensure the I/O callback functions are properly registered
	if (tls_socket == nullptr)
		return RES_TLS_NOT_CONFIGURED;

	//Initialize status code
	res = RES_OK;

	//Actual number of bytes written
	totalLength = 0;

	//Send as much data as possible
	while (totalLength < length)
	{
		//Check current state
		if (tls_state == TLS_STATE_APPLICATION_DATA)
		{
			//The record length must not exceed 16384 bytes
			n = min(length - totalLength, TLS_MAX_RECORD_LENGTH);
			last_txrc.msg_len = n;
			res = tls_record_get_lens(&last_txrc);
			if(res == RES_OK)
			{
				memcpy(last_txrc.msg_start(), data, last_txrc.msg_len);
				last_txrc.tls_record.rec_type = TLS_TYPE_APPLICATION_DATA;
				res = tls_record_write(&last_txrc);
			}
			//Check status code
			if (res == RES_OK)
			{
				//Advance data pointer
				data = (uint8_t *) data + n;
				//Update byte counter
				totalLength += n;
			}
			else
			{
				//Send an alert message to the peer, if applicable
				tlsProcessError(res);
			}
		}
		else
		{
			//The connection has not yet been established
			res = RES_TLS_NOT_CONNECTED;
		}

		//Any error to report?
		if (res != RES_OK)
			break;
	}

	//Total number of data that have been written
	if (written != nullptr)
		*written = totalLength;

	//Return status code
	return res;
}

/**
 * @brief Receive application data from a the remote host using TLS
 * @param[in] context Pointer to the TLS context
 * @param[out] data Buffer into which received data will be placed
 * @param[in] size Maximum number of bytes that can be received
 * @param[out] received Number of bytes that have been received
 * @param[in] flags Set of flags that influences the behavior of this function
 * @return Error code
 **/
RES_CODE tls_context_t::tls_read(void* data, size_t size, size_t* received, uint32_t flags)
{
	RES_CODE res;
	size_t i;
	size_t n;
	uint8_t *p;

	//Check parameters
	if (data == nullptr || received == nullptr)
		return RES_TLS_INVALID_PARAMETER;

	//Ensure the I/O callback functions are properly registered
	if (tls_socket == nullptr)
		return RES_TLS_NOT_CONFIGURED;

	//Initialize status code
	res = RES_OK;

	//No data has been read yet
	*received = 0;

	//Read as much data as possible
	while (*received < size)
	{
		//Check current state
		if (tls_state == TLS_STATE_APPLICATION_DATA)
		{
			//The TLS record layer receives uninterpreted data from higher layers
			if (last_rxrc.msg_len == 0)
			{
				last_rxrc.iv_len = 0;
				last_rxrc.rec_len = 0;
				res = tls_message_read(&last_rxrc);
			}

			//Check status code
			if (res == RES_OK)
			{
				//Application data received?
				if (last_rxrc.tls_record.rec_type == TLS_TYPE_APPLICATION_DATA)
				{
					//Limit the number of bytes to read at a time
					n = min(last_rxrc.msg_len, size - *received);
					p = last_rxrc.msg_start();

					//The TLS_FLAG_BREAK_CHAR flag causes the function to stop reading
					//data as soon as the specified break character is encountered
					if (flags & TLS_FLAG_BREAK_CHAR)
					{
						//Retrieve the break character code
						char c = flags;

						//Search for the specified break character
						for (i = 0; i < n && p[i] != c; i++)
							;
						//Adjust the number of data to read
						n = min(n, i + 1);

						//Copy data to user buffer
						memcpy(data, p, n);
						//Total number of data that have been read
						*received += n;

						//Advance data pointer
						last_rxrc.iv_len += n;
						last_rxrc.msg_len -= n;

						//Check whether a break character has been found
						if (n > 0 && p[n - 1] == c)
							break;
					}
					else
					{
						//Copy data to user buffer
						memcpy(data, p, n);
						//Total number of data that have been read
						*received += n;

						//Advance data pointer
						last_rxrc.iv_len += n;
						last_rxrc.msg_len -= n;

						//The TLS_FLAG_WAIT_ALL flag causes the function to return
						//only when the requested number of bytes have been read
						if (!(flags & TLS_FLAG_WAIT_ALL))
							break;
					}

					//Advance data pointer
					data = (uint8_t *) data + n;
				}
				//Alert message received?
				else if (last_rxrc.tls_record.rec_type == TLS_TYPE_ALERT)
				{
					//Parse Alert message
					res = tls_parse_alert(&last_rxrc);
					last_rxrc.msg_len = 0;
				}
				//An inappropriate message was received?
				else
				{
					//Report an error
					res = RES_TLS_UNEXPECTED_MESSAGE;
				}
			}

			//Any error to report?
			if (res != RES_OK)
			{
				//Send an alert message to the peer, if applicable
				tlsProcessError(res);
				last_rxrc.msg_len = 0;
			}
		}
		else if (tls_state == TLS_STATE_CLOSING	|| tls_state == TLS_STATE_CLOSED)
		{
			//Check whether a fatal alert message has been sent or received
			if (fatal_alert_sent || fatal_alert_received)
			{
				//Alert messages with a level of fatal result in the immediate
				//termination of the connection
				res = RES_TLS_FAIL;
			}
			else
			{
				//The user must be satisfied with data already on hand
				if (*received > 0)
				{
					//Some data are pending in the receive buffer
					res = RES_OK;
					break;
				}
				else
				{
					//The receive buffer is empty
					res = RES_EOF;
				}
			}
		}
		else
		{
			//The connection has not yet been established
			res = RES_TLS_NOT_CONNECTED;
		}

		//Any error to report?
		if (res != RES_OK)
			break;
	}

	//Return status code
	return res;
}

/**
 * @brief Gracefully close TLS session
 * @param[in] context Pointer to the TLS context
 **/
RES_CODE tls_context_t::tls_shutdown()
{
	//Either party may initiate a close by sending a close_notify alert
	return tls_shutdown_ex(false);
}

/**
 * @brief Gracefully close TLS session
 * @param[in] context Pointer to the TLS context
 * @param[in] waitForCloseNotify Wait for the close notify alert from the peer
 **/
RES_CODE tls_context_t::tls_shutdown_ex(bool waitForCloseNotify)
{
	RES_CODE res;

	//Ensure the I/O callback functions are properly registered
	if (tls_socket == nullptr)
		return RES_TLS_NOT_CONFIGURED;

	//Initialize status code
	res = RES_OK;

	//Wait for the TLS session to be closed
	while (tls_state != TLS_STATE_CLOSED)
	{
		//Check current state
		if (tls_state == TLS_STATE_APPLICATION_DATA)
		{
			//Flush send buffer

			//Either party may initiate a close by sending a close_notify alert
			tls_state = TLS_STATE_CLOSING;
		}
		if (tls_state == TLS_STATE_CLOSING)
		{
			//Flush send buffer

			//Unless some other fatal alert has been transmitted, each party
			//is required to send a close_notify alert before closing the
			//write side of the connection
			if (fatal_alert_sent || fatal_alert_received)
			{
				//Close the connection immediately
				tls_state = TLS_STATE_CLOSED;
			}
			else if (!close_notify_sent)
			{
				//Notifies the recipient that the sender will not send any
				//more messages on this connection
				res = tlsSendAlert(TLS_ALERT_LEVEL_WARNING,	TLS_ALERT_CLOSE_NOTIFY);
			}
			else if (!close_notify_received && waitForCloseNotify)
			{
				//Wait for the responding close_notify alert
				last_rxrc.msg_len = 0;
				res = tls_message_read(&last_rxrc);

				//Check status code
				if (res == RES_OK)
				{
					//Application data received?
					if (last_rxrc.tls_record.rec_type == TLS_TYPE_APPLICATION_DATA)
					{
						//Advance data pointer
						last_rxrc.msg_len = 0;
					}
					//Alert message received?
					else if (last_rxrc.tls_record.rec_type == TLS_TYPE_ALERT)
					{
						//Parse Alert message
						res = tls_parse_alert(&last_rxrc);
						last_rxrc.msg_len = 0;

					}
					//An inappropriate message was received?
					else
					{
						//Report an error
						res = RES_TLS_UNEXPECTED_MESSAGE;
					}
				}
			}
			else
			{
				//The connection is closed
				tls_state = TLS_STATE_CLOSED;
			}
		}
		else
		{
			//Report an error
			res = RES_TLS_NOT_CONNECTED;
		}

		//Any error to report?
		if (res != RES_OK)
			break;
	}

	//Return status code
	return res;
}

RES_CODE tls_context_t::tls_read_cbk(void* data, uint32_t len)
{
	RES_CODE res;
	uint32_t l;

	do
	{
	    TRACELN_TLS("TLS r_cbk len=%u", len);
		if (tls_socket == nullptr || tls_socket->res >= RES_CLOSED)
			res = RES_TLS_NOT_CONFIGURED;
		else
			res = tls_socket->tsk_read(data, len);
		if(res != RES_OK)
			break;
		l = len - tls_socket->len;
		len -= l;
		data = (char*)data +l;

	} while (len && l);
    TRACELN_TLS("TLS r_cbk res=%u", res);
    return res;
}

RES_CODE tls_context_t::tls_write_cbk(const void* data, uint32_t len)
{
	RES_CODE res;

    TRACELN_TLS("TLS w_cbk len=%u", len);
	if (tls_socket == nullptr || tls_socket->res >= RES_CLOSED)
		res = RES_TLS_NOT_CONFIGURED;
	else
		res = tls_socket->tsk_write(data, len);
    TRACELN_TLS("TLS w_cbk res=%u", res);
    return res;
}

