/*
 * systick_drv.cpp
 *
 *  Created on: 2010-2-25
 *      Author: Miroslav Kostadinov
 */

#include <drivers.h>



void dcr_ClockDriver(ClockDriver* driver, unsigned int reason, void* param)
{
	switch(reason)
	{
	case DCR_RESET:
		/*
			hw_base->CTRL = NVIC_ST_CTRL_CLK_SRC;
			HWREG(NVIC_ST_CTRL) = NVIC_ST_CTRL_CLK_SRC;
		    HWREG(NVIC_ST_RELOAD) = (SysCtlClockGet()/1000)*OS_QUANTUM_PERIOD - 1;
		    HWREG(NVIC_ST_CURRENT) = 0;
		    HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE|NVIC_ST_CTRL_INTEN;
		    */
		unsigned int ticks;
		SysTick->CTRL = NVIC_ST_CTRL_CLK_SRC;
//		SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_SYSDIV_25);
		SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_OSC|SYSCTL_SYSDIV_2);
		ticks = (SYSCTL->SysCtlClockGet()/1000)*OS_QUANTUM_PERIOD - 1;
		SysTick->SysTick_Config(ticks);
		NVIC->NVIC_SetPriority (SysTick_IRQn, 0);  /* SYSTICK MUST HAVE THE HIGHEST PRIORITY !!!! */

	}
}

void dsr_ClockDriver(ClockDriver* driver, HANDLE hnd)
{


}

//unsigned int test;
//void isr_ClockDriver(ClockDriver* driver)
//{
//	test++;
//}



