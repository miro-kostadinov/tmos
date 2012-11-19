/*
 * crc16.h
 *
 *  Created on: Mar 7, 2012
 *      Author: miro
 */

#ifndef CRC16_H_
#define CRC16_H_
extern "C"
{

unsigned int CalculateCRC16(unsigned int crc, const char* buf, unsigned int len);
unsigned int CalculateCRC16CCITT(unsigned int crc, const char* buf, unsigned int len);

}
#endif /* CRC16_H_ */



