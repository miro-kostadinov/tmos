/*
 * tls.h
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_TLS_TLS_H_
#define SERVICES_SECURITY_TLS_TLS_H_

#include <tmos.h>
#include <tls_opt.h>
#include <common.h>
#include <crypto.h>
#include <tls_cipher_suites.h>
#include <memory.h>
#include <rsa.h>
#include <ecdh.h>
#include <dh.h>
#include <ecdsa.h>
#include <csocket.h>

enum tls_version_t: uint16_t
{
	SSL_VER30 = REV16(SSL_VERSION_3_0),
	TLS_VER10 = REV16(TLS_VERSION_1_0),
	TLS_VER11 = REV16(TLS_VERSION_1_1),
	TLS_VER12 = REV16(TLS_VERSION_1_2)
};

enum TlsFlags
{
   TLS_FLAG_WAIT_ALL   = 0x0800,
   TLS_FLAG_BREAK_CHAR = 0x1000,
   TLS_FLAG_BREAK_CRLF = 0x100A,
   TLS_FLAG_WAIT_ACK   = 0x2000,
   TLS_FLAG_NO_DELAY   = 0x4000,
   TLS_FLAG_DELAY      = 0x8000
};


struct tls_random_t
{
   uint32_t gmt_unix_time;
   uint8_t 	random_bytes[28];
} __attribute__ ((packed));

enum tls_message_type_t: uint8_t
{
   TLS_TYPE_HELLO_REQUEST        = 0,
   TLS_TYPE_CLIENT_HELLO         = 1,
   TLS_TYPE_SERVER_HELLO         = 2,
   TLS_TYPE_HELLO_VERIFY_REQUEST = 3,
   TLS_TYPE_NEW_SESSION_TICKET   = 4,
   TLS_TYPE_CERTIFICATE          = 11,
   TLS_TYPE_SERVER_KEY_EXCHANGE  = 12,
   TLS_TYPE_CERTIFICATE_REQUEST  = 13,
   TLS_TYPE_SERVER_HELLO_DONE    = 14,
   TLS_TYPE_CERTIFICATE_VERIFY   = 15,
   TLS_TYPE_CLIENT_KEY_EXCHANGE  = 16,
   TLS_TYPE_FINISHED             = 20,
   TLS_TYPE_CERTIFICATE_URL      = 21,
   TLS_TYPE_CERTIFICATE_STATUS   = 22,
   TLS_TYPE_SUPPLEMENTAL_DATA    = 23
};

/**
 * @brief TLS handshake message
 **/

struct tls_handshake_t
{
	tls_message_type_t	msgType;
	uint8_t 			length[3];

	void set(tls_message_type_t	type, uint32_t len)
	{
		*(uint32_t*)(void*)this = __REV(len) | type;
	}

	uint32_t get_handshake_len() const
	{
		return (length[0] << 16) + (length[1] << 8) + length[2];
	}
}__attribute__ ((packed));


struct tls_client_hello_t
{
	tls_handshake_t		header;
	tls_version_t		clientVersion;
	tls_random_t 		random;
	uint8_t 			sessionIdLen;
	uint8_t 			sessionId[];
}__attribute__ ((packed));



struct tls_server_hello_t
{
	tls_handshake_t		header;
	tls_version_t		serverVersion;
	tls_random_t 		random;
	uint8_t 			sessionIdLength;
	uint8_t 			sessionId[];
}__attribute__ ((packed));


struct tls_certificate_msg_t
{
	tls_handshake_t		header;
	uint8_t 			certificate_list_Length[3];
	uint8_t 			certificate_list[];
}__attribute__ ((packed));


struct tls_server_keyexchange_t
{
	tls_handshake_t		header;
	uint8_t 			data[];
}__attribute__ ((packed));


struct tls_certificate_request_t
{
	tls_handshake_t		header;
	uint8_t 			certificateTypesLength;
	uint8_t 			certificateTypes[];
}__attribute__ ((packed));


struct tls_server_hellodone_t
{
	tls_handshake_t		header;
}__attribute__ ((packed));


struct tls_client_key_exchange_t
{
	tls_handshake_t		header;
	uint8_t 			data[];
}__attribute__ ((packed));


struct tls_certificate_verify_t
{
	tls_handshake_t		header;
	uint8_t 			signature[];
}__attribute__ ((packed));


struct tls_finished_t
{
	tls_handshake_t		header;
	uint8_t 			verifyData[];
}__attribute__ ((packed));


