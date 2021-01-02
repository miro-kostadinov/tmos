/**************************************************************************//**
 * @ingroup	hardware_stm32h7_dbgmcu
 * @file	hardware/stm32/stm32h7/dbgmcu_h7.h
 * @brief  	STM32H7 DBGMCU
 * @date    28. December 2019
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_dbgmcu  MCU debug component (DBGMCU)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 MCU debug component (DBGMCU)
 * @{
 *
 ******************************************************************************/

#ifndef DBGMCU_H7_H_
#define DBGMCU_H7_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  MCU debug component (DBGMCU)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DBGMCU_IDCODE;      //!< (dbgmcu offset: 0x00) MCU device ID code
  __IO uint32_t DBGMCU_CR;          //!< (dbgmcu offset: 0x04) Debug MCU configuration register
  __IO uint32_t reserved1[11];      //!< (dbgmcu offset: 0x08) reserved
  __IO uint32_t DBGMCU_APB3FZ1;     //!< (dbgmcu offset: 0x34) Debug MCU APB3FZ1 freeze register
  __IO uint32_t reserved2;          //!< (dbgmcu offset: 0x38) reserved
  __IO uint32_t DBGMCU_APB1LFZ1;    //!< (dbgmcu offset: 0x3C) Debug MCU APB1LFZ1 freeze register
  __IO uint32_t reserved3;          //!< (dbgmcu offset: 0x40) reserved
  __IO uint32_t DBGMCU_APB1HFZ1;    //!< (dbgmcu offset: 0x44) Debug MCU APB1HFZ1 freeze register
  __IO uint32_t reserved4;          //!< (dbgmcu offset: 0x48) reserved
  __IO uint32_t DBGMCU_APB2FZ1;     //!< (dbgmcu offset: 0x4C) Debug MCU APB2FZ1 freeze register
  __IO uint32_t reserved5;          //!< (dbgmcu offset: 0x50) reserved
  __IO uint32_t DBGMCU_APB4FZ1;     //!< (dbgmcu offset: 0x54) Debug MCU APB4FZ1 freeze register
} DBGMCU_TypeDef;


/***************************************************************************//**
 * @relates DBGMCU_TypeDef
 * @{
 ******************************************************************************/

/** @name DBGMCU_IDCODE:	(dbgmcu offset: 0x00) MCU device ID code		  */
/** @{ */
#define DBGMCU_IDCODE_REV_ID        0xFFFF0000 //!< REV_ID[15:0] bits (Revision Identifier)
#define DBGMCU_IDCODE_REV_ID_Z      0x10010000 //!<  Revision Z
#define DBGMCU_IDCODE_REV_ID_Y      0x10030000 //!<  Revision Y
#define DBGMCU_IDCODE_REV_ID_X      0x20010000 //!<  Revision X
#define DBGMCU_IDCODE_REV_ID_V      0x20030000 //!<  Revision V
#define DBGMCU_IDCODE_DEV_ID        0x00000FFF //!< Device Identifier
#define DBGMCU_IDCODE_DEV_ID_450    0x00000450 //!<  STM32H742, STM32H743/753 and STM32H750
/** @} */

/** @name DBGMCU_CR:	(dbgmcu Offset: 0x04) Debug MCU configuration register  */
/** @{ */
#define DBGMCU_CR_TRGOEN	        0x10000000 //!< External trigger output enable
#define DBGMCU_CR_D3DBGCKEN	        0x00400000 //!< D3 debug clock enable
#define DBGMCU_CR_D1DBGCKEN         0x00200000 //!< D1 debug clock enable
#define DBGMCU_CR_TRACECLKEN        0x00100000 //!< Trace port clock enable
#define DBGMCU_CR_DBGSTBY_D3        0x00000100 //!< Allow debug in D3 Standby mode
#define DBGMCU_CR_DBGSTOP_D3        0x00000080 //!< Allow debug in D3 Stop mode
#define DBGMCU_CR_DBGSTBY_D1        0x00000004 //!< Allow D1 domain debug in Standby mode
#define DBGMCU_CR_DBGSTOP_D1        0x00000002 //!< Allow D1 domain debug in Stop mode
#define DBGMCU_CR_DBGSLEEP_D1	    0x00000001 //!< Allow D1 domain debug in Sleep mode
/** @} */

