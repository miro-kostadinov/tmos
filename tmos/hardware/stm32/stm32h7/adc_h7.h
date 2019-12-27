/**************************************************************************//**
 * @ingroup	hardware_stm32h7_adc
 * @file	hardware/stm32/stm32f7/adc_h7.h
 * @brief  	STM32H7 ADC
 * @date    5. December 2019
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_adc  Analog-to-digital converter (ADC)
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Analog-to-digital converter
 * @{
 *
 ******************************************************************************/

#ifndef ADC_H7_H_
#define ADC_H7_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Analog to Digital Converter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t ADC_ISR;		//!< (adc Offset: 0x000) ADC interrupt and status register
  __IO uint32_t ADC_IER;        //!< (adc Offset: 0x004) ADC interrupt enable register
  __IO uint32_t ADC_CR;	        //!< (adc Offset: 0x008) ADC Control Register 1
  __IO uint32_t ADC_CFGR;       //!< (adc Offset: 0x00C) ADC Configuration register
  __IO uint32_t ADC_CFGR2;      //!< (adc Offset: 0x010) ADC Configuration register 2
  __IO uint32_t ADC_SMPR1;      //!< (adc Offset: 0x014) ADC sample time register 1
  __IO uint32_t ADC_SMPR2;      //!< (adc Offset: 0x018) ADC sample time register 2
  __IO uint32_t ADC_PCSEL;      //!< (adc Offset: 0x01C) ADC channel preselection register
  __IO uint32_t ADC_LTR1;       //!< (adc Offset: 0x020) ADC watchdog Lower threshold register 1
  __IO uint32_t ADC_HTR1;       //!< (adc Offset: 0x024) ADC watchdog higher threshold register 1
  __IO uint32_t reserved1[2];   //!< (adc Offset: 0x028) reserved
  __IO uint32_t ADC_SQR1;       //!< (adc Offset: 0x030) ADC regular sequence register 1
  __IO uint32_t ADC_SQR2;       //!< (adc Offset: 0x034) ADC regular sequence register 2
  __IO uint32_t ADC_SQR3;       //!< (adc Offset: 0x038) ADC regular sequence register 3
  __IO uint32_t ADC_SQR4;       //!< (adc Offset: 0x03C) ADC regular sequence register 4
  __IO uint32_t ADC_DR;         //!< (adc Offset: 0x040) ADC regular data register
  __IO uint32_t reserved2[2];   //!< (adc Offset: 0x044) reserved
  __IO uint32_t ADC_JSQR;       //!< (adc Offset: 0x04C) ADC injected sequence register
  __IO uint32_t reserved3[4];   //!< (adc Offset: 0x050) reserved
  __IO uint32_t ADC_OFRy[4];    //!< (adc Offset: 0x060) ADC channel y offset register
  __IO uint32_t reserved4[4];   //!< (adc Offset: 0x070) reserved
  __IO uint32_t ADC_JDRy[4];    //!< (adc Offset: 0x080) ADC injected channel y data register
  __IO uint32_t reserved5[4];   //!< (adc Offset: 0x090) reserved
  __IO uint32_t ADC_AWD2CR;     //!< (adc Offset: 0x0A0) ADC  Analog Watchdog 2 Configuration Register
  __IO uint32_t ADC_AWD3CR;     //!< (adc Offset: 0x0A4) ADC  Analog Watchdog 3 Configuration Register
  __IO uint32_t reserved6[2];   //!< (adc Offset: 0x0A8) reserved
  __IO uint32_t ADC_LTR2;       //!< (adc Offset: 0x0B0) ADC watchdog Lower threshold register 2
  __IO uint32_t ADC_HTR2;       //!< (adc Offset: 0x0B4) ADC watchdog Higher threshold register 2
  __IO uint32_t ADC_LTR3;       //!< (adc Offset: 0x0B8) ADC watchdog Lower threshold register 3
  __IO uint32_t ADC_HTR3;       //!< (adc Offset: 0x0BC) ADC watchdog Higher threshold register 3
  __IO uint32_t ADC_DIFSEL;     //!< (adc Offset: 0x0C0) ADC  Differential Mode Selection Register
  __IO uint32_t ADC_CALFACT;    //!< (adc Offset: 0x0C4) ADC  Calibration Factors
  __IO uint32_t ADC_CALFACT2;   //!< (adc Offset: 0x0C8) ADC  Linearity Calibration Factors
  __IO uint32_t reserved7[13];  //!< (adc Offset: 0x0CC) reserved
} ADC_TypeDef;