enum tls_record_type_t: uint8_t
{
   TLS_TYPE_NONE               = 0,
   TLS_TYPE_CHANGE_CIPHER_SPEC = 20,
   TLS_TYPE_ALERT              = 21,
   TLS_TYPE_HANDSHAKE          = 22,
   TLS_TYPE_APPLICATION_DATA   = 23
};

enum tls_compression_method_t: uint8_t
{
   TLS_COMPRESSION_METHOD_NULL    = 0,
   TLS_COMPRESSION_METHOD_DEFLATE = 1
};

enum tls_extension_type_t: uint16_t
{
   TLS_EXT_SERVER_NAME            = REV16(0),
   TLS_EXT_MAX_FRAGMENT_LENGTH    = REV16(1),
   TLS_EXT_CLIENT_CERTIFICATE_URL = REV16(2),
   TLS_EXT_TRUSTED_CA_KEYS        = REV16(3),
   TLS_EXT_TRUNCATED_HMAC         = REV16(4),
   TLS_EXT_STATUS_REQUEST         = REV16(5),
   TLS_EXT_USER_MAPPING           = REV16(6),
   TLS_EXT_CLIENT_AUTHZ           = REV16(7),
   TLS_EXT_SERVER_AUTHZ           = REV16(8),
   TLS_EXT_CERT_TYPE              = REV16(9),
   TLS_EXT_ELLIPTIC_CURVES        = REV16(10),
   TLS_EXT_EC_POINT_FORMATS       = REV16(11),
   TLS_EXT_SRP                    = REV16(12),
   TLS_EXT_SIGNATURE_ALGORITHMS   = REV16(13),
   TLS_EXT_USE_SRTP               = REV16(14),
   TLS_EXT_HEARTBEAT              = REV16(15),
   TLS_EXT_ALPN                   = REV16(16),
   TLS_EXT_SESSION_TICKET         = REV16(35),
   TLS_EXT_RENEGOTIATION_INFO     = REV16(65281)
};

enum tls_name_type_t: uint8_t
{
   TLS_NAME_TYPE_HOSTNAME 		  = 0
};

enum tls_ecnamedcurve_t: uint16_t
{
   TLS_EC_CURVE_NONE                     = REV16(0),
   TLS_EC_CURVE_SECT163K1                = REV16(1),     //RFC 4492
   TLS_EC_CURVE_SECT163R1                = REV16(2),     //RFC 4492
   TLS_EC_CURVE_SECT163R2                = REV16(3),     //RFC 4492
   TLS_EC_CURVE_SECT193R1                = REV16(4),     //RFC 4492
   TLS_EC_CURVE_SECT193R2                = REV16(5),     //RFC 4492
   TLS_EC_CURVE_SECT233K1                = REV16(6),     //RFC 4492
   TLS_EC_CURVE_SECT233R1                = REV16(7),     //RFC 4492
   TLS_EC_CURVE_SECT239K1                = REV16(8),     //RFC 4492
   TLS_EC_CURVE_SECT283K1                = REV16(9),     //RFC 4492
   TLS_EC_CURVE_SECT283R1                = REV16(10),    //RFC 4492
   TLS_EC_CURVE_SECT409K1                = REV16(11),    //RFC 4492
   TLS_EC_CURVE_SECT409R1                = REV16(12),    //RFC 4492
   TLS_EC_CURVE_SECT571K1                = REV16(13),    //RFC 4492
   TLS_EC_CURVE_SECT571R1                = REV16(14),    //RFC 4492
   TLS_EC_CURVE_SECP160K1                = REV16(15),    //RFC 4492
   TLS_EC_CURVE_SECP160R1                = REV16(16),    //RFC 4492
   TLS_EC_CURVE_SECP160R2                = REV16(17),    //RFC 4492
   TLS_EC_CURVE_SECP192K1                = REV16(18),    //RFC 4492
   TLS_EC_CURVE_SECP192R1                = REV16(19),    //RFC 4492
   TLS_EC_CURVE_SECP224K1                = REV16(20),    //RFC 4492
   TLS_EC_CURVE_SECP224R1                = REV16(21),    //RFC 4492
   TLS_EC_CURVE_SECP256K1                = REV16(22),    //RFC 4492
   TLS_EC_CURVE_SECP256R1                = REV16(23),    //RFC 4492
   TLS_EC_CURVE_SECP384R1                = REV16(24),    //RFC 4492
   TLS_EC_CURVE_SECP521R1                = REV16(25),    //RFC 4492
   TLS_EC_CURVE_BRAINPOOLP256R1          = REV16(26),    //RFC 7027
   TLS_EC_CURVE_BRAINPOOLP384R1          = REV16(27),    //RFC 7027
   TLS_EC_CURVE_BRAINPOOLP512R1          = REV16(28),    //RFC 7027
   TLS_EC_CURVE_ECDH_X25519              = REV16(29),    //RFC draft
   TLS_EC_CURVE_ECDH_X448                = REV16(30),    //RFC draft
   TLS_EC_CURVE_FFDHE2048                = REV16(256),   //RFC 7919
   TLS_EC_CURVE_FFDHE3072                = REV16(257),   //RFC 7919
   TLS_EC_CURVE_FFDHE4096                = REV16(258),   //RFC 7919
   TLS_EC_CURVE_FFDHE6144                = REV16(259),   //RFC 7919
   TLS_EC_CURVE_FFDHE8192                = REV16(260),   //RFC 7919
   TLS_EC_CURVE_ARBITRARY_EXPLICIT_PRIME = REV16(65281), //RFC 4492
   TLS_EC_CURVE_ARBITRARY_EXPLICIT_CHAR2 = REV16(65282)  //RFC 4492
};

