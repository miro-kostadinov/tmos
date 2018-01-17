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

typedef struct
{
	int64_t quot;
	int64_t rem;
} lldiv_t;

typedef struct
{
	uint64_t quot;
	uint64_t rem;
} ulldiv_t;

unsigned long long udivdi3(unsigned long long u, unsigned long long v);
long long divdi3(long long u, long long v);
long long int llabs(long long int u);

/* Workaround for __value_in_regs
 *
 */
int64_t tmos_ldivmod(int64_t u, int64_t v, int64_t* rem);
uint64_t tmos_uldivmod(uint64_t u, uint64_t v, uint64_t* res);

#ifdef __cplusplus
}
#endif	/* end C++ */

#endif /* TMOS_LLONG_H_ */
