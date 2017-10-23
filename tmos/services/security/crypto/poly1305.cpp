/*
 * poly1305.cpp
 *
 *  Created on: Jun 19, 2017
 *     License: see LICENSE file
 */

#include <tmos.h>
#include <poly1305.h>


#if POLY1305_SUPPORT

extern "C" void poly1305_ctxt_process_block(uint32_t* acc_r, uint8_t* buf);
extern "C" void poly1305_ctxt_result(uint32_t* acc_r, uint8_t *tag);

void poly1305_ctxt_t::Reset(const uint32_t *key)
{
	//The 256-bit key is partitioned into two parts, called r and s

	//Certain bits of r are required to be 0
	r[0] = key[0] & 0x0FFFFFFF;
	r[1] = key[1] & 0x0FFFFFFC;
	r[2] = key[2] & 0x0FFFFFFC;
	r[3] = key[3] & 0x0FFFFFFC;

	s[0] = key[4];
	s[1] = key[5];
	s[2] = key[6];
	s[3] = key[7];

	//The accumulator is set to zero
	acc[0] = 0;
	acc[1] = 0;
	acc[2] = 0;
	acc[3] = 0;
	acc[4] = 0;

	//Number of bytes in the buffer
	size = 0;
}

void poly1305_ctxt_t::Input(const void *data, size_t len)
{
	size_t n;

	//Process the incoming data
	while (len > 0)
	{
		n = min(len, 16 - size);

		memcpy(buffer + size, data, n);

		size += n;
		data = (uint8_t *) data + n;
		len -= n;

		//Process message in 16-byte blocks
		if (size == 16)
		{
			//Transform the 16-byte block
			acc[4] += 1;
			poly1305_ctxt_process_block(acc, buffer);
			size = 0;
		}
	}
}

void poly1305_ctxt_t::Result(uint8_t *tag)
{
	//Process the last block
	if (size != 0)
	{
		buffer[size++] = 0x01;

		//If the resulting block is not 17 bytes long (the last block),
		//pad it with zeros
		while (size < 16)
			buffer[size++] = 0x00;
		poly1305_ctxt_process_block (acc, buffer);
	}

	poly1305_ctxt_result(acc, tag);
}


#endif // POLY1305_SUPPORT
