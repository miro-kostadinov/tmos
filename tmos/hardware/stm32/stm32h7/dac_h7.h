/**************************************************************************//**
 * @ingroup	hardware_stm32h7_dac
 * @file	hardware/stm32/stm32h7/dac_h7.h
 * @brief  	STM32H7 DAC
 * @date    5. December 2019
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_dac  Digital-to-analog converter (DAC)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Digital-to-analog converter (DAC)
 * @{
 *
 ******************************************************************************/

#ifndef HARDWARE_STM32_STM32H7_DAC_H7_H_
#define HARDWARE_STM32_STM32H7_DAC_H7_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
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
  __IO uint32_t DAC_SR;      	//!< (dac Offset: 0x34) DAC status register
  __IO uint32_t DAC_CCR;        //!< (dac Offset: 0x38) DAC calibration control register
  __IO uint32_t DAC_MCR;        //!< (dac Offset: 0x3C) DAC mode control register
  __IO uint32_t DAC_SHSR1;      //!< (dac Offset: 0x40) DAC Sample and Hold sample time register 1
  __IO uint32_t DAC_SHSR2;      //!< (dac Offset: 0x44) DAC Sample and Hold sample time register 2
  __IO uint32_t DAC_SHHR;       //!< (dac Offset: 0x48) DAC Sample and Hold hold time register
  __IO uint32_t DAC_SHRR;       //!< (dac Offset: 0x4C) DAC Sample and Hold refresh time register
} DAC_TypeDef;

/***************************************************************************//**
 * @relates DAC_TypeDef
 * @{
 ******************************************************************************/

/** @name DAC_CR:      	(dac Offset: 0x00) DAC control register    	           */
/** @{ */
#define DAC_CR_CEN2		            0x40000000 //!< DAC channel2 calibration enable
#define DAC_CR_DMAUDRIE2            0x20000000 //!< DAC channel2 DMA underrun interrupt enable
#define DAC_CR_DMAEN2               0x10000000 //!< DAC channel2 DMA enabled
#define DAC_CR_MAMP2                0x0F000000 //!< MAMP2[3:0] (DAC channel2 Mask/Amplitude selector)
#define DAC_CR_WAVE2                0x00C00000 //!< WAVE2[1:0] (DAC channel2 noise/triangle wave generation enable)
#define DAC_CR_TSEL2                0x003C0000 //!< TSEL2[3:0] (DAC channel2 Trigger selection)
#define DAC_CR_TEN2                 0x00020000 //!< DAC channel2 Trigger enable
#define DAC_CR_EN2                  0x00010000 //!< DAC channel2 enable

#define DAC_CR_CEN1		            0x00004000 //!< DAC channel1 calibration enable
#define DAC_CR_DMAUDRIE1            0x00002000 //!< DAC channel1 DMA Underrun Interrupt enable
#define DAC_CR_DMAEN1               0x00001000 //!< DAC channel1 DMA enable
#define DAC_CR_MAMP1                0x00000F00 //!< MAMP1[3:0] (DAC channel1 Mask/Amplitude selector)
#define DAC_CR_WAVE1                0x000000C0 //!< WAVE1[1:0] (DAC channel1 noise/triangle wave generation enable)
#define DAC_CR_TSEL1                0x0000003C //!< TSEL1[3:0] (DAC channel1 Trigger selection)
#define DAC_CR_TEN1                 0x00000002 //!< DAC channel1 Trigger enable
#define DAC_CR_EN1                  0x00000001 //!< DAC channel1 enable
/** @} */

/** @name DAC_SWTRIGR: 	(dac Offset: 0x04) DAC software trigger register 	   */
/** @{ */
#define DAC_SWTRIGR_SWTRIG1         0x00000001 //!< DAC channel1 software trigger
#define DAC_SWTRIGR_SWTRIG2         0x00000002 //!< DAC channel2 software trigger
/** @} */

/** @name DAC_DHR12R1: 	(dac Offset: 0x08) DAC channel1 12-bit right-aligned data holding register */
/** @{ */
#define DAC_DHR12R1_DACC1DHR        0x00000FFF //!< DAC channel1 12-bit Right aligned data
/** @} */

/** @name DAC_DHR12L1: 	(dac Offset: 0x0C) DAC channel1 12-bit left aligned data holding register  */
/** @{ */
#define DAC_DHR12L1_DACC1DHR        0x0000FFF0 //!< DAC channel1 12-bit Left aligned data *
/** @} */

/** @name DAC_DHR8R1:  	(dac Offset: 0x10) DAC channel1 8-bit right aligned data holding register  */
/** @{ */
#define DAC_DHR8R1_DACC1DHR         0x000000FF //!< DAC channel1 8-bit Right aligned data
/** @} */

/** @name DAC_DHR12R2: 	(dac Offset: 0x14) DAC channel2 12-bit right aligned data holding register */
/** @{ */
#define DAC_DHR12R2_DACC2DHR        0x00000FFF //!< DAC channel2 12-bit Right aligned data
/** @} */

/** @name DAC_DHR12L2: 	(dac Offset: 0x18) DAC channel2 12-bit left aligned data holding register  */
/** @{ */
#define DAC_DHR12L2_DACC2DHR        0x0000FFF0 //!< DAC channel2 12-bit Left aligned data
/** @} */

