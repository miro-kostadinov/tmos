/*
 * crc_f0.h
 *
 *  Created on: Oct 28, 2014
 *      Author: miro
 */

#ifndef CRC_F0_H_
#define CRC_F0_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 * CRC calculation unit registers
 ******************************************************************************/
#ifndef __ASSEMBLY__
typedef struct
{
  __IO uint32_t CRC_DR;      //!< (crc Offset: 0x00) CRC Data register
  __IO uint32_t CRC_IDR;     //!< (crc offset: 0x04) CRC Independent data register
  __IO uint32_t CRC_CR;      //!< (crc offset: 0x08) CRC Control register
  __IO uint32_t CRC_INIT;    //!< (crc offset: 0x08) Initial CRC value
} CRC_TypeDef;
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup CRCC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup CRC_DR:   (crc Offset: 0x00) CRC Data register                  */
#define CRC_DR_DR	   		0xFFFFFFFF //!< Used as an input register when writing new data into the CRC calculator. Holds the previous CRC calculation result when it is read
/** @} */

/** @defgroup CRC_IDR:  (crc offset: 0x04) CRC Independent data register      */
#define CRC_IDR_IDR	   		0x000000FF //!< General-purpose 8-bit data register bits
/** @} */

/** @defgroup CRC_CR:   (crc offset: 0x08) CRC Control register               */
#define CRC_CR_REV_OUT		0x00000100 //!< Bit 7 REV_OUT: Reverse output data
#define CRC_CR_REV_IN		0x000000C0 //!< Bits 6:5 REV_IN[1:0]: Reverse input data
#define CRC_CR_REV_IN_NO	0x00000000 //!<  Bit order not affected
#define CRC_CR_REV_IN_8 	0x00000040 //!<  Bit reversal done by byte
#define CRC_CR_REV_IN_16	0x00000080 //!<  Bit reversal done by half-word
#define CRC_CR_REV_IN_32	0x000000C0 //!<  Bit reversal done by word
#define CRC_CR_RESET		0x00000001 //!< Resets the CRC calculation unit and sets the data register to 0xFFFF FFFF
/** @} */


/** @} */ // @defgroup CRC_regs_define

#ifndef __ASSEMBLY__
EXTERN_C unsigned int stm_crc32_le(const void* buf, unsigned int len);
#endif /* __ASSEMBLY__ */



#endif /* CRC_F0_H_ */