enum tls_ecpoint_format_t: uint8_t
{
	TLS_EC_POINT_FORMAT_UNCOMPRESSED              = 0,
	TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_PRIME = 1,
	TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_CHAR2 = 2
};

enum TlsEcCurveType: uint8_t
{
	TLS_EC_CURVE_TYPE_EXPLICIT_PRIME = 1,
	TLS_EC_CURVE_TYPE_EXPLICIT_CHAR2 = 2,
	TLS_EC_CURVE_TYPE_NAMED_CURVE 	 = 3
};

enum tls_hash_algo_t: uint8_t
{
   TLS_HASH_ALGO_NONE   = 0,
   TLS_HASH_ALGO_MD5    = 1,
   TLS_HASH_ALGO_SHA1   = 2,
   TLS_HASH_ALGO_SHA224 = 3,
   TLS_HASH_ALGO_SHA256 = 4,
   TLS_HASH_ALGO_SHA384 = 5,
   TLS_HASH_ALGO_SHA512 = 6
};

enum tls_sign_algo_t: uint8_t
{
   TLS_SIGN_ALGO_ANONYMOUS = 0,
   TLS_SIGN_ALGO_RSA       = 1,
   TLS_SIGN_ALGO_DSA       = 2,
   TLS_SIGN_ALGO_ECDSA     = 3
};

typedef enum
{
   TLS_CERT_NONE             = 0,
   TLS_CERT_RSA_SIGN         = 1,
   TLS_CERT_DSS_SIGN         = 2,
   TLS_CERT_RSA_FIXED_DH     = 3,
   TLS_CERT_DSS_FIXED_DH     = 4,
   TLS_CERT_RSA_EPHEMERAL_DH = 5,
   TLS_CERT_DSS_EPHEMERAL_DH = 6,
   TLS_CERT_FORTEZZA_DMS     = 20,
   TLS_CERT_ECDSA_SIGN       = 64,
   TLS_CERT_RSA_FIXED_ECDH   = 65,
   TLS_CERT_ECDSA_FIXED_ECDH = 66
} TlsCertificateType;


struct tls_record_t
{
	tls_record_type_t 	rec_type;
	tls_version_t		rec_version;
	uint16_t 			rec_length;
	uint8_t 			rec_data[0];
} __attribute__ ((packed));

struct tls_change_cipherspec_t
{
   uint8_t type;
} __attribute__ ((packed));

enum tls_alert_level_t : uint8_t
{
   TLS_ALERT_LEVEL_WARNING = 1,
   TLS_ALERT_LEVEL_FATAL   = 2,
};

