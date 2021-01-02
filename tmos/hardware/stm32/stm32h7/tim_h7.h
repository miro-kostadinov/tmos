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
  __IO uint32_t TIM_CR1;       //!< (tim Offset: 0x00) TIM control register 1
  __IO uint32_t TIM_CR2;       //!< (tim Offset: 0x04) TIM control register 2
  __IO uint32_t TIM_SMCR;      //!< (tim Offset: 0x08) TIM slave mode control register
  __IO uint32_t TIM_DIER;      //!< (tim Offset: 0x0C) TIM DMA/interrupt enable register
  __IO uint32_t TIM_SR;        //!< (tim Offset: 0x10) TIM status register
  __IO uint32_t TIM_EGR;       //!< (tim Offset: 0x14) TIM event generation register
  __IO uint32_t TIM_CCMR1;     //!< (tim Offset: 0x18) TIM capture/compare mode register 1
  __IO uint32_t TIM_CCMR2;     //!< (tim Offset: 0x1C) TIM capture/compare mode register 2
  __IO uint32_t TIM_CCER;      //!< (tim Offset: 0x20) TIM capture/compare enable register
  __IO uint32_t TIM_CNT;       //!< (tim Offset: 0x24) TIM counter register
  __IO uint32_t TIM_PSC;       //!< (tim Offset: 0x28) TIM prescaler
  __IO uint32_t TIM_ARR;       //!< (tim Offset: 0x2C) TIM auto-reload register
  __IO uint32_t TIM_RCR;       //!< (tim Offset: 0x30) TIM repetition counter register
  __IO uint32_t TIM_CCRx[4];   //!< (tim Offset: 0x34) TIM capture/compare register TIM_CCR1-TIM_CCR4
  __IO uint32_t TIM_BDTR;      //!< (tim Offset: 0x44) TIM break and dead-time register
  __IO uint32_t TIM_DCR;       //!< (tim Offset: 0x48) TIM DMA control register
  __IO uint32_t TIM_DMAR;      //!< (tim Offset: 0x4C) TIM DMA address for full transfer
  __I  uint32_t reserved1;     //!< (tim Offset: 0x50) reserved
  __IO uint32_t TIM_CCMR3;     //!< (tim Offset: 0x54) TIM capture/compare mode register 3
  __IO uint32_t TIM_CCR5;      //!< (tim Offset: 0x58) TIM capture/compare register5
  __IO uint32_t TIM_CCR6;      //!< (tim Offset: 0x5C) TIM capture/compare register6
  __IO uint32_t TIM_AF1;       //!< (tim Offset: 0x60) TIM alternate function option register 1
  __IO uint32_t TIM_AF2;       //!< (tim Offset: 0x64) TIM alternate function option register 2
  __IO uint32_t TIM_TISEL;     //!< (tim Offset: 0x68) TIM Input Selection register
} TIM_TypeDef;

/*******************************************************************************
 * @relates TIM_TypeDef
 * @{
 ******************************************************************************/

/** @name TIM_CR1:      (tim Offset: 0x00) TIM control register 1			  */
/** @{ */
#define TIM_CR1_UIFREMAP 			0x0800 //!< UIF status bit remapping
#define TIM_CR1_CKD                 0x0300 //!< CKD[1:0] bits (clock division)
#define TIM_CR1_ARPE                0x0080 //!< Auto-reload preload enable
#define TIM_CR1_CMS                 0x0060 //!< CMS[1:0] bits (Center-aligned mode selection)
#define TIM_CR1_DIR                 0x0010 //!< Direction
#define TIM_CR1_OPM                 0x0008 //!< One pulse mode
#define TIM_CR1_URS                 0x0004 //!< Update request source
#define TIM_CR1_UDIS                0x0002 //!< Update disable
#define TIM_CR1_CEN                 0x0001 //!< Counter enable
/** @} */

