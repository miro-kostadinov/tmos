/**************************************************************************//**
 * @file     systick_drv.cpp
 * @ingroup  DRV_AT91_SYSTICK
 * @brief    SYSTICK driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <systick_drv.h>
#include <cmsis_cpp.h>
#include <pmc_drv.h>


void SYSTICK_DCR(SYSTICK_INFO drv_info, unsigned int reason, void* param)
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
		ticks = (sysdrv_clock_frequency/1000)*drv_info->OS_QUANTUM_PERIOD - 1;
		SysTick->SysTick_Config(ticks);
		NVIC->NVIC_SetPriority (SysTick_IRQn, 0);  /* SYSTICK MUST HAVE THE HIGHEST PRIORITY !!!! */

	}
}