/// Combined ADCs
typedef struct
{
	ADC_TypeDef ADCx[3];		//!< (adcs Offset: 0x000) ADC1/2 or ADC3
  __IO uint32_t ADC_CSR;        //!< (adcs Offset: 0x300) ADC Common status register
  __IO uint32_t reserved;       //!< (adcs Offset: 0x304) reserved
  __IO uint32_t ADC_CCR;        //!< (adcs Offset: 0x308) ADC common control register
  __IO uint32_t ADC_CDR;        //!< (adcs Offset: 0x30C) ADCx common regular data register for dual mode
  __IO uint32_t ADC_CDR2;       //!< (adcs Offset: 0x310) ADCx common regular data register for 32-bit dual mode
} ADCS_TypeDef;

/***************************************************************************//**
 * @relates ADC_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_ISR:	    (adc Offset: 0x000) ADC interrupt and status register  */
/** @{ */
#define ADC_ISR_JQOVF               0x00000400 //!< Injected context queue overflow
#define ADC_ISR_AWD3                0x00000200 //!< Analog watchdog 3 flag
#define ADC_ISR_AWD2                0x00000100 //!< Analog watchdog 2 flag
#define ADC_ISR_AWD1                0x00000080 //!< Analog watchdog 1 flag
#define ADC_ISR_JEOS                0x00000040 //!< Injected channel end of sequence flag
#define ADC_ISR_JEOC                0x00000020 //!< Injected channel end of conversion
#define ADC_ISR_OVR                 0x00000010 //!< ADC overrun
#define ADC_ISR_EOS                 0x00000008 //!< End of regular sequence flag
#define ADC_ISR_EOC                 0x00000003 //!< End of conversion
#define ADC_ISR_EOSMP               0x00000002 //!< End of sampling flag
#define ADC_ISR_ADRDY               0x00000001 //!< ADC ready
/** @} */

/** @name ADC_IER:      (adc Offset: 0x004) ADC interrupt enable register	   */
/** @{ */
#define ADC_IER_JQOVFIE             0x00000400 //!< Injected context queue overflow interrupt enable
#define ADC_IER_AWD3IE              0x00000200 //!< Analog watchdog 3 interrupt enable
#define ADC_IER_AWD2IE              0x00000100 //!< Analog watchdog 2 interrupt enable
#define ADC_IER_AWD1IE              0x00000080 //!< Analog watchdog 1 interrupt enable
#define ADC_IER_JEOSIE              0x00000040 //!< End of injected sequence of conversions interrupt enable
#define ADC_IER_JEOCIE              0x00000020 //!< End of injected conversion interrupt enable
#define ADC_IER_OVRIE               0x00000010 //!< Overrun interrupt enable
#define ADC_IER_EOSIE               0x00000008 //!< End of regular sequence of conversions interrupt enable
#define ADC_IER_EOCIE               0x00000003 //!< End of regular conversion interrupt enable
#define ADC_IER_EOSMPIE             0x00000002 //!< End of sampling flag interrupt enable for regular conversions
#define ADC_IER_ADRDYIE             0x00000001 //!< ADC ready interrupt enable
/** @} */

/** @name ADC_CR:	    (adc Offset: 0x008) ADC Control Register 1	 	 	   */
/** @{ */
#define ADC_CR_ADCAL	            0x80000000 //!< ADC calibration
#define ADC_CR_ADCALDIF	            0x40000000 //!< Differential mode for calibration
#define ADC_CR_DEEPPWD	            0x20000000 //!< Deep-power-down enable
#define ADC_CR_ADVREGEN	            0x10000000 //!< ADC voltage regulator enable
#define ADC_CR_LINCALRDYW6          0x08000000 //!< Linearity calibration ready Word 6
#define ADC_CR_LINCALRDYW5          0x04000000 //!< Linearity calibration ready Word 5
#define ADC_CR_LINCALRDYW4          0x02000000 //!< Linearity calibration ready Word 4
#define ADC_CR_LINCALRDYW3          0x01000000 //!< Linearity calibration ready Word 3
#define ADC_CR_LINCALRDYW2          0x00800000 //!< Linearity calibration ready Word 2
#define ADC_CR_LINCALRDYW1          0x00400000 //!< Linearity calibration ready Word 1
#define ADC_CR_ADCALLIN	            0x00010000 //!< Linearity calibration
#define ADC_CR_BOOST	            0x00000300 //!< Boost mode control
#define ADC_CR_BOOST_Set(x)           ((x)<<8) //!<  Boost mode Set macro
#define ADC_CR_JADSTP	            0x00000020 //!< ADC stop of injected conversion command
#define ADC_CR_ADSTP	            0x00000010 //!< ADC stop of regular conversion command
#define ADC_CR_JADSTART	            0x00000008 //!< ADC start of injected conversion
#define ADC_CR_ADSTART              0x00000004 //!< ADC start of regular conversion
#define ADC_CR_ADDIS	            0x00000002 //!< ADC disable command
#define ADC_CR_ADEN		            0x00000001 //!< ADC enable control
/** @} */

