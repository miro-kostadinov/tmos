/*
 * tmos_llong.h
 *
 *  Created on: Apr 27, 2012
 *      Author: miro
 */

#ifndef TMOS_LLONG_H_
#define TMOS_LLONG_H_

#ifdef __cplusplus
extern "C"
{
#endif

unsigned long long udivdi3(unsigned long long u, unsigned long long v);
long long divdi3(long long u, long long v);
unsigned long long llabs(long long u);

#ifdef __cplusplus
}
#endif	/* end C++ */

#endif /* TMOS_LLONG_H_ */
