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
 * @defgroup  AT91_SPI Serial Peripheral Interface (SPI)
 * Interface for Serial Peripheral Interface (SPI) controller.
 * @{
 *
 * \section spi_module Working with SPI
 * The SPI driver provides the interface to configure and use the SPI
 * peripheral.
 *
 * The Serial Peripheral Interface (SPI) circuit is a synchronous serial
 * data link that provides communication with external devices in Master
 * or Slave Mode.
 *
 * To use the SPI, the user has to follow these few steps:
 * -# Enable the SPI pins required by the application (see pio.h).
 * -# Configure the SPI using the \ref SPI_Configure(). This enables the
 *    peripheral clock. The mode register is loaded with the given value.
 * -# Configure all the necessary chip selects with \ref SPI_ConfigureNPCS().
 * -# Enable the SPI by calling \ref SPI_Enable().
 * -# Send/receive data using \ref SPI_Write() and \ref SPI_Read(). Note that \ref SPI_Read()
 *    must be called after \ref SPI_Write() to retrieve the last value read.
 * -# Send/receive data using the PDC with the \ref SPI_WriteBuffer() and
 *    \ref SPI_ReadBuffer() functions.
 * -# Disable the SPI by calling \ref SPI_Disable().
 *
 * For more accurate information, please look at the SPI section of the
 * Datasheet.
 *
 * @file     hardware/at91/spi.h
 * @ingroup	 AT91_SPI
 * @brief    SPI header
 *
 * @}
*/

#ifndef _SPI_
#define _SPI_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

//#include "chip.h"
#include <series_inc.h>

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/

/**
 *
 * Here are several macros which should be used when configuring a SPI
 * peripheral.
 *
 * \section spi_configuration_macros SPI Configuration Macros
 * - \ref SPI_PCS
 * - \ref SPI_SCBR
 * - \ref SPI_DLYBS
 * - \ref SPI_DLYBCT
 */

/** Calculate the PCS field value given the chip select NPCS value */
#define SPI_PCS(npcs)       ((~(1 << npcs) & 0xF) << 16)

/** Calculates the value of the CSR SCBR field given the baudrate and MCK. */
#define SPI_SCBR(baudrate, masterClock) ((uint32_t) (masterClock / baudrate) << 8)

/** Calculates the value of the CSR DLYBS field given the desired delay (in ns) */
#define SPI_DLYBS(delay, masterClock) ((uint32_t) (((masterClock / 1000000) * delay) / 1000) << 16)

/** Calculates the value of the CSR DLYBCT field given the desired delay (in ns) */
#define SPI_DLYBCT(delay, masterClock) ((uint32_t) (((masterClock / 1000000) * delay) / 32000) << 24)

/*------------------------------------------------------------------------------ */

