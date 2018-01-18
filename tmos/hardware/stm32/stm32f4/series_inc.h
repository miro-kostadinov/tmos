/**************************************************************************//**
 * @file     hardware/stm32/stm32f4/series_inc.h
 * @ingroup	 hardware_stm32f4
 * @brief    STM32F4 series CPP header
 * @version  V3.00
 * @date     12. April 2017
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f4 STM32F4 series
 * @ingroup	 hardware_stm32
 * Source and definitions for STM32F4 series mcu's
 *
 *
 ******************************************************************************/

#ifndef HARDWARE_STM32_STM32F4_SERIES_INC_H_
#define HARDWARE_STM32_STM32F4_SERIES_INC_H_

#include <mcu_inc.h>


#define stm32f2  0x535432f4
#define CFG_SERIES stm32f4

/**
 * @ingroup hardware_sam3s
 * @addtogroup CMSIS_CM3_core_definitions CM3 Core Definitions
 * This file defines all structures and symbols for CMSIS core:
 *  - CMSIS version number
 *  - Cortex-M core registers and bitfields
 *  - Cortex-M core peripheral base address
 * @{
 */

#define __CORTEX_M                (0x04)  //!< Cortex core

#define __MPU_PRESENT             1 		//!< STM32F2XX provide an MPU */
#define __NVIC_PRIO_BITS          4 //!< STM32F2XX uses 4 Bits for the Priority Levels
#define NVIC_PIORITY(priority) (((priority) << (8 - __NVIC_PRIO_BITS)) &0xFF)

/**	 @} end of addtogroup CMSIS_CM3_core_definitions	*/




#endif /* HARDWARE_STM32_STM32F4_SERIES_INC_H_ */
