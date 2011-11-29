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
/**
 * @ingroup	 at91_cortex_m3
 * @defgroup  AT91_EEFC Enhanced Embedded Flash Controller (EEFC)
 * Interface for configuration the Enhanced Embedded Flash Controller (EEFC) peripheral.
 * @{
 *
 * \section Usage
 *
 * -# Enable/disable %flash ready interrupt sources using EFC_EnableFrdyIt()
 *    and EFC_DisableFrdyIt().
 * -# Translates the given address into which EEFC, page and offset values
 *    for difference density %flash memory using EFC_TranslateAddress().
 * -# Computes the address of a %flash access given the EFC, page and offset
 *    for difference density %flash memory using EFC_ComputeAddress().
 * -# Start the executing command with EFC_StartCommand()
 * -# Retrieve the current status of the EFC using EFC_GetStatus().
 * -# Retrieve the result of the last executed command with EFC_GetResult().
 *
 *  \section efc_module Working with EEFC
 * The EEFC driver provides the interface to configure and use the EEFC
 * peripheral.
 *
 * The user needs to set the number of wait states depending on the frequency used.\n
 * Configure number of cycles for flash read/write operations in the FWS field of EEFC_FMR.
 *
 * It offers a function to send flash command to EEFC and waits for the
 * flash to be ready.
 *
 * To send flash command, the user could do in either of following way:
 * <ul>
 * <li>Write a correct key, command and argument in EEFC_FCR. </li>
 * <li>Or, Use IAP (In Application Programming) function which is executed from
 * ROM directly, this allows flash programming to be done by code running in flash.</li>
 * <li>Once the command is achieved, it can be detected even by polling EEFC_FSR or interrupt.
 * </ul>
 *
 * The command argument could be a page number,GPNVM number or nothing, it depends on
 * the command itself. Some useful functions in this driver could help user tranlate physical
 * flash address into a page number and vice verse.
 *
 * For more accurate information, please look at the EEFC section of the
 * Datasheet.
 *
 * @file     hardware/at91/cortex-m3/eefc.h
 * @ingroup	 AT91_EEFC
 * @brief    EEFC header
 *
 * @}
*/

#ifndef _EEFC_
#define _EEFC_

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
/* EFC command */
#define EFC_FCMD_GETD    0x00	//!< doc!
#define EFC_FCMD_WP      0x01	//!< doc!
#define EFC_FCMD_WPL     0x02	//!< doc!
#define EFC_FCMD_EWP     0x03	//!< doc!
#define EFC_FCMD_EWPL    0x04	//!< doc!
#define EFC_FCMD_EA      0x05	//!< doc!
#define EFC_FCMD_SLB     0x08	//!< doc!
#define EFC_FCMD_CLB     0x09	//!< doc!
#define EFC_FCMD_GLB     0x0A	//!< doc!
#define EFC_FCMD_SFB     0x0B	//!< doc!
#define EFC_FCMD_CFB     0x0C	//!< doc!
#define EFC_FCMD_GFB     0x0D	//!< doc!
#define EFC_FCMD_STUI    0x0E	//!< Start unique ID
#define EFC_FCMD_SPUI    0x0F	//!< Stop unique ID

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Embedded Flash Controller */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Efc hardware registers */
typedef struct {
RwReg EEFC_FMR; //!< (Efc Offset: 0x00) EEFC Flash Mode Register */
WoReg EEFC_FCR; //!< (Efc Offset: 0x04) EEFC Flash Command Register */
RoReg EEFC_FSR; //!< (Efc Offset: 0x08) EEFC Flash Status Register */
RoReg EEFC_FRR; //!< (Efc Offset: 0x0C) EEFC Flash Result Register */
} Efc;
#endif /* __ASSEMBLY__ */
/* -------- EEFC_FMR : (EFC Offset: 0x00) EEFC Flash Mode Register -------- */
#define EEFC_FMR_FRDY (0x1u << 0) //!< (EEFC_FMR) Ready Interrupt Enable */
#define EEFC_FMR_FWS_Pos 8	//!< doc!
#define EEFC_FMR_FWS_Msk (0xfu << EEFC_FMR_FWS_Pos) //!< (EEFC_FMR) Flash Wait State */
#define EEFC_FMR_FWS(value) ((EEFC_FMR_FWS_Msk & ((value) << EEFC_FMR_FWS_Pos)))	//!< doc!
#define EEFC_FMR_FAM (0x1u << 24) //!< (EEFC_FMR) Flash Access Mode */
/* -------- EEFC_FCR : (EFC Offset: 0x04) EEFC Flash Command Register -------- */
#define EEFC_FCR_FCMD_Pos 0	//!< doc!
#define EEFC_FCR_FCMD_Msk (0xffu << EEFC_FCR_FCMD_Pos) //!< (EEFC_FCR) Flash Command */
#define EEFC_FCR_FCMD(value) ((EEFC_FCR_FCMD_Msk & ((value) << EEFC_FCR_FCMD_Pos)))	//!< doc!
#define EEFC_FCR_FARG_Pos 8	//!< doc!
#define EEFC_FCR_FARG_Msk (0xffffu << EEFC_FCR_FARG_Pos) //!< (EEFC_FCR) Flash Command Argument */
#define EEFC_FCR_FARG(value) ((EEFC_FCR_FARG_Msk & ((value) << EEFC_FCR_FARG_Pos)))	//!< doc!
#define EEFC_FCR_FKEY_Pos 24	//!< doc!
#define EEFC_FCR_FKEY_Msk (0xffu << EEFC_FCR_FKEY_Pos) //!< (EEFC_FCR) Flash Writing Protection Key */
#define EEFC_FCR_FKEY(value) ((EEFC_FCR_FKEY_Msk & ((value) << EEFC_FCR_FKEY_Pos)))	//!< doc!
/* -------- EEFC_FSR : (EFC Offset: 0x08) EEFC Flash Status Register -------- */
#define EEFC_FSR_FRDY (0x1u << 0) //!< (EEFC_FSR) Flash Ready Status */
#define EEFC_FSR_FCMDE (0x1u << 1) //!< (EEFC_FSR) Flash Command Error Status */
#define EEFC_FSR_FLOCKE (0x1u << 2) //!< (EEFC_FSR) Flash Lock Error Status */
/* -------- EEFC_FRR : (EFC Offset: 0x0C) EEFC Flash Result Register -------- */
#define EEFC_FRR_FVALUE_Pos 0	//!< doc!
#define EEFC_FRR_FVALUE_Msk (0xffffffffu << EEFC_FRR_FVALUE_Pos) //!< (EEFC_FRR) Flash Result Value */

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

extern void EFC_EnableFrdyIt( Efc* efc ) ;

extern void EFC_DisableFrdyIt( Efc* efc ) ;

extern void EFC_SetWaitState( Efc* efc, uint8_t cycles ) ;

extern void EFC_TranslateAddress( Efc** pEfc, uint32_t dwAddress, uint16_t *pwPage, uint16_t *pwOffset ) ;

extern void EFC_ComputeAddress( Efc* efc, uint16_t wPage, uint16_t wOffset, uint32_t *pdwAddress ) ;

extern void EFC_StartCommand( Efc* efc, uint32_t dwCommand, uint32_t dwArgument ) ;

extern uint32_t EFC_PerformCommand( Efc* efc, uint32_t dwCommand, uint32_t dwArgument, uint32_t dwUseIAP ) ;

extern uint32_t EFC_GetStatus( Efc* efc ) ;

extern uint32_t EFC_GetResult( Efc* efc ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _EEFC_ */

