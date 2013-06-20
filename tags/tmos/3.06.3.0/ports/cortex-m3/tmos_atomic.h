/*
 * tmos_atomic.h
 *
 *  Created on: Mar 27, 2013
 *      Author: miro
 */

#ifndef TMOS_ATOMIC_H_
#define TMOS_ATOMIC_H_


#ifdef __cplusplus
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	inline  C++ atomic functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//-------------------- fetch -------------------------------------------------//

static inline int atomic_fetch(volatile char* obj)
{
	int res;
	asm volatile (
		"	ldrexb			%0, [%1]			\n\t"

	  : "=r"(res)
	  : "r" (obj)
	  :
	  );
	return res;
}

static inline int atomic_fetch(volatile short* obj)
{
	int res;
	asm volatile (
		"	ldrexh			%0, [%1]			\n\t"

	  : "=r"(res)
	  : "r" (obj)
	  :
	  );
	return res;
}

static inline int atomic_fetch(volatile int* obj)
{
	int res;
	asm volatile (
		"	ldrex			%0, [%1]			\n\t"

	  : "=r"(res)
	  : "r" (obj)
	  :
	  );
	return res;
}

//-------------------- store -------------------------------------------------//
static inline void atomic_clrex()
{
	asm volatile (
		"	clrex					\n\t"

	  :  :	  :
	  );
}

static inline int atomic_store(volatile char* obj, int arg)
{
	int res;
	asm volatile (
		"	strexb			%0, %2, [%1]		\n\t"

	  : "=&r"(res)
	  : "r" (obj), "r" (arg)
	  : "memory"
	  );
	return res;
}

static inline int atomic_store(volatile short* obj, int arg)
{
	int res;
	asm volatile (
		"	strexh			%0, %2, [%1]		\n\t"

	  : "=&r"(res)
	  : "r" (obj), "r" (arg)
	  : "memory"
	  );
	return res;
}

static inline int atomic_store(volatile int* obj, int arg)
{
	int res;
	asm volatile (
		"	strex			%0, %2, [%1]		\n\t"

	  : "=&r"(res)
	  : "r" (obj), "r" (arg)
	  : "memory"
	  );
	return res;
}




#else
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	inline  C atomic functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// not implemented

#endif


#endif /* TMOS_ATOMIC_H_ */
