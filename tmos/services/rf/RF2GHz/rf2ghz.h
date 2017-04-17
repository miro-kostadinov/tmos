/*
 * rf2ghz.h
 *
 *  Created on: Feb 23, 2017
 *      Author: miro
 */

/* Common library for RFM73(RF70) and nrf2401A, nrf24L01, nrf24L01+
 *
 */

#ifndef SERVICES_RF_RF2GHZ_RF2GHZ_H_
#define SERVICES_RF_RF2GHZ_RF2GHZ_H_


///-----------------------------------------------------------------------------
/// SPI Commands (LSB first)					   data bytes:
///-----------------------------------------------------------------------------
#define RF2G_CMD_R_REGISTER(reg)	(0x00 |(reg))	//!< 1-5  Read command and status registers

#define RF2G_CMD_W_REGISTER(reg)	(0x20 |(reg))	//!< 1-5  Write command and status registers.
 	 	 	 	 	 	 	 	 	 	 	 	 	//!<	  Executable in power down or standby modes only.

#define RF2G_CMD_R_RX_PAYLOAD 			0x61		//!< 1-32 Read RX-payload. A read operation always starts at byte 0.
 	 	 	 	 	 	 	 	 	 	 	 	 	//!  	  Payload is deleted from FIFO after it is read. Used in RX mode.

#define RF2G_CMD_W_TX_PAYLOAD 			0xA0 		//!< 1-32 Write TX-payload. A write operation always starts at byte 0
													//!		  used in TX payload.

#define RF2G_CMD_FLUSH_TX 				0xE1		//!< 0    Flush TX FIFO, used in TX mode

#define RF2G_CMD_FLUSH_RX 				0xE2		//!< 0    Flush RX FIFO, used in RX mode
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  Should not be executed during transmission of acknowledge,
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  that is, acknowledge package will not be completed.

#define RF2G_CMD_REUSE_TX_PL 			0xE3		//!< 0 	  Used for a PTX device. Reuse last transmitted payload.
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  Packets are repeatedly retransmitted as long as CE is high.
  	  	  	  	  	  	  	  	  	  	  	  	  	//! 	  TX payload reuse is active until W_TX_PAYLOAD or FLUSH_TX
  	  	  	  	  	  	  	  	  	  	  	  	  	//! 	  is executed. TX payload reuse must not be activated or
  	  	  	  	  	  	  	  	  	  	  	  	  	//! 	  deactivated during package transmission

#define RF2G_CMD_R_RX_PL_WID			0x60		//!<	  Read RX-payload width for the top R_RX_PAYLOAD in
													//!  	  the RX FIFO.

#define RF2G_CMD_W_ACK_PAYLOAD(PPP) (0xA8|(PPP))	//!< 1-32 Used in RX mode.
 	 	 	 	 	 	 	 	 	 	 	 	 	//!		  Write Payload to be transmitted together with ACK packet
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  on PIPE PPP. (PPP valid in the range from 000 to 101).
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  Maximum three ACK packet payloads can be pending. Payloads
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  with same PPP are handled using first in - first out
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  principle. A write operation always starts at byte 0.

#define RF2G_CMD_W_TX_PAYLOAD_NOACK		0xB0		//!< 1-32 Used in TX mode. Disables AUTOACK on this specific packet.

#define RF2G_CMD_NOP					0xFF		//!< 0	  No Operation. Might be used to read the STATUS register

#define RFM73_CMD_ACTIVATE				0x50		//!< 1	  This write command followed by data 0x73 activates
 	 	 	 	 	 	 	 	 	 	 	 	 	//!		  the following features:
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 		• R_RX_PL_WID
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 		• W_ACK_PAYLOAD
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 		• W_TX_PAYLOAD_NOACK
 	 	 	 	 	 	 	 	 	 	 	 	 	//!
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  A new ACTIVATE command with the same data	deactivates them
 	 	 	 	 	 	 	 	 	 	 	 	 	//!       again. This is executable in power down or stand by modes only
 	 	 	 	 	 	 	 	 	 	 	 	 	//!
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  The R_RX_PL_WID, W_ACK_PAYLOAD, and W_TX_PAYLOAD_NOACK
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  features registers are initially in a deactivated state;
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  a write has no effect, a read only results in zeros on MISO.
 	 	 	 	 	 	 	 	 	 	 	 	 	//!       To activate these registers, use the ACTIVATE command
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  followed by data 0x73. Then they can be accessed as any
 	 	 	 	 	 	 	 	 	 	 	 	 	//!  	  other register. Use the same command and data to deactivate
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  the registers again.
 	 	 	 	 	 	 	 	 	 	 	 	 	//!
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  This write command followed by data 0x53 toggles the register
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  bank, and the current register bank number can be read out
 	 	 	 	 	 	 	 	 	 	 	 	 	//! 	  from REG7 [7]