enum tls_alert_description_t : uint8_t
{
   TLS_ALERT_CLOSE_NOTIFY                    = 0,
   TLS_ALERT_UNEXPECTED_MESSAGE              = 10,
   TLS_ALERT_BAD_RECORD_MAC                  = 20,
   TLS_ALERT_DECRYPTION_FAILED               = 21,
   TLS_ALERT_RECORD_OVERFLOW                 = 22,
   TLS_ALERT_DECOMPRESSION_FAILURE           = 30,
   TLS_ALERT_HANDSHAKE_FAILURE               = 40,
   TLS_ALERT_NO_CERTIFICATE                  = 41,
   TLS_ALERT_BAD_CERTIFICATE                 = 42,
   TLS_ALERT_UNSUPPORTED_CERTIFICATE         = 43,
   TLS_ALERT_CERTIFICATE_REVOKED             = 44,
   TLS_ALERT_CERTIFICATE_EXPIRED             = 45,
   TLS_ALERT_CERTIFICATE_UNKNOWN             = 46,
   TLS_ALERT_ILLEGAL_PARAMETER               = 47,
   TLS_ALERT_UNKNOWN_CA                      = 48,
   TLS_ALERT_ACCESS_DENIED                   = 49,
   TLS_ALERT_DECODE_ERROR                    = 50,
   TLS_ALERT_DECRYPT_ERROR                   = 51,
   TLS_ALERT_EXPORT_RESTRICTION              = 60,
   TLS_ALERT_PROTOCOL_VERSION                = 70,
   TLS_ALERT_INSUFFICIENT_SECURITY           = 71,
   TLS_ALERT_INTERNAL_ERROR                  = 80,
   TLS_ALERT_INAPPROPRIATE_FALLBACK          = 86,
   TLS_ALERT_USER_CANCELED                   = 90,
   TLS_ALERT_NO_RENEGOTIATION                = 100,
   TLS_ALERT_UNSUPPORTED_EXTENSION           = 110,
   TLS_ALERT_CERTIFICATE_UNOBTAINABLE        = 111,
   TLS_ALERT_UNRECOGNIZED_NAME               = 112,
   TLS_ALERT_BAD_CERTIFICATE_STATUS_RESPONSE = 113,
   TLS_ALERT_BAD_CERTIFICATE_HASH_VALUE      = 114,
   TLS_ALERT_UNKNOWN_PSK_IDENTITY            = 115
};

struct tls_alert_t
{
	tls_alert_level_t level;
	tls_alert_description_t description;
} __attribute__ ((packed));


struct tls_cipher_suites_t
{
	uint16_t 				length;
	tls_suite_id_t 			value[];
} __attribute__ ((packed));

struct tls_compression_methods_t
{
	uint8_t 				length;
	tls_compression_method_t value[];
} __attribute__ ((packed));

struct tls_extension_t
{
	tls_extension_type_t	type;
	uint16_t 				length;
	uint8_t 				value[];
} __attribute__ ((packed));


struct tls_extensions_t
{
	uint16_t 				length;
	tls_extension_t			value[];
} __attribute__ ((packed));

struct tls_server_name_t
{
	tls_name_type_t			type;
	uint16_t 				length;
	char 					hostname[];
} __attribute__ ((packed));

struct tls_servername_list_t
{
	uint16_t 				length;
	tls_server_name_t		value[];
} __attribute__ ((packed));

struct tls_protocol_name_t
{
	uint8_t 				length;
	char 					value[];
} __attribute__ ((packed));

struct tls_protocol_name_list_t
{
	uint16_t 				length;
	tls_protocol_name_t		value[];
} __attribute__ ((packed));


struct tls_elliptic_curve_list_t
{
	uint16_t 				length;
	tls_ecnamedcurve_t		value[];
} __attribute__ ((packed));

struct tls_ecpoint_format_list_t
{
	uint8_t 				length;
	tls_ecpoint_format_t	value[];
} __attribute__ ((packed));


struct TlsPskIdentity
{
	uint16_t length;
	uint8_t value[];
} __attribute__ ((packed));


struct TlsPskIdentityHint
{
   uint16_t length;
   char value[];
} __attribute__ ((packed));


struct tls_signature_algo_t
{
	tls_hash_algo_t			hash;
	tls_sign_algo_t			signature;
} __attribute__ ((packed));

struct tls_sign_algos_t
{
	uint16_t 				length;
	tls_signature_algo_t 	value[];
} __attribute__ ((packed));

struct TlsCertAuthorities
{
   uint16_t length; //0-1
   uint8_t value[]; //2
} __attribute__ ((packed));

/// Digitally-signed element (SSL 3.0, TLS 1.0 and TLS 1.1)
struct TlsDigitalSignature
{
	uint16_t length;
	uint8_t value[];
} __attribute__ ((packed));


/// Digitally-signed element (TLS 1.2)
struct TlsDigitalSignature2
{
	tls_signature_algo_t algorithm;
	TlsDigitalSignature signature;
} __attribute__ ((packed));


