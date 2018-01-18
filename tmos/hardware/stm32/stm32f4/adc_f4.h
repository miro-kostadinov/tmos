/**************************************************************************//**
 * @ingroup	hardware_stm32f4_adc
 * @file	hardware/stm32/stm32f4/adc_f4.h
 * @brief  	STM32F4 ADC
 * @date    22. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f4_adc  Analog to Digital Converter (ADC)
 * @ingroup	 hardware_stm32f4
 * Source and definitions for STM32F4 Analog to Digital Converter (ADC)
 * @{
 *
 ******************************************************************************/

#ifndef ADC_F4_H_
#define ADC_F4_H_


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
  __IO uint32_t ADC_JOFRx[4];   //!< (adc Offset: 0x014) ADC injected channel data offset register x
  __IO uint32_t ADC_HTR;        //!< (adc Offset: 0x024) ADC watchdog high threshold register
  __IO uint32_t ADC_LTR;        //!< (adc Offset: 0x028) ADC watchdog low threshold register
  __IO uint32_t ADC_SQR1;       //!< (adc Offset: 0x02C) ADC regular sequence register 1
  __IO uint32_t ADC_SQR2;       //!< (adc Offset: 0x030) ADC regular sequence register 2
  __IO uint32_t ADC_SQR3;       //!< (adc Offset: 0x034) ADC regular sequence register 3
  __IO uint32_t ADC_JSQR;		//!< (adc Offset: 0x038) ADC injected sequence register
  __IO uint32_t ADC_JDRx[4];	//!< (adc Offset: 0x03C) ADC injected data register x
  __IO uint32_t ADC_DR;         //!< (adc Offset: 0x04C) ADC data register
  __IO uint32_t reserved[44];   //!< (adc Offset: 0x050) reserved
} ADC_TypeDef;

/// Combined ADCs
typedef struct
{
	ADC_TypeDef ADCx[3];		//!< (adcs Offset: 0x000) ADC1-ADC3
  __IO uint32_t ADC_CSR;        //!< (adcs Offset: 0x300) ADC Common status register
  __IO uint32_t ADC_CCR;        //!< (adcs Offset: 0x304) ADC common control register
  __IO uint32_t ADC_CDR;        //!< (adcs Offset: 0x308) ADC common regular data register for dual and triple modes
} ADCS_TypeDef;

/***************************************************************************//**
 * @relates ADC_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_SR:		(adc Offset: 0x000) ADC Status Register				  */
/** @{ */
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
#define ADC_CR1_RES_12              0x00000000 //!<  12-bit (15 ADCCLK cycles)
#define ADC_CR1_RES_10              0x01000000 //!<  10-bit (13 ADCCLK cycles)
#define ADC_CR1_RES_8               0x02000000 //!<  8-bit (11 ADCCLK cycles)
#define ADC_CR1_RES_6               0x03000000 //!<  6-bit (9 ADCCLK cycles)
#define ADC_CR1_AWDEN               0x00800000 //!< Analog watchdog enable on regular channels
#define ADC_CR1_JAWDEN              0x00400000 //!< Analog watchdog enable on injected channels
#define ADC_CR1_DISCNUM             0x0000E000 //!< DISCNUM[2:0] bits (Discontinuous mode channel count)
#define ADC_CR1_JDISCEN             0x00001000 //!< Discontinuous mode on injected channels
#define ADC_CR1_DISCEN              0x00000800 //!< Discontinuous mode on regular channels
#define ADC_CR1_JAUTO               0x00000400 //!< Automatic injected group conversion
#define ADC_CR1_AWDSGL              0x00000200 //!< Enable the watchdog on a single channel in scan mode
#define ADC_CR1_SCAN                0x00000100 //!< Scan mode
#define ADC_CR1_JEOCIE              0x00000080 //!< Interrupt enable for injected channels
#define ADC_CR1_AWDIE               0x00000040 //!< Analog Watchdog interrupt enable
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
#define ADC_CR2_CONT                0x00000002 //!< Continuous Conversion
#define ADC_CR2_ADON                0x00000001 //!< A/D Converter ON / OFF
/** @} */

