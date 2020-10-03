/**************************************************************************//**
 * @ingroup	hardware_stm32h7_flash
 * @file	hardware/stm32/stm32h7/flash_h7.h
 * @brief  	STM32F2 Flash
 * @date    26. January 2020
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_flash Embedded Flash memory (FLASH)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Embedded Flash memory (FLASH)
 * @{
 *
 ******************************************************************************/

#ifndef FLASH_H7_H_
#define FLASH_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Embedded Flash memory (FLASH) registers
 ******************************************************************************/
typedef struct
{
	__IO uint32_t FLASH_ACR; 		//!< (flash Offset: 0x000) FLASH access control register
	__IO uint32_t FLASH_KEYR;       //!< (flash Offset: 0x004) Flash Key Register
	__IO uint32_t FLASH_OPTKEYR;    //!< (flash Offset: 0x008) Flash Option Key Register
	__IO uint32_t FLASH_CR;         //!< (flash Offset: 0x00C) Flash Control Register
	__IO uint32_t FLASH_SR;         //!< (flash Offset: 0x010) Flash Status Register
	__IO uint32_t FLASH_CCR;        //!< (flash Offset: 0x014) Flash clear control register
	__IO uint32_t FLASH_OPTCR;      //!< (flash Offset: 0x018) Flash Option Control Register
	__IO uint32_t FLASH_OPTSR_CUR;  //!< (flash Offset: 0x01C) Flash Option Status Current Register
	__IO uint32_t FLASH_OPTSR_PRG;  //!< (flash Offset: 0x020) Flash Option Status to Program Register
	__IO uint32_t FLASH_OPTCCR;     //!< (flash Offset: 0x024) Flash Option Clear Control Register
	__IO uint32_t FLASH_PRAR_CUR;   //!< (flash Offset: 0x028) Flash Current Protection Address Register
	__IO uint32_t FLASH_PRAR_PRG;   //!< (flash Offset: 0x02C) Flash Protection Address to Program Register
	__IO uint32_t FLASH_SCAR_CUR;   //!< (flash Offset: 0x030) Flash Current Secure Address Register
	__IO uint32_t FLASH_SCAR_PRG;	//!< (flash Offset: 0x034) Flash Secure Address to Program Register
	__IO uint32_t FLASH_WPSN_CUR;   //!< (flash Offset: 0x038) Flash Current Write Protection Register
	__IO uint32_t FLASH_WPSN_PRG;   //!< (flash Offset: 0x03C) Flash Write Protection to Program Register
	__IO uint32_t FLASH_BOOT_CUR;   //!< (flash Offset: 0x040) Flash Current Boot Address
	__IO uint32_t FLASH_BOOT_PRG;   //!< (flash Offset: 0x044) Flash Boot Address to Program
	__IO uint32_t reserved0[2]; 	//!< (flash Offset: 0x048) reserved
	__IO uint32_t FLASH_CRCCR;      //!< (flash Offset: 0x050) Flash CRC Control register
	__IO uint32_t FLASH_CRCSADD;    //!< (flash Offset: 0x054) Flash CRC Start Address Register
	__IO uint32_t FLASH_CRCEADD;    //!< (flash Offset: 0x058) Flash CRC End Address Register
	__IO uint32_t FLASH_CRCDATA;    //!< (flash Offset: 0x05C) Flash CRC Data Register
	__IO uint32_t FLASH_ECC_FA;     //!< (flash Offset: 0x060) Flash ECC Fail Address
	__IO uint32_t reserved1[39]; 	//!< (flash Offset: 0x064) reserved
} FLASH_BankDef;

typedef struct
{
	FLASH_BankDef fl_bank[2];
} FLASH_TypeDef;

/***************************************************************************//**
 * @relates FLASH_BankDef
 * @{
 ******************************************************************************/