/** @name ADC_CFGR:     (adc Offset: 0x00C) ADC Configuration register		   */
/** @{ */
#define ADC_CFGR_JQDIS	            0x80000000 //!< Injected Queue disable
#define ADC_CFGR_AWD1CH	            0x7C000000 //!< Analog watchdog 1 channel selection
#define ADC_CFGR_AWD1CH_Set(x)       ((x)<<26) //!<  watchdog 1 channel Set macro
#define ADC_CFGR_JAUTO	            0x02000000 //!< Automatic injected group conversion
#define ADC_CFGR_JAWD1EN            0x01000000 //!< Analog watchdog 1 enable on injected channels
#define ADC_CFGR_AWD1EN	            0x00800000 //!< Analog watchdog 1 enable on regular channels
#define ADC_CFGR_AWD1SGL            0x00400000 //!< Enable the watchdog 1 on a single channel or on all channels
#define ADC_CFGR_JQM	            0x00200000 //!< JSQR queue mode
#define ADC_CFGR_JDISCEN            0x00100000 //!< Discontinuous mode on injected channels
#define ADC_CFGR_DISCNUM            0x000E0000 //!< Discontinuous mode channel count
#define ADC_CFGR_DISCNUM_Set(x)      ((x)<<17) //!<  Discontinuous mode channel count Set macro
#define ADC_CFGR_DISCEN	            0x00010000 //!< Discontinuous mode for regular channels
#define ADC_CFGR_AUTDLY	            0x00004000 //!< Delayed conversion mode
#define ADC_CFGR_CONT	            0x00002000 //!< Single / continuous conversion mode for regular conversions
#define ADC_CFGR_OVRMOD	            0x00001000 //!< Overrun Mode
#define ADC_CFGR_EXTEN	            0x00000C00 //!< External trigger enable and polarity selection for regular channels
#define ADC_CFGR_EXTEN_OFF          0x00000000 //!<  Hardware trigger detection disabled (conversions can be launched by software)
#define ADC_CFGR_EXTEN_RISE         0x00000400 //!<  Hardware trigger detection on the rising edge
#define ADC_CFGR_EXTEN_FALL         0x00000800 //!<  Hardware trigger detection on the falling edge
#define ADC_CFGR_EXTEN_BOTH         0x00000C00 //!<  Hardware trigger detection on both the rising and falling edges
#define ADC_CFGR_EXTSEL	            0x000003E0 //!< External trigger selection for regular group
#define ADC_CFGR_EXTSEL_Set(x)        ((x)<<5) //!<  External trigger Set macro
#define ADC_CFGR_RES	            0x0000001C //!< Data resolution
#define ADC_CFGR_RES_16b            0x00000000 //!<  16 bit
#define ADC_CFGR_RES_14b            0x00000004 //!<  14 bit
#define ADC_CFGR_RES_12b            0x00000008 //!<  12 bit
#define ADC_CFGR_RES_10b            0x0000000C //!<  10 bit
#define ADC_CFGR_RES_8b             0x0000001C //!<  8 bit
#define ADC_CFGR_DMNGT	            0x00000003 //!< Data Management configuration
#define ADC_CFGR_DMNGT_DR           0x00000000 //!<  Regular conversion data stored in DR only
#define ADC_CFGR_DMNGT_DMAOSM       0x00000001 //!<  DMA One Shot Mode selected
#define ADC_CFGR_DMNGT_DFSDM        0x00000002 //!<  DFSDM mode selected
#define ADC_CFGR_DMNGT_DMACM        0x00000003 //!<  DMA Circular Mode selected
/** @} */

