/**************************************************************************//**
 * @ingroup	 hardware_stm32f4
 * @{
 * @file     hardware/stm32/stm32f4/series_cpp.h
 * @brief    STM32F2 series CPP header
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
#include "rcc_f4.h"
#include "flash_f4.h"
#include "dbgmcu_f4.h"
#include "gpio_f4.h"
#include "usart_f4.h"
#include "dma_f4.h"
#include "pwr_f4.h"
#include "rtc_f4.h"
#include "adc_f4.h"
#include "tim_f4.h"
#include "crc_f4.h"
#if USE_NET
#include "eth_f4.h"
#include "eth_phy_f4.h"
#endif
#include "dac_f4.h"

#endif /* SERIES_CPP_H_ */
/** @} ingroup hardware_stm32f4 */
