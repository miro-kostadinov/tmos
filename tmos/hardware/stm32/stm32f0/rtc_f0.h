/**************************************************************************//**
 * @ingroup	hardware_stm32f0_rtc
 * @file	hardware/stm32/stm32f0/rtc_f0.h
 * @brief  	STM32F0 RTC
 * @date    13. February 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f0_rtc  Real-Time Clock (RTC)
 * @ingroup	 hardware_stm32f0
 * Source and definitions for STM32F0 Real-Time Clock (RTC)
 * @{
 *
 ******************************************************************************/

#ifndef RTC_F0_H_
#define RTC_F0_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Real-Time Clock
 ******************************************************************************/
typedef struct
{
  __IO uint32_t RTC_TR;      		//!< (rtc Offset: 0x00) RTC time register
  __IO uint32_t RTC_DR;      		//!< (rtc Offset: 0x04) RTC date register
  __IO uint32_t RTC_CR;      		//!< (rtc Offset: 0x08) RTC control register
  __IO uint32_t RTC_ISR;     		//!< (rtc Offset: 0x0C) RTC initialization and status register
  __IO uint32_t RTC_PRER;    		//!< (rtc Offset: 0x10) RTC prescaler register
  __IO uint32_t reserved1[2];  		//!< (rtc Offset: 0x14) Reserved
  __IO uint32_t RTC_ALRMAR;  		//!< (rtc Offset: 0x1C) RTC alarm A register
  __IO uint32_t reserved2;  		//!< (rtc Offset: 0x20) Reserved
  __IO uint32_t RTC_WPR;     		//!< (rtc Offset: 0x24) RTC write protection register
  __IO uint32_t RTC_SSR; 	 		//!< (rtc Offset: 0x28) RTC sub second register
  __IO uint32_t RTC_SHIFTR;  		//!< (rtc Offset: 0x2C) RTC shift control register
  __IO uint32_t RTC_TSTR;    		//!< (rtc Offset: 0x30) RTC time stamp time register
  __IO uint32_t RTC_TSDR;    		//!< (rtc Offset: 0x34) RTC time stamp date register
  __IO uint32_t RTC_TSSSR;  		//!< (rtc Offset: 0x38) RTC time-stamp sub second register
  __IO uint32_t RTC_CALR;	  		//!< (rtc Offset: 0x3C) RTC calibration register
  __IO uint32_t RTC_TAFCR;   		//!< (rtc Offset: 0x40) RTC tamper and alternate function configuration register
  __IO uint32_t RTC_ALRMASSR;  		//!< (rtc Offset: 0x44) RTC alarm A sub second register
  __IO uint32_t reserved3[2];  		//!< (rtc Offset: 0x48) Reserved
  __IO uint32_t RTC_BKPxR[5]; 		//!< (rtc Offset: 0x50) RTC backup registers

} RTC_TypeDef;

/***************************************************************************//**
 * @relates RTC_TypeDef
 * @{
 ******************************************************************************/

/** @name RTC_TR;      	(rtc Offset: 0x00) RTC time register				  */
/** @{ */
#define RTC_TR_PM                   0x00400000 //!< PM: AM/PM notation
#define RTC_TR_HT                   0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_TR_HU                   0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_TR_MNT                  0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_TR_MNU                  0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_TR_ST                   0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_TR_SU                   0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @name RTC_DR;      	(rtc Offset: 0x04) RTC date register				  */
/** @{ */
#define RTC_DR_YT                   0x00F00000 //!< YT[3:0]: Year tens in BCD format
#define RTC_DR_YU                   0x000F0000 //!< YU[3:0]: Year units in BCD format
#define RTC_DR_WDU                  0x0000E000 //!< WDU[2:0]: Week day units (1-7)
#define RTC_DR_MT                   0x00001000 //!< MT: Month tens in BCD format
#define RTC_DR_MU                   0x00000F00 //!< MU: Month units in BCD format
#define RTC_DR_DT                   0x00000030 //!< DT[1:0]: Date tens in BCD format
#define RTC_DR_DU                   0x0000000F //!< DU[3:0]: Date units in BCD format
/** @} */

