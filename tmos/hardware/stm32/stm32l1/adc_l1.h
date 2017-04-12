/**************************************************************************//**
 * @ingroup	hardware_stm32l1_adc
 * @file	hardware/stm32/stm32l1/adc_l1.h
 * @brief  	STM32L1 ADC
 * @date    22. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32l1_adc  Analog to Digital Converter (ADC)
 * @ingroup	 hardware_stm32l1
 * Source and definitions for STM32L1 Analog to Digital Converter (ADC)
 * @{
 *
 ******************************************************************************/

#ifndef ADC_L1_H_
#define ADC_L1_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Analog to Digital Converter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t ADC_SR;			//!< (adc Offset: 0x000) ADC Status Register
  __IO uint32_t ADC_CR1;        //!< (adc Offset: 0x004) ADC Control Register 1
  __IO uint32_t ADC_CR2;        //!< (adc Offset: 0x008) ADC Control Register 2
  __IO uint32_t ADC_SMPR1;      //!< (adc Offset: 0x00C) ADC Sample time register 1
  __IO uint32_t ADC_SMPR2;      //!< (adc Offset: 0x010) ADC Sample time register 2
  __IO uint32_t ADC_SMPR3;      //!< (adc Offset: 0x014) ADC Sample time register 3
  __IO uint32_t ADC_JOFRx[4];   //!< (adc Offset: 0x018) ADC injected channel data offset register x
  __IO uint32_t ADC_HTR;        //!< (adc Offset: 0x028) ADC watchdog high threshold register
  __IO uint32_t ADC_LTR;        //!< (adc Offset: 0x02C) ADC watchdog low threshold register
  __IO uint32_t ADC_SQR1;       //!< (adc Offset: 0x030) ADC regular sequence register 1
  __IO uint32_t ADC_SQR2;       //!< (adc Offset: 0x034) ADC regular sequence register 2
  __IO uint32_t ADC_SQR3;       //!< (adc Offset: 0x038) ADC regular sequence register 3
  __IO uint32_t ADC_SQR4;       //!< (adc Offset: 0x03C) ADC regular sequence register 4
  __IO uint32_t ADC_SQR5;       //!< (adc Offset: 0x040) ADC regular sequence register 5
  __IO uint32_t ADC_JSQR;		//!< (adc Offset: 0x044) ADC injected sequence register
  __IO uint32_t ADC_JDRx[4];	//!< (adc Offset: 0x048) ADC injected data register x
  __IO uint32_t ADC_DR;         //!< (adc Offset: 0x058) ADC regular data register
  __IO uint32_t SMPR0;        	//!< (adc Offset: 0x05C) ADC sample time register 0
  __IO uint32_t reserved[40];   //!< (adc Offset: 0x060) reserved
} ADC_TypeDef;

/// Combined ADCs
typedef struct
{
	ADC_TypeDef ADCx[3];		//!< (adcs Offset: 0x000) ADC1-ADC3
  __IO uint32_t ADC_CSR;        //!< (adcs Offset: 0x300) ADC Common status register
  __IO uint32_t ADC_CCR;        //!< (adcs Offset: 0x304) ADC common control register
} ADCS_TypeDef;

/***************************************************************************//**
 * @relates ADC_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_SR:		(adc Offset: 0x000) ADC Status Register				  */
/** @{ */
#define ADC_SR_JCNR                 0x00000200 //!< Injected channel not ready flag
#define ADC_SR_RCNR                 0x00000100 //!< Regular channel not ready flag
#define ADC_SR_ADONS                0x00000040 //!< ADC ON status
#define ADC_SR_OVR                  0x00000020 //!< Overrun flag
#define ADC_SR_STRT                 0x00000010 //!< Regular channel Start flag
#define ADC_SR_JSTRT                0x00000008 //!< Injected channel Start flag
#define ADC_SR_JEOC                 0x00000004 //!< Injected channel end of conversion
#define ADC_SR_EOC                  0x00000002 //!< End of conversion
#define ADC_SR_AWD                  0x00000001 //!< Analog watchdog flag
/** @} */

