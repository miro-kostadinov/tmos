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
		SYST->SYST_CSR = SYST_CSR_CLKSOURCE;
		ticks = ( GET_SYSTICK_CLOCK /1000 )*drv_info->OS_QUANTUM_PERIOD - 1;

		// set reload register
		SYST->SYST_RVR  = ticks - 1;
		// Load the SysTick Counter Value
		SYST->SYST_CVR   = 0;
		// Enable SysTick IRQ and SysTick Timer
		SYST->SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;

		// SYSTICK MUST HAVE THE HIGHEST PRIORITY (0) !!!!
		NVIC_SetPriority(SysTick_IRQn, 0);
		NVIC_SetPriority(PendSV_IRQn, 255);
		break;
	}
}