/** @name ADC_CFGR2:    (adc Offset: 0x010) ADC Configuration register 2	   */
/** @{ */
#define ADC_CFGR2_LSHIFT            0xF0000000 //!< Left shift factor
#define ADC_CFGR2_LSHIFT_Set(x)      ((x)<<28) //!<  Shift left bits Set macro
#define ADC_CFGR2_OSVR	            0x03FF0000 //!< Oversampling ratio
#define ADC_CFGR2_OSVR_Set(x)      	 ((x)<<16) //!<  Oversampling ratio Set macro
#define ADC_CFGR2_RSHIFT4           0x00004000 //!< Right-shift data after Offset 4 correction
#define ADC_CFGR2_RSHIFT3           0x00002000 //!< Right-shift data after Offset 3 correction
#define ADC_CFGR2_RSHIFT2           0x00001000 //!< Right-shift data after Offset 2 correction
#define ADC_CFGR2_RSHIFT1           0x00000800 //!< Right-shift data after Offset 1 correction
#define ADC_CFGR2_ROVSM	            0x00000400 //!< Regular Oversampling mode
#define ADC_CFGR2_TROVS	            0x00000200 //!< Triggered Regular Oversampling
#define ADC_CFGR2_OVSS	            0x000001E0 //!< Oversampling right shift
#define ADC_CFGR2_OVSS_Set(x)         ((x)<<5) //!<  Oversampling ratio Set macro
#define ADC_CFGR2_JOVSE	            0x00000002 //!< Injected Oversampling Enable
#define ADC_CFGR2_ROVSE	            0x00000001 //!< Regular Oversampling Enable
/** @} */

/** @name ADC_SMPR1:    (adc Offset: 0x014) ADC sample time register 1		   */
/** @{ */
#define ADC_SMPR1_SMP_Set(ch,time) 	 ((time)<<((ch)*3))  //!< Channel x sampling time selection (x = 0 to 9)
#define ADC_SMPR1_SMP_x_1_5         0x00000000 //!<  1.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_2_5         0x00000001 //!<  2.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_8_5         0x00000002 //!<  8.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_16_5        0x00000003 //!<  16.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_32_5        0x00000004 //!<  32.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_65_5        0x00000005 //!<  64.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_387_5       0x00000006 //!<  387.5 ADC clock cycles
#define ADC_SMPR1_SMP_x_810_5       0x00000007 //!<  810.5 ADC clock cycles
/** @} */

/** @name ADC_SMPR2:    (adc Offset: 0x018) ADC sample time register 2		   */
/** @{ */
#define ADC_SMPR2_SMP_Set(ch,time) 	 ((time)<<((ch)*3))  //!< Channel x sampling time selection (x = 10 to 19)
#define ADC_SMPR2_SMP_x_1_5         0x00000000 //!<  1.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_2_5         0x00000001 //!<  2.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_8_5         0x00000002 //!<  8.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_16_5        0x00000003 //!<  16.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_32_5        0x00000004 //!<  32.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_65_5        0x00000005 //!<  64.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_387_5       0x00000006 //!<  387.5 ADC clock cycles
#define ADC_SMPR2_SMP_x_810_5       0x00000007 //!<  810.5 ADC clock cycles
/** @} */

/** @name ADC_PCSEL:    (adc Offset: 0x01C) ADC channel preselection register  */
/** @{ */
#define ADC_PCSEL_Set(ch) 	 		 (1<<(ch)) //!< Channel x ( V INP[i] ) pre selection (x = 0 to 19)
/** @} */

/** @name ADC_LTR1:     (adc Offset: 0x020) ADC watchdog Lower threshold register 1  */
/** @{ */
#define ADC_LTR1_LTR1	            0x03FFFFFF //!< Analog watchdog 1 lower threshold
/** @} */

/** @name ADC_HTR1:     (adc Offset: 0x024) ADC watchdog higher threshold register 1 */
/** @{ */
#define ADC_HTR1_HTR1	            0x03FFFFFF //!< Analog watchdog 1 higher threshold
/** @} */

/** @name ADC_SQR1:     (adc Offset: 0x030) ADC regular sequence register 1	   */
/** @{ */
#define ADC_SQR1_SQ4	            0x1F000000 //!< 4th conversion in regular sequence
#define ADC_SQR1_SQ3	            0x007C0000 //!< 3rd conversion in regular sequence
#define ADC_SQR1_SQ2	            0x0001F000 //!< 2nd conversion in regular sequence
#define ADC_SQR1_SQ1	            0x000007C0 //!< 1st conversion in regular sequence
#define ADC_SQR1_L		            0x0000000F //!< Regular channel sequence length
#define ADC_SQR1_L_Set(x)       	 ((x)&0xF) //!<  Regular channel sequence length Set macro