//Forward declaration of TlsContext structure
struct tls_context_t;

/// Pre-shared key callback function
typedef RES_CODE (*TlsPskCallback)(tls_context_t* context, const char* pskIdentity);

/// ECDH key agreement callback function
typedef RES_CODE (*TlsEcdhCallback)(tls_context_t* context);

/// ECDSA signature generation callback function
typedef RES_CODE (*TlsEcdsaSignCallback)(tls_context_t* context,
		const uint8_t* digest, size_t digestLength, EcdsaSignature* signature);

/// ECDSA signature verification callback function
typedef RES_CODE (*TlsEcdsaVerifyCallback)(tls_context_t* context,
		const uint8_t* digest, size_t digestLength, EcdsaSignature* signature);




/* Record:			hash:
 * 		IV
 * 					message
 * 					digest
 * 		pad
 * 		tag
 */
struct record_ctxt_t
{
	size_t msg_len;					// length of the record's message
	size_t rec_len;					// msg_len + iv_len + dig_len + pad_len + tag_len
	size_t iv_len;
	uint8_t dig_len;
	uint8_t pad_len;
	uint8_t tag_len;
	auto_ptr<uint8_t> data;
	tls_record_t  tls_record;

	record_ctxt_t(): msg_len(0), rec_len(0), iv_len(0), dig_len(0), pad_len(0),
			tag_len(0)
	{

	}

	uint8_t* msg_start() const
	{
		uint8_t* ptr;

		ptr = data.get();
		if(ptr)
			ptr += iv_len;
		return ptr;
	}
	uint8_t* msg_end() const
	{
		uint8_t* ptr;

		ptr = data.get();
		if(ptr)
			ptr += + msg_len + iv_len;
		return ptr;
	}

	uint8_t* msg_reserve(uint32_t size)
	{
		uint8_t* ptr;

		ptr = data.get();
		if(size)
		{
			if(ptr == nullptr || size >= dyn_sizeof((void*)ptr))
			{
				ptr = (uint8_t*)tsk_realloc(ptr, size);
				if(ptr)
				{
					data.release();
					data = ptr;
				}
			}
		}
		return ptr;
	}

};

struct tls_randoms_t
{
	tls_random_t client_random;
	tls_random_t server_random;
	auto_ptr<prng_algo_t> prng_algo;

	RES_CODE tls_random_generate(tls_random_t& random);
};

struct tls_seq_num_t
{
	uint8_t data[8];

	void increment();
};

struct tls_xfer_ctxt_t
{
	auto_ptr<cipher_algo_t> cipher_algo;  	//!< Bulk cipher algorithm
	uint8_t*				mac_key; 		//!< MAC key
	uint8_t*				enc_key;        //!< Encryption key
	uint8_t* 				iv;             //!< Read/Write IV
	tls_seq_num_t 			seq_num;     	//!< Sequence number

	tls_xfer_ctxt_t()
	{
		memclr(this, sizeof(*this));
	}
};



enum tls_state_t : char
{
	TLS_STATE_INIT                      = 0,
	TLS_STATE_CLIENT_HELLO              = 1,
	TLS_STATE_SERVER_HELLO              = 2,
	TLS_STATE_SERVER_CERTIFICATE        = 3,
	TLS_STATE_SERVER_KEY_EXCHANGE       = 4,
	TLS_STATE_CERTIFICATE_REQUEST       = 5,
	TLS_STATE_SERVER_HELLO_DONE         = 6,
	TLS_STATE_CLIENT_CERTIFICATE        = 7,
	TLS_STATE_CLIENT_KEY_EXCHANGE       = 8,
	TLS_STATE_CERTIFICATE_VERIFY        = 9,
	TLS_STATE_CLIENT_CHANGE_CIPHER_SPEC = 10,
	TLS_STATE_CLIENT_FINISHED           = 11,
	TLS_STATE_SERVER_CHANGE_CIPHER_SPEC = 12,
	TLS_STATE_SERVER_FINISHED           = 13,
	TLS_STATE_APPLICATION_DATA          = 14,
	TLS_STATE_CLOSING                   = 15,
	TLS_STATE_CLOSED                    = 16
};

enum tls_connection_end_t : char
{
   TLS_CONNECTION_END_CLIENT = 0,
   TLS_CONNECTION_END_SERVER = 1
};

enum tls_client_auth_mode_t : char
{
   TLS_CLIENT_AUTH_NONE     = 0,
   TLS_CLIENT_AUTH_OPTIONAL = 1,
   TLS_CLIENT_AUTH_REQUIRED = 2
};