/** @name FLASH_ACR: 		(flash Offset: 0x000) FLASH access control register */
/** @{ */
#define FLASH_ACR_WRHIGHFREQ       0x00000030 //!< Flash signal delay
#define FLASH_ACR_LATENCY          0x0000000F //!< Read latency
#define FLASH_ACR_LATENCY_0WS      0x00000000 //!<  Zero wait state
#define FLASH_ACR_LATENCY_1WS      0x00000001 //!<  One wait state
#define FLASH_ACR_LATENCY_2WS      0x00000002 //!<  Two wait states
#define FLASH_ACR_LATENCY_3WS      0x00000003 //!<  Three wait states
#define FLASH_ACR_LATENCY_4WS      0x00000004 //!<  4 wait states
/** @} */

/** @name FLASH_KEYR:       (flash Offset: 0x004) Flash Key Register		   */
/** @{ */
#define FLASH_KEYR_KEYR	           0xFFFFFFFF //!< bank configuration access unlock key
#define FLASH_KEYR_KEYR_1		   0x45670123 //!<  key 1
#define FLASH_KEYR_KEYR_2          0xCDEF89AB //!<  key 2
/** @} */

/** @name FLASH_OPTKEYR:    (flash Offset: 0x008) Flash Option Key Register    */
/** @{ */
#define FLASH_OPTKEYR_OPTKEYR      0xFFFFFFFF //!< FLASH option bytes control access unlock key
#define FLASH_OPTKEYR_OPTKEYR_1    0x08192A3B //!<  key 1
#define FLASH_OPTKEYR_OPTKEYR_2	   0x4C5D6E7F //!<  key 2
/** @} */

/** @name FLASH_CR:         (flash Offset: 0x00C) Flash Control Register 	   */
/** @{ */
#define FLASH_CR_CRCENDIE		   0x08000000 //!< Bank CRC end of calculation interrupt enable bit
#define FLASH_CR_DBECCERRIE		   0x04000000 //!< Bank ECC double detection error interrupt enable bit
#define FLASH_CR_SNECCERRIE		   0x02000000 //!< Bank ECC single correction error interrupt enable bit
#define FLASH_CR_RDSERRIE		   0x01000000 //!< Bank secure error interrupt enable bit
#define FLASH_CR_RDPERRIE		   0x00800000 //!< Bank read protection error interrupt enable bit
#define FLASH_CR_OPERRIE		   0x00400000 //!< Bank write/erase error interrupt enable bit
#define FLASH_CR_INCERRIE		   0x00200000 //!< Bank inconsistency error interrupt enable bit
#define FLASH_CR_STRBERRIE		   0x00080000 //!< Bank strobe error interrupt enable bit
#define FLASH_CR_PGSERRIE		   0x00040000 //!< Bank programming sequence error interrupt enable bit
#define FLASH_CR_WRPERRIE		   0x00020000 //!< Bank write protection error interrupt enable bit
#define FLASH_CR_EOPIE			   0x00010000 //!< Bank end-of-program interrupt control bit
#define FLASH_CR_CRC_EN			   0x00008000 //!< Bank CRC control bit
#define FLASH_CR_SNB 			   0x00000700 //!< Bank sector erase selection number
#define FLASH_CR_START			   0x00000080 //!< Bank erase start control bit
#define FLASH_CR_FW	  			   0x00000040 //!< Bank write forcing control bit
#define FLASH_CR_PSIZE			   0x00000030 //!< Bank program size
#define FLASH_CR_BER			   0x00000008 //!< Bank erase request
#define FLASH_CR_SER 			   0x00000004 //!< Bank sector erase request
#define FLASH_CR_PG  			   0x00000002 //!< Bank internal buffer control bit
#define FLASH_CR_LOCK			   0x00000001 //!< Bank configuration lock bit
/** @} */

