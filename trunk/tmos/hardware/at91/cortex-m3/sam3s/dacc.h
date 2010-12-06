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
 * @ingroup	 hardware_sam3s
 * @defgroup  SAM3S_DACC Analog-to-Digital Converter (DACC)
 * Interface for configuration the Analog-to-Digital Converter (DACC) peripheral.
 * @{
 *
 *  \section Usage
 *
 *  -# Configurate the pins for DACC
 *  -# Initialize the DACC with DACC_Initialize().
 *  -# Select the active channel using DACC_EnableChannel()
 *  -# Start the conversion with DACC_StartConversion()
 *  -# Wait the end of the conversion by polling status with DACC_GetStatus()
 *  -# Finally, get the converted data using DACC_GetConvertedData()
 *
 *  \section dacc_module Working with DACC
 * The DACC driver provides the interface to configure and use the DACC peripheral.\n
 *
 * The DACC(Digital-to-Analog Converter Controller) converts digital code to analog output.
 * The data to be converted are sent in a common register for all channels. It offers up to 2
 * analog outputs.The output voltage ranges from (1/6)ADVREF to (5/6)ADVREF.
 *
 * To Enable a DACC conversion,the user has to follow these few steps:
 * <ul>
 * <li> Select an appropriate reference voltage on ADVREF   </li>
 * <li> Configure the DACC according to its requirements and special needs,which could be
        broken down into several parts:
 * -#   Enable DACC in free running mode by clearing TRGEN in DACC_MR;
 * -#   Configure Startup Time and Refresh Period through setting STARTUP and REFRESH fields
 *      in DACC_MR; The refresh mechanism is used to protect the output analog value from
 *      decreasing.
 * -#   Enable channels and write digital code to DACC_CDR,in free running mode, the conversion
 *      is started right after at least one channel is enabled and data is written .
   </li>
 * </ul>
 *
 * For more accurate information, please look at the DACC section of the
 * Datasheet.
 *
 *
 * @file     hardware/at91/cortex-m3/sam3s/dacc.h
 * @ingroup	 SAM3S_DACC
 * @brief    DACC header
 *
 * @}
*/

#ifndef _DACC_
#define _DACC_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
//#include "chip.h"
//#include <stdint.h>
//#include <assert.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/
#define DACC_CHANNEL_0 0	//!< doc!
#define DACC_CHANNEL_1 1	//!< doc!

