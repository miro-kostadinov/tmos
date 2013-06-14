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
 * @defgroup HW_SAM3S_WDT Watchdog Timer Controller
 * Interface for Watchdog Timer (WDT) controller.
 * @{
 *
 * \section Usage
 * -# Enable watchdog with given mode using \ref WDT_Enable().
 * -# Disable watchdog using \ref WDT_Disable()
 * -# Restart the watchdog using \ref WDT_Restart().
 * -# Get watchdog status using \ref  WDT_GetStatus().
 * -# Calculate watchdog period value using \ref WDT_GetPeriod().
 *
 * \section wdt_module Working with WDT
 * The WDT driver provides the interface to configure and use the WDT
 * peripheral.
 *
 * The WDT can be used to prevent system lock-up if the software becomes
 * trapped in a deadlock. It can generate a general reset or a processor
 * reset only. It is clocked by slow clock divided by 128.
 *
 * The WDT is running at reset with 16 seconds watchdog period (slow clock at 32.768 kHz)
 * and external reset generation enabled. The user must either disable it or
 * reprogram it to meet the application requires.
 *
 * To use the WDT, the user could follow these few steps:
 * <ul>
 * <li>Enable watchdog with given mode using \ref WDT_Enable().
 * <li>Restart the watchdog using \ref WDT_Restart() within the watchdog period.
 * </ul>
 *
 * For more accurate information, please look at the WDT section of the
 * Datasheet.
 *
 * \note
 * The Watchdog Mode Register (WDT_MR) can be written only once.\n
 *
 * @file     hardware/at91/cortex-m3/wdt.h
 * @ingroup	 HW_SAM3S_WDT
 * @brief    WDT header
 *
 * @}
 */

#ifndef _WDT_
#define _WDT_

//#include "chip.h"
//#include <stdint.h>
#include <at91_types.h>
#include <tmos_cfg.h>

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Watchdog Timer */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Wdt hardware registers */
typedef struct {
  WoReg WDT_CR; //!< (Wdt Offset: 0x00) Control Register */
  RwReg WDT_MR; //!< (Wdt Offset: 0x04) Mode Register */
  RoReg WDT_SR; //!< (Wdt Offset: 0x08) Status Register */
} Wdt;
#endif /* __ASSEMBLY__ */
/* -------- WDT_CR : (WDT Offset: 0x00) Control Register -------- */
#define WDT_CR_WDRSTT (0x1u << 0) //!< (WDT_CR) Watchdog Restart */
#define WDT_CR_KEY_Pos 24 //!< doc!
#define WDT_CR_KEY_Msk (0xffu << WDT_CR_KEY_Pos) //!< (WDT_CR) Password */
#define WDT_CR_KEY(value) ((WDT_CR_KEY_Msk & ((value) << WDT_CR_KEY_Pos))) //!< doc!
/* -------- WDT_MR : (WDT Offset: 0x04) Mode Register -------- */
#define WDT_MR_WDV_Pos 0 //!< doc!
#define WDT_MR_WDV_Msk (0xfffu << WDT_MR_WDV_Pos) //!< (WDT_MR) Watchdog Counter Value */
#define WDT_MR_WDV(value) ((WDT_MR_WDV_Msk & ((value) << WDT_MR_WDV_Pos))) //!< doc!
#define WDT_MR_WDFIEN (0x1u << 12) //!< (WDT_MR) Watchdog Fault Interrupt Enable */
#define WDT_MR_WDRSTEN (0x1u << 13) //!< (WDT_MR) Watchdog Reset Enable */
#define WDT_MR_WDRPROC (0x1u << 14) //!< (WDT_MR) Watchdog Reset Processor */
#define WDT_MR_WDDIS (0x1u << 15) //!< (WDT_MR) Watchdog Disable */
#define WDT_MR_WDD_Pos 16 //!< doc!
#define WDT_MR_WDD_Msk (0xfffu << WDT_MR_WDD_Pos) //!< (WDT_MR) Watchdog Delta Value */
#define WDT_MR_WDD(value) ((WDT_MR_WDD_Msk & ((value) << WDT_MR_WDD_Pos))) //!< doc!
#define WDT_MR_WDDBGHLT (0x1u << 28) //!< (WDT_MR) Watchdog Debug Halt */
#define WDT_MR_WDIDLEHLT (0x1u << 29) //!< (WDT_MR) Watchdog Idle Halt */
/* -------- WDT_SR : (WDT Offset: 0x08) Status Register -------- */
#define WDT_SR_WDUNF (0x1u << 0) //!< (WDT_SR) Watchdog Underflow */
#define WDT_SR_WDERR (0x1u << 1) //!< (WDT_SR) Watchdog Error */



/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

extern void WDT_Enable( Wdt* pWDT, uint32_t dwMode ) ;

extern void WDT_Disable( Wdt* pWDT ) ;

extern void WDT_Restart( Wdt* pWDT ) ;

extern uint32_t WDT_GetStatus( Wdt* pWDT ) ;

extern uint32_t WDT_GetPeriod( uint32_t dwMs ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _WDT_ */

