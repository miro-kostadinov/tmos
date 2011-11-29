/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
/**************************************************************************//**
 * @ingroup	 hardware_sam3s
 * @defgroup HW_SAM3S_RTC Power Management Controller
 * @{
 *  \section rtc_module Working with RTC
 * The RTC driver provides the interface to configure and use the RTC
 * peripheral.
 *
 * It manages date, time, and alarms.\n
 * This timer is clocked by the 32kHz system clock, and is not impacted by
 * power management settings (PMC). To be accurate, it is better to use an
 * external 32kHz crystal instead of the internal 32kHz RC.\n
 *
 * It uses BCD format, and time can be set in AM/PM or 24h mode through a
 * configuration bit in the mode register.\n
 *
 * To update date or time, the user has to follow these few steps :
 * <ul>
 * <li>Set UPDTIM and/or UPDCAL bit(s) in RTC_CR,</li>
 * <li>Polling or IRQ on the ACKUPD bit of RTC_CR,</li>
 * <li>Clear ACKUPD bit in RTC_SCCR,</li>
 * <li>Update Time and/or Calendar values in RTC_TIMR/RTC_CALR (BCD format),</li>
 * <li>Clear UPDTIM and/or UPDCAL bit in RTC_CR.</li>
 * </ul>
 * An alarm can be set to happen on month, date, hours, minutes or seconds,
 * by setting the proper "Enable" bit of each of these fields in the Time and
 * Calendar registers.
 * This allows a large number of configurations to be available for the user.
 * Alarm occurrence can be detected even by polling or interrupt.
 *
 * A check of the validity of the date and time format and values written by the
 * user is automatically done. Errors are reported through the Valid Entry
 * Register.
 *
 * For more accurate information, please look at the RTC section of the
 * Datasheet.
 *
 * @file     hardware/at91/cortex-m3/rtc.h
 * @ingroup	 HW_SAM3S_RTC
 * @brief    RTC header
 *
 * @}
 */

#ifndef _RTC_
#define _RTC_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"
//#include <stdint.h>
#include <at91_types.h>
#include <tmos_cfg.h>

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

#define RTC_HOUR_BIT_LEN_MASK   0x3F //!< doc!
#define RTC_MIN_BIT_LEN_MASK    0x7F //!< doc!
#define RTC_SEC_BIT_LEN_MASK    0x7F //!< doc!
#define RTC_CENT_BIT_LEN_MASK   0x7F //!< doc!
#define RTC_YEAR_BIT_LEN_MASK   0xFF //!< doc!
#define RTC_MONTH_BIT_LEN_MASK  0x1F //!< doc!
#define RTC_DATE_BIT_LEN_MASK   0x3F //!< doc!
#define RTC_WEEK_BIT_LEN_MASK   0x07 //!< doc!

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif


/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Real-time Clock */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Rtc hardware registers */
typedef struct {
  RwReg RTC_CR;     //!< (Rtc Offset: 0x00) Control Register */
  RwReg RTC_MR;     //!< (Rtc Offset: 0x04) Mode Register */
  RwReg RTC_TIMR;   //!< (Rtc Offset: 0x08) Time Register */
  RwReg RTC_CALR;   //!< (Rtc Offset: 0x0C) Calendar Register */
  RwReg RTC_TIMALR; //!< (Rtc Offset: 0x10) Time Alarm Register */
  RwReg RTC_CALALR; //!< (Rtc Offset: 0x14) Calendar Alarm Register */
  RoReg RTC_SR;     //!< (Rtc Offset: 0x18) Status Register */
  WoReg RTC_SCCR;   //!< (Rtc Offset: 0x1C) Status Clear Command Register */
  WoReg RTC_IER;    //!< (Rtc Offset: 0x20) Interrupt Enable Register */
  WoReg RTC_IDR;    //!< (Rtc Offset: 0x24) Interrupt Disable Register */
  RoReg RTC_IMR;    //!< (Rtc Offset: 0x28) Interrupt Mask Register */
  RoReg RTC_VER;    //!< (Rtc Offset: 0x2C) Valid Entry Register */
} Rtc;
#endif /* __ASSEMBLY__ */

