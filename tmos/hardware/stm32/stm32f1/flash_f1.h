/**************************************************************************//**
 * @ingroup	hardware_stm32f1_flash
 * @file	hardware/stm32/stm32f1/flash_f1.h
 * @brief  	STM32F1 Flash
 * @date    25. October 2012
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f1_flash  Flash program and erase controller (FPEC)
 * @ingroup	 hardware_stm32f1
 * Source and definitions for STM32F1 Flash program and erase controller (FPEC)
 * @{
 *
 ******************************************************************************/

#ifndef FLASH_F1_H_
#define FLASH_F1_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Flash program and erase controller (FPEC) registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t 	FLASH_ACR;		//!< (flash Offset: 0x00) Flash access control register
	__IO uint32_t 	FLASH_KEYR;		//!< (flash Offset: 0x04) FPEC key register
	__IO uint32_t 	FLASH_OPTKEYR;	//!< (flash Offset: 0x08) Flash OPTKEY register
	__IO uint32_t 	FLASH_SR;		//!< (flash Offset: 0x0C) Flash status register
	__IO uint32_t 	FLASH_CR;		//!< (flash Offset: 0x10) Flash control register
	__IO uint32_t 	FLASH_AR;		//!< (flash Offset: 0x14) Flash address register
	__IO uint32_t 	RESERVED;		//!< (flash Offset: 0x18) reserved
	__IO uint32_t 	FLASH_OBR;		//!< (flash Offset: 0x1C) Option byte register
	__IO uint32_t 	FLASH_WRPR;		//!< (flash Offset: 0x20) Write protection register
	#ifdef STM32F10X_XL
	  uint32_t 		RESERVED1[8]; 	//!< (flash Offset: 0x24) reserved
	  __IO uint32_t FLASH_KEYR2;	//!< (flash Offset: 0x44) FPEC key register2
	  uint32_t 		RESERVED2;   	//!< (flash Offset: 0x48) reserved
	  __IO uint32_t FLASH_SR2;		//!< (flash Offset: 0x4C) Flash status register2
	  __IO uint32_t FLASH_CR2;		//!< (flash Offset: 0x50) Flash control register2
	  __IO uint32_t FLASH_AR2; 		//!< (flash Offset: 0x54) Flash address register2
	#endif /* STM32F10X_XL */
} FLASH_TypeDef;

/***************************************************************************//**
 * @relates FLASH_TypeDef
 * @{
 ******************************************************************************/

/** @name FLASH_ACR:	(flash Offset: 0x00) Flash access control register	  */
/** @{ */
#define  FLASH_ACR_LATENCY          0x00000007 //!<  LATENCY[2:0] bits (Latency)
#define  FLASH_ACR_LATENCY_0WS      0x00000000 //!<  Zero wait state, if 0 < SYSCLK <= 24 MHz
#define  FLASH_ACR_LATENCY_1WS      0x00000001 //!<  One wait state, if 24 MHz < SYSCLK <= 48 MHz
#define  FLASH_ACR_LATENCY_2WS      0x00000002 //!<  Two wait states, if 48 MHz < SYSCLK <= 72 MHz

#define  FLASH_ACR_HLFCYA           0x00000008 //!<  Flash Half Cycle Access Enable
#define  FLASH_ACR_PRFTBE           0x00000010 //!<  Prefetch Buffer Enable
#define  FLASH_ACR_PRFTBS           0x00000020 //!<  Prefetch Buffer Status
/** @} */

/** @name FLASH_KEYR:	(flash Offset: 0x04) FPEC key register				  */
/** @{ */
#define  FLASH_KEYR_FKEYR           0xFFFFFFFF //!<  FPEC Key
/** @} */

/** @name FLASH_OPTKEYR:(flash Offset: 0x08) Flash OPTKEY register			  */
/** @{ */
#define  FLASH_OPTKEYR_OPTKEYR      0xFFFFFFFF //!<  Option Byte Key
/** @} */

/** @name FLASH_SR:		(flash Offset: 0x0C) Flash status register			  */
/** @{ */
#define  FLASH_SR_BSY               0x00000001 //!<  Busy
#define  FLASH_SR_PGERR             0x00000004 //!<  Programming Error
#define  FLASH_SR_WRPRTERR          0x00000010 //!<  Write Protection Error
#define  FLASH_SR_EOP               0x00000020 //!<  End of operation
/** @} */

/** @name FLASH_CR:		(flash Offset: 0x10) Flash control register			  */
/** @{ */
#define  FLASH_CR_PG                0x00000001 //!<  Programming
#define  FLASH_CR_PER               0x00000002 //!<  Page Erase
#define  FLASH_CR_MER               0x00000004 //!<  Mass Erase
#define  FLASH_CR_OPTPG             0x00000010 //!<  Option Byte Programming
#define  FLASH_CR_OPTER             0x00000020 //!<  Option Byte Erase
#define  FLASH_CR_STRT              0x00000040 //!<  Start
#define  FLASH_CR_LOCK              0x00000080 //!<  Lock
#define  FLASH_CR_OPTWRE            0x00000200 //!<  Option Bytes Write Enable
#define  FLASH_CR_ERRIE             0x00000400 //!<  Error Interrupt Enable
#define  FLASH_CR_EOPIE             0x00001000 //!<  End of operation interrupt enable
/** @} */

/** @name FLASH_AR:		(flash Offset: 0x14) Flash address register  		  */
/** @{ */
#define  FLASH_AR_FAR               0xFFFFFFFF //!<  Flash Address
/** @} */

/** @name FLASH_OBR:	(flash Offset: 0x1C) Option byte register			  */
/** @{ */
#define  FLASH_OBR_OPTERR           0x00000001 //!<  Option Byte Error
#define  FLASH_OBR_RDPRT            0x00000002 //!<  Read protection

#define  FLASH_OBR_USER             0x000003FC //!<  User Option Bytes
#define  FLASH_OBR_WDG_SW           0x00000004 //!<  WDG_SW
#define  FLASH_OBR_nRST_STOP        0x00000008 //!<  nRST_STOP
#define  FLASH_OBR_nRST_STDBY       0x00000010 //!<  nRST_STDBY
#define  FLASH_OBR_BFB2             0x00000020 //!<  BFB2
/** @} */

/** @name FLASH_WRPR:	(flash Offset: 0x20) Write protection register		  */
/** @{ */
#define  FLASH_WRPR_WRP             0xFFFFFFFF //!<  Write Protect
/** @} */



/** @} */ // @relates FLASH_TypeDef



#endif /* FLASH_F1_H_ */

/** @} */