/** @name ADC_SMPR1:    (adc Offset: 0x00C) ADC Sample time register 1		  */
/** @{ */
#define ADC_SMPR1_SMP18             0x07000000 //!< SMP18[2:0] bits (Channel 18 Sample time selection)
#define ADC_SMPR1_SMP17             0x00E00000 //!< SMP17[2:0] bits (Channel 17 Sample time selection)
#define ADC_SMPR1_SMP16             0x001C0000 //!< SMP16[2:0] bits (Channel 16 Sample time selection)
#define ADC_SMPR1_SMP15             0x00038000 //!< SMP15[2:0] bits (Channel 15 Sample time selection)
#define ADC_SMPR1_SMP14             0x00007000 //!< SMP14[2:0] bits (Channel 14 Sample time selection)
#define ADC_SMPR1_SMP13             0x00000E00 //!< SMP13[2:0] bits (Channel 13 Sample time selection)
#define ADC_SMPR1_SMP12             0x000001C0 //!< SMP12[2:0] bits (Channel 12 Sample time selection)
#define ADC_SMPR1_SMP11             0x00000038 //!< SMP11[2:0] bits (Channel 11 Sample time selection)
#define ADC_SMPR1_SMP10             0x00000007 //!< SMP10[2:0] bits (Channel 10 Sample time selection)
/** @} */

/** @name ADC_SMPR2:    (adc Offset: 0x010) ADC Sample time register 2		  */
/** @{ */
#define ADC_SMPR2_SMP9              0x38000000 //!< SMP9[2:0] bits (Channel 9 Sample time selection)
#define ADC_SMPR2_SMP8              0x07000000 //!< SMP8[2:0] bits (Channel 8 Sample time selection)
#define ADC_SMPR2_SMP7              0x00E00000 //!< SMP7[2:0] bits (Channel 7 Sample time selection)
#define ADC_SMPR2_SMP6              0x001C0000 //!< SMP6[2:0] bits (Channel 6 Sample time selection)
#define ADC_SMPR2_SMP5              0x00038000 //!< SMP5[2:0] bits (Channel 5 Sample time selection)
#define ADC_SMPR2_SMP4              0x00007000 //!< SMP4[2:0] bits (Channel 4 Sample time selection)
#define ADC_SMPR2_SMP3              0x00000E00 //!< SMP3[2:0] bits (Channel 3 Sample time selection)
#define ADC_SMPR2_SMP2              0x000001C0 //!< SMP2[2:0] bits (Channel 2 Sample time selection)
#define ADC_SMPR2_SMP1              0x00000038 //!< SMP1[2:0] bits (Channel 1 Sample time selection)
#define ADC_SMPR2_SMP0              0x00000007 //!< SMP0[2:0] bits (Channel 0 Sample time selection)
/** @} */

/** @name ADC_JOFRx[4]: (adc Offset: 0x014) ADC injected channel data offset register x */
/** @{ */
#define ADC_JOFR1_JOFFSETx          0x00000FFF //!< Data offset for injected channel x
/** @} */

/** @name ADC_HTR:      (adc Offset: 0x024) ADC watchdog high threshold register */
/** @{ */
#define ADC_HTR_HT                  0x00000FFF //!< Analog watchdog high threshold
/** @} */

/** @name ADC_LTR:      (adc Offset: 0x028) ADC watchdog low threshold register */
/** @{ */
#define ADC_LTR_LT                  0x00000FFF //!< Analog watchdog low threshold
/** @} */