/** @name TIM_CR2:      (tim Offset: 0x04) TIM control register 2			  */
/** @{ */
#define TIM_CR2_MMS2 				0x00F00000 //!< Master mode selection 2
#define TIM_CR2_MMS2_RESET 			0x00000000 //!<  the UG bit from the TIMx_EGR register is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_ENABLE			0x00100000 //!<  the Counter Enable signal CNT_EN is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_UPDATE			0x00200000 //!<  the update event is selected as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMPPULSE		0x00300000 //!<  the trigger output sends a positive pulse when the CC1IF flag is to be set
#define TIM_CR2_MMS2_CMP1 			0x00400000 //!<  OC1REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMP2 			0x00500000 //!<  OC2REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMP3 			0x00600000 //!<  OC3REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMP4			0x00700000 //!<  OC4REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMP5 			0x00800000 //!<  OC5REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMP6 			0x00900000 //!<  OC6REF signal is used as trigger output (TRGO2)
#define TIM_CR2_MMS2_CMPPULSE1		0x00A00000 //!<  OC4REF rising or falling edges generate pulses on TRGO2
#define TIM_CR2_MMS2_CMPPULSE2		0x00B00000 //!<  OC6REF rising or falling edges generate pulses on TRGO2
#define TIM_CR2_MMS2_CMPPULSE3		0x00C00000 //!<  OC4REF or OC6REF rising edges generate pulses on TRGO2
#define TIM_CR2_MMS2_CMPPULSE4		0x00D00000 //!<  OC4REF rising or OC6REF falling edges generate pulses on TRGO2
#define TIM_CR2_MMS2_CMPPULSE5		0x00E00000 //!<  OC5REF or OC6REF rising edges generate pulses on TRGO2
#define TIM_CR2_MMS2_CMPPULSE6		0x00F00000 //!<  OC5REF rising or OC6REF falling edges generate pulses on TRGO2
#define TIM_CR2_OIS6  				0x00040000 //!< Output Idle state 6 (OC6 output)
#define TIM_CR2_OIS5  				0x00010000 //!< Output Idle state 5 (OC5 output)
#define TIM_CR2_OIS4                0x00004000 //!< Output Idle state 4 (OC4 output)
#define TIM_CR2_OIS3N               0x00002000 //!< Output Idle state 3 (OC3N output)
#define TIM_CR2_OIS3                0x00001000 //!< Output Idle state 3 (OC3 output)
#define TIM_CR2_OIS2N               0x00000800 //!< Output Idle state 2 (OC2N output)
#define TIM_CR2_OIS2                0x00000400 //!< Output Idle state 2 (OC2 output)
#define TIM_CR2_OIS1N               0x00000200 //!< Output Idle state 1 (OC1N output)
#define TIM_CR2_OIS1                0x00000100 //!< Output Idle state 1 (OC1 output)
#define TIM_CR2_TI1S                0x00000080 //!< TI1 Selection
#define TIM_CR2_MMS                 0x00000070 //!< MMS[2:0] bits (Master Mode Selection)
#define TIM_CR2_MMS_RESET           0x00000000 //!<  Reset - the UG bit from the TIMx_EGR register is used as trigger output (TRGO)
#define TIM_CR2_MMS_ENABLE          0x00000010 //!<  Enable - the Counter Enable signal CNT_EN is used as trigger output (TRGO)
#define TIM_CR2_MMS_UPDATE          0x00000020 //!<  Update - The update event is selected as trigger output (TRGO)
#define TIM_CR2_MMS_CC1IF           0x00000030 //!<  Compare Pulse - The trigger output send a positive pulse when the CC1IF flag is to be set (even if it was already high)
#define TIM_CR2_MMS_OC1REF          0x00000040 //!<  Compare - OC1REF signal is used as trigger output (TRGO)
#define TIM_CR2_MMS_OC2REF          0x00000050 //!<  Compare - OC2REF signal is used as trigger output (TRGO)
#define TIM_CR2_MMS_OC3REF          0x00000060 //!<  Compare - OC3REF signal is used as trigger output (TRGO)
#define TIM_CR2_MMS_OC4REF          0x00000070 //!<  Compare - OC4REF signal is used as trigger output (TRGO)
#define TIM_CR2_CCDS                0x00000008 //!< Capture/Compare DMA Selection
#define TIM_CR2_CCUS                0x00000004 //!< Capture/Compare Control Update Selection
#define TIM_CR2_CCPC                0x00000001 //!< Capture/Compare Preloaded Control
/** @} */