///-----------------------------------------------------------------------------
/// Register Bank 0
/// nrf24xx - this is the only bank
/// RF70: There are two register banks, which can be toggled by SPI command “ACTIVATE”
/// followed with 0x53 byte, and bank status can be read from Bank0_REG7 [7].
///-----------------------------------------------------------------------------
#define RF2G_REG_CONFIG			0x00	//!< Configuration Register
#define RF2G_REG_EN_AA			0x01	//!< Enable „Auto Acknowledgment" Function
 	 	 	 	 	 	 	 	 	 	//!  Disable this functionality to be compatible with nRF2401
#define RF2G_REG_EN_RXADDR		0x02	//!< Enabled RX Addresses
#define RF2G_REG_SETUP_AW		0x03	//!< Setup of Address Widths (common for all data pipes)
#define RF2G_REG_SETUP_RETR		0x04	//!< Setup of Automatic Retransmission
#define RF2G_REG_RF_CH			0x05	//!< RF Channel
#define RF2G_REG_RF_SETUP		0x05	//!< RF Setup Register
#define RF2G_REG_STATUS			0x07	//!< Status Register
										//!  (In parallel to the SPI command word applied on the MOSI pin,
 	 	 	  	 	 	 	 	 		//!  the STATUS register is shifted serially out on the MISO pin)
#define RF2G_REG_OBSERVE_TX 	0x08    //!< Transmit observe register
#define RF2G_REG_CD          	0x09    //!< Carrier Detect
#define RF2G_REG_RX_ADDR_P0		0x0A    //!< Receive address data pipe 0.
 	 	 	 	 	 	 	 	 	 	//!  5 Bytes maximum length. (LSB byte is written first. Write the
 	 	 	 	 	 	 	 	 	 	//!  number of bytes defined by SETUP_AW)
#define RF2G_REG_RX_ADDR_P1     0x0B    //!< Receive address data pipe 1.
#define RF2G_REG_RX_ADDR_P2     0x0C    //!< Receive address data pipe 2.
#define RF2G_REG_RX_ADDR_P3     0x0D    //!< Receive address data pipe 3.
#define RF2G_REG_RX_ADDR_P4     0x0E    //!< Receive address data pipe 4.
#define RF2G_REG_RX_ADDR_P5     0x0F    //!< Receive address data pipe 5.
#define RF2G_REG_TX_ADDR        0x10    //!< Transmit address.
 	 	 	 	 	 	 	 		 	//!  Used for a PTX device only. (LSB byte is written first)
 	 	 	 	 	 	 	 	 	 	//!  Set RX_ADDR_P0 equal to this address to handle automatic
 	 	 	 	 	 	 	 	 	 	//!  acknowledge if this is a PTX device
#define RF2G_REG_RX_PW_P0       0x11    //!< Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).
#define RF2G_REG_RX_PW_P1       0x12    //!< Number of bytes in RX payload in data pipe 1 (1 to 32 bytes).
#define RF2G_REG_RX_PW_P2       0x13    //!< Number of bytes in RX payload in data pipe 2 (1 to 32 bytes).
#define RF2G_REG_RX_PW_P3       0x14    //!< Number of bytes in RX payload in data pipe 3 (1 to 32 bytes).
#define RF2G_REG_RX_PW_P4       0x15    //!< Number of bytes in RX payload in data pipe 4 (1 to 32 bytes).
#define RF2G_REG_RX_PW_P5       0x16    //!< Number of bytes in RX payload in data pipe 5 (1 to 32 bytes).
#define RF2G_REG_FIFO_STATUS    0x17    //!< FIFO Status Register
#define RF2G_REG_DYNPD          0x1C    //!< Enable dynamic payload length
#define RF2G_REG_FEATURE        0x1D    //!< Feature Register


