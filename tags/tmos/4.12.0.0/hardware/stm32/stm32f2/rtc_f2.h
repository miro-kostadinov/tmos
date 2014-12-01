/*
 * rtc_f2.h
 *
 *  Created on: Feb 13, 2013
 *      Author: miro
 */

#ifndef RTC_F2_H_
#define RTC_F2_H_

#include <brd_cfg.h>
#include <mcu_inc.h>
#include <tmos_types.h>
#if USE_TIME
#include <tmos_time.h>
#endif

/*******************************************************************************
 *  Real-Time Clock
 ******************************************************************************/
typedef struct
{
  __IO uint32_t RTC_TR;      		//!< (rtc Offset: 0x00) RTC time register
  __IO uint32_t RTC_DR;      		//!< (rtc Offset: 0x04) RTC date register
  __IO uint32_t RTC_CR;      		//!< (rtc Offset: 0x08) RTC control register
  __IO uint32_t RTC_ISR;     		//!< (rtc Offset: 0x0C) RTC initialization and status register
  __IO uint32_t RTC_PRER;    		//!< (rtc Offset: 0x10) RTC prescaler register
  __IO uint32_t RTC_WUTR;    		//!< (rtc Offset: 0x14) RTC wakeup timer register
  __IO uint32_t RTC_CALIBR;  		//!< (rtc Offset: 0x18) RTC calibration register
  __IO uint32_t RTC_ALRMAR;  		//!< (rtc Offset: 0x1C) RTC alarm A register
  __IO uint32_t RTC_ALRMBR;  		//!< (rtc Offset: 0x20) RTC alarm B register
  __IO uint32_t RTC_WPR;     		//!< (rtc Offset: 0x24) RTC write protection register
  __IO uint32_t reserved1[2];  		//!< (rtc Offset: 0x28) Reserved
  __IO uint32_t RTC_TSTR;    		//!< (rtc Offset: 0x30) RTC time stamp time register
  __IO uint32_t RTC_TSDR;    		//!< (rtc Offset: 0x34) RTC time stamp date register
  __IO uint32_t reserved2[2];  		//!< (rtc Offset: 0x38) Reserved
  __IO uint32_t RTC_TAFCR;   		//!< (rtc Offset: 0x40) RTC tamper and alternate function configuration register
  __IO uint32_t reserved3[3];  		//!< (rtc Offset: 0x44) Reserved
  __IO uint32_t RTC_BKPxR[20]; 		//!< (rtc Offset: 0x50) RTC backup registers

} RTC_TypeDef;

/*******************************************************************************
 * @defgroup RTC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup RTC_TR:       (rtc Offset: 0x00) RTC time register			  */
#define RTC_TR_PM                   0x00400000 //!< PM: AM/PM notation
#define RTC_TR_HT                   0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_TR_HU                   0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_TR_MNT                  0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_TR_MNU                  0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_TR_ST                   0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_TR_SU                   0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @defgroup RTC_DR:      	(rtc Offset: 0x04) RTC date register			  */
#define RTC_DR_YT                   0x00F00000 //!< YT[3:0]: Year tens in BCD format
#define RTC_DR_YU                   0x000F0000 //!< YU[3:0]: Year units in BCD format
#define RTC_DR_WDU                  0x0000E000 //!< WDU[2:0]: Week day units (1-7)
#define RTC_DR_MT                   0x00001000 //!< MT: Month tens in BCD format
#define RTC_DR_MU                   0x00000F00 //!< MU: Month units in BCD format
#define RTC_DR_DT                   0x00000030 //!< DT[1:0]: Date tens in BCD format
#define RTC_DR_DU                   0x0000000F //!< DU[3:0]: Date units in BCD format
/** @} */