/* -------- RTC_CR : (RTC Offset: 0x00) Control Register -------- */
#define RTC_CR_UPDTIM (0x1u << 0) //!< (RTC_CR) Update Request Time Register */
#define RTC_CR_UPDCAL (0x1u << 1) //!< (RTC_CR) Update Request Calendar Register */
#define RTC_CR_TIMEVSEL_Pos 8 //!< doc!
#define RTC_CR_TIMEVSEL_Msk (0x3u << RTC_CR_TIMEVSEL_Pos) //!< (RTC_CR) Time Event Selection */
#define   RTC_CR_TIMEVSEL_MINUTE (0x0u << 8) //!< (RTC_CR) Minute change */
#define   RTC_CR_TIMEVSEL_HOUR (0x1u << 8) //!< (RTC_CR) Hour change */
#define   RTC_CR_TIMEVSEL_MIDNIGHT (0x2u << 8) //!< (RTC_CR) Every day at midnight */
#define   RTC_CR_TIMEVSEL_NOON (0x3u << 8) //!< (RTC_CR) Every day at noon */
#define RTC_CR_CALEVSEL_Pos 16 //!< doc!
#define RTC_CR_CALEVSEL_Msk (0x3u << RTC_CR_CALEVSEL_Pos) //!< (RTC_CR) Calendar Event Selection */
#define   RTC_CR_CALEVSEL_WEEK (0x0u << 16) //!< (RTC_CR) Week change (every Monday at time 00:00:00) */
#define   RTC_CR_CALEVSEL_MONTH (0x1u << 16) //!< (RTC_CR) Month change (every 01 of each month at time 00:00:00) */
#define   RTC_CR_CALEVSEL_YEAR (0x2u << 16) //!< (RTC_CR) Year change (every January 1 at time 00:00:00) */
/* -------- RTC_MR : (RTC Offset: 0x04) Mode Register -------- */
#define RTC_MR_HRMOD (0x1u << 0) //!< (RTC_MR) 12-/24-hour Mode */
/* -------- RTC_TIMR : (RTC Offset: 0x08) Time Register -------- */
#define RTC_TIMR_SEC_Pos 0 //!< doc!
#define RTC_TIMR_SEC_Msk (0x7fu << RTC_TIMR_SEC_Pos) //!< (RTC_TIMR) Current Second */
#define RTC_TIMR_SEC(value) ((RTC_TIMR_SEC_Msk & ((value) << RTC_TIMR_SEC_Pos))) //!< doc!
#define RTC_TIMR_MIN_Pos 8 //!< doc!
#define RTC_TIMR_MIN_Msk (0x7fu << RTC_TIMR_MIN_Pos) //!< (RTC_TIMR) Current Minute */
#define RTC_TIMR_MIN(value) ((RTC_TIMR_MIN_Msk & ((value) << RTC_TIMR_MIN_Pos))) //!< doc!
#define RTC_TIMR_HOUR_Pos 16 //!< doc!
#define RTC_TIMR_HOUR_Msk (0x3fu << RTC_TIMR_HOUR_Pos) //!< (RTC_TIMR) Current Hour */
#define RTC_TIMR_HOUR(value) ((RTC_TIMR_HOUR_Msk & ((value) << RTC_TIMR_HOUR_Pos))) //!< doc!
#define RTC_TIMR_AMPM (0x1u << 22) //!< (RTC_TIMR) Ante Meridiem Post Meridiem Indicator */
/* -------- RTC_CALR : (RTC Offset: 0x0C) Calendar Register -------- */
#define RTC_CALR_CENT_Pos 0 //!< doc!
#define RTC_CALR_CENT_Msk (0x7fu << RTC_CALR_CENT_Pos) //!< (RTC_CALR) Current Century */
#define RTC_CALR_CENT(value) ((RTC_CALR_CENT_Msk & ((value) << RTC_CALR_CENT_Pos))) //!< doc!
#define RTC_CALR_YEAR_Pos 8 //!< doc!
#define RTC_CALR_YEAR_Msk (0xffu << RTC_CALR_YEAR_Pos) //!< (RTC_CALR) Current Year */
#define RTC_CALR_YEAR(value) ((RTC_CALR_YEAR_Msk & ((value) << RTC_CALR_YEAR_Pos))) //!< doc!
#define RTC_CALR_MONTH_Pos 16 //!< doc!
#define RTC_CALR_MONTH_Msk (0x1fu << RTC_CALR_MONTH_Pos) //!< (RTC_CALR) Current Month */
#define RTC_CALR_MONTH(value) ((RTC_CALR_MONTH_Msk & ((value) << RTC_CALR_MONTH_Pos))) //!< doc!
#define RTC_CALR_DAY_Pos 21 //!< doc!
#define RTC_CALR_DAY_Msk (0x7u << RTC_CALR_DAY_Pos) //!< (RTC_CALR) Current Day in Current Week */
#define RTC_CALR_DAY(value) ((RTC_CALR_DAY_Msk & ((value) << RTC_CALR_DAY_Pos))) //!< doc!
#define RTC_CALR_DATE_Pos 24 //!< doc!
#define RTC_CALR_DATE_Msk (0x3fu << RTC_CALR_DATE_Pos) //!< (RTC_CALR) Current Day in Current Month */
#define RTC_CALR_DATE(value) ((RTC_CALR_DATE_Msk & ((value) << RTC_CALR_DATE_Pos))) //!< doc!
/* -------- RTC_TIMALR : (RTC Offset: 0x10) Time Alarm Register -------- */
#define RTC_TIMALR_SEC_Pos 0 //!< doc!
#define RTC_TIMALR_SEC_Msk (0x7fu << RTC_TIMALR_SEC_Pos) //!< (RTC_TIMALR) Second Alarm */
#define RTC_TIMALR_SEC(value) ((RTC_TIMALR_SEC_Msk & ((value) << RTC_TIMALR_SEC_Pos))) //!< doc!
#define RTC_TIMALR_SECEN (0x1u << 7) //!< (RTC_TIMALR) Second Alarm Enable */
#define RTC_TIMALR_MIN_Pos 8 //!< doc!
#define RTC_TIMALR_MIN_Msk (0x7fu << RTC_TIMALR_MIN_Pos) //!< (RTC_TIMALR) Minute Alarm */
#define RTC_TIMALR_MIN(value) ((RTC_TIMALR_MIN_Msk & ((value) << RTC_TIMALR_MIN_Pos))) //!< doc!
#define RTC_TIMALR_MINEN (0x1u << 15) //!< (RTC_TIMALR) Minute Alarm Enable */
#define RTC_TIMALR_HOUR_Pos 16 //!< doc!
#define RTC_TIMALR_HOUR_Msk (0x3fu << RTC_TIMALR_HOUR_Pos) //!< (RTC_TIMALR) Hour Alarm */
#define RTC_TIMALR_HOUR(value) ((RTC_TIMALR_HOUR_Msk & ((value) << RTC_TIMALR_HOUR_Pos))) //!< doc!
#define RTC_TIMALR_AMPM (0x1u << 22) //!< (RTC_TIMALR) AM/PM Indicator */
#define RTC_TIMALR_HOUREN (0x1u << 23) //!< (RTC_TIMALR) Hour Alarm Enable */
/* -------- RTC_CALALR : (RTC Offset: 0x14) Calendar Alarm Register -------- */
#define RTC_CALALR_MONTH_Pos 16 //!< doc!
#define RTC_CALALR_MONTH_Msk (0x1fu << RTC_CALALR_MONTH_Pos) //!< (RTC_CALALR) Month Alarm */
#define RTC_CALALR_MONTH(value) ((RTC_CALALR_MONTH_Msk & ((value) << RTC_CALALR_MONTH_Pos))) //!< doc!
#define RTC_CALALR_MTHEN (0x1u << 23) //!< (RTC_CALALR) Month Alarm Enable */
#define RTC_CALALR_DATE_Pos 24 //!< doc!
#define RTC_CALALR_DATE_Msk (0x3fu << RTC_CALALR_DATE_Pos) //!< (RTC_CALALR) Date Alarm */
#define RTC_CALALR_DATE(value) ((RTC_CALALR_DATE_Msk & ((value) << RTC_CALALR_DATE_Pos))) //!< doc!
#define RTC_CALALR_DATEEN (0x1u << 31) //!< (RTC_CALALR) Date Alarm Enable */
/* -------- RTC_SR : (RTC Offset: 0x18) Status Register -------- */
#define RTC_SR_ACKUPD (0x1u << 0) //!< (RTC_SR) Acknowledge for Update */
#define RTC_SR_ALARM (0x1u << 1) //!< (RTC_SR) Alarm Flag */
#define RTC_SR_SEC (0x1u << 2) //!< (RTC_SR) Second Event */
#define RTC_SR_TIMEV (0x1u << 3) //!< (RTC_SR) Time Event */
#define RTC_SR_CALEV (0x1u << 4) //!< (RTC_SR) Calendar Event */
/* -------- RTC_SCCR : (RTC Offset: 0x1C) Status Clear Command Register -------- */
#define RTC_SCCR_ACKCLR (0x1u << 0) //!< (RTC_SCCR) Acknowledge Clear */
#define RTC_SCCR_ALRCLR (0x1u << 1) //!< (RTC_SCCR) Alarm Clear */
#define RTC_SCCR_SECCLR (0x1u << 2) //!< (RTC_SCCR) Second Clear */
#define RTC_SCCR_TIMCLR (0x1u << 3) //!< (RTC_SCCR) Time Clear */
#define RTC_SCCR_CALCLR (0x1u << 4) //!< (RTC_SCCR) Calendar Clear */
/* -------- RTC_IER : (RTC Offset: 0x20) Interrupt Enable Register -------- */
#define RTC_IER_ACKEN (0x1u << 0) //!< (RTC_IER) Acknowledge Update Interrupt Enable */
#define RTC_IER_ALREN (0x1u << 1) //!< (RTC_IER) Alarm Interrupt Enable */
#define RTC_IER_SECEN (0x1u << 2) //!< (RTC_IER) Second Event Interrupt Enable */
#define RTC_IER_TIMEN (0x1u << 3) //!< (RTC_IER) Time Event Interrupt Enable */
#define RTC_IER_CALEN (0x1u << 4) //!< (RTC_IER) Calendar Event Interrupt Enable */
/* -------- RTC_IDR : (RTC Offset: 0x24) Interrupt Disable Register -------- */
#define RTC_IDR_ACKDIS (0x1u << 0) //!< (RTC_IDR) Acknowledge Update Interrupt Disable */
#define RTC_IDR_ALRDIS (0x1u << 1) //!< (RTC_IDR) Alarm Interrupt Disable */
#define RTC_IDR_SECDIS (0x1u << 2) //!< (RTC_IDR) Second Event Interrupt Disable */
#define RTC_IDR_TIMDIS (0x1u << 3) //!< (RTC_IDR) Time Event Interrupt Disable */
#define RTC_IDR_CALDIS (0x1u << 4) //!< (RTC_IDR) Calendar Event Interrupt Disable */
/* -------- RTC_IMR : (RTC Offset: 0x28) Interrupt Mask Register -------- */
#define RTC_IMR_ACK (0x1u << 0) //!< (RTC_IMR) Acknowledge Update Interrupt Mask */
#define RTC_IMR_ALR (0x1u << 1) //!< (RTC_IMR) Alarm Interrupt Mask */
#define RTC_IMR_SEC (0x1u << 2) //!< (RTC_IMR) Second Event Interrupt Mask */
#define RTC_IMR_TIM (0x1u << 3) //!< (RTC_IMR) Time Event Interrupt Mask */
#define RTC_IMR_CAL (0x1u << 4) //!< (RTC_IMR) Calendar Event Interrupt Mask */
/* -------- RTC_VER : (RTC Offset: 0x2C) Valid Entry Register -------- */
#define RTC_VER_NVTIM (0x1u << 0) //!< (RTC_VER) Non-valid Time */
#define RTC_VER_NVCAL (0x1u << 1) //!< (RTC_VER) Non-valid Calendar */
#define RTC_VER_NVTIMALR (0x1u << 2) //!< (RTC_VER) Non-valid Time Alarm */
#define RTC_VER_NVCALALR (0x1u << 3) //!< (RTC_VER) Non-valid Calendar Alarm */



