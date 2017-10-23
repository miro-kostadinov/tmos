/*
 * crypto.cpp
 *
 *  Created on: Jul 18, 2017
 *     License: see LICENSE file
 */
#include <tmos.h>
#include <crypto.h>


void hash_algo_t::Pad(uint8_t data, size_t len)
{
    while (len--)
    {
    	buf8[indx++] = data;
    	if(indx == hash_info->algo_info.block_size)
    	{
			process_block(buf32);
			indx = 0;
    	}
    }
}

void hash_algo_t::Input(const void* data, size_t len)
{
	const uint8_t*msg = (const uint8_t*)data;
	uint32_t size = hash_info->algo_info.block_size;

    while (len)
    {
    	if(indx || len < size)
    	{
    		uint32_t n;

    		n = min(len, size-indx);

			memcpy(buf8 + indx, msg, n);

			indx += n;
			if (indx < size)
				break;

			msg += n;
			len -= n;

			process_block(buf32);
			indx = 0;

    	}

    	while(len >= size)
    	{
    		process_block((const uint32_t*)(void*)msg);
    		len -= size;
    		msg += size;
    	}
    }
}


// must be overloaded
WEAK uint32_t get_gmt_seconds()
{
	TRACELN1("GMT seconds not provided!");
	return 0;
}


