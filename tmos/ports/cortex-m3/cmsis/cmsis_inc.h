/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @defgroup cores_cmsis CMSIS
 * @{
 * @file     cortex-m3/cmsis/cmsis_inc.h
 * @brief    Cortex M3 CMSIS inc header
 * @version  V3.00
 * @date     07. April 2010
 * @author	 Miroslav Kostadinov
 *
 * @note
 * Copyright (C) 2009 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 ******************************************************************************/
#ifndef CMSIS_INC_H_
#define CMSIS_INC_H_

#include "nvic_inc.h"

/* Memory mapping of Cortex-M3 Private peripheral Hardware */
#define SCS_BASE        (0xE000E000)          /*!< System Control Space Base Address */
#define ITM_BASE        (0xE0000000)          /*!< ITM Base Address                  */
#define TPIU_BASE       (0xE0040000)		  /*!< TPIU Base Address				 */
#define CoreDebug_BASE  (0xE000EDF0)          /*!< Core Debug Base Address           */
#define SysTick_BASE    (SCS_BASE +  0x0010)  /*!< SysTick Base Address              */
#define NVIC_BASE       (SCS_BASE +  0x0100)  /*!< NVIC Base Address                 */
#define SCB_BASE        (SCS_BASE +  0x0D00)  /*!< System Control Block Base Address */
#define MPU_BASE      	(SCS_BASE +  0x0D90)  /*!< Memory Protection Unit            */

#endif /* CMSIS_INC_H_ */
/** @} defgroup cores_cmsis */