/** @defgroup RTC_CR:      	(rtc Offset: 0x08) RTC control register			  */
#define RTC_CR_COE                  0x00800000 //!< COE: Calibration output enable
#define RTC_CR_OSEL                 0x00600000 //!< OSEL[1:0]: Output selection
#define RTC_CR_OSEL_DIS             0x00000000 //!<  Output disabled
#define RTC_CR_OSEL_ALRMA           0x00200000 //!<  Alarm A output enabled
#define RTC_CR_OSEL_ALRMB           0x00400000 //!<  Alarm B output enabled
#define RTC_CR_OSEL_WKUP            0x00600000 //!<  Wakeup output enabled
#define RTC_CR_POL                  0x00100000 //!< POL: Output polarity
#define RTC_CR_CALSEL               0x00080000 //!<
#define RTC_CR_BKP                  0x00040000 //!< BKP: Backup
#define RTC_CR_SUB1H                0x00020000 //!< SUB1H: Subtract 1 hour (winter time change)
#define RTC_CR_ADD1H                0x00010000 //!< ADD1H: Add 1 hour (summer time change)
#define RTC_CR_TSIE                 0x00008000 //!< TSIE: Timestamp interrupt enable
#define RTC_CR_WUTIE                0x00004000 //!< WUTIE: Wakeup timer interrupt enable
#define RTC_CR_ALRBIE               0x00002000 //!< ALRBIE: Alarm B interrupt enable
#define RTC_CR_ALRAIE               0x00001000 //!< ALRAIE: Alarm A interrupt enable
#define RTC_CR_TSE                  0x00000800 //!< TSE: Time stamp enable
#define RTC_CR_WUTE                 0x00000400 //!< WUTE: Wakeup timer enable
#define RTC_CR_ALRBE                0x00000200 //!< ALRBE: Alarm B enable
#define RTC_CR_ALRAE                0x00000100 //!< ALRAE: Alarm A enable
#define RTC_CR_DCE                  0x00000080 //!< DCE: Coarse digital calibration enable
#define RTC_CR_FMT                  0x00000040 //!< FMT: Hour format
#define RTC_CR_BYPSHAD              0x00000020 //!<
#define RTC_CR_REFCKON              0x00000010 //!< REFCKON: Reference clock detection enable (50 or 60 Hz)
#define RTC_CR_TSEDGE               0x00000008 //!< TSEDGE: Timestamp event active edge
#define RTC_CR_WUCKSEL              0x00000007 //!< WUCKSEL[2:0]: Wakeup clock selection
#define RTC_CR_WUCKSEL_RTC_16       0x00000000 //!<  RTC/16 clock is selected
#define RTC_CR_WUCKSEL_RTC_8        0x00000001 //!<  RTC/8 clock is selected
#define RTC_CR_WUCKSEL_RTC_4        0x00000002 //!<  RTC/4 clock is selected
#define RTC_CR_WUCKSEL_RTC_2        0x00000003 //!<  RTC/2 clock is selected
#define RTC_CR_WUCKSEL_CK_SPRE      0x00000004 //!<  ck_spre (usually 1 Hz) clock is selected
#define RTC_CR_WUCKSEL_CK_SPRE2     0x00000006 //!<  ck_spre (usually 1 Hz) clock is selected and 216 is added to the WUT counter value

/** @} */

/** @defgroup RTC_ISR:     	(rtc Offset: 0x0C) RTC initialization and status register */
#define RTC_ISR_RECALPF             0x00010000 //!<
#define RTC_ISR_TAMP2F              0x00004000 //!<
#define RTC_ISR_TAMP1F              0x00002000 //!< TAMP1F: Tamper detection flag
#define RTC_ISR_TSOVF               0x00001000 //!< TSOVF: Timestamp overflow flag
#define RTC_ISR_TSF                 0x00000800 //!< TSF: Timestamp flag
#define RTC_ISR_WUTF                0x00000400 //!< WUTF: Wakeup timer flag
#define RTC_ISR_ALRBF               0x00000200 //!< ALRBF: Alarm B flag
#define RTC_ISR_ALRAF               0x00000100 //!< ALRAF: Alarm A flag
#define RTC_ISR_INIT                0x00000080 //!< INIT: Initialization mode
#define RTC_ISR_INITF               0x00000040 //!< INITF: Initialization flag
#define RTC_ISR_RSF                 0x00000020 //!< RSF: Registers synchronization flag
#define RTC_ISR_INITS               0x00000010 //!< INITS: Initialization status flag
#define RTC_ISR_SHPF                0x00000008 //!<
#define RTC_ISR_WUTWF               0x00000004 //!< WUTWF: Wakeup timer write flag
#define RTC_ISR_ALRBWF              0x00000002 //!< ALRBWF: Alarm B write flag
#define RTC_ISR_ALRAWF              0x00000001 //!< ALRAWF: Alarm A write flag
/** @} */

/** @defgroup RTC_PRER:    	(rtc Offset: 0x10) RTC prescaler register 		  */
#define RTC_PRER_PREDIV_A           0x007F0000 //!< PREDIV_A[6:0]: Asynchronous prescaler factor
#define RTC_PRER_PREDIV_A_Set(x)    ((x)<<16)
#define RTC_PRER_PREDIV_S           0x00001FFF //!< PREDIV_S[12:0]: Synchronous prescaler factor
#define RTC_PRER_PREDIV_S_Set(x)    (x)
/** @} */

/** @defgroup RTC_WUTR:    	(rtc Offset: 0x14) RTC wakeup timer register	  */
#define RTC_WUTR_WUT	            0x0000FFFF //!< WUT[15:0]: Wakeup auto-reload value bits
/** @} */