/** @name ADC_SQR1:     (adc Offset: 0x02C) ADC regular sequence register 1   */
/** @{ */
#define ADC_SQR1_L                  0x00F00000 //!< L[3:0] bits (Regular channel sequence length)
#define ADC_SQR1_SQ16               0x000F8000 //!< SQ16[4:0] bits (16th conversion in regular sequence)
#define ADC_SQR1_SQ15               0x00007C00 //!< SQ15[4:0] bits (15th conversion in regular sequence)
#define ADC_SQR1_SQ14               0x000003E0 //!< SQ14[4:0] bits (14th conversion in regular sequence)
#define ADC_SQR1_SQ13               0x0000001F //!< SQ13[4:0] bits (13th conversion in regular sequence)
/** @} */

/** @name ADC_SQR2:     (adc Offset: 0x030) ADC regular sequence register 2   */
/** @{ */
#define ADC_SQR2_SQ12               0x3E000000 //!< SQ12[4:0] bits (12th conversion in regular sequence)
#define ADC_SQR2_SQ11               0x01F00000 //!< SQ11[4:0] bits (11th conversion in regular sequence)
#define ADC_SQR2_SQ10               0x000F8000 //!< SQ10[4:0] bits (10th conversion in regular sequence)
#define ADC_SQR2_SQ9                0x00007C00 //!< SQ9[4:0] bits (9th conversion in regular sequence)
#define ADC_SQR2_SQ8                0x000003E0 //!< SQ8[4:0] bits (8th conversion in regular sequence)
#define ADC_SQR2_SQ7                0x0000001F //!< SQ7[4:0] bits (7th conversion in regular sequence)
/** @} */

/** @name ADC_SQR3:     (adc Offset: 0x034) ADC regular sequence register 3   */
/** @{ */
#define ADC_SQR3_SQ6                0x3E000000 //!< SQ6[4:0] bits (6th conversion in regular sequence)
#define ADC_SQR3_SQ5                0x01F00000 //!< SQ5[4:0] bits (5th conversion in regular sequence)
#define ADC_SQR3_SQ4                0x000F8000 //!< SQ4[4:0] bits (4th conversion in regular sequence)
#define ADC_SQR3_SQ3                0x00007C00 //!< SQ3[4:0] bits (3rd conversion in regular sequence)
#define ADC_SQR3_SQ2                0x000003E0 //!< SQ2[4:0] bits (2nd conversion in regular sequence)
#define ADC_SQR3_SQ1                0x0000001F //!< SQ1[4:0] bits (1st conversion in regular sequence)

#define ADC_SQR3_SQ6_Set(x)          ((x)<<25) //!<  SQ6 Set macro
#define ADC_SQR3_SQ5_Set(x)          ((x)<<20) //!<  SQ5 Set macro
#define ADC_SQR3_SQ4_Set(x)          ((x)<<15) //!<  SQ4 Set macro
#define ADC_SQR3_SQ3_Set(x)          ((x)<<10) //!<  SQ3 Set macro
#define ADC_SQR3_SQ2_Set(x)           ((x)<<5) //!<  SQ2 Set macro
#define ADC_SQR3_SQ1_Set(x)           ((x)<<0) //!<  SQ1 Set macro
/** @} */

/** @name ADC_JSQR:		(adc Offset: 0x038) ADC injected sequence register 	  */
/** @{ */
#define ADC_JSQR_JL                 0x00300000 //!< JL[1:0] bits (Injected Sequence length)
#define ADC_JSQR_JL_Get(x)       (((x)>>20)&3) //!<  JL Get macro
#define ADC_JSQR_JL_Set(x)       (((x)-1)<<20) //!<  JL Set macro
#define ADC_JSQR_JSQ4               0x000F8000 //!< JSQ4[4:0] bits (4th conversion in injected sequence)
#define ADC_JSQR_JSQ3               0x00007C00 //!< JSQ3[4:0] bits (3rd conversion in injected sequence)
#define ADC_JSQR_JSQ2               0x000003E0 //!< JSQ2[4:0] bits (2nd conversion in injected sequence)
#define ADC_JSQR_JSQ1               0x0000001F //!< JSQ1[4:0] bits (1st conversion in injected sequence)
/** @} */