/// Certificate descriptor
struct TlsCertDesc
{
   CSTRING certChain;      		//!< End entity certificate chain (PEM format)
   const char* privateKey;     		//!< Private key (PEM format)
   size_t privateKeyLength;    		//!< Length of the private key
   size_t privateKeyAvailable;		//!< Size of the private key modulus. Not 0 when private key is not available (e.g. stored in TPM)
   TlsCertificateType type;    		//!< End entity certificate type
   tls_sign_algo_t signAlgo;   		//!< Signature algorithm used to sign the end entity certificate
   tls_hash_algo_t hashAlgo;       	//!< Hash algorithm used to sign the end entity certificate
   tls_ecnamedcurve_t namedCurve;	//!< Named curve used to generate the EC public key
};

struct tls_context_t
{
	CSocket* 			tls_socket;
	tls_state_t 		tls_state; 			//!< context state
	tls_connection_end_t entity;            //!< Client or server operation
	tls_randoms_t 		tls_randoms;

	tls_suite_id_t		cipherSuite;        //!< Negotiated cipher suite
	tls_compression_method_t compressionMethod;  //!< Negotiated compression algorithm
	tls_ecnamedcurve_t	named_curve;        //!< Named curve
	uint8_t				sessionIdLen;   	//!< Length of the session identifier
	uint8_t 			sessionId[32];  	//!< Session identifier

	tls_version_t 		client_version; 	//!< Latest version supported by the client
	tls_version_t 		tls_version; 		//!< Negotiated TLS version

	uint16_t 			num_ciphers; 		//!< Number of ciphers in the list
	const uint16_t*		cipher_suites; 		//!< List of preferred cipher suites
	bool 				change_cipher_sent; //!< A ChangeCipherSpec message has been sent
	bool 				change_cipher_received; //!< A ChangeCipherSpec message has been received
	bool				tls_resume;         //!< The connection is established by resuming a session
	bool				fatal_alert_sent;   //!< A fatal alert message has been sent
	bool				fatal_alert_received; //!< A fatal alert message has been received from the peer
	bool				close_notify_sent;    //!< a closure alert has been sent
	bool 				close_notify_received; //!< A closure alert has been received from the peer
	bool				ecPointFormatExtFound; //!< The EcPointFormats extension has been received
	const tls_chipher_suite_info* cipher_info; //!< Cipher suite info

	CSTRING				server_name;
#if TLS_ALPN_SUPPORT
	CSTRING				protocol_list;
#endif

#if (TLS_PSK_SUPPORT || TLS_RSA_PSK_SUPPORT || \
   TLS_DHE_PSK_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	char* psk;                             	//!< Pre-shared key
	size_t pskLen;                         	//!< Length of the pre-shared key, in bytes
	CSTRING pskIdentity; 					//!< PSK identity
	CSTRING psk_identity_hint;              //!< PSK identity hint
	TlsPskCallback pskCallback;             //!< PSK callback function
#endif
#if (TLS_DH_ANON_SUPPORT || TLS_DHE_RSA_SUPPORT || \
   TLS_DHE_DSS_SUPPORT || TLS_DHE_PSK_SUPPORT)
   DhContext dhContext;                     //!< Diffie-Hellman context
#endif
#if (TLS_ECDH_ANON_SUPPORT || TLS_ECDHE_RSA_SUPPORT || \
   TLS_ECDHE_ECDSA_SUPPORT || TLS_ECDHE_PSK_SUPPORT)
	EcdhContext ecdhContext; //!< ECDH context
#endif

	TlsCertDesc certs[TLS_MAX_CERTIFICATES]; //!< End entity certificates
	uint32_t numCerts; 						 //!< Number of certificates available
	TlsCertDesc *cert; 						 //!< Pointer to the currently selected certificate

	auto_ptr<hash_algo_t> hash_algo; 		//!< Hash algorithm for MAC operations
	auto_ptr<hash_algo_t> prf_hash_algo;	//!< Hash algorithm for PRF operations

	auto_ptr<hash_algo_t> handshake_sha1;   //!< SHA-1 context used to compute verify data
	auto_ptr<hash_algo_t> handshake_hash;   //!< Hash context used to compute verify data (TLS 1.2)

	tls_xfer_ctxt_t wrc;     		     	//!< Write context
	tls_xfer_ctxt_t rdc;	            	//!< Read context

	tls_hash_algo_t signHashAlgo;           //!< Hash algorithm used for signing
	TlsCertificateType peerCertType;      	//!< Peer's certificate type
	tls_client_auth_mode_t client_auth_mode;  //!<Client authentication mode
	bool clientCertRequested;      	        //!< This flag tells whether the client certificate is requested
	uint8_t verify_data_len;                //! <Length of the verify data
	uint8_t verifyData[64];           		//!< Verify data
	uint8_t premasterSecret[TLS_MAX_PREMASTER_SECRET_SIZE]; //!< Premaster secret
	size_t premasterSecretLen; 				//! <Length of the premaster secret
	uint8_t masterSecret[48]; 				//! <Master secret
	uint8_t keyBlock[192];                  //!< Key material

