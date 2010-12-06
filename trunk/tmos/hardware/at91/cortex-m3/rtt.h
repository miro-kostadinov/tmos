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
 * @defgroup HW_SAM3S_RTT Real Time Timer Controller
 * Interface for Real Time Timer (RTT) controller.
 * @{
 *
 * \section Usage
 *
 * -# Changes the prescaler value of the given RTT and restarts it
 *    using \ref RTT_SetPrescaler().
 * -# Get current value of the RTT using \ref RTT_GetTime().
 * -# Enables the specified RTT interrupt using \ref RTT_EnableIT().
 * -# Get the status register value of the given RTT using \ref RTT_GetStatus().
 * -# Configures the RTT to generate an alarm at the given time
 *    using \ref RTT_SetAlarm().
 *
 * \section rtt_module Working with RTT
 * The RTT driver provides the interface to configure and use the RTT
 * peripheral.
 *
 * The Real-time Timer is used to count elapsed seconds.\n
 * This timer is clocked by the 32kHz system clock divided by a programmable
 * 16-bit balue. To be accurate, it is better to use an
 * external 32kHz crystal instead of the internal 32kHz RC.\n
 *
 * To count elapsed seconds, the user could follow these few steps:
 * <ul>
 * <li>Programming PTPRES in RTT_MR to feeding the timer with a 1Hz signal.</li>
 * <li>Writing the bit RTTRST in RTT_MR to restart the timer with new settings.</li>
 * </ul>
 *
 * An alarm can be set to happen on second by setting alarm value in RTT_AR.
 * Alarm occurrence can be detected by polling or interrupt.
 *
 * For more accurate information, please look at the RTT section of the
 * Datasheet.
 *
 * @file     hardware/at91/cortex-m3/rtt.h
 * @ingroup	 HW_SAM3S_RTT
 * @brief    RTT header
 *
 * @}
 */

#ifndef _RTT_
#define _RTT_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

//#include "chip.h"
//#include <stdint.h>
#include <at91_types.h>
#include <tmos_cfg.h>

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Real-time Timer */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Rtt hardware registers */
typedef struct {
  RwReg RTT_MR; //!< (Rtt Offset: 0x00) Mode Register */
  RwReg RTT_AR; //!< (Rtt Offset: 0x04) Alarm Register */
  RoReg RTT_VR; //!< (Rtt Offset: 0x08) Value Register */
  RoReg RTT_SR; //!< (Rtt Offset: 0x0C) Status Register */
} Rtt;
#endif /* __ASSEMBLY__ */
/* -------- RTT_MR : (RTT Offset: 0x00) Mode Register -------- */
#define RTT_MR_RTPRES_Pos 0 //!< doc!
#define RTT_MR_RTPRES_Msk (0xffffu << RTT_MR_RTPRES_Pos) //!< (RTT_MR) Real-time Timer Prescaler Value */
#define RTT_MR_RTPRES(value) ((RTT_MR_RTPRES_Msk & ((value) << RTT_MR_RTPRES_Pos))) //!< doc!
#define RTT_MR_ALMIEN (0x1u << 16) //!< (RTT_MR) Alarm Interrupt Enable */
#define RTT_MR_RTTINCIEN (0x1u << 17) //!< (RTT_MR) Real-time Timer Increment Interrupt Enable */
#define RTT_MR_RTTRST (0x1u << 18) //!< (RTT_MR) Real-time Timer Restart */
/* -------- RTT_AR : (RTT Offset: 0x04) Alarm Register -------- */
#define RTT_AR_ALMV_Pos 0 //!< doc!
#define RTT_AR_ALMV_Msk (0xffffffffu << RTT_AR_ALMV_Pos) //!< (RTT_AR) Alarm Value */
#define RTT_AR_ALMV(value) ((RTT_AR_ALMV_Msk & ((value) << RTT_AR_ALMV_Pos))) //!< doc!
/* -------- RTT_VR : (RTT Offset: 0x08) Value Register -------- */
#define RTT_VR_CRTV_Pos 0 //!< doc!
#define RTT_VR_CRTV_Msk (0xffffffffu << RTT_VR_CRTV_Pos) //!< (RTT_VR) Current Real-time Value */
/* -------- RTT_SR : (RTT Offset: 0x0C) Status Register -------- */
#define RTT_SR_ALMS (0x1u << 0) //!< (RTT_SR) Real-time Alarm Status */
#define RTT_SR_RTTINC (0x1u << 1) //!< (RTT_SR) Real-time Timer Increment */


extern void RTT_SetPrescaler( Rtt* pRtt, uint16_t wPrescaler ) ;

extern uint32_t RTT_GetTime( Rtt* pRtt ) ;

extern void RTT_EnableIT( Rtt* pRtt, uint32_t dwSources ) ;

extern uint32_t RTT_GetStatus( Rtt *pRtt ) ;

extern void RTT_SetAlarm( Rtt *pRtt, uint32_t dwTime ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef RTT_H */

