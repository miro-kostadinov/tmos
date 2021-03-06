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
#include <sysctl_lm3s.h>

void wdt_feet(WDT_INFO drv_info)
{
	WDT_Type* wdt = drv_info->hw_base;
	unsigned int ctrl;
	unsigned int locked;

	__disable_irq();
	locked = wdt->WDTLOCK;
	//Unlock
	ctrl = wdt->WDTCTL;
	if(locked)
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
	if(locked)
	{
		wdt->WDTLOCK = WDT_WDTLOCK_LOCKED;
		while(ctrl != wdt->WDTCTL)
		{
		}
	}
	__enable_irq();

}
/**
 * WDT helper task
 * @param drv_info
 */
void wdt_thread(WDT_INFO drv_info)
{

	while(1)
	{

		wdt_feet(drv_info);

		//Sleep
		tsk_sleep(drv_info->feed_sleep);
	}

}
#define WDT_TASK_STACK_SIZE 20

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
		drv_enable_isr(&drv_info->info);

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
	FaultHandler();
}


