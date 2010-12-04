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
#include <drivers.h>
#if USE_TIME
#include "time.h"
#endif

extern TASK_STRU main_task;
extern TASK_DESCRIPTION main_task_desc;
// main task declaration




//*----------------------------------------------------------------------------
//*			Helper functions
//*----------------------------------------------------------------------------

extern char end;

extern "C" void HardFaultIsr(void)
{
    unsigned status = SCB->CFSR;
    unsigned int i=1;
    TRACELN1("\r\nException ");
    if(status&0xFF)
    	TRACELN("MPU: %2.2x at %x ", status&0xFF, SCB->MMFAR );
    if(status&0xFF00)
    	TRACELN("BUS: %2.2x at %x ", (status&0xFF00)>>8, SCB->BFAR);
    if(status&0xFFFF0000)
    	TRACELN("USAGE: %4.4x",(status&0xFFFF0000)>>16);
    SCB->CFSR = status;
    while(i){}
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

extern "C" void sys_kernel_init( void)
{
    DRIVER_INFO drv_info;
    char *ptr;
    int i, j;


    //--------------- Start trace -----------------------------//
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    //TPIU
    *(volatile unsigned int*)(TPIU_BASE +0x304) = 0x00;	//Formatter and Flush Control Register
    *(volatile unsigned int*)(TPIU_BASE +0x0F0) = 0x01;	//Selected Pin Protocol Register
    *(volatile unsigned int*)(TPIU_BASE +0x304) = 0x00;	//Formatter and Flush Control Register
    *(volatile unsigned int*)(TPIU_BASE +0x004) = 0x08;	//Current Sync Port Size Register
    *(volatile unsigned int*)(TPIU_BASE +0x010) = 0x02;	//Async Clock Prescaler Register

    //ITM

    ITM->LAR = 0xC5ACCE55;						//Lock Access Register
    ITM->TCR |= 0x10009 | ITM_TCR_ITMENA_Msk;	//Trace Control Register = trace ID = 1, ITM and DTW enabled
    ITM->TER |= 0xFFFFFFFF ;					//Trace Enable = all stimulus ports
    ITM->TPR = 0x0;								//Trace Privilege = all

	TRACELN1("===== "__DATE__ " === " __TIME__ " =====    "); //few more spaces (a clock change follows)

	//------------- initialize dynamic memory  ---------------//
	svc_pool_init(&end, (void*)(SRAM_BASE + RAM_SIZE));


//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
//	UDMA->uDMAEnable();
//	UDMA->uDMAControlBaseSet(UDMATable);

    // initialize main task
    usr_task_init_static(&main_task_desc, false);
    //extra initialization needed for main_task
    main_task.time = 0;								// current time is 000000000
    main_task.state = TSKSTATE_READY;				// leave it suspend ???
#if USE_TIME
    usr_task_init_static(&time_task_desc, true);
#endif
    // call sysclock and gpio drivers DCRs with DCR_RESET first
    for(i=0; i < (signed)sizeof(DRV_RESET_FIRST_TABLE); i++)
    {
    	ptr =DRV_TABLE[DRV_RESET_FIRST_TABLE[i]];
    	drv_info = (DRIVER_INFO)(void*)(ptr-1);
        drv_info->dcr(drv_info, DCR_RESET, NULL);
    }
    // call all drivers DCRs with DCR_RESET
    i=SysTick_IRQn; j=0;
    while( (ptr =DRV_TABLE[i++]) )
    {
    	if( i ==  DRV_RESET_FIRST_TABLE[j]+1)
    	{
    		j++;
    		continue;
    	}
    	drv_info = (DRIVER_INFO)(void*)(ptr-1);
        drv_info->dcr(drv_info, DCR_RESET, NULL);

    }


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
void usr_task_init_static(TASK_DESCRIPTION const *  desc, int bStart)
{
	Task *task;
	TASK_STACKED_CTX ctx;

	ctx = (TASK_STACKED_CTX)desc->stack;
	ctx--;
	ctx->psr.as_int = 0x01000000;	//thumb mode
	ctx->pc.as_voidptr = (void*)desc->func;
	ctx->lr.as_voidptr = (void*)sys_task_return;

	task = desc->tsk;
	task->sp = ctx;

	task->priority = desc->priority;
	task->time = main_task.time;
	task->prev = task->next = task->tprev = task->tnext = task;
	task->signals = 0;
	task->aloc_sig = 0;
    task->state = TSKSTATE_SUSPEND;
    for(int i=0; ; i++)
    {
        if(! (task->name[i] = desc->name[i]))
        	break;
    }


    if(bStart)
    {
        if(__get_CONTROL() & 2)

        	usr_task_schedule(task);

        else
        	svc_task_schedule(task);

    }



}