/** @name FLASH_SR:         (flash Offset: 0x010) Flash Status Register		   */
/** @{ */
#define FLASH_SR_CRCEND	    	   0x08000000 //!< Bank CRC end of calculation flag
#define FLASH_SR_DBECCERR	       0x04000000 //!< Bank ECC double detection error flag
#define FLASH_SR_SNECCERR	       0x02000000 //!< Bank single correction error flag
#define FLASH_SR_RDSERR            0x01000000 //!< Bank secure error flag
#define FLASH_SR_RDPERR            0x00800000 //!< Bank read protection error flag
#define FLASH_SR_OPERR	           0x00400000 //!< Bank write/erase error flag
#define FLASH_SR_INCERR	           0x00200000 //!< Bank inconsistency error flag
#define FLASH_SR_STRBERR           0x00080000 //!< Bank strobe error flag
#define FLASH_SR_PGSERR	           0x00040000 //!< Bank programming sequence error flag
#define FLASH_SR_WRPERR 	       0x00020000 //!< Bank write protection error flag
#define FLASH_SR_EOP        	   0x00010000 //!< Bank end-of-program flag
#define FLASH_SR_CRC_BUSY     	   0x00000008 //!< Bank CRC busy flag
#define FLASH_SR_QW                0x00000004 //!< Bank wait queue flag
#define FLASH_SR_WBNE	           0x00000002 //!< Bank write buffer not empty flag
#define FLASH_SR_BSY 	           0x00000001 //!< Bank busy flag

#define FLASH_SR_ERRORS (FLASH_SR_DBECCERR | FLASH_SR_SNECCERR | FLASH_SR_RDSERR | \
		FLASH_SR_RDPERR | FLASH_SR_OPERR | FLASH_SR_INCERR | FLASH_SR_STRBERR | \
		FLASH_SR_PGSERR | FLASH_SR_WRPERR)

/** @} */


/** @name FLASH_CCR:        (flash Offset: 0x014) Flash clear control register */
/** @{ */
#define FLASH_CCR_CLR_CRCEND	   0x08000000 //!< Bank CRCEND flag clear bit
#define FLASH_CCR_CLR_DBECCERR	   0x04000000 //!< Bank DBECCERR flag clear bit
#define FLASH_CCR_CLR_SNECCERR	   0x02000000 //!< Bank SNECCERR flag clear bit
#define FLASH_CCR_CLR_RDSERR	   0x01000000 //!< Bank RDSERR flag clear bit
#define FLASH_CCR_CLR_RDPERR	   0x00800000 //!< Bank RDPERR flag clear bit
#define FLASH_CCR_CLR_OPERR		   0x00400000 //!< Bank OPERR flag clear bit
#define FLASH_CCR_CLR_INCERR	   0x00200000 //!< Bank INCERR flag clear bit
#define FLASH_CCR_CLR_STRBERR	   0x00080000 //!< Bank STRBERR flag clear bit
#define FLASH_CCR_CLR_PGSERR	   0x00040000 //!< Bank PGSERR flag clear bit
#define FLASH_CCR_CLR_WRPERR	   0x00020000 //!< Bank WRPERR flag clear bit
#define FLASH_CCR_CLR_EOP		   0x00010000 //!< Bank EOP flag clear bit
/** @} */

/** @name FLASH_OPTCR:      (flash Offset: 0x018) Flash Option Control Register */
/** @{ */
#define FLASH_OPTCR_SWAP_BANK	   0x80000000 //!< Bank swapping option configuration bit
#define FLASH_OPTCR_OPTCHANGEERRIE 0x40000000 //!< Option byte change error interrupt enable bit
#define FLASH_OPTCR_MER			   0x00000010 //!< mass erase request
#define FLASH_OPTCR_OPTSTART	   0x00000002 //!< Option byte start change option configuration bit
#define FLASH_OPTCR_OPTLOCK		   0x00000001 //!< FLASH_OPTCR lock option configuration bit
/** @} */

