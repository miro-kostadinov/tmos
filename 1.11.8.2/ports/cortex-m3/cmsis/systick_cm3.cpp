/**
 * @file: systick_cm3.cpp
 *
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 *
 * license: check README.TXT
 * @date 2010-3-12
 *
 */

#include <tmos.h>
#include "cmsis_cpp.h"

/* ##################################    SysTick function  ############################################ */

#if (!defined (__Vendor_SysTickConfig)) || (__Vendor_SysTickConfig == 0)

	/**
	 * @brief  Initialize and start the SysTick counter and its interrupt.
	 *
	 * @param   ticks   number of ticks between two interrupts
	 * @return  1 = failed, 0 = successful
	 *
	 * Initialise the system tick timer and its interrupt and start the
	 * system tick timer / counter in free running mode to generate
	 * periodical interrupts.
	 */
	uint32_t SysTick_Type::SysTick_Config(uint32_t ticks)
	{
	  if (ticks > SysTick_LOAD_RELOAD_Msk)  return (1);            /* Reload value impossible */

	  this->LOAD  = (ticks & SysTick_LOAD_RELOAD_Msk) - 1;      /* set reload register */
	  NVIC->NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Cortex-M0 System Interrupts */
	  this->VAL   = 0;                                          /* Load the SysTick Counter Value */
	  this->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
					   SysTick_CTRL_TICKINT_Msk   |
					   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
	  return (0);                                                  /* Function successful */
	}

#endif