/** @defgroup RF2G_REG_CONFIG:		(adr 0x00)	Configuration Register		*/
#define RF2G_REG_CONFIG_MASK_RX_DR		0x40	//!< Mask interrupt caused by RX_DR
 	 	 	 	 	 	 	 	 	 	 	 	//!  1: Interrupt not reflected on the IRQ pin
 	 	 	 	 	 	 	 	 	 	 	 	//!  0: Reflect RX_DR as active low interrupt on the IRQ pin
#define RF2G_REG_CONFIG_MASK_TX_DS		0x20	//!< Mask interrupt caused by TX_DS
#define RF2G_REG_CONFIG_MASK_MAX_RT		0x10	//!< Mask interrupt caused by MAX_RT
#define RF2G_REG_CONFIG_EN_CRC			0x08	//!< Enable CRC. Forced high if one of the bits in the EN_AA is high
#define RF2G_REG_CONFIG_CRCO			0x04	//!< CRC encoding scheme ('0' - 1 byte, '1' - 2 bytes)
#define RF2G_REG_CONFIG_PWR_UP			0x02	//!< 1: POWER UP, 0:POWER DOWN
#define RF2G_REG_CONFIG_PRIM_RX			0x01	//!< RX/TX control, 1: PRX, 0: PTX
/** @} */

/** @defgroup RF2G_REG_EN_AA:	 	(adr 0x01)	Enable „Auto Acknowledgment" Function */
#define RF2G_REG_EN_AA_ENAA_P5			0x20	//!< Enable auto acknowledgement data pipe 5
#define RF2G_REG_EN_AA_ENAA_P4			0x10	//!< Enable auto acknowledgement data pipe 4
#define RF2G_REG_EN_AA_ENAA_P3			0x08	//!< Enable auto acknowledgement data pipe 3
#define RF2G_REG_EN_AA_ENAA_P2			0x04	//!< Enable auto acknowledgement data pipe 2
#define RF2G_REG_EN_AA_ENAA_P1			0x02	//!< Enable auto acknowledgement data pipe 1
#define RF2G_REG_EN_AA_ENAA_P0			0x01	//!< Enable auto acknowledgement data pipe 0
/** @} */

/** @defgroup RF2G_REG_EN_RXADDR:	(adr 0x02)	Enabled RX Addresses		*/
#define RF2G_REG_EN_RXADDR_ERX_P5		0x20	//!< Enable data pipe 5.
#define RF2G_REG_EN_RXADDR_ERX_P4		0x10	//!< Enable data pipe 4.
#define RF2G_REG_EN_RXADDR_ERX_P3		0x08	//!< Enable data pipe 3.
#define RF2G_REG_EN_RXADDR_ERX_P2		0x04	//!< Enable data pipe 2.
#define RF2G_REG_EN_RXADDR_ERX_P1		0x02	//!< Enable data pipe 1.
#define RF2G_REG_EN_RXADDR_ERX_P0		0x01	//!< Enable data pipe 0.
/** @} */

/** @defgroup RF2G_REG_SETUP_AW:		(adr 0x03)	Setup of Address Widths (common for all data pipes)*/
#define RF2G_REG_SETUP_AW_AW_3bytes		0x01	//!< 3 bytes
#define RF2G_REG_SETUP_AW_AW_4bytes		0x02	//!< 4 bytes
#define RF2G_REG_SETUP_AW_AW_5bytes		0x03	//!< 5 bytes
/** @} */

/** @defgroup RF2G_REG_SETUP_RETR:	(adr 0x04)	Setup of Automatic Retransmission */
#define RF2G_REG_SETUP_RETR_ARD(x)	((x)<<4)	//!< Auto Retransmission Delay, (x+1)*250uS
#define RF2G_REG_SETUP_RETR_ARC(x)		(x)		//!< Auto Retransmission Count (0-15)
/** @} */

/** @defgroup RF2G_REG_RF_CH:		(adr 0x05)	RF Channel					*/
#define RF2G_REG_RF_CH_RF_CH(x)			(x)		//!< Sets the frequency channel (0x00-0x7F)
/** @} */

