/*
 * dac_f0.h
 *
 *  Created on: Sep 10, 2015
 *      Author: miro
 */

#ifndef HARDWARE_STM32_STM32F0_DAC_F0_H_
#define HARDWARE_STM32_STM32F0_DAC_F0_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Digital-to-analog converter (DAC) (STM32F05xxx series)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DAC_CR;      	//!< (dac Offset: 0x00) DAC control register
  __IO uint32_t DAC_SWTRIGR; 	//!< (dac Offset: 0x04) DAC software trigger register
  __IO uint32_t DAC_DHR12R1; 	//!< (dac Offset: 0x08) DAC channel1 12-bit right-aligned data holding register
  __IO uint32_t DAC_DHR12L1; 	//!< (dac Offset: 0x0C) DAC channel1 12-bit left aligned data holding register
  __IO uint32_t DAC_DHR8R1;  	//!< (dac Offset: 0x10) DAC channel1 8-bit right aligned data holding register
  __IO uint32_t reserved1[6]; 	//!< (dac Offset: 0x14)
  __IO uint32_t DAC_DOR1;    	//!< (dac Offset: 0x2C) DAC channel1 data output register
  __IO uint32_t reserved2;    	//!< (dac Offset: 0x30)
  __IO uint32_t DAC_SR;      	//!< (dac Offset: 0x34) DAC status register
} DAC_TypeDef;

/*******************************************************************************
 * @defgroup DAC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup DAC_CR:      	(dac Offset: 0x00) DAC control register             */
#define DAC_CR_DMAUDRIE1            0x00002000 //!< DAC channel1 DMA Underrun Interrupt enable
#define DAC_CR_DMAEN1               0x00001000 //!< DAC channel1 DMA enable
#define DAC_CR_TSEL1                0x00000038 //!< TSEL1[2:0] (DAC channel1 Trigger selection)
#define DAC_CR_TSEL1_TIM6           0x00000000 //!<  Timer 6 TRGO event
#define DAC_CR_TSEL1_TIM8           0x00000008 //!<  Timer 8 TRGO event
#define DAC_CR_TSEL1_TIM7           0x00000010 //!<  Timer 7 TRGO event
#define DAC_CR_TSEL1_TIM5           0x00000018 //!<  Timer 5 TRGO event
#define DAC_CR_TSEL1_TIM2           0x00000020 //!<  Timer 2 TRGO event
#define DAC_CR_TSEL1_TIM4           0x00000028 //!<  Timer 4 TRGO event
#define DAC_CR_TSEL1_EXTI9          0x00000030 //!<  External line9
#define DAC_CR_TSEL1_SW	            0x00000038 //!<  Software trigger
#define DAC_CR_TEN1                 0x00000004 //!< DAC channel1 Trigger enable
#define DAC_CR_BOFF1                0x00000002 //!< DAC channel1 output buffer disable
#define DAC_CR_EN1                  0x00000001 //!< DAC channel1 enable
/** @} */

/** @defgroup DAC_SWTRIGR: 	(dac Offset: 0x04) DAC software trigger register    */
#define DAC_SWTRIGR_SWTRIG1         0x00000001 //!< DAC channel1 software trigger
/** @} */

/** @defgroup DAC_DHR12R1: 	(dac Offset: 0x08) DAC channel1 12-bit right-aligned data holding register */
#define DAC_DHR12R1_DACC1DHR        0x00000FFF //!< DAC channel1 12-bit Right aligned data
/** @} */

/** @defgroup DAC_DHR12L1: 	(dac Offset: 0x0C) DAC channel1 12-bit left aligned data holding register  */
#define DAC_DHR12L1_DACC1DHR        0x0000FFF0 //!< DAC channel1 12-bit Left aligned data *
/** @} */

/** @defgroup DAC_DHR8R1:  	(dac Offset: 0x10) DAC channel1 8-bit right aligned data holding register  */
#define DAC_DHR8R1_DACC1DHR         0x000000FF //!< DAC channel1 8-bit Right aligned data
/** @} */

/** @defgroup DAC_DOR1:    	(dac Offset: 0x2C) DAC channel1 data output register*/
#define DAC_DOR1_DACC1DOR           0x00000FFF //!< DAC channel1 data output
/** @} */

/** @defgroup DAC_SR:      	(dac Offset: 0x34) DAC status register              */
#define DAC_SR_DMAUDR1              0x00002000 //!< DAC channel1 DMA underrun flag
/** @} */




#endif /* HARDWARE_STM32_STM32F0_DAC_F0_H_ */
