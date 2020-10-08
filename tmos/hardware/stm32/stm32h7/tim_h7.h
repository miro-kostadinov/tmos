/**************************************************************************//**
 * @ingroup	hardware_stm32h7_tim
 * @file	hardware/stm32/stm32h7/tim_h7.h
 * @brief  	STM32H7 TIM
 * @date    28. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_tim  Timers (TIM)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 family timers (TIM)
 * @{
 *
 ******************************************************************************/

#ifndef TIM_H7_H_
#define TIM_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  STM32 H7 family timers
 ******************************************************************************
 *
 *	Timer channels	BIF	TIF	COMIF	BITS
 *	  1		4		 *	 *	  *		 16
 *	  2		4			 *			 32
 *	  3		4			 *			 16
 *	  4		4			 *			 16
 *	  5		4			 *			 32
 *	  6		0						 16
 *	  7		0						 16
 *	  8		4		 *	 *	  *		 16
 *	  9		2			 *			 16
 *	 10		1						 16
 *	 11		1						 16
 *	 12		2			 *			 16
 *	 13		1						 16
 *	 14		1						 16
 *
 *	IRQ
 *	 24		BIF1	9
 *	 25		UI1		10
 *	 26		TIF1	COMIF1	11
 *	 27		CC1
 *	 28		2
 *	 29		3
 *	 30		4
 *	 43		BIF8	12
 *	 44		UI8		13
 *	 45		TIF8	COMIF8	14
 *	 46		CC8
 *	 50		5
 *	 54		6 		(DACxx)
 *	 55		7
 *
 ******************************************************************************/

/***************************************************************************//**
 *  Timers
 ******************************************************************************/
typedef struct
{
  __IO uint32_t TIM_CR1;       //!< (adc Offset: 0x00) TIM control register 1
  __IO uint32_t TIM_CR2;       //!< (adc Offset: 0x04) TIM control register 2
  __IO uint32_t TIM_SMCR;      //!< (adc Offset: 0x08) TIM slave mode control register
  __IO uint32_t TIM_DIER;      //!< (adc Offset: 0x0C) TIM DMA/interrupt enable register
  __IO uint32_t TIM_SR;        //!< (adc Offset: 0x10) TIM status register
  __IO uint32_t TIM_EGR;       //!< (adc Offset: 0x14) TIM event generation register
  __IO uint32_t TIM_CCMR1;     //!< (adc Offset: 0x18) TIM capture/compare mode register 1
  __IO uint32_t TIM_CCER;      //!< (adc Offset: 0x20) TIM capture/compare enable register
  __IO uint32_t TIM_CNT;       //!< (adc Offset: 0x24) TIM counter register
  __IO uint32_t TIM_PSC;       //!< (adc Offset: 0x28) TIM prescaler
  __IO uint32_t TIM_ARR;       //!< (adc Offset: 0x2C) TIM auto-reload register
  __IO uint32_t TIM_CCR1;      //!< (adc Offset: 0x34) TIM capture/compare register 1-4
  __IO uint32_t TIM_CCR2;      //!< (adc Offset: 0x38) TIM capture/compare register 2
  __IO uint32_t TIM_TISEL;     //!< (adc Offset: 0x68) TIM Input Selection register
} TIM_TypeDef;

/*******************************************************************************
 * @relates TIM_TypeDef
 * @{
 ******************************************************************************/

/** @name TIM_CR1:      (adc Offset: 0x00) TIM control register 1			  */
/** @{ */
#define TIM_CR1_UIFREMAP       	 	0x0800 //!< UIF status bit remapping
#define TIM_CR1_CKD           		0x0300 //!< Clock division
#define TIM_CR1_ARPE 				0x0080 //!< Auto-reload preload enable
#define TIM_CR1_OPM                 0x0008 //!< One pulse mode
#define TIM_CR1_URS                 0x0004 //!< Update request source
#define TIM_CR1_UDIS                0x0002 //!< Update disable
#define TIM_CR1_CEN                 0x0001 //!< Counter enable
/** @} */

/** @name TIM_CR2:      (adc Offset: 0x04) TIM control register 2			  */
/** @{ */
#define TIM_CR2_TI1S                0x0000 //!< TI1 Selection
#define TIM_CR2_MMS 				0x0000 //!< Master mode selection
/** @} */

/** @name TIM_SMCR:     (adc Offset: 0x08) TIM slave mode control register (32 bit register)	 */
/** @{ */
#define TIM_SMCR_TS1				0x00300000 //!<
#define TIM_SMCR_SMS1				0x00010000 //!<
#define TIM_SMCR_MSM                0x00000080 //!< Master/slave mode
#define TIM_SMCR_TS2                0x00000070 //!< TS[2:0] bits (Trigger selection)
#define TIM_SMCR_SMS                0x00000007 //!< SMS[2:0] bits (Slave mode selection)
/** @} */

/** @name TIM_DIER:     (adc Offset: 0x0C) TIM DMA/interrupt enable register  */
/** @{ */
#define TIM_DIER_TIE                0x0040 //!< Trigger interrupt enable
#define TIM_DIER_CC2IE              0x0004 //!< Capture/Compare 2 interrupt enable
#define TIM_DIER_CC1IE              0x0002 //!< Capture/Compare 1 interrupt enable
#define TIM_DIER_UIE                0x0001 //!< Update interrupt enable
/** @} */