#define ADC_SQR1_SQ4_Set(x)          ((x)<<24) //!<  SQ4 Set macro
#define ADC_SQR1_SQ3_Set(x)          ((x)<<18) //!<  SQ3 Set macro
#define ADC_SQR1_SQ2_Set(x)          ((x)<<12) //!<  SQ2 Set macro
#define ADC_SQR1_SQ1_Set(x)          ((x)<< 6) //!<  SQ1 Set macro

/** @} */

/** @name ADC_SQR2:     (adc Offset: 0x034) ADC regular sequence register 2	   */
/** @{ */
#define ADC_SQR2_SQ9	            0x1F000000 //!< 9th conversion in regular sequence
#define ADC_SQR2_SQ8	            0x007C0000 //!< 8th conversion in regular sequence
#define ADC_SQR2_SQ7	            0x0001F000 //!< 7th conversion in regular sequence
#define ADC_SQR2_SQ6	            0x000007C0 //!< 6th conversion in regular sequence
#define ADC_SQR2_SQ5	            0x0000001F //!< 5th conversion in regular sequence

#define ADC_SQR2_SQ9_Set(x)          ((x)<<24) //!<  SQ9 Set macro
#define ADC_SQR2_SQ8_Set(x)          ((x)<<18) //!<  SQ8 Set macro
#define ADC_SQR2_SQ7_Set(x)          ((x)<<12) //!<  SQ7 Set macro
#define ADC_SQR2_SQ6_Set(x)          ((x)<< 6) //!<  SQ6 Set macro
#define ADC_SQR2_SQ5_Set(x)          ((x)<< 0) //!<  SQ5 Set macro
/** @} */

/** @name ADC_SQR3:     (adc Offset: 0x038) ADC regular sequence register 3	   */
/** @{ */
#define ADC_SQR3_SQ14	            0x1F000000 //!< 14th conversion in regular sequence
#define ADC_SQR3_SQ13	            0x007C0000 //!< 13th conversion in regular sequence
#define ADC_SQR3_SQ12	            0x0001F000 //!< 12th conversion in regular sequence
#define ADC_SQR3_SQ11	            0x000007C0 //!< 11th conversion in regular sequence
#define ADC_SQR3_SQ10	            0x0000001F //!< 10th conversion in regular sequence

#define ADC_SQR3_SQ14_Set(x)         ((x)<<24) //!<  SQ14 Set macro
#define ADC_SQR3_SQ13_Set(x)         ((x)<<18) //!<  SQ13 Set macro
#define ADC_SQR3_SQ12_Set(x)         ((x)<<12) //!<  SQ12 Set macro
#define ADC_SQR3_SQ11_Set(x)         ((x)<< 6) //!<  SQ11 Set macro
#define ADC_SQR3_SQ10_Set(x)         ((x)<< 0) //!<  SQ10 Set macro
/** @} */

/** @name ADC_SQR4:     (adc Offset: 0x03C) ADC regular sequence register 4	   */
/** @{ */
#define ADC_SQR4_SQ16	            0x000007C0 //!< 16th conversion in regular sequence
#define ADC_SQR4_SQ15	            0x0000001F //!< 15th conversion in regular sequence

#define ADC_SQR4_SQ16_Set(x)         ((x)<<24) //!<  SQ16 Set macro
#define ADC_SQR4_SQ15_Set(x)         ((x)<<18) //!<  SQ15 Set macro
/** @} */

/** @name ADC_DR:       (adc Offset: 0x040) ADC regular data register		   */
/** @{ */
#define ADC_DR_RDATA	            0xFFFFFFFF //!< Regular Data converted
/** @} */

/** @name ADC_JSQR:     (adc Offset: 0x04C) ADC injected sequence register	   */
/** @{ */
#define ADC_JSQR_JSQ4	            0xF8000000 //!< 4th conversion in the injected sequence
#define ADC_JSQR_JSQ3	            0x03E00000 //!< 3rd conversion in the injected sequence
#define ADC_JSQR_JSQ2	            0x000F8000 //!< 2nd conversion in the injected sequence
#define ADC_JSQR_JSQ1	            0x00003E00 //!< 1st conversion in the injected sequence
#define ADC_JSQR_JEXTEN	            0x00000180 //!< External Trigger Enable and Polarity Selection for injected channels
#define ADC_JSQR_JEXTEN_SW          0x00000000 //!<  If JQDIS=0 (queue enabled), Hardware and software trigger detection disabled
 	 	 	 	 	 	 	 	 	 	 	   //!<  If JQDIS=1 (queue disabled), Hardware trigger detection disabled (conversions can be launched by software)
