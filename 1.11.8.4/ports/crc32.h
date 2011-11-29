/*
 * crc32.h
 *
 *  Created on: 2008-10-1
 *      Author: Miroslav Kostadinov
 */

#ifndef CRC32_H_
#define CRC32_H_
extern "C"
{

unsigned int CalculateCRC32(const unsigned char * buf, unsigned int len);

}
#endif /* CRC32_H_ */