/** @name ADC_JDRx[4]:	(adc Offset: 0x03C) ADC injected data register x	  */
/** @{ */
#define ADC_JDRx_JDATA              0x0000FFFF //!< Injected data
/** @} */

/** @name ADC_DR:       (adc Offset: 0x04C) ADC data register				  */
/** @{ */
#define ADC_DR_ADC2DATA             0xFFFF0000 //!< ADC2 data
#define ADC_DR_DATA                 0x0000FFFF //!< Regular data
/** @} */

/** @} */ // @relates ADC_TypeDef

/***************************************************************************//**
 * @relates ADCS_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_CSR:      (adcs Offset: 0x300) ADC Common status register	 	  */
/** @{ */
#define ADC_CSR_DOVR3               0x00200000 //!< ADC3 DMA overrun  flag
#define ADC_CSR_STRT3               0x00100000 //!< ADC3 Regular channel Start flag
#define ADC_CSR_JSTRT3              0x00080000 //!< ADC3 Injected channel Start flag
#define ADC_CSR_JEOC3               0x00040000 //!< ADC3 Injected channel end of conversion
#define ADC_CSR_EOC3                0x00020000 //!< ADC3 End of conversion
#define ADC_CSR_AWD3                0x00010000 //!< ADC3 Analog watchdog flag
#define ADC_CSR_DOVR2               0x00002000 //!< ADC2 DMA overrun  flag
#define ADC_CSR_STRT2               0x00001000 //!< ADC2 Regular channel Start flag
#define ADC_CSR_JSTRT2              0x00000800 //!< ADC2 Injected channel Start flag
#define ADC_CSR_JEOC2               0x00000400 //!< ADC2 Injected channel end of conversion
#define ADC_CSR_EOC2                0x00000200 //!< ADC2 End of conversion
#define ADC_CSR_AWD2                0x00000100 //!< ADC2 Analog watchdog flag
#define ADC_CSR_DOVR1               0x00000020 //!< ADC1 DMA overrun  flag
#define ADC_CSR_STRT1               0x00000010 //!< ADC1 Regular channel Start flag
#define ADC_CSR_JSTRT1              0x00000008 //!< ADC1 Injected channel Start flag
#define ADC_CSR_JEOC1               0x00000004 //!< ADC1 Injected channel end of conversion
#define ADC_CSR_EOC1                0x00000002 //!< ADC1 End of conversion
#define ADC_CSR_AWD1                0x00000001 //!< ADC1 Analog watchdog flag
/** @} */

