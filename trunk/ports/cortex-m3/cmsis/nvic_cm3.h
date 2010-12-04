/**************************************************************************//**
 * @ingroup	 cores_cmsis
 * @{
 * @file     cortex-m3/cmsis/nvic_cm3.h
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 * @}
 *
 ******************************************************************************/
#ifndef NVIC_CM3_H
#define NVIC_CM3_H


/**
 * @ingroup	 cores_cmsis
 * @defgroup CMSIS_CM3_NVIC CMSIS CM3 NVIC
  memory mapped structure for Nested Vectored Interrupt Controller (NVIC)
  @{
 */
struct NVIC_Type
{
  __IO uint32_t ISER[8];    	/*!< Offset: 0x000  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];	/*!<														*/
  __IO uint32_t ICER[8];        /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];	/*!<														*/
  __IO uint32_t ISPR[8];        /*!< Offset: 0x100  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];	/*!<														*/
  __IO uint32_t ICPR[8];        /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];	/*!<														*/
  __IO uint32_t IABR[8];        /*!< Offset: 0x200  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];	/*!<														*/
  __IO uint8_t  IP[240];        /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];	/*!<														*/
  __O  uint32_t STIR;           /*!< Offset: 0xE00  Software Trigger Interrupt Register     */

  void 			NVIC_EnableIRQ(DRIVER_INDEX IRQn);
  void 			NVIC_DisableIRQ(DRIVER_INDEX IRQn);
  uint32_t 		NVIC_GetPendingIRQ(DRIVER_INDEX IRQn);
  void 			NVIC_SetPendingIRQ(DRIVER_INDEX IRQn);
  void 			NVIC_ClearPendingIRQ(DRIVER_INDEX IRQn);
  uint32_t 		NVIC_GetActive(DRIVER_INDEX IRQn);
  void 			NVIC_SetPriority(DRIVER_INDEX IRQn, uint32_t priority);
  uint32_t 		NVIC_GetPriority(DRIVER_INDEX IRQn);

};

#define NVIC ((NVIC_Type *) NVIC_BASE)  /*!< NVIC configuration struct        */

uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority);
void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority);

/** @} end of group CMSIS_CM3_NVIC */

/**
 * @ingroup	 cores_cmsis
 * @defgroup CMSIS_CM3_InterruptType CMSIS CM3 Interrupt Type
  memory mapped structure for Interrupt Type
  @{
 */
typedef struct
{
       uint32_t RESERVED0; /*!<			  									  */
  __I  uint32_t ICTR;      /*!< Offset: 0x04  Interrupt Control Type Register */
  __IO uint32_t ACTLR;     /*!< Offset: 0x08  Auxiliary Control Register      */
} InterruptType_Type;

/* Interrupt Controller Type Register Definitions */
#define InterruptType_ICTR_INTLINESNUM_Pos  0                                             /*!< InterruptType ICTR: INTLINESNUM Position */
#define InterruptType_ICTR_INTLINESNUM_Msk (0x1Ful << InterruptType_ICTR_INTLINESNUM_Pos) /*!< InterruptType ICTR: INTLINESNUM Mask */

/* Auxiliary Control Register Definitions */
#define InterruptType_ACTLR_DISFOLD_Pos     2                                             /*!< InterruptType ACTLR: DISFOLD Position */
#define InterruptType_ACTLR_DISFOLD_Msk    (1ul << InterruptType_ACTLR_DISFOLD_Pos)       /*!< InterruptType ACTLR: DISFOLD Mask */

#define InterruptType_ACTLR_DISDEFWBUF_Pos  1                                             /*!< InterruptType ACTLR: DISDEFWBUF Position */
#define InterruptType_ACTLR_DISDEFWBUF_Msk (1ul << InterruptType_ACTLR_DISDEFWBUF_Pos)    /*!< InterruptType ACTLR: DISDEFWBUF Mask */

#define InterruptType_ACTLR_DISMCYCINT_Pos  0                                             /*!< InterruptType ACTLR: DISMCYCINT Position */
#define InterruptType_ACTLR_DISMCYCINT_Msk (1ul << InterruptType_ACTLR_DISMCYCINT_Pos)    /*!< InterruptType ACTLR: DISMCYCINT Mask */

#define InterruptType       ((InterruptType_Type *) SCS_BASE)         /*!< Interrupt Type Register           */

/** @} end of group CMSIS_CM3_InterruptType */

#endif /* NVIC_CM3_H */