/** @name FLASH_OPTSR_CUR:  (flash Offset: 0x01C) Flash Option Status Current Register */
/** @{ */
#define FLASH_OPTSR_CUR_SWAP_BANK_OPT   0x80000000 //!< Bank swapping option status bit
#define FLASH_OPTSR_CUR_OPTCHANGEERR    0x40000000 //!< Option byte change error flag
#define FLASH_OPTSR_CUR_IO_HSLV		    0x20000000 //!< I/O high-speed at low-voltage status bit
#define FLASH_OPTSR_CUR_SECURITY	    0x00200000 //!< Security enable option status bit
#define FLASH_OPTSR_CUR_ST_RAM_SIZE	    0x00180000 //!< ST RAM size option status
#define FLASH_OPTSR_CUR_ST_RAM_SIZE_2K  0x00000000 //!<  2 Kbytes reserved to ST code
#define FLASH_OPTSR_CUR_ST_RAM_SIZE_4K  0x00080000 //!<  4 Kbytes reserved to ST code
#define FLASH_OPTSR_CUR_ST_RAM_SIZE_8K  0x00100000 //!<  8 Kbytes reserved to ST code
#define FLASH_OPTSR_CUR_ST_RAM_SIZE_16K 0x00180000 //!<  16 Kbytes reserved to ST code
#define FLASH_OPTSR_CUR_IWDG_FZ_SDBY    0x00040000 //!< IWDG Standby mode freeze option status bit
#define FLASH_OPTSR_CUR_17 IWDG_FZ_STOP 0x00020000 //!< IWDG Stop mode freeze option status bit
#define FLASH_OPTSR_CUR_RDP	  		    0x0000FF00 //!< Readout protection level option status byte
#define FLASH_OPTSR_CUR_NRST_STDY_D1    0x00000080 //!< D1 domain DStandby entry reset option status bit
#define FLASH_OPTSR_CUR_NRST_STOP_D1    0x00000040 //!< D1 domain DStop entry reset option status bit
#define FLASH_OPTSR_CUR_IWDG1_SW 	    0x00000010 //!< IWDG1 control mode option status bit
#define FLASH_OPTSR_CUR_BOR_LEV  	    0x0000000C //!< Brownout level option status bit
#define FLASH_OPTSR_CUR_BOR_LEV_0  	    0x00000000 //!<  V BOR0 , brownout reset threshold 0
#define FLASH_OPTSR_CUR_BOR_LEV_1  	    0x00000004 //!<  V BOR1 , brownout reset threshold 1
#define FLASH_OPTSR_CUR_BOR_LEV_2  	    0x00000008 //!<  V BOR2 , brownout reset threshold 2
#define FLASH_OPTSR_CUR_BOR_LEV_3  	    0x0000000C //!<  V BOR3 , brownout reset threshold 3
#define FLASH_OPTSR_CUR_OPT_BUSY 	    0x00000001 //!< Option byte change ongoing flag
/** @} */

/** @name FLASH_OPTSR_PRG:  (flash Offset: 0x020) Flash Option Status to Program Register */
/** @{ */
#define FLASH_OPTSR_PRG_SWAP_BANK_OPT   0x80000000 //!< Bank swapping option configuration bit
#define FLASH_OPTSR_PRG_IO_HSLV		    0x20000000 //!< I/O high-speed at low-voltage configuration bit
#define FLASH_OPTSR_PRG_SECURITY	    0x00200000 //!< Security enable option configuration bit
#define FLASH_OPTSR_PRG_ST_RAM_SIZE	    0x00180000 //!< ST RAM size option configuration bits
#define FLASH_OPTSR_PRG_ST_RAM_SIZE_2K  0x00000000 //!<  2 Kbytes reserved to ST code
#define FLASH_OPTSR_PRG_ST_RAM_SIZE_4K  0x00080000 //!<  4 Kbytes reserved to ST code
#define FLASH_OPTSR_PRG_ST_RAM_SIZE_8K  0x00100000 //!<  8 Kbytes reserved to ST code
#define FLASH_OPTSR_PRG_ST_RAM_SIZE_16K 0x00180000 //!<  16 Kbytes reserved to ST code
#define FLASH_OPTSR_PRG_IWDG_FZ_SDBY    0x00040000 //!< IWDG Standby mode freeze option configuration bit
#define FLASH_OPTSR_PRG_17 IWDG_FZ_STOP 0x00020000 //!< IWDG Stop mode freeze option configuration bit
#define FLASH_OPTSR_PRG_RDP	  		    0x0000FF00 //!< Readout protection level option configuration bits
#define FLASH_OPTSR_PRG_NRST_STDY_D1    0x00000080 //!< D1 domain DStandby entry reset option configuration bit
#define FLASH_OPTSR_PRG_NRST_STOP_D1    0x00000040 //!< D1 domain DStop entry reset option configuration bit
#define FLASH_OPTSR_PRG_IWDG1_SW 	    0x00000010 //!< IWDG1 control mode option configuration bit
#define FLASH_OPTSR_PRG_BOR_LEV  	    0x0000000C //!< Brownout level option configuration bit
#define FLASH_OPTSR_PRG_BOR_LEV_0  	    0x00000000 //!<  V BOR0 , brownout reset threshold 0
#define FLASH_OPTSR_PRG_BOR_LEV_1  	    0x00000004 //!<  V BOR1 , brownout reset threshold 1
#define FLASH_OPTSR_PRG_BOR_LEV_2  	    0x00000008 //!<  V BOR2 , brownout reset threshold 2
#define FLASH_OPTSR_PRG_BOR_LEV_3  	    0x0000000C //!<  V BOR3 , brownout reset threshold 3
/** @} */

