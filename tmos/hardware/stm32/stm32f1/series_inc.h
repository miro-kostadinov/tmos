/**************************************************************************//**
 * @file     hardware/stm32/cortex-m3/stm32f1/series_inc.h
 * @ingroup	 hardware_stm32f1
 * @brief    STM32F1 series CPP header
 * @version  V3.00
 * @date     13. December 2011
 * @author	 Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f1 STM32F1 series
 * @ingroup	 hardware_stm32
 * Source and definitions for STM32F1 series mcu's
 *
 *
 ******************************************************************************/

#ifndef SERIES_INC_H_
#define SERIES_INC_H_

#include <mcu_inc.h>


/**
 * @ingroup hardware_sam3s
 * @addtogroup CMSIS_CM3_core_definitions CM3 Core Definitions
 * This file defines all structures and symbols for CMSIS core:
 *  - CMSIS version number
 *  - Cortex-M core registers and bitfields
 *  - Cortex-M core peripheral base address
 * @{
 */

#define __CM3_CMSIS_VERSION_MAIN  (0x01)  //!< [31:16] CMSIS HAL main version
#define __CM3_CMSIS_VERSION_SUB   (0x30)  //!< [15:0]  CMSIS HAL sub version
#define __CORTEX_M                (0x03)  //!< Cortex core

#ifdef STM32F10X_XL
 #define __MPU_PRESENT             1 //!< STM32 XL-density devices provide an MPU
#else
 #define __MPU_PRESENT             0 //!< Other STM32 devices does not provide an MPU
#endif /* STM32F10X_XL */
#define __NVIC_PRIO_BITS          4 //!< STM32 uses 4 Bits for the Priority Levels

/**	 @} end of addtogroup CMSIS_CM3_core_definitions	*/




#endif /* SERIES_INC_H_ */
