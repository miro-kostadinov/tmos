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
 * @defgroup  AT91_USART Universal Synchronous Asynchronous Receiver Transceiver (USART)
 * Interface for USART
 * @{
 *
 *  \section Purpose
 * This module provides several definitions and methods for using an USART
 * peripheral.
 *
 *  \section Usage
 *
 * -# Enable the USART peripheral clock in the PMC.
 * -# Enable the required USART PIOs (see pio.h).
 * -# Configure the USART by calling USART_Configure.
 * -# Enable the transmitter and/or the receiver of the USART using
 *    USART_SetTransmitterEnabled and USART_SetReceiverEnabled.
 * -# Send data through the USART using the USART_Write and
 *    USART_WriteBuffer methods.
 * -# Receive data from the USART using the USART_Read and
 *    USART_ReadBuffer functions; the availability of data can be polled
 *    with USART_IsDataAvailable.
 * -# Disable the transmitter and/or the receiver of the USART with
 *    USART_SetTransmitterEnabled and USART_SetReceiverEnabled.
 *
 * \section usart_module Working with USART
 * The USART driver provides the interface to configure and use the USART peripheral.\n
 *
 * The USART supports several kinds of communication modes such as full-duplex asynchronous/
 * synchronous serial communication,RS485 with driver control signal,ISO7816,SPI and Test modes.
 *
 * To start a USART transfer with \ref AT91_PDC "PDC" support, the user could follow these steps:
 * <ul>
 * <li> Configure USART with expected mode and baudrate(see \ref USART_Configure), which could be done by:
 * -# Resetting and disabling transmitter and receiver by setting US_CR(Control Register). </li>
 * -# Configuring the USART in a specific mode by setting USART_MODE bits in US_MR(Mode Register) </li>
 * -# Setting baudrate which is different from mode to mode.
   </li>
 * <li> Enable transmitter or receiver respectively by set US_CR_TXEN or US_CR_RXEN in US_CR.</li>
 * <li> Read from or write to the peripheral with  \ref USART_ReadBuffer or \ref USART_WriteBuffer.
        These operations could be done by polling or interruption. </li>
 * <li> For polling, check the status bit US_CSR_ENDRX/US_CSR_RXBUFF (READ) or US_CSR_ENDTX/
        US_CSR_TXBUFE (WRITE).  </li>
 * <li> For interruption,"enable" the status bit through US_IER and
        realize the hanler with USARTx_IrqHandler according to IRQ vector
        table which is defined in board_cstartup_<toolchain>.c
        To enable the interruption of USART,it should be configured with priority and enabled first through
        NVIC .</li>
 * </ul>
 *
 * For more accurate information, please look at the USART section of the
 * Datasheet.
 *
 * @file     hardware/at91/usart.h
 * @ingroup	 AT91_USART
 * @brief    USART header
 *
 * @}
*/

#ifndef _USART_
#define _USART_

/*------------------------------------------------------------------------------
 *         Headers
 *------------------------------------------------------------------------------*/
#include <fam_inc.h>
//#include "chip.h"
//#include <stdint.h>

/*------------------------------------------------------------------------------
 *         Definitions
 *------------------------------------------------------------------------------*/

/**
 * \section USART_mode USART modes
 * This section lists several common operating modes for an USART peripheral.
 *
 * \b Modes
 * - USART_MODE_ASYNCHRONOUS
 * - USART_MODE_IRDA
 */

/** Basic asynchronous mode, i.e. 8 bits no parity.*/
#define USART_MODE_ASYNCHRONOUS        (US_MR_CHRL_8_BIT | US_MR_PAR_NO)

/** IRDA mode*/
#define USART_MODE_IRDA                (AT91C_US_USMODE_IRDA | AT91C_US_CHRL_8_BITS | AT91C_US_PAR_NONE | AT91C_US_FILTER)

/** SPI mode*/
#define AT91C_US_USMODE_SPIM     0xE
#define US_SPI_CPOL_0      (0x0<<16)	//!< doc!
#define US_SPI_CPHA_0      (0x0<<8)	//!< doc!
#define US_SPI_CPOL_1      (0x1<<16)	//!< doc!
#define US_SPI_CPHA_1      (0x1<<8)	//!< doc!
#define US_SPI_BPMODE_0    (US_SPI_CPOL_0|US_SPI_CPHA_1)	//!< doc!
#define US_SPI_BPMODE_1    (US_SPI_CPOL_0|US_SPI_CPHA_0)	//!< doc!
#define US_SPI_BPMODE_2    (US_SPI_CPOL_1|US_SPI_CPHA_1)	//!< doc!
#define US_SPI_BPMODE_3    (US_SPI_CPOL_1|US_SPI_CPHA_0)	//!< doc!

