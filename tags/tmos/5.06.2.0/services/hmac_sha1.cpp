/*
 * hmac_sha1.cpp
 *
 *  Created on: Jun 11, 2012
 *      Author: stanly
 */

#include "hmac_sha1.h"

bool hmac_sha1::Reset(const char* key, unsigned int key_len)
{
	SHA1::Reset();

	hmac_computed = false;

	if(!SHA1_key)
    	SHA1_key = new char [SHA1_BLOCK_SIZE];
    if(!m_pad)
    	m_pad = new char [SHA1_BLOCK_SIZE];

    if(!m_pad || !SHA1_key)
    	return false;

    memset(SHA1_key, 0, SHA1_BLOCK_SIZE);
    /* repeated 64 times for values in ipad */
    memset(m_pad, 0x36, SHA1_BLOCK_SIZE);


    if (key_len > SHA1_BLOCK_SIZE)
    {
    		SHA1::Input(key, key_len);
    		SHA1::Result(SHA1_key);
    		key_len = SHA1_DIGEST_LENGTH;
    }
    else
            memcpy(SHA1_key, key, key_len);


    for (int i=0; i < SHA1_BLOCK_SIZE; i++)
            m_pad[i] ^= SHA1_key[i];

    SHA1::Reset();
    SHA1::Input(m_pad, SHA1_BLOCK_SIZE);

    return true;
}


bool hmac_sha1::Result(CSTRING& message)
{
	if(Result() && &message)
		return SHA1::Result(message);
	return false;
}
bool hmac_sha1::Result(unsigned* digest_array)
{
	if(Result() && digest_array)
		return SHA1::Result(digest_array);
	return false;
}


bool hmac_sha1::Result(char* digest_array)
{
	if(Result() && digest_array)
		return SHA1::Result(digest_array);
	return false;
}

bool hmac_sha1::Result(void)
{
    if(!hmac_computed)
    {
        if(!m_pad || !SHA1_key || Corrupted)
        	return false;

        char* result = new char [SHA1_DIGEST_LENGTH];
		if(result)
		{
			memset(m_pad, 0x5c, SHA1_BLOCK_SIZE);

			SHA1::Result(result);
			for (int i=0; i < SHA1_BLOCK_SIZE; i++)
				  m_pad[i] ^= SHA1_key[i];

			SHA1::Reset();
			SHA1::Input(m_pad, SHA1_BLOCK_SIZE);
			SHA1::Input(result, SHA1_DIGEST_LENGTH);
			hmac_computed = true;

			delete result;
		}
    }

	return hmac_computed;
}