/** @name RTC_CR;      	(rtc Offset: 0x08) RTC control register				  */
/** @{ */
#define RTC_CR_COE                  0x00800000 //!< COE: Calibration output enable
#define RTC_CR_OSEL                 0x00600000 //!< OSEL[1:0]: Output selection
#define RTC_CR_OSEL_DIS             0x00000000 //!<  Output disabled
#define RTC_CR_OSEL_ALRMA           0x00200000 //!<  Alarm A output enabled
#define RTC_CR_POL                  0x00100000 //!< POL: Output polarity
#define RTC_CR_COSEL                0x00080000 //!< COSEL : Calibration output selection
#define RTC_CR_BKP                  0x00040000 //!< BKP: Backup
#define RTC_CR_SUB1H                0x00020000 //!< SUB1H: Subtract 1 hour (winter time change)
#define RTC_CR_ADD1H                0x00010000 //!< ADD1H: Add 1 hour (summer time change)
#define RTC_CR_TSIE                 0x00008000 //!< TSIE: Timestamp interrupt enable
#define RTC_CR_ALRAIE               0x00001000 //!< ALRAIE: Alarm A interrupt enable
#define RTC_CR_TSE                  0x00000800 //!< TSE: Time stamp enable
#define RTC_CR_ALRAE                0x00000100 //!< ALRAE: Alarm A enable
#define RTC_CR_FMT                  0x00000040 //!< FMT: Hour format
#define RTC_CR_BYPSHAD              0x00000020 //!< BYPSHAD: Bypass the shadow registers
#define RTC_CR_REFCKON              0x00000010 //!< REFCKON: Reference clock detection enable (50 or 60 Hz)
#define RTC_CR_TSEDGE               0x00000008 //!< TSEDGE: Timestamp event active edge
/** @} */

/** @name RTC_ISR;     	(rtc Offset: 0x0C) RTC initialization and status register */
/** @{ */
#define RTC_ISR_RECALPF             0x00010000 //!< RECALPF: Recalibration pending Flag
#define RTC_ISR_TAMP2F              0x00004000 //!< TAMP2F: TAMPER2 detection flag
#define RTC_ISR_TAMP1F              0x00002000 //!< TAMP1F: TAMPER1 detection flag
#define RTC_ISR_TSOVF               0x00001000 //!< TSOVF: Timestamp overflow flag
#define RTC_ISR_TSF                 0x00000800 //!< TSF: Timestamp flag
#define RTC_ISR_ALRAF               0x00000100 //!< ALRAF: Alarm A flag
#define RTC_ISR_INIT                0x00000080 //!< INIT: Initialization mode
#define RTC_ISR_INITF               0x00000040 //!< INITF: Initialization flag
#define RTC_ISR_RSF                 0x00000020 //!< RSF: Registers synchronization flag
#define RTC_ISR_INITS               0x00000010 //!< INITS: Initialization status flag
#define RTC_ISR_SHPF                0x00000008 //!< SHPF:Shift operation pending
#define RTC_ISR_ALRAWF              0x00000001 //!< ALRAWF: Alarm A write flag
/** @} */

/** @name RTC_PRER;    	(rtc Offset: 0x10) RTC prescaler register			  */
/** @{ */
#define RTC_PRER_PREDIV_A           0x007F0000 //!< PREDIV_A[6:0]: Asynchronous prescaler factor
#define RTC_PRER_PREDIV_S           0x00007FFF //!< PREDIV_S[14:0]: Synchronous prescaler factor
/** @} */

/** @name RTC_ALRMAR;  	(rtc Offset: 0x1C) RTC alarm A register				  */
/** @{ */
#define RTC_ALRMAR_MSK4             0x80000000u//!< MSK4: Alarm A date mask
#define RTC_ALRMAR_WDSEL            0x40000000 //!< WDSEL: Week day selection
#define RTC_ALRMAR_DT               0x30000000 //!< DT[1:0]: Date tens in BCD format
#define RTC_ALRMAR_DU               0x0F000000 //!< DU[3:0]: Date units or day in BCD format
#define RTC_ALRMAR_MSK3             0x00800000 //!< MSK3: Alarm A hours mask
#define RTC_ALRMAR_PM               0x00400000 //!< PM: AM/PM notation
#define RTC_ALRMAR_HT               0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_ALRMAR_HU               0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_ALRMAR_MSK2             0x00008000 //!< MSK2: Alarm A minutes mask
#define RTC_ALRMAR_MNT              0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_ALRMAR_MNU              0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_ALRMAR_MSK1             0x00000080 //!< MSK1: Alarm A seconds mask
#define RTC_ALRMAR_ST               0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_ALRMAR_SU               0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @name RTC_WPR;     	(rtc Offset: 0x24) RTC write protection register	  */
/** @{ */
#define RTC_WPR_KEY                 0x000000FF //!< KEY[7:0]: Write protection key
/** @} */

/** @name RTC_SSR; 	 	(rtc Offset: 0x28) RTC sub second register			  */
/** @{ */
#define RTC_SSR_SS                  0x0000FFFF //!< SS: Sub second value
/** @} */