/** @name TIM_SMCR:     (tim Offset: 0x08) TIM slave mode control register	 */
/** @{ */
#define TIM_SMCR_TS 				0x00300070 //!< Trigger selection
#define TIM_SMCR_SMS 				0x00010007 //!< Slave mode selection
#define TIM_SMCR_ETP                0x00008000 //!< External trigger polarity
#define TIM_SMCR_ECE                0x00004000 //!< External clock enable
#define TIM_SMCR_ETPS               0x00003000 //!< External trigger prescaler
#define TIM_SMCR_ETF                0x00000F00 //!< External trigger filter
#define TIM_SMCR_MSM                0x00000080 //!< Master/slave mode
/** @} */

/** @name TIM_DIER:     (tim Offset: 0x0C) TIM DMA/interrupt enable register  */
/** @{ */
#define TIM_DIER_TDE                0x4000 //!< Trigger DMA request enable
#define TIM_DIER_COMDE              0x2000 //!< COM DMA request enable
#define TIM_DIER_CC4DE              0x1000 //!< Capture/Compare 4 DMA request enable
#define TIM_DIER_CC3DE              0x0800 //!< Capture/Compare 3 DMA request enable
#define TIM_DIER_CC2DE              0x0400 //!< Capture/Compare 2 DMA request enable
#define TIM_DIER_CC1DE              0x0200 //!< Capture/Compare 1 DMA request enable
#define TIM_DIER_UDE                0x0100 //!< Update DMA request enable
#define TIM_DIER_BIE                0x0080 //!< Break interrupt enable
#define TIM_DIER_TIE                0x0040 //!< Trigger interrupt enable
#define TIM_DIER_COMIE              0x0020 //!< COM interrupt enable
#define TIM_DIER_CC4IE              0x0010 //!< Capture/Compare 4 interrupt enable
#define TIM_DIER_CC3IE              0x0008 //!< Capture/Compare 3 interrupt enable
#define TIM_DIER_CC2IE              0x0004 //!< Capture/Compare 2 interrupt enable
#define TIM_DIER_CC1IE              0x0002 //!< Capture/Compare 1 interrupt enable
#define TIM_DIER_UIE                0x0001 //!< Update interrupt enable
/** @} */

/** @name TIM_SR:       (tim Offset: 0x10) TIM status register				  */
/** @{ */
#define TIM_SR_CC6IF  				0x00020000 //!< Compare 6 interrupt flag
#define TIM_SR_CC5IF 				0x00010000 //!< Compare 5 interrupt flag
#define TIM_SR_SBIF 				0x00002000 //!< System Break interrupt flag
#define TIM_SR_CC4OF                0x00001000 //!< Capture/Compare 4 Overcapture Flag
#define TIM_SR_CC3OF                0x00000800 //!< Capture/Compare 3 Overcapture Flag
#define TIM_SR_CC2OF                0x00000400 //!< Capture/Compare 2 Overcapture Flag
#define TIM_SR_CC1OF                0x00000200 //!< Capture/Compare 1 Overcapture Flag
#define TIM_SR_BIF                  0x00000080 //!< Break interrupt Flag
#define TIM_SR_TIF                  0x00000040 //!< Trigger interrupt Flag
#define TIM_SR_COMIF                0x00000020 //!< COM interrupt Flag
#define TIM_SR_CC4IF                0x00000010 //!< Capture/Compare 4 interrupt Flag
#define TIM_SR_CC3IF                0x00000008 //!< Capture/Compare 3 interrupt Flag
#define TIM_SR_CC2IF                0x00000004 //!< Capture/Compare 2 interrupt Flag
#define TIM_SR_CC1IF                0x00000002 //!< Capture/Compare 1 interrupt Flag
#define TIM_SR_UIF                  0x00000001 //!< Update interrupt Flag
/** @} */

