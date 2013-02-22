/*
 * hmac_sha1.h
 *
 *  Created on: Jun 11, 2012
 *      Author: stanly
 */

#ifndef HMAC_SHA1_H_
#define HMAC_SHA1_H_

#include <tmos.h>
#include <str_list.h>
#include "sha_1.h"

#define SHA1_BLOCK_SIZE		64
#define SHA1_DIGEST_LENGTH	20

class hmac_sha1: public SHA1
{
public:
	hmac_sha1():
		SHA1(), hmac_computed(false), m_pad(NULL), SHA1_key(NULL)
	{}

	virtual ~hmac_sha1()
	{
		if(m_pad)
			delete m_pad;
		if(SHA1_key)
			delete SHA1_key;
	}

	bool Reset(const char* key, unsigned int key_len);

    bool Result(CSTRING& message);
    bool Result(unsigned *digest_array);
    bool Result(char* digest_array);

private:
    bool  hmac_computed;
	char* m_pad;
	char* SHA1_key;

    bool Result(void);

};

#endif /* HMAC_SHA1_H_ */