/** @name RTC_SHIFTR;  	(rtc Offset: 0x2C) RTC shift control register		  */
/** @{ */
#define RTC_SHIFTR_ADD1S            0x80000000u//!< ADD1S: Add one second
#define RTC_SHIFTR_SUBFS            0x00007FFF //!< SUBFS: Subtract a fraction of a second
/** @} */

/** @name RTC_TSTR;    	(rtc Offset: 0x30) RTC time stamp time register 	  */
/** @{ */
#define RTC_TSTR_PM                 0x00400000 //!< PM: AM/PM notation
#define RTC_TSTR_HT                 0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_TSTR_HU                 0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_TSTR_MNT                0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_TSTR_MNU                0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_TSTR_ST                 0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_TSTR_SU                 0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @name RTC_TSDR;    	(rtc Offset: 0x34) RTC time stamp date register		  */
/** @{ */
#define RTC_TSDR_WDU                0x0000E000 //!< WDU[1:0]: Week day units
#define RTC_TSDR_MT                 0x00001000 //!< MT: Month tens in BCD format
#define RTC_TSDR_MU                 0x00000F00 //!< MU[3:0]: Month units in BCD format
#define RTC_TSDR_DT                 0x00000030 //!< DT[1:0]: Date tens in BCD format
#define RTC_TSDR_DU                 0x0000000F //!< DU[3:0]: Date units in BCD forma
/** @} */

/** @name RTC_TSSSR;  	(rtc Offset: 0x38) RTC time-stamp sub second register */
/** @{ */
#define RTC_TSSSR_SS                0x0000FFFF //!< SS: Sub second value
/** @} */

/** @name RTC_CALR;	  	(rtc Offset: 0x3C) RTC calibration register			  */
/** @{ */
#define RTC_CALR_CALP               0x00008000 //!< CALP: Increase frequency of RTC by 488.5 ppm
#define RTC_CALR_CALW8              0x00004000 //!< CALW8: Use an 8-second calibration cycle period
#define RTC_CALR_CALW16             0x00002000 //!< CALW16: Use a 16-second calibration cycle period
#define RTC_CALR_CALM               0x000001FF //!< CALM[8:0]: Calibration minus
/** @} */

/** @name RTC_TAFCR;   	(rtc Offset: 0x40) RTC tamper and alternate function configuration register */
/** @{ */
#define RTC_TAFCR_PC15MODE	        0x00800000 //!< PC15MODE: PC15 mode
#define RTC_TAFCR_PC15VALUE         0x00400000 //!< PC15VALUE: PC15 value
#define RTC_TAFCR_PC14MODE          0x00200000 //!< PC14MODE: PC14 mode
#define RTC_TAFCR_PC14VALUE         0x00100000 //!< PC14VALUE: PC14 value
#define RTC_TAFCR_PC13MODE          0x00080000 //!< PC13MODE: PC13 mode
#define RTC_TAFCR_PC13VALUE	        0x00040000 //!< PC13VALUE: RTC_ALARM output type/PC13 value
#define RTC_TAFCR_TAMPPUDIS         0x00008000 //!< TAMPPUDIS: TAMPER pull-up disable
#define RTC_TAFCR_TAMPPRCH          0x00006000 //!< TAMPPRCH[1:0]: Tamper precharge duration
#define RTC_TAFCR_TAMPFLT           0x00001800 //!< TAMPFLT[1:0]: Tamper filter count
#define RTC_TAFCR_TAMPFREQ          0x00000700 //!< TAMPFREQ[2:0]: Tamper sampling frequency
#define RTC_TAFCR_TAMPTS            0x00000080 //!< TAMPTS: Activate timestamp on tamper detection event
#define RTC_TAFCR_TAMP2TRG          0x00000010 //!< TAMP2TRG: Active level for tamper 2
#define RTC_TAFCR_TAMP2E            0x00000008 //!< TAMP2E: Tamper 2 detection enable
#define RTC_TAFCR_TAMPIE            0x00000004 //!< TAMPIE: Tamper interrupt enable
#define RTC_TAFCR_TAMP1TRG          0x00000002 //!< TAMP1TRG: Active level for tamper 1
#define RTC_TAFCR_TAMP1E            0x00000001 //!< TAMP1E: Tamper 1 detection enable
/** @} */

/** @name RTC_ALRMASSR; (rtc Offset: 0x44) RTC alarm A sub second register	  */
/** @{ */
#define RTC_ALRMASSR_MASKSS         0x0F000000 //!< MASKSS[3:0]: Mask the most-significant bits starting at this bit
#define RTC_ALRMASSR_SS             0x00007FFF //!< SS[14:0]: Sub seconds value
/** @} */


/** @} */ // @relates RTC_TypeDef



#endif /* RTC_F0_H_ */

/** @} */