#ifdef __cplusplus
 extern "C" {
#endif

 /* ============================================================================= */
 /**  SOFTWARE API DEFINITION FOR Digital-to-Analog Converter Controller */
 /* ============================================================================= */

 #ifndef __ASSEMBLY__
 /** \brief Dacc hardware registers */
 typedef struct {
   WoReg DACC_CR;       //!< (Dacc Offset: 0x00) Control Register
   RwReg DACC_MR;       //!< (Dacc Offset: 0x04) Mode Register
   RwReg Reserved1[2];	//!< doc!
   WoReg DACC_CHER;     //!< (Dacc Offset: 0x10) Channel Enable Register
   WoReg DACC_CHDR;     //!< (Dacc Offset: 0x14) Channel Disable Register
   RoReg DACC_CHSR;     //!< (Dacc Offset: 0x18) Channel Status Register
   RwReg Reserved2[1];	//!< doc!
   WoReg DACC_CDR;      //!< (Dacc Offset: 0x20) Conversion Data Register
   WoReg DACC_IER;      //!< (Dacc Offset: 0x24) Interrupt Enable Register
   WoReg DACC_IDR;      //!< (Dacc Offset: 0x28) Interrupt Disable Register
   RoReg DACC_IMR;      //!< (Dacc Offset: 0x2C) Interrupt Mask Register
   RoReg DACC_ISR;      //!< (Dacc Offset: 0x30) Interrupt Status Register
   RwReg Reserved3[24];	//!< doc!
   RwReg DACC_ACR;      //!< (Dacc Offset: 0x94) Analog Current Register
   RwReg Reserved4[19];	//!< doc!
   RwReg DACC_WPMR;     //!< (Dacc Offset: 0xE4) Write Protect Mode register
   RoReg DACC_WPSR;     //!< (Dacc Offset: 0xE8) Write Protect Status register
   RwReg Reserved5[5];	//!< doc!
   RwReg DACC_RPR;      //!< (Dacc Offset: 0x100) Receive Pointer Register
   RwReg DACC_RCR;      //!< (Dacc Offset: 0x104) Receive Counter Register
   RwReg DACC_TPR;      //!< (Dacc Offset: 0x108) Transmit Pointer Register
   RwReg DACC_TCR;      //!< (Dacc Offset: 0x10C) Transmit Counter Register
   RwReg DACC_RNPR;     //!< (Dacc Offset: 0x110) Receive Next Pointer Register
   RwReg DACC_RNCR;     //!< (Dacc Offset: 0x114) Receive Next Counter Register
   RwReg DACC_TNPR;     //!< (Dacc Offset: 0x118) Transmit Next Pointer Register
   RwReg DACC_TNCR;     //!< (Dacc Offset: 0x11C) Transmit Next Counter Register
   WoReg DACC_PTCR;     //!< (Dacc Offset: 0x120) Transfer Control Register
   RoReg DACC_PTSR;     //!< (Dacc Offset: 0x124) Transfer Status Register
 } Dacc;
 #endif /* __ASSEMBLY__ */
 /* -------- DACC_CR : (DACC Offset: 0x00) Control Register -------- */
 #define DACC_CR_SWRST (0x1u << 0) //!< (DACC_CR) Software Reset
 /* -------- DACC_MR : (DACC Offset: 0x04) Mode Register -------- */
 #define DACC_MR_TRGEN (0x1u << 0) //!< (DACC_MR) Trigger Enable
 #define   DACC_MR_TRGEN_DIS (0x0u << 0) //!< (DACC_MR) External trigger mode disabled. DACC in free running mode.
 #define   DACC_MR_TRGEN_EN (0x1u << 0) //!< (DACC_MR) External trigger mode enabled.
 #define DACC_MR_TRGSEL_Pos 1	//!< doc!
 #define DACC_MR_TRGSEL_Msk (0x7u << DACC_MR_TRGSEL_Pos) //!< (DACC_MR) Trigger Selection
 #define DACC_MR_TRGSEL(value) ((DACC_MR_TRGSEL_Msk & ((value) << DACC_MR_TRGSEL_Pos)))	//!< doc!
 #define DACC_MR_WORD (0x1u << 4) //!< (DACC_MR) Word Transfer
 #define   DACC_MR_WORD_HALF (0x0u << 4) //!< (DACC_MR) Half-Word transfer
 #define   DACC_MR_WORD_WORD (0x1u << 4) //!< (DACC_MR) Word Transfer
 #define DACC_MR_SLEEP (0x1u << 5) //!< (DACC_MR) Sleep Mode
 #define DACC_MR_FASTWKUP (0x1u << 6) //!< (DACC_MR) Fast Wake up Mode
 #define DACC_MR_REFRESH_Pos 8	//!< doc!
 #define DACC_MR_REFRESH_Msk (0xffu << DACC_MR_REFRESH_Pos) //!< (DACC_MR) Refresh Period
 #define DACC_MR_REFRESH(value) ((DACC_MR_REFRESH_Msk & ((value) << DACC_MR_REFRESH_Pos)))	//!< doc!
 #define DACC_MR_USER_SEL_Pos 16	//!< doc!
 #define DACC_MR_USER_SEL_Msk (0x3u << DACC_MR_USER_SEL_Pos) //!< (DACC_MR) User Channel Selection
 #define   DACC_MR_USER_SEL_CHANNEL0 (0x0u << 16) //!< (DACC_MR) Channel 0
 #define   DACC_MR_USER_SEL_CHANNEL1 (0x1u << 16) //!< (DACC_MR) Channel 1
 #define DACC_MR_TAG (0x1u << 20) //!< (DACC_MR) Tag Selection Mode
 #define   DACC_MR_TAG_DIS (0x0u << 20) //!< (DACC_MR) Tag selection mode disabled. Using USER_SEL to select the channel for the conversion
 #define   DACC_MR_TAG_EN (0x1u << 20) //!< (DACC_MR) Tag selection mode enabled
 #define DACC_MR_MAXS (0x1u << 21) //!< (DACC_MR) Max Speed Mode
 #define DACC_MR_STARTUP_Pos 24	//!< doc!
 #define DACC_MR_STARTUP_Msk (0x3fu << DACC_MR_STARTUP_Pos) //!< (DACC_MR) Startup Time Selection
 #define   DACC_MR_STARTUP_0 (0x0u << 24) //!< (DACC_MR) 0 periods of DACClock
 #define   DACC_MR_STARTUP_8 (0x1u << 24) //!< (DACC_MR) 8 periods of DACClock
 #define   DACC_MR_STARTUP_16 (0x2u << 24) //!< (DACC_MR) 16 periods of DACClock
 #define   DACC_MR_STARTUP_24 (0x3u << 24) //!< (DACC_MR) 24 periods of DACClock
 #define   DACC_MR_STARTUP_64 (0x4u << 24) //!< (DACC_MR) 64 periods of DACClock
 #define   DACC_MR_STARTUP_80 (0x5u << 24) //!< (DACC_MR) 80 periods of DACClock
 #define   DACC_MR_STARTUP_96 (0x6u << 24) //!< (DACC_MR) 96 periods of DACClock
 #define   DACC_MR_STARTUP_112 (0x7u << 24) //!< (DACC_MR) 112 periods of DACClock
 #define   DACC_MR_STARTUP_512 (0x8u << 24) //!< (DACC_MR) 512 periods of DACClock
 #define   DACC_MR_STARTUP_576 (0x9u << 24) //!< (DACC_MR) 576 periods of DACClock
 #define   DACC_MR_STARTUP_640 (0xAu << 24) //!< (DACC_MR) 640 periods of DACClock
 #define   DACC_MR_STARTUP_704 (0xBu << 24) //!< (DACC_MR) 704 periods of DACClock
 #define   DACC_MR_STARTUP_768 (0xCu << 24) //!< (DACC_MR) 768 periods of DACClock
 #define   DACC_MR_STARTUP_832 (0xDu << 24) //!< (DACC_MR) 832 periods of DACClock
 #define   DACC_MR_STARTUP_896 (0xEu << 24) //!< (DACC_MR) 896 periods of DACClock
 #define   DACC_MR_STARTUP_960 (0xFu << 24) //!< (DACC_MR) 960 periods of DACClock
 /* -------- DACC_CHER : (DACC Offset: 0x10) Channel Enable Register -------- */
 #define DACC_CHER_CH0 (0x1u << 0) //!< (DACC_CHER) Channel 0 Enable
 #define DACC_CHER_CH1 (0x1u << 1) //!< (DACC_CHER) Channel 1 Enable
 /* -------- DACC_CHDR : (DACC Offset: 0x14) Channel Disable Register -------- */
 #define DACC_CHDR_CH0 (0x1u << 0) //!< (DACC_CHDR) Channel 0 Disable
 #define DACC_CHDR_CH1 (0x1u << 1) //!< (DACC_CHDR) Channel 1 Disable
 /* -------- DACC_CHSR : (DACC Offset: 0x18) Channel Status Register -------- */
 #define DACC_CHSR_CH0 (0x1u << 0) //!< (DACC_CHSR) Channel 0 Status
 #define DACC_CHSR_CH1 (0x1u << 1) //!< (DACC_CHSR) Channel 1 Status
 /* -------- DACC_CDR : (DACC Offset: 0x20) Conversion Data Register -------- */
 #define DACC_CDR_DATA_Pos 0	//!< doc!
 #define DACC_CDR_DATA_Msk (0xffffffffu << DACC_CDR_DATA_Pos) //!< (DACC_CDR) Data to Convert
 #define DACC_CDR_DATA(value) ((DACC_CDR_DATA_Msk & ((value) << DACC_CDR_DATA_Pos)))	//!< doc!
 /* -------- DACC_IER : (DACC Offset: 0x24) Interrupt Enable Register -------- */
 #define DACC_IER_TXRDY (0x1u << 0) //!< (DACC_IER) Transmit Ready Interrupt Enable
 #define DACC_IER_EOC (0x1u << 1) //!< (DACC_IER) End of Conversion Interrupt Enable
 #define DACC_IER_ENDTX (0x1u << 2) //!< (DACC_IER) End of Transmit Buffer Interrupt Enable
 #define DACC_IER_TXBUFE (0x1u << 3) //!< (DACC_IER) Transmit Buffer Empty Interrupt Enable
 /* -------- DACC_IDR : (DACC Offset: 0x28) Interrupt Disable Register -------- */
 #define DACC_IDR_TXRDY (0x1u << 0) //!< (DACC_IDR) Transmit Ready Interrupt Disable.
 #define DACC_IDR_EOC (0x1u << 1) //!< (DACC_IDR) End of Conversion Interrupt Disable
 #define DACC_IDR_ENDTX (0x1u << 2) //!< (DACC_IDR) End of Transmit Buffer Interrupt Disable
 #define DACC_IDR_TXBUFE (0x1u << 3) //!< (DACC_IDR) Transmit Buffer Empty Interrupt Disable
 /* -------- DACC_IMR : (DACC Offset: 0x2C) Interrupt Mask Register -------- */
 #define DACC_IMR_TXRDY (0x1u << 0) //!< (DACC_IMR) Transmit Ready Interrupt Mask
 #define DACC_IMR_EOC (0x1u << 1) //!< (DACC_IMR) End of Conversion Interrupt Mask
 #define DACC_IMR_ENDTX (0x1u << 2) //!< (DACC_IMR) End of Transmit Buffer Interrupt Mask
 #define DACC_IMR_TXBUFE (0x1u << 3) //!< (DACC_IMR) Transmit Buffer Empty Interrupt Mask
 /* -------- DACC_ISR : (DACC Offset: 0x30) Interrupt Status Register -------- */
 #define DACC_ISR_TXRDY (0x1u << 0) //!< (DACC_ISR) Transmit Ready Interrupt Flag
 #define DACC_ISR_EOC (0x1u << 1) //!< (DACC_ISR) End of Conversion Interrupt Flag
 #define DACC_ISR_ENDTX (0x1u << 2) //!< (DACC_ISR) End of DMA Interrupt Flag
 #define DACC_ISR_TXBUFE (0x1u << 3) //!< (DACC_ISR) Transmit Buffer Empty
 /* -------- DACC_ACR : (DACC Offset: 0x94) Analog Current Register -------- */
 #define DACC_ACR_IBCTLCH0_Pos 0	//!< doc!
 #define DACC_ACR_IBCTLCH0_Msk (0x3u << DACC_ACR_IBCTLCH0_Pos) //!< (DACC_ACR) Analog Output Current Control
 #define DACC_ACR_IBCTLCH0(value) ((DACC_ACR_IBCTLCH0_Msk & ((value) << DACC_ACR_IBCTLCH0_Pos)))	//!< doc!
 #define DACC_ACR_IBCTLCH1_Pos 2	//!< doc!
 #define DACC_ACR_IBCTLCH1_Msk (0x3u << DACC_ACR_IBCTLCH1_Pos) //!< (DACC_ACR) Analog Output Current Control
 #define DACC_ACR_IBCTLCH1(value) ((DACC_ACR_IBCTLCH1_Msk & ((value) << DACC_ACR_IBCTLCH1_Pos)))	//!< doc!
 #define DACC_ACR_IBCTLDACCORE_Pos 8	//!< doc!
 #define DACC_ACR_IBCTLDACCORE_Msk (0x3u << DACC_ACR_IBCTLDACCORE_Pos) //!< (DACC_ACR) Bias Current Control for DAC Core
 #define DACC_ACR_IBCTLDACCORE(value) ((DACC_ACR_IBCTLDACCORE_Msk & ((value) << DACC_ACR_IBCTLDACCORE_Pos)))	//!< doc!
 /* -------- DACC_WPMR : (DACC Offset: 0xE4) Write Protect Mode register -------- */
 #define DACC_WPMR_WPEN (0x1u << 0) //!< (DACC_WPMR) Write Protect Enable
 #define DACC_WPMR_WPKEY_Pos 8	//!< doc!
 #define DACC_WPMR_WPKEY_Msk (0xffffffu << DACC_WPMR_WPKEY_Pos) //!< (DACC_WPMR) Write Protect KEY
 #define DACC_WPMR_WPKEY(value) ((DACC_WPMR_WPKEY_Msk & ((value) << DACC_WPMR_WPKEY_Pos)))	//!< doc!
 /* -------- DACC_WPSR : (DACC Offset: 0xE8) Write Protect Status register -------- */
 #define DACC_WPSR_WPROTERR (0x1u << 0) //!< (DACC_WPSR) Write protection error
 #define DACC_WPSR_WPROTADDR_Pos 8	//!< doc!
 #define DACC_WPSR_WPROTADDR_Msk (0xffu << DACC_WPSR_WPROTADDR_Pos) //!< (DACC_WPSR) Write protection error address
 /* -------- DACC_RPR : (DACC Offset: 0x100) Receive Pointer Register -------- */
 #define DACC_RPR_RXPTR_Pos 0	//!< doc!
 #define DACC_RPR_RXPTR_Msk (0xffffffffu << DACC_RPR_RXPTR_Pos) //!< (DACC_RPR) Receive Pointer Register
 #define DACC_RPR_RXPTR(value) ((DACC_RPR_RXPTR_Msk & ((value) << DACC_RPR_RXPTR_Pos)))	//!< doc!
 /* -------- DACC_RCR : (DACC Offset: 0x104) Receive Counter Register -------- */
 #define DACC_RCR_RXCTR_Pos 0	//!< doc!
 #define DACC_RCR_RXCTR_Msk (0xffffu << DACC_RCR_RXCTR_Pos) //!< (DACC_RCR) Receive Counter Register
 #define DACC_RCR_RXCTR(value) ((DACC_RCR_RXCTR_Msk & ((value) << DACC_RCR_RXCTR_Pos)))	//!< doc!
 /* -------- DACC_TPR : (DACC Offset: 0x108) Transmit Pointer Register -------- */
 #define DACC_TPR_TXPTR_Pos 0	//!< doc!
 #define DACC_TPR_TXPTR_Msk (0xffffffffu << DACC_TPR_TXPTR_Pos) //!< (DACC_TPR) Transmit Counter Register
 #define DACC_TPR_TXPTR(value) ((DACC_TPR_TXPTR_Msk & ((value) << DACC_TPR_TXPTR_Pos)))	//!< doc!
 /* -------- DACC_TCR : (DACC Offset: 0x10C) Transmit Counter Register -------- */
 #define DACC_TCR_TXCTR_Pos 0	//!< doc!
 #define DACC_TCR_TXCTR_Msk (0xffffu << DACC_TCR_TXCTR_Pos) //!< (DACC_TCR) Transmit Counter Register
 #define DACC_TCR_TXCTR(value) ((DACC_TCR_TXCTR_Msk & ((value) << DACC_TCR_TXCTR_Pos)))	//!< doc!
 /* -------- DACC_RNPR : (DACC Offset: 0x110) Receive Next Pointer Register -------- */
 #define DACC_RNPR_RXNPTR_Pos 0	//!< doc!
 #define DACC_RNPR_RXNPTR_Msk (0xffffffffu << DACC_RNPR_RXNPTR_Pos) //!< (DACC_RNPR) Receive Next Pointer
 #define DACC_RNPR_RXNPTR(value) ((DACC_RNPR_RXNPTR_Msk & ((value) << DACC_RNPR_RXNPTR_Pos)))	//!< doc!
 /* -------- DACC_RNCR : (DACC Offset: 0x114) Receive Next Counter Register -------- */
 #define DACC_RNCR_RXNCTR_Pos 0	//!< doc!
 #define DACC_RNCR_RXNCTR_Msk (0xffffu << DACC_RNCR_RXNCTR_Pos) //!< (DACC_RNCR) Receive Next Counter
 #define DACC_RNCR_RXNCTR(value) ((DACC_RNCR_RXNCTR_Msk & ((value) << DACC_RNCR_RXNCTR_Pos)))	//!< doc!
 /* -------- DACC_TNPR : (DACC Offset: 0x118) Transmit Next Pointer Register -------- */
 #define DACC_TNPR_TXNPTR_Pos 0	//!< doc!
 #define DACC_TNPR_TXNPTR_Msk (0xffffffffu << DACC_TNPR_TXNPTR_Pos) //!< (DACC_TNPR) Transmit Next Pointer
 #define DACC_TNPR_TXNPTR(value) ((DACC_TNPR_TXNPTR_Msk & ((value) << DACC_TNPR_TXNPTR_Pos)))	//!< doc!
 /* -------- DACC_TNCR : (DACC Offset: 0x11C) Transmit Next Counter Register -------- */
 #define DACC_TNCR_TXNCTR_Pos 0	//!< doc!
 #define DACC_TNCR_TXNCTR_Msk (0xffffu << DACC_TNCR_TXNCTR_Pos) //!< (DACC_TNCR) Transmit Counter Next
 #define DACC_TNCR_TXNCTR(value) ((DACC_TNCR_TXNCTR_Msk & ((value) << DACC_TNCR_TXNCTR_Pos)))	//!< doc!
 /* -------- DACC_PTCR : (DACC Offset: 0x120) Transfer Control Register -------- */
 #define DACC_PTCR_RXTEN (0x1u << 0) //!< (DACC_PTCR) Receiver Transfer Enable
 #define DACC_PTCR_RXTDIS (0x1u << 1) //!< (DACC_PTCR) Receiver Transfer Disable
 #define DACC_PTCR_TXTEN (0x1u << 8) //!< (DACC_PTCR) Transmitter Transfer Enable
 #define DACC_PTCR_TXTDIS (0x1u << 9) //!< (DACC_PTCR) Transmitter Transfer Disable
 /* -------- DACC_PTSR : (DACC Offset: 0x124) Transfer Status Register -------- */
 #define DACC_PTSR_RXTEN (0x1u << 0) //!< (DACC_PTSR) Receiver Transfer Enable
 #define DACC_PTSR_TXTEN (0x1u << 8) //!< (DACC_PTSR) Transmitter Transfer Enable

 /*------------------------------------------------------------------------------
 *         Macros function of register access
 *------------------------------------------------------------------------------*/
#define DACC_CfgModeReg(pDACC, mode)  { \
             (pDACC)->DACC_MR = (mode);\
        }	//!< doc!