/** @name TIM_EGR:      (tim Offset: 0x14) TIM event generation register 	  */
/** @{ */
#define TIM_EGR_B2G 				0x0000 //!< Break 2 generation
#define TIM_EGR_BG                  0x0080 //!< Break Generation
#define TIM_EGR_TG                  0x0040 //!< Trigger Generation
#define TIM_EGR_COMG                0x0020 //!< Capture/Compare Control Update Generation
#define TIM_EGR_CC4G                0x0010 //!< Capture/Compare 4 Generation
#define TIM_EGR_CC3G                0x0008 //!< Capture/Compare 3 Generation
#define TIM_EGR_CC2G                0x0004 //!< Capture/Compare 2 Generation
#define TIM_EGR_CC1G                0x0002 //!< Capture/Compare 1 Generation
#define TIM_EGR_UG                  0x0001 //!< Update Generation
/** @} */

/** @name TIM_CCMR1:    (tim Offset: 0x18) TIM capture/compare mode register 1  */
/** @{ */


#define TIM_CCMR1_IC2F            	0x0000F000 //!< Input capture 2 filter
#define TIM_CCMR1_IC2PSC 			0x00000C00 //!< Input capture 2 prescaler
#define TIM_CCMR1_CC2S				0x00000300 //!< Capture/Compare 2 selection
#define TIM_CCMR1_IC1F 				0x000000F0 //!< Input capture 1 filter
#define TIM_CCMR1_IC1PSC 			0x0000000C //!< Input capture 1 prescaler
#define TIM_CCMR1_CC1S 				0x00000003 //!< Capture/Compare 1 Selection

/** @} */

/** @name TIM_CCMR2:    (tim Offset: 0x1C) TIM capture/compare mode register 2  */
/** @{ */
#define TIM_CCMR2_IC4F 				0x0000F000 //!< Input capture 4 filter
#define TIM_CCMR2_IC4PSC 			0x00000C00 //!< Input capture 4 prescaler
#define TIM_CCMR2_CC4S  			0x00000300 //!< Capture/Compare 4 selection
#define TIM_CCMR2_IC3F  			0x000000F0 //!< Input capture 3 filter
#define TIM_CCMR2_IC3PSC  			0x0000000C //!< Input capture 3 prescaler
#define TIM_CCMR2_CC3S  			0x00000003 //!< Capture/compare 3 selection

/** @} */

/** @name TIM_CCER:     (tim Offset: 0x20) TIM capture/compare enable register  */
/** @{ */
#define TIM_CCER_CC6P 				0x00200000 //!< Capture/Compare 6 output polarity
#define TIM_CCER_CC6E 				0x00200000 //!< Capture/Compare 6 output enable
#define TIM_CCER_CC5P 				0x00020000 //!< Capture/Compare 5 output polarity
#define TIM_CCER_CC5E 				0x00010000 //!< Capture/Compare 5 output enable
#define TIM_CCER_CC4NP              0x00008000 //!< Capture/Compare 4 Complementary output Polarity
#define TIM_CCER_CC4P               0x00002000 //!< Capture/Compare 4 output Polarity
#define TIM_CCER_CC4E               0x00001000 //!< Capture/Compare 4 output enable
#define TIM_CCER_CC3NP              0x00000800 //!< Capture/Compare 3 Complementary output Polarity
#define TIM_CCER_CC3NE              0x00000400 //!< Capture/Compare 3 Complementary output enable
#define TIM_CCER_CC3P               0x00000200 //!< Capture/Compare 3 output Polarity
#define TIM_CCER_CC3E               0x00000100 //!< Capture/Compare 3 output enable
#define TIM_CCER_CC2NP              0x00000080 //!< Capture/Compare 2 Complementary output Polarity
#define TIM_CCER_CC2NE              0x00000040 //!< Capture/Compare 2 Complementary output enable
#define TIM_CCER_CC2P               0x00000020 //!< Capture/Compare 2 output Polarity
#define TIM_CCER_CC2E               0x00000010 //!< Capture/Compare 2 output enable
#define TIM_CCER_CC1NP              0x00000008 //!< Capture/Compare 1 Complementary output Polarity
#define TIM_CCER_CC1NE              0x00000004 //!< Capture/Compare 1 Complementary output enable
#define TIM_CCER_CC1P               0x00000002 //!< Capture/Compare 1 output Polarity
#define TIM_CCER_CC1E               0x00000001 //!< Capture/Compare 1 output enable
/** @} */

/** @name TIM_CNT:      (tim Offset: 0x24) TIM counter register				  */
/** @{ */
#define TIM_CNT_UIFCPY 				0x80000000 //!< UIF copy
#define TIM_CNT_CNT                 0x0000FFFF //!< Counter Value
/** @} */

