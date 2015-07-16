/*
 * nvic_cm3.h
 *
 *  Created on: Dec 18, 2013
 *      Author: miro
 */

#ifndef NVIC_CM3_H_
#define NVIC_CM3_H_

#include <tmos_types.h>

/*******************************************************************************
 *  Nested Vectored Interrupt Controller (NVIC)
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct NVIC_Type
{
  __IO uint32_t NVIC_ISER[8]; 	//!< (NVIC Offset: 0x000) Interrupt Set Enable Register    */
	   uint32_t reserved0[24];	//!< (NVIC Offset: 0x020)
  __IO uint32_t NVIC_ICER[8];   //!< (NVIC Offset: 0x080) Interrupt Clear Enable Register  */
	   uint32_t rserved1[24];	//!< (NVIC Offset: 0x0A0)
  __IO uint32_t NVIC_ISPR[8];   //!< (NVIC Offset: 0x100) Interrupt Set Pending Register   */
	   uint32_t reserved2[24];	//!< (NVIC Offset: 0x120)
  __IO uint32_t NVIC_ICPR[8];   //!< (NVIC Offset: 0x180) Interrupt Clear Pending Register */
	   uint32_t reserved3[24];	//!< (NVIC Offset: 0x1A0)
  __IO uint32_t NVIC_IABR[8];   //!< (NVIC Offset: 0x200) Interrupt Active bit Register    */
	   uint32_t reserved4[56];	//!< (NVIC Offset: 0x220)
  __IO uint8_t  NVIC_IPR[240];  //!< (NVIC Offset: 0x300) Interrupt Priority Register (8Bit wide) */
	   uint32_t reserved5[644];	//!< (NVIC Offset: 0x3F0)
  __O  uint32_t NVIC_STIR;      //!< (NVIC Offset: 0xE00) Software Trigger Interrupt Register */
};
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup NVIC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup NVIC_ISER[8]:  (NVIC Offset: 0x000) Interrupt Set Enable Register    */
// 8*32=240 bits
/** @} */

/** @defgroup NVIC_ICER[8]:  (NVIC Offset: 0x080) Interrupt Clear Enable Register  */
// 8*32=240 bits
/** @} */

/** @defgroup NVIC_ISPR[8]:  (NVIC Offset: 0x100) Interrupt Set Pending Register   */
// 8*32=240 bits
/** @} */

/** @defgroup NVIC_ICPR[8]:  (NVIC Offset: 0x180) Interrupt Clear Pending Register */
// 8*32=240 bits
/** @} */

/** @defgroup NVIC_IABR[8]:  (NVIC Offset: 0x200) Interrupt Active bit Register    */
// 8*32=240 bits
/** @} */

/** @defgroup NVIC_IPR[240]: (NVIC Offset: 0x300) Interrupt Priority Register (8Bit wide) */
// 240 8-bit registers
//  The lower the value, the greater the priority of the corresponding interrupt. Register
// priority value fields are 8 bits wide, and un-implemented low-order bits read as zero
// and ignore writes

/** @} */

/** @defgroup NVIC_STIR:     (NVIC Offset: 0xE00) Software Trigger Interrupt Register */
#define NVIC_STIR_INTID 		   0x000000FF //!< Interrupt ID of the interrupt to trigger, in the range 0-239
/** @} */



/** @} */ // @defgroup NVIC_regs_define


#ifndef __ASSEMBLY__
EXTERN_C void NVIC_EnableIRQ(uint32_t IRQn);
EXTERN_C void NVIC_DisableIRQ(uint32_t IRQn);
EXTERN_C uint32_t NVIC_GetPendingIRQ(uint32_t IRQn);
EXTERN_C void NVIC_SetPendingIRQ(uint32_t IRQn);
EXTERN_C void NVIC_ClearPendingIRQ(uint32_t IRQn);
EXTERN_C uint32_t NVIC_GetActive(uint32_t IRQn);
EXTERN_C void NVIC_SetPriority(int32_t IRQn, uint32_t priority);
EXTERN_C uint32_t NVIC_GetPriority(int32_t IRQn);
EXTERN_C uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);
EXTERN_C void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);
#endif /* __ASSEMBLY__ */


#endif /* NVIC_CM3_H_ */
