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
 * @defgroup  AT91_UART Universal Asynchronous Receiver Transmitter (UART)
 * Interface for UART
 * @{
 *
 *  \section Purpose
 * This module provides several definitions and methods for using an UART
 * peripheral.
 *
 *
 * @file     hardware/at91/uart.h
 * @ingroup	 AT91_UART
 * @brief    UART header
 *
 * @}
*/

#ifndef _UART_
#define _UART_

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/
#include <fam_inc.h>
#include <at91_types.h>
/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/


#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================= */
/**  SOFTWARE API DEFINITION FOR Universal Asynchronous Receiver Transmitter */
/* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Uart hardware registers */
typedef struct {
  WoReg UART_CR;       //!< (Uart Offset: 0x0000) Control Register */
  RwReg UART_MR;       //!< (Uart Offset: 0x0004) Mode Register */
  WoReg UART_IER;      //!< (Uart Offset: 0x0008) Interrupt Enable Register */
  WoReg UART_IDR;      //!< (Uart Offset: 0x000C) Interrupt Disable Register */
  RoReg UART_IMR;      //!< (Uart Offset: 0x0010) Interrupt Mask Register */
  RoReg UART_SR;       //!< (Uart Offset: 0x0014) Status Register */
  RoReg UART_RHR;      //!< (Uart Offset: 0x0018) Receive Holding Register */
  WoReg UART_THR;      //!< (Uart Offset: 0x001C) Transmit Holding Register */
  RwReg UART_BRGR;     //!< (Uart Offset: 0x0020) Baud Rate Generator Register */
  RwReg Reserved1[55]; //!< doc!
  RwReg UART_RPR;      //!< (Uart Offset: 0x100) Receive Pointer Register */
  RwReg UART_RCR;      //!< (Uart Offset: 0x104) Receive Counter Register */
  RwReg UART_TPR;      //!< (Uart Offset: 0x108) Transmit Pointer Register */
  RwReg UART_TCR;      //!< (Uart Offset: 0x10C) Transmit Counter Register */
  RwReg UART_RNPR;     //!< (Uart Offset: 0x110) Receive Next Pointer Register */
  RwReg UART_RNCR;     //!< (Uart Offset: 0x114) Receive Next Counter Register */
  RwReg UART_TNPR;     //!< (Uart Offset: 0x118) Transmit Next Pointer Register */
  RwReg UART_TNCR;     //!< (Uart Offset: 0x11C) Transmit Next Counter Register */
  WoReg UART_PTCR;     //!< (Uart Offset: 0x120) Transfer Control Register */
  RoReg UART_PTSR;     //!< (Uart Offset: 0x124) Transfer Status Register */
} Uart;
#endif /* __ASSEMBLY__ */
/* -------- UART_CR : (UART Offset: 0x0000) Control Register -------- */
#define UART_CR_RSTRX (0x1u << 2) //!< (UART_CR) Reset Receiver */
#define UART_CR_RSTTX (0x1u << 3) //!< (UART_CR) Reset Transmitter */
#define UART_CR_RXEN (0x1u << 4) //!< (UART_CR) Receiver Enable */
#define UART_CR_RXDIS (0x1u << 5) //!< (UART_CR) Receiver Disable */
#define UART_CR_TXEN (0x1u << 6) //!< (UART_CR) Transmitter Enable */
#define UART_CR_TXDIS (0x1u << 7) //!< (UART_CR) Transmitter Disable */
#define UART_CR_RSTSTA (0x1u << 8) //!< (UART_CR) Reset Status Bits */
/* -------- UART_MR : (UART Offset: 0x0004) Mode Register -------- */
#define UART_MR_PAR_Pos 9 //!< doc!
#define UART_MR_PAR_Msk (0x7u << UART_MR_PAR_Pos) //!< (UART_MR) Parity Type */
#define   UART_MR_PAR_EVEN (0x0u << 9) //!< (UART_MR) Even parity */
#define   UART_MR_PAR_ODD (0x1u << 9) //!< (UART_MR) Odd parity */
#define   UART_MR_PAR_SPACE (0x2u << 9) //!< (UART_MR) Space: parity forced to 0 */
#define   UART_MR_PAR_MARK (0x3u << 9) //!< (UART_MR) Mark: parity forced to 1 */
#define   UART_MR_PAR_NO (0x4u << 9) //!< (UART_MR) No parity */
#define UART_MR_CHMODE_Pos 14 //!< doc!
#define UART_MR_CHMODE_Msk (0x3u << UART_MR_CHMODE_Pos) //!< (UART_MR) Channel Mode */
#define   UART_MR_CHMODE_NORMAL (0x0u << 14) //!< (UART_MR) Normal Mode */
#define   UART_MR_CHMODE_AUTOMATIC (0x1u << 14) //!< (UART_MR) Automatic Echo */
#define   UART_MR_CHMODE_LOCAL_LOOPBACK (0x2u << 14) //!< (UART_MR) Local Loopback */
#define   UART_MR_CHMODE_REMOTE_LOOPBACK (0x3u << 14) //!< (UART_MR) Remote Loopback */
/* -------- UART_IER : (UART Offset: 0x0008) Interrupt Enable Register -------- */
#define UART_IER_RXRDY (0x1u << 0) //!< (UART_IER) Enable RXRDY Interrupt */
#define UART_IER_TXRDY (0x1u << 1) //!< (UART_IER) Enable TXRDY Interrupt */
#define UART_IER_ENDRX (0x1u << 3) //!< (UART_IER) Enable End of Receive Transfer Interrupt */
#define UART_IER_ENDTX (0x1u << 4) //!< (UART_IER) Enable End of Transmit Interrupt */
#define UART_IER_OVRE (0x1u << 5) //!< (UART_IER) Enable Overrun Error Interrupt */
#define UART_IER_FRAME (0x1u << 6) //!< (UART_IER) Enable Framing Error Interrupt */
#define UART_IER_PARE (0x1u << 7) //!< (UART_IER) Enable Parity Error Interrupt */
#define UART_IER_TXEMPTY (0x1u << 9) //!< (UART_IER) Enable TXEMPTY Interrupt */
#define UART_IER_TXBUFE (0x1u << 11) //!< (UART_IER) Enable Buffer Empty Interrupt */
#define UART_IER_RXBUFF (0x1u << 12) //!< (UART_IER) Enable Buffer Full Interrupt */
/* -------- UART_IDR : (UART Offset: 0x000C) Interrupt Disable Register -------- */
#define UART_IDR_RXRDY (0x1u << 0) //!< (UART_IDR) Disable RXRDY Interrupt */
#define UART_IDR_TXRDY (0x1u << 1) //!< (UART_IDR) Disable TXRDY Interrupt */
#define UART_IDR_ENDRX (0x1u << 3) //!< (UART_IDR) Disable End of Receive Transfer Interrupt */
#define UART_IDR_ENDTX (0x1u << 4) //!< (UART_IDR) Disable End of Transmit Interrupt */
#define UART_IDR_OVRE (0x1u << 5) //!< (UART_IDR) Disable Overrun Error Interrupt */
#define UART_IDR_FRAME (0x1u << 6) //!< (UART_IDR) Disable Framing Error Interrupt */
#define UART_IDR_PARE (0x1u << 7) //!< (UART_IDR) Disable Parity Error Interrupt */
#define UART_IDR_TXEMPTY (0x1u << 9) //!< (UART_IDR) Disable TXEMPTY Interrupt */
#define UART_IDR_TXBUFE (0x1u << 11) //!< (UART_IDR) Disable Buffer Empty Interrupt */
#define UART_IDR_RXBUFF (0x1u << 12) //!< (UART_IDR) Disable Buffer Full Interrupt */
/* -------- UART_IMR : (UART Offset: 0x0010) Interrupt Mask Register -------- */
#define UART_IMR_RXRDY (0x1u << 0) //!< (UART_IMR) Mask RXRDY Interrupt */
#define UART_IMR_TXRDY (0x1u << 1) //!< (UART_IMR) Disable TXRDY Interrupt */
#define UART_IMR_ENDRX (0x1u << 3) //!< (UART_IMR) Mask End of Receive Transfer Interrupt */
#define UART_IMR_ENDTX (0x1u << 4) //!< (UART_IMR) Mask End of Transmit Interrupt */
#define UART_IMR_OVRE (0x1u << 5) //!< (UART_IMR) Mask Overrun Error Interrupt */
#define UART_IMR_FRAME (0x1u << 6) //!< (UART_IMR) Mask Framing Error Interrupt */
#define UART_IMR_PARE (0x1u << 7) //!< (UART_IMR) Mask Parity Error Interrupt */
#define UART_IMR_TXEMPTY (0x1u << 9) //!< (UART_IMR) Mask TXEMPTY Interrupt */
#define UART_IMR_TXBUFE (0x1u << 11) //!< (UART_IMR) Mask TXBUFE Interrupt */
#define UART_IMR_RXBUFF (0x1u << 12) //!< (UART_IMR) Mask RXBUFF Interrupt */
/* -------- UART_SR : (UART Offset: 0x0014) Status Register -------- */
#define UART_SR_RXRDY (0x1u << 0) //!< (UART_SR) Receiver Ready */
#define UART_SR_TXRDY (0x1u << 1) //!< (UART_SR) Transmitter Ready */
#define UART_SR_ENDRX (0x1u << 3) //!< (UART_SR) End of Receiver Transfer */
#define UART_SR_ENDTX (0x1u << 4) //!< (UART_SR) End of Transmitter Transfer */
#define UART_SR_OVRE (0x1u << 5) //!< (UART_SR) Overrun Error */
#define UART_SR_FRAME (0x1u << 6) //!< (UART_SR) Framing Error */
#define UART_SR_PARE (0x1u << 7) //!< (UART_SR) Parity Error */
#define UART_SR_TXEMPTY (0x1u << 9) //!< (UART_SR) Transmitter Empty */
#define UART_SR_TXBUFE (0x1u << 11) //!< (UART_SR) Transmission Buffer Empty */
#define UART_SR_RXBUFF (0x1u << 12) //!< (UART_SR) Receive Buffer Full */
/* -------- UART_RHR : (UART Offset: 0x0018) Receive Holding Register -------- */
#define UART_RHR_RXCHR_Pos 0 //!< doc!
#define UART_RHR_RXCHR_Msk (0xffu << UART_RHR_RXCHR_Pos) //!< (UART_RHR) Received Character */
/* -------- UART_THR : (UART Offset: 0x001C) Transmit Holding Register -------- */
#define UART_THR_TXCHR_Pos 0 //!< doc!
#define UART_THR_TXCHR_Msk (0xffu << UART_THR_TXCHR_Pos) //!< (UART_THR) Character to be Transmitted */
#define UART_THR_TXCHR(value) ((UART_THR_TXCHR_Msk & ((value) << UART_THR_TXCHR_Pos))) //!< doc!
/* -------- UART_BRGR : (UART Offset: 0x0020) Baud Rate Generator Register -------- */
#define UART_BRGR_CD_Pos 0 //!< doc!
#define UART_BRGR_CD_Msk (0xffffu << UART_BRGR_CD_Pos) //!< (UART_BRGR) Clock Divisor */
#define UART_BRGR_CD(value) ((UART_BRGR_CD_Msk & ((value) << UART_BRGR_CD_Pos))) //!< doc!
/* -------- UART_RPR : (UART Offset: 0x100) Receive Pointer Register -------- */
#define UART_RPR_RXPTR_Pos 0 //!< doc!
#define UART_RPR_RXPTR_Msk (0xffffffffu << UART_RPR_RXPTR_Pos) //!< (UART_RPR) Receive Pointer Register */
#define UART_RPR_RXPTR(value) ((UART_RPR_RXPTR_Msk & ((value) << UART_RPR_RXPTR_Pos))) //!< doc!
/* -------- UART_RCR : (UART Offset: 0x104) Receive Counter Register -------- */
#define UART_RCR_RXCTR_Pos 0 //!< doc!
#define UART_RCR_RXCTR_Msk (0xffffu << UART_RCR_RXCTR_Pos) //!< (UART_RCR) Receive Counter Register */
#define UART_RCR_RXCTR(value) ((UART_RCR_RXCTR_Msk & ((value) << UART_RCR_RXCTR_Pos))) //!< doc!
/* -------- UART_TPR : (UART Offset: 0x108) Transmit Pointer Register -------- */
#define UART_TPR_TXPTR_Pos 0 //!< doc!
#define UART_TPR_TXPTR_Msk (0xffffffffu << UART_TPR_TXPTR_Pos) //!< (UART_TPR) Transmit Counter Register */
#define UART_TPR_TXPTR(value) ((UART_TPR_TXPTR_Msk & ((value) << UART_TPR_TXPTR_Pos))) //!< doc!
/* -------- UART_TCR : (UART Offset: 0x10C) Transmit Counter Register -------- */
#define UART_TCR_TXCTR_Pos 0 //!< doc!
#define UART_TCR_TXCTR_Msk (0xffffu << UART_TCR_TXCTR_Pos) //!< (UART_TCR) Transmit Counter Register */
#define UART_TCR_TXCTR(value) ((UART_TCR_TXCTR_Msk & ((value) << UART_TCR_TXCTR_Pos))) //!< doc!
/* -------- UART_RNPR : (UART Offset: 0x110) Receive Next Pointer Register -------- */
#define UART_RNPR_RXNPTR_Pos 0 //!< doc!
#define UART_RNPR_RXNPTR_Msk (0xffffffffu << UART_RNPR_RXNPTR_Pos) //!< (UART_RNPR) Receive Next Pointer */
#define UART_RNPR_RXNPTR(value) ((UART_RNPR_RXNPTR_Msk & ((value) << UART_RNPR_RXNPTR_Pos))) //!< doc!
/* -------- UART_RNCR : (UART Offset: 0x114) Receive Next Counter Register -------- */
#define UART_RNCR_RXNCTR_Pos 0 //!< doc!
#define UART_RNCR_RXNCTR_Msk (0xffffu << UART_RNCR_RXNCTR_Pos) //!< (UART_RNCR) Receive Next Counter */
#define UART_RNCR_RXNCTR(value) ((UART_RNCR_RXNCTR_Msk & ((value) << UART_RNCR_RXNCTR_Pos))) //!< doc!
/* -------- UART_TNPR : (UART Offset: 0x118) Transmit Next Pointer Register -------- */
#define UART_TNPR_TXNPTR_Pos 0 //!< doc!
#define UART_TNPR_TXNPTR_Msk (0xffffffffu << UART_TNPR_TXNPTR_Pos) //!< (UART_TNPR) Transmit Next Pointer */
#define UART_TNPR_TXNPTR(value) ((UART_TNPR_TXNPTR_Msk & ((value) << UART_TNPR_TXNPTR_Pos))) //!< doc!
/* -------- UART_TNCR : (UART Offset: 0x11C) Transmit Next Counter Register -------- */
#define UART_TNCR_TXNCTR_Pos 0 //!< doc!
#define UART_TNCR_TXNCTR_Msk (0xffffu << UART_TNCR_TXNCTR_Pos) //!< (UART_TNCR) Transmit Counter Next */
#define UART_TNCR_TXNCTR(value) ((UART_TNCR_TXNCTR_Msk & ((value) << UART_TNCR_TXNCTR_Pos))) //!< doc!
/* -------- UART_PTCR : (UART Offset: 0x120) Transfer Control Register -------- */
#define UART_PTCR_RXTEN (0x1u << 0) //!< (UART_PTCR) Receiver Transfer Enable */
#define UART_PTCR_RXTDIS (0x1u << 1) //!< (UART_PTCR) Receiver Transfer Disable */
#define UART_PTCR_TXTEN (0x1u << 8) //!< (UART_PTCR) Transmitter Transfer Enable */
#define UART_PTCR_TXTDIS (0x1u << 9) //!< (UART_PTCR) Transmitter Transfer Disable */
/* -------- UART_PTSR : (UART Offset: 0x124) Transfer Status Register -------- */
#define UART_PTSR_RXTEN (0x1u << 0) //!< (UART_PTSR) Receiver Transfer Enable */
#define UART_PTSR_TXTEN (0x1u << 8) //!< (UART_PTSR) Transmitter Transfer Enable */



/*------------------------------------------------------------------------------*/
/*         Exported functions                                                   */
/*------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _UART_ */