/** @name TIM_SR:       (adc Offset: 0x10) TIM status register				  */
/** @{ */
#define TIM_SR_CC2OF                0x0400 //!< Capture/Compare 2 Overcapture Flag
#define TIM_SR_CC1OF                0x0200 //!< Capture/Compare 1 Overcapture Flag
#define TIM_SR_TIF                  0x0040 //!< Trigger interrupt Flag
#define TIM_SR_CC2IF                0x0004 //!< Capture/Compare 2 interrupt Flag
#define TIM_SR_CC1IF                0x0002 //!< Capture/Compare 1 interrupt Flag
#define TIM_SR_UIF                  0x0001 //!< Update interrupt Flag
/** @} */

/** @name TIM_EGR:      (adc Offset: 0x14) TIM event generation register 	  */
/** @{ */
#define TIM_EGR_TG                  0x0040 //!< Trigger Generation
#define TIM_EGR_CC2G                0x0004 //!< Capture/Compare 2 Generation
#define TIM_EGR_CC1G                0x0002 //!< Capture/Compare 1 Generation
#define TIM_EGR_UG                  0x0001 //!< Update Generation
/** @} */

/** @name TIM_CCMR1:    (adc Offset: 0x18) TIM capture/compare mode register 1 (32 bit register) */
/** @{ */
#define TIM_CCMR1_IC2F              0x0000F000 //!< IC2F[3:0] bits (Input Capture 2 Filter)
#define TIM_CCMR1_IC2PSC            0x00000C00 //!< IC2PSC[1:0] bits (Input Capture 2 Prescaler)
#define TIM_CCMR1_CC2S 				0x00000300 //!< Capture/compare 2 selection
#define TIM_CCMR1_IC1F              0x000000F0 //!< IC1F[3:0] bits (Input Capture 1 Filter)
#define TIM_CCMR1_IC1PSC            0x0000000C //!< IC1PSC[1:0] bits (Input Capture 1 Prescaler)
#define TIM_CCMR1_CC1S 				0x00000003 //!< Capture/Compare 1 selection
/** @} */

/** @name TIM_CCER:     (adc Offset: 0x20) TIM capture/compare enable register  */
/** @{ */
#define TIM_CCER_CC2NP              0x0080 //!< Capture/Compare 2 Complementary output Polarity
#define TIM_CCER_CC2P               0x0020 //!< Capture/Compare 2 output Polarity
#define TIM_CCER_CC2E               0x0010 //!< Capture/Compare 2 output enable
#define TIM_CCER_CC1NP              0x0008 //!< Capture/Compare 1 Complementary output Polarity
#define TIM_CCER_CC1P               0x0002 //!< Capture/Compare 1 output Polarity
#define TIM_CCER_CC1E               0x0001 //!< Capture/Compare 1 output enable
/** @} */

/** @name TIM_CNT:      (adc Offset: 0x24) TIM counter register	(32 bit register)			  */
/** @{ */
#define TIM_CNT_UIFCPY 				0x00000000 //!< UIF Copy
#define TIM_CNT_CNT                 0x0000FFFF //!< Counter Value
/** @} */

/** @name TIM_PSC:      (adc Offset: 0x28) TIM prescaler					  */
/** @{ */
#define TIM_PSC_PSC                 0xFFFF //!< Prescaler Value
/** @} */

/** @name TIM_ARR:      (adc Offset: 0x2C) TIM auto-reload register			  */
/** @{ */
#define TIM_ARR_ARR                 0xFFFF //!< actual auto-reload Value
/** @} */

/** @name TIM_CCR1:     (adc Offset: 0x34) TIM capture/compare register 1	  */
/** @{ */
#define TIM_CCR1_CCR1               0xFFFF //!< Capture/Compare 1 Value
/** @} */

/** @name TIM_CCR2:     (adc Offset: 0x38) TIM capture/compare register 2	  */
/** @{ */
#define TIM_CCR2_CCR2               0xFFFF //!< Capture/Compare 2 Value
/** @} */

/** @name TIM_TISEL:    (adc Offset: 0x68) TIM Input Selection register		  */
/** @{ */
#define TIM_TISEL_TI2SEL 			0x0000 //!< selects TI2[0] to TI2[15] input
#define TIM_TISEL_TI1SEL			0x0000 //!< selects TI1[0] to TI1[15] input
/** @} */


// The clock of the involved Timer counter is stopped when the core is halted
#define DebugFreezeTimer(periph_id) 						\
{															\
	__IO uint32_t* reg;										\
    if(periph_id < ID_NO_PERIPH)							\
    {														\
		reg = &DBGMCU->DBGMCU_APB1FZ;						\
		reg[((periph_id)>>5)-4] |= 1<<((periph_id)&0x1f);	\
    }														\
}



/** @} */ // @relates TIM_TypeDef



#endif /* TIM_H7_H_ */

/** @} */
