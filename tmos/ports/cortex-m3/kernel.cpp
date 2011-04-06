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
#include <tmos.h>
#include <cmsis_cpp.h>

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
#endif

//*----------------------------------------------------------------------------
//*			Helper functions
//*----------------------------------------------------------------------------

extern char end;

extern "C" void HardFaultIsr(void)
{
    volatile unsigned int i=1;
    unsigned status = SCB->CFSR;

#if (TRACE_IS != TRACE_DISABLED) || defined(USE_EXCEPTION_RECORD)
    unsigned int mem_adr = SCB->MMFAR;
    unsigned int bus_adr = SCB->BFAR;

    TRACELN1("\r\nException ");
    if(status&0xFF)
    	TRACELN("MPU: %2.2x at %x ", status&0xFF,  mem_adr);
    if(status&0xFF00)
    	TRACELN("BUS: %2.2x at %x ", (status&0xFF00)>>8, bus_adr);
    if(status&0xFFFF0000)
    	TRACELN("USAGE: %4.4x",(status&0xFFFF0000)>>16);
#endif

    SCB->CFSR = status;

#if USE_EXCEPTION_RECORD
    exception_record.CFSR = status;
    exception_record.MMFAR = mem_adr;
    exception_record.BFAR = bus_adr;
    exception_record.cur_task = (unsigned int)CURRENT_TASK;
    if( ((unsigned int)CURRENT_TASK > SRAM_BASE) && ((char*)CURRENT_TASK < &end) )
    {
    	exception_record.task_name = CURRENT_TASK->name[0]
				+ (CURRENT_TASK->name[1] << 8) + (CURRENT_TASK->name[2] << 16)
				+ (CURRENT_TASK->name[3] << 24);
	} else
    {
    	exception_record.task_name = 0;
    }
#endif

    if(restart_on_exception)
    {
    	// stop systick
		SysTick->CTRL = 0;
		LowLevelReboot();
    }


    while(i){}
}

/**
 *  First reset Table with drivers that must be initialized first
 *
 *  The table can be declared in the application, that's why we use it here as
 *  extern linker symbol. The table must be terminated with INALID_DRV_INDX
 */
extern signed char const DRV_RESET_FIRST_TABLE[];

/**
 *  DRV_TABLE - extern linker symbol
 */
extern char* const DRV_TABLE[];

static bool is_first_reset(int index)
{
	for(int i=0; DRV_RESET_FIRST_TABLE[i] < INALID_DRV_INDX; i++ )
	{
		if(index == DRV_RESET_FIRST_TABLE[i])
			return true;
	}

	return false;
}

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
    //--------------- Start trace -----------------------------//
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    //TPIU
    *(volatile unsigned int*)(TPIU_BASE +0x304) = 0x00;	//Formatter and Flush Control Register
    *(volatile unsigned int*)(TPIU_BASE +0x0F0) = 0x01;	//Selected Pin Protocol Register
    *(volatile unsigned int*)(TPIU_BASE +0x304) = 0x00;	//Formatter and Flush Control Register
    *(volatile unsigned int*)(TPIU_BASE +0x004) = 0x08;	//Current Sync Port Size Register
    *(volatile unsigned int*)(TPIU_BASE +0x010) = 0x04;	//Async Clock Prescaler Register

    //ITM

    ITM->LAR = 0xC5ACCE55;						//Lock Access Register
    ITM->TCR |= 0x10009 | ITM_TCR_ITMENA_Msk;	//Trace Control Register = trace ID = 1, ITM and DTW enabled
    ITM->TER |= 0xFFFFFFFF ;					//Trace Enable = all stimulus ports
    ITM->TPR = 0x0;								//Trace Privilege = all

	TRACELN1("===== "__DATE__ " === " __TIME__ " =====    "); //few more spaces (a clock change follows)
#endif

	//------------- initialize dynamic memory  ---------------//
#if USE_TMOS_STDLIB
	svc_pool_init(&end, (void*)(SRAM_BASE + RAM_SIZE));
#endif

    // initialize main task
    usr_task_init_static(&main_task_desc, false);
    //extra initialization needed for main_task
    main_task.time = 0;								// current time is 000000000
    main_task.state = TSKSTATE_READY;				// leave it suspend ???

    // Reset the drivers in DRV_RESET_FIRST_TABLE
    for (i = 0; (ptr =DRV_TABLE[DRV_RESET_FIRST_TABLE[i]]) ; i++)
    {
    	drv_info = (DRIVER_INFO)(void*)(ptr-1);
        drv_info->dcr(drv_info, DCR_RESET, NULL);
    }

    // Reset the remaining drivers
    for (i = SysTick_IRQn; i < INALID_DRV_INDX; i++)
    {
    	if(!is_first_reset(i))
    	{
    		ptr =DRV_TABLE[i];
        	drv_info = (DRIVER_INFO)(void*)(ptr-1);
            drv_info->dcr(drv_info, DCR_RESET, NULL);
    	}
    }

    // Application level init
    app_init();

    sys_call_static_ctors();
}

//*----------------------------------------------------------------------------
//*     __cxa_pure_virtual()
//*
//*	Required from the linker. It is placed in vtable for classes with pure virtual
//*
//*----------------------------------------------------------------------------
extern "C" void __cxa_pure_virtual()
{
	TRACE_ERROR("\r\nERROR: pure virtual function is called!");
}

/* Dummy function to avoid linker complaints */
extern "C" void __aeabi_unwind_cpp_pr0(void)
{
};

/*-----------------------------------------------------------
 * TASK API
 *----------------------------------------------------------*/


void sys_task_return(void)
{
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

	return signal;
}

/** release signal
 *
 * @param signal
 */
void tsk_signal_release(unsigned int signal)
{
	CURRENT_TASK->aloc_sig &= ~signal;
}