/** @defgroup RTC_CALIBR:  	(rtc Offset: 0x18) RTC calibration register		  */
#define RTC_CALIBR_DCS              0x00000080 //!< DCS: Digital calibration sign
#define RTC_CALIBR_DC               0x0000001F //!< DC[4:0]: Digital calibration
/** @} */

/** @defgroup RTC_ALRMAR:  	(rtc Offset: 0x1C) RTC alarm A register			  */
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

/** @defgroup RTC_ALRMBR:  	(rtc Offset: 0x20) RTC alarm B register			  */
#define RTC_ALRMBR_MSK4             0x80000000u//!< MSK4: Alarm B date mask
#define RTC_ALRMBR_WDSEL            0x40000000 //!< WDSEL: Week day selection
#define RTC_ALRMBR_DT               0x30000000 //!< DT[1:0]: Date tens in BCD format
#define RTC_ALRMBR_DU               0x0F000000 //!< DU[3:0]: Date units or day in BCD format
#define RTC_ALRMBR_MSK3             0x00800000 //!< MSK3: Alarm B hours mask
#define RTC_ALRMBR_PM               0x00400000 //!< PM: AM/PM notation
#define RTC_ALRMBR_HT               0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_ALRMBR_HU               0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_ALRMBR_MSK2             0x00008000 //!< MSK2: Alarm B minutes mask
#define RTC_ALRMBR_MNT              0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_ALRMBR_MNU              0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_ALRMBR_MSK1             0x00000080 //!< MSK1: Alarm B seconds mask
#define RTC_ALRMBR_ST               0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_ALRMBR_SU               0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @defgroup RTC_WPR:     	(rtc Offset: 0x24) RTC write protection register  */
#define RTC_WPR_KEY                 0x000000FF //!< KEY[7:0]: Write protection key
/** @} */

/** @defgroup RTC_TSTR:    	(rtc Offset: 0x30) RTC time stamp time register	  */
#define RTC_TSTR_PM                 0x00400000 //!< PM: AM/PM notation
#define RTC_TSTR_HT                 0x00300000 //!< HT[1:0]: Hour tens in BCD format
#define RTC_TSTR_HU                 0x000F0000 //!< HU[3:0]: Hour units in BCD format
#define RTC_TSTR_MNT                0x00007000 //!< MNT[2:0]: Minute tens in BCD format
#define RTC_TSTR_MNU                0x00000F00 //!< MNU[3:0]: Minute units in BCD format
#define RTC_TSTR_ST                 0x00000070 //!< ST[2:0]: Second tens in BCD format
#define RTC_TSTR_SU                 0x0000000F //!< SU[3:0]: Second units in BCD format
/** @} */

/** @defgroup RTC_TSDR:    	(rtc Offset: 0x34) RTC time stamp date register	  */
#define RTC_TSDR_WDU                0x0000E000 //!< WDU[1:0]: Week day units
#define RTC_TSDR_MT                 0x00001000 //!< MT: Month tens in BCD format
#define RTC_TSDR_MU                 0x00000F00 //!< MU[3:0]: Month units in BCD format
#define RTC_TSDR_DT                 0x00000030 //!< DT[1:0]: Date tens in BCD format
#define RTC_TSDR_DU                 0x0000000F //!< DU[3:0]: Date units in BCD forma
/** @} */

/** @defgroup RTC_TAFCR:   	(rtc Offset: 0x40) RTC tamper and alternate function configuration register */
#define RTC_TAFCR_ALARMOUTTYPE      0x00040000 //!< ALARMOUTTYPE: AFO_ALARM output type
#define RTC_TAFCR_TSINSEL	        0x00020000 //!< TSINSEL: TIMESTAMP mapping
#define RTC_TAFCR_TAMP1INSEL        0x00010000 //!< TAMP1INSEL: TAMPER1 mapping
#define RTC_TAFCR_TAMPIE            0x00000004 //!< TAMPIE: Tamper interrupt enable
#define RTC_TAFCR_TAMP1TRG          0x00000002 //!< TAMP1TRG: Active level for tamper 1
#define RTC_TAFCR_TAMP1E            0x00000001 //!< TAMP1E: Tamper 1 detection enable
/** @} */



/** @} */ // @defgroup RTC_regs_define

bool rtc_wait_sync(RTC_TypeDef* rtc);
bool rtc_enter_init_mode(RTC_TypeDef* rtc);
void rtc_exit_init_mode(RTC_TypeDef* rtc);
#if USE_TIME
void rtc_get_time(RTC_TypeDef* rtc, time_t* time);
bool rtc_set_time(RTC_TypeDef* rtc, const time_t* time);
#endif
bool rtc_set_wut(RTC_TypeDef* rtc, uint32_t period);

#endif /* RTC_F2_H_ */