/** @name ADC_CR1:      (adc Offset: 0x004) ADC Control Register 1			  */
/** @{ */
#define ADC_CR1_OVRIE               0x04000000 //!< overrun interrupt enable
#define ADC_CR1_RES                 0x03000000 //!< RES[2:0] bits (Resolution)
#define ADC_CR1_AWDEN               0x00800000 //!< Analog watchdog enable on regular channels
#define ADC_CR1_JAWDEN              0x00400000 //!< Analog watchdog enable on injected channels
#define ADC_CR1_PDI                 0x00020000 //!< Power Down during Idle phase
#define ADC_CR1_PDD                 0x00010000 //!< Power Down during Delay phase
#define ADC_CR1_DISCNUM             0x0000E000 //!< DISCNUM[2:0] bits (Discontinuous mode channel count)
#define ADC_CR1_JDISCEN             0x00001000 //!< Discontinuous mode on injected channels
#define ADC_CR1_DISCEN              0x00000800 //!< Discontinuous mode on regular channels
#define ADC_CR1_JAUTO               0x00000400 //!< Automatic injected group conversion
#define ADC_CR1_AWDSGL              0x00000200 //!< Enable the watchdog on a single channel in scan mode
#define ADC_CR1_SCAN                0x00000100 //!< Scan mode
#define ADC_CR1_JEOCIE              0x00000080 //!< Interrupt enable for injected channels
#define ADC_CR1_AWDIE               0x00000040 //!< AAnalog Watchdog interrupt enable
#define ADC_CR1_EOCIE               0x00000020 //!< Interrupt enable for EOC
#define ADC_CR1_AWDCH               0x0000001F //!< AWDCH[4:0] bits (Analog watchdog channel select bits)
/** @} */

/** @name ADC_CR2:      (adc Offset: 0x008) ADC Control Register 2			  */
/** @{ */
#define ADC_CR2_SWSTART             0x40000000 //!< Start Conversion of regular channels
#define ADC_CR2_EXTEN               0x30000000 //!< EXTEN[1:0] bits (External Trigger Conversion mode for regular channelsp)
#define ADC_CR2_EXTSEL              0x0F000000 //!< EXTSEL[3:0] bits (External Event Select for regular group)
#define ADC_CR2_JSWSTART            0x00400000 //!< Start Conversion of injected channels
#define ADC_CR2_JEXTEN              0x00300000 //!< JEXTEN[1:0] bits (External Trigger Conversion mode for injected channelsp)
#define ADC_CR2_JEXTSEL             0x000F0000 //!< JEXTSEL[3:0] bits (External event select for injected group)
#define ADC_CR2_ALIGN               0x00000800 //!< Data Alignment
#define ADC_CR2_EOCS                0x00000400 //!< End of conversion selection
#define ADC_CR2_DDS                 0x00000200 //!< DMA disable selection (Single ADC)
#define ADC_CR2_DMA                 0x00000100 //!< Direct Memory access mode
#define ADC_CR2_DELS                0x00000070 //!< DELS[2:0] bits (Delay selection)
#define ADC_CR2_CFG                 0x00000004 //!< ADC Configuration
#define ADC_CR2_CONT                0x00000002 //!< Continuous Conversion
#define ADC_CR2_ADON                0x00000001 //!< A/D Converter ON / OFF
/** @} */

