/*
 * rtc_f1.h
 *
 *  Created on: Feb 13, 2013
 *      Author: miro
 */

#ifndef RTC_F1_H_
#define RTC_F1_H_

#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Real-Time Clock
 ******************************************************************************/
typedef struct
{
  __IO uint32_t RTC_CRH;			//!< (rtc Offset: 0x00) RTC control register high
  __IO uint32_t RTC_CRL;            //!< (rtc Offset: 0x04) RTC control register low
  __IO uint32_t RTC_PRLH;           //!< (rtc Offset: 0x08) RTC prescaler load register high
  __IO uint32_t RTC_PRLL;           //!< (rtc Offset: 0x0C) RTC prescaler load register low
  __IO uint32_t RTC_DIVH;           //!< (rtc Offset: 0x10) RTC prescaler divider register high
  __IO uint32_t RTC_DIVL;           //!< (rtc Offset: 0x14) RTC prescaler divider register low
  __IO uint32_t RTC_CNTH;           //!< (rtc Offset: 0x18) RTC counter register high
  __IO uint32_t RTC_CNTL;           //!< (rtc Offset: 0x1C) RTC counter register low
  __IO uint32_t RTC_ALRH;           //!< (rtc Offset: 0x20) RTC alarm register high
  __IO uint32_t RTC_ALRL;           //!< (rtc Offset: 0x24) RTC alarm register low
} RTC_TypeDef;

/*******************************************************************************
 * @defgroup RTC_regs_define
 * @{
 ******************************************************************************/

/** @defgroup RTC_CRH:		(rtc Offset: 0x00) RTC control register high	  */
#define  RTC_CRH_OWIE               0x0004 //!< OverfloW Interrupt Enable
#define  RTC_CRH_ALRIE              0x0002 //!< Alarm Interrupt Enable
#define  RTC_CRH_SECIE              0x0001 //!< Second Interrupt Enable
/** @} */

/** @defgroup RTC_CRL:      (rtc Offset: 0x04) RTC control register low		  */
#define  RTC_CRL_RTOFF              0x0020 //!< RTC operation OFF
#define  RTC_CRL_CNF                0x0010 //!< Configuration Flag
#define  RTC_CRL_RSF                0x0008 //!< Registers Synchronized Flag
#define  RTC_CRL_OWF                0x0004 //!< OverfloW Flag
#define  RTC_CRL_ALRF               0x0002 //!< Alarm Flag
#define  RTC_CRL_SECF               0x0001 //!< Second Flag
/** @} */

/** @defgroup RTC_PRLH:     (rtc Offset: 0x08) RTC prescaler load register high */
#define  RTC_PRLH_PRL               0x000F //!< RTC Prescaler Reload Value High
/** @} */

/** @defgroup RTC_PRLL:     (rtc Offset: 0x0C) RTC prescaler load register low */
#define  RTC_PRLL_PRL               0xFFFF //!< RTC Prescaler Reload Value Low
/** @} */

/** @defgroup RTC_DIVH:     (rtc Offset: 0x10) RTC prescaler divider register high */
#define  RTC_DIVH_RTC_DIV           0x000F //!< RTC Clock Divider High
/** @} */

/** @defgroup RTC_DIVL:     (rtc Offset: 0x14) RTC prescaler divider register low */
#define  RTC_DIVL_RTC_DIV           0xFFFF //!< RTC Clock Divider Low
/** @} */

/** @defgroup RTC_CNTH:     (rtc Offset: 0x18) RTC counter register high	  */
#define  RTC_CNTH_RTC_CNT           0xFFFF //!< RTC Counter High
/** @} */

/** @defgroup RTC_CNTL:     (rtc Offset: 0x1C) RTC counter register low		  */
#define  RTC_CNTL_RTC_CNT           0xFFFF //!< RTC Counter Low
/** @} */

/** @defgroup RTC_ALRH:     (rtc Offset: 0x20) RTC alarm register high		  */
#define  RTC_ALRH_RTC_ALR           0xFFFF //!< RTC Alarm High
/** @} */

/** @defgroup RTC_ALRL:     (rtc Offset: 0x24) RTC alarm register low		  */
#define  RTC_ALRL_RTC_ALR           0xFFFF //!< RTC Alarm Low
/** @} */


/** @} */ // @defgroup RTC_regs_define


bool rtc_enter_init_mode(RTC_TypeDef* rtc);


#endif /* RTC_F1_H_ */
