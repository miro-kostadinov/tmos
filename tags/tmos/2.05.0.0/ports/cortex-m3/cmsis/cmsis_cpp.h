/**************************************************************************//**
 * @ingroup	 cores_cmsis
 * @{
 * @file     cortex-m3/cmsis/cmsis_cpp.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version  V3.00
 * @date     07. April 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef CMSIS_CPP_H_
#define CMSIS_CPP_H_

#include "cmsis_inc.h"
#include <hardware_inc.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *                 Register Abstraction
 ******************************************************************************/
/**
 * @ingroup	 cores_cmsis
 * @defgroup CMSIS_CM3_core_register CMSIS CM3 Core Register
 @{
 * @defgroup CMSIS_ARM_CORE CMSIS ARM Core
 memory mapped structure for Nested Vectored Interrupt Controller (NVIC)
 @{
 */

/**
 * Application PSR
 */
union APSR_Type {
	struct {
#if (__CORTEX_M != 0x04)
		uint32_t _reserved0 :27;
#else
		uint32_t _reserved0:16;
		uint32_t GE:4; /*!< bit: 16..19  Greater than or Equal flags  */
		uint32_t _reserved1:7;
#endif
		uint32_t Q :1; /*!< bit:     27  Saturation condition flag    */
		uint32_t V :1; /*!< bit:     28  Overflow condition code flag */
		uint32_t C :1; /*!< bit:     29  Carry condition code flag    */
		uint32_t Z :1; /*!< bit:     30  Zero condition code flag     */
		uint32_t N :1; /*!< bit:     31  Negative condition code flag */
	} b; /**< APSR as bit field*/
	uint32_t w; /**< APSR as uint32*/
};

/**
 * Interrupt PSR
 */
union IPSR_Type {
	struct {
		uint32_t ISR :9; /*!< bit: 00..08  Exception number             */
		uint32_t _reserved0 :23;
	} b; /**< as bit field*/
	uint32_t w; /**< as uint32*/
};

/**
 * Cortex M3 PSR (Processor Status Register)
 */
union xPSR_Type {
	struct {
		uint32_t ISR :9; /*!< bit: 00..08  Exception number             */
#if (__CORTEX_M != 0x04)
		uint32_t _reserved0 :15;
#else
		uint32_t _reserved0:7;
		uint32_t GE:4; /*!< bit: 16..19  Greater than or Equal flags  */
		uint32_t _reserved1:4;
#endif
		uint32_t T :1; /*!< bit:     24  Thumb bit        (read 0)    */
		uint32_t IT :2;/*!< bit: 25..26  saved IT state   (read 0)    */
		uint32_t Q :1; /*!< bit:     27  Saturation condition flag    */
		uint32_t V :1; /*!< bit:     28  Overflow condition code flag */
		uint32_t C :1; /*!< bit:     29  Carry condition code flag    */
		uint32_t Z :1; /*!< bit:     30  Zero condition code flag     */
		uint32_t N :1; /*!< bit:     31  Negative condition code flag */
	} b; /**< as bit field*/
	uint32_t w; /**< as uint32*/
};

/**
 * CONTROL register
 */
union CONTROL_Type {
	struct {
		uint32_t nPRIV :1;/*!< bit:  00  Execution privilege in Thread mode */
		uint32_t SPSEL :1;/*!< bit:  01  Stack to be used                   */
		uint32_t FPCA :1; /*!< bit:  02  FP extension active flag           */
		uint32_t _reserved0 :29;
	} b; /**< as bit field*/
	uint32_t w; /**< as uint32*/
};

/**@} end of group CMSIS_ARM_CORE */

/* Memory mapping of Cortex-M3 Hardware */
#define SCS_BASE            (0xE000E000)      	/*!< System Control Space Base Address */
#define CoreDebug_BASE      (0xE000EDF0)        /*!< Core Debug Base Address           */
#define SysTick_BASE        (SCS_BASE +  0x0010)/*!< SysTick Base Address              */
#define SCB_BASE            (SCS_BASE +  0x0D00)/*!< System Control Block Base Address */

/**@} end of group CMSIS_CM3_core_register */
/*******************************************************************************
 *                Hardware Abstraction Layer
 ******************************************************************************/

#if defined ( __CC_ARM   )
#define __ASM            __asm     /*!< asm keyword for ARM Compiler          */
#define __INLINE         __inline  /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
#define __ASM           __asm      /*!< asm keyword for IAR Compiler          */
#define __INLINE        inline     /*!< inline keyword for IAR Compiler. 	  */

#elif defined   (  __GNUC__  )
#define __ASM            __asm     /*!< asm keyword for GNU Compiler          */
#define __INLINE         inline    /*!< inline keyword for GNU Compiler       */

#elif defined   (  __TASKING__  )
#define __ASM            __asm     /*!< asm keyword for TASKING Compiler      */
#define __INLINE         inline    /*!< inline keyword for TASKING Compiler   */

#endif

/* ##########################  Core Instruction Access  ######################### */
#include "core_cmInstr.h"

/* ###########################  Core Function Access  ########################### */
#include "core_cmFunc.h"

#ifdef __cplusplus
}
#endif

#include "nvic_cm3.h"
#include "scb_cm3.h"
#include "systick_cm3.h"
#include "itm_cm3.h"
#include "mpu_cm3.h"

#endif /* CMSIS_CPP_H_ */
/** @} ingroup cores_cmsis */
