/**************************************************************************//**
 * @ingroup	hardware_stm32h7_crc
 * @file	hardware/stm32/stm32h7/crc_h7.h
 * @brief  	STM32H7 CRC
 * @date    5. December 2019
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_crc  CRC calculation unit (CRC)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 CRC calculation unit (CRC)
 * @{
 *
 ******************************************************************************/

#ifndef CRC_H7_H_
#define CRC_H7_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 * CRC calculation unit registers
 ******************************************************************************/
#ifndef __ASSEMBLY__
typedef struct
{
  __IO uint32_t CRC_DR;      //!< (crc Offset: 0x00) CRC Data register
  __IO uint32_t CRC_IDR;     //!< (crc offset: 0x04) CRC Independent data register
  __IO uint32_t CRC_CR;      //!< (crc offset: 0x08) CRC Control register
  __IO uint32_t reserved;	 //!< (crc offset: 0x0C) reserved
  __IO uint32_t CRC_INIT;  	 //!< (crc offset: 0x10) CRC initial value register
  __IO uint32_t POL;         //!< (crc offset: 0x14) CRC polynomial register
} CRC_TypeDef;
#endif /* __ASSEMBLY__ */

/***************************************************************************//**
 * @relates CRC_TypeDef
 * @{
 ******************************************************************************/

/** @name CRC_DR:   (crc Offset: 0x00) CRC Data register 	           	       */
/** @{ */
#define CRC_DR_DR	   		0xFFFFFFFF //!< Used as an input register when writing new data into the CRC calculator. Holds the previous CRC calculation result when it is read
/** @} */

/** @name CRC_IDR:  (crc offset: 0x04) CRC Independent data register  		   */
/** @{ */
#define CRC_IDR_IDR	   		0xFFFFFFFF //!< General-purpose 8-bit data register bits
/** @} */

/** @name CRC_CR:   (crc offset: 0x08) CRC Control register            		   */
/** @{ */
#define CRC_CR_RESET		0x00000001 //!< Resets the CRC calculation unit and sets the data register to 0xFFFF FFFF
#define CRC_CR_POLYSIZE		0x00000018 //!< Polynomial size
#define CRC_CR_POLYSIZE_32b	0x00000000 //!<  32 bit polynomial
#define CRC_CR_POLYSIZE_16b	0x00000008 //!<  16 bit polynomial
#define CRC_CR_POLYSIZE_8b	0x00000010 //!<  8 bit polynomial
#define CRC_CR_POLYSIZE_7b	0x00000018 //!<  7 bit polynomial
#define CRC_CR_REV_IN		0x00000060 //!< Reverse input data
#define CRC_CR_REV_IN_0b	0x00000000 //!<  Bit order not affected
#define CRC_CR_REV_IN_8b	0x00000020 //!<  Bit reversal done by byte
#define CRC_CR_REV_IN_16b	0x00000040 //!<  Bit reversal done by half-word
#define CRC_CR_REV_IN_32b	0x00000060 //!<  Bit reversal done by word
#define CRC_CR_REV_OUT		0x00000080 //!< Reverse output data
/** @} */


/** @} */ // @relates CRC_TypeDef

#ifndef __ASSEMBLY__
EXTERN_C unsigned int stm_crc32_le(const void* buf, unsigned int len);
#endif /* __ASSEMBLY__ */



#endif /* CRC_H7_H_ */

/** @} */