/** @name TIM_PSC:      (tim Offset: 0x28) TIM prescaler					  */
/** @{ */
#define TIM_PSC_PSC                 0xFFFF //!< Prescaler Value
/** @} */

/** @name TIM_ARR:      (tim Offset: 0x2C) TIM auto-reload register			  */
/** @{ */
#define TIM_ARR_ARR                 0xFFFF //!< actual auto-reload Value
/** @} */

/** @name TIM_RCR:      (tim Offset: 0x30) TIM repetition counter register 	  */
/** @{ */
#define TIM_RCR_REP                 0x00FF //!< Repetition Counter Value
/** @} */

/** @name TIM_CCR1:     (tim Offset: 0x34) TIM capture/compare register 1	  */
/** @{ */
#define TIM_CCR1_CCR1               0xFFFF //!< Capture/Compare 1 Value
/** @} */

/** @name TIM_CCR2:     (tim Offset: 0x38) TIM capture/compare register 2	  */
/** @{ */
#define TIM_CCR2_CCR2               0xFFFF //!< Capture/Compare 2 Value
/** @} */

/** @name TIM_CCR3:     (tim Offset: 0x3C) TIM capture/compare register 3	  */
/** @{ */
#define TIM_CCR3_CCR3               0xFFFF //!< Capture/Compare 3 Value
/** @} */

/** @name TIM_CCR4:     (tim Offset: 0x40) TIM capture/compare register 4	  */
/** @{ */
#define TIM_CCR4_CCR4               0xFFFF //!< Capture/Compare 4 Value
/** @} */

/** @name TIM_BDTR:     (tim Offset: 0x44) TIM break and dead-time register	  */
/** @{ */
#define TIM_BDTR_BK2P 				0x02000000 //!< Break 2 polarity
#define TIM_BDTR_BK2E 				0x01000000 //!< Break 2 enable
#define TIM_BDTR_BK2F 				0x00F00000 //!< Break 2 filter
#define TIM_BDTR_BKF 				0x000F0000 //!< Break filter
#define TIM_BDTR_MOE                0x00008000 //!< Main Output enable
#define TIM_BDTR_AOE                0x00004000 //!< Automatic Output enable
#define TIM_BDTR_BKP                0x00002000 //!< Break Polarity
#define TIM_BDTR_BKE                0x00001000 //!< Break enable
#define TIM_BDTR_OSSR               0x00000800 //!< Off-State Selection for Run mode
#define TIM_BDTR_OSSI               0x00000400 //!< Off-State Selection for Idle mode
#define TIM_BDTR_LOCK               0x00000300 //!< LOCK[1:0] bits (Lock Configuration)
#define TIM_BDTR_DTG                0x000000FF //!< DTG[0:7] bits (Dead-Time Generator set-up)
/** @} */

/** @name TIM_DCR:      (tim Offset: 0x48) TIM DMA control register			  */
/** @{ */
#define TIM_DCR_DBL                 0x1F00 //!< DBL[4:0] bits (DMA Burst Length)
#define TIM_DCR_DBA                 0x001F //!< DBA[4:0] bits (DMA Base Address)
/** @} */

/** @name TIM_DMAR:     (tim Offset: 0x4C) TIM DMA address for full transfer  */
/** @{ */
#define TIM_DMAR_DMAB 				0xFFFFFFFF //!< DMA register for burst accesses
/** @} */

/** @name TIM_CCMR3:    (tim Offset: 0x54) TIM capture/compare mode register 3 */
/** @{ */
#define TIM_CCMR3_OC6M 				0x01007000 //!< Output compare 6 mode
#define TIM_CCMR3_OC5M 				0x00010070 //!< Output compare 5 mode
#define TIM_CCMR3_OC6CE 			0x00008000 //!< Output compare 6 clear enable
#define TIM_CCMR3_OC6PE 			0x00000800 //!< Output compare 6 preload enable
#define TIM_CCMR3_OC6FE 			0x00000400 //!< Output compare 6 fast enable
#define TIM_CCMR3_OC5CE 			0x00000080 //!< Output compare 5 clear enable
#define TIM_CCMR3_OC5PE 			0x00000008 //!< Output compare 5 preload enable
#define TIM_CCMR3_OC5FE 			0x00000004 //!< Output compare 5 fast enable
/** @} */

