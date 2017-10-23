/*
 * base64.h
 *
 *  Created on: Aug 1, 2017
 *     License: see LICENSE file
 */

#ifndef SERVICES_SECURITY_CRYPTO_BASE64_H_
#define SERVICES_SECURITY_CRYPTO_BASE64_H_

#include <tmos.h>
#include <crypto.h>


//Base64 encoding related functions
void base64Encode(const void* input, size_t inputLength, char* output,
		size_t* outputLength);

RES_CODE base64Decode(const char* input, size_t inputLength, void* output,
		size_t* outputLength);


#endif /* SERVICES_SECURITY_CRYPTO_BASE64_H_ */