/** @defgroup RF2G_REG_RF_SETUP:		(adr 0x05)	RF Setup Register			*/
#define NRF_REG_RF_SETUP_CONT_WAVE		0x80	//!< Enables continuous carrier transmit when high.
#define RF2G_REG_RF_SETUP_RF_DR_LOW		0x20	//!< Set Air Data Rate. See RF_DR_HIGH for encoding
#define RF2G_REG_RF_SETUP_PLL_LOCK		0x10	//!< Force PLL lock signal. Only used in test
#define RF2G_REG_RF_SETUP_RF_DR_HIGH	0x08	//!< Set Air Data Rate.
#define RF2G_REG_RF_SETUP_RF_DR_1Mbps	0x00	//!<  – 1Mbps
#define RF2G_REG_RF_SETUP_RF_DR_2Mbps	0x08	//!<  – 2Mbps (default)
#define RF2G_REG_RF_SETUP_RF_DR_250Kbps	0x20	//!<  – 250Kbps
#define RF2G_REG_RF_SETUP_RF_DR_2Mbps_	0x28	//!<  – 2Mbps
#define RF2G_REG_RF_SETUP_RF_PWR		0x06	//!< Set RF output power in TX mode
#define RF2G_REG_RF_SETUP_RF_PWR(x)	((x)<< 1)	//!<  x (0-3)
#define RFM73_REG_RF_SETUP_RF_PWR_M10dBm 0x00	//!<  -10 dBm
#define RFM73_REG_RF_SETUP_RF_PWR_M5dBm	0x02	//!<  -5 dBm
#define RFM73_REG_RF_SETUP_RF_PWR_0dBm	0x04	//!<   0 dBm
#define RFM73_REG_RF_SETUP_RF_PWR_5dBm	0x06	//!<   5 dBm
#define RF2G_REG_RF_SETUP_RF_PWR_M18dBm	0x00	//!<  -10 dBm
#define RF2G_REG_RF_SETUP_RF_PWR_M12dBm	0x02	//!<  -12 dBm
#define RF2G_REG_RF_SETUP_RF_PWR_M6dBm	0x04	//!<  -6 dBm
#define RF2G_REG_RF_SETUP_RF_PWR_0dBm	0x06	//!<   5 dBm
#define RFM73_REG_RF_SETUP_LNA_HCURR	0x01	//!< Setup LNA gain, 0-Low gain(20dB down)
/** @} */

/** @defgroup RF2G_REG_STATUS:		(adr 0x07)	Status Register				*/
#define	RFM73_REG_STATUS_RBANK			0x80	//!< Register bank selection states
#define	RF2G_REG_STATUS_RX_DR			0x40	//!< Data Ready RX FIFO interrupt (Write 1 to clear bit)
#define	RF2G_REG_STATUS_TX_DS			0x20	//!< Data Sent TX FIFO interrupt (Write 1 to clear bit)
#define	RF2G_REG_STATUS_MAX_RT			0x10	//!< Maximum number of TX retransmits interrupt
#define	RF2G_REG_STATUS_RX_P_NO			0x0E	//!< Data pipe number for the payload available for reading from RX_FIFO
#define	RF2G_REG_STATUS_TX_FULL			0x01	//!< TX FIFO full flag
/** @} */

/** @defgroup RF2G_REG_OBSERVE_TX:   (adr 0x08)  Transmit observe register	*/
#define RF2G_REG_OBSERVE_TX_PLOS_CNT	0xF0	//!< Count lost packets. The counter is overflow protected to 15,
												//!  and discontinues at max until reset. The counter is reset by
												//!  writing to RF_CH.
#define RF2G_REG_OBSERVE_TX_ARC_CNT		0x0F	//!< Count retransmitted packets. The counter is reset when
 	 	 	 	 	 	 	 	 	 	 	 	//!  transmission of a new packet starts.
/** @} */