#define ADC_JSQR_JEXTEN_RISE        0x00000080 //!<  Hardware trigger detection on the rising edge
#define ADC_JSQR_JEXTEN_FALL        0x00000100 //!<  Hardware trigger detection on the falling edge
#define ADC_JSQR_JEXTEN_BOTH        0x00000180 //!<  Hardware trigger detection on both the rising and falling edges
#define ADC_JSQR_JEXTSEL            0x0000007C //!< External Trigger Selection for injected group
#define ADC_JSQR_JEXTSEL_Set(x)       ((x)<<2) //!<  External Trigger (Event x=0-31) Set macro
#define ADC_JSQR_JL		            0x00000003 //!< Injected channel sequence length
#define ADC_JSQR_JL_1               0x00000000 //!<  1 conversion
#define ADC_JSQR_JL_2               0x00000001 //!<  2 conversions
#define ADC_JSQR_JL_3               0x00000002 //!<  3 conversions
#define ADC_JSQR_JL_4               0x00000003 //!<  4 conversions

#define ADC_JSQR_JSQ4_Set(x)         ((x)<<27) //!<  JSQ4 Set macro
#define ADC_JSQR_JSQ3_Set(x)         ((x)<<21) //!<  JSQ3 Set macro
#define ADC_JSQR_JSQ2_Set(x)         ((x)<<15) //!<  JSQ2 Set macro
#define ADC_JSQR_JSQ1_Set(x)         ((x)<< 9) //!<  JSQ1 Set macro
/** @} */

/** @name ADC_OFRy:     (adc Offset: 0x060) ADC channel y (1 to 4) offset register */
/** @{ */
#define ADC_OFRy_SSATE	            0x80000000 //!< Signed saturation Enable
#define ADC_OFRy_OFFSETy_CH         0x7C000000 //!< Channel selection for the Data offset y
#define ADC_OFRy_OFFSETy_CH_Set(ch) ((ch)<<26) //!<  Channel (x=0-19) selection Set macro
#define ADC_OFRy_OFFSETy	        0x03FFFFFF //!< Data offset y for the channel programmed into bits OFFSETy_CH
#define ADC_OFRy_OFFSETy_Set(x)            (x) //!<  Data offset Set macro
/** @} */

/** @name ADC_JDRy:     (adc Offset: 0x080) ADC injected channel y (1 to 4) data register */
/** @{ */
#define ADC_JDRy_JDATA	            0xFFFFFFFF //!< Injected data
/** @} */

/** @name ADC_AWD2CR:   (adc Offset: 0x0A0) ADC  Analog Watchdog 2 Configuration Register  */
/** @{ */
#define ADC_AWD2CR_AWD2CH           0x000FFFFF //!< Analog watchdog 2 channel selection
#define ADC_AWD2CR_Set(ch)           (1<<(ch)) //!<  enable channel (0-19) to be guarded by AWD2
/** @} */

/** @name ADC_AWD3CR:   (adc Offset: 0x0A4) ADC  Analog Watchdog 3 Configuration Register  */
/** @{ */
#define ADC_AWD3CR_AWD2CH           0x000FFFFF //!< Analog watchdog 3 channel selection
#define ADC_AWD3CR_Set(ch)           (1<<(ch)) //!<  enable channel (0-19) to be guarded by AWD3
/** @} */

/** @name ADC_LTR2:     (adc Offset: 0x0B0) ADC watchdog Lower threshold register 2  */
/** @{ */
#define ADC_LTR2_LTR2	            0x03FFFFFF //!< Analog watchdog 2 lower threshold
/** @} */

/** @name ADC_HTR2:     (adc Offset: 0x0B4) ADC watchdog Higher threshold register 2 */
/** @{ */
#define ADC_HTR2_HTR2	            0x03FFFFFF //!< Analog watchdog 2 higher threshold
/** @} */

/** @name ADC_LTR3:     (adc Offset: 0x0B8) ADC watchdog Lower threshold register 3  */
/** @{ */
#define ADC_LTR3_LTR3	            0x03FFFFFF //!< Analog watchdog 3 lower threshold
/** @} */

