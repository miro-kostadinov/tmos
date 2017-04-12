/**************************************************************************//**
 * @ingroup	hardware_stm32f0_adc
 * @file	hardware/stm32/stm32f0/adc_f0.h
 * @brief  	STM32F0 ADC
 * @date    22. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f0_adc  Analog-to-digital converter (ADC)
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32F0 Analog-to-digital converter
 * @{
 *
 ******************************************************************************/

#ifndef ADC_F0_H_
#define ADC_F0_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Analog to Digital Converter
 ******************************************************************************/
typedef struct
{
  __IO uint32_t ADC_ISR;          //!< (adc Offset: 0x000) ADC Interrupt and Status register
  __IO uint32_t ADC_IER;          //!< (adc Offset: 0x004) ADC Interrupt Enable register
  __IO uint32_t ADC_CR;           //!< (adc Offset: 0x008) ADC Control register
  __IO uint32_t ADC_CFGR1;        //!< (adc Offset: 0x00C) ADC Configuration register 1
  __IO uint32_t ADC_CFGR2;        //!< (adc Offset: 0x010) ADC Configuration register 2
  __IO uint32_t ADC_SMPR;         //!< (adc Offset: 0x014) ADC Sampling time register
  __IO uint32_t reserved1[2];     //!< (adc Offset: 0x018) Reserved
  __IO uint32_t ADC_TR;           //!< (adc Offset: 0x020) ADC watchdog threshold register
  __IO uint32_t reserved2;        //!< (adc Offset: 0x024) Reserved
  __IO uint32_t ADC_CHSELR;       //!< (adc Offset: 0x028) ADC channel selection register
  __IO uint32_t reserved3[5];     //!< (adc Offset: 0x02C) Reserved
  __IO uint32_t ADC_DR;           //!< (adc Offset: 0x040) ADC data register
  __IO uint32_t reserved4[177];   //!< (adc Offset: 0x044) Reserved
  __IO uint32_t ADC_CCR;          //!< (adc Offset: 0x308) ADC common configuration register

} ADC_TypeDef;

/***************************************************************************//**
 * @relates ADC_TypeDef
 * @{
 ******************************************************************************/

/** @name ADC_ISR:      (adc Offset: 0x000) ADC Interrupt and Status register */
/** @{ */
#define ADC_ISR_AWD                 0x00000080 //!< Analog watchdog flag
#define ADC_ISR_OVR                 0x00000010 //!< Overrun flag
#define ADC_ISR_EOSEQ               0x00000008 //!< End of Sequence flag
#define ADC_ISR_EOC                 0x00000004 //!< End of Conversion
#define ADC_ISR_EOSMP               0x00000002 //!< End of sampling flag
#define ADC_ISR_ADRDY               0x00000001 //!< ADC Ready
/** @} */

/** @name ADC_IER:      (adc Offset: 0x004) ADC Interrupt Enable register  */
/** @{ */
#define ADC_IER_AWDIE               0x00000080 //!< Analog Watchdog interrupt enable
#define ADC_IER_OVRIE               0x00000010 //!< Overrun interrupt enable
#define ADC_IER_EOSEQIE             0x00000008 //!< End of Sequence of conversion interrupt enable
#define ADC_IER_EOCIE               0x00000004 //!< End of Conversion interrupt enable
#define ADC_IER_EOSMPIE             0x00000002 //!< End of sampling interrupt enable
#define ADC_IER_ADRDYIE             0x00000001 //!< ADC Ready interrupt enable
/** @} */

/** @name ADC_CR:       (adc Offset: 0x008) ADC Control register			  */
/** @{ */
#define ADC_CR_ADCAL                0x80000000 //!< ADC calibration
#define ADC_CR_ADSTP                0x00000010 //!< ADC stop of conversion command
#define ADC_CR_ADSTART              0x00000004 //!< ADC start of conversion
#define ADC_CR_ADDIS                0x00000002 //!< ADC disable command
#define ADC_CR_ADEN                 0x00000001 //!< ADC enable control
#define ADC_CR_BUSY					(ADC_CR_ADCAL | ADC_CR_ADSTP | ADC_CR_ADDIS)
/** @} */

/** @name ADC_CFGR1:    (adc Offset: 0x00C) ADC Configuration register 1 	  */
/** @{ */
#define ADC_CFGR1_AWDCH             0x7C000000 //!< AWDCH[4:0] bits (Analog watchdog channel select bits)
#define ADC_CFGR1_AWDCH_Set(x)      ((x)<<26)  //!<  AWDCH set macro
#define ADC_CFGR1_AWDEN             0x00800000 //!< Analog watchdog enable on regular channels
#define ADC_CFGR1_AWDSGL            0x00400000 //!< Enable the watchdog on a single channel or on all channels
#define ADC_CFGR1_DISCEN            0x00010000 //!< Discontinuous mode on regular channels
#define ADC_CFGR1_AUTOFF            0x00008000 //!< ADC auto power off
#define ADC_CFGR1_WAIT              0x00004000 //!< ADC wait conversion mode
#define ADC_CFGR1_CONT              0x00002000 //!< Continuous Conversion
#define ADC_CFGR1_OVRMOD            0x00001000 //!< Overrun mode
#define ADC_CFGR1_EXTEN             0x00000C00 //!< EXTEN[1:0] bits (External Trigger Conversion mode for regular channels)
#define ADC_CFGR1_EXTEN_OFF         0x00000000 //!<  Hardware trigger detection disabled
#define ADC_CFGR1_EXTEN_RE          0x00000400 //!<  Hardware trigger detection on the rising edge
#define ADC_CFGR1_EXTEN_FE          0x00000800 //!<  Hardware trigger detection on the falling edge
#define ADC_CFGR1_EXTEN_BE          0x00000C00 //!<  Hardware trigger detection on both
#define ADC_CFGR1_EXTSEL            0x000001C0 //!< EXTSEL[2:0] bits (External Event Select for regular group) *
#define ADC_CFGR1_EXTSEL_Set(x)     ((x)<<6)   //!<  EXTSEL set macro
#define ADC_CFGR1_ALIGN             0x00000020 //!< Data Alignment
#define ADC_CFGR1_RES               0x00000018 //!< RES[1:0] bits (Resolution)
#define ADC_CFGR1_RES_12            0x00000000 //!<  12 bits
#define ADC_CFGR1_RES_10            0x00000008 //!<  10 bits
#define ADC_CFGR1_RES_8             0x00000010 //!<  8 bits
#define ADC_CFGR1_RES_6             0x00000018 //!<  6 bits
#define ADC_CFGR1_SCANDIR           0x00000004 //!< Sequence scan direction
#define ADC_CFGR1_DMACFG            0x00000002 //!< Direct memory access configuration
#define ADC_CFGR1_DMAEN             0x00000001 //!< Direct memory access enable
/** @} */