extern void RTC_SetHourMode( Rtc* pRtc, uint32_t dwMode ) ;

extern uint32_t RTC_GetHourMode( Rtc* pRtc ) ;

extern void RTC_EnableIt( Rtc* pRtc, uint32_t dwSources ) ;

extern void RTC_DisableIt( Rtc* pRtc, uint32_t dwSources ) ;

extern int RTC_SetTime( Rtc* pRtc, uint8_t ucHour, uint8_t ucMinute, uint8_t ucSecond ) ;

extern void RTC_GetTime( Rtc* pRtc, uint8_t *pucHour, uint8_t *pucMinute, uint8_t *pucSecond ) ;

extern int RTC_SetTimeAlarm( Rtc* pRtc, uint8_t *pucHour, uint8_t *pucMinute, uint8_t *pucSecond ) ;

extern void RTC_GetDate( Rtc* pRtc, uint16_t *pwYear, uint8_t *pucMonth, uint8_t *pucDay, uint8_t *pucWeek ) ;

extern int RTC_SetDate( Rtc* pRtc, uint16_t wYear, uint8_t ucMonth, uint8_t ucDay, uint8_t ucWeek ) ;

extern int RTC_SetDateAlarm( Rtc* pRtc, uint8_t *pucMonth, uint8_t *pucDay ) ;

extern void RTC_ClearSCCR( Rtc* pRtc, uint32_t dwMask ) ;

extern uint32_t RTC_GetSR( Rtc* pRtc, uint32_t dwMask ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _RTC_ */

