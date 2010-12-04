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
 * @defgroup  AT91_SSC Synchronous Serial (SSC) controller
 * Interface for Synchronous Serial (SSC) controller.
 * @{
 *
 * \section ssc_module Working with SSC
 * The SSC driver provides the interface to configure and use the SSC
 * peripheral.
 *
 * !Usage
 *
 * -# Enable the SSC interface pins.
 * -# Configure the SSC to operate at a specific frequency by calling
 *    SSC_Configure(). This function enables the peripheral clock of the SSC,
 *    but not its PIOs.
 * -# Configure the transmitter and/or the receiver using the
 *    SSC_ConfigureTransmitter() and SSC_ConfigureEmitter() functions.
 * -# Enable the PIOs or the transmitter and/or the received.
 * -# Enable the transmitter and/or the receiver using SSC_EnableTransmitter()
 *    and SSC_EnableReceiver()
 * -# Send data through the transmitter using SSC_Write() and SSC_WriteBuffer()
 * -# Receive data from the receiver using SSC_Read() and SSC_ReadBuffer()
 * -# Disable the transmitter and/or the receiver using SSC_DisableTransmitter()
 *    and SSC_DisableReceiver()
 *
 * For more accurate information, please look at the RTC section of the
 * Datasheet.
 *
 * @file     hardware/at91/ssc.h
 * @ingroup	 AT91_SSC
 * @brief    SSC header
 *
 * @}
*/

#ifndef _SSC_
#define _SSC_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"
//#include <stdint.h>
#include <fam_inc.h>

