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
// Date:          2010-01-20
// Description:   Kernel
//
//========================================================================


#include <tmos.h>

extern TASK_STRU main_task;
extern TASK_DESCRIPTION main_task_desc;
// main task declaration

/**
 * system_clock_frequency is gloabal variable and shows the current clock
 * frequency
 */
volatile __no_init unsigned int system_clock_frequency;

#if USE_EXCEPTION_RECORD
volatile __no_init EXCEPTION_RECORD_STRU exception_record;

extern "C" unsigned int exception_crc(const unsigned int* record)
{
	unsigned int crc=0;
    for(int i = sizeof(EXCEPTION_RECORD_STRU); i>4; i-=4)
        crc += *record++;

    return crc;
}
#endif

//*----------------------------------------------------------------------------
//*			Helper functions
//*----------------------------------------------------------------------------

#if USE_TMOS_STDLIB
extern char end;
#endif

static void process_exception()
{
#if USE_EXCEPTION_RECORD
	TASK_STACKED_CTX_STRU* msp;
#endif



#if USE_EXCEPTION_RECORD
    exception_record.restart_cause = __get_IPSR();
    exception_record.CFSR = 0;
	msp = (TASK_STACKED_CTX_STRU*)__get_PSP();
	if ((uint32_t)msp < BASE_SRAM || (uint32_t)msp >= (BASE_SRAM + RAM_SIZE) || ((uint32_t)msp & 3))
	{
		exception_record.MMFAR = (uint32_t)msp;
		exception_record.BFAR = 0;
	} else
	{
		exception_record.MMFAR = msp->pc.as_int;
		exception_record.BFAR = msp->lr.as_int;
	}
    exception_record.cur_task = (unsigned int)CURRENT_TASK;
    if (((unsigned int) CURRENT_TASK > BASE_SRAM)
			&& ((unsigned int) CURRENT_TASK < (BASE_SRAM + RAM_SIZE)))
    {
    	exception_record.task_name = CURRENT_TASK->name[0]
				+ (CURRENT_TASK->name[1] << 8) + (CURRENT_TASK->name[2] << 16)
				+ (CURRENT_TASK->name[3] << 24);
	} else
    {
    	exception_record.task_name = 0;
    }
    exception_record.record_crc = exception_crc((const unsigned int*)&exception_record);
#endif

}

extern "C" void FaultHandler( void )
{
    volatile unsigned int i=1;

    process_exception();

    if(restart_on_exception)
    {
    	// stop systick
    	SYST->SYST_CSR = 0;
		LowLevelReboot();
    }

    while(i){}
}

/**
 *  DRV_TABLE - extern linker symbol
 */
extern char* const DRV_TABLE[];

WEAK_C void app_init(void)
{

}

//*----------------------------------------------------------------------------
//*     sys_kernel_init
//*
//*	Performs low level initialization.
//*        this function is called before any hi-level C initialization
//*
//*	NOTE: JTAG Debugging may fail in this function if you step while CPU clock
//* 		is lower than JTAG clock
//*----------------------------------------------------------------------------

extern "C" void sys_call_static_ctors(void);

extern "C" void sys_kernel_init( void)
{
    DRIVER_INFO drv_info;
    char *ptr;
    int i;

#if TRACE_IS > TRACE_DISABLED
/*
    //--------------- Start trace -----------------------------//
    CDBG->CDBG_DEMCR |= CDBG_DEMCR_TRCENA;

    //TPIU
    TPIU->TPIU_FFCR = 0x00;							//Formatter and Flush Control Register
    TPIU->TPIU_SPPR = TPIU_SPPR_ASYNC_MANCHESTER;	//Selected Pin Protocol Register
    TPIU->TPIU_FFCR = 0x00;							//Formatter and Flush Control Register
    TPIU->TPIU_CSPSR = TPIU_CSPSR_WORD;				//Current Sync Port Size Register
    TPIU->TPIU_ACPR = 0x04;							//Async Clock Prescaler Register

    //ITM

    //Lock Access Register
    ITM->ITM_LAR = ITM_LAR_KEY;
    //Trace Control Register = trace ID = 1, ITM and DTW enabled
    ITM->ITM_TCR = 0x10000 | ITM_TCR_DWTENA | ITM_TCR_ITMENA;
    ITM->ITM_TER = 0xFFFFFFFF ;						//Trace Enable = all stimulus ports
    ITM->ITM_TPR = 0x0;								//Trace Privilege = all
*/

	TRACELN1("===== " __DATE__ " === " __TIME__ " =====    "); //few more spaces (a clock change follows)
#endif

	//------------- initialize dynamic memory  ---------------//
#if USE_TMOS_STDLIB
	svc_pool_init(&end, (void*)(BASE_SRAM + RAM_SIZE));
#endif

    // initialize main task
    usr_task_init_static(&main_task_desc, false);
    PMAIN_TASK->tlist = PMAIN_TASK;

    //extra initialization needed for main_task
    PMAIN_TASK->time = 0;								// current time is 000000000
    PMAIN_TASK->state = TSKSTATE_READY;				// leave it suspend ???
#if USE_MEMORY_TRACKING
    PMAIN_TASK->aloc_mem = 0;
    PMAIN_TASK->aloc_ptrs =0;
#endif

    // Reset the drivers i
    for (i = SysTick_IRQn; i < INALID_DRV_INDX; i++)
    {
		ptr = DRV_TABLE[i];
		drv_info = (DRIVER_INFO) (void*) (ptr - 1);
		drv_info->dcr(drv_info, DCR_RESET, NULL);
    }

    // Application level init
    app_init();
#if USE_STATIC_CONSTRUCTORS
    sys_call_static_ctors();
#endif
}