/** @defgroup RF2G_REG_CD:           (adr 0x09)  Carrier Detect				*/
#define RF2G_REG_CD_CD					0x01	//!< Carrier Detect
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P0:	(adr 0x0A)  Receive address data pipe 0.*/
#define RF2G_REG_RX_ADDR_P0_default	0xE7E7E7E7E7
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P1:   (adr 0x0B)  Receive address data pipe 1.*/
#define RF2G_REG_RX_ADDR_P1_default	0xC2C2C2C2C2
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P2:   (adr 0x0C)  Receive address data pipe 2.*/
#define RF2G_REG_RX_ADDR_P2_default	0xC3
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P3:   (adr 0x0D)  Receive address data pipe 3.*/
#define RF2G_REG_RX_ADDR_P3_default	0xC4
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P4:   (adr 0x0E)  Receive address data pipe 4.*/
#define RF2G_REG_RX_ADDR_P4_default	0xC5
/** @} */

/** @defgroup RF2G_REG_RX_ADDR_P5:   (adr 0x0F)  Receive address data pipe 5.*/
#define RF2G_REG_RX_ADDR_P5_default	0xC6
/** @} */

/** @defgroup RF2G_REG_TX_ADDR:      (adr 0x10)  Transmit address.			*/
#define RF2G_REG_TX_ADDR_default		0xE7E7E7E7E7
/** @} */

/** @defgroup RF2G_REG_RX_PW_P0:     (adr 0x11)  Number of bytes in RX payload in data pipe 0 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_RX_PW_P1:     (adr 0x12)  Number of bytes in RX payload in data pipe 1 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_RX_PW_P2:     (adr 0x13)  Number of bytes in RX payload in data pipe 2 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_RX_PW_P3:     (adr 0x14)  Number of bytes in RX payload in data pipe 3 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_RX_PW_P4:     (adr 0x15)  Number of bytes in RX payload in data pipe 4 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_RX_PW_P5:     (adr 0x16)  Number of bytes in RX payload in data pipe 5 (1 to 32 bytes).*/
/** @} */

/** @defgroup RF2G_REG_FIFO_STATUS:  (adr 0x17)  FIFO Status Register		*/
#define RF2G_REG_FIFO_STATUS_TX_REUSE	0x40	//!< Reuse last transmitted data packet if set high.
#define RF2G_REG_FIFO_STATUS_TX_FULL	0x20	//!< TX FIFO full flag
#define RF2G_REG_FIFO_STATUS_TX_EMPTY	0x10	//!< TX FIFO empty flag.
#define RF2G_REG_FIFO_STATUS_RX_FULL	0x02	//!< RX FIFO full flag
#define RF2G_REG_FIFO_STATUS_RX_EMPTY	0x01	//!< RX FIFO empty flag.
/** @} */

/** @defgroup RF2G_REG_DYNPD:        (adr 0x1C)  Enable dynamic payload length*/
#define RF2G_REG_DYNPD_DPL_P5			0x20	//!< Enable dynamic payload length data pipe 5
#define RF2G_REG_DYNPD_DPL_P4			0x10	//!< Enable dynamic payload length data pipe 4
#define RF2G_REG_DYNPD_DPL_P3			0x08	//!< Enable dynamic payload length data pipe 3
#define RF2G_REG_DYNPD_DPL_P2			0x04	//!< Enable dynamic payload length data pipe 2
#define RF2G_REG_DYNPD_DPL_P1			0x02	//!< Enable dynamic payload length data pipe 1
#define RF2G_REG_DYNPD_DPL_P0			0x01	//!< Enable dynamic payload length data pipe 0
/** @} */

/** @defgroup RF2G_REG_FEATURE:      (adr 0x1D)  Feature Register			*/
#define RF2G_REG_FEATURE_EN_DPL			0x04	//!< Enables Dynamic Payload Length
#define RF2G_REG_FEATURE_EN_ACK_PAY		0x02	//!< Enables Payload with ACK
#define RF2G_REG_FEATURE_EN_DYN_ACK		0x01	//!< Enables the W_TX_PAYLOAD_NOACK command
/** @} */

