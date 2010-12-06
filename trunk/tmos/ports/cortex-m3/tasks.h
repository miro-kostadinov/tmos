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
// Date:          2008-05-01
// Description:   Common types for TMOS
//
//========================================================================
#ifndef	OSTYPES_H
#define	OSTYPES_H

#ifndef TMOS_H
#error "use tmos.h"
#endif


#ifdef __cplusplus
extern "C"
{
#endif

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 Task related declarations
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define CURRENT_TIME (PMAIN_TASK->time)


typedef union
{
	unsigned int 	as_int;			/**< as int		*/
	void *			as_voidptr;		/**< as void*	*/
	const void *	as_cvoidptr;	/**< as const void* 	*/
	unsigned int *	as_intptr;		/**< as uint*			*/
	unsigned char * as_string;		/**< as uchar *			*/
	unsigned char * as_byteptr;		/**< as uchar *			*/
	char * 			as_charptr;		/**< as char *			*/
} word_reg;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 TASK
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef void     (*TASK_FUNCTION)(void);

typedef struct
{
	word_reg 		r0;		/**< r0			*/
	word_reg 		r1;		/**< r1			*/
	word_reg 		r2;		/**< r2			*/
	word_reg 		r3;		/**< r3			*/
	word_reg 		r12;	/**< r12		*/
	word_reg 		lr;		/**< LR			*/
	word_reg 		pc;		/**< PC			*/
	word_reg 		psr;	/**< PSR		*/
} TASK_STACKED_CTX_STRU, *TASK_STACKED_CTX;

typedef struct TASK_STRU_t Task;
typedef struct TASK_STRU_t
{
    //Task Control Block
	TASK_STACKED_CTX	sp;	/**< task stack		*/
	word_reg 		r4;		/**< r4			*/
	word_reg 		r5;		/**< r5			*/
	word_reg 		r6;		/**< r6			*/
	word_reg 		r7;		/**< r7			*/
	word_reg 		r8;		/**< r8			*/
	word_reg 		r9;		/**< r9			*/

    unsigned short	aloc_sig;	/**< allocated signals		*/
    unsigned char	state;		/**< task state			*/
    unsigned char	priority;	/**< task priority			*/
    unsigned int	signals;	/**< task signals			*/
    volatile Task	*next;		/**< pointer to the next task	*/
    volatile Task	*prev;		/**< pointer to the prev task	*/
    volatile unsigned int	time;	/**< task time	*/
    Task			*tnext;		/**< next in time list	*/
    Task			*tprev;		/**< prev in time list	*/
    Task			*tlist;		/**< all tasks list(todo)*/
	char			name[8];	/**< task name	*/
	RES_CODE		res;		/**< task status doc!	*/
} TASK_STRU;

typedef struct
{
	Task 			*tsk;	/**< pointer to the task	*/
    unsigned 		*stack;	/**< pointer to the task stack	*/
    TASK_FUNCTION	func;	/**< thread function	*/
    unsigned char	priority;	/**< task priority	*/
	const char		name[8];	/**< task name	*/
}TASK_DESCRIPTION;

// macro to declare a task statically
#ifdef __cplusplus

#define TASK_DECLARE_STATIC(task_name, aname, func, priority, stacksz) 	\
  	 TASK_STRU task_name __attribute__ ((section (".task."#task_name".tcb")));					\
     unsigned task_name##_stack[stacksz+8] __attribute__ ((section (".task."#task_name".tstack"))); \
     extern "C" const TASK_DESCRIPTION task_name##_desc __attribute__ ((section (".rominitsec"))) = 							\
	 {&task_name, &task_name##_stack[stacksz-1], func, priority, aname};
#else
#define TASK_DECLARE_STATIC(task_name, aname, func, priority, stacksz) 	\
  	 TASK_STRU task_name __attribute__ ((section (".task."#task_name".tcb")));					\
     unsigned task_name##_stack[stacksz+8] __attribute__ ((section (".task."#task_name".tstack"))); \
   	 const TASK_DESCRIPTION task_name##_desc __attribute__ ((section (".rominitsec"))) = 							\
   	 {&task_name, &task_name##_stack[stacksz-1], func, priority, aname};
#endif



register TASK_STRU *PMAIN_TASK asm("r10");
register TASK_STRU *CURRENT_TASK asm("r11");

#ifdef __cplusplus
}
#endif

#endif


