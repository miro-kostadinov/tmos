/**************************************************************************//**
 * @ingroup	hardware_stm32f1_dbgmcu
 * @file	hardware/stm32/stm32f1/dbgmcu_f1.h
 * @brief  	STM32F1 DBGMCU
 * @date    29. October 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f1_dbgmcu  MCU debug component (DBGMCU)
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F1 MCU debug component (DBGMCU)
 * @{
 *
 ******************************************************************************/

#ifndef DBGMCU_F1_H_
#define DBGMCU_F1_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  MCU debug component (DBGMCU)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DBGMCU_IDCODE;			//!< (dbgmcu offset: 0x00) MCU device ID code
  __IO uint32_t DBGMCU_CR;				//!< (dbgmcu Offset: 0x04) Debug MCU configuration register
} DBGMCU_TypeDef;


/***************************************************************************//**
 * @relates DBGMCU_TypeDef
 * @{
 ******************************************************************************/

/** @name DBGMCU_IDCODE:	(dbgmcu offset: 0x00) MCU device ID code		  */
/** @{ */
#define DBGMCU_IDCODE_DEV_ID        0x00000FFF //!< Device Identifier
#define DBGMCU_IDCODE_REV_ID        0xFFFF0000 //!< REV_ID[15:0] bits (Revision Identifier)
/** @} */

/** @name DBGMCU_CR:	(dbgmcu Offset: 0x04) Debug MCU configuration register  */
/** @{ */
#define DBGMCU_CR_DBG_SLEEP         0x00000001 //!< Debug Sleep Mode
#define DBGMCU_CR_DBG_STOP          0x00000002 //!< Debug Stop Mode
#define DBGMCU_CR_DBG_STANDBY       0x00000004 //!< Debug Standby mode
#define DBGMCU_CR_TRACE_IOEN        0x00000020 //!< Trace Pin Assignment Control

#define DBGMCU_CR_TRACE_MODE        0x000000C0 //!< TRACE_MODE[1:0] bits (Trace Pin Assignment Control)
#define DBGMCU_CR_TRACE_MODE_ASYNC  0x00000000 //!< TRACE pin assignment for Asynchronous Mode
#define DBGMCU_CR_TRACE_MODE_SYNC1  0x00000040 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 1
#define DBGMCU_CR_TRACE_MODE_SYNC2  0x00000080 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 2
#define DBGMCU_CR_TRACE_MODE_SYNC4  0x000000C0 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 4

#define DBGMCU_CR_DBG_IWDG_STOP     0x00000100 //!< Debug Independent Watchdog stopped when Core is halted
#define DBGMCU_CR_DBG_WWDG_STOP     0x00000200 //!< Debug Window Watchdog stopped when Core is halted
#define DBGMCU_CR_DBG_TIM1_STOP     0x00000400 //!< TIM1 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM2_STOP     0x00000800 //!< TIM2 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM3_STOP     0x00001000 //!< TIM3 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM4_STOP     0x00002000 //!< TIM4 counter stopped when core is halted
#define DBGMCU_CR_DBG_CAN1_STOP     0x00004000 //!< Debug CAN1 stopped when Core is halted
#define DBGMCU_CR_DBG_I2C1_SMBUS_TIMEOUT   0x00008000 //!< SMBUS timeout mode stopped when Core is halted
#define DBGMCU_CR_DBG_I2C2_SMBUS_TIMEOUT   0x00010000 //!< SMBUS timeout mode stopped when Core is halted
#define DBGMCU_CR_DBG_TIM8_STOP     0x00020000 //!< TIM8 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM5_STOP     0x00040000 //!< TIM5 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM6_STOP     0x00080000 //!< TIM6 counter stopped when core is halted
#define DBGMCU_CR_DBG_TIM7_STOP     0x00100000 //!< TIM7 counter stopped when core is halted
#define DBGMCU_CR_DBG_CAN2_STOP     0x00200000 //!< Debug CAN2 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM15_STOP    0x00400000 //!< Debug TIM15 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM16_STOP    0x00800000 //!< Debug TIM16 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM17_STOP    0x01000000 //!< Debug TIM17 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM12_STOP    0x02000000 //!< Debug TIM12 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM13_STOP    0x04000000 //!< Debug TIM13 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM14_STOP    0x08000000 //!< Debug TIM14 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM9_STOP     0x10000000 //!< Debug TIM9 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM10_STOP    0x20000000 //!< Debug TIM10 stopped when Core is halted
#define DBGMCU_CR_DBG_TIM11_STOP    0x40000000 //!< Debug TIM11 stopped when Core is halted
/** @} */


/** @} */ // @relates DBGMCU_TypeDef




#endif /* DBGMCU_F1_H_ */

/** @} */
