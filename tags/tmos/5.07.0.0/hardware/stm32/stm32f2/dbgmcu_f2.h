/*
 * dbgmcu_f2.h
 *
 *  Created on: Oct 29, 2012
 *      Author: miro
 */

#ifndef DBGMCU_F2_H_
#define DBGMCU_F2_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  MCU debug component (DBGMCU)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DBGMCU_IDCODE;			//!< (dbgmcu offset: 0x00) MCU device ID code
  __IO uint32_t DBGMCU_CR;				//!< (dbgmcu Offset: 0x04) Debug MCU configuration register
  __IO uint32_t DBGMCU_APB1FZ;  		//!< (dbgmcu Offset: 0x08) Debug MCU APB1 freeze register
  __IO uint32_t DBGMCU_APB2FZ;  		//!< (dbgmcu Offset: 0x0C) Debug MCU APB2 freeze register

} DBGMCU_TypeDef;


/*******************************************************************************
 * @defgroup DBGMCU_regs_define
 * @{
 ******************************************************************************/

/** @defgroup DBGMCU_IDCODE:	(dbgmcu offset: 0x00) MCU device ID code		  */
#define DBGMCU_IDCODE_DEV_ID        0x00000FFF //!< Device Identifier
#define DBGMCU_IDCODE_REV_ID        0xFFFF0000 //!< REV_ID[15:0] bits (Revision Identifier)
/** @} */

/** @defgroup DBGMCU_CR:	(dbgmcu Offset: 0x04) Debug MCU configuration register		  */
#define DBGMCU_CR_DBG_SLEEP         0x00000001 //!< Debug Sleep Mode
#define DBGMCU_CR_DBG_STOP          0x00000002 //!< Debug Stop Mode
#define DBGMCU_CR_DBG_STANDBY       0x00000004 //!< Debug Standby mode
#define DBGMCU_CR_TRACE_IOEN        0x00000020 //!< Trace Pin Assignment Control

#define DBGMCU_CR_TRACE_MODE        0x000000C0 //!< TRACE_MODE[1:0] bits (Trace Pin Assignment Control)
#define DBGMCU_CR_TRACE_MODE_ASYNC  0x00000000 //!< TRACE pin assignment for Asynchronous Mode
#define DBGMCU_CR_TRACE_MODE_SYNC1  0x00000040 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 1
#define DBGMCU_CR_TRACE_MODE_SYNC2  0x00000080 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 2
#define DBGMCU_CR_TRACE_MODE_SYNC4  0x000000C0 //!< TRACE pin assignment for Synchronous Mode with a TRACEDATA size of 4
/** @} */

/** @defgroup DBGMCU_APB1FZ: (dbgmcu Offset: 0x08) Debug MCU APB1 freeze register */
#define DBGMCU_APB1_FZ_DBG_TIM2_STOP        0x00000001 //!< TIM2 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM3_STOP        0x00000002 //!< TIM3 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM4_STOP        0x00000004 //!< TIM4 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM5_STOP        0x00000008 //!< TIM5 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM6_STOP        0x00000010 //!< TIM6 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM7_STOP        0x00000020 //!< TIM7 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM12_STOP       0x00000040 //!< TIM12 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM13_STOP       0x00000080 //!< TIM13 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM14_STOP       0x00000100 //!< TIM14 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_RTC_STOP         0x00000400 //!< RTC stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_WWDG_STOP        0x00000800 //!< Debug Window Watchdog stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_IWDEG_STOP       0x00001000 //!< Debug independent watchdog stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT  0x00200000 //!< I2C1 SMBUS timeout mode stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT  0x00400000 //!< I2C2 SMBUS timeout mode stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_I2C3_SMBUS_TIMEOUT  0x00800000 //!< I2C3 SMBUS timeout mode stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_CAN1_STOP           0x02000000 //!< Debug CAN1 stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_CAN2_STOP           0x04000000 //!< Debug CAN2 stopped when Core is halted
/** @} */

/** @defgroup DBGMCU_APB2FZ: (dbgmcu Offset: 0x0C) Debug MCU APB2 freeze register */
#define DBGMCU_APB1_FZ_DBG_TIM1_STOP        0x00000001 //!< TIM1 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM8_STOP        0x00000002 //!< TIM8 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM9_STOP        0x00010000 //!< TIM9 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM10_STOP       0x00020000 //!< TIM10 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM11_STOP       0x00040000 //!< TIM11 counter stopped when core is halted
/** @} */

/** @} */ // @defgroup RCC_DBGMCU_regs_define


#endif /* DBGMCU_F2_H_ */
