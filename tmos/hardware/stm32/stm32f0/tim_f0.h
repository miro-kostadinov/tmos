/**************************************************************************//**
 * @ingroup	hardware_stm32f0_tim
 * @file	hardware/stm32/stm32f0/tim_f0.h
 * @brief  	STM32F0 Timers
 * @date    28. October 2014
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f0_tim  STM32 F0 family timers
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32 F0 family timers
 * @{
 *
 ******************************************************************************/

#ifndef TIM_F0_H_
#define TIM_F0_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  STM32 F0 family timers
 ******************************************************************************
 *
 *	Timer channels	BIF	TIF	COMIF	BITS
 *	  1		4		 *	 *	  *		 16		 - Advanced
 *	  3		4			 *			 16		 - General-purpose 16bit
 *	  6		0						 16		 - Basic
 *	 14		1						 16		 - 1-channel
 *	 15		1						 16		 - 1-channel with one complementary output
 *	 16		2						 16		 - 2-channel with one complementary output
 *	 17		2						 16		 - 2-channel with one complementary output
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
  __IO uint32_t TIM_CCRx[4];   //!< (tim Offset: 0x34) TIM capture/compare register 1-4
  __IO uint32_t TIM_BDTR;      //!< (tim Offset: 0x44) TIM break and dead-time register
  __IO uint32_t TIM_DCR;       //!< (tim Offset: 0x48) TIM DMA control register
  __IO uint32_t TIM_DMAR;      //!< (tim Offset: 0x4C) TIM DMA address for full transfer
} TIM_TypeDef;

/*******************************************************************************
 * @relates TIM_TypeDef
 * @{
 ******************************************************************************/

/** @name TIM_CR1:      (tim Offset: 0x00) TIM control register 1			  */
/** @{ */
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
#define TIM_CR2_OIS4                0x4000 //!< Output Idle state 4 (OC4 output)
#define TIM_CR2_OIS3N               0x2000 //!< Output Idle state 3 (OC3N output)
#define TIM_CR2_OIS3                0x1000 //!< Output Idle state 3 (OC3 output)
#define TIM_CR2_OIS2N               0x0800 //!< Output Idle state 2 (OC2N output)
#define TIM_CR2_OIS2                0x0400 //!< Output Idle state 2 (OC2 output)
#define TIM_CR2_OIS1N               0x0200 //!< Output Idle state 1 (OC1N output)
#define TIM_CR2_OIS1                0x0100 //!< Output Idle state 1 (OC1 output)
#define TIM_CR2_TI1S                0x0080 //!< TI1 Selection
#define TIM_CR2_MMS                 0x0070 //!< MMS[2:0] bits (Master Mode Selection)
#define TIM_CR2_CCDS                0x0008 //!< Capture/Compare DMA Selection
#define TIM_CR2_CCUS                0x0004 //!< Capture/Compare Control Update Selection
#define TIM_CR2_CCPC                0x0001 //!< Capture/Compare Preloaded Control
/** @} */

/** @name TIM_SMCR:     (tim Offset: 0x08) TIM slave mode control register	  */
/** @{ */
#define TIM_SMCR_ETP                0x8000 //!< External trigger polarity
#define TIM_SMCR_ECE                0x4000 //!< External clock enable
#define TIM_SMCR_ETPS               0x3000 //!< ETPS[1:0] bits (External trigger prescaler)
#define TIM_SMCR_ETF                0x0F00 //!< ETF[3:0] bits (External trigger filter)
#define TIM_SMCR_MSM                0x0080 //!< Master/slave mode
#define TIM_SMCR_TS                 0x0070 //!< TS[2:0] bits (Trigger selection)
#define TIM_SMCR_SMS                0x0007 //!< SMS[2:0] bits (Slave mode selection)
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
#define TIM_SR_CC4OF                0x1000 //!< Capture/Compare 4 Overcapture Flag
#define TIM_SR_CC3OF                0x0800 //!< Capture/Compare 3 Overcapture Flag
#define TIM_SR_CC2OF                0x0400 //!< Capture/Compare 2 Overcapture Flag
#define TIM_SR_CC1OF                0x0200 //!< Capture/Compare 1 Overcapture Flag
#define TIM_SR_BIF                  0x0080 //!< Break interrupt Flag
#define TIM_SR_TIF                  0x0040 //!< Trigger interrupt Flag
#define TIM_SR_COMIF                0x0020 //!< COM interrupt Flag
#define TIM_SR_CC4IF                0x0010 //!< Capture/Compare 4 interrupt Flag
#define TIM_SR_CC3IF                0x0008 //!< Capture/Compare 3 interrupt Flag
#define TIM_SR_CC2IF                0x0004 //!< Capture/Compare 2 interrupt Flag
#define TIM_SR_CC1IF                0x0002 //!< Capture/Compare 1 interrupt Flag
#define TIM_SR_UIF                  0x0001 //!< Update interrupt Flag
/** @} */

