/*
 * tls_record.cpp
 *
 *  Created on: Jun 8, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <tls_record.h>
#include <chacha20_poly1305.h>
#include <hmac.h>


RES_CODE tls_context_t::tls_record_get_lens(record_ctxt_t* lens)
{
	uint32_t size;
	size_t rec_len;

	lens->iv_len = 0;
	lens->dig_len = 0;
	lens->pad_len = 0;
	lens->tag_len = 0;
	rec_len = lens->msg_len;

	if(change_cipher_sent)
	{
		if ( hash_algo.get() )
		{
#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
			if(tls_version != SSL_VER30)
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_0 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
			//Check whether TLS 1.0, TLS 1.1 or TLS 1.2 is currently used
			if (tls_version < TLS_VER10)
#endif
			{
				return RES_TLS_INVALID_VERSION;
			}

			size = hash_algo->hash_info->digest_size;
			lens->dig_len = size;
			rec_len += size;
		}

		//Encryption is required?
		if (cipher_info)
		{
#if TLS_STREAM_CIPHER_SUPPORT
			if(cipher_info->cipher_mode != CIPHER_MODE_STREAM)
#endif
			{
#if TLS_CBC_CIPHER_SUPPORT
			//CBC block cipher?
			if(cipher_info->cipher_mode == CIPHER_MODE_CBC)
			{

#if (TLS_MAX_VERSION >= TLS_VERSION_1_1 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
				//TLS 1.1 and 1.2 use an explicit IV
				if(tls_version >= TLS_VER11)
				{
					size = cipher_info->recordIV_len;
					lens->iv_len = size;
					rec_len += size;
				}
#endif

				rec_len++; // pad length
				size = rec_len % wrc.cipher_algo->algo_info->block_size;
				if(size)
				{
					size = wrc.cipher_algo->algo_info->block_size - size;
					lens->pad_len = size;
					rec_len += size;
				}
			}
			else
#endif
#if (TLS_CCM_CIPHER_SUPPORT || TLS_CCM_8_CIPHER_SUPPORT || TLS_GCM_CIPHER_SUPPORT)
			//CCM or GCM AEAD cipher?
			if (cipher_info->cipher_mode == CIPHER_MODE_CCM
					|| cipher_info->cipher_mode == CIPHER_MODE_GCM)
			{

				size = cipher_info->recordIV_len;
				lens->iv_len = size;
				rec_len += size;

				size = cipher_info->auth_tag_len;
				lens->tag_len = size;
				rec_len += size;
			}
			else
#endif
#if TLS_CHACHA20_POLY1305_SUPPORT
			//ChaCha20Poly1305 AEAD cipher?
			if (cipher_info->cipher_mode == CIPHER_MODE_CHACHA20_POLY1305)
			{
				size = cipher_info->auth_tag_len;
				lens->tag_len = size;
				rec_len += size;
			}
			else
#endif
			//Invalid cipher mode?
			{
				//The specified cipher mode is not supported
				return RES_TLS_INVALID_CIPHER;
			}
			}
		}

	}
	lens->rec_len = rec_len;

	// Allocate
	if(lens->msg_reserve(last_txrc.rec_len) == nullptr)
			return RES_OUT_OF_MEMORY;

	lens->tls_record.rec_version = tls_version;
	lens->tls_record.rec_length = __REV16(lens->msg_len);	// rec_length will be updated...
	return RES_OK;
}

RES_CODE tls_context_t::tls_record_write(record_ctxt_t* rc)
{
	RES_CODE res;


    TRACELN1_TLS("TLS write record");
    tls_dump_record(rc);
	if(change_cipher_sent)
		res = tls_record_encrypt(rc);
	else
		res = RES_OK;

	if(res == RES_OK)
	{
		res = tls_write_cbk(&rc->tls_record, sizeof(tls_record_t));
		if(res == RES_OK && rc->rec_len)
			res = tls_write_cbk(rc->data.get(), rc->rec_len);
	}
    TRACELN_TLS("TLS write res %u", res);
	return res;
}

RES_CODE tls_context_t::tls_record_encrypt(record_ctxt_t* rc)
{
	RES_CODE res;
	size_t length;

	length = rc->msg_len;
	if(hash_algo.get())
	{
#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
		if(tls_version == SSL_VER30)
		{
			//SSL 3.0 uses an older obsolete version of the HMAC construction
			res = sslComputeMac(rc, wrc, rc->msg_end());

			if(res != RES_OK)
				return res;
		}
		else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_0 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
		//Check whether TLS 1.0, TLS 1.1 or TLS 1.2 is currently used
		if (tls_version >= TLS_VER10)
		{
			//TLS uses a HMAC construction
			hmac_ctxt_t hmac;

			hmac.Reset(hash_algo.get(),	wrc.mac_key, cipher_info->mac_key_len);

			//Compute MAC over the sequence number and the record contents
			hash_algo->Input(&wrc.seq_num, sizeof(wrc.seq_num));
			hash_algo->Input(&rc->tls_record,	sizeof(tls_record_t));
			hash_algo->Input(rc->msg_start(), length);

			//Append the resulting MAC to the message
			hmac.Result(rc->msg_end());
		}
		else
#endif
		//The negotiated TLS version is not valid...
		{
			//Report an error
			return RES_TLS_INVALID_VERSION;
		}

	      //Adjust the length of the message
		length += hash_algo->hash_info->digest_size;

		//Fix length field
		rc->tls_record.rec_length = __REV16(length);

		wrc.seq_num.increment();
	}


	//Encryption is required?
	if (cipher_info)
	{
#if TLS_STREAM_CIPHER_SUPPORT
		//Stream cipher?
		if(cipher_info->cipher_mode == CIPHER_MODE_STREAM)
		{
			//Encrypt record contents
			wrc.cipher_algo->encrypt(rc->data.get(), rc->data.get(), length);
		}
		else
#endif
#if TLS_CBC_CIPHER_SUPPORT
		//CBC block cipher?
		if(cipher_info->cipher_mode == CIPHER_MODE_CBC)
		{

#if (TLS_MAX_VERSION >= TLS_VERSION_1_1 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
			//TLS 1.1 and 1.2 use an explicit IV
			if(tls_version >= TLS_VER11)
			{
				//The initialization vector should be chosen at random
				res = tls_randoms.prng_algo->prng_read(rc->data.get(), rc->iv_len);
				//Any error to report?
				if(res != RES_OK)
					return res;

				//Adjust the length of the message
				length += rc->iv_len;
			}
#endif

			//Write padding bytes
			for(uint32_t i = 0; i <= rc->pad_len; i++)
				rc->data.get()[length++] = rc->pad_len;

			//Fix length field
			rc->tls_record.rec_length = __REV16(length);

			//CBC encryption
			res = wrc.cipher_algo->cbc_encrypt(wrc.iv, rc->data.get(), length);

			if(res != RES_OK)
				return res;
		}
		else
#endif
#if (TLS_CCM_CIPHER_SUPPORT || TLS_CCM_8_CIPHER_SUPPORT || TLS_GCM_CIPHER_SUPPORT)
		//CCM or GCM AEAD cipher?
		if (cipher_info->cipher_mode == CIPHER_MODE_CCM
				|| cipher_info->cipher_mode == CIPHER_MODE_GCM)
		{

			uint8_t *data;
			uint8_t *tag;
			size_t nonceLength;
			uint8_t nonce[12];
			uint8_t a[13];

			//Determine the total length of the nonce
			nonceLength = cipher_info->fixedIV_len + rc->iv_len;
			//The salt is the implicit part of the nonce and is not sent in the packet
			memcpy(nonce, wrc.iv, cipher_info->fixedIV_len);

			//The explicit part of the nonce is chosen by the sender
			res = tls_randoms.prng_algo->prng_read(nonce + cipher_info->fixedIV_len, rc->iv_len);
			if(res != RES_OK)
				return res;

			//The explicit part of the nonce is carried in each TLS record
			memcpy(rc->data.get(), nonce + cipher_info->fixedIV_len, rc->iv_len);

			//Additional data to be authenticated
			memcpy(a, &wrc.seq_num, sizeof(wrc.seq_num));
			memcpy(a + sizeof(wrc.seq_num), &rc->tls_record, sizeof(tls_record_t));

			//Point to the plaintext
			data = rc->data.get() + rc->iv_len;
			//Point to the buffer where to store the authentication tag
			tag = data + length;

#if (TLS_CCM_CIPHER_SUPPORT || TLS_CCM_8_CIPHER_SUPPORT)
			//CCM AEAD cipher?
			if (cipher_info->cipher_mode == CIPHER_MODE_CCM)
			{
				//Authenticated encryption using CCM
				res = wrc.cipher_algo->ccm_encrypt(nonce, nonceLength, a, 13,
						data, length, tag, cipher_info->auth_tag_len);
			}
			else
#endif
#if TLS_GCM_CIPHER_SUPPORT
			//GCM AEAD cipher?
			if(cipher_info->cipher_mode == CIPHER_MODE_GCM)
			{
				//Authenticated encryption using GCM
				res = wrc.cipher_algo->gcm_encrypt(nonce, nonceLength, a, 13,
						data, length, tag, cipher_info->auth_tag_len);
			}
			else
#endif
			//Invalid cipher mode?
			{
				//The specified cipher mode is not supported
				res = RES_TLS_INVALID_CIPHER;
			}

			//Failed to encrypt data?
			if(res != RES_OK)
				return res;

			//Compute the length of the resulting message
			length += rc->iv_len + cipher_info->auth_tag_len;
			//Fix length field
			rc->tls_record.rec_length = __REV16(length);

			//Increment sequence number
			wrc.seq_num.increment();

		}
		else
#endif
#if TLS_CHACHA20_POLY1305_SUPPORT
		//ChaCha20Poly1305 AEAD cipher?
		if (cipher_info->cipher_mode == CIPHER_MODE_CHACHA20_POLY1305)
		{
			size_t i;
			uint8_t nonce[12];
			uint8_t a[13];

			//The 64-bit record sequence number is serialized as an 8-byte,
			//big-endian value and padded on the left with four 0x00 bytes
			memcpy(nonce + 4, wrc.seq_num.data, 8);
			memclr(nonce, 4);

			//The padded sequence number is XORed with the write IV to form
			//the 96-bit nonce
			for (i = 0; i < cipher_info->fixedIV_len; i++)
				nonce[i] ^= wrc.iv[i];

			//Additional data to be authenticated
			memcpy(a, &wrc.seq_num, sizeof(wrc.seq_num));
			memcpy(a + sizeof(wrc.seq_num), &rc->tls_record, sizeof(tls_record_t));

			//Point to the buffer where to store the authentication tag

			//Authenticated encryption using ChaCha20Poly1305
			res = chacha20Poly1305Encrypt(wrc.enc_key, cipher_info->enc_key_len,
					nonce, 12, a, 13, rc->data.get(), length,
					rc->data.get() + length, cipher_info->auth_tag_len);

			//Failed to encrypt data?
			if (res != RES_OK)
				return res;

			//Compute the length of the resulting message
			length += cipher_info->auth_tag_len;
			//Fix length field
			rc->tls_record.rec_length = __REV16(length);

			//Increment sequence number
			wrc.seq_num.increment();

		}
		else
#endif
		//Invalid cipher mode?
		{
			//The specified cipher mode is not supported
			return RES_TLS_INVALID_CIPHER;
		}
	}



	//Successful encryption
	return RES_OK;
}

RES_CODE tls_context_t::tls_record_decrypt(record_ctxt_t* rc)
{
	RES_CODE res=RES_TLS_DECODING_FAILED;

	//Convert the length field to host byte order
	rc->msg_len = rc->rec_len;

	//Decryption is required?
	if (cipher_info)
	{

#if TLS_STREAM_CIPHER_SUPPORT
		//Stream cipher?
		if(cipher_info->cipher_mode == CIPHER_MODE_STREAM)
		{
			//Encrypt record contents
			rdc.cipher_algo->encrypt(rc->data.get(), rc->data.get(), rc->msg_len);
		}
		else
#endif
#if TLS_CBC_CIPHER_SUPPORT
		//CBC block cipher?
		if(cipher_info->cipher_mode == CIPHER_MODE_CBC)
		{
			//The length of the data must be a multiple of the block size
			if ((rc->rec_len % rdc.cipher_algo->algo_info->block_size) != 0)
				return RES_TLS_DECODING_FAILED;

#if (TLS_MAX_VERSION >= TLS_VERSION_1_1 && TLS_MIN_VERSION <= TLS_VERSION_1_2)

			uint32_t iv_len;

			//TLS 1.1 and 1.2 use an explicit IV
			if(tls_version >= TLS_VER11)
			{
				//Make sure the message length is acceptable
				if (rc->rec_len < cipher_info->recordIV_len)
					return RES_TLS_DECODING_FAILED;


				//Adjust the length of the message
				iv_len = cipher_info->recordIV_len;
				rc->rec_len -= iv_len;
				res = rdc.cipher_algo->cbc_decrypt(rdc.iv, rc->data.get(),
						rc->data.get(), iv_len);
				if (res != RES_OK)
					return res;
			} else
				iv_len = 0;

#endif
			{
				//CBC decryption
				res = rdc.cipher_algo->cbc_decrypt(rdc.iv, rc->data.get() +	iv_len,
						rc->data.get(), rc->rec_len);
				if (res != RES_OK)
					return res;
			}

			//Make sure the message length is acceptable
			if(rc->rec_len < rdc.cipher_algo->algo_info->block_size)
				return RES_TLS_DECODING_FAILED;

			//Compute the length of the padding string
			rc->pad_len = rc->data.get()[rc->rec_len - 1];

			//Erroneous padding length?
			if (rc->pad_len >= rc->rec_len)
				return RES_TLS_BAD_RECORD_MAC;

			//The receiver must check the padding
			for(uint32_t i = 1; i <= rc->pad_len; i++)
			{
				//Each byte in the padding data must be filled
				//with the padding length value
				if (rc->data.get()[rc->rec_len -1 - i] != rc->pad_len)
					return RES_TLS_BAD_RECORD_MAC;
			}

			//Remove padding bytes
			rc->pad_len++;
			rc->rec_len -= rc->pad_len;
			//Fix the length field of the TLS record
			rc->tls_record.rec_length = __REV16(rc->rec_len);

		}
		else
#endif
#if (TLS_CCM_CIPHER_SUPPORT || TLS_CCM_8_CIPHER_SUPPORT || TLS_GCM_CIPHER_SUPPORT)
		//CCM or GCM AEAD cipher?
		if (cipher_info->cipher_mode == CIPHER_MODE_CCM
				|| cipher_info->cipher_mode == CIPHER_MODE_GCM)
		{
			size_t nonceLength;
			uint8_t *data;
			uint8_t *tag;
			uint8_t nonce[12];
			uint8_t a[13];

			rc->tag_len = cipher_info->auth_tag_len;

			//Make sure the message length is acceptable
			if (rc->rec_len < (cipher_info->recordIV_len + rc->tag_len))
				return RES_TLS_DECODING_FAILED;

			//Determine the total length of the nonce
			nonceLength = cipher_info->fixedIV_len + cipher_info->recordIV_len;
			//The salt is the implicit part of the nonce and is not sent in the packet
			memcpy(nonce, rdc.iv, cipher_info->fixedIV_len);
			//The explicit part of the nonce is chosen by the sender
			memcpy(nonce + cipher_info->fixedIV_len, rc->data.get(), cipher_info->recordIV_len);

			//Calculate the length of the ciphertext
			rc->rec_len -= cipher_info->recordIV_len + rc->tag_len;
			//Fix the length field of the TLS record
			rc->tls_record.rec_length = __REV16(rc->rec_len);

			//Additional data to be authenticated
			memcpy(a, &rdc.seq_num, sizeof(rdc.seq_num));
			memcpy(a + sizeof(rdc.seq_num), &rc->tls_record, sizeof(tls_record_t));

			//Point to the ciphertext
			data = rc->data.get() + cipher_info->recordIV_len;
			//Point to the authentication tag
			tag = data + rc->rec_len;



#if (TLS_CCM_CIPHER_SUPPORT || TLS_CCM_8_CIPHER_SUPPORT)
			//CCM AEAD cipher?
			if (cipher_info->cipher_mode == CIPHER_MODE_CCM)
			{
				//Authenticated decryption using CCM
				res = rdc.cipher_algo->ccm_decrypt(nonce, nonceLength, a, 13,
						rc->data.get() + cipher_info->recordIV_len, rc->data.get(),
						rc->rec_len, tag, rc->tag_len);

			}
			else
#endif
#if TLS_GCM_CIPHER_SUPPORT
			//GCM AEAD cipher?
			if(cipher_info->cipher_mode == CIPHER_MODE_GCM)
			{
				//Authenticated decryption using GCM
				res = rdc.cipher_algo->gcm_decrypt(nonce, nonceLength,	a, 13,
						rc->data.get() + cipher_info->recordIV_len, rc->data.get(),
						rc->rec_len, tag, rc->tag_len);
			}
			else
#endif
			//Invalid cipher mode?
			{
				//The specified cipher mode is not supported
				res = RES_TLS_INVALID_CIPHER;
			}

			//Failed to encrypt data?
			if(res != RES_OK)
				return RES_TLS_BAD_RECORD_MAC;

			//Increment sequence number
			rdc.seq_num.increment();

		}
		else
#endif
#if TLS_CHACHA20_POLY1305_SUPPORT
		//ChaCha20Poly1305 AEAD cipher?
		if (cipher_info->cipher_mode == CIPHER_MODE_CHACHA20_POLY1305)
		{
			size_t i;
			uint8_t nonce[12];
			uint8_t a[13];

			//Make sure the message length is acceptable
			rc->tag_len = cipher_info->auth_tag_len;
			if (rc->rec_len < rc->tag_len)
				return RES_TLS_DECODING_FAILED;

			//The 64-bit record sequence number is serialized as an 8-byte,
			//big-endian value and padded on the left with four 0x00 bytes
			memcpy(nonce + 4, rdc.seq_num.data, 8);
			memclr(nonce, 4);

			//The padded sequence number is XORed with the read IV to form
			//the 96-bit nonce
			for (i = 0; i < cipher_info->fixedIV_len; i++)
				nonce[i] ^= rdc.iv[i];

			//Calculate the length of the ciphertext
			rc->rec_len -= rc->tag_len;
			//Fix the length field of the TLS record
			rc->tls_record.rec_length = __REV16(rc->rec_len);

			//Additional data to be authenticated
			memcpy(a, &rdc.seq_num, sizeof(rdc.seq_num));
			memcpy(a + sizeof(rdc.seq_num), &rc->tls_record, sizeof(tls_record_t));

			//Authenticated decryption using ChaCha20Poly1305
			res = chacha20Poly1305Decrypt(rdc.enc_key, cipher_info->enc_key_len,
					nonce, 12, a, 13, rc->data.get(), rc->rec_len,
					rc->data.get()+ rc->rec_len, rc->tag_len);
			//Wrong authentication tag?
			if (res != RES_OK)
				return RES_TLS_BAD_RECORD_MAC;

			//Increment sequence number
			rdc.seq_num.increment();
		}
		else
#endif
		//Invalid cipher mode?
		{
			//The specified cipher mode is not supported
			return RES_TLS_INVALID_CIPHER;
		}
	}

	if(hash_algo.get())
	{
		hmac_ctxt_t hmac;

		//Make sure the message length is acceptable
		rc->dig_len = hash_algo->hash_info->digest_size;
		if (rc->rec_len < rc->dig_len)
			return RES_TLS_DECODING_FAILED;

		//Adjust the length of the message
		rc->rec_len -= rc->dig_len;
		//Fix the length field of the TLS record
		rc->tls_record.rec_length = __REV16(rc->rec_len);

#if (TLS_MAX_VERSION >= SSL_VERSION_3_0 && TLS_MIN_VERSION <= SSL_VERSION_3_0)
		if(tls_version == SSL_VER30)
		{
			//SSL 3.0 uses an older obsolete version of the HMAC construction
			res = sslComputeMac(rc, rdc, hmac.hmac_digest);
			if(res != RES_OK)
				return res;
		}
		else
#endif
#if (TLS_MAX_VERSION >= TLS_VERSION_1_0 && TLS_MIN_VERSION <= TLS_VERSION_1_2)
		//Check whether TLS 1.0, TLS 1.1 or TLS 1.2 is currently used
		if (tls_version >= TLS_VER10)
		{
			//TLS uses a HMAC construction
			hmac.Reset(hash_algo.get(),	rdc.mac_key, cipher_info->mac_key_len);

			//Compute MAC over the sequence number and the record contents
			hash_algo->Input(&rdc.seq_num, sizeof(rdc.seq_num));
			hash_algo->Input(&rc->tls_record,	sizeof(tls_record_t));
			hash_algo->Input(rc->data.get(), rc->rec_len);

			hmac.Result(hmac.hmac_digest);
		}
		else
#endif
		//The negotiated TLS version is not valid...
		{
			//Report an error
			return RES_TLS_INVALID_VERSION;
		}

		//Check the message authentication code
		if (memcmp(rc->data.get() + rc->rec_len, hmac.hmac_digest, hash_algo->hash_info->digest_size))
			return RES_TLS_BAD_RECORD_MAC;

		//Increment sequence number
		rdc.seq_num.increment();
	}

	//Successful decryption
	return RES_OK;
}

RES_CODE tls_context_t::tls_record_read(record_ctxt_t* rc)
{
	RES_CODE res;


	rc->iv_len = 0;
	rc->pad_len = 0;
	rc->tag_len = 0;

	// 1. Read the TLS record header
	res = tls_read_cbk(&rc->tls_record, sizeof(tls_record_t));
	if(res == RES_OK)
	{
		//2.  check the allocation size
		rc->rec_len = __REV16(rc->tls_record.rec_length);
		rc->dig_len = rc->rec_len;
		if(rc->rec_len)
		{
			if(rc->msg_reserve(rc->rec_len))
			{
				// 3. Read TLS Record contents
				res = tls_read_cbk(rc->data.get(), rc->rec_len);
			} else
				res = RES_OUT_OF_MEMORY;
		}

		// 4. Checking
		if(res == RES_OK)
		{
		    TRACELN_TLS("TLS record read type=%u v=%x len=%u",
		    		rc->tls_record.rec_type, rc->tls_record.rec_version, rc->rec_len);
			if (tls_state > TLS_STATE_SERVER_HELLO)
			{
				//Once the server has sent the ServerHello message, enforce
				//incoming record versions
				if (rc->tls_record.rec_version != tls_version)
					res = RES_TLS_INVALID_VERSION;
			}

			if(change_cipher_received)
				res = tls_record_decrypt(rc);
		} else
			res = RES_TLS_READ_FAILED;
	}

    TRACELN_TLS("TLS read record res %u", res);
    tls_dump_record(rc);
	return res;
}

RES_CODE tls_context_t::sslComputeMac(record_ctxt_t* rc, tls_xfer_ctxt_t& xfer, uint8_t *mac)
{
	size_t padLength;
	hash_algo_t* hash;

	hash = hash_algo.get();

	//The length of pad1 and pad2 depends on hash algorithm
	if (hash->hash_info == MD5_HASH_ALGO)
	{
		//48-byte long patterns are used with MD5
		padLength = 48;
	}
	else if (hash->hash_info == SHA1_HASH_ALGO)
	{
		//40-byte long patterns are used with SHA-1
		padLength = 40;
	}
	else
	{
		//SSL 3.0 supports only MD5 and SHA-1 hash functions
		return RES_TLS_INVALID_PARAMETER;
	}

	//Compute hash(secret + pad1 + seqNum + type + length + data)
	hash->Reset();
	hash->Input(xfer.mac_key, cipher_info->mac_key_len);
	hash->Pad(0x36, padLength);
	hash->Input(&xfer.seq_num, sizeof(tls_seq_num_t));
	hash->Input(&rc->tls_record.rec_type, sizeof(rc->tls_record.rec_type));
	hash->Input(&rc->tls_record.rec_length, sizeof(rc->tls_record.rec_length));
	hash->Input(rc->msg_start(), rc->msg_len);
	hash->Result(mac);

	//Then compute hash(secret + pad2 + hash(secret + pad1 + seqNum + type + length + data))
	hash->Reset();
	hash->Input(xfer.mac_key, cipher_info->mac_key_len);
	hash->Pad(0x5C, padLength);
	hash->Input(mac, hash->hash_info->digest_size);
	hash->Result(mac);

	//Successful processing
	return RES_OK;
}

RES_CODE tls_context_t::tls_message_read(record_ctxt_t* rc)
{
	RES_CODE res;

	// Read a TLS record
	if(rc->rec_len)
		res = RES_OK;
	else
		res = tls_record_read(rc);
	rc->msg_len = rc->rec_len;

	// Check and loop reading fragments
	while(res == RES_OK)
	{
		switch(rc->tls_record.rec_type)
		{
		case TLS_TYPE_CHANGE_CIPHER_SPEC:
			rc->msg_len = sizeof(tls_change_cipherspec_t);
			break;

		case TLS_TYPE_ALERT:
			rc->msg_len = sizeof(tls_alert_t);
			break;

		case TLS_TYPE_HANDSHAKE:
			if(rc->rec_len < sizeof(tls_handshake_t))
				rc->msg_len = sizeof(tls_handshake_t);
			else
				rc->msg_len = sizeof(tls_handshake_t) + static_cast<tls_handshake_t*>((void*)rc->data.get())->get_handshake_len();
			break;

		case TLS_TYPE_APPLICATION_DATA:
			break;

		default:
			return RES_TLS_UNEXPECTED_MESSAGE;
		}

		// read more?
		if(rc->rec_len >= rc->msg_len)
			break;
		else
		{
			record_ctxt_t rc2;

			res = tls_record_read(&rc2);
			if(res == RES_OK)
			{
				if(rc2.tls_record.rec_type != rc->tls_record.rec_type)
					res = RES_TLS_UNEXPECTED_MESSAGE;
				else
				{
					if(rc2.rec_len)
					{
						uint8_t* data;

						data = rc->msg_reserve(rc->rec_len + rc2.rec_len);
						if(data)
						{
							memcpy(data + rc->rec_len, rc2.data.get(), rc2.rec_len);
							rc->rec_len += rc2.rec_len;
						} else
							res = RES_OUT_OF_MEMORY;
					}
				}
			}
		}

	}

    TRACELN_TLS("TLS message read res %u", res);
	return res;
}
