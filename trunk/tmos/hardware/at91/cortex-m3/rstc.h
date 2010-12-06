/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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

/**
 * @ingroup	 at91_cortex_m3
 * @defgroup  AT91_RSTC Reset Controller (RSTC)
 * Interface for Reset Controller (RSTC) peripheral.
 * @{
 *
 *
 * @file     hardware/at91/cortex-m3/rstc.h
 * @ingroup	 AT91_RSTC
 * @brief    RSTC header
 *
 * @}
*/

#ifndef RSTC_H_
#define RSTC_H_

#include <at91_types.h>

#ifdef __cplusplus
 extern "C" {
#endif

 /* ============================================================================= */
 /**  SOFTWARE API DEFINITION FOR Reset Controller */
 /* ============================================================================= */

#ifndef __ASSEMBLY__
 /** \brief Rstc hardware registers */
typedef struct {
   WoReg RSTC_CR; //!< (Rstc Offset: 0x00) Control Register */
   RoReg RSTC_SR; //!< (Rstc Offset: 0x04) Status Register */
   RwReg RSTC_MR; //!< (Rstc Offset: 0x08) Mode Register */
#ifdef CFG_SAM3U
   RoReg Reserved0[60]; 	//!< reserved
   RoReg RSTC_VER; 			//!<  Version Register
#endif /* CFG_SAM3U */
} Rstc;
#endif /* __ASSEMBLY__ */

/* -------- RSTC_CR : (RSTC Offset: 0x00) Control Register -------- */
#define RSTC_CR_PROCRST (0x1u << 0) //!< (RSTC_CR) Processor Reset */
#define RSTC_CR_PERRST (0x1u << 2) //!< (RSTC_CR) Peripheral Reset */
#define RSTC_CR_EXTRST (0x1u << 3) //!< (RSTC_CR) External Reset */
#define RSTC_CR_KEY_Pos 24 //!< doc!
#define RSTC_CR_KEY_Msk (0xffu << RSTC_CR_KEY_Pos) //!< (RSTC_CR) Password */
#define RSTC_CR_KEY(value) ((RSTC_CR_KEY_Msk & ((value) << RSTC_CR_KEY_Pos))) //!< doc!
/* -------- RSTC_SR : (RSTC Offset: 0x04) Status Register -------- */
#define RSTC_SR_URSTS (0x1u << 0) //!< (RSTC_SR) User Reset Status */
#define RSTC_SR_RSTTYP_Pos 8 //!< doc!
#define RSTC_SR_RSTTYP_Msk (0x7u << RSTC_SR_RSTTYP_Pos) //!< (RSTC_SR) Reset Type */
#define RSTC_SR_NRSTL (0x1u << 16) //!< (RSTC_SR) NRST Pin Level */
#define RSTC_SR_SRCMP (0x1u << 17) //!< (RSTC_SR) Software Reset Command in Progress */
/* -------- RSTC_MR : (RSTC Offset: 0x08) Mode Register -------- */
#define RSTC_MR_URSTEN (0x1u << 0) //!< (RSTC_MR) User Reset Enable */
#define RSTC_MR_URSTIEN (0x1u << 4) //!< (RSTC_MR) User Reset Interrupt Enable */
#define RSTC_MR_ERSTL_Pos 8 //!< doc!
#define RSTC_MR_ERSTL_Msk (0xfu << RSTC_MR_ERSTL_Pos) //!< (RSTC_MR) External Reset Length */
#define RSTC_MR_ERSTL(value) ((RSTC_MR_ERSTL_Msk & ((value) << RSTC_MR_ERSTL_Pos))) //!< doc!
#define RSTC_MR_KEY_Pos 24 //!< doc!
#define RSTC_MR_KEY_Msk (0xffu << RSTC_MR_KEY_Pos) //!< (RSTC_MR) Password */
#define RSTC_MR_KEY(value) ((RSTC_MR_KEY_Msk & ((value) << RSTC_MR_KEY_Pos))) //!< doc!

#ifdef __cplusplus
}
#endif

#endif /* RSTC_H_ */