/** @name DBGMCU_APB3FZ1:  (dbgmcu offset: 0x34) Debug MCU APB3FZ1 freeze register */
/** @{ */
#define DBGMCU_APB3FZ1_WWDG1   		0x00000000 //!< WWDG1 stop in debug
/** @} */

/** @name DBGMCU_APB1LFZ1: (dbgmcu offset: 0x3C) Debug MCU APB1LFZ1 freeze register*/
/** @{ */
#define DBGMCU_APB1LFZ1_DBG_I2C3   	0x00800000 //!< I2C3 SMBUS timeout stop in debug
#define DBGMCU_APB1LFZ1_DBG_I2C2   	0x00400000 //!< I2C2 SMBUS timeout stop in debug
#define DBGMCU_APB1LFZ1_DBG_I2C1   	0x00200000 //!< I2C1 SMBUS timeout stop in debug
#define DBGMCU_APB1LFZ1_DBG_LPTIM1 	0x00000200 //!< LPTIM1 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM14 	0x00000100 //!< TIM14 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM13  	0x00000080 //!< TIM13 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM12  	0x00000040 //!< TIM12 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM7   	0x00000020 //!< TIM7 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM6   	0x00000010 //!< TIM6 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM5   	0x00000008 //!< TIM5 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM4   	0x00000004 //!< TIM4 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM3   	0x00000002 //!< TIM3 stop in debug
#define DBGMCU_APB1LFZ1_DBG_TIM2   	0x00000001 //!< TIM2 stop in debug
/** @} */

/** @name DBGMCU_APB1HFZ1: (dbgmcu offset: 0x44) Debug MCU APB1HFZ1 freeze register*/
/** @{ */
#define DBGMCU_APB1HFZ1_DBG_FDCAN  	0x00000100 //!< FDCAN stop in debug
/** @} */

/** @name DBGMCU_APB2FZ1:  (dbgmcu offset: 0x4C) Debug MCU APB2FZ1 freeze register */
/** @{ */
#define DBGMCU_APB2FZ1_DBG_HRTIM	0x20000000 //!< HRTIM stop in debug
#define DBGMCU_APB2FZ1_DBG_TIM17	0x00040000 //!< TIM17 stop in debug
#define DBGMCU_APB2FZ1_DBG_TIM16	0x00020000 //!< TIM16 stop in debug
#define DBGMCU_APB2FZ1_DBG_TIM15	0x00010000 //!< TIM15 stop in debug
#define DBGMCU_APB2FZ1_DBG_TIM8		0x00000002 //!< TIM8 stop in debug
#define DBGMCU_APB2FZ1_DBG_TIM1		0x00000001 //!< TIM1 stop in debug
/** @} */

/** @name DBGMCU_APB4FZ1:  (dbgmcu offset: 0x54) Debug MCU APB4FZ1 freeze register */
/** @{ */
#define DBGMCU_APB4FZ1_DBG_IIWDG1	0x00040000 //!< Independent watchdog for D1 stop in debug
#define DBGMCU_APB4FZ1_DBG_RTC 		0x00010000 //!< RTC stop in debug
#define DBGMCU_APB4FZ1_DBG_LPTIM5	0x00001000 //!< LPTIM5 stop in debug
#define DBGMCU_APB4FZ1_DBG_LPTIM4	0x00000800 //!< LPTIM4 stop in debug
#define DBGMCU_APB4FZ1_DBG_LPTIM3	0x00000400 //!< LPTIM3 stop in debug
#define DBGMCU_APB4FZ1_DBG_LPTIM2	0x00000200 //!< LPTIM2 stop in debug
#define DBGMCU_APB4FZ1_DBG_I2C4		0x00000080 //!< I2C4 SMBUS timeout stop in debug
/** @} */


/** @} */ // @relates DBGMCU_TypeDef


#endif /* DBGMCU_H7_H_ */

/** @} */