/** @name TIM_CCR5:     (tim Offset: 0x58) TIM capture/compare register5	  */
/** @{ */
#define TIM_CCR5_GC5C3 				0x80000000 //!< Group Channel 5 and Channel 3
#define TIM_CCR5_GC5C2 				0x40000000 //!< Group Channel 5 and Channel 2
#define TIM_CCR5_GC5C1 				0x20000000 //!< Group Channel 5 and Channel 1
#define TIM_CCR5_CCR5  				0x0000FFFF //!< Capture/Compare 5 value
/** @} */

/** @name TIM_CCR6:     (tim Offset: 0x5C) TIM capture/compare register6      */
/** @{ */
#define TIM_CCR6_CCR6  				0x0000 //!< Capture/Compare 6 value
/** @} */

/** @name TIM_AF1:      (tim Offset: 0x60) TIM alternate function option register 1 */
/** @{ */
#define TIM_AF1_ETRSEL 				0x0003C000 //!< ETR source selection
#define TIM_AF1_BKCMP2P 			0x00000800 //!< BRK COMP2 input polarity
#define TIM_AF1_BKCMP1P 			0x00000400 //!< BRK COMP1 input polarity
#define TIM_AF1_BKINP 				0x00000200 //!< BRK BKIN input polarity
#define TIM_AF1_BKDF1BK0E 			0x00000100 //!< BRK dfsdm1_break[0] enable
#define TIM_AF1_BKCMP2E 			0x00000004 //!< BRK COMP2 enable
#define TIM_AF1_BKCMP1E 			0x00000002 //!< BRK COMP1 enable
#define TIM_AF1_BKINE 				0x00000001 //!< BRK BKIN input enable
/** @} */

/** @name TIM_AF2:      (tim Offset: 0x64) TIM alternate function option register 2 */
/** @{ */
#define TIM_AF2_BK2CMP2P 			0x00000800 //!< BRK2 COMP2 input polarity
#define TIM_AF2_BK2CMP1P 			0x00000400 //!< BRK2 COMP1 input polarity
#define TIM_AF2_BK2INP 				0x00000200 //!< BRK2 BKIN2 input polarity
#define TIM_AF2_BK2DF1BK1E 			0x00000100 //!< BRK2 dfsdm1_break[1] enable
#define TIM_AF2_BK2CMP2E 			0x00000004 //!< BRK2 COMP2 enable
#define TIM_AF2_BK2CMP1E 			0x00000002 //!< BRK2 COMP1 enable
#define TIM_AF2_BK2INE 				0x00000001 //!< BRK2 BKIN input enable
/** @} */

/** @name TIM_TISEL:    (tim Offset: 0x68) TIM Input Selection register		  */
/** @{ */
#define TIM_TISEL_TI4SEL 			0x0F000000 //!< selects TI4[0] to TI4[15] input
#define TIM_TISEL_TI3SEL 			0x000F0000 //!< selects TI3[0] to TI3[15] input
#define TIM_TISEL_TI2SEL 			0x00000F00 //!< selects TI2[0] to TI2[15] input
#define TIM_TISEL_TI1SEL 			0x0000000F //!< selects TI1[0] to TI1[15] input
/** @} */


// The clock of the involved Timer counter is stopped when the core is halted
#define DebugFreezeTimer(periph_id) 						\
{															\
	__IO uint32_t* reg =nullptr;							\
	switch((periph_id)>>5)									\
	{														\
		case 5:												\
			reg = &DBGMCU->DBGMCU_APB1LFZ1;					\
			break;											\
		case 7:												\
			reg = &DBGMCU->DBGMCU_APB2FZ1;					\
			break;											\
		case 8:												\
			reg = &DBGMCU->DBGMCU_APB4FZ1;					\
			break;											\
	}														\
    if(reg)													\
		reg[0] |= 1<<((periph_id)&0x1f);					\
}



/** @} */ // @relates TIM_TypeDef



#endif /* TIM_H7_H_ */

/** @} */