/** @name ADC_SMPR1:    (adc Offset: 0x00C) ADC Sample time register 1		  */
/** @{ */
#define ADC_SMPR1_SMP29             0x38000000 //!< SMP29[2:0] bits (Channel 29 Sample time selection)
#define ADC_SMPR1_SMP28             0x07000000 //!< SMP28[2:0] bits (Channel 28 Sample time selection)
#define ADC_SMPR1_SMP27             0x00E00000 //!< SMP27[2:0] bits (Channel 27 Sample time selection)
#define ADC_SMPR1_SMP26             0x001C0000 //!< SMP26[2:0] bits (Channel 26 Sample time selection)
#define ADC_SMPR1_SMP25             0x00038000 //!< SMP25[2:0] bits (Channel 25 Sample time selection)
#define ADC_SMPR1_SMP24             0x00007000 //!< SMP24[2:0] bits (Channel 24 Sample time selection)
#define ADC_SMPR1_SMP23             0x00000E00 //!< SMP23[2:0] bits (Channel 23 Sample time selection)
#define ADC_SMPR1_SMP22             0x000001C0 //!< SMP22[2:0] bits (Channel 22 Sample time selection)
#define ADC_SMPR1_SMP21             0x00000038 //!< SMP21[2:0] bits (Channel 21 Sample time selection)
#define ADC_SMPR1_SMP20             0x00000007 //!< SMP20[2:0] bits (Channel 20 Sample time selection)
/** @} */

/** @name ADC_SMPR2:    (adc Offset: 0x010) ADC Sample time register 2		  */
/** @{ */
#define ADC_SMPR2_SMP19             0x38000000 //!< SMP19[2:0] bits (Channel 19 Sample time selection)
#define ADC_SMPR2_SMP18             0x07000000 //!< SMP18[2:0] bits (Channel 18 Sample time selection)
#define ADC_SMPR2_SMP17             0x00E00000 //!< SMP17[2:0] bits (Channel 17 Sample time selection)
#define ADC_SMPR2_SMP16             0x001C0000 //!< SMP16[2:0] bits (Channel 16 Sample time selection)
#define ADC_SMPR2_SMP15             0x00038000 //!< SMP15[2:0] bits (Channel 15 Sample time selection)
#define ADC_SMPR2_SMP14             0x00007000 //!< SMP14[2:0] bits (Channel 14 Sample time selection)
#define ADC_SMPR2_SMP13             0x00000E00 //!< SMP13[2:0] bits (Channel 13 Sample time selection)
#define ADC_SMPR2_SMP12             0x000001C0 //!< SMP12[2:0] bits (Channel 12 Sample time selection)
#define ADC_SMPR2_SMP11             0x00000038 //!< SMP11[2:0] bits (Channel 11 Sample time selection)
#define ADC_SMPR2_SMP10             0x00000007 //!< SMP10[2:0] bits (Channel 10 Sample time selection)
/** @} */

/** @name ADC_SMPR3:    (adc Offset: 0x014) ADC Sample time register 3		  */
/** @{ */
#define ADC_SMPR3_SMP9              0x38000000 //!< SMP9[2:0] bits (Channel 9 Sample time selection)
#define ADC_SMPR3_SMP8              0x07000000 //!< SMP8[2:0] bits (Channel 8 Sample time selection)
#define ADC_SMPR3_SMP7              0x00E00000 //!< SMP7[2:0] bits (Channel 7 Sample time selection)
#define ADC_SMPR3_SMP6              0x001C0000 //!< SMP6[2:0] bits (Channel 6 Sample time selection)
#define ADC_SMPR3_SMP5              0x00038000 //!< SMP5[2:0] bits (Channel 5 Sample time selection)
#define ADC_SMPR3_SMP4              0x00007000 //!< SMP4[2:0] bits (Channel 4 Sample time selection)
#define ADC_SMPR3_SMP3              0x00000E00 //!< SMP3[2:0] bits (Channel 3 Sample time selection)
#define ADC_SMPR3_SMP2              0x000001C0 //!< SMP2[2:0] bits (Channel 2 Sample time selection)
#define ADC_SMPR3_SMP1              0x00000038 //!< SMP1[2:0] bits (Channel 1 Sample time selection)
#define ADC_SMPR3_SMP0              0x00000007 //!< SMP0[2:0] bits (Channel 0 Sample time selection)
/** @} */

/** @name ADC_JOFRx[4]: (adc Offset: 0x018) ADC injected channel data offset register x */
/** @{ */
#define ADC_JOFR1_JOFFSETx          0x00000FFF //!< Data offset for injected channel x
/** @} */

