/**************************************************************************//**
 * @ingroup	 hardware_stm32
 * @{
 * @file     hardware/stm32/fam_cpp.h
 * @brief    STM32 Family CPP header
 * @version  V3.00
 * @date     13. December 2011
 * @author	 Miroslav Kostadinov
 *
 *
 *
 ******************************************************************************/

#ifndef FAM_CPP_H_
#define FAM_CPP_H_

#include <hardware_inc.h>

#include "series_cpp.h"
#include <gpio_stm32.h>
#include <exti_stm32.h>

unsigned int get_systick_source_clock();

/**
  * The RCC feeds the Cortex System Timer (SysTick) external clock with the AHB
  * clock (HCLK) divided by 8. The SysTick can work either with this clock or
  * with the Cortex clock(HCLK), configurable in the SysTick Control and Status
  * Register.
  */
#ifndef GET_SYSTICK_CLOCK
#define GET_SYSTICK_CLOCK get_systick_source_clock()
#endif


#endif /* FAM_CPP_H_ */
/** @} ingroup hardware_stm32 */