#ifdef __cplusplus
 extern "C" {
#endif

 /* ============================================================================= */
 /**  SOFTWARE API DEFINITION FOR Serial Peripheral Interface */
 /* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Spi hardware registers */
typedef struct {
	WoReg SPI_CR;        //!< (Spi Offset: 0x00) Control Register */
	RwReg SPI_MR;        //!< (Spi Offset: 0x04) Mode Register */
	RoReg SPI_RDR;       //!< (Spi Offset: 0x08) Receive Data Register */
	WoReg SPI_TDR;       //!< (Spi Offset: 0x0C) Transmit Data Register */
	RoReg SPI_SR;        //!< (Spi Offset: 0x10) Status Register */
	WoReg SPI_IER;       //!< (Spi Offset: 0x14) Interrupt Enable Register */
	WoReg SPI_IDR;       //!< (Spi Offset: 0x18) Interrupt Disable Register */
	RoReg SPI_IMR;       //!< (Spi Offset: 0x1C) Interrupt Mask Register */
	RwReg Reserved1[4];  //!< doc!
	RwReg SPI_CSR[4];    //!< (Spi Offset: 0x30) Chip Select Register */
	RwReg Reserved2[41]; //!< doc!

#ifdef	CFG_SAM3S
	RwReg SPI_WPMR;      //!< (Spi Offset: 0xE4) Write Protection Control Register */
	RoReg SPI_WPSR;      //!< (Spi Offset: 0xE8) Write Protection Status Register */
#else
	RwReg Reserved3[2];  //!< doc!
#endif

#ifdef CFG_SAM3U
	RwReg SPI_ADDRSIZE;  //!< (Spi Offset: 0xEC) ADDRSIZE REGISTER
	RwReg SPI_IPNAME1; 	 //!< (Spi Offset: 0xF0) IPNAME1 REGISTER
	RwReg SPI_IPNAME2; 	 //!< (Spi Offset: 0xF4) IPNAME2 REGISTER
	RwReg SPI_FEATURES;  //!< (Spi Offset: 0xF8) FEATURES REGISTER
	RwReg SPI_VER; 		 //!< (Spi Offset: 0xFC) Register
#else
	RwReg Reserved4[5];  //!< doc!
#endif
	RwReg SPI_RPR;       //!< (Spi Offset: 0x100) Receive Pointer Register */
	RwReg SPI_RCR;       //!< (Spi Offset: 0x104) Receive Counter Register */
	RwReg SPI_TPR;       //!< (Spi Offset: 0x108) Transmit Pointer Register */
	RwReg SPI_TCR;       //!< (Spi Offset: 0x10C) Transmit Counter Register */
	RwReg SPI_RNPR;      //!< (Spi Offset: 0x110) Receive Next Pointer Register */
	RwReg SPI_RNCR;      //!< (Spi Offset: 0x114) Receive Next Counter Register */
	RwReg SPI_TNPR;      //!< (Spi Offset: 0x118) Transmit Next Pointer Register */
	RwReg SPI_TNCR;      //!< (Spi Offset: 0x11C) Transmit Next Counter Register */
	WoReg SPI_PTCR;      //!< (Spi Offset: 0x120) Transfer Control Register */
	RoReg SPI_PTSR;      //!< (Spi Offset: 0x124) Transfer Status Register */
} Spi;
#endif /* __ASSEMBLY__ */
/* -------- SPI_CR : (SPI Offset: 0x00) Control Register -------- */
#define SPI_CR_SPIEN (0x1u << 0) //!< (SPI_CR) SPI Enable */
#define SPI_CR_SPIDIS (0x1u << 1) //!< (SPI_CR) SPI Disable */
#define SPI_CR_SWRST (0x1u << 7) //!< (SPI_CR) SPI Software Reset */
#define SPI_CR_LASTXFER (0x1u << 24) //!< (SPI_CR) Last Transfer */
/* -------- SPI_MR : (SPI Offset: 0x04) Mode Register -------- */
#define SPI_MR_MSTR (0x1u << 0) //!< (SPI_MR) Master/Slave Mode */
#define SPI_MR_PS (0x1u << 1) //!< (SPI_MR) Peripheral Select */
#define SPI_MR_PCSDEC (0x1u << 2) //!< (SPI_MR) Chip Select Decode */
#define SPI_MR_MODFDIS (0x1u << 4) //!< (SPI_MR) Mode Fault Detection */
#define SPI_MR_WDRBT (0x1u << 5) //!< (SPI_MR) Wait Data Read Before Transfer */
#define SPI_MR_LLB (0x1u << 7) //!< (SPI_MR) Local Loopback Enable */
#define SPI_MR_PCS_Pos 16 //!< doc!
#define SPI_MR_PCS_Msk (0xfu << SPI_MR_PCS_Pos) //!< (SPI_MR) Peripheral Chip Select */
#define SPI_MR_PCS(value) ((SPI_MR_PCS_Msk & ((value) << SPI_MR_PCS_Pos))) //!< doc!
#define SPI_MR_DLYBCS_Pos 24 //!< doc!
#define SPI_MR_DLYBCS_Msk (0xffu << SPI_MR_DLYBCS_Pos) //!< (SPI_MR) Delay Between Chip Selects */
#define SPI_MR_DLYBCS(value) ((SPI_MR_DLYBCS_Msk & ((value) << SPI_MR_DLYBCS_Pos))) //!< doc!
/* -------- SPI_RDR : (SPI Offset: 0x08) Receive Data Register -------- */
#define SPI_RDR_RD_Pos 0 //!< doc!
#define SPI_RDR_RD_Msk (0xffffu << SPI_RDR_RD_Pos) //!< (SPI_RDR) Receive Data */
#define SPI_RDR_PCS_Pos 16 //!< doc!
#define SPI_RDR_PCS_Msk (0xfu << SPI_RDR_PCS_Pos) //!< (SPI_RDR) Peripheral Chip Select */
/* -------- SPI_TDR : (SPI Offset: 0x0C) Transmit Data Register -------- */
#define SPI_TDR_TD_Pos 0 //!< doc!
#define SPI_TDR_TD_Msk (0xffffu << SPI_TDR_TD_Pos) //!< (SPI_TDR) Transmit Data */
#define SPI_TDR_TD(value) ((SPI_TDR_TD_Msk & ((value) << SPI_TDR_TD_Pos))) //!< doc!
#define SPI_TDR_PCS_Pos 16 //!< doc!
#define SPI_TDR_PCS_Msk (0xfu << SPI_TDR_PCS_Pos) //!< (SPI_TDR) Peripheral Chip Select */
#define SPI_TDR_PCS(value) ((SPI_TDR_PCS_Msk & ((value) << SPI_TDR_PCS_Pos))) //!< doc!
#define SPI_TDR_LASTXFER (0x1u << 24) //!< (SPI_TDR) Last Transfer */
/* -------- SPI_SR : (SPI Offset: 0x10) Status Register -------- */
#define SPI_SR_RDRF (0x1u << 0) //!< (SPI_SR) Receive Data Register Full */
#define SPI_SR_TDRE (0x1u << 1) //!< (SPI_SR) Transmit Data Register Empty */
#define SPI_SR_MODF (0x1u << 2) //!< (SPI_SR) Mode Fault Error */
#define SPI_SR_OVRES (0x1u << 3) //!< (SPI_SR) Overrun Error Status */
#define SPI_SR_ENDRX (0x1u << 4) //!< (SPI_SR) End of RX buffer */
#define SPI_SR_ENDTX (0x1u << 5) //!< (SPI_SR) End of TX buffer */
#define SPI_SR_RXBUFF (0x1u << 6) //!< (SPI_SR) RX Buffer Full */
#define SPI_SR_TXBUFE (0x1u << 7) //!< (SPI_SR) TX Buffer Empty */
#define SPI_SR_NSSR (0x1u << 8) //!< (SPI_SR) NSS Rising */
#define SPI_SR_TXEMPTY (0x1u << 9) //!< (SPI_SR) Transmission Registers Empty */
#define SPI_SR_UNDES (0x1u << 10) //!< (SPI_SR) Underrun Error Status (Slave Mode Only) */
#define SPI_SR_SPIENS (0x1u << 16) //!< (SPI_SR) SPI Enable Status */
/* -------- SPI_IER : (SPI Offset: 0x14) Interrupt Enable Register -------- */
#define SPI_IER_RDRF (0x1u << 0) //!< (SPI_IER) Receive Data Register Full Interrupt Enable */
#define SPI_IER_TDRE (0x1u << 1) //!< (SPI_IER) SPI Transmit Data Register Empty Interrupt Enable */
#define SPI_IER_MODF (0x1u << 2) //!< (SPI_IER) Mode Fault Error Interrupt Enable */
#define SPI_IER_OVRES (0x1u << 3) //!< (SPI_IER) Overrun Error Interrupt Enable */
#define SPI_IER_ENDRX (0x1u << 4) //!< (SPI_IER) End of Receive Buffer Interrupt Enable */
#define SPI_IER_ENDTX (0x1u << 5) //!< (SPI_IER) End of Transmit Buffer Interrupt Enable */
#define SPI_IER_RXBUFF (0x1u << 6) //!< (SPI_IER) Receive Buffer Full Interrupt Enable */
#define SPI_IER_TXBUFE (0x1u << 7) //!< (SPI_IER) Transmit Buffer Empty Interrupt Enable */
#define SPI_IER_NSSR (0x1u << 8) //!< (SPI_IER) NSS Rising Interrupt Enable */
#define SPI_IER_TXEMPTY (0x1u << 9) //!< (SPI_IER) Transmission Registers Empty Enable */
#define SPI_IER_UNDES (0x1u << 10) //!< (SPI_IER) Underrun Error Interrupt Enable */
/* -------- SPI_IDR : (SPI Offset: 0x18) Interrupt Disable Register -------- */
#define SPI_IDR_RDRF (0x1u << 0) //!< (SPI_IDR) Receive Data Register Full Interrupt Disable */
#define SPI_IDR_TDRE (0x1u << 1) //!< (SPI_IDR) SPI Transmit Data Register Empty Interrupt Disable */
#define SPI_IDR_MODF (0x1u << 2) //!< (SPI_IDR) Mode Fault Error Interrupt Disable */
#define SPI_IDR_OVRES (0x1u << 3) //!< (SPI_IDR) Overrun Error Interrupt Disable */
#define SPI_IDR_ENDRX (0x1u << 4) //!< (SPI_IDR) End of Receive Buffer Interrupt Disable */
#define SPI_IDR_ENDTX (0x1u << 5) //!< (SPI_IDR) End of Transmit Buffer Interrupt Disable */
#define SPI_IDR_RXBUFF (0x1u << 6) //!< (SPI_IDR) Receive Buffer Full Interrupt Disable */
#define SPI_IDR_TXBUFE (0x1u << 7) //!< (SPI_IDR) Transmit Buffer Empty Interrupt Disable */
#define SPI_IDR_NSSR (0x1u << 8) //!< (SPI_IDR) NSS Rising Interrupt Disable */
#define SPI_IDR_TXEMPTY (0x1u << 9) //!< (SPI_IDR) Transmission Registers Empty Disable */
#define SPI_IDR_UNDES (0x1u << 10) //!< (SPI_IDR) Underrun Error Interrupt Disable */
/* -------- SPI_IMR : (SPI Offset: 0x1C) Interrupt Mask Register -------- */
#define SPI_IMR_RDRF (0x1u << 0) //!< (SPI_IMR) Receive Data Register Full Interrupt Mask */
#define SPI_IMR_TDRE (0x1u << 1) //!< (SPI_IMR) SPI Transmit Data Register Empty Interrupt Mask */
#define SPI_IMR_MODF (0x1u << 2) //!< (SPI_IMR) Mode Fault Error Interrupt Mask */
#define SPI_IMR_OVRES (0x1u << 3) //!< (SPI_IMR) Overrun Error Interrupt Mask */
#define SPI_IMR_ENDRX (0x1u << 4) //!< (SPI_IMR) End of Receive Buffer Interrupt Mask */
#define SPI_IMR_ENDTX (0x1u << 5) //!< (SPI_IMR) End of Transmit Buffer Interrupt Mask */
#define SPI_IMR_RXBUFF (0x1u << 6) //!< (SPI_IMR) Receive Buffer Full Interrupt Mask */
#define SPI_IMR_TXBUFE (0x1u << 7) //!< (SPI_IMR) Transmit Buffer Empty Interrupt Mask */
#define SPI_IMR_NSSR (0x1u << 8) //!< (SPI_IMR) NSS Rising Interrupt Mask */
#define SPI_IMR_TXEMPTY (0x1u << 9) //!< (SPI_IMR) Transmission Registers Empty Mask */
#define SPI_IMR_UNDES (0x1u << 10) //!< (SPI_IMR) Underrun Error Interrupt Mask */
/* -------- SPI_CSR[4] : (SPI Offset: 0x30) Chip Select Register -------- */
#define SPI_CSR_CPOL (0x1u << 0) //!< (SPI_CSR[4]) Clock Polarity */
#define SPI_CSR_NCPHA (0x1u << 1) //!< (SPI_CSR[4]) Clock Phase */
#define SPI_CSR_CSNAAT (0x1u << 2) //!< (SPI_CSR[4]) Chip Select Not Active After Transfer (Ignored if CSAAT = 1) */
#define SPI_CSR_CSAAT (0x1u << 3) //!< (SPI_CSR[4]) Chip Select Not Active After Transfer (Ignored if CSAAT = 1) */
#define SPI_CSR_BITS_Pos 4 //!< doc!
#define SPI_CSR_BITS_Msk (0xfu << SPI_CSR_BITS_Pos) //!< (SPI_CSR[4]) Bits Per Transfer */
#define   SPI_CSR_BITS_8_BIT (0x0u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_9_BIT (0x1u << 4) //!< (SPI_CSR[4]) 9_bits for transfer */
#define   SPI_CSR_BITS_10_BIT (0x2u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_11_BIT (0x3u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_12_BIT (0x4u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_13_BIT (0x5u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_14_BIT (0x6u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_15_BIT (0x7u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define   SPI_CSR_BITS_16_BIT (0x8u << 4) //!< (SPI_CSR[4]) 8_bits for transfer */
#define SPI_CSR_SCBR_Pos 8 //!< doc!
#define SPI_CSR_SCBR_Msk (0xffu << SPI_CSR_SCBR_Pos) //!< (SPI_CSR[4]) Serial Clock Baud Rate */
#define SPI_CSR_SCBR(value) ((SPI_CSR_SCBR_Msk & ((value) << SPI_CSR_SCBR_Pos))) //!< doc!
#define SPI_CSR_DLYBS_Pos 16 //!< doc!
#define SPI_CSR_DLYBS_Msk (0xffu << SPI_CSR_DLYBS_Pos) //!< (SPI_CSR[4]) Delay Before SPCK */
#define SPI_CSR_DLYBS(value) ((SPI_CSR_DLYBS_Msk & ((value) << SPI_CSR_DLYBS_Pos))) //!< doc!
#define SPI_CSR_DLYBCT_Pos 24 //!< doc!
#define SPI_CSR_DLYBCT_Msk (0xffu << SPI_CSR_DLYBCT_Pos) //!< (SPI_CSR[4]) Delay Between Consecutive Transfers */
#define SPI_CSR_DLYBCT(value) ((SPI_CSR_DLYBCT_Msk & ((value) << SPI_CSR_DLYBCT_Pos))) //!< doc!

#ifdef	CFG_SAM3S
/* -------- SPI_WPMR : (SPI Offset: 0xE4) Write Protection Control Register -------- */
#define SPI_WPMR_SPIWPEN (0x1u << 0) //!< (SPI_WPMR) SPI Write Protection Enable */
#define SPI_WPMR_SPIWPKEY_Pos 8 //!< doc!
#define SPI_WPMR_SPIWPKEY_Msk (0xffffffu << SPI_WPMR_SPIWPKEY_Pos) //!< (SPI_WPMR) SPI Write Protection Key Password */
#define SPI_WPMR_SPIWPKEY(value) ((SPI_WPMR_SPIWPKEY_Msk & ((value) << SPI_WPMR_SPIWPKEY_Pos))) //!< doc!
/* -------- SPI_WPSR : (SPI Offset: 0xE8) Write Protection Status Register -------- */
#define SPI_WPSR_SPIWPVS_Pos 0 //!< doc!
#define SPI_WPSR_SPIWPVS_Msk (0x7u << SPI_WPSR_SPIWPVS_Pos) //!< (SPI_WPSR) SPI Write Protection Violation Status */
#define SPI_WPSR_SPIWPVSRC_Pos 8 //!< doc!
#define SPI_WPSR_SPIWPVSRC_Msk (0xffu << SPI_WPSR_SPIWPVSRC_Pos) //!< (SPI_WPSR) SPI Write Protection Violation Source */
#endif

/* -------- SPI_RPR : (SPI Offset: 0x100) Receive Pointer Register -------- */
#define SPI_RPR_RXPTR_Pos 0 //!< doc!
#define SPI_RPR_RXPTR_Msk (0xffffffffu << SPI_RPR_RXPTR_Pos) //!< (SPI_RPR) Receive Pointer Register */
#define SPI_RPR_RXPTR(value) ((SPI_RPR_RXPTR_Msk & ((value) << SPI_RPR_RXPTR_Pos))) //!< doc!
/* -------- SPI_RCR : (SPI Offset: 0x104) Receive Counter Register -------- */
#define SPI_RCR_RXCTR_Pos 0 //!< doc!
#define SPI_RCR_RXCTR_Msk (0xffffu << SPI_RCR_RXCTR_Pos) //!< (SPI_RCR) Receive Counter Register */
#define SPI_RCR_RXCTR(value) ((SPI_RCR_RXCTR_Msk & ((value) << SPI_RCR_RXCTR_Pos))) //!< doc!
/* -------- SPI_TPR : (SPI Offset: 0x108) Transmit Pointer Register -------- */
#define SPI_TPR_TXPTR_Pos 0 //!< doc!
#define SPI_TPR_TXPTR_Msk (0xffffffffu << SPI_TPR_TXPTR_Pos) //!< (SPI_TPR) Transmit Counter Register */
#define SPI_TPR_TXPTR(value) ((SPI_TPR_TXPTR_Msk & ((value) << SPI_TPR_TXPTR_Pos))) //!< doc!
/* -------- SPI_TCR : (SPI Offset: 0x10C) Transmit Counter Register -------- */
#define SPI_TCR_TXCTR_Pos 0 //!< doc!
#define SPI_TCR_TXCTR_Msk (0xffffu << SPI_TCR_TXCTR_Pos) //!< (SPI_TCR) Transmit Counter Register */
#define SPI_TCR_TXCTR(value) ((SPI_TCR_TXCTR_Msk & ((value) << SPI_TCR_TXCTR_Pos))) //!< doc!
/* -------- SPI_RNPR : (SPI Offset: 0x110) Receive Next Pointer Register -------- */
#define SPI_RNPR_RXNPTR_Pos 0 //!< doc!
#define SPI_RNPR_RXNPTR_Msk (0xffffffffu << SPI_RNPR_RXNPTR_Pos) //!< (SPI_RNPR) Receive Next Pointer */
#define SPI_RNPR_RXNPTR(value) ((SPI_RNPR_RXNPTR_Msk & ((value) << SPI_RNPR_RXNPTR_Pos))) //!< doc!
/* -------- SPI_RNCR : (SPI Offset: 0x114) Receive Next Counter Register -------- */
#define SPI_RNCR_RXNCTR_Pos 0 //!< doc!
#define SPI_RNCR_RXNCTR_Msk (0xffffu << SPI_RNCR_RXNCTR_Pos) //!< (SPI_RNCR) Receive Next Counter */
#define SPI_RNCR_RXNCTR(value) ((SPI_RNCR_RXNCTR_Msk & ((value) << SPI_RNCR_RXNCTR_Pos))) //!< doc!
/* -------- SPI_TNPR : (SPI Offset: 0x118) Transmit Next Pointer Register -------- */
#define SPI_TNPR_TXNPTR_Pos 0 //!< doc!
#define SPI_TNPR_TXNPTR_Msk (0xffffffffu << SPI_TNPR_TXNPTR_Pos) //!< (SPI_TNPR) Transmit Next Pointer */
#define SPI_TNPR_TXNPTR(value) ((SPI_TNPR_TXNPTR_Msk & ((value) << SPI_TNPR_TXNPTR_Pos))) //!< doc!
/* -------- SPI_TNCR : (SPI Offset: 0x11C) Transmit Next Counter Register -------- */
#define SPI_TNCR_TXNCTR_Pos 0 //!< doc!
#define SPI_TNCR_TXNCTR_Msk (0xffffu << SPI_TNCR_TXNCTR_Pos) //!< (SPI_TNCR) Transmit Counter Next */
#define SPI_TNCR_TXNCTR(value) ((SPI_TNCR_TXNCTR_Msk & ((value) << SPI_TNCR_TXNCTR_Pos))) //!< doc!
/* -------- SPI_PTCR : (SPI Offset: 0x120) Transfer Control Register -------- */
#define SPI_PTCR_RXTEN (0x1u << 0) //!< (SPI_PTCR) Receiver Transfer Enable */
#define SPI_PTCR_RXTDIS (0x1u << 1) //!< (SPI_PTCR) Receiver Transfer Disable */
#define SPI_PTCR_TXTEN (0x1u << 8) //!< (SPI_PTCR) Transmitter Transfer Enable */
#define SPI_PTCR_TXTDIS (0x1u << 9) //!< (SPI_PTCR) Transmitter Transfer Disable */
/* -------- SPI_PTSR : (SPI Offset: 0x124) Transfer Status Register -------- */
#define SPI_PTSR_RXTEN (0x1u << 0) //!< (SPI_PTSR) Receiver Transfer Enable */
#define SPI_PTSR_TXTEN (0x1u << 8) //!< (SPI_PTSR) Transmitter Transfer Enable */


/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

extern void SPI_Enable( Spi* spi ) ;
extern void SPI_Disable( Spi* spi ) ;
extern void SPI_EnableIt( Spi* spi, uint32_t dwSources ) ;
extern void SPI_DisableIt( Spi* spi, uint32_t dwSources ) ;

extern void SPI_Configure( Spi* spi, uint32_t dwId, uint32_t dwConfiguration ) ;
extern void SPI_ConfigureNPCS( Spi* spi, uint32_t dwNpcs, uint32_t dwConfiguration ) ;

extern uint32_t SPI_Read( Spi* spi ) ;
extern void SPI_Write( Spi* spi, uint32_t dwNpcs, uint16_t wData ) ;

extern uint32_t SPI_GetStatus( Spi* spi ) ;
extern uint32_t SPI_IsFinished( Spi* pSpi ) ;

extern void SPI_PdcEnableTx( Spi* spi ) ;
extern void SPI_PdcDisableTx( Spi* spi ) ;
extern void SPI_PdcEnableRx( Spi* spi ) ;
extern void SPI_PdcDisableRx( Spi* spi ) ;

extern void SPI_PdcSetTx( Spi* spi, void* pvTxBuf, uint32_t dwTxCount, void* pvTxNextBuf, uint32_t dwTxNextCount ) ;
extern void SPI_PdcSetRx( Spi* spi, void* pvRxBuf, uint32_t dwRxCount, void* pvRxNextBuf, uint32_t dwRxNextCount ) ;

extern uint32_t SPI_WriteBuffer( Spi* spi, void* pvBuffer, uint32_t dwLength ) ;

extern uint32_t SPI_ReadBuffer( Spi* spi, void* pvBuffer, uint32_t dwLength ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SPI_ */