/** @name DAC_DHR8R2:  	(dac Offset: 0x1C) DAC channel2 8-bit right-aligned data holding register  */
/** @{ */
#define DAC_DHR8R2_DACC2DHR         0x000000FF //!< DAC channel2 8-bit Right aligned data
/** @} */

/** @name DAC_DHR12RD: 	(dac Offset: 0x20) Dual DAC 12-bit right-aligned data holding register     */
/** @{ */
#define DAC_DHR12RD_DACC2DHR        0x0FFF0000 //!< DAC channel2 12-bit Right aligned data
#define DAC_DHR12RD_DACC1DHR        0x00000FFF //!< DAC channel1 12-bit Right aligned data
/** @} */

/** @name DAC_DHR12LD: 	(dac Offset: 0x24) DUAL DAC 12-bit left aligned data holding register      */
/** @{ */
#define DAC_DHR12LD_DACC2DHR        0xFFF00000 //!< DAC channel2 12-bit Left aligned data
#define DAC_DHR12LD_DACC1DHR        0x0000FFF0 //!< DAC channel1 12-bit Left aligned data
/** @} */

/** @name DAC_DHR8RD:  	(dac Offset: 0x28) DUAL DAC 8-bit right aligned data holding register      */
/** @{ */
#define DAC_DHR8RD_DACC2DHR         0x0000FF00 //!< DAC channel2 8-bit Right aligned data
#define DAC_DHR8RD_DACC1DHR         0x000000FF //!< DAC channel1 8-bit Right aligned data
/** @} */

/** @name DAC_DOR1:    	(dac Offset: 0x2C) DAC channel1 data output register*/
/** @{ */
#define DAC_DOR1_DACC1DOR           0x00000FFF //!< DAC channel1 data output
/** @} */

/** @name DAC_DOR2:    	(dac Offset: 0x30) DAC channel2 data output register*/
/** @{ */
#define DAC_DOR2_DACC2DOR           0x00000FFF //!< DAC channel2 data output
/** @} */

/** @name DAC_SR:      	(dac Offset: 0x34) DAC status register             	   */
/** @{ */
#define DAC_SR_BWST2                0x80000000 //!< DAC channel2 busy writing sample time flag
#define DAC_SR_CAL_FLAG2            0x40000000 //!< DAC channel2 calibration offset status
#define DAC_SR_DMAUDR2              0x20000000 //!< DAC channel2 DMA underrun flag
#define DAC_SR_BWST1                0x00008000 //!< DAC channel1 busy writing sample time flag
#define DAC_SR_CAL_FLAG1            0x00004000 //!< DAC channel1 calibration offset status
#define DAC_SR_DMAUDR1              0x00002000 //!< DAC channel1 DMA underrun flag
/** @} */

/** @name DAC_CCR:      (dac Offset: 0x38) DAC calibration control register    */
/** @{ */
#define DAC_CCR_OTRIM2	            0x001F0000 //!< DAC channel2 offset trimming value
#define DAC_CCR_OTRIM1	            0x0000001F //!< DAC channel1 offset trimming value
/** @} */

/** @name DAC_MCR:      (dac Offset: 0x3C) DAC mode control register	       */
/** @{ */
#define DAC_MCR_MODE2	            0x00070000 //!< DAC channel2 mode
#define DAC_MCR_MODE2_buf_pin       0x00000000 //!<  DAC channel2 is connected to external pin with Buffer enabled
#define DAC_MCR_MODE2_int_buf_pin   0x00010000 //!<  DAC channel2 is connected to external pin and to on chip peripherals with buffer enabled
#define DAC_MCR_MODE2_pin           0x00020000 //!<  DAC channel2 is connected to external pin with buffer disabled
#define DAC_MCR_MODE2_int           0x00030000 //!<  DAC channel2 is connected to on chip peripherals with Buffer disabled
#define DAC_MCR_MODE2	            0x00070000 //!< DAC channel2 mode
#define DAC_MCR_MODE2	            0x00070000 //!< DAC channel2 mode
#define DAC_MCR_MODE2	            0x00070000 //!< DAC channel2 mode
#define DAC_MCR  	                0x00000000 //!<
/** @} */

/** @name DAC_SHSR1:    (dac Offset: 0x40) DAC Sample and Hold sample time register 1 */
/** @{ */
#define DAC_SHSR1 		            0x00000000 //!<
#define DAC_SHSR1	                0x00000000 //!<
/** @} */

/** @name DAC_SHSR2:    (dac Offset: 0x44) DAC Sample and Hold sample time register 2 */
/** @{ */
#define DAC_SHSR2	                0x00000000 //!<
#define DAC_SHSR2	                0x00000000 //!<
/** @} */

/** @name DAC_SHHR:     (dac Offset: 0x48) DAC Sample and Hold hold time register */
/** @{ */
#define DAC_SHHR	                0x00000000 //!<
#define DAC_SHHR	                0x00000000 //!<
/** @} */

/** @name DAC_SHRR:     (dac Offset: 0x4C) DAC Sample and Hold refresh time register */
/** @{ */
#define DAC_SHRR 		            0x00000000 //!<
#define DAC_SHRR	                0x00000000 //!<
/** @} */



/** @} */ // @relates DAC_TypeDef




#endif /* HARDWARE_STM32_STM32H7_DAC_H7_H_ */

/** @} */