/** @name FLASH_OPTCCR:     (flash Offset: 0x024) Flash Option Clear Control Register */
/** @{ */
#define FLASH_OPTCCR_CLR_OPTCHANGEERR   0x40000000 //!< OPTCHANGEERR reset bit
/** @} */

/** @name FLASH_PRAR_CUR:   (flash Offset: 0x028) Flash Current Protection Address Register */
/** @{ */
#define FLASH_PRAR_CUR_DMEP        		0x80000000 //!< Bank PCROP protected erase enable option status bit
#define FLASH_PRAR_CUR_PROT_AREA_END	0x0FFF0000 //!< Bank PCROP area end status bits
#define FLASH_PRAR_CUR_PROT_AREA_START  0x00000FFF //!< Bank PCROP area start status bits
/** @} */

/** @name FLASH_PRAR_PRG:   (flash Offset: 0x02C) Flash Protection Address to Program Register */
/** @{ */
#define FLASH_PRAR_PRG_DMEP        		0x80000000 //!< Bank PCROP protected erase enable option configuration bit
#define FLASH_PRAR_PRG_PROT_AREA_END	0x0FFF0000 //!< Bank PCROP area end configuration bits
#define FLASH_PRAR_PRG_PROT_AREA_START  0x00000FFF //!< Bank PCROP area start configuration bits
/** @} */

/** @name FLASH_SCAR_CUR:   (flash Offset: 0x030) Flash Current Secure Address Register */
/** @{ */
#define FLASH_SCAR_CUR_DMES        		0x80000000 //!< Bank secure access protected erase enable option status bit
#define FLASH_SCAR_CUR_SEC_AREA_END		0x0FFF0000 //!< Bank secure-only area end status bits
#define FLASH_SCAR_CUR_SEC_AREA_START   0x00000FFF //!< Bank secure-only area start status bits
/** @} */

/** @name FLASH_SCAR_PRG:   (flash Offset: 0x034) Flash Secure Address to Program Register */
/** @{ */
#define FLASH_SCAR_PRG_DMES        		0x80000000 //!< Bank secure access protected erase enable option configuration bit
#define FLASH_SCAR_PRG_SEC_AREA_END		0x0FFF0000 //!< Bank secure-only area end configuration bits
#define FLASH_SCAR_PRG_SEC_AREA_START   0x00000FFF //!< Bank secure-only area start configuration bits
/** @} */

/** @name FLASH_WPSN_CUR:   (flash Offset: 0x038) Flash Current Write Protection Register */
/** @{ */
#define FLASH_WPSN_CUR_WPSN			    0x000000FF //!< Bank sector write protection option status byte
/** @} */

/** @name FLASH_WPSN_PRG:   (flash Offset: 0x03C) Flash Write Protection to Program Register */
/** @{ */
#define FLASH_WPSN_PRG_WPSN			    0x000000FF //!< Bank sector write protection option status byte
/** @} */

