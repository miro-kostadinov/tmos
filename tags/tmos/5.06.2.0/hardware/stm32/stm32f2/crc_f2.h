/*
 * crc_f2.h
 *
 *  Created on: Dec 21, 2013
 *      Author: miro
 */

#ifndef CRC_F2_H_
#define CRC_F2_H_


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
#define CRC_CR_RESET		0x00000001 //!< Resets the CRC calculation unit and sets the data register to 0xFFFF FFFF
/** @} */


/** @} */ // @defgroup CRC_regs_define

#ifndef __ASSEMBLY__
EXTERN_C unsigned int stm_crc32_le(const void* buf, unsigned int len);
#endif /* __ASSEMBLY__ */



#endif /* CRC_F2_H_ */