#if TLS_RSA_SIGN_SUPPORT || TLS_RSA_SUPPORT || TLS_DHE_RSA_SUPPORT || TLS_ECDHE_RSA_SUPPORT
	RsaPublicKey peerRsaPublicKey;           //!< Peer's RSA public key
#endif

#if (TLS_DSA_SIGN_SUPPORT || TLS_DHE_DSS_SUPPORT)
	DsaPublicKey peerDsaPublicKey;           //!< Peer's DSA public key
#endif

#if TLS_ECDSA_SIGN_SUPPORT || TLS_ECDHE_ECDSA_SUPPORT
	EcDomainParameters peerEcParams;         //!< Peer's EC domain parameters
	EcPoint peerEcPublicKey;                 //!< Peer's EC public key
#endif

#if TLS_ECC_CALLBACK_SUPPORT
   TlsEcdhCallback ecdhCallback;
   TlsEcdsaSignCallback ecdsaSignCallback;
   TlsEcdsaVerifyCallback ecdsaVerifyCallback;
#endif

	const char *trustedCaList;             	 //!< List of trusted CA (PEM format)
	size_t trustedCaListLen;                 //!< Number of trusted CA in the list
	record_ctxt_t 		last_txrc;
	record_ctxt_t 		last_rxrc;

#if TLS_CLIENT_SUPPORT		// implemented in tls_client.cpp

	RES_CODE tls_client_hello_msg_len(record_ctxt_t* rc);
	void tls_client_make_hello(tls_client_hello_t* message, uint32_t len);
	RES_CODE tls_client_send_hello();
	RES_CODE tls_clent_handshake();
	RES_CODE tlsParsePskIdentityHint(const uint8_t* p, size_t length, size_t* consumed);
	RES_CODE tlsParseServerKeyParams(const uint8_t* p, size_t length, size_t* consumed);
	RES_CODE tlsVerifyServerKeySignature(const uint8_t* p, size_t length,
			const uint8_t* params, size_t paramsLen, size_t* consumed);
	RES_CODE tlsParseServerKeyExchange(record_ctxt_t* rc);
	RES_CODE tlsParseCertificateRequest(record_ctxt_t* rc);
	RES_CODE tlsParseServerHelloDone(record_ctxt_t* rc);
	RES_CODE tls_parse_server_message(record_ctxt_t& rc);
	RES_CODE tls_parse_server_hello(record_ctxt_t* rc);
	RES_CODE tlsSendClientKeyExchange();
	RES_CODE tls_client_key_exchange_msg_len(record_ctxt_t* rc);
	RES_CODE tls_client_key_exchange_msg(tls_client_key_exchange_t* message, uint32_t len);
	RES_CODE tlsSendCertificateVerify();
	RES_CODE tls_make_Certificate_verify(tls_certificate_verify_t** msg, size_t* len);
#endif

	//server
#if TLS_SERVER_SUPPORT
	RES_CODE tlsServerHandshake();
	RES_CODE tlsSendServerHello();
	RES_CODE tlsSendServerKeyExchange();
	RES_CODE tlsSendCertificateRequest();
	RES_CODE tlsSendServerHelloDone();
	RES_CODE tlsParseClientMessage();
	RES_CODE tls_server_hello_msg_len(record_ctxt_t* rc);
	RES_CODE tls_server_make_hello(tls_server_hello_t* message, uint32_t len);
	RES_CODE tls_make_server_key_exchange_msg(uint8_t*& msg, size_t& len);
	RES_CODE tls_certificate_request_msg_len(record_ctxt_t* rc);
	RES_CODE tls_certificate_request_msg(tls_certificate_request_t* msg, size_t len);
	RES_CODE tlsParseClientHello(record_ctxt_t* rc);
	RES_CODE tlsParseClientKeyExchange(record_ctxt_t* rc);
	RES_CODE tlsParseCertificateVerify(record_ctxt_t* rc);
	RES_CODE tlsParsePskIdentity(const uint8_t* p, size_t length, size_t* consumed);
	RES_CODE tlsParseClientKeyParams(const uint8_t* p, size_t length, size_t* consumed);