/** @name ADC_CCR:      (adcs Offset: 0x304) ADC common control register	  */
/** @{ */
#define ADC_CCR_TSVREFE             0x00800000 //!< Temperature Sensor and VREFINT Enable
#define ADC_CCR_VBATE               0x00400000 //!< VBAT Enable
#define ADC_CCR_ADCPRE              0x00030000 //!< ADCPRE[1:0] bits (ADC prescaler)
#define ADC_CCR_ADCPRE_2            0x00000000 //!<  PCLK2 divided by 2
#define ADC_CCR_ADCPRE_4            0x00010000 //!<  PCLK2 divided by 4
#define ADC_CCR_ADCPRE_6            0x00020000 //!<  PCLK2 divided by 6
#define ADC_CCR_ADCPRE_8            0x00030000 //!<  PCLK2 divided by 8
#define ADC_CCR_DMA                 0x0000C000 //!< DMA[1:0] bits (Direct Memory Access mode for multimode)
#define ADC_CCR_DDS                 0x00002000 //!< DMA disable selection (Multi-ADC mode)
#define ADC_CCR_DELAY               0x00000F00 //!< DELAY[3:0] bits (Delay between 2 sampling phases)
#define ADC_CCR_DELAY_5             0x00000000 //!<   5 * TADCCLK
#define ADC_CCR_DELAY_6             0x00000100 //!<   6 * TADCCLK
#define ADC_CCR_DELAY_7             0x00000200 //!<   7 * TADCCLK
#define ADC_CCR_DELAY_8             0x00000300 //!<   8 * TADCCLK
#define ADC_CCR_DELAY_9             0x00000400 //!<   9 * TADCCLK
#define ADC_CCR_DELAY_10            0x00000500 //!<   10 * TADCCLK
#define ADC_CCR_DELAY_11            0x00000600 //!<   11 * TADCCLK
#define ADC_CCR_DELAY_12            0x00000700 //!<   12 * TADCCLK
#define ADC_CCR_DELAY_13            0x00000800 //!<   13 * TADCCLK
#define ADC_CCR_DELAY_14            0x00000900 //!<   14 * TADCCLK
#define ADC_CCR_DELAY_15            0x00000A00 //!<   15 * TADCCLK
#define ADC_CCR_DELAY_16            0x00000B00 //!<   16 * TADCCLK
#define ADC_CCR_DELAY_17            0x00000C00 //!<   17 * TADCCLK
#define ADC_CCR_DELAY_18            0x00000D00 //!<   18 * TADCCLK
#define ADC_CCR_DELAY_19            0x00000E00 //!<   19 * TADCCLK
#define ADC_CCR_DELAY_20            0x00000F00 //!<   20 * TADCCLK
#define ADC_CCR_MULTI               0x0000001F //!< MULTI[4:0] bits (Multi-ADC mode selection)
#define ADC_CCR_MULTI_independent   		0x00000000 //!<  All the ADCs independent
#define ADC_CCR_MULTI_dual_regsim_injsim   	0x00000001 //!<  ADC1-2, regular simultaneous + injected simultaneous
#define ADC_CCR_MULTI_dual_regsim_alter   	0x00000002 //!<  ADC1-2, regular simultaneous + alternate trigger mode
#define ADC_CCR_MULTI_dual_injsim		   	0x00000005 //!<  ADC1-2, injected simultaneous mode only
#define ADC_CCR_MULTI_dual_regsim		   	0x00000006 //!<  ADC1-2, regular simultaneous mode only
#define ADC_CCR_MULTI_dual_inter		   	0x00000007 //!<  ADC1-2, interleaved mode only
#define ADC_CCR_MULTI_dual_alter		   	0x00000009 //!<  ADC1-2, alternate trigger mode only
#define ADC_CCR_MULTI_triple_regsim_injsim 	0x00000011 //!<  ADC1-3, regular simultaneous + injected simultaneous mode
#define ADC_CCR_MULTI_triple_regsim_alter  	0x00000012 //!<  ADC1-3, regular simultaneous + alternate trigger mode
#define ADC_CCR_MULTI_triple_injsim		   	0x00000015 //!<  ADC1-3, injected simultaneous mode only
#define ADC_CCR_MULTI_triple_regsim		   	0x00000016 //!<  ADC1-3, regular simultaneous mode only
#define ADC_CCR_MULTI_triple_inter		   	0x00000017 //!<  ADC1-3, interleaved mode only
#define ADC_CCR_MULTI_triple_alter		   	0x00000019 //!<  ADC1-3, alternate trigger mode only
/** @} */

/** @name ADC_CDR:      (adcs Offset: 0x308) ADC common regular data register for dual and triple modes */
/** @{ */
#define ADC_CDR_DATA2               0xFFFF0000 //!< 2nd data of a pair of regular conversions
#define ADC_CDR_DATA1               0x0000FFFF //!< 1st data of a pair of regular conversions
/** @} */

/** @} */ // @relates ADCS_TypeDef



#endif /* ADC_F4_H_ */

/** @} */

