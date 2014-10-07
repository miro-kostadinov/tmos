/*
 * crc32.h
 *
 *  Created on: 2008-10-1
 *      Author: Miroslav Kostadinov
 */

#ifndef CRC32_H_
#define CRC32_H_

#define INIT_REFLECTED 0xffffffff
#define XOROT 0xffffffff

extern "C"
{
extern const unsigned long g_crctable[256];

unsigned int CalculateCRC32(const unsigned char * buf, unsigned int len);
unsigned int CalculateCRC32Slow(unsigned int crc, const void * buf,
		unsigned int len);
unsigned int crc32_4_le(const void* buf, unsigned int len);

}
#endif /* CRC32_H_ */
