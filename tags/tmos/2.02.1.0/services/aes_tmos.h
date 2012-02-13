/*
 * aes_tmos.h
 *
 *  Created on: 14.01.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef AES_TMOS_H_
#define AES_TMOS_H_

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define USE_ENCRYPTION
#define ENCRYPTION_ECB
#define ENCRYPTION_KEY_LENGTH	16
#define ENCRYPTION_BLOCK_LENGTH 16


#define ENC_BSIZE               (ENCRYPTION_BLOCK_LENGTH / 4)
#define ENC_KSIZE               (ENCRYPTION_KEY_LENGTH / 4)
#if (ENC_KSIZE >= ENC_BSIZE)
  #define ENC_ROUNDS  	        (ENC_KSIZE + 6)
#else
  #define ENC_ROUNDS           	(ENC_BSIZE + 6)
#endif


// AES CONTEXT structure offsets
#define AES_CONTEXT_RNDS		0
#define AES_CONTEXT_SB			4
#define AES_CONTEXT_FT			8
#define AES_CONTEXT_RSB			12
#define AES_CONTEXT_RT			16
#define AES_CONTEXT_KEY			20


#ifndef __ASSEMBLY__

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
	uint32_t  rounds;                    	// must be first element
	uint8_t*  Sb;							// Forward SBox table
	uint32_t* FT;							// Forward Table
	uint8_t*  RSb;							// Reverse SBox table
	uint32_t* RT;							// Reverse Table
	uint32_t  expanded_key[68];				// key after processing for rounds
											// ENC_KSIZE*4+36=68
} aes_context;

void aes_tmos_en_key(const aes_context* ctxt, unsigned char* input, unsigned key_size);
void aes_tmos_de_key(const aes_context* ctxt, unsigned char* input, unsigned key_size);

void aes_tmos_de_ecb(const aes_context* ctxt, const unsigned char* input,
		unsigned char* output, unsigned blocks);

#ifdef __cplusplus
}
#endif


#endif /* __ASSEMBLY__ */

#endif /* AES_TMOS_H_ */
