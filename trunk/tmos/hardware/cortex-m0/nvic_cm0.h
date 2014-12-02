/*
 * nvic_cm0.h
 *
 *  Created on: Sep 9, 2014
 *      Author: miro
 */

#ifndef NVIC_CM0_H_
#define NVIC_CM0_H_


#include <tmos_types.h>

/*******************************************************************************
 *  Nested Vectored Interrupt Controller (NVIC) 0xE000E100
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct NVIC_Type
{
  __IO uint32_t NVIC_ISER;	 	//!< (NVIC Offset: 0x000) Interrupt Set Enable Register
	   uint32_t reserved0[31];	//!< (NVIC Offset: 0x004)
  __IO uint32_t NVIC_ICER;		//!< (NVIC Offset: 0x080) Interrupt Clear Enable Register
	   uint32_t rserved1[31];	//!< (NVIC Offset: 0x084)
  __IO uint32_t NVIC_ISPR;      //!< (NVIC Offset: 0x100) Interrupt Set Pending Register
	   uint32_t reserved2[31];	//!< (NVIC Offset: 0x104)
  __IO uint32_t NVIC_ICPR;      //!< (NVIC Offset: 0x180) Interrupt Clear Pending Register
	   uint32_t reserved3[95];	//!< (NVIC Offset: 0x184)
  __IO uint32_t NVIC_IPR[8];	//!< (NVIC Offset: 0x300) Interrupt Priority Register (8Bit wide) */
};
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup NVIC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup NVIC_ISER[8]:  (NVIC Offset: 0x000) Interrupt Set Enable Register    */
// 32 bits
/** @} */

/** @defgroup NVIC_ICER[8]:  (NVIC Offset: 0x080) Interrupt Clear Enable Register  */
// 32 bits
/** @} */

/** @defgroup NVIC_ISPR[8]:  (NVIC Offset: 0x100) Interrupt Set Pending Register   */
// 320 bits
/** @} */

/** @defgroup NVIC_ICPR[8]:  (NVIC Offset: 0x180) Interrupt Clear Pending Register */
// 32 bits
/** @} */

/** @defgroup NVIC_IPR[240]: (NVIC Offset: 0x300) Interrupt Priority Register (8Bit wide) */
// 32 8-bit registers
//  The lower the value, the greater the priority of the corresponding interrupt. Register
// priority value fields are 8 bits wide, and un-implemented low-order bits read as zero
// and ignore writes

/** @} */




/** @} */ // @defgroup NVIC_regs_define


#ifndef __ASSEMBLY__
EXTERN_C void NVIC_EnableIRQ(uint32_t IRQn);
EXTERN_C void NVIC_DisableIRQ(uint32_t IRQn);
EXTERN_C uint32_t NVIC_GetPendingIRQ(uint32_t IRQn);
EXTERN_C void NVIC_SetPendingIRQ(uint32_t IRQn);
EXTERN_C void NVIC_ClearPendingIRQ(uint32_t IRQn);
EXTERN_C void NVIC_SetPriority(int32_t IRQn, uint32_t priority);
EXTERN_C uint32_t NVIC_GetPriority(int32_t IRQn);
#endif /* __ASSEMBLY__ */


#endif /* NVIC_CM0_H_ */
