/*
 * flash_f0.h
 *
 *  Created on: Oct 27, 2014
 *      Author: miro
 */

#ifndef FLASH_F0_H_
#define FLASH_F0_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
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
	__IO uint32_t 	reserved;		//!< (flash Offset: 0x18)
	__IO uint32_t 	FLASH_OBR;		//!< (flash Offset: 0x1C) Option byte register
	__IO uint32_t 	FLASH_WRPR;		//!< (flash Offset: 0x20) Write protection register
} FLASH_TypeDef;

/*******************************************************************************
 * @defgroup FLASH_regs_define
 * @{
 ******************************************************************************/

/** @defgroup FLASH_ACR:	(flash Offset: 0x00) Flash access control register*/
#define FLASH_ACR_PRFTBS           0x00000020 //!< Prefetch buffer status
#define FLASH_ACR_PRFTEN           0x00000010 //!< Prefetch enable
#define FLASH_ACR_LATENCY          0x00000007 //!< LATENCY[2:0] bits (Latency)
#define FLASH_ACR_LATENCY_0WS      0x00000000 //!<  Zero wait state, if 0 < SYSCLK<= 24 MHz
#define FLASH_ACR_LATENCY_1WS      0x00000001 //!<  One wait state, if 24 MHz < SYSCLK <= 48 MHz
/** @} */

/** @defgroup FLASH_KEYR:	(flash Offset: 0x04) FPEC key register			  */
#define FLASH_KEYR_KEY	           0xFFFFFFFF //!< FPEC Key
#define FLASH_KEYR_KEY_1		   0x45670123 //!<  key 1
#define FLASH_KEYR_KEY_2           0xCDEF89AB //!<  key 2
/** @} */

/** @defgroup FLASH_OPTKEYR:(flash Offset: 0x08) Flash OPTKEY register		  */
#define FLASH_OPTKEYR_KEY	       0xFFFFFFFF //!< Option Byte Key
#define FLASH_OPTKEYR_KEY_1        0x08192A3B //!<  key 1
#define FLASH_OPTKEYR_KEY_2		   0x4C5D6E7F //!<  key 2
/** @} */

/** @defgroup FLASH_SR:		(flash Offset: 0x0C) Flash status register		  */
#define FLASH_SR_EOP               0x00000020 //!< End of operation
#define FLASH_SR_WRPERR            0x00000010 //!< Write protection error
#define FLASH_SR_PGERR             0x00000008 //!< Programming error
#define FLASH_SR_BSY               0x00000001 //!< Busy

#define FLASH_SR_ERRORS 		  (FLASH_SR_WRPERR | FLASH_SR_PGERR)

/** @} */

/** @defgroup FLASH_CR:		(flash Offset: 0x10) Flash control register		  */
#define FLASH_CR_OBL_LAUNCH        0x00002000 //!< Force option byte loading
#define FLASH_CR_EOPIE             0x00001000 //!< End of operation interrupt enable
#define FLASH_CR_ERRIE             0x00000400 //!< Error interrupt enable
#define FLASH_CR_OPTWRE            0x00000200 //!< Option bytes write enable
#define FLASH_CR_LOCK              0x80000080 //!< Lock
#define FLASH_CR_STRT              0x00000040 //!< Start
#define FLASH_CR_OPTER             0x00000020 //!< Option byte erase
#define FLASH_CR_OPTPG             0x00000010 //!< Option byte programming
#define FLASH_CR_MER               0x00000004 //!< Mass Erase
#define FLASH_CR_PER               0x00000002 //!< Page Erase
#define FLASH_CR_PG                0x00000001 //!< Programming
/** @} */


/** @defgroup FLASH_AR:		(flash Offset: 0x14) Flash address register        */
/** @} */

/** @defgroup reserved:		(flash Offset: 0x18)                               */
/** @} */

/** @defgroup FLASH_OBR:	(flash Offset: 0x1C) Option byte register          */
#define FLASH_OBR_RAM_PARITY_CHECK 0x00004000 //!< User option bytes
#define FLASH_OBR_VDDA_MONITOR	   0x00002000 //!< User option bytes
#define FLASH_OBR_nBOOT1		   0x00001000 //!< User option bytes
#define FLASH_OBR_nRST_STDBY	   0x00000400 //!< User option bytes
#define FLASH_OBR_nRST_STOP		   0x00000200 //!< User option bytes
#define FLASH_OBR_WDG_SW		   0x00000100 //!< User option bytes
/** @} */

/** @defgroup FLASH_WRPR:	(flash Offset: 0x20) Write protection register     */
/** @} */



/** @} */ // @defgroup FLASH_regs_define


uint32_t flash_unlock();
void flash_lock();
uint32_t flash_erase_page(uint32_t address);
uint32_t flash_erase_all();
uint32_t flash_write_byte(uint32_t address, uint32_t value);
uint32_t flash_write_hword(uint32_t address, uint32_t value);
uint32_t flash_write_word(uint32_t address, uint32_t value);
uint32_t flash_write(uint32_t address, const void* data, uint32_t len);
//uint32_t flash_ob_read();
//uint32_t flash_ob_write(uint32_t value);



#endif /* FLASH_F0_H_ */