/** @name TIM_EGR:      (tim Offset: 0x14) TIM event generation register	  */
/** @{ */
#define TIM_EGR_BG                  0x0080 //!< Break Generation
#define TIM_EGR_TG                  0x0040 //!< Trigger Generation
#define TIM_EGR_COMG                0x0020 //!< Capture/Compare Control Update Generation
#define TIM_EGR_CC4G                0x0010 //!< Capture/Compare 4 Generation
#define TIM_EGR_CC3G                0x0008 //!< Capture/Compare 3 Generation
#define TIM_EGR_CC2G                0x0004 //!< Capture/Compare 2 Generation
#define TIM_EGR_CC1G                0x0002 //!< Capture/Compare 1 Generation
#define TIM_EGR_UG                  0x0001 //!< Update Generation
/** @} */

#define TIM_CCMRx_OCxCE             0x0080 //!< Output Compare 1Clear Enable
#define TIM_CCMRx_OCxM              0x0070 //!< OC1M[2:0] bits (Output Compare 1 Mode)
#define  TIM_CCMRx_OCxM_000		    0x0000 //!<  The comparison between the output compare register TIMx_CCRx and the counter TIMx_CNT has no effect on the outputs
#define  TIM_CCMRx_OCxM_001		    0x0010 //!<  Set channel 1 to active level on match
#define  TIM_CCMRx_OCxM_010		    0x0020 //!<  Set channel 1 to inactive level on match
#define  TIM_CCMRx_OCxM_011		    0x0030 //!<  Toggle
#define  TIM_CCMRx_OCxM_100		    0x0040 //!<  Force inactive level - OC1REF is forced low
#define  TIM_CCMRx_OCxM_101		    0x0050 //!<  Force active level - OC1REF is forced high.
#define  TIM_CCMRx_OCxM_110		    0x0060 //!<  PWM mode 1
#define  TIM_CCMRx_OCxM_111		    0x0070 //!<  PWM mode 2


/** @name TIM_CCMR1:    (tim Offset: 0x18) TIM capture/compare mode register 1  */
/** @{ */
#define TIM_CCMR1_OC2CE             0x8000 //!< Output Compare 2 Clear Enable
#define TIM_CCMR1_OC2M              0x7000 //!< OC2M[2:0] bits (Output Compare 2 Mode)
#define TIM_CCMR1_OC2PE             0x0800 //!< Output Compare 2 Preload enable
#define TIM_CCMR1_OC2FE             0x0400 //!< Output Compare 2 Fast enable
#define TIM_CCMR1_CC2S              0x0300 //!< CC2S[1:0] bits (Capture/Compare 2 Selection)
#define TIM_CCMR1_OC1CE             0x0080 //!< Output Compare 1Clear Enable
#define TIM_CCMR1_OC1M              0x0070 //!< OC1M[2:0] bits (Output Compare 1 Mode)
#define TIM_CCMR1_OC1PE             0x0008 //!< Output Compare 1 Preload enable
#define TIM_CCMR1_OC1FE             0x0004 //!< Output Compare 1 Fast enable
#define TIM_CCMR1_CC1S              0x0003 //!< CC1S[1:0] bits (Capture/Compare 1 Selection)
/*----------------------------------------------------------------------------*/
#define TIM_CCMR1_IC2F              0xF000 //!< IC2F[3:0] bits (Input Capture 2 Filter)
#define TIM_CCMR1_IC2PSC            0x0C00 //!< IC2PSC[1:0] bits (Input Capture 2 Prescaler)
#define TIM_CCMR1_IC1F              0x00F0 //!< IC1F[3:0] bits (Input Capture 1 Filter)
#define TIM_CCMR1_IC1PSC            0x000C //!< IC1PSC[1:0] bits (Input Capture 1 Prescaler)
/** @} */

/** @name TIM_CCMR2:    (tim Offset: 0x1C) TIM capture/compare mode register 2  */
/** @{ */
#define TIM_CCMR2_OC4CE             0x8000 //!< Output Compare 4 Clear Enable
#define TIM_CCMR2_OC4M              0x7000 //!< OC4M[2:0] bits (Output Compare 4 Mode)
#define TIM_CCMR2_OC4PE             0x0800 //!< Output Compare 4 Preload enable
#define TIM_CCMR2_OC4FE             0x0400 //!< Output Compare 4 Fast enable
#define TIM_CCMR2_CC4S              0x0300 //!< CC4S[1:0] bits (Capture/Compare 4 Selection)
#define TIM_CCMR2_OC3CE             0x0080 //!< Output Compare 3 Clear Enable
#define TIM_CCMR2_OC3M              0x0070 //!< OC3M[2:0] bits (Output Compare 3 Mode)
#define TIM_CCMR2_OC3PE             0x0008 //!< Output Compare 3 Preload enable
#define TIM_CCMR2_OC3FE             0x0004 //!< Output Compare 3 Fast enable
#define TIM_CCMR2_CC3S              0x0003 //!< CC3S[1:0] bits (Capture/Compare 3 Selection)
/*----------------------------------------------------------------------------*/
#define TIM_CCMR2_IC4F              0xF000 //!< IC4F[3:0] bits (Input Capture 4 Filter)
#define TIM_CCMR2_IC4PSC            0x0C00 //!< IC4PSC[1:0] bits (Input Capture 4 Prescaler)
#define TIM_CCMR2_IC3F              0x00F0 //!< IC3F[3:0] bits (Input Capture 3 Filter)
#define TIM_CCMR2_IC3PSC            0x000C //!< IC3PSC[1:0] bits (Input Capture 3 Prescaler)
/** @} */

