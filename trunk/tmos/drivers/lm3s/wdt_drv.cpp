/*******************************************************************************
 * @file     wdt_drv.cpp
 * @ingroup  DRV_LM3S_WDT
 * @brief    WDT driver implementation
 * @version  V3.00
 * @date     25. October 2011
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "wdt_drv.h"
#include <cmsis_cpp.h>
#include <sysctl_lm3s.h>


/**
 * WDT helper task
 * @param drv_info
 */
void wdt_thread(WDT_INFO drv_info)
{
	WDT_Type* wdt = drv_info->hw_base;
	unsigned int ctrl;

	while(1)
	{
//		__disable_irq();
//		SysCtlPeripheralReset(drv_info->info.peripheral_indx);
//		wdt->WDTLOAD = drv_info->wdt_load;
//		wdt->WDTTEST = WDT_WDTTEST_STALL;
//		wdt->WDTCTL = WDT_WDTCTL_INTEN | WDT_WDTCTL_RESEN;
//		__enable_irq();

		//Unlock
		ctrl = wdt->WDTCTL;
		wdt->WDTLOCK = WDT_WDTLOCK_UNLOCK;
		while(ctrl != wdt->WDTCTL)
		{
		}

		//Feed (due to errata do it twice)
		do
		{
			wdt->WDTLOAD = drv_info->wdt_load +1;
			while(ctrl != wdt->WDTCTL)
			{
			}
		} while (wdt->WDTLOAD != drv_info->wdt_load +1);
		do
		{
			wdt->WDTLOAD = drv_info->wdt_load ;
			while(ctrl != wdt->WDTCTL)
			{
			}
		} while (wdt->WDTLOAD != drv_info->wdt_load);

		//Lock
		wdt->WDTLOCK = WDT_WDTLOCK_LOCKED;
		while(ctrl != wdt->WDTCTL)
		{
		}


		//Sleep
		tsk_sleep(drv_info->feed_sleep);
	}

}
#define WDT_TASK_STACK_SIZE 5

/**
 * Watchdog DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void WDT_DCR(WDT_INFO drv_info, unsigned int reason, HANDLE hnd)
{
	Task* task;

	WDT_Type* wdt = drv_info->hw_base;
	unsigned int ctrl;


	if(reason == DCR_RESET)
	{
       	//Initialize the helper task
		task = usr_task_create_dynamic("WDTT", (TASK_FUNCTION) wdt_thread, 0,
			WDT_TASK_STACK_SIZE);
		if (task)
		{
			svc_task_schedule(task);
			task->sp->r0.as_cvoidptr = drv_info;
		}

		//Enable Watchdog 0 / 1
		SysCtlPeripheralReset(drv_info->info.peripheral_indx);


		//Unlock
		ctrl = wdt->WDTCTL;

		// Load
		do
		{
			wdt->WDTLOAD = drv_info->wdt_load;
			while(ctrl != wdt->WDTCTL)
			{
			}

		} while (wdt->WDTLOAD != drv_info->wdt_load); //errata

		//Enable stall
		wdt->WDTTEST = WDT_WDTTEST_STALL;
		while(ctrl != wdt->WDTCTL)
		{
		}

		//Enable the watchdog timer
		ctrl |= WDT_WDTCTL_INTEN | WDT_WDTCTL_RESEN;
		wdt->WDTCTL = ctrl;
		while(ctrl != wdt->WDTCTL)
		{
		}

		//Lock
		wdt->WDTLOCK = WDT_WDTLOCK_LOCKED;
		while(ctrl != wdt->WDTCTL)
		{
		}


		//Enable the ISR
		NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
		NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);

	}
}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void WDT_DSR(WDT_INFO drv_info, HANDLE hnd)
{
	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

/**
 * ISR
 * @param drv_info
 */
void WDT_ISR(WDT_INFO drv_info )
{
    volatile unsigned int i=1;

#if (TRACE_IS != TRACE_DISABLED) || defined(USE_EXCEPTION_RECORD)
    TRACELN1("\r\nWatchdog!!!");
#endif


#if USE_EXCEPTION_RECORD
    exception_record.CFSR = 0;
    exception_record.MMFAR = 0;
    exception_record.BFAR = 0;
    exception_record.cur_task = (unsigned int)CURRENT_TASK;
    if (((unsigned int) CURRENT_TASK > SRAM_BASE)
			&& ((unsigned int) CURRENT_TASK < (SRAM_BASE + RAM_SIZE)))
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