#ifdef __cplusplus
 extern "C" {
#endif

 /* ============================================================================= */
 /**  SOFTWARE API DEFINITION FOR Synchronous Serial Controller */
 /* ============================================================================= */

#ifndef __ASSEMBLY__
/** \brief Ssc hardware registers */
typedef struct {
	WoReg SSC_CR;        //!< (Ssc Offset: 0x0) Control Register
	RwReg SSC_CMR;       //!< (Ssc Offset: 0x4) Clock Mode Register
	RwReg Reserved1[2];	 //!< doc!
	RwReg SSC_RCMR;      //!< (Ssc Offset: 0x10) Receive Clock Mode Register
	RwReg SSC_RFMR;      //!< (Ssc Offset: 0x14) Receive Frame Mode Register
	RwReg SSC_TCMR;      //!< (Ssc Offset: 0x18) Transmit Clock Mode Register
	RwReg SSC_TFMR;      //!< (Ssc Offset: 0x1C) Transmit Frame Mode Register
	RoReg SSC_RHR;       //!< (Ssc Offset: 0x20) Receive Holding Register
	WoReg SSC_THR;       //!< (Ssc Offset: 0x24) Transmit Holding Register
	RwReg Reserved2[2];	 //!< doc!
	RoReg SSC_RSHR;      //!< (Ssc Offset: 0x30) Receive Sync. Holding Register
	RwReg SSC_TSHR;      //!< (Ssc Offset: 0x34) Transmit Sync. Holding Register
	RwReg SSC_RC0R;      //!< (Ssc Offset: 0x38) Receive Compare 0 Register
	RwReg SSC_RC1R;      //!< (Ssc Offset: 0x3C) Receive Compare 1 Register
	RoReg SSC_SR;        //!< (Ssc Offset: 0x40) Status Register
	WoReg SSC_IER;       //!< (Ssc Offset: 0x44) Interrupt Enable Register
	WoReg SSC_IDR;       //!< (Ssc Offset: 0x48) Interrupt Disable Register
	RoReg SSC_IMR;       //!< (Ssc Offset: 0x4C) Interrupt Mask Register
	RwReg Reserved3[37];	 //!< doc!
#ifdef CFG_SAM3S
	RwReg SSC_WPMR;      //!< (Ssc Offset: 0xE4) Write Protect Mode Register
	RoReg SSC_WPSR;      //!< (Ssc Offset: 0xE8) Write Protect Status Register
#else
	RwReg Reserved4[2];	 //!< doc!
#endif
	RwReg Reserved5[5];	 //!< doc!
	RwReg SSC_RPR;       //!< (Ssc Offset: 0x100) Receive Pointer Register
	RwReg SSC_RCR;       //!< (Ssc Offset: 0x104) Receive Counter Register
	RwReg SSC_TPR;       //!< (Ssc Offset: 0x108) Transmit Pointer Register
	RwReg SSC_TCR;       //!< (Ssc Offset: 0x10C) Transmit Counter Register
	RwReg SSC_RNPR;      //!< (Ssc Offset: 0x110) Receive Next Pointer Register
	RwReg SSC_RNCR;      //!< (Ssc Offset: 0x114) Receive Next Counter Register
	RwReg SSC_TNPR;      //!< (Ssc Offset: 0x118) Transmit Next Pointer Register
	RwReg SSC_TNCR;      //!< (Ssc Offset: 0x11C) Transmit Next Counter Register
	WoReg SSC_PTCR;      //!< (Ssc Offset: 0x120) Transfer Control Register
	RoReg SSC_PTSR;      //!< (Ssc Offset: 0x124) Transfer Status Register
} Ssc;
#endif /* __ASSEMBLY__ */
/* -------- SSC_CR : (SSC Offset: 0x0) Control Register -------- */
#define SSC_CR_RXEN (0x1u << 0) //!< (SSC_CR) Receive Enable
#define SSC_CR_RXDIS (0x1u << 1) //!< (SSC_CR) Receive Disable
#define SSC_CR_TXEN (0x1u << 8) //!< (SSC_CR) Transmit Enable
#define SSC_CR_TXDIS (0x1u << 9) //!< (SSC_CR) Transmit Disable
#define SSC_CR_SWRST (0x1u << 15) //!< (SSC_CR) Software Reset
/* -------- SSC_CMR : (SSC Offset: 0x4) Clock Mode Register -------- */
#define SSC_CMR_DIV_Pos 0	 //!< doc!
#define SSC_CMR_DIV_Msk (0xfffu << SSC_CMR_DIV_Pos) //!< (SSC_CMR) Clock Divider
#define SSC_CMR_DIV(value) ((SSC_CMR_DIV_Msk & ((value) << SSC_CMR_DIV_Pos)))	 //!< doc!
/* -------- SSC_RCMR : (SSC Offset: 0x10) Receive Clock Mode Register -------- */
#define SSC_RCMR_CKS_Pos 0	 //!< doc!
#define SSC_RCMR_CKS_Msk (0x3u << SSC_RCMR_CKS_Pos) //!< (SSC_RCMR) Receive Clock Selection
#define   SSC_RCMR_CKS_MCK (0x0u << 0) //!< (SSC_RCMR) Divided Clock
#define   SSC_RCMR_CKS_TK (0x1u << 0) //!< (SSC_RCMR) TK Clock signal
#define   SSC_RCMR_CKS_RK (0x2u << 0) //!< (SSC_RCMR) RK pin
#define SSC_RCMR_CKO_Pos 2	 //!< doc!
#define SSC_RCMR_CKO_Msk (0x7u << SSC_RCMR_CKO_Pos) //!< (SSC_RCMR) Receive Clock Output Mode Selection
#define   SSC_RCMR_CKO_NONE (0x0u << 2) //!< (SSC_RCMR) None
#define   SSC_RCMR_CKO_CONTINUOUS (0x1u << 2) //!< (SSC_RCMR) Continuous Receive Clock
#define   SSC_RCMR_CKO_TRANSFER (0x2u << 2) //!< (SSC_RCMR) Receive Clock only during data transfers
#define SSC_RCMR_CKI (0x1u << 5) //!< (SSC_RCMR) Receive Clock Inversion
#define SSC_RCMR_CKG_Pos 6	 //!< doc!
#define SSC_RCMR_CKG_Msk (0x3u << SSC_RCMR_CKG_Pos) //!< (SSC_RCMR) Receive Clock Gating Selection
#define   SSC_RCMR_CKG_NONE (0x0u << 6) //!< (SSC_RCMR) None
#define   SSC_RCMR_CKG_CONTINUOUS (0x1u << 6) //!< (SSC_RCMR) Continuous Receive Clock
#define   SSC_RCMR_CKG_TRANSFER (0x2u << 6) //!< (SSC_RCMR) Receive Clock only during data transfers
#define SSC_RCMR_START_Pos 8	 //!< doc!
#define SSC_RCMR_START_Msk (0xfu << SSC_RCMR_START_Pos) //!< (SSC_RCMR) Receive Start Selection
#define   SSC_RCMR_START_CONTINUOUS (0x0u << 8) //!< (SSC_RCMR) Continuous, as soon as the receiver is enabled, and immediately after the end of transfer of the previous data.
#define   SSC_RCMR_START_TRANSMIT (0x1u << 8) //!< (SSC_RCMR) Transmit start
#define   SSC_RCMR_START_RF_LOW (0x2u << 8) //!< (SSC_RCMR) Detection of a low level on RF signal
#define   SSC_RCMR_START_RF_HIGH (0x3u << 8) //!< (SSC_RCMR) Detection of a high level on RF signal
#define   SSC_RCMR_START_RF_FALLING (0x4u << 8) //!< (SSC_RCMR) Detection of a falling edge on RF signal
#define   SSC_RCMR_START_RF_RISING (0x5u << 8) //!< (SSC_RCMR) Detection of a rising edge on RF signal
#define   SSC_RCMR_START_RF_LEVEL (0x6u << 8) //!< (SSC_RCMR) Detection of any level change on RF signal
#define   SSC_RCMR_START_RF_EDGE (0x7u << 8) //!< (SSC_RCMR) Detection of any edge on RF signal
#define   SSC_RCMR_START_CMP_0 (0x8u << 8) //!< (SSC_RCMR) Compare 0
#define SSC_RCMR_STOP (0x1u << 12) //!< (SSC_RCMR) Receive Stop Selection
#define SSC_RCMR_STTDLY_Pos 16	 //!< doc!
#define SSC_RCMR_STTDLY_Msk (0xffu << SSC_RCMR_STTDLY_Pos) //!< (SSC_RCMR) Receive Start Delay
#define SSC_RCMR_STTDLY(value) ((SSC_RCMR_STTDLY_Msk & ((value) << SSC_RCMR_STTDLY_Pos)))	 //!< doc!
#define SSC_RCMR_PERIOD_Pos 24	 //!< doc!
#define SSC_RCMR_PERIOD_Msk (0xffu << SSC_RCMR_PERIOD_Pos) //!< (SSC_RCMR) Receive Period Divider Selection
#define SSC_RCMR_PERIOD(value) ((SSC_RCMR_PERIOD_Msk & ((value) << SSC_RCMR_PERIOD_Pos)))	 //!< doc!
/* -------- SSC_RFMR : (SSC Offset: 0x14) Receive Frame Mode Register -------- */
#define SSC_RFMR_DATLEN_Pos 0	 //!< doc!
#define SSC_RFMR_DATLEN_Msk (0x1fu << SSC_RFMR_DATLEN_Pos) //!< (SSC_RFMR) Data Length
#define SSC_RFMR_DATLEN(value) ((SSC_RFMR_DATLEN_Msk & ((value) << SSC_RFMR_DATLEN_Pos)))	 //!< doc!
#define SSC_RFMR_LOOP (0x1u << 5) //!< (SSC_RFMR) Loop Mode
#define SSC_RFMR_MSBF (0x1u << 7) //!< (SSC_RFMR) Most Significant Bit First
#define SSC_RFMR_DATNB_Pos 8	 //!< doc!
#define SSC_RFMR_DATNB_Msk (0xfu << SSC_RFMR_DATNB_Pos) //!< (SSC_RFMR) Data Number per Frame
#define SSC_RFMR_DATNB(value) ((SSC_RFMR_DATNB_Msk & ((value) << SSC_RFMR_DATNB_Pos)))	 //!< doc!
#define SSC_RFMR_FSLEN_Pos 16	 //!< doc!
#define SSC_RFMR_FSLEN_Msk (0xfu << SSC_RFMR_FSLEN_Pos) //!< (SSC_RFMR) Receive Frame Sync Length
#define SSC_RFMR_FSLEN(value) ((SSC_RFMR_FSLEN_Msk & ((value) << SSC_RFMR_FSLEN_Pos)))	 //!< doc!
#define SSC_RFMR_FSOS_Pos 20	 //!< doc!
#define SSC_RFMR_FSOS_Msk (0x7u << SSC_RFMR_FSOS_Pos) //!< (SSC_RFMR) Receive Frame Sync Output Selection
#define   SSC_RFMR_FSOS_NONE (0x0u << 20) //!< (SSC_RFMR) None
#define   SSC_RFMR_FSOS_NEGATIVE (0x1u << 20) //!< (SSC_RFMR) Negative Pulse
#define   SSC_RFMR_FSOS_POSITIVE (0x2u << 20) //!< (SSC_RFMR) Positive Pulse
#define   SSC_RFMR_FSOS_LOW (0x3u << 20) //!< (SSC_RFMR) Driven Low during data transfer
#define   SSC_RFMR_FSOS_HIGH (0x4u << 20) //!< (SSC_RFMR) Driven High during data transfer
#define   SSC_RFMR_FSOS_TOGGLING (0x5u << 20) //!< (SSC_RFMR) Toggling at each start of data transfer
#define SSC_RFMR_FSEDGE (0x1u << 24) //!< (SSC_RFMR) Frame Sync Edge Detection
#define   SSC_RFMR_FSEDGE_POSITIVE (0x0u << 24) //!< (SSC_RFMR) Positive Edge Detection
#define   SSC_RFMR_FSEDGE_NEGATIVE (0x1u << 24) //!< (SSC_RFMR) Negative Edge Detection
#define SSC_RFMR_FSLEN_EXT_Pos 28	 //!< doc!
#define SSC_RFMR_FSLEN_EXT_Msk (0xfu << SSC_RFMR_FSLEN_EXT_Pos) //!< (SSC_RFMR) FSLEN Field Extension
#define SSC_RFMR_FSLEN_EXT(value) ((SSC_RFMR_FSLEN_EXT_Msk & ((value) << SSC_RFMR_FSLEN_EXT_Pos)))	 //!< doc!
/* -------- SSC_TCMR : (SSC Offset: 0x18) Transmit Clock Mode Register -------- */
#define SSC_TCMR_CKS_Pos 0	 //!< doc!
#define SSC_TCMR_CKS_Msk (0x3u << SSC_TCMR_CKS_Pos) //!< (SSC_TCMR) Transmit Clock Selection
#define   SSC_TCMR_CKS_MCK (0x0u << 0) //!< (SSC_TCMR) Divided Clock
#define   SSC_TCMR_CKS_TK (0x1u << 0) //!< (SSC_TCMR) TK Clock signal
#define   SSC_TCMR_CKS_RK (0x2u << 0) //!< (SSC_TCMR) RK pin
#define SSC_TCMR_CKO_Pos 2	 //!< doc!
#define SSC_TCMR_CKO_Msk (0x7u << SSC_TCMR_CKO_Pos) //!< (SSC_TCMR) Transmit Clock Output Mode Selection
#define   SSC_TCMR_CKO_NONE (0x0u << 2) //!< (SSC_TCMR) None
#define   SSC_TCMR_CKO_CONTINUOUS (0x1u << 2) //!< (SSC_TCMR) Continuous Receive Clock
#define   SSC_TCMR_CKO_TRANSFER (0x2u << 2) //!< (SSC_TCMR) Transmit Clock only during data transfers
#define SSC_TCMR_CKI (0x1u << 5) //!< (SSC_TCMR) Transmit Clock Inversion
#define SSC_TCMR_CKG_Pos 6	 //!< doc!
#define SSC_TCMR_CKG_Msk (0x3u << SSC_TCMR_CKG_Pos) //!< (SSC_TCMR) Transmit Clock Gating Selection
#define   SSC_TCMR_CKG_NONE (0x0u << 6) //!< (SSC_TCMR) None
#define   SSC_TCMR_CKG_CONTINUOUS (0x1u << 6) //!< (SSC_TCMR) Transmit Clock enabled only if TF Low
#define   SSC_TCMR_CKG_TRANSFER (0x2u << 6) //!< (SSC_TCMR) Transmit Clock enabled only if TF High
#define SSC_TCMR_START_Pos 8	 //!< doc!
#define SSC_TCMR_START_Msk (0xfu << SSC_TCMR_START_Pos) //!< (SSC_TCMR) Transmit Start Selection
#define   SSC_TCMR_START_CONTINUOUS (0x0u << 8) //!< (SSC_TCMR) Continuous, as
				//!< soon as a word is written in the SSC_THR Register (if
				//!< Transmit is enabled), and immediately after the end of transfer of the previous data.
#define   SSC_TCMR_START_RECEIVE (0x1u << 8) //!< (SSC_TCMR) Receive start
#define   SSC_TCMR_START_RF_LOW (0x2u << 8) //!< (SSC_TCMR) Detection of a low level on TF signal
#define   SSC_TCMR_START_RF_HIGH (0x3u << 8) //!< (SSC_TCMR) Detection of a high level on TF signal
#define   SSC_TCMR_START_RF_FALLING (0x4u << 8) //!< (SSC_TCMR) Detection of a falling edge on TF signal
#define   SSC_TCMR_START_RF_RISING (0x5u << 8) //!< (SSC_TCMR) Detection of a rising edge on TF signal
#define   SSC_TCMR_START_RF_LEVEL (0x6u << 8) //!< (SSC_TCMR) Detection of any level change on TF signal
#define   SSC_TCMR_START_RF_EDGE (0x7u << 8) //!< (SSC_TCMR) Detection of any edge on TF signal
#define   SSC_TCMR_START_CMP_0 (0x8u << 8) //!< (SSC_TCMR) Compare 0
#define SSC_TCMR_STTDLY_Pos 16	 //!< doc!
#define SSC_TCMR_STTDLY_Msk (0xffu << SSC_TCMR_STTDLY_Pos) //!< (SSC_TCMR) Transmit Start Delay
#define SSC_TCMR_STTDLY(value) ((SSC_TCMR_STTDLY_Msk & ((value) << SSC_TCMR_STTDLY_Pos)))	 //!< doc!
#define SSC_TCMR_PERIOD_Pos 24	 //!< doc!
#define SSC_TCMR_PERIOD_Msk (0xffu << SSC_TCMR_PERIOD_Pos) //!< (SSC_TCMR) Transmit Period Divider Selection
#define SSC_TCMR_PERIOD(value) ((SSC_TCMR_PERIOD_Msk & ((value) << SSC_TCMR_PERIOD_Pos)))	 //!< doc!
/* -------- SSC_TFMR : (SSC Offset: 0x1C) Transmit Frame Mode Register -------- */
#define SSC_TFMR_DATLEN_Pos 0	 //!< doc!
#define SSC_TFMR_DATLEN_Msk (0x1fu << SSC_TFMR_DATLEN_Pos) //!< (SSC_TFMR) Data Length
#define SSC_TFMR_DATLEN(value) ((SSC_TFMR_DATLEN_Msk & ((value) << SSC_TFMR_DATLEN_Pos)))	 //!< doc!
#define SSC_TFMR_DATDEF (0x1u << 5) //!< (SSC_TFMR) Data Default Value
#define SSC_TFMR_MSBF (0x1u << 7) //!< (SSC_TFMR) Most Significant Bit First
#define SSC_TFMR_DATNB_Pos 8	 //!< doc!
#define SSC_TFMR_DATNB_Msk (0xfu << SSC_TFMR_DATNB_Pos) //!< (SSC_TFMR) Data Number per frame
#define SSC_TFMR_DATNB(value) ((SSC_TFMR_DATNB_Msk & ((value) << SSC_TFMR_DATNB_Pos)))	 //!< doc!
#define SSC_TFMR_FSLEN_Pos 16	 //!< doc!
#define SSC_TFMR_FSLEN_Msk (0xfu << SSC_TFMR_FSLEN_Pos) //!< (SSC_TFMR) Transmit Frame Sync Length
#define SSC_TFMR_FSLEN(value) ((SSC_TFMR_FSLEN_Msk & ((value) << SSC_TFMR_FSLEN_Pos)))	 //!< doc!
#define SSC_TFMR_FSOS_Pos 20	 //!< doc!
#define SSC_TFMR_FSOS_Msk (0x7u << SSC_TFMR_FSOS_Pos) //!< (SSC_TFMR) Transmit Frame Sync Output Selection
#define   SSC_TFMR_FSOS_NONE (0x0u << 20) //!< (SSC_TFMR) None
#define   SSC_TFMR_FSOS_NEGATIVE (0x1u << 20) //!< (SSC_TFMR) Negative Pulse
#define   SSC_TFMR_FSOS_POSITIVE (0x2u << 20) //!< (SSC_TFMR) Positive Pulse
#define   SSC_TFMR_FSOS_LOW (0x3u << 20) //!< (SSC_TFMR) Driven Low during data transfer
#define   SSC_TFMR_FSOS_HIGH (0x4u << 20) //!< (SSC_TFMR) Driven High during data transfer
#define   SSC_TFMR_FSOS_TOGGLING (0x5u << 20) //!< (SSC_TFMR) Toggling at each start of data transfer
#define SSC_TFMR_FSDEN (0x1u << 23) //!< (SSC_TFMR) Frame Sync Data Enable
#define SSC_TFMR_FSEDGE (0x1u << 24) //!< (SSC_TFMR) Frame Sync Edge Detection
#define   SSC_TFMR_FSEDGE_POSITIVE (0x0u << 24) //!< (SSC_TFMR) Positive Edge Detection
#define   SSC_TFMR_FSEDGE_NEGATIVE (0x1u << 24) //!< (SSC_TFMR) Negative Edge Detection
#define SSC_TFMR_FSLEN_EXT_Pos 28	 //!< doc!
#define SSC_TFMR_FSLEN_EXT_Msk (0xfu << SSC_TFMR_FSLEN_EXT_Pos) //!< (SSC_TFMR) FSLEN Field Extension
#define SSC_TFMR_FSLEN_EXT(value) ((SSC_TFMR_FSLEN_EXT_Msk & ((value) << SSC_TFMR_FSLEN_EXT_Pos)))	 //!< doc!
/* -------- SSC_RHR : (SSC Offset: 0x20) Receive Holding Register -------- */
#define SSC_RHR_RDAT_Pos 0	 //!< doc!
#define SSC_RHR_RDAT_Msk (0xffffffffu << SSC_RHR_RDAT_Pos) //!< (SSC_RHR) Receive Data
/* -------- SSC_THR : (SSC Offset: 0x24) Transmit Holding Register -------- */
#define SSC_THR_TDAT_Pos 0	 //!< doc!
#define SSC_THR_TDAT_Msk (0xffffffffu << SSC_THR_TDAT_Pos) //!< (SSC_THR) Transmit Data
#define SSC_THR_TDAT(value) ((SSC_THR_TDAT_Msk & ((value) << SSC_THR_TDAT_Pos)))	 //!< doc!
/* -------- SSC_RSHR : (SSC Offset: 0x30) Receive Sync. Holding Register -------- */
#define SSC_RSHR_RSDAT_Pos 0	 //!< doc!
#define SSC_RSHR_RSDAT_Msk (0xffffu << SSC_RSHR_RSDAT_Pos) //!< (SSC_RSHR) Receive Synchronization Data
/* -------- SSC_TSHR : (SSC Offset: 0x34) Transmit Sync. Holding Register -------- */
#define SSC_TSHR_TSDAT_Pos 0	 //!< doc!
#define SSC_TSHR_TSDAT_Msk (0xffffu << SSC_TSHR_TSDAT_Pos) //!< (SSC_TSHR) Transmit Synchronization Data
#define SSC_TSHR_TSDAT(value) ((SSC_TSHR_TSDAT_Msk & ((value) << SSC_TSHR_TSDAT_Pos)))	 //!< doc!
/* -------- SSC_RC0R : (SSC Offset: 0x38) Receive Compare 0 Register -------- */
#define SSC_RC0R_CP0_Pos 0	 //!< doc!
#define SSC_RC0R_CP0_Msk (0xffffu << SSC_RC0R_CP0_Pos) //!< (SSC_RC0R) Receive Compare Data 0
#define SSC_RC0R_CP0(value) ((SSC_RC0R_CP0_Msk & ((value) << SSC_RC0R_CP0_Pos)))	 //!< doc!
/* -------- SSC_RC1R : (SSC Offset: 0x3C) Receive Compare 1 Register -------- */
#define SSC_RC1R_CP1_Pos 0	 //!< doc!
#define SSC_RC1R_CP1_Msk (0xffffu << SSC_RC1R_CP1_Pos) //!< (SSC_RC1R) Receive Compare Data 1
#define SSC_RC1R_CP1(value) ((SSC_RC1R_CP1_Msk & ((value) << SSC_RC1R_CP1_Pos)))	 //!< doc!
/* -------- SSC_SR : (SSC Offset: 0x40) Status Register -------- */
#define SSC_SR_TXRDY (0x1u << 0) //!< (SSC_SR) Transmit Ready
#define SSC_SR_TXEMPTY (0x1u << 1) //!< (SSC_SR) Transmit Empty
#define SSC_SR_ENDTX (0x1u << 2) //!< (SSC_SR) End of Transmission
#define SSC_SR_TXBUFE (0x1u << 3) //!< (SSC_SR) Transmit Buffer Empty
#define SSC_SR_RXRDY (0x1u << 4) //!< (SSC_SR) Receive Ready
#define SSC_SR_OVRUN (0x1u << 5) //!< (SSC_SR) Receive Overrun
#define SSC_SR_ENDRX (0x1u << 6) //!< (SSC_SR) End of Reception
#define SSC_SR_RXBUFF (0x1u << 7) //!< (SSC_SR) Receive Buffer Full
#define SSC_SR_CP0 (0x1u << 8) //!< (SSC_SR) Compare 0
#define SSC_SR_CP1 (0x1u << 9) //!< (SSC_SR) Compare 1
#define SSC_SR_TXSYN (0x1u << 10) //!< (SSC_SR) Transmit Sync
#define SSC_SR_RXSYN (0x1u << 11) //!< (SSC_SR) Receive Sync
#define SSC_SR_TXEN (0x1u << 16) //!< (SSC_SR) Transmit Enable
#define SSC_SR_RXEN (0x1u << 17) //!< (SSC_SR) Receive Enable
/* -------- SSC_IER : (SSC Offset: 0x44) Interrupt Enable Register -------- */
#define SSC_IER_TXRDY (0x1u << 0) //!< (SSC_IER) Transmit Ready Interrupt Enable
#define SSC_IER_TXEMPTY (0x1u << 1) //!< (SSC_IER) Transmit Empty Interrupt Enable
#define SSC_IER_ENDTX (0x1u << 2) //!< (SSC_IER) End of Transmission Interrupt Enable
#define SSC_IER_TXBUFE (0x1u << 3) //!< (SSC_IER) Transmit Buffer Empty Interrupt Enable
#define SSC_IER_RXRDY (0x1u << 4) //!< (SSC_IER) Receive Ready Interrupt Enable
#define SSC_IER_OVRUN (0x1u << 5) //!< (SSC_IER) Receive Overrun Interrupt Enable
#define SSC_IER_ENDRX (0x1u << 6) //!< (SSC_IER) End of Reception Interrupt Enable
#define SSC_IER_RXBUFF (0x1u << 7) //!< (SSC_IER) Receive Buffer Full Interrupt Enable
#define SSC_IER_CP0 (0x1u << 8) //!< (SSC_IER) Compare 0 Interrupt Enable
#define SSC_IER_CP1 (0x1u << 9) //!< (SSC_IER) Compare 1 Interrupt Enable
#define SSC_IER_TXSYN (0x1u << 10) //!< (SSC_IER) Tx Sync Interrupt Enable
#define SSC_IER_RXSYN (0x1u << 11) //!< (SSC_IER) Rx Sync Interrupt Enable
/* -------- SSC_IDR : (SSC Offset: 0x48) Interrupt Disable Register -------- */
#define SSC_IDR_TXRDY (0x1u << 0) //!< (SSC_IDR) Transmit Ready Interrupt Disable
#define SSC_IDR_TXEMPTY (0x1u << 1) //!< (SSC_IDR) Transmit Empty Interrupt Disable
#define SSC_IDR_ENDTX (0x1u << 2) //!< (SSC_IDR) End of Transmission Interrupt Disable
#define SSC_IDR_TXBUFE (0x1u << 3) //!< (SSC_IDR) Transmit Buffer Empty Interrupt Disable
#define SSC_IDR_RXRDY (0x1u << 4) //!< (SSC_IDR) Receive Ready Interrupt Disable
#define SSC_IDR_OVRUN (0x1u << 5) //!< (SSC_IDR) Receive Overrun Interrupt Disable
#define SSC_IDR_ENDRX (0x1u << 6) //!< (SSC_IDR) End of Reception Interrupt Disable
#define SSC_IDR_RXBUFF (0x1u << 7) //!< (SSC_IDR) Receive Buffer Full Interrupt Disable
#define SSC_IDR_CP0 (0x1u << 8) //!< (SSC_IDR) Compare 0 Interrupt Disable
#define SSC_IDR_CP1 (0x1u << 9) //!< (SSC_IDR) Compare 1 Interrupt Disable
#define SSC_IDR_TXSYN (0x1u << 10) //!< (SSC_IDR) Tx Sync Interrupt Enable
#define SSC_IDR_RXSYN (0x1u << 11) //!< (SSC_IDR) Rx Sync Interrupt Enable
/* -------- SSC_IMR : (SSC Offset: 0x4C) Interrupt Mask Register -------- */
#define SSC_IMR_TXRDY (0x1u << 0) //!< (SSC_IMR) Transmit Ready Interrupt Mask
#define SSC_IMR_TXEMPTY (0x1u << 1) //!< (SSC_IMR) Transmit Empty Interrupt Mask
#define SSC_IMR_ENDTX (0x1u << 2) //!< (SSC_IMR) End of Transmission Interrupt Mask
#define SSC_IMR_TXBUFE (0x1u << 3) //!< (SSC_IMR) Transmit Buffer Empty Interrupt Mask
#define SSC_IMR_RXRDY (0x1u << 4) //!< (SSC_IMR) Receive Ready Interrupt Mask
#define SSC_IMR_OVRUN (0x1u << 5) //!< (SSC_IMR) Receive Overrun Interrupt Mask
#define SSC_IMR_ENDRX (0x1u << 6) //!< (SSC_IMR) End of Reception Interrupt Mask
#define SSC_IMR_RXBUFF (0x1u << 7) //!< (SSC_IMR) Receive Buffer Full Interrupt Mask
#define SSC_IMR_CP0 (0x1u << 8) //!< (SSC_IMR) Compare 0 Interrupt Mask
#define SSC_IMR_CP1 (0x1u << 9) //!< (SSC_IMR) Compare 1 Interrupt Mask
#define SSC_IMR_TXSYN (0x1u << 10) //!< (SSC_IMR) Tx Sync Interrupt Mask
#define SSC_IMR_RXSYN (0x1u << 11) //!< (SSC_IMR) Rx Sync Interrupt Mask
/* -------- SSC_WPMR : (SSC Offset: 0xE4) Write Protect Mode Register -------- */
#define SSC_WPMR_WPEN (0x1u << 0) //!< (SSC_WPMR) Write Protect Enable
#define SSC_WPMR_WPKEY_Pos 8	 //!< doc!
#define SSC_WPMR_WPKEY_Msk (0xffffffu << SSC_WPMR_WPKEY_Pos) //!< (SSC_WPMR) Write Protect KEY
#define SSC_WPMR_WPKEY(value) ((SSC_WPMR_WPKEY_Msk & ((value) << SSC_WPMR_WPKEY_Pos)))	 //!< doc!
/* -------- SSC_WPSR : (SSC Offset: 0xE8) Write Protect Status Register -------- */
#define SSC_WPSR_WPVS (0x1u << 0) //!< (SSC_WPSR) Write Protect Violation Status
#define SSC_WPSR_WPVSRC_Pos 8	 //!< doc!
#define SSC_WPSR_WPVSRC_Msk (0xffffu << SSC_WPSR_WPVSRC_Pos) //!< (SSC_WPSR) Write Protect Violation Source
/* -------- SSC_RPR : (SSC Offset: 0x100) Receive Pointer Register -------- */
#define SSC_RPR_RXPTR_Pos 0	 //!< doc!
#define SSC_RPR_RXPTR_Msk (0xffffffffu << SSC_RPR_RXPTR_Pos) //!< (SSC_RPR) Receive Pointer Register
#define SSC_RPR_RXPTR(value) ((SSC_RPR_RXPTR_Msk & ((value) << SSC_RPR_RXPTR_Pos)))	 //!< doc!
/* -------- SSC_RCR : (SSC Offset: 0x104) Receive Counter Register -------- */
#define SSC_RCR_RXCTR_Pos 0	 //!< doc!
#define SSC_RCR_RXCTR_Msk (0xffffu << SSC_RCR_RXCTR_Pos) //!< (SSC_RCR) Receive Counter Register
#define SSC_RCR_RXCTR(value) ((SSC_RCR_RXCTR_Msk & ((value) << SSC_RCR_RXCTR_Pos)))	 //!< doc!
/* -------- SSC_TPR : (SSC Offset: 0x108) Transmit Pointer Register -------- */
#define SSC_TPR_TXPTR_Pos 0	 //!< doc!
#define SSC_TPR_TXPTR_Msk (0xffffffffu << SSC_TPR_TXPTR_Pos) //!< (SSC_TPR) Transmit Counter Register
#define SSC_TPR_TXPTR(value) ((SSC_TPR_TXPTR_Msk & ((value) << SSC_TPR_TXPTR_Pos)))	 //!< doc!
/* -------- SSC_TCR : (SSC Offset: 0x10C) Transmit Counter Register -------- */
#define SSC_TCR_TXCTR_Pos 0	 //!< doc!
#define SSC_TCR_TXCTR_Msk (0xffffu << SSC_TCR_TXCTR_Pos) //!< (SSC_TCR) Transmit Counter Register
#define SSC_TCR_TXCTR(value) ((SSC_TCR_TXCTR_Msk & ((value) << SSC_TCR_TXCTR_Pos)))	 //!< doc!
/* -------- SSC_RNPR : (SSC Offset: 0x110) Receive Next Pointer Register -------- */
#define SSC_RNPR_RXNPTR_Pos 0	 //!< doc!
#define SSC_RNPR_RXNPTR_Msk (0xffffffffu << SSC_RNPR_RXNPTR_Pos) //!< (SSC_RNPR) Receive Next Pointer
#define SSC_RNPR_RXNPTR(value) ((SSC_RNPR_RXNPTR_Msk & ((value) << SSC_RNPR_RXNPTR_Pos)))	 //!< doc!
/* -------- SSC_RNCR : (SSC Offset: 0x114) Receive Next Counter Register -------- */
#define SSC_RNCR_RXNCTR_Pos 0	 //!< doc!
#define SSC_RNCR_RXNCTR_Msk (0xffffu << SSC_RNCR_RXNCTR_Pos) //!< (SSC_RNCR) Receive Next Counter
#define SSC_RNCR_RXNCTR(value) ((SSC_RNCR_RXNCTR_Msk & ((value) << SSC_RNCR_RXNCTR_Pos)))	 //!< doc!
/* -------- SSC_TNPR : (SSC Offset: 0x118) Transmit Next Pointer Register -------- */
#define SSC_TNPR_TXNPTR_Pos 0	 //!< doc!
#define SSC_TNPR_TXNPTR_Msk (0xffffffffu << SSC_TNPR_TXNPTR_Pos) //!< (SSC_TNPR) Transmit Next Pointer
#define SSC_TNPR_TXNPTR(value) ((SSC_TNPR_TXNPTR_Msk & ((value) << SSC_TNPR_TXNPTR_Pos)))	 //!< doc!
/* -------- SSC_TNCR : (SSC Offset: 0x11C) Transmit Next Counter Register -------- */
#define SSC_TNCR_TXNCTR_Pos 0	 //!< doc!
#define SSC_TNCR_TXNCTR_Msk (0xffffu << SSC_TNCR_TXNCTR_Pos) //!< (SSC_TNCR) Transmit Counter Next
#define SSC_TNCR_TXNCTR(value) ((SSC_TNCR_TXNCTR_Msk & ((value) << SSC_TNCR_TXNCTR_Pos)))	 //!< doc!
/* -------- SSC_PTCR : (SSC Offset: 0x120) Transfer Control Register -------- */
#define SSC_PTCR_RXTEN (0x1u << 0) //!< (SSC_PTCR) Receiver Transfer Enable
#define SSC_PTCR_RXTDIS (0x1u << 1) //!< (SSC_PTCR) Receiver Transfer Disable
#define SSC_PTCR_TXTEN (0x1u << 8) //!< (SSC_PTCR) Transmitter Transfer Enable
#define SSC_PTCR_TXTDIS (0x1u << 9) //!< (SSC_PTCR) Transmitter Transfer Disable
/* -------- SSC_PTSR : (SSC Offset: 0x124) Transfer Status Register -------- */
#define SSC_PTSR_RXTEN (0x1u << 0) //!< (SSC_PTSR) Receiver Transfer Enable
#define SSC_PTSR_TXTEN (0x1u << 8) //!< (SSC_PTSR) Transmitter Transfer Enable



/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/
extern void SSC_Configure(uint32_t bitRate, uint32_t masterClock);
extern void SSC_ConfigureTransmitter(uint32_t tcmr, uint32_t tfmr);
extern void SSC_ConfigureReceiver(uint32_t rcmr, uint32_t rfmr);
extern void SSC_EnableTransmitter(void);
extern void SSC_DisableTransmitter(void);
extern void SSC_EnableReceiver(void);
extern void SSC_DisableReceiver(void);
extern void SSC_EnableInterrupts(uint32_t sources);
extern void SSC_DisableInterrupts(uint32_t sources);
extern void SSC_Write(uint32_t frame);
extern uint32_t SSC_Read(void);
extern uint8_t SSC_WriteBuffer(void *buffer, uint32_t length);
extern uint8_t SSC_ReadBuffer(void *buffer, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SSC_ */