/** @name ADC_HTR3:     (adc Offset: 0x0BC) ADC watchdog Higher threshold register 3 */
/** @{ */
#define ADC_HTR3_HTR3	            0x03FFFFFF //!< Analog watchdog 3 higher threshold
/** @} */

/** @name ADC_DIFSEL:   (adc Offset: 0x0C0) ADC  Differential Mode Selection Register */
/** @{ */
#define ADC_DIFSEL_DIFSEL           0x000FFFFF //!< Differential mode for channels 19 to 0
#define ADC_DIFSEL_Set(ch)           (1<<(ch)) //!<  channel (0-19) Set macro
/** @} */

/** @name ADC_CALFACT:  (adc Offset: 0x0C4) ADC  Calibration Factors  		   */
/** @{ */
#define ADC_CALFACT_CALFACT_D       0x07FF0000 //!< Calibration Factors in differential mode
#define ADC_CALFACT_CALFACT_S       0x000007FF //!< Calibration Factors In Single-Ended mode
/** @} */

/** @name ADC_CALFACT2: (adc Offset: 0x0C8) ADC  Linearity Calibration Factors */
/** @{ */
#define ADC_CALFACT2_LINCALFACT     0x3FFFFFFF //!< Linearity Calibration Factor
/** @} */




/** @} */ // @relates ADC_TypeDef

/***************************************************************************//**
 * @relates ADCS_TypeDef
 * @{
 ******************************************************************************/

/** @name ADCx[3]:      (adcs Offset: 0x000) ADC1/2 or ADC3				       */
#define ADC_ADCx_1             			 0 //!< ADC1 index
#define ADC_ADCx_2             			 2 //!< ADC2 index
#define ADC_ADCx_3             			 0 //!< ADC3 index
/** @} */

/** @name ADC_CSR:      (adcs Offset: 0x300) ADC Common status register	 	   */
/** @{ */
#define ADC_CSR_JQOVF_SLV           0x04000000 //!< Injected Context Queue Overflow flag of the slave ADC
#define ADC_CSR_AWD3_SLV            0x02000000 //!< Analog watchdog 3 flag of the slave ADC
#define ADC_CSR_AWD2_SLV            0x01000000 //!< Analog watchdog 2 flag of the slave ADC
#define ADC_CSR_AWD1_SLV	        0x00800000 //!< Analog watchdog 1 flag of the slave ADC
#define ADC_CSR_JEOS_SLV            0x00400000 //!< End of injected sequence flag of the slave ADC
#define ADC_CSR_JEOC_SLV            0x00200000 //!< End of injected conversion flag of the slave ADC
#define ADC_CSR_OVR_SLV             0x00100000 //!< Overrun flag of the slave ADC
#define ADC_CSR_EOS_SLV             0x00080000 //!< End of regular sequence flag of the slave ADC
#define ADC_CSR_EOC_SLV             0x00030000 //!< End of regular conversion of the slave ADC
#define ADC_CSR_EOSMP_SLV           0x00020000 //!< End of Sampling phase flag of the slave ADC
#define ADC_CSR_ADRDY_SLV           0x00010000 //!< Slave ADC ready
#define ADC_CSR_JQOVF_MST           0x00000400 //!< Injected Context Queue Overflow flag of the master ADC
#define ADC_CSR_AWD3_MST            0x00000200 //!< Analog watchdog 3 flag of the master ADC
#define ADC_CSR_AWD2_MST            0x00000100 //!< Analog watchdog 2 flag of the master ADC
#define ADC_CSR_AWD1_MST	        0x00000080 //!< Analog watchdog 1 flag of the master ADC
#define ADC_CSR_JEOS_MST            0x00000040 //!< End of injected sequence flag of the master ADC
#define ADC_CSR_JEOC_MST            0x00000020 //!< End of injected conversion flag of the master ADC
#define ADC_CSR_OVR_MST             0x00000010 //!< Overrun flag of the master ADC
#define ADC_CSR_EOS_MST             0x00000008 //!< End of regular sequence flag of the master ADC
#define ADC_CSR_EOC_MST             0x00000003 //!< End of regular conversion of the master ADC
#define ADC_CSR_EOSMP_MST           0x00000002 //!< End of Sampling phase flag of the master ADC
#define ADC_CSR_ADRDY_MST           0x00000001 //!< Master ADC ready
/** @} */