#define DACC_GetModeReg(pDACC)                ((pDACC)->DACC_MR)	//!< doc!

#define DACC_StartConversion(pDACC)           ((pDACC)->DACC_CR = DACC_CR_START)	//!< doc!

#define DACC_SoftReset(pDACC)                 ((pDACC)->DACC_CR = DACC_CR_SWRST)	//!< doc!

#define DACC_EnableChannel(pDACC, channel)    {\
            (pDACC)->DACC_CHER = (1 << (channel));\
        }	//!< doc!

#define DACC_DisableChannel(pDACC, channel)  {\
            (pDACC)->DACC_CHDR = (1 << (channel));\
        }	//!< doc!

#define DACC_EnableIt(pDACC, mode)            {\
            assert( ((mode)&0xFFF00000)== 0 ) ;\
            (pDACC)->DACC_IER = (mode);\
        }	//!< doc!

#define DACC_DisableIt(pDACC, mode)           {\
            assert( ((mode)&0xFFF00000)== 0 ) ;\
            (pDACC)->DACC_IDR = (mode);\
        }	//!< doc!

#define DACC_EnableDataReadyIt(pDACC)         ((pDACC)->DACC_IER = AT91C_DACC_DRDY)	//!< doc!

#define DACC_GetStatus(pDACC)                 ((pDACC)->DACC_ISR)	//!< doc!

