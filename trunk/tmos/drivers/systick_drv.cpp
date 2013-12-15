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

		// set reload register
		SysTick->LOAD  = ticks - 1;
		// Load the SysTick Counter Value
		SysTick->VAL   = 0;
		// Enable SysTick IRQ and SysTick Timer
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |
						SysTick_CTRL_ENABLE_Msk;

		// SYSTICK MUST HAVE THE HIGHEST PRIORITY (0) !!!!
	    SCB->SHP[((uint32_t)(SysTick_IRQn) & 0xF)-4] = 0;
	    SCB->SHP[((uint32_t)(PendSV_IRQn) & 0xF)-4] = -1;
		break;
	}
}