#endif


	//common
	RES_CODE tls_parse_change_cipherspec(record_ctxt_t* rc);
	RES_CODE tls_parse_alert(record_ctxt_t* rc);
	RES_CODE tls_parse_certificate(record_ctxt_t* rc);
	RES_CODE tlsParseFinished(record_ctxt_t* rc);
	RES_CODE tlsSendCertificate();
	static uint32_t tls_get_alert_len();
	RES_CODE tls_certificate_msg_len(record_ctxt_t* rc);
	RES_CODE tls_make_certificate_msg(tls_certificate_msg_t* message, uint32_t len);
	RES_CODE tlsSendChangeCipherSpec();
	RES_CODE tlsSendFinished();
	RES_CODE tlsSendAlert(tls_alert_level_t level, tls_alert_description_t description);
	RES_CODE tlsHandshake();

	// record
	RES_CODE tls_record_get_lens(record_ctxt_t* lens);
	RES_CODE tls_record_write(record_ctxt_t* rc);
	RES_CODE tls_record_encrypt(record_ctxt_t* rc);
	RES_CODE tls_record_decrypt(record_ctxt_t* rc);
	RES_CODE tls_record_read(record_ctxt_t* rc);
	RES_CODE sslComputeMac(record_ctxt_t* rc, tls_xfer_ctxt_t& xfer, uint8_t *mac);
	RES_CODE tls_message_read(record_ctxt_t* rc);

	// misc
	RES_CODE tls_set_version(tls_version_t version);
	RES_CODE tls_set_ciphersuite(tls_suite_id_t suite);
	RES_CODE tls_set_compression_method(tls_compression_method_t method);
	RES_CODE tls_handshake_hash_init();
	void tls_handshake_hash_update(const void *data, uint32_t len);
	RES_CODE tlsFinalizeHandshakeHash(const hash_algo_t* hash,
			const char *label, uint8_t *output);
	RES_CODE tls_generate_keys();
	RES_CODE tls_init_decryption_engine();
	RES_CODE tlsInitEncryptionEngine();
	RES_CODE tlsVerifyEcdsaSignature(const uint8_t* digest, size_t digestLength,
			const TlsDigitalSignature* signature);
	RES_CODE tlsSelectSignHashAlgo(tls_sign_algo_t signAlgo,
			const tls_sign_algos_t* supportedSignAlgos);
	RES_CODE tlsComputeVerifyData(tls_connection_end_t ce);
	RES_CODE tlsGeneratePskPremasterSecret();
	RES_CODE tlsGenerateDsaSignature(const uint8_t* digest,
	   size_t digestLength, uint8_t** signature, size_t* signatureLength);
	RES_CODE tlsGenerateEcdsaSignature(const uint8_t* digest,
	   size_t digestLength, uint8_t** signature, size_t* signatureLength);
	void tlsProcessError(RES_CODE errorCode);
	RES_CODE tlsSelectNamedCurve(const tls_elliptic_curve_list_t* curveList);


	// tls
	tls_context_t();

	RES_CODE tlsSetConnectionEnd(tls_connection_end_t ce);
	RES_CODE tlsSetPrng(prng_algo_t* prngAlgo);
	RES_CODE tls_connect(CSocket* sock);
	RES_CODE tls_write(const void* data, size_t length, size_t* written);
	RES_CODE tls_read(void* data, size_t size, size_t* received, uint32_t flags);
	RES_CODE tls_shutdown();
	RES_CODE tls_shutdown_ex(bool waitForCloseNotify);


	RES_CODE tls_read_cbk(void* data, uint32_t len);
	RES_CODE tls_write_cbk(const void* data, uint32_t len);
};

void tls_dump_record(const record_ctxt_t* rc);

RES_CODE private_certificate_verify(tls_context_t* context, const uint8_t *data, size_t length);
RES_CODE private_rsa_signature(tls_context_t* context, tls_certificate_verify_t** msg, size_t* len);

#endif /* SERVICES_SECURITY_TLS_TLS_H_ */
