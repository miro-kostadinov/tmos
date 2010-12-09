//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2008 Informa Ltd.
//
// TMOS is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later version.
//
// TMOS is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
//==========================================================================
//
// Author(s):     Miroslav Kostadinov
// Date:          2008-Apr-03
// Description:   System functions implemented as SWI
//
//========================================================================
#ifndef TMOS_SWI_H
#define TMOS_SWI_H

#ifndef TMOS_H
#error "use tmos.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 System Service Routines
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifdef usr_task_suspend_swi
inline static   void usr_task_suspend(Task* volatile task)
{
	register Task *_task asm("r0") = task;

	asm volatile ("swi %1"
	  : "=r"(_task)
	  : "I" (usr_task_suspend_swi),  "0" (_task)
	  : "r1", "r2", "r3", "r12", "r14");
}
#endif


#ifdef usr_task_schedule_swi
inline static   void usr_task_schedule(Task* volatile task)
{
	register Task *_task asm("r0") = task;

	asm volatile ("swi %1"
	  : "=r"(_task)
	  : "I" (usr_task_schedule_swi),  "0" (_task)
	  : "r1", "r2", "r3", "r12", "r14");
}
#endif


#ifdef tsk_sleep_swi
inline static   void tsk_sleep( unsigned int volatile time)
{
	register unsigned int _time asm("r0") = time;

	asm volatile ("swi %1"
	  : "=r"(_time)
	  : "I" (tsk_sleep_swi), "0" (_time)
	  : "r1", "r2", "r3", "r12");
}
#endif


#ifdef tsk_sleep_rel_swi
inline static   void tsk_sleep_rel( unsigned int volatile time)
{
	register unsigned int _time asm("r0") = time;

	asm volatile ("swi %1"
	  : "=r"(_time)
	  : "I" (tsk_sleep_rel_swi), "0" (_time)
	  : "r1", "r2", "r3", "r12");
}
#endif


#ifdef tsk_sleep_until_swi
inline static   void tsk_sleep_until( unsigned int volatile time)
{
	register unsigned int _time asm("r0") = time;

	asm volatile ("swi %1"
	  : "=r"(_time)
	  : "I" (tsk_sleep_until_swi), "0" (_time)
	  : "r1", "r2", "r3", "r12");
}
#endif


#ifdef sys_increment_ticks_swi
inline static   void sys_increment_ticks(unsigned int volatile time)
{
	register unsigned int _time asm("r0") = time;

	asm volatile ("swi %1"
	  : "=r"(_time)
	  : "I" (sys_increment_ticks_swi), "0" (_time)
	  : "r1", "r2", "r3", "r12", "r14");
}
#endif


#ifdef usr_send_signal_swi
inline static   void usr_send_signal(Task* volatile task, unsigned int volatile signal)
{
	register Task *_task asm("r0") = task;
	register unsigned int _signal asm("r1") = signal;

	asm volatile ("swi %2"
	  : "=r" (_task), "=r"(_signal)
	  : "I" (usr_send_signal_swi), "0" (_task), "1"(_signal)
	  : "r2", "r3", "r12", "r14");
}
inline static   void tsk_send_signal(Task* volatile task, unsigned int volatile signal)
{
	register Task *_task asm("r0") = task;
	register unsigned int _signal asm("r1") = signal;

	asm volatile ("swi %2"
	  :  "=r" (_task), "=r"(_signal)
	  : "I" (usr_send_signal_swi), "0" (_task), "1"(_signal)
	  : "r2", "r3", "r12");
}
#endif


#ifdef tsk_try_signal_swi
inline static   unsigned int tsk_try_signal(unsigned int volatile signal)
{
	register unsigned int _signal asm("r0") = signal;

	asm volatile ("swi %1"
	  : "=r"(_signal)
	  : "I" (tsk_try_signal_swi), "0"(_signal)
	  : "r1", "r2", "r3", "r12");
	return _signal;
}
#endif


#ifdef tsk_get_signal_swi
inline static   unsigned int tsk_get_signal(unsigned int volatile signal)
{
	register unsigned int _signal asm("r0") = signal;

	asm volatile ("swi %1"
	  : "=r"(_signal)
	  : "I" (tsk_get_signal_swi), "0"(_signal)
	  : "r1", "r2", "r3", "r12");
	return _signal;
}
#endif


#ifdef tsk_wait_signal_swi
inline static   unsigned int tsk_wait_signal(unsigned int volatile signal, unsigned int volatile time)
{
	register unsigned int _signal asm("r0") = signal;
	register unsigned int _time asm("r1") = time;

	asm volatile ("swi %2"
	  : "=r"(_signal), "=r"(_time)
	  : "I" (tsk_wait_signal_swi), "0"(_signal), "1"(_time)
	  : "r2", "r3", "r12");
	return _signal;
}
#endif


#ifdef tsk_resume_wait_signal_swi
inline static   unsigned int tsk_resume_wait_signal(unsigned int volatile signal)
{
	register unsigned int _signal asm("r0") = signal;

	asm volatile ("swi %1"
	  : "=r"(_signal)
	  : "I" (tsk_resume_wait_signal_swi), "0"(_signal)
	  : "r1", "r2", "r3", "r12");
	return _signal;
}
#endif


#ifdef usr_drv_icontrol_swi
inline static   void usr_drv_icontrol(DRIVER_INDEX volatile drv_index, unsigned int volatile reason, void * volatile param)
{
	register unsigned int _drv_index asm("r0") = drv_index;
	register unsigned int _reason asm("r1") = reason;
	register void * _param asm("r2") = param;

	asm volatile ("swi %0"
	  :
	  : "I" (usr_drv_icontrol_swi), "r"(_drv_index), "r"(_reason), "r"(_param)
	  : "r3");
}
#define tsk_drv_icontrol(args...) usr_drv_icontrol(args)
#endif


#ifdef isr_contol_swi
typedef const struct DRIVER_INFO_Type * DRIVER_INFO;
inline static void isr_contol(DRIVER_INFO volatile drv_info, unsigned int volatile reason, void* volatile param)
{
	register DRIVER_INFO _drv_info asm("r0") = drv_info;
	register unsigned int _reason asm("r1") = reason;
	register void * _param asm("r2") = param;

	asm volatile ("swi %0"
	  :
	  : "I" (isr_contol_swi), "r"(_drv_info), "r"(_reason), "r"(_param)
	  : "r3");
}
#endif





#ifdef malloc_swi
inline static void* usr_malloc(unsigned int volatile size)
{
	register unsigned int _size asm("r0") = size;

	asm volatile ("swi %1"
	  : "=r"(_size)
	  : "I" (malloc_swi), "0"(_size)
	  : "r3");
	return (void*)_size;
}
#endif

#ifdef free_swi
inline static void usr_free(void* volatile ptr)
{
	register void* _ptr asm("r0") = ptr;

	asm volatile ("swi %1"
	  : "=r"(_ptr)
	  : "I" (free_swi), "0"(_ptr)
	  : "r3");
}
#endif

#ifdef realloc_swi
inline static void* usr_realloc(void* volatile ptr, unsigned int volatile size)
{
	register void* _ptr asm("r0") = ptr;
	register unsigned int _size asm("r1") = size;

	asm volatile ("swi %1"
	  : "=r"(_ptr)
	  : "I" (realloc_swi), "0"(_ptr), "r"(_size)
	  : "r3");
	return (void*)_ptr;
}
#endif

#ifdef __cplusplus
}
#endif

#endif



