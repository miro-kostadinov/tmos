/**************************************************************************//**
 * @ingroup	 hardware
 * @defgroup hardware_lm3s LM3S
 * @{
 * @file
 * @brief    LM3S FAMILY inc header
 * @version  V3.00
 * @date     11. November 2010
 * @author	 Miroslav Kostadinov
 *
 * hardware/lm3s/fam_inc.h
 *
 ******************************************************************************/

#ifndef FAMILY_INC_H_
#define FAMILY_INC_H_

#include <mcu_inc.h>
#include "memmap_inc.h"
#include "sysctl_inc.h"
#include "gpio_inc.h"
#include "ssi_inc.h"
#include "uart_inc.h"
#include "timer_inc.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 LM3S  CMSIS configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/** @addtogroup CMSIS_CM3_core_definitions CM3 Core Definitions
  This file defines all structures and symbols for CMSIS core:
    - CMSIS version number
    - Cortex-M core registers and bitfields
    - Cortex-M core peripheral base address
	@{
 */
#define __CM3_CMSIS_VERSION_MAIN  (0x01)                                                       /*!< [31:16] CMSIS HAL main version */
#define __CM3_CMSIS_VERSION_SUB   (0x40)                                                       /*!< [15:0]  CMSIS HAL sub version  */
#define __CM3_CMSIS_VERSION       ((__CM3_CMSIS_VERSION_MAIN << 16) | __CM3_CMSIS_VERSION_SUB) /*!< CMSIS HAL version number       */

#define __CORTEX_M                (0x03)                                                       /*!< Cortex core                    */

#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    4               /*!< standard definition for NVIC Priority Bits */
#endif

/**	 @} end of addtogroup CMSIS_CM3_core_definitions	*/

#endif /* FAMILY_INC_H_ */
/** @} defgroup hardware_lm3s */