/** @name FLASH_BOOT_CUR:   (flash Offset: 0x040) Flash Current Boot Address   */
/** @{ */
#define FLASH_BOOT_CUR_BOOT_ADD1	    0xFFFF0000 //!< Arm Cortex-M7 boot address 1
#define FLASH_BOOT_CUR_BOOT_ADD0	    0x0000FFFF //!< Arm Cortex-M7 boot address 0
/** @} */

/** @name FLASH_BOOT_PRG:   (flash Offset: 0x044) Flash Boot Address to Program */
/** @{ */
#define FLASH_BOOT_PRG_BOOT_ADD1	    0xFFFF0000 //!< Arm Cortex-M7 boot address 1
#define FLASH_BOOT_PRG_BOOT_ADD0	    0x0000FFFF //!< Arm Cortex-M7 boot address 0
/** @} */

/** @name FLASH_CRCCR:      (flash Offset: 0x050) Flash CRC Control register   */
/** @{ */
#define FLASH_CRCCR_CRC_BURST		    0x00300000 //!< Bank CRC burst size
#define FLASH_CRCCR_CRC_BURST_4		    0x00000000 //!<  every burst has a size of 4 Flash words (256-bit)
#define FLASH_CRCCR_CRC_BURST_16	    0x00100000 //!<  every burst has a size of 16 Flash words (256-bit)
#define FLASH_CRCCR_CRC_BURST_64	    0x00200000 //!<  every burst has a size of 64 Flash words (256-bit)
#define FLASH_CRCCR_CRC_BURST_256	    0x00300000 //!<  every burst has a size of 256 Flash words (256-bit)
#define FLASH_CRCCR_CLEAN_CRC		    0x00020000 //!< Bank CRC clear bit
#define FLASH_CRCCR_START_CRC		    0x00010000 //!< Bank CRC start bit
#define FLASH_CRCCR_CLEAN_SECT		    0x00000400 //!< Bank CRC sector list clear bit
#define FLASH_CRCCR_ADD_SECT		    0x00000200 //!< Bank CRC sector select bit
#define FLASH_CRCCR_CRC_BY_SECT		    0x00000100 //!< Bank CRC sector mode select bit
#define FLASH_CRCCR_ALL_BANK		    0x00000080 //!< Bank CRC select bit
#define FLASH_CRCCR_CRC_SECT		    0x00000007 //!< Bank CRC sector number
/** @} */

/** @name FLASH_CRCSADD:    (flash Offset: 0x054) Flash CRC Start Address Register */
/** @{ */
#define FLASH_CRCSADD_CRC_START_ADDR    0x000FFFFC //!< CRC start address
/** @} */

/** @name FLASH_CRCEADD:    (flash Offset: 0x058) Flash CRC End Address Register */
/** @{ */
#define FLASH_CRCEADD_CRC_END_ADDR	    0x000FFFFC //!< CRC end address
/** @} */

/** @name FLASH_CRCDATA:    (flash Offset: 0x05C) Flash CRC Data Register  	   */
/** @{ */
#define FLASH_CRCDATA_CRC_DATA		    0xFFFFFFFF //!< CRC result
/** @} */

/** @name FLASH_ECC_FA:     (flash Offset: 0x060) Flash ECC Fail Address 	   */
/** @{ */
#define FLASH_ECC_FA_FAIL_ECC_ADDR	    0x00007FFF //!< ECC error address
/** @} */


/** @} */ // @relates FLASH_BankDef



uint32_t flash_unlock();
void flash_lock();
uint32_t flash_erase_page(uint32_t address);
uint32_t flash_erase_all();
uint32_t flash_write_byte(uint32_t address, uint32_t value);
uint32_t flash_write_hword(uint32_t address, uint32_t value);
uint32_t flash_write_word(uint32_t address, uint32_t value);
uint32_t flash_write(uint32_t address, const void* data, uint32_t len);
uint32_t flash_ob_read();
uint32_t flash_ob_write(uint32_t value);



#endif /* FLASH_H7_H_ */

/** @} */