/** @name TIM_CCER:     (tim Offset: 0x20) TIM capture/compare enable register  */
/** @{ */
#define TIM_CCER_CC4NP              0x8000 //!< Capture/Compare 4 Complementary output Polarity
#define TIM_CCER_CC4P               0x2000 //!< Capture/Compare 4 output Polarity
#define TIM_CCER_CC4E               0x1000 //!< Capture/Compare 4 output enable
#define TIM_CCER_CC3NP              0x0800 //!< Capture/Compare 3 Complementary output Polarity
#define TIM_CCER_CC3NE              0x0400 //!< Capture/Compare 3 Complementary output enable
#define TIM_CCER_CC3P               0x0200 //!< Capture/Compare 3 output Polarity
#define TIM_CCER_CC3E               0x0100 //!< Capture/Compare 3 output enable
#define TIM_CCER_CC2NP              0x0080 //!< Capture/Compare 2 Complementary output Polarity
#define TIM_CCER_CC2NE              0x0040 //!< Capture/Compare 2 Complementary output enable
#define TIM_CCER_CC2P               0x0020 //!< Capture/Compare 2 output Polarity
#define TIM_CCER_CC2E               0x0010 //!< Capture/Compare 2 output enable
#define TIM_CCER_CC1NP              0x0008 //!< Capture/Compare 1 Complementary output Polarity
#define TIM_CCER_CC1NE              0x0004 //!< Capture/Compare 1 Complementary output enable
#define TIM_CCER_CC1P               0x0002 //!< Capture/Compare 1 output Polarity
#define TIM_CCER_CC1E               0x0001 //!< Capture/Compare 1 output enable

#define TIM_CCER_CCxNP              0x0008 //!< Capture/Compare X Complementary output Polarity
#define TIM_CCER_CCxNE              0x0004 //!< Capture/Compare X Complementary output enable
#define TIM_CCER_CCxP               0x0002 //!< Capture/Compare X output Polarity
#define TIM_CCER_CCxE               0x0001 //!< Capture/Compare X output enable
/** @} */

/** @name TIM_CNT:      (tim Offset: 0x24) TIM counter register				  */
/** @{ */
#define TIM_CNT_CNT                 0xFFFF //!< Counter Value
/** @} */

/** @name TIM_PSC:      (tim Offset: 0x28) TIM prescaler					  */
/** @{ */
#define TIM_PSC_PSC                 0xFFFF //!< Prescaler Value
/** @} */

/** @name TIM_ARR:      (tim Offset: 0x2C) TIM auto-reload register			  */
/** @{ */
#define TIM_ARR_ARR                 0xFFFF //!< actual auto-reload Value
/** @} */

/** @name TIM_RCR:      (tim Offset: 0x30) TIM repetition counter register	  */
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
#define TIM_BDTR_MOE                0x8000 //!< Main Output enable
#define TIM_BDTR_AOE                0x4000 //!< Automatic Output enable
#define TIM_BDTR_BKP                0x2000 //!< Break Polarity
#define TIM_BDTR_BKE                0x1000 //!< Break enable
#define TIM_BDTR_OSSR               0x0800 //!< Off-State Selection for Run mode
#define TIM_BDTR_OSSI               0x0400 //!< Off-State Selection for Idle mode
#define TIM_BDTR_LOCK               0x0300 //!< LOCK[1:0] bits (Lock Configuration)
#define TIM_BDTR_DTG                0x00FF //!< DTG[0:7] bits (Dead-Time Generator set-up)
/** @} */

/** @name TIM_DCR:      (tim Offset: 0x48) TIM DMA control register			  */
/** @{ */
#define TIM_DCR_DBL                 0x1F00 //!< DBL[4:0] bits (DMA Burst Length)
#define TIM_DCR_DBA                 0x001F //!< DBA[4:0] bits (DMA Base Address)
/** @} */

/** @name TIM_DMAR:     (tim Offset: 0x4C) TIM DMA address for full transfer  */
/** @{ */
#define TIM_DMAR_DMAB               0xFFFF //!< DMA register for burst accesses
/** @} */


/// The clock of the involved Timer counter is stopped when the core is halted
#define DebugFreezeTimer(periph_id) 						\
{															\
	__IO uint32_t* reg;										\
    if(periph_id < ID_NO_PERIPH)							\
    {														\
		reg = &DBGMCU->DBGMCU_APB1_FZ;						\
		reg[((periph_id)>>5)-4] |= 1<<((periph_id)&0x1f);	\
    }														\
}



/** @} */ // @relates TIM_TypeDef






#endif /* TIM_F0_H_ */

/** @} */