/** @name ADC_CCR:      (adcs Offset: 0x308) ADC common control register	   */
/** @{ */
#define ADC_CCR_VBATEN              0x01000000 //!< VBAT Enable
#define ADC_CCR_VSENSEEN            0x00800000 //!< Temperature sensor voltage enable
#define ADC_CCR_VREFEN	            0x00400000 //!< V_REFINT enable
#define ADC_CCR_PRESC	            0x003C0000 //!< ADC prescaler
#define ADC_CCR_PRESC_1	            0x00000000 //!<  input ADC clock not divided
#define ADC_CCR_PRESC_2	            0x00040000 //!<  input ADC clock divided by 2
#define ADC_CCR_PRESC_4	            0x00080000 //!<  input ADC clock divided by 4
#define ADC_CCR_PRESC_6	            0x000C0000 //!<  input ADC clock divided by 6
#define ADC_CCR_PRESC_8	            0x00100000 //!<  input ADC clock divided by 8
#define ADC_CCR_PRESC_10            0x00140000 //!<  input ADC clock divided by 10
#define ADC_CCR_PRESC_12            0x00180000 //!<  input ADC clock divided by 12
#define ADC_CCR_PRESC_16            0x001C0000 //!<  input ADC clock divided by 16
#define ADC_CCR_PRESC_32            0x00200000 //!<  input ADC clock divided by 32
#define ADC_CCR_PRESC_64            0x00240000 //!<  input ADC clock divided by 64
#define ADC_CCR_PRESC_128           0x00280000 //!<  input ADC clock divided by 128
#define ADC_CCR_PRESC_256           0x002C0000 //!<  input ADC clock divided by 256
#define ADC_CCR_CKMODE	            0x00030000 //!< ADC clock mode
#define ADC_CCR_CKMODE_CK_ADCx      0x00000000 //!<  CK_ADCx (x=1 to 3) (Asynchronous clock mode), generated at product level
#define ADC_CCR_CKMODE_HKLK         0x00010000 //!<  adc_hclk/1 (Synchronous clock mode)
#define ADC_CCR_CKMODE_HKLK_2       0x00020000 //!<  adc_hclk/2 (Synchronous clock mode)
#define ADC_CCR_CKMODE_HKLK_4       0x00030000 //!<  adc_hclk/4 (Synchronous clock mode)
#define ADC_CCR_DAMDF	            0x0000C000 //!< Dual ADC Mode Data Format
#define ADC_CCR_DAMDF_NO_CDR        0x00000000 //!<  Dual ADC mode without data packing (ADCx_CDR and ADCx_CDR2 registers not used)
#define ADC_CCR_DAMDF_10_32         0x00008000 //!<  Data formatting mode for 32 down to 10-bit resolution
#define ADC_CCR_DAMDF_8	            0x0000C000 //!<  Data formatting mode for 8-bit resolution
#define ADC_CCR_DELAY	            0x00000F00 //!< Delay between 2 sampling phases
#define ADC_CCR_DELAY_Set(x)         ((x)<< 8) //!<  Delay Set macro
#define ADC_CCR_DUAL	            0x0000001F //!< Dual ADC mode selection
#define ADC_CCR_DUAL_independent   	0x00000000 //!<  All the ADCs independent
#define ADC_CCR_DUAL_regsim_injsim  0x00000001 //!<  Combined regular simultaneous + injected simultaneous mode
#define ADC_CCR_DUAL_regsim_alter   0x00000002 //!<  Combined regular simultaneous + alternate trigger mode
#define ADC_CCR_DUAL_inter_injsim   0x00000003 //!<  Combined Interleaved mode + injected simultaneous mode
#define ADC_CCR_DUAL_injsim         0x00000005 //!<  Injected simultaneous mode only
#define ADC_CCR_DUAL_inter          0x00000007 //!<  Interleaved mode only
#define ADC_CCR_DUAL_alter          0x00000009 //!<  Alternate trigger mode only
/** @} */

/** @name ADC_CDR:      (adcs Offset: 0x30C) ADCx common regular data register for dual mode */
/** @{ */
#define ADC_CDR_RDATA_SLV           0xFFFF0000 //!< Regular data of the slave ADC
#define ADC_CDR_RDATA_MST           0x0000FFFF //!< Regular data of the master ADC
/** @} */

/** @name ADC_CDR2:      (adcs Offset: 0x310) ADCx common regular data register for 32-bit dual mode */
/** @{ */
#define ADC_CDR2_RDATA_ALT          0xFFFFFFFF //!< Regular data of the master/slave alternated ADCs
/** @} */


/** @} */ // @relates ADCS_TypeDef


#endif /* ADC_H7_H_ */

/** @} */