/** @name ADC_HTR:      (adc Offset: 0x028) ADC watchdog high threshold register */
/** @{ */
#define ADC_HTR_HT                  0x00000FFF //!< Analog watchdog high threshold
/** @} */

/** @name ADC_LTR:      (adc Offset: 0x02C) ADC watchdog low threshold register */
/** @{ */
#define ADC_LTR_LT                  0x00000FFF //!< Analog watchdog low threshold
/** @} */

/** @name ADC_SQR1:     (adc Offset: 0x030) ADC regular sequence register 1	  */
/** @{ */
#define ADC_SQR1_L                  0x00F00000 //!< L[3:0] bits (Regular channel sequence length)
#define ADC_SQR1_SQ27               0x00007C00 //!< SQ27[4:0] bits (27th conversion in regular sequence)
#define ADC_SQR1_SQ26               0x000003E0 //!< SQ26[4:0] bits (26th conversion in regular sequence)
#define ADC_SQR1_SQ25               0x0000001F //!< SQ25[4:0] bits (25th conversion in regular sequence)
#define ADC_SQR1_SQ28               0x000F8000 //!< SQ28[4:0] bits (25th conversion in regular sequence)
/** @} */

/** @name ADC_SQR2:     (adc Offset: 0x034) ADC regular sequence register 2   */
/** @{ */
#define ADC_SQR2_SQ19               0x0000001F //!< SQ19[4:0] bits (19th conversion in regular sequence)
#define ADC_SQR2_SQ20               0x000003E0 //!< SQ20[4:0] bits (20th conversion in regular sequence)
#define ADC_SQR2_SQ21               0x00007C00 //!< SQ21[4:0] bits (21th conversion in regular sequence)
#define ADC_SQR2_SQ22               0x000F8000 //!< SQ22[4:0] bits (22th conversion in regular sequence)
#define ADC_SQR2_SQ23               0x01F00000 //!< SQ23[4:0] bits (23th conversion in regular sequence)
#define ADC_SQR2_SQ24               0x3E000000 //!< SQ24[4:0] bits (24th conversion in regular sequence)
/** @} */

/** @name ADC_SQR3:     (adc Offset: 0x038) ADC regular sequence register 3	  */
/** @{ */
#define ADC_SQR3_SQ18               0x3E000000 //!< SQ18[4:0] bits (18th conversion in regular sequence)
#define ADC_SQR3_SQ17               0x01F00000 //!< SQ17[4:0] bits (17th conversion in regular sequence)
#define ADC_SQR3_SQ16               0x000F8000 //!< SQ16[4:0] bits (16th conversion in regular sequence)
#define ADC_SQR3_SQ15               0x00007C00 //!< SQ15[4:0] bits (15th conversion in regular sequence)
#define ADC_SQR3_SQ14               0x000003E0 //!< SQ14[4:0] bits (14th conversion in regular sequence)
#define ADC_SQR3_SQ13               0x0000001F //!< SQ13[4:0] bits (13th conversion in regular sequence)
/** @} */

/** @name ADC_SQR4:     (adc Offset: 0x03C) ADC regular sequence register 4	  */
/** @{ */
#define ADC_SQR4_SQ12               0x3E000000 //!< SQ12[4:0] bits (12th conversion in regular sequence)
#define ADC_SQR4_SQ11               0x01F00000 //!< SQ11[4:0] bits (11th conversion in regular sequence)
#define ADC_SQR4_SQ10               0x000F8000 //!< SQ10[4:0] bits (10th conversion in regular sequence)
#define ADC_SQR4_SQ9                0x00007C00 //!< SQ9[4:0] bits (9th conversion in regular sequence)
#define ADC_SQR4_SQ8                0x000003E0 //!< SQ8[4:0] bits (8th conversion in regular sequence)
#define ADC_SQR4_SQ7                0x0000001F //!< SQ7[4:0] bits (7th conversion in regular sequence)
/** @} */

