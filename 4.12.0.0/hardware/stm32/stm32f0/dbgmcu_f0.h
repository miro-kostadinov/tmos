/*
 * dbgmcu_f0.h
 *
 *  Created on: Oct 27, 2014
 *      Author: miro
 */

#ifndef DBGMCU_F0_H_
#define DBGMCU_F0_H_



#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  MCU debug component (DBGMCU)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DBGMCU_IDCODE;			//!< (dbgmcu offset: 0x00) MCU device ID code
  __IO uint32_t DBGMCU_CR;				//!< (dbgmcu Offset: 0x04) Debug MCU configuration register
  __IO uint32_t DBGMCU_APB1_FZ;		    //!< (dbgmcu Offset: 0x08) Debug MCU APB1 freeze register
  __IO uint32_t DBGMCU_APB2_FZ;  		//!< (dbgmcu Offset: 0x0C) Debug MCU APB2 freeze register

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
#define DBGMCU_CR_DBG_STANDBY       0x00000004 //!< Debug Standby mode
#define DBGMCU_CR_DBG_STOP          0x00000002 //!< Debug Stop Mode
/** @} */

/** @defgroup DBGMCU_APB1FZ: (dbgmcu Offset: 0x08) Debug MCU APB1 freeze register */
#define DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT  0x00200000 //!< I2C1 SMBUS timeout mode stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_IWDG_STOP        0x00001000 //!< Debug independent watchdog stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_WWDG_STOP        0x00000800 //!< Debug Window Watchdog stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_RTC_STOP         0x00000400 //!< RTC stopped when Core is halted
#define DBGMCU_APB1_FZ_DBG_TIM14_STOP       0x00000100 //!< TIM14 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM6_STOP        0x00000010 //!< TIM6 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM3_STOP        0x00000002 //!< TIM3 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM2_STOP        0x00000001 //!< TIM2 counter stopped when core is halted
/** @} */

/** @defgroup DBGMCU_APB2FZ: (dbgmcu Offset: 0x0C) Debug MCU APB2 freeze register */
#define DBGMCU_APB1_FZ_DBG_TIM17_STOP       0x00004000 //!< TIM17 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM16_STOP       0x00002000 //!< TIM10 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM15_STOP       0x00001000 //!< TIM9 counter stopped when core is halted
#define DBGMCU_APB1_FZ_DBG_TIM1_STOP        0x00000100 //!< TIM1 counter stopped when core is halted
/** @} */

/** @} */ // @defgroup RCC_DBGMCU_regs_define



#endif /* DBGMCU_F0_H_ */
