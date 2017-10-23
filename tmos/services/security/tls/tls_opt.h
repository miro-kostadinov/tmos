/*
 * tls_opt.h
 *
 *  Created on: Jun 6, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_TLS_TLS_OPT_H_
#define SERVICES_SECURITY_TLS_TLS_OPT_H_

#include <tmos.h>


//TLS version numbers
#define SSL_VERSION_3_0 0x0300u
#define TLS_VERSION_1_0 0x0301u
#define TLS_VERSION_1_1 0x0302u
#define TLS_VERSION_1_2 0x0303u

/// TLS client functionality
#ifndef TLS_CLIENT_SUPPORT
#define TLS_CLIENT_SUPPORT	1
#endif

/// TLS server functionality
#ifndef TLS_SERVER_SUPPORT
#define TLS_SERVER_SUPPORT	0
#endif


//Minimum TLS version that can be negotiated
#ifndef TLS_MIN_VERSION
   #define TLS_MIN_VERSION TLS_VERSION_1_0
#endif

//Maximum TLS version that can be negotiated
#ifndef TLS_MAX_VERSION
   #define TLS_MAX_VERSION TLS_VERSION_1_2
#endif

//Session resumption mechanism
#ifndef TLS_SESSION_RESUME_SUPPORT
   #define TLS_SESSION_RESUME_SUPPORT 1
#endif

//Lifetime of session cache entries
#ifndef TLS_SESSION_CACHE_LIFETIME
   #define TLS_SESSION_CACHE_LIFETIME 3600000
#endif

//ECC callback functions
#ifndef TLS_ECC_CALLBACK_SUPPORT
   #define TLS_ECC_CALLBACK_SUPPORT 0
#endif

//SNI (Server Name Indication) extension
#ifndef TLS_SNI_SUPPORT
   #define TLS_SNI_SUPPORT 1
#endif

//ALPN (Application-Layer Protocol Negotiation) extension
#ifndef TLS_ALPN_SUPPORT
   #define TLS_ALPN_SUPPORT 0
#endif

//Maximum number of certificates the end entity can load
#ifndef TLS_MAX_CERTIFICATES
   #define TLS_MAX_CERTIFICATES 3
#endif

//RSA key exchange support
#ifndef TLS_RSA_SUPPORT
   #define TLS_RSA_SUPPORT 1
#endif

//DHE_RSA key exchange support
#ifndef TLS_DHE_RSA_SUPPORT
   #define TLS_DHE_RSA_SUPPORT 1
#endif

//DHE_DSS key exchange support
#ifndef TLS_DHE_DSS_SUPPORT
   #define TLS_DHE_DSS_SUPPORT 0
#endif

//DH_anon key exchange support
#ifndef TLS_DH_ANON_SUPPORT
   #define TLS_DH_ANON_SUPPORT 0
#endif

//ECDHE_RSA key exchange support
#ifndef TLS_ECDHE_RSA_SUPPORT
   #define TLS_ECDHE_RSA_SUPPORT 1
#endif

//ECDHE_ECDSA key exchange support
#ifndef TLS_ECDHE_ECDSA_SUPPORT
   #define TLS_ECDHE_ECDSA_SUPPORT 1
#endif

//ECDH_anon key exchange support
#ifndef TLS_ECDH_ANON_SUPPORT
   #define TLS_ECDH_ANON_SUPPORT 0
#endif

//PSK key exchange support
#ifndef TLS_PSK_SUPPORT
   #define TLS_PSK_SUPPORT 0
#endif

//RSA_PSK key exchange support
#ifndef TLS_RSA_PSK_SUPPORT
   #define TLS_RSA_PSK_SUPPORT 0
#endif

//DHE_PSK key exchange support
#ifndef TLS_DHE_PSK_SUPPORT
   #define TLS_DHE_PSK_SUPPORT 0
#endif

//ECDHE_PSK key exchange support
#ifndef TLS_ECDHE_PSK_SUPPORT
   #define TLS_ECDHE_PSK_SUPPORT 0
#endif

//RSA signature capability
#ifndef TLS_RSA_SIGN_SUPPORT
   #define TLS_RSA_SIGN_SUPPORT 1
#endif

//DSA signature capability
#ifndef TLS_DSA_SIGN_SUPPORT
   #define TLS_DSA_SIGN_SUPPORT 0
#endif

//ECDSA signature capability
#ifndef TLS_ECDSA_SIGN_SUPPORT
   #define TLS_ECDSA_SIGN_SUPPORT 1
#endif

//Stream cipher support
#ifndef TLS_STREAM_CIPHER_SUPPORT
   #define TLS_STREAM_CIPHER_SUPPORT 1
#endif

//CBC block cipher support
#ifndef TLS_CBC_CIPHER_SUPPORT
   #define TLS_CBC_CIPHER_SUPPORT 1
#endif

//CCM AEAD support
#ifndef TLS_CCM_CIPHER_SUPPORT
   #define TLS_CCM_CIPHER_SUPPORT 0
#endif

//CCM_8 AEAD support
#ifndef TLS_CCM_8_CIPHER_SUPPORT
   #define TLS_CCM_8_CIPHER_SUPPORT 0
#endif

//GCM AEAD support
#ifndef TLS_GCM_CIPHER_SUPPORT
   #define TLS_GCM_CIPHER_SUPPORT 1
#endif

//ChaCha20Poly1305 AEAD support
#ifndef TLS_CHACHA20_POLY1305_SUPPORT
   #define TLS_CHACHA20_POLY1305_SUPPORT 1
#endif

//RC4 cipher support
#ifndef TLS_RC4_SUPPORT
   #define TLS_RC4_SUPPORT 1
#endif

//IDEA cipher support
#ifndef TLS_IDEA_SUPPORT
   #define TLS_IDEA_SUPPORT 0
#endif

//DES cipher support
#ifndef TLS_DES_SUPPORT
   #define TLS_DES_SUPPORT 0
#endif

//Triple DES cipher support
#ifndef TLS_3DES_SUPPORT
   #define TLS_3DES_SUPPORT 1
#endif

//AES cipher support
#ifndef TLS_AES_SUPPORT
   #define TLS_AES_SUPPORT 1
#endif

//Camellia cipher support
#ifndef TLS_CAMELLIA_SUPPORT
   #define TLS_CAMELLIA_SUPPORT 1
#endif

//SEED cipher support
#ifndef TLS_SEED_SUPPORT
   #define TLS_SEED_SUPPORT 0
#endif

//ARIA cipher support
#ifndef TLS_ARIA_SUPPORT
   #define TLS_ARIA_SUPPORT 0
#endif

//MD5 hash support
#ifndef TLS_MD5_SUPPORT
   #define TLS_MD5_SUPPORT 1
#endif

//SHA-1 hash support
#ifndef TLS_SHA1_SUPPORT
   #define TLS_SHA1_SUPPORT 1
#endif

//SHA-224 hash support
#ifndef TLS_SHA224_SUPPORT
   #define TLS_SHA224_SUPPORT 1
#endif

//SHA-256 hash support
#ifndef TLS_SHA256_SUPPORT
   #define TLS_SHA256_SUPPORT 1
#endif

//SHA-384 hash support
#ifndef TLS_SHA384_SUPPORT
   #define TLS_SHA384_SUPPORT 1
#endif

//SHA-512 hash support
#ifndef TLS_SHA512_SUPPORT
   #define TLS_SHA512_SUPPORT 0
#endif

//secp160k1 elliptic curve support
#ifndef TLS_SECP160K1_SUPPORT
   #define TLS_SECP160K1_SUPPORT 0
#endif

//secp160r1 elliptic curve support
#ifndef TLS_SECP160R1_SUPPORT
   #define TLS_SECP160R1_SUPPORT 0
#endif

//secp160r2 elliptic curve support
#ifndef TLS_SECP160R2_SUPPORT
   #define TLS_SECP160R2_SUPPORT 0
#endif

//secp192k1 elliptic curve support
#ifndef TLS_SECP192K1_SUPPORT
   #define TLS_SECP192K1_SUPPORT 0
#endif

//secp192r1 elliptic curve support (NIST P-192)
#ifndef TLS_SECP192R1_SUPPORT
   #define TLS_SECP192R1_SUPPORT 1
#endif

//secp224k1 elliptic curve support
#ifndef TLS_SECP224K1_SUPPORT
   #define TLS_SECP224K1_SUPPORT 0
#endif

//secp224r1 elliptic curve support (NIST P-224)
#ifndef TLS_SECP224R1_SUPPORT
   #define TLS_SECP224R1_SUPPORT 1
#endif

//secp256k1 elliptic curve support
#ifndef TLS_SECP256K1_SUPPORT
   #define TLS_SECP256K1_SUPPORT 0
#endif

//secp256r1 elliptic curve support (NIST P-256)
#ifndef TLS_SECP256R1_SUPPORT
   #define TLS_SECP256R1_SUPPORT 1
#endif

//secp384r1 elliptic curve support (NIST P-384)
#ifndef TLS_SECP384R1_SUPPORT
   #define TLS_SECP384R1_SUPPORT 1
#endif

//secp521r1 elliptic curve support (NIST P-521)
#ifndef TLS_SECP521R1_SUPPORT
   #define TLS_SECP521R1_SUPPORT 1
#endif

//brainpoolP256r1 elliptic curve support
#ifndef TLS_BRAINPOOLP256R1_SUPPORT
   #define TLS_BRAINPOOLP256R1_SUPPORT 0
#endif

//brainpoolP384r1 elliptic curve support
#ifndef TLS_BRAINPOOLP384R1_SUPPORT
   #define TLS_BRAINPOOLP384R1_SUPPORT 0
#endif

//brainpoolP512r1 elliptic curve support
#ifndef TLS_BRAINPOOLP512R1_SUPPORT
   #define TLS_BRAINPOOLP512R1_SUPPORT 0
#endif

//Minimum acceptable size for Diffie-Hellman prime modulus
#ifndef TLS_MIN_DH_MODULUS_SIZE
   #define TLS_MIN_DH_MODULUS_SIZE 1024
#endif

//Maximum acceptable size for Diffie-Hellman prime modulus
#ifndef TLS_MAX_DH_MODULUS_SIZE
   #define TLS_MAX_DH_MODULUS_SIZE 4096
#endif

//Minimum acceptable size for RSA modulus
#ifndef TLS_MIN_RSA_MODULUS_SIZE
   #define TLS_MIN_RSA_MODULUS_SIZE 1024
#endif

//Maximum acceptable size for RSA modulus
#ifndef TLS_MAX_RSA_MODULUS_SIZE
   #define TLS_MAX_RSA_MODULUS_SIZE 4096
#endif

//Minimum acceptable size for DSA prime modulus
#ifndef TLS_MIN_DSA_MODULUS_SIZE
   #define TLS_MIN_DSA_MODULUS_SIZE 1024
#endif

//Maximum acceptable size for DSA prime modulus
#ifndef TLS_MAX_DSA_MODULUS_SIZE
   #define TLS_MAX_DSA_MODULUS_SIZE 4096
#endif

//Maximum size for premaster secret
#ifndef TLS_MAX_PREMASTER_SECRET_SIZE
   #define TLS_MAX_PREMASTER_SECRET_SIZE 256
#endif

//Maximum plaintext record length
#define TLS_MAX_RECORD_LENGTH 16384u

//REV 16-bit integer
#define REV16(x) ( (((x) & 0xFF) << 8) | ((x) >> 8) )


#endif /* SERVICES_SECURITY_TLS_TLS_OPT_H_ */