/** @name ADC_SQR5:     (adc Offset: 0x040) ADC regular sequence register 5	  */
/** @{ */
#define ADC_SQR5_SQ6                0x3E000000 //!< SQ6[4:0] bits (6th conversion in regular sequence)
#define ADC_SQR5_SQ5                0x01F00000 //!< SQ5[4:0] bits (5th conversion in regular sequence)
#define ADC_SQR5_SQ4                0x000F8000 //!< SQ4[4:0] bits (4th conversion in regular sequence)
#define ADC_SQR5_SQ3                0x00007C00 //!< SQ3[4:0] bits (3rd conversion in regular sequence)
#define ADC_SQR5_SQ2                0x000003E0 //!< SQ2[4:0] bits (2nd conversion in regular sequence)
#define ADC_SQR5_SQ1                0x0000001F //!< SQ1[4:0] bits (1st conversion in regular sequence)
/** @} */


/** @name ADC_JSQR:		(adc Offset: 0x044) ADC injected sequence register	  */
/** @{ */
#define ADC_JSQR_JL                 0x00300000 //!< JL[1:0] bits (Injected Sequence length)
#define ADC_JSQR_JSQ4               0x000F8000 //!< JSQ4[4:0] bits (4th conversion in injected sequence)
#define ADC_JSQR_JSQ3               0x00007C00 //!< JSQ3[4:0] bits (3rd conversion in injected sequence)
#define ADC_JSQR_JSQ2               0x000003E0 //!< JSQ2[4:0] bits (2nd conversion in injected sequence)
#define ADC_JSQR_JSQ1               0x0000001F //!< JSQ1[4:0] bits (1st conversion in injected sequence)
/** @} */

/** @name ADC_JDRx[4]:	(adc Offset: 0x048) ADC injected data register x	  */
/** @{ */
#define ADC_JDRx_JDATA              0x0000FFFF //!< Injected data
/** @} */

/** @name ADC_DR:       (adc Offset: 0x058) ADC data register				  */
/** @{ */
#define ADC_DR_ADC2DATA             0xFFFF0000 //!< ADC2 data
#define ADC_DR_DATA                 0x0000FFFF //!< Regular data
/** @} */

/** @name SMPR0:        (adc Offset: 0x05C) ADC sample time register 0  	  */
/** @{ */
#define ADC_SMPR3_SMP31             0x00000038 //!< SMP31[2:0] bits (Channel 31 Sample time selection)
#define ADC_SMPR3_SMP30             0x00000007 //!< SMP30[2:0] bits (Channel 30 Sample time selection)
/** @} */

/** @} */ // @relates ADC_TypeDef

/***************************************************************************//**
 * @relates ADCS_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_CSR:      (adcs Offset: 0x300) ADC Common status register		  */
/** @{ */
#define ADC_CSR_ADONS1              0x00000040 //!< ADON status of ADC1 */
#define ADC_CSR_OVR1                0x00000020 //!< ADC1 DMA overrun  flag
#define ADC_CSR_STRT1               0x00000010 //!< ADC1 Regular channel Start flag
#define ADC_CSR_JSTRT1              0x00000008 //!< ADC1 Injected channel Start flag
#define ADC_CSR_JEOC1               0x00000004 //!< ADC1 Injected channel end of conversion
#define ADC_CSR_EOC1                0x00000002 //!< ADC1 End of conversion
#define ADC_CSR_AWD1                0x00000001 //!< ADC1 Analog watchdog flag
/** @} */

/** @name ADC_CCR:      (adcs Offset: 0x304) ADC common control register	  */
/** @{ */
#define ADC_CCR_ADCPRE              0x00030000 //!< ADC prescaler*/
#define ADC_CCR_TSVREFE             0x00800000 //!< Temperature Sensor and VREFINT Enable
/** @} */


/** @} */ // @relates ADCS_TypeDef


#endif /* ADC_L1_H_ */

/** @} */