///-----------------------------------------------------------------------------
/// Register Bank 1
/// There are two register banks, which can be toggled by SPI command “ACTIVATE”
/// followed with 0x53 byte, and bank status can be read from Bank0_REG7 [7].
///-----------------------------------------------------------------------------
#define RFM73_BANK1_REG0			0x00	//!< Must write with 0x404B01E2
#define RFM73_BANK1_REG1			0x01	//!< Must write with 0xC04B0000
#define RFM73_BANK1_REG2			0x02	//!< Must write with 0xD0FC8C02
#define RFM73_BANK1_REG3			0x03	//!< Must write with 0x99003941
#define RFM73_BANK1_REG4			0x04	//!< Must write with 0xD99E860B, For single carrier mode:0xD99E8621
#define RFM73_BANK1_REG5			0x05	//!< Must write with 0x24067FA6 (Disable RSSI)
#define RFM73_BANK1_REG7			0x07	//!< Register bank selection states.
#define RFM73_BANK1_CHIP_ID			0x08	//!< Chip ID: 0x00000063(RF73)
#define RFM73_BANK1_REGC			0x0C	//!< Please initialize with 0x05731200 For 120us mode:0x00731200
#define RFM73_BANK1_NEW_FEATURE		0x0D	//!< Please initialize with 0x0080B436
#define RFM73_BANK1_RAMP			0x0E	//!< Ramp curve

/** @defgroup RFM73_BANK1_REG0:			(adr: 0x00)	 Must write with 0x404B01E2 */
#define RFM73_BANK1_REG0_val		0x404B01E2
/** @} */

/** @defgroup RFM73_BANK1_REG1:			(adr: 0x01)	 Must write with 0xC04B0000 */
#define RFM73_BANK1_REG1_val		0xC04B0000
/** @} */

/** @defgroup RFM73_BANK1_REG2:			(adr: 0x02)	 Must write with 0xD0FC8C02 */
#define RFM73_BANK1_REG2_val		0xD0FC8C02
/** @} */

/** @defgroup RFM73_BANK1_REG3:			(adr: 0x03)	 Must write with 0x99003941 */
#define RFM73_BANK1_REG3_val		0x99003941
/** @} */

/** @defgroup RFM73_BANK1_REG4:			(adr: 0x04)	 Must write with 0xD99E860B, For single carrier mode:0xD99E8621 */
#define RFM73_BANK1_REG4_val1		0xD99E860B
#define RFM73_BANK1_REG4_val2		0xD99E8621		//!< single carrier mode
#define RFM73_BANK1_REG4_RX_SEN		0x00200000		//!< High sensitivity RX mode (different CD detection values)
#define RFM73_BANK1_REG4_TX_PWR		0x00100000		//!< RF output power in TX mode. 0:Low power(-30dB down)
/** @} */

/** @defgroup RFM73_BANK1_REG5:			(adr: 0x05)	 Must write with 0x24067FA6 (Disable RSSI) */
#define RFM73_BANK1_REG5_val		0x24067FA6		//!< Disable RSSI
#define RFM73_BANK1_REG5_RSSI_TH	0x0F000000		//!< RSS I Threshold for CD detect
#define RFM73_BANK1_REG5_RSSI_EN	0x00040000		//!< RSSI measurement: 0:Enable
/** @} */

/** @defgroup RFM73_BANK1_REG7:			(adr: 0x07)	 Register bank selection states. */
#define RFM73_BANK1_REG7_RBANK		0x80			//!< Register bank
/** @} */

/** @defgroup RFM73_BANK1_CHIP_ID:		(adr: 0x08)	 Chip ID: 0x00000063(RF73)  */
#define RFM73_BANK1_CHIP_ID_RF73	0x00000063
/** @} */

/** @defgroup RFM73_BANK1_REGC:			(adr: 0x0C)	 Please initialize with 0x05731200 For 120us mode:0x00731200 */
#define RFM73_BANK1_REGC_val1		0x05731200
#define RFM73_BANK1_REGC_val2		0x00731200
/** @} */

/** @defgroup RFM73_BANK1_NEW_FEATURE:	(adr: 0x0D)	 Please initialize with 0x0080B436 */
#define RFM73_BANK1_NEW_FEATURE_val	0x0080B436
/** @} */

/** @defgroup RFM73_BANK1_RAMP:			(adr: 0x0E)	 Ramp curve					*/
#define RFM73_BANK1_RAMP_val		0xFFEF7DF208082082041041
/** @} */



#endif /* SERVICES_RF_RF2GHZ_RF2GHZ_H_ */
