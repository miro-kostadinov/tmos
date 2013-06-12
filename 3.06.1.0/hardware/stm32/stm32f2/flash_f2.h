/*
 * flash_f2.h
 *
 *  Created on: Oct 29, 2012
 *      Author: miro
 */

#ifndef FLASH_F2_H_
#define FLASH_F2_H_

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
	__IO uint32_t 	FLASH_OPTCR;	//!< (flash Offset: 0x14) FLASH option control register
} FLASH_TypeDef;

/*******************************************************************************
 * @defgroup FLASH_regs_define
 * @{
 ******************************************************************************/

/** @defgroup FLASH_ACR:	(flash Offset: 0x00) Flash access control register*/
#define FLASH_ACR_LATENCY          0x00000007 //!<  LATENCY[2:0] bits (Latency)
#define FLASH_ACR_LATENCY_0WS      0x00000000 //!<  Zero wait state
#define FLASH_ACR_LATENCY_1WS      0x00000001 //!<  One wait state
#define FLASH_ACR_LATENCY_2WS      0x00000002 //!<  Two wait states
#define FLASH_ACR_LATENCY_3WS      0x00000003 //!<  Three wait states
#define FLASH_ACR_LATENCY_4WS      0x00000004 //!<  4 wait states
#define FLASH_ACR_LATENCY_5WS      0x00000005 //!<  5 wait states
#define FLASH_ACR_LATENCY_6WS      0x00000006 //!<  6 wait states
#define FLASH_ACR_LATENCY_7WS      0x00000007 //!<  7 wait states

#define FLASH_ACR_PRFTEN           0x00000100 //!< Prefetch enable
#define FLASH_ACR_ICEN             0x00000200 //!< Instruction cache enable
#define FLASH_ACR_DCEN             0x00000400 //!< Data cache enable
#define FLASH_ACR_ICRST            0x00000800 //!< Instruction cache reset
#define FLASH_ACR_DCRST            0x00001000 //!< Data cache reset
#define FLASH_ACR_BYTE0_ADDRESS    0x40023C00 //!<
#define FLASH_ACR_BYTE2_ADDRESS    0x40023C03 //!<
/** @} */

/** @defgroup FLASH_KEYR:	(flash Offset: 0x04) FPEC key register			  */
#define FLASH_KEYR_FKEYR           0xFFFFFFFF //!<  FPEC Key
/** @} */

/** @defgroup FLASH_OPTKEYR:(flash Offset: 0x08) Flash OPTKEY register		  */
#define FLASH_OPTKEYR_OPTKEYR      0xFFFFFFFF //!<  Option Byte Key
/** @} */

/** @defgroup FLASH_SR:		(flash Offset: 0x0C) Flash status register		  */
#define FLASH_SR_EOP               0x00000001 //!< End of operation
#define FLASH_SR_OPERR             0x00000002 //!< Operation error
#define FLASH_SR_WRPERR            0x00000010 //!< Write protection error
#define FLASH_SR_PGAERR            0x00000020 //!< Programming alignment error
#define FLASH_SR_PGPERR            0x00000040 //!< Programming parallelism error
#define FLASH_SR_PGSERR            0x00000080 //!< Programming sequence error
#define FLASH_SR_BSY               0x00010000 //!< Busy
/** @} */

/** @defgroup FLASH_CR:		(flash Offset: 0x10) Flash control register		  */
#define FLASH_CR_PG                0x00000001 //!< Programming
#define FLASH_CR_SER               0x00000002 //!< Sector Erase
#define FLASH_CR_MER               0x00000004 //!< Mass Erase

#define FLASH_CR_SNB               0x00000078 //!< Sector number. These bits select the sector to erase

#define FLASH_CR_PSIZE             0x00000300 //!< Program size
#define FLASH_CR_PSIZE_x8          0x00000000 //!< program x8
#define FLASH_CR_PSIZE_x16         0x00000100 //!< program x16
#define FLASH_CR_PSIZE_x32         0x00000200 //!< program x32
#define FLASH_CR_PSIZE_x64         0x00000300 //!< program x64

#define FLASH_CR_STRT              0x00010000 //!< Start
#define FLASH_CR_EOPIE             0x01000000 //!< End of operation interrupt enable
#define FLASH_CR_ERRIE             0x02000000 //!< Error interrupt enable
#define FLASH_CR_LOCK              0x80000000 //!< Lock
/** @} */

/** @defgroup FLASH_OPTCR:	(flash Offset: 0x14) FLASH option control register */
#define FLASH_OPTCR_OPTLOCK        0x00000001 //!< Option lock
#define FLASH_OPTCR_OPTSTRT        0x00000002 //!< Option start

#define FLASH_OPTCR_BOR_LEV        0x0000000C //!< BOR reset Level
#define FLASH_OPTCR_BOR_LEV_VBOR3  0x00000004 //!<  reset threshold level for 2.70 to 3.60 V voltage range
#define FLASH_OPTCR_BOR_LEV_VBOR2  0x00000008 //!<  reset threshold level for 2.40 to 2.70 V voltage range
#define FLASH_OPTCR_BOR_LEV_VBOR1  0x00000008 //!<  reset threshold level for 2.10 to 2.40 V voltage range
#define FLASH_OPTCR_BOR_LEV_VBOR0  0x00000008 //!<  reset threshold level for 1.80 to 2.10 V voltage range

#define FLASH_OPTCR_USER           0x000000E0 //!< User option bytes
#define FLASH_OPTCR_WDG_SW         0x00000020 //!<
#define FLASH_OPTCR_nRST_STOP      0x00000040 //!<
#define FLASH_OPTCR_nRST_STDBY     0x00000080 //!<

#define FLASH_OPTCR_RDP            0x0000FF00 //!< Read protect
#define FLASH_OPTCR_RDP_0          0x00000100 //!<
#define FLASH_OPTCR_RDP_1          0x00000200 //!<
#define FLASH_OPTCR_RDP_2          0x00000400 //!<
#define FLASH_OPTCR_RDP_3          0x00000800 //!<
#define FLASH_OPTCR_RDP_4          0x00001000 //!<
#define FLASH_OPTCR_RDP_5          0x00002000 //!<
#define FLASH_OPTCR_RDP_6          0x00004000 //!<
#define FLASH_OPTCR_RDP_7          0x00008000 //!<

#define FLASH_OPTCR_nWRP	       0x0FFF0000 //!< Not write protect
#define FLASH_OPTCR_nWRP_0         0x00010000 //!<
#define FLASH_OPTCR_nWRP_1         0x00020000 //!<
#define FLASH_OPTCR_nWRP_2         0x00040000 //!<
#define FLASH_OPTCR_nWRP_3         0x00080000 //!<
#define FLASH_OPTCR_nWRP_4         0x00100000 //!<
#define FLASH_OPTCR_nWRP_5         0x00200000 //!<
#define FLASH_OPTCR_nWRP_6         0x00400000 //!<
#define FLASH_OPTCR_nWRP_7         0x00800000 //!<
#define FLASH_OPTCR_nWRP_8         0x01000000 //!<
#define FLASH_OPTCR_nWRP_9         0x02000000 //!<
#define FLASH_OPTCR_nWRP_10        0x04000000 //!<
#define FLASH_OPTCR_nWRP_11        0x08000000 //!<
/** @} */


/** @} */ // @defgroup FLASH_regs_define





#endif /* FLASH_F2_H_ */
