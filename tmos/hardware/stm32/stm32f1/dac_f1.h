/*
 * dac_f1.h
 *
 *  Created on: Sep 10, 2015
 *      Author: miro
 */

#ifndef HARDWARE_STM32_STM32F1_DAC_F1_H_
#define HARDWARE_STM32_STM32F1_DAC_F1_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Digital-to-analog converter (DAC)
 ******************************************************************************/
typedef struct
{
  __IO uint32_t DAC_CR;      	//!< (dac Offset: 0x00) DAC control register
  __IO uint32_t DAC_SWTRIGR; 	//!< (dac Offset: 0x04) DAC software trigger register
  __IO uint32_t DAC_DHR12R1; 	//!< (dac Offset: 0x08) DAC channel1 12-bit right-aligned data holding register
  __IO uint32_t DAC_DHR12L1; 	//!< (dac Offset: 0x0C) DAC channel1 12-bit left aligned data holding register
  __IO uint32_t DAC_DHR8R1;  	//!< (dac Offset: 0x10) DAC channel1 8-bit right aligned data holding register
  __IO uint32_t DAC_DHR12R2; 	//!< (dac Offset: 0x14) DAC channel2 12-bit right aligned data holding register
  __IO uint32_t DAC_DHR12L2; 	//!< (dac Offset: 0x18) DAC channel2 12-bit left aligned data holding register
  __IO uint32_t DAC_DHR8R2;  	//!< (dac Offset: 0x1C) DAC channel2 8-bit right-aligned data holding register
  __IO uint32_t DAC_DHR12RD; 	//!< (dac Offset: 0x20) Dual DAC 12-bit right-aligned data holding register
  __IO uint32_t DAC_DHR12LD; 	//!< (dac Offset: 0x24) DUAL DAC 12-bit left aligned data holding register
  __IO uint32_t DAC_DHR8RD;  	//!< (dac Offset: 0x28) DUAL DAC 8-bit right aligned data holding register
  __IO uint32_t DAC_DOR1;    	//!< (dac Offset: 0x2C) DAC channel1 data output register
  __IO uint32_t DAC_DOR2;    	//!< (dac Offset: 0x30) DAC channel2 data output register
} DAC_TypeDef;

#define USE_DAC_INTERRUPRT 0	//!< Disable Interrupt (not available in F1 series)

/*******************************************************************************
 * @defgroup DAC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup DAC_CR:      	(dac Offset: 0x00) DAC control register             */
#define DAC_CR_DMAEN2               0x10000000 //!< DAC channel2 DMA enabled
#define DAC_CR_MAMP2                0x0F000000 //!< MAMP2[3:0] (DAC channel2 Mask/Amplitude selector)
#define DAC_CR_WAVE2                0x00C00000 //!< WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable)
#define DAC_CR_TSEL2                0x00380000 //!< TSEL2[2:0] (DAC channel2 Trigger selection)
#define DAC_CR_TEN2                 0x00040000 //!< DAC channel2 Trigger enable
#define DAC_CR_BOFF2                0x00020000 //!< DAC channel2 output buffer disable
#define DAC_CR_EN2                  0x00010000 //!< DAC channel2 enable

#define DAC_CR_DMAEN1               0x00001000 //!< DAC channel1 DMA enable
#define DAC_CR_MAMP1                0x00000F00 //!< MAMP1[3:0] (DAC channel1 Mask/Amplitude selector)
#define DAC_CR_WAVE1                0x000000C0 //!< WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable)
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
#define DAC_SWTRIGR_SWTRIG2         0x00000002 //!< DAC channel2 software trigger
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

/** @defgroup DAC_DHR12R2: 	(dac Offset: 0x14) DAC channel2 12-bit right aligned data holding register */
#define DAC_DHR12R2_DACC2DHR        0x00000FFF //!< DAC channel2 12-bit Right aligned data
/** @} */

/** @defgroup DAC_DHR12L2: 	(dac Offset: 0x18) DAC channel2 12-bit left aligned data holding register  */
#define DAC_DHR12L2_DACC2DHR        0x0000FFF0 //!< DAC channel2 12-bit Left aligned data
/** @} */

/** @defgroup DAC_DHR8R2:  	(dac Offset: 0x1C) DAC channel2 8-bit right-aligned data holding register  */
#define DAC_DHR8R2_DACC2DHR         0x000000FF //!< DAC channel2 8-bit Right aligned data
/** @} */

/** @defgroup DAC_DHR12RD: 	(dac Offset: 0x20) Dual DAC 12-bit right-aligned data holding register     */
#define DAC_DHR12RD_DACC1DHR        0x00000FFF //!< DAC channel1 12-bit Right aligned data
#define DAC_DHR12RD_DACC2DHR        0x0FFF0000 //!< DAC channel2 12-bit Right aligned data
/** @} */

/** @defgroup DAC_DHR12LD: 	(dac Offset: 0x24) DUAL DAC 12-bit left aligned data holding register      */
#define DAC_DHR12LD_DACC1DHR        0x0000FFF0 //!< DAC channel1 12-bit Left aligned data
#define DAC_DHR12LD_DACC2DHR        0xFFF00000 //!< DAC channel2 12-bit Left aligned data
/** @} */

/** @defgroup DAC_DHR8RD:  	(dac Offset: 0x28) DUAL DAC 8-bit right aligned data holding register      */
#define DAC_DHR8RD_DACC1DHR         0x000000FF //!< DAC channel1 8-bit Right aligned data
#define DAC_DHR8RD_DACC2DHR         0x0000FF00 //!< DAC channel2 8-bit Right aligned data
/** @} */

/** @defgroup DAC_DOR1:    	(dac Offset: 0x2C) DAC channel1 data output register*/
#define DAC_DOR1_DACC1DOR           0x00000FFF //!< DAC channel1 data output
/** @} */

/** @defgroup DAC_DOR2:    	(dac Offset: 0x30) DAC channel2 data output register*/
#define DAC_DOR2_DACC2DOR           0x00000FFF //!< DAC channel2 data output
/** @} */

#endif /* HARDWARE_STM32_STM32F1_DAC_F1_H_ */