//*----------------------------------------------------------------------------
//*     __cxa_pure_virtual()
//*
//*	Required from the linker. It is placed in vtable for classes with pure virtual
//*
//*----------------------------------------------------------------------------
extern "C" void __cxa_pure_virtual()
{

	TRACE_ERROR("\r\n\e[31mERROR: pure virtual function is called!\e[m");
}

/*-----------------------------------------------------------
 * TASK API
 *----------------------------------------------------------*/


void sys_task_return(void)
{
	while(1)
		usr_task_suspend(CURRENT_TASK);
}

/**
 * Task init
 * @param desc
 * @param bStart
 */
void usr_task_init_static(TASK_DESCRIPTION const * desc, int bStart)
{
	Task *task;
	TASK_STACKED_CTX ctx;

	ctx = (TASK_STACKED_CTX) desc->stack;
	ctx--;
	ctx->psr.as_int = 0x01000000; //thumb mode
	ctx->pc.as_voidptr = (void*) desc->func;
	ctx->lr.as_voidptr = (void*) sys_task_return;

	task = desc->tsk;
	task->sp = ctx;

	task->priority = desc->priority;
	task->time = main_task.time;
	task->prev = task->next = task->tprev = task->tnext = task;
	task->signals = 0;
	task->aloc_sig = 0;
	task->state = TSKSTATE_SUSPEND;
	for (int i = 0;; i++)
	{
		if (!(task->name[i] = desc->name[i]))
			break;
	}

	if (bStart)
	{
		if (__get_CONTROL() & 2)

			usr_task_schedule(task);

		else
			svc_task_schedule(task);
	}
}

#ifdef malloc_swi
Task* usr_task_create_dynamic(const char* name, TASK_FUNCTION func,
		unsigned char priority, unsigned int stack_words)
{
	Task* task;

	//Allocate task control block and stack
	stack_words = (stack_words * 4) + sizeof(TASK_STRU);
	if (__get_CONTROL() & 2)
	{
		task = (Task*)((unsigned int)usr_malloc(stack_words+4) +4);
	} else
	{
		task = (Task*)((unsigned int)svc_malloc(stack_words+4)+4);
	}

	if(task != (void *)4)
	{
		TASK_STACKED_CTX ctx;
		ctx = (TASK_STACKED_CTX) (((unsigned int)task) + stack_words);
		ctx--;
		ctx->psr.as_int = 0x01000000; //thumb mode
		ctx->pc.as_voidptr = (void*) func;
		ctx->lr.as_voidptr = (void*) sys_task_return;

		task->sp = ctx;

		task->priority = priority;
		task->time = main_task.time;
		task->prev = task->next = task->tprev = task->tnext = task;
		task->signals = 0;
		task->aloc_sig = 0;
		task->state = TSKSTATE_SUSPEND;
		strcpy(task->name, name);
	}
	else
		task = NULL;

	return (task);
}
#endif /*malloc_swi*/

/**
 * Allocates one signal (from 8 to 16)
 * @return signal
 */
unsigned int tsk_signal_allocate(void)
{
	unsigned short signal;

    signal = CURRENT_TASK->aloc_sig;
    signal = (signal+256) & ~signal;
	CURRENT_TASK->aloc_sig |= signal;

	return (signal);
}

/** release signal
 *
 * @param signal
 */
void tsk_signal_release(unsigned int signal)
{
	CURRENT_TASK->aloc_sig &= ~signal;
}


