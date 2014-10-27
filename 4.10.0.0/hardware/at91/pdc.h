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
 * @ingroup	 hardware_at91
 * @defgroup  AT91_PDC Peripheral DMA Interface (PDA)
 * Interface for Peripheral DMA controller (PDC).
 * @{
 *
 * @file     hardware/at91/pdc.h
 * @ingroup	 AT91_PDC
 * @brief    PDC header
 *
 * @}
*/

#ifndef PDC_H_
#define PDC_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* ========================================================================== */
/**  SOFTWARE API DEFINITION FOR Peripheral DMA Controller */
/* ========================================================================== */

#ifndef __ASSEMBLY__
/** \brief Pdc hardware registers */
typedef struct {
  RwReg Reserved1[64]; //!< doc!
  RwReg PERIPH_RPR;    //!< (Pdc Offset: 0x100) Receive Pointer Register */
  RwReg PERIPH_RCR;    //!< (Pdc Offset: 0x104) Receive Counter Register */
  RwReg PERIPH_TPR;    //!< (Pdc Offset: 0x108) Transmit Pointer Register */
  RwReg PERIPH_TCR;    //!< (Pdc Offset: 0x10C) Transmit Counter Register */
  RwReg PERIPH_RNPR;   //!< (Pdc Offset: 0x110) Receive Next Pointer Register */
  RwReg PERIPH_RNCR;   //!< (Pdc Offset: 0x114) Receive Next Counter Register */
  RwReg PERIPH_TNPR;   //!< (Pdc Offset: 0x118) Transmit Next Pointer Register */
  RwReg PERIPH_TNCR;   //!< (Pdc Offset: 0x11C) Transmit Next Counter Register */
  WoReg PERIPH_PTCR;   //!< (Pdc Offset: 0x120) Transfer Control Register */
  RoReg PERIPH_PTSR;   //!< (Pdc Offset: 0x124) Transfer Status Register */
} Pdc;
#endif /* __ASSEMBLY__ */
/* -------- PERIPH_RPR : (PDC Offset: 0x100) Receive Pointer Register -------- */
#define PERIPH_RPR_RXPTR_Pos 0 //!< doc!
#define PERIPH_RPR_RXPTR_Msk (0xffffffffu << PERIPH_RPR_RXPTR_Pos) //!< (PERIPH_RPR) Receive Pointer Register */
#define PERIPH_RPR_RXPTR(value) ((PERIPH_RPR_RXPTR_Msk & ((value) << PERIPH_RPR_RXPTR_Pos))) //!< doc!
/* -------- PERIPH_RCR : (PDC Offset: 0x104) Receive Counter Register -------- */
#define PERIPH_RCR_RXCTR_Pos 0 //!< doc!
#define PERIPH_RCR_RXCTR_Msk (0xffffu << PERIPH_RCR_RXCTR_Pos) //!< (PERIPH_RCR) Receive Counter Register */
#define PERIPH_RCR_RXCTR(value) ((PERIPH_RCR_RXCTR_Msk & ((value) << PERIPH_RCR_RXCTR_Pos))) //!< doc!
/* -------- PERIPH_TPR : (PDC Offset: 0x108) Transmit Pointer Register -------- */
#define PERIPH_TPR_TXPTR_Pos 0 //!< doc!
#define PERIPH_TPR_TXPTR_Msk (0xffffffffu << PERIPH_TPR_TXPTR_Pos) //!< (PERIPH_TPR) Transmit Counter Register */
#define PERIPH_TPR_TXPTR(value) ((PERIPH_TPR_TXPTR_Msk & ((value) << PERIPH_TPR_TXPTR_Pos))) //!< doc!
/* -------- PERIPH_TCR : (PDC Offset: 0x10C) Transmit Counter Register -------- */
#define PERIPH_TCR_TXCTR_Pos 0 //!< doc!
#define PERIPH_TCR_TXCTR_Msk (0xffffu << PERIPH_TCR_TXCTR_Pos) //!< (PERIPH_TCR) Transmit Counter Register */
#define PERIPH_TCR_TXCTR(value) ((PERIPH_TCR_TXCTR_Msk & ((value) << PERIPH_TCR_TXCTR_Pos))) //!< doc!
/* -------- PERIPH_RNPR : (PDC Offset: 0x110) Receive Next Pointer Register -------- */
#define PERIPH_RNPR_RXNPTR_Pos 0 //!< doc!
#define PERIPH_RNPR_RXNPTR_Msk (0xffffffffu << PERIPH_RNPR_RXNPTR_Pos) //!< (PERIPH_RNPR) Receive Next Pointer */
#define PERIPH_RNPR_RXNPTR(value) ((PERIPH_RNPR_RXNPTR_Msk & ((value) << PERIPH_RNPR_RXNPTR_Pos))) //!< doc!
/* -------- PERIPH_RNCR : (PDC Offset: 0x114) Receive Next Counter Register -------- */
#define PERIPH_RNCR_RXNCTR_Pos 0 //!< doc!
#define PERIPH_RNCR_RXNCTR_Msk (0xffffu << PERIPH_RNCR_RXNCTR_Pos) //!< (PERIPH_RNCR) Receive Next Counter */
#define PERIPH_RNCR_RXNCTR(value) ((PERIPH_RNCR_RXNCTR_Msk & ((value) << PERIPH_RNCR_RXNCTR_Pos))) //!< doc!
/* -------- PERIPH_TNPR : (PDC Offset: 0x118) Transmit Next Pointer Register -------- */
#define PERIPH_TNPR_TXNPTR_Pos 0 //!< doc!
#define PERIPH_TNPR_TXNPTR_Msk (0xffffffffu << PERIPH_TNPR_TXNPTR_Pos) //!< (PERIPH_TNPR) Transmit Next Pointer */
#define PERIPH_TNPR_TXNPTR(value) ((PERIPH_TNPR_TXNPTR_Msk & ((value) << PERIPH_TNPR_TXNPTR_Pos))) //!< doc!
/* -------- PERIPH_TNCR : (PDC Offset: 0x11C) Transmit Next Counter Register -------- */
#define PERIPH_TNCR_TXNCTR_Pos 0 //!< doc!
#define PERIPH_TNCR_TXNCTR_Msk (0xffffu << PERIPH_TNCR_TXNCTR_Pos) //!< (PERIPH_TNCR) Transmit Counter Next */
#define PERIPH_TNCR_TXNCTR(value) ((PERIPH_TNCR_TXNCTR_Msk & ((value) << PERIPH_TNCR_TXNCTR_Pos))) //!< doc!
/* -------- PERIPH_PTCR : (PDC Offset: 0x120) Transfer Control Register -------- */
#define PERIPH_PTCR_RXTEN (0x1u << 0) //!< (PERIPH_PTCR) Receiver Transfer Enable */
#define PERIPH_PTCR_RXTDIS (0x1u << 1) //!< (PERIPH_PTCR) Receiver Transfer Disable */
#define PERIPH_PTCR_TXTEN (0x1u << 8) //!< (PERIPH_PTCR) Transmitter Transfer Enable */
#define PERIPH_PTCR_TXTDIS (0x1u << 9) //!< (PERIPH_PTCR) Transmitter Transfer Disable */
/* -------- PERIPH_PTSR : (PDC Offset: 0x124) Transfer Status Register -------- */
#define PERIPH_PTSR_RXTEN (0x1u << 0) //!< (PERIPH_PTSR) Receiver Transfer Enable */
#define PERIPH_PTSR_TXTEN (0x1u << 8) //!< (PERIPH_PTSR) Transmitter Transfer Enable */


#ifdef __cplusplus
}
#endif

#endif /* PDC_H_ */
