/**************************************************************************//**
 * @file     systick_drv.cpp
 * @ingroup  DRV_SYSTICK
 * @brief    SYSTICK driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <systick_drv.h>
#include <cmsis_cpp.h>

/** SYSTICK DCR
 *
 * @param drv_info
 * @param reason
 * @param param
 */
void SYSTICK_DCR(SYSTICK_INFO drv_info, unsigned int reason, void* param)
{
	switch(reason)
	{
	case DCR_RESET:
		unsigned int ticks;
		SysTick->CTRL = NVIC_ST_CTRL_CLK_SRC;
		ticks = (system_clock_frequency/1000)*drv_info->OS_QUANTUM_PERIOD - 1;
		SysTick->SysTick_Config(ticks);
		NVIC->NVIC_SetPriority (SysTick_IRQn, 0);  /* SYSTICK MUST HAVE THE HIGHEST PRIORITY !!!! */
		NVIC->NVIC_SetPriority (PendSV_IRQn, -1);
		break;
	}
}





