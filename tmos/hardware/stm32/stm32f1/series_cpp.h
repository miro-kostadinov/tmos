/**************************************************************************//**
 * @ingroup	 hardware_stm32f1
 * @{
 * @file     hardware/stm32/stm32f1/series_cpp.h
 * @brief    STM32F1 series CPP header
 * @version  V3.00
 * @date     13. December 2011
 * @author	 Miroslav Kostadinov
 *
 *
 *
 ******************************************************************************/
#ifndef SERIES_CPP_H_
#define SERIES_CPP_H_

#include "series_inc.h"
#include <tmos_types.h>

#include "mcu_cpp.h"
#include "rcc_f1.h"
#include "flash_f1.h"
#include "dbgmcu_f1.h"
#include "gpio_f1.h"
#include "usart_f1.h"
#include "dma_f1.h"
#include "pwr_f1.h"
#include "rtc_f1.h"
#include "tim_f1.h"
#include "dac_f1.h"

/**
  * The RCC feeds the Cortex System Timer (SysTick) external clock with the AHB
  * clock (HCLK) divided by 8. The SysTick can work either with this clock or
  * with the Cortex clock(HCLK), configurable in the SysTick Control and Status
  * Register.
  */
//#ifndef GET_SYSTICK_CLOCK
//#define GET_SYSTICK_CLOCK (system_clock_frequency >> 3)
//#endif


#endif /* SERIES_CPP_H_ */
/** @} ingroup hardware_stm32f1 */