/** @name ADC_CFGR2:    (adc Offset: 0x010) ADC Configuration register 2 	  */
/** @{ */
#define ADC_CFGR2_JITOFFDIV4        0x80000000 //!< Jitter Off when ADC clocked by PCLK div4
#define ADC_CFGR2_JITOFFDIV2        0x40000000 //!< Jitter Off when ADC clocked by PCLK div2
/** @} */

/** @name ADC_SMPR:     (adc Offset: 0x014) ADC Sampling time register		  */
/** @{ */
#define ADC_SMPR1_SMPR              0x00000007 //!< SMPR[2:0] bits (Sampling time selection)
#define ADC_SMPR1_SMPR_1_5          0x00000000 //!<  000: 1.5 ADC clock cycles
#define ADC_SMPR1_SMPR_7_5          0x00000001 //!<  001: 7.5 ADC clock cycles
#define ADC_SMPR1_SMPR_13_5         0x00000002 //!<  010: 13.5 ADC clock cycles
#define ADC_SMPR1_SMPR_28_5         0x00000003 //!<  011: 28.5 ADC clock cycles
#define ADC_SMPR1_SMPR_41_5         0x00000004 //!<  100: 41.5 ADC clock cycles
#define ADC_SMPR1_SMPR_55_5         0x00000005 //!<  101: 55.5 ADC clock cycles
#define ADC_SMPR1_SMPR_71_5         0x00000006 //!<  110: 71.5 ADC clock cycles
#define ADC_SMPR1_SMPR_239_5        0x00000007 //!<  111: 239.5 ADC clock cycles
/** @} */

/** @name ADC_TR:       (adc Offset: 0x020) ADC watchdog threshold register	  */
/** @{ */
#define ADC_TR_HT                   0x0FFF0000 //!< Analog watchdog high threshold
#define ADC_TR_LT                   0x00000FFF //!< Analog watchdog low threshold
/** @} */

/** @name ADC_CHSELR:   (adc Offset: 0x028) ADC channel selection register	  */
/** @{ */
#define ADC_CHSELR_CHSEL17          0x00020000 //!< Channel 17 selection
#define ADC_CHSELR_CHSEL16          0x00010000 //!< Channel 16 selection
#define ADC_CHSELR_CHSEL15          0x00008000 //!< Channel 15 selection
#define ADC_CHSELR_CHSEL14          0x00004000 //!< Channel 14 selection
#define ADC_CHSELR_CHSEL13          0x00002000 //!< Channel 13 selection
#define ADC_CHSELR_CHSEL12          0x00001000 //!< Channel 12 selection
#define ADC_CHSELR_CHSEL11          0x00000800 //!< Channel 11 selection
#define ADC_CHSELR_CHSEL10          0x00000400 //!< Channel 10 selection
#define ADC_CHSELR_CHSEL9           0x00000200 //!< Channel 9 selection
#define ADC_CHSELR_CHSEL8           0x00000100 //!< Channel 8 selection
#define ADC_CHSELR_CHSEL7           0x00000080 //!< Channel 7 selection
#define ADC_CHSELR_CHSEL6           0x00000040 //!< Channel 6 selection
#define ADC_CHSELR_CHSEL5           0x00000020 //!< Channel 5 selection
#define ADC_CHSELR_CHSEL4           0x00000010 //!< Channel 4 selection
#define ADC_CHSELR_CHSEL3           0x00000008 //!< Channel 3 selection
#define ADC_CHSELR_CHSEL2           0x00000004 //!< Channel 2 selection
#define ADC_CHSELR_CHSEL1           0x00000002 //!< Channel 1 selection
#define ADC_CHSELR_CHSEL0           0x00000001 //!< Channel 0 selection
/** @} */

/** @name ADC_DR:       (adc Offset: 0x040) ADC data register				  */
/** @{ */
#define ADC_DR_DATA                0x0000FFFF //!< Regular data */
/** @} */


/** @name ADC_CCR:      (adc Offset: 0x308) ADC common configuration register */
/** @{ */
#define ADC_CCR_VBATEN              0x01000000 //!< Voltage battery enable
#define ADC_CCR_TSEN                0x00800000 //!< Temperature sensor enable
#define ADC_CCR_VREFEN              0x00400000 //!< Temperature sensor and Vrefint enable
/** @} */


/** @} */ // @relates ADC_TypeDef


#endif /* ADC_F0_H_ */

/** @} */