#ifdef __cplusplus
 extern "C" {
#endif

/* ============================================================================ */
/**  API DEFINITION FOR Universal Synchronous Asynchronous Receiver Transmitter */
/* ============================================================================ */
#ifndef __ASSEMBLY__
/** \brief Usart hardware registers */
typedef struct {
	WoReg US_CR;         //!< (Usart Offset: 0x0000) Control Register
	RwReg US_MR;         //!< (Usart Offset: 0x0004) Mode Register
	WoReg US_IER;        //!< (Usart Offset: 0x0008) Interrupt Enable Register
	WoReg US_IDR;        //!< (Usart Offset: 0x000C) Interrupt Disable Register
	RoReg US_IMR;        //!< (Usart Offset: 0x0010) Interrupt Mask Register
	RoReg US_CSR;        //!< (Usart Offset: 0x0014) Channel Status Register
	RoReg US_RHR;        //!< (Usart Offset: 0x0018) Receiver Holding Register
	WoReg US_THR;        //!< (Usart Offset: 0x001C) Transmitter Holding Register
	RwReg US_BRGR;       //!< (Usart Offset: 0x0020) Baud Rate Generator Register
	RwReg US_RTOR;       //!< (Usart Offset: 0x0024) Receiver Time-out Register
	RwReg US_TTGR;       //!< (Usart Offset: 0x0028) Transmitter Timeguard Register
	RwReg Reserved1[5];	 //!< doc!
	RwReg US_FIDI;       //!< (Usart Offset: 0x0040) FI DI Ratio Register
	RoReg US_NER;        //!< (Usart Offset: 0x0044) Number of Errors Register
	RwReg Reserved2[1];	 //!< doc!
	RwReg US_IF;         //!< (Usart Offset: 0x004C) IrDA Filter Register
	RwReg US_MAN;        //!< (Usart Offset: 0x0050) Manchester Encoder Decoder Register
	RwReg Reserved3[36]; //!< doc!

#ifdef CFG_SAM3S
	RwReg US_WPMR;       //!< (Usart Offset: 0xE4) Write Protect Mode Register
	RoReg US_WPSR;       //!< (Usart Offset: 0xE8) Write Protect Status Register
#else
	RwReg Reserved4[2];	 //!< doc!
#endif

#ifdef CFG_SAM3U
	RwReg US_ADDRSIZE;   //!< (Usart Offset: 0xEC) ADDRSIZE REGISTER
	RwReg US_IPNAME1     //!< (Usart Offset: 0xF0) IPNAME1 REGISTER
	RwReg US_IPNAME2;    //!< (Usart Offset: 0xF4) IPNAME2 REGISTER
	RwReg US_FEATURES;   //!< (Usart Offset: 0xF8) FEATURES REGISTER
	RwReg US_VER;        //!< (Usart Offset: 0xFC) VERSION Register
#else
	RwReg Reserved5[5];  //!< doc!
#endif

	RwReg US_RPR;        //!< (Usart Offset: 0x100) Receive Pointer Register
	RwReg US_RCR;        //!< (Usart Offset: 0x104) Receive Counter Register
	RwReg US_TPR;        //!< (Usart Offset: 0x108) Transmit Pointer Register
	RwReg US_TCR;        //!< (Usart Offset: 0x10C) Transmit Counter Register
	RwReg US_RNPR;       //!< (Usart Offset: 0x110) Receive Next Pointer Register
	RwReg US_RNCR;       //!< (Usart Offset: 0x114) Receive Next Counter Register
	RwReg US_TNPR;       //!< (Usart Offset: 0x118) Transmit Next Pointer Register
	RwReg US_TNCR;       //!< (Usart Offset: 0x11C) Transmit Next Counter Register
	WoReg US_PTCR;       //!< (Usart Offset: 0x120) Transfer Control Register
	RoReg US_PTSR;       //!< (Usart Offset: 0x124) Transfer Status Register
} Usart;
#endif /* __ASSEMBLY__ */
/* -------- US_CR : (USART Offset: 0x0000) Control Register -------- */
#define US_CR_RSTRX (0x1u << 2) //!< (US_CR) Reset Receiver
#define US_CR_RSTTX (0x1u << 3) //!< (US_CR) Reset Transmitter
#define US_CR_RXEN (0x1u << 4) //!< (US_CR) Receiver Enable
#define US_CR_RXDIS (0x1u << 5) //!< (US_CR) Receiver Disable
#define US_CR_TXEN (0x1u << 6) //!< (US_CR) Transmitter Enable
#define US_CR_TXDIS (0x1u << 7) //!< (US_CR) Transmitter Disable
#define US_CR_RSTSTA (0x1u << 8) //!< (US_CR) Reset Status Bits
#define US_CR_STTBRK (0x1u << 9) //!< (US_CR) Start Break
#define US_CR_STPBRK (0x1u << 10) //!< (US_CR) Stop Break
#define US_CR_STTTO (0x1u << 11) //!< (US_CR) Start Time-out
#define US_CR_SENDA (0x1u << 12) //!< (US_CR) Send Address
#define US_CR_RSTIT (0x1u << 13) //!< (US_CR) Reset Iterations
#define US_CR_RSTNACK (0x1u << 14) //!< (US_CR) Reset Non Acknowledge
#define US_CR_RETTO (0x1u << 15) //!< (US_CR) Rearm Time-out
#define US_CR_DTREN (0x1u << 16) //!< (US_CR) Data Terminal Ready Enable
#define US_CR_DTRDIS (0x1u << 17) //!< (US_CR) Data Terminal Ready Disable
#define US_CR_RTSEN (0x1u << 18) //!< (US_CR) Request to Send Enable
#define US_CR_FCS (0x1u << 18) //!< (US_CR) Force SPI Chip Select
#define US_CR_RTSDIS (0x1u << 19) //!< (US_CR) Request to Send Disable
#define US_CR_RCS (0x1u << 19) //!< (US_CR) Release SPI Chip Select
/* -------- US_MR : (USART Offset: 0x0004) Mode Register -------- */
#define US_MR_USART_MODE_Pos 0	//!< doc!
#define US_MR_USART_MODE_Msk (0xfu << US_MR_USART_MODE_Pos) //!< (US_MR)
#define   US_MR_USART_MODE_NORMAL (0x0u << 0) //!< (US_MR) Normal mode
#define   US_MR_USART_MODE_RS485 (0x1u << 0) //!< (US_MR) RS485
#define   US_MR_USART_MODE_HW_HANDSHAKING (0x2u << 0) //!< (US_MR) Hardware Handshaking
#define   US_MR_USART_MODE_MODEM (0x3u << 0) //!< (US_MR) Modem
#define   US_MR_USART_MODE_IS07816_T_0 (0x4u << 0) //!< (US_MR) IS07816 Protocol: T = 0
#define   US_MR_USART_MODE_IS07816_T_1 (0x6u << 0) //!< (US_MR) IS07816 Protocol: T = 1
#define   US_MR_USART_MODE_IRDA (0x8u << 0) //!< (US_MR) IrDA
#define   US_MR_USART_MODE_SPI_MASTER (0xEu << 0) //!< (US_MR) SPI Master
#define   US_MR_USART_MODE_SPI_SLAVE (0xFu << 0) //!< (US_MR) SPI Slave
#define US_MR_USCLKS_Pos 4	//!< doc!
#define US_MR_USCLKS_Msk (0x3u << US_MR_USCLKS_Pos) //!< (US_MR) Clock Selection
#define   US_MR_USCLKS_MCK (0x0u << 4) //!< (US_MR) Master Clock MCK is selected
#define   US_MR_USCLKS_DIV (0x1u << 4) //!< (US_MR) Internal Clock Divided MCK/DIV (DIV=8) is selected
#define   US_MR_USCLKS_SCK (0x3u << 4) //!< (US_MR) Serial Clock SLK is selected
#define US_MR_CHRL_Pos 6	//!< doc!
#define US_MR_CHRL_Msk (0x3u << US_MR_CHRL_Pos) //!< (US_MR) Character Length.
#define   US_MR_CHRL_5_BIT (0x0u << 6) //!< (US_MR) Character length is 5 bits
#define   US_MR_CHRL_6_BIT (0x1u << 6) //!< (US_MR) Character length is 6 bits
#define   US_MR_CHRL_7_BIT (0x2u << 6) //!< (US_MR) Character length is 7 bits
#define   US_MR_CHRL_8_BIT (0x3u << 6) //!< (US_MR) Character length is 8 bits
#define US_MR_SYNC (0x1u << 8) //!< (US_MR) Synchronous Mode Select
#define US_MR_CPHA (0x1u << 8) //!< (US_MR) SPI Clock Phase
#define US_MR_PAR_Pos 9	//!< doc!
#define US_MR_PAR_Msk (0x7u << US_MR_PAR_Pos) //!< (US_MR) Parity Type
#define   US_MR_PAR_EVEN (0x0u << 9) //!< (US_MR) Even parity
#define   US_MR_PAR_ODD (0x1u << 9) //!< (US_MR) Odd parity
#define   US_MR_PAR_SPACE (0x2u << 9) //!< (US_MR) Parity forced to 0 (Space)
#define   US_MR_PAR_MARK (0x3u << 9) //!< (US_MR) Parity forced to 1 (Mark)
#define   US_MR_PAR_NO (0x4u << 9) //!< (US_MR) No parity
#define   US_MR_PAR_MULTIDROP (0x6u << 9) //!< (US_MR) Multidrop mode
#define US_MR_NBSTOP_Pos 12	//!< doc!
#define US_MR_NBSTOP_Msk (0x3u << US_MR_NBSTOP_Pos) //!< (US_MR) Number of Stop Bits
#define   US_MR_NBSTOP_1_BIT (0x0u << 12) //!< (US_MR) 1 stop bit
#define   US_MR_NBSTOP_1_5_BIT (0x1u << 12) //!< (US_MR) 1.5 stop bit (SYNC = 0) or reserved (SYNC = 1)
#define   US_MR_NBSTOP_2_BIT (0x2u << 12) //!< (US_MR) 2 stop bits
#define US_MR_CHMODE_Pos 14	//!< doc!
#define US_MR_CHMODE_Msk (0x3u << US_MR_CHMODE_Pos) //!< (US_MR) Channel Mode
#define   US_MR_CHMODE_NORMAL (0x0u << 14) //!< (US_MR) Normal Mode
#define   US_MR_CHMODE_AUTOMATIC (0x1u << 14) //!< (US_MR) Automatic Echo. Receiver input is connected to the TXD pin.
#define   US_MR_CHMODE_LOCAL_LOOPBACK (0x2u << 14) //!< (US_MR) Local Loopback. Transmitter output is connected to the Receiver Input.
#define   US_MR_CHMODE_REMOTE_LOOPBACK (0x3u << 14) //!< (US_MR) Remote Loopback. RXD pin is internally connected to the TXD pin.
#define US_MR_MSBF (0x1u << 16) //!< (US_MR) Bit Order
#define US_MR_CPOL (0x1u << 16) //!< (US_MR) SPI Clock Polarity
#define US_MR_MODE9 (0x1u << 17) //!< (US_MR) 9-bit Character Length
#define US_MR_CLKO (0x1u << 18) //!< (US_MR) Clock Output Select
#define US_MR_OVER (0x1u << 19) //!< (US_MR) Oversampling Mode
#define US_MR_INACK (0x1u << 20) //!< (US_MR) Inhibit Non Acknowledge
#define US_MR_DSNACK (0x1u << 21) //!< (US_MR) Disable Successive NACK
#define US_MR_VAR_SYNC (0x1u << 22) //!< (US_MR) Variable Synchronization of Command/Data Sync Start Frame Delimiter
#define US_MR_INVDATA (0x1u << 23) //!< (US_MR) INverted Data
#define US_MR_MAX_ITERATION_Pos 24	//!< doc!
#define US_MR_MAX_ITERATION_Msk (0x7u << US_MR_MAX_ITERATION_Pos) //!< (US_MR)
#define US_MR_MAX_ITERATION(value) ((US_MR_MAX_ITERATION_Msk & ((value) << US_MR_MAX_ITERATION_Pos)))	//!< doc!
#define US_MR_FILTER (0x1u << 28) //!< (US_MR) Infrared Receive Line Filter
#define US_MR_MAN (0x1u << 29) //!< (US_MR) Manchester Encoder/Decoder Enable
#define US_MR_MODSYNC (0x1u << 30) //!< (US_MR) Manchester Synchronization Mode
#define US_MR_ONEBIT (0x1u << 31) //!< (US_MR) Start Frame Delimiter Selector
/* -------- US_IER : (USART Offset: 0x0008) Interrupt Enable Register -------- */
#define US_IER_RXRDY (0x1u << 0) //!< (US_IER) RXRDY Interrupt Enable
#define US_IER_TXRDY (0x1u << 1) //!< (US_IER) TXRDY Interrupt Enable
#define US_IER_RXBRK (0x1u << 2) //!< (US_IER) Receiver Break Interrupt Enable
#define US_IER_ENDRX (0x1u << 3) //!< (US_IER) End of Receive Transfer Interrupt Enable
#define US_IER_ENDTX (0x1u << 4) //!< (US_IER) End of Transmit Interrupt Enable
#define US_IER_OVRE (0x1u << 5) //!< (US_IER) Overrun Error Interrupt Enable
#define US_IER_FRAME (0x1u << 6) //!< (US_IER) Framing Error Interrupt Enable
#define US_IER_PARE (0x1u << 7) //!< (US_IER) Parity Error Interrupt Enable
#define US_IER_TIMEOUT (0x1u << 8) //!< (US_IER) Time-out Interrupt Enable
#define US_IER_TXEMPTY (0x1u << 9) //!< (US_IER) TXEMPTY Interrupt Enable
#define US_IER_ITER (0x1u << 10) //!< (US_IER) Max number of Repetitions Reached
#define US_IER_UNRE (0x1u << 10) //!< (US_IER) SPI Underrun Error
#define US_IER_TXBUFE (0x1u << 11) //!< (US_IER) Buffer Empty Interrupt Enable
#define US_IER_RXBUFF (0x1u << 12) //!< (US_IER) Buffer Full Interrupt Enable
#define US_IER_NACK (0x1u << 13) //!< (US_IER) Non AcknowledgeInterrupt Enable
#define US_IER_RIIC (0x1u << 16) //!< (US_IER) Ring Indicator Input Change Enable
#define US_IER_DSRIC (0x1u << 17) //!< (US_IER) Data Set Ready Input Change Enable
#define US_IER_DCDIC (0x1u << 18) //!< (US_IER) Data Carrier Detect Input Change Interrupt Enable
#define US_IER_CTSIC (0x1u << 19) //!< (US_IER) Clear to Send Input Change Interrupt Enable
#define US_IER_MANE (0x1u << 24) //!< (US_IER) Manchester Error Interrupt Enable
/* -------- US_IDR : (USART Offset: 0x000C) Interrupt Disable Register -------- */
#define US_IDR_RXRDY (0x1u << 0) //!< (US_IDR) RXRDY Interrupt Disable
#define US_IDR_TXRDY (0x1u << 1) //!< (US_IDR) TXRDY Interrupt Disable
#define US_IDR_RXBRK (0x1u << 2) //!< (US_IDR) Receiver Break Interrupt Disable
#define US_IDR_ENDRX (0x1u << 3) //!< (US_IDR) End of Receive Transfer Interrupt Disable
#define US_IDR_ENDTX (0x1u << 4) //!< (US_IDR) End of Transmit Interrupt Disable
#define US_IDR_OVRE (0x1u << 5) //!< (US_IDR) Overrun Error Interrupt Disable
#define US_IDR_FRAME (0x1u << 6) //!< (US_IDR) Framing Error Interrupt Disable
#define US_IDR_PARE (0x1u << 7) //!< (US_IDR) Parity Error Interrupt Disable
#define US_IDR_TIMEOUT (0x1u << 8) //!< (US_IDR) Time-out Interrupt Disable
#define US_IDR_TXEMPTY (0x1u << 9) //!< (US_IDR) TXEMPTY Interrupt Disable
#define US_IDR_ITER (0x1u << 10) //!< (US_IDR) Max number of Repetitions Reached Disable
#define US_IDR_UNRE (0x1u << 10) //!< (US_IDR) SPI Underrun Error Disable
#define US_IDR_TXBUFE (0x1u << 11) //!< (US_IDR) Buffer Empty Interrupt Disable
#define US_IDR_RXBUFF (0x1u << 12) //!< (US_IDR) Buffer Full Interrupt Disable
#define US_IDR_NACK (0x1u << 13) //!< (US_IDR) Non AcknowledgeInterrupt Disable
#define US_IDR_RIIC (0x1u << 16) //!< (US_IDR) Ring Indicator Input Change Disable
#define US_IDR_DSRIC (0x1u << 17) //!< (US_IDR) Data Set Ready Input Change Disable
#define US_IDR_DCDIC (0x1u << 18) //!< (US_IDR) Data Carrier Detect Input Change Interrupt Disable
#define US_IDR_CTSIC (0x1u << 19) //!< (US_IDR) Clear to Send Input Change Interrupt Disable
#define US_IDR_MANE (0x1u << 24) //!< (US_IDR) Manchester Error Interrupt Disable
/* -------- US_IMR : (USART Offset: 0x0010) Interrupt Mask Register -------- */
#define US_IMR_RXRDY (0x1u << 0) //!< (US_IMR) RXRDY Interrupt Mask
#define US_IMR_TXRDY (0x1u << 1) //!< (US_IMR) TXRDY Interrupt Mask
#define US_IMR_RXBRK (0x1u << 2) //!< (US_IMR) Receiver Break Interrupt Mask
#define US_IMR_ENDRX (0x1u << 3) //!< (US_IMR) End of Receive Transfer Interrupt Mask
#define US_IMR_ENDTX (0x1u << 4) //!< (US_IMR) End of Transmit Interrupt Mask
#define US_IMR_OVRE (0x1u << 5) //!< (US_IMR) Overrun Error Interrupt Mask
#define US_IMR_FRAME (0x1u << 6) //!< (US_IMR) Framing Error Interrupt Mask
#define US_IMR_PARE (0x1u << 7) //!< (US_IMR) Parity Error Interrupt Mask
#define US_IMR_TIMEOUT (0x1u << 8) //!< (US_IMR) Time-out Interrupt Mask
#define US_IMR_TXEMPTY (0x1u << 9) //!< (US_IMR) TXEMPTY Interrupt Mask
#define US_IMR_ITER (0x1u << 10) //!< (US_IMR) Max number of Repetitions Reached Mask
#define US_IMR_UNRE (0x1u << 10) //!< (US_IMR) SPI Underrun Error Mask
#define US_IMR_TXBUFE (0x1u << 11) //!< (US_IMR) Buffer Empty Interrupt Mask
#define US_IMR_RXBUFF (0x1u << 12) //!< (US_IMR) Buffer Full Interrupt Mask
#define US_IMR_NACK (0x1u << 13) //!< (US_IMR) Non AcknowledgeInterrupt Mask
#define US_IMR_RIIC (0x1u << 16) //!< (US_IMR) Ring Indicator Input Change Mask
#define US_IMR_DSRIC (0x1u << 17) //!< (US_IMR) Data Set Ready Input Change Mask
#define US_IMR_DCDIC (0x1u << 18) //!< (US_IMR) Data Carrier Detect Input Change Interrupt Mask
#define US_IMR_CTSIC (0x1u << 19) //!< (US_IMR) Clear to Send Input Change Interrupt Mask
#define US_IMR_MANE (0x1u << 24) //!< (US_IMR) Manchester Error Interrupt Mask
/* -------- US_CSR : (USART Offset: 0x0014) Channel Status Register -------- */
#define US_CSR_RXRDY (0x1u << 0) //!< (US_CSR) Receiver Ready
#define US_CSR_TXRDY (0x1u << 1) //!< (US_CSR) Transmitter Ready
#define US_CSR_RXBRK (0x1u << 2) //!< (US_CSR) Break Received/End of Break
#define US_CSR_ENDRX (0x1u << 3) //!< (US_CSR) End of Receiver Transfer
#define US_CSR_ENDTX (0x1u << 4) //!< (US_CSR) End of Transmitter Transfer
#define US_CSR_OVRE (0x1u << 5) //!< (US_CSR) Overrun Error
#define US_CSR_FRAME (0x1u << 6) //!< (US_CSR) Framing Error
#define US_CSR_PARE (0x1u << 7) //!< (US_CSR) Parity Error
#define US_CSR_TIMEOUT (0x1u << 8) //!< (US_CSR) Receiver Time-out
#define US_CSR_TXEMPTY (0x1u << 9) //!< (US_CSR) Transmitter Empty
#define US_CSR_ITER (0x1u << 10) //!< (US_CSR) Max number of Repetitions Reached
#define US_CSR_UNRE (0x1u << 10) //!< (US_CSR) SPI Underrun Error
#define US_CSR_TXBUFE (0x1u << 11) //!< (US_CSR) Transmission Buffer Empty
#define US_CSR_RXBUFF (0x1u << 12) //!< (US_CSR) Reception Buffer Full
#define US_CSR_NACK (0x1u << 13) //!< (US_CSR) Non AcknowledgeInterrupt
#define US_CSR_RIIC (0x1u << 16) //!< (US_CSR) Ring Indicator Input Change Flag
#define US_CSR_DSRIC (0x1u << 17) //!< (US_CSR) Data Set Ready Input Change Flag
#define US_CSR_DCDIC (0x1u << 18) //!< (US_CSR) Data Carrier Detect Input Change Flag
#define US_CSR_CTSIC (0x1u << 19) //!< (US_CSR) Clear to Send Input Change Flag
#define US_CSR_RI (0x1u << 20) //!< (US_CSR) Image of RI Input
#define US_CSR_DSR (0x1u << 21) //!< (US_CSR) Image of DSR Input
#define US_CSR_DCD (0x1u << 22) //!< (US_CSR) Image of DCD Input
#define US_CSR_CTS (0x1u << 23) //!< (US_CSR) Image of CTS Input
#define US_CSR_MANERR (0x1u << 24) //!< (US_CSR) Manchester Error
/* -------- US_RHR : (USART Offset: 0x0018) Receiver Holding Register -------- */
#define US_RHR_RXCHR_Pos 0	//!< doc!
#define US_RHR_RXCHR_Msk (0x1ffu << US_RHR_RXCHR_Pos) //!< (US_RHR) Received Character
#define US_RHR_RXSYNH (0x1u << 15) //!< (US_RHR) Received Sync
/* -------- US_THR : (USART Offset: 0x001C) Transmitter Holding Register -------- */
#define US_THR_TXCHR_Pos 0	//!< doc!
#define US_THR_TXCHR_Msk (0x1ffu << US_THR_TXCHR_Pos) //!< (US_THR) Character to be Transmitted
#define US_THR_TXCHR(value) ((US_THR_TXCHR_Msk & ((value) << US_THR_TXCHR_Pos)))	//!< doc!
#define US_THR_TXSYNH (0x1u << 15) //!< (US_THR) Sync Field to be transmitted
/* -------- US_BRGR : (USART Offset: 0x0020) Baud Rate Generator Register -------- */
#define US_BRGR_CD_Pos 0	//!< doc!
#define US_BRGR_CD_Msk (0xffffu << US_BRGR_CD_Pos) //!< (US_BRGR) Clock Divider
#define US_BRGR_CD(value) ((US_BRGR_CD_Msk & ((value) << US_BRGR_CD_Pos)))	//!< doc!
#define US_BRGR_FP_Pos 16	//!< doc!
#define US_BRGR_FP_Msk (0x7u << US_BRGR_FP_Pos) //!< (US_BRGR) Fractional Part
#define US_BRGR_FP(value) ((US_BRGR_FP_Msk & ((value) << US_BRGR_FP_Pos)))	//!< doc!
/* -------- US_RTOR : (USART Offset: 0x0024) Receiver Time-out Register -------- */
#define US_RTOR_TO_Pos 0	//!< doc!
#define US_RTOR_TO_Msk (0xffffu << US_RTOR_TO_Pos) //!< (US_RTOR) Time-out Value
#define US_RTOR_TO(value) ((US_RTOR_TO_Msk & ((value) << US_RTOR_TO_Pos)))	//!< doc!
/* -------- US_TTGR : (USART Offset: 0x0028) Transmitter Timeguard Register -------- */
#define US_TTGR_TG_Pos 0	//!< doc!
#define US_TTGR_TG_Msk (0xffu << US_TTGR_TG_Pos) //!< (US_TTGR) Timeguard Value
#define US_TTGR_TG(value) ((US_TTGR_TG_Msk & ((value) << US_TTGR_TG_Pos)))	//!< doc!
/* -------- US_FIDI : (USART Offset: 0x0040) FI DI Ratio Register -------- */
#define US_FIDI_FI_DI_RATIO_Pos 0	//!< doc!
#define US_FIDI_FI_DI_RATIO_Msk (0x7ffu << US_FIDI_FI_DI_RATIO_Pos) //!< (US_FIDI) FI Over DI Ratio Value
#define US_FIDI_FI_DI_RATIO(value) ((US_FIDI_FI_DI_RATIO_Msk & ((value) << US_FIDI_FI_DI_RATIO_Pos)))	//!< doc!
/* -------- US_NER : (USART Offset: 0x0044) Number of Errors Register -------- */
#define US_NER_NB_ERRORS_Pos 0	//!< doc!
#define US_NER_NB_ERRORS_Msk (0xffu << US_NER_NB_ERRORS_Pos) //!< (US_NER) Number of Errors
/* -------- US_IF : (USART Offset: 0x004C) IrDA Filter Register -------- */
#define US_IF_IRDA_FILTER_Pos 0	//!< doc!
#define US_IF_IRDA_FILTER_Msk (0xffu << US_IF_IRDA_FILTER_Pos) //!< (US_IF) IrDA Filter
#define US_IF_IRDA_FILTER(value) ((US_IF_IRDA_FILTER_Msk & ((value) << US_IF_IRDA_FILTER_Pos)))	//!< doc!
/* -------- US_MAN : (USART Offset: 0x0050) Manchester Encoder Decoder Register -------- */
#define US_MAN_TX_PL_Pos 0	//!< doc!
#define US_MAN_TX_PL_Msk (0xfu << US_MAN_TX_PL_Pos) //!< (US_MAN) Transmitter Preamble Length
#define US_MAN_TX_PL(value) ((US_MAN_TX_PL_Msk & ((value) << US_MAN_TX_PL_Pos)))	//!< doc!
#define US_MAN_TX_PP_Pos 8	//!< doc!
#define US_MAN_TX_PP_Msk (0x3u << US_MAN_TX_PP_Pos) //!< (US_MAN) Transmitter Preamble Pattern
#define   US_MAN_TX_PP_ALL_ONE (0x0u << 8) //!< (US_MAN) The preamble is composed of '1's
#define   US_MAN_TX_PP_ALL_ZERO (0x1u << 8) //!< (US_MAN) The preamble is composed of '0's
#define   US_MAN_TX_PP_ZERO_ONE (0x2u << 8) //!< (US_MAN) The preamble is composed of '01's
#define   US_MAN_TX_PP_ONE_ZERO (0x3u << 8) //!< (US_MAN) The preamble is composed of '10's
#define US_MAN_TX_MPOL (0x1u << 12) //!< (US_MAN) Transmitter Manchester Polarity
#define US_MAN_RX_PL_Pos 16	//!< doc!
#define US_MAN_RX_PL_Msk (0xfu << US_MAN_RX_PL_Pos) //!< (US_MAN) Receiver Preamble Length
#define US_MAN_RX_PL(value) ((US_MAN_RX_PL_Msk & ((value) << US_MAN_RX_PL_Pos)))	//!< doc!
#define US_MAN_RX_PP_Pos 24	//!< doc!
#define US_MAN_RX_PP_Msk (0x3u << US_MAN_RX_PP_Pos) //!< (US_MAN) Receiver Preamble Pattern detected
#define   US_MAN_RX_PP_ALL_ONE (0x0u << 24) //!< (US_MAN) The preamble is composed of '1's
#define   US_MAN_RX_PP_ALL_ZERO (0x1u << 24) //!< (US_MAN) The preamble is composed of '0's
#define   US_MAN_RX_PP_ZERO_ONE (0x2u << 24) //!< (US_MAN) The preamble is composed of '01's
#define   US_MAN_RX_PP_ONE_ZERO (0x3u << 24) //!< (US_MAN) The preamble is composed of '10's
#define US_MAN_RX_MPOL (0x1u << 28) //!< (US_MAN) Receiver Manchester Polarity
#define US_MAN_STUCKTO1 (0x1u << 29) //!< (US_MAN)
#define US_MAN_DRIFT (0x1u << 30) //!< (US_MAN) Drift compensation

#ifdef CFG_SAM3S
/* -------- US_WPMR : (USART Offset: 0xE4) Write Protect Mode Register -------- */
#define US_WPMR_WPEN (0x1u << 0) //!< (US_WPMR) Write Protect Enable
#define US_WPMR_WPKEY_Pos 8
#define US_WPMR_WPKEY_Msk (0xffffffu << US_WPMR_WPKEY_Pos) //!< (US_WPMR) Write Protect KEY
#define US_WPMR_WPKEY(value) ((US_WPMR_WPKEY_Msk & ((value) << US_WPMR_WPKEY_Pos)))
/* -------- US_WPSR : (USART Offset: 0xE8) Write Protect Status Register -------- */
#define US_WPSR_WPVS (0x1u << 0) //!< (US_WPSR) Write Protect Violation Status
#define US_WPSR_WPVSRC_Pos 8
#define US_WPSR_WPVSRC_Msk (0xffffu << US_WPSR_WPVSRC_Pos) //!< (US_WPSR) Write Protect Violation Source
#endif

/* -------- US_RPR : (USART Offset: 0x100) Receive Pointer Register -------- */
#define US_RPR_RXPTR_Pos 0	//!< doc!
#define US_RPR_RXPTR_Msk (0xffffffffu << US_RPR_RXPTR_Pos) //!< (US_RPR) Receive Pointer Register
#define US_RPR_RXPTR(value) ((US_RPR_RXPTR_Msk & ((value) << US_RPR_RXPTR_Pos)))	//!< doc!
/* -------- US_RCR : (USART Offset: 0x104) Receive Counter Register -------- */
#define US_RCR_RXCTR_Pos 0	//!< doc!
#define US_RCR_RXCTR_Msk (0xffffu << US_RCR_RXCTR_Pos) //!< (US_RCR) Receive Counter Register
#define US_RCR_RXCTR(value) ((US_RCR_RXCTR_Msk & ((value) << US_RCR_RXCTR_Pos)))	//!< doc!
/* -------- US_TPR : (USART Offset: 0x108) Transmit Pointer Register -------- */
#define US_TPR_TXPTR_Pos 0	//!< doc!
#define US_TPR_TXPTR_Msk (0xffffffffu << US_TPR_TXPTR_Pos) //!< (US_TPR) Transmit Counter Register
#define US_TPR_TXPTR(value) ((US_TPR_TXPTR_Msk & ((value) << US_TPR_TXPTR_Pos)))	//!< doc!
/* -------- US_TCR : (USART Offset: 0x10C) Transmit Counter Register -------- */
#define US_TCR_TXCTR_Pos 0	//!< doc!
#define US_TCR_TXCTR_Msk (0xffffu << US_TCR_TXCTR_Pos) //!< (US_TCR) Transmit Counter Register
#define US_TCR_TXCTR(value) ((US_TCR_TXCTR_Msk & ((value) << US_TCR_TXCTR_Pos)))	//!< doc!
/* -------- US_RNPR : (USART Offset: 0x110) Receive Next Pointer Register -------- */
#define US_RNPR_RXNPTR_Pos 0	//!< doc!
#define US_RNPR_RXNPTR_Msk (0xffffffffu << US_RNPR_RXNPTR_Pos) //!< (US_RNPR) Receive Next Pointer
#define US_RNPR_RXNPTR(value) ((US_RNPR_RXNPTR_Msk & ((value) << US_RNPR_RXNPTR_Pos)))	//!< doc!
/* -------- US_RNCR : (USART Offset: 0x114) Receive Next Counter Register -------- */
#define US_RNCR_RXNCTR_Pos 0	//!< doc!
#define US_RNCR_RXNCTR_Msk (0xffffu << US_RNCR_RXNCTR_Pos) //!< (US_RNCR) Receive Next Counter
#define US_RNCR_RXNCTR(value) ((US_RNCR_RXNCTR_Msk & ((value) << US_RNCR_RXNCTR_Pos)))	//!< doc!
/* -------- US_TNPR : (USART Offset: 0x118) Transmit Next Pointer Register -------- */
#define US_TNPR_TXNPTR_Pos 0	//!< doc!
#define US_TNPR_TXNPTR_Msk (0xffffffffu << US_TNPR_TXNPTR_Pos) //!< (US_TNPR) Transmit Next Pointer
#define US_TNPR_TXNPTR(value) ((US_TNPR_TXNPTR_Msk & ((value) << US_TNPR_TXNPTR_Pos)))	//!< doc!
/* -------- US_TNCR : (USART Offset: 0x11C) Transmit Next Counter Register -------- */
#define US_TNCR_TXNCTR_Pos 0	//!< doc!
#define US_TNCR_TXNCTR_Msk (0xffffu << US_TNCR_TXNCTR_Pos) //!< (US_TNCR) Transmit Counter Next
#define US_TNCR_TXNCTR(value) ((US_TNCR_TXNCTR_Msk & ((value) << US_TNCR_TXNCTR_Pos)))	//!< doc!
/* -------- US_PTCR : (USART Offset: 0x120) Transfer Control Register -------- */
#define US_PTCR_RXTEN (0x1u << 0) //!< (US_PTCR) Receiver Transfer Enable
#define US_PTCR_RXTDIS (0x1u << 1) //!< (US_PTCR) Receiver Transfer Disable
#define US_PTCR_TXTEN (0x1u << 8) //!< (US_PTCR) Transmitter Transfer Enable
#define US_PTCR_TXTDIS (0x1u << 9) //!< (US_PTCR) Transmitter Transfer Disable
/* -------- US_PTSR : (USART Offset: 0x124) Transfer Status Register -------- */
#define US_PTSR_RXTEN (0x1u << 0) //!< (US_PTSR) Receiver Transfer Enable
#define US_PTSR_TXTEN (0x1u << 8) //!< (US_PTSR) Transmitter Transfer Enable

/*------------------------------------------------------------------------------*/
/*         Exported functions                                                   */
/*------------------------------------------------------------------------------*/

extern void USART_Configure( Usart *usart, uint32_t mode, uint32_t baudrate, uint32_t masterClock ) ;
extern uint32_t USART_GetStatus( Usart *usart ) ;
extern void USART_EnableIt( Usart *usart,uint32_t mode ) ;
extern void USART_DisableIt( Usart *usart,uint32_t mode ) ;
extern void USART_SetTransmitterEnabled( Usart *usart, uint8_t enabled ) ;

extern void USART_SetReceiverEnabled( Usart *usart, uint8_t enabled ) ;

extern void USART_Write( Usart *usart, uint16_t data, volatile uint32_t timeOut ) ;

extern uint8_t USART_WriteBuffer( Usart *usart, void *buffer, uint32_t size ) ;

extern uint16_t USART_Read( Usart *usart, volatile uint32_t timeOut ) ;

extern uint8_t USART_ReadBuffer( Usart *usart, void *buffer, uint32_t size ) ;

extern uint8_t USART_IsDataAvailable( Usart *usart ) ;

extern void USART_SetIrdaFilter(Usart *pUsart, uint8_t filter);

extern void USART_PutChar( Usart *usart, uint8_t c ) ;

extern uint32_t USART_IsRxReady( Usart *usart ) ;

extern uint8_t USART_GetChar( Usart *usart ) ;

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _USART_ */

