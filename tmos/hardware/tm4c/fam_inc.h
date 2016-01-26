/**************************************************************************//**
 * @ingroup	 hardware
 * @defgroup hardware_tm4c TM4C
 * @{
 * @file
 * @brief    TM4C FAMILY inc header
 * @version  V3.00
 * @date     22. August 2014
 * @author	 Miroslav Kostadinov
 *
 * hardware/lm3s/fam_inc.h
 *
 ******************************************************************************/

#ifndef FAMILY_INC_H_
#define FAMILY_INC_H_

#include <mcu_inc.h>
//#include "sysctl_inc.h"
//#include "ssi_inc.h"
//#include "uart_inc.h"
//#include "timer_inc.h"

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 TM4C  CMSIS configuration
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/** @addtogroup CMSIS_CM4_core_definitions CM4 Core Definitions
  This file defines all structures and symbols for CMSIS core:
    - CMSIS version number
    - Cortex-M core registers and bitfields
    - Cortex-M core peripheral base address
	@{
 */

#define __CORTEX_M                (0x03)                                                       /*!< Cortex core                    */

#ifndef __NVIC_PRIO_BITS
  #define __NVIC_PRIO_BITS    3               /*!< standard definition for NVIC Priority Bits */
#endif
#define NVIC_PIORITY(priority) (((priority) << (8 - __NVIC_PRIO_BITS)) &0xFF)

/**	 @} end of addtogroup CMSIS_CM4_core_definitions	*/

#endif /* FAMILY_INC_H_ */
/** @} defgroup hardware_tm4c */
