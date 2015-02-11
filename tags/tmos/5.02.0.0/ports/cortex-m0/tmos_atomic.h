/*
 * tmos_atomic.h
 *
 *  Created on: Mar 27, 2013
 *      Author: miro
 */

#ifndef TMOS_ATOMIC_H_
#define TMOS_ATOMIC_H_

#include <core_instr.h>

#ifdef __cplusplus
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	inline  C++ atomic functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


//-------------------- fetch -------------------------------------------------//

static inline int atomic_fetch(volatile char* obj)
{
	__disable_irq();
	return *obj;
}

static inline int atomic_fetch(volatile short* obj)
{
	__disable_irq();
	return *obj;
}

static inline int atomic_fetch(volatile int* obj)
{
	__disable_irq();
	return *obj;
}

//-------------------- store -------------------------------------------------//
static inline void atomic_clrex()
{
	__enable_irq();
}

static inline int atomic_store(volatile char* obj, int arg)
{
	*obj = arg;
	__enable_irq();
	return 0;
}

static inline int atomic_store(volatile short* obj, int arg)
{
	*obj = arg;
	__enable_irq();
	return 0;
}

static inline int atomic_store(volatile int* obj, int arg)
{
	*obj = arg;
	__enable_irq();
	return 0;
}




#else
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 	inline  C atomic functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	// not implemented

#endif


#endif /* TMOS_ATOMIC_H_ */