#define DACC_GetChannelStatus(pDACC)          ((pDACC)->DACC_CHSR)	//!< doc!

#define DACC_GetInterruptMaskStatus(pDACC)    ((pDACC)->DACC_IMR)	//!< doc!

#define DACC_GetLastConvertedData(pDACC)      ((pDACC)->DACC_LCDR)	//!< doc!

#define DACC_CfgAnalogCtrlReg(pDACC,mode)     {\
            assert( ((mode) & 0xFFFCFF3C)==0 ) ;\
            (pDACC)->DACC_ACR = (mode);\
        }	//!< doc!

#define DACC_CfgExtModeReg(pDACC, extmode)    {\
            assert( ((extmode) & 0xFF00FFFE)==0 ) ;\
            (pDACC)->DACC_EMR = (extmode);\
        }	//!< doc!

#define DACC_GetAnalogCtrlReg(pDACC)          ((pDACC)->DACC_ACR)	//!< doc!

/*------------------------------------------------------------------------------
 *         Exported functions
 *------------------------------------------------------------------------------*/
extern void DACC_Initialize( Dacc* pDACC,
                     uint8_t idDACC,
                     uint8_t trgEn,
                     uint8_t trgSel,
                     uint8_t word,
                     uint8_t sleepMode,
                     uint32_t mck,
                     uint8_t refresh,	//!< refresh period
                     uint8_t user_sel,	//!< user channel selection
                     uint32_t tag_mode,	//!< using tag for channel number
                     uint32_t startup
                     );


extern void DACC_SetConversionData( Dacc* pDACC, uint32_t dwData ) ;

extern uint32_t DACC_WriteBuffer( Dacc* pDACC, uint16_t* pwBuffer, uint32_t dwSize ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _DACC_ */
