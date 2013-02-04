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
#include <hardware_cpp.h>

 /**
  * In some CPU families the SysTick clock is configurable. In this case the
  * target must provide this macro that returns the actual value.
  * By default SysTick clock is equal to the system clock:
  */
 #ifndef GET_SYSTICK_CLOCK
 #define GET_SYSTICK_CLOCK system_clock_frequency
 #endif


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
		ticks = ( GET_SYSTICK_CLOCK /1000 )*drv_info->OS_QUANTUM_PERIOD - 1;
		SysTick->SysTick_Config(ticks);
		NVIC->NVIC_SetPriority (SysTick_IRQn, 0);  /* SYSTICK MUST HAVE THE HIGHEST PRIORITY !!!! */
		NVIC->NVIC_SetPriority (PendSV_IRQn, -1);
		break;
	}
}





