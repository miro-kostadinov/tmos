/**************************************************************************//**
 * @ingroup	hardware_stm32f2_eth_phy
 * @file	hardware/stm32/stm32f2/eth_phy_f2.h
 * @brief  	STM32F2 ETH PHY
 * @date    27. July 2016
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32f2_eth_phy  Ethernet PHY Registers
 * @ingroup	 hardware_stm32f2
 * Source and definitions for Ethernet PHY Registers (TLK110, LAN87xx)
 * @{
 *
 ******************************************************************************/

#ifndef HARDWARE_STM32_STM32F2_ETH_PHY_F2_H_
#define HARDWARE_STM32_STM32F2_ETH_PHY_F2_H_

#include <eth_f2.h>

//// Common PHY Registers
#define PHY_REG_BCR 	0x00    //!< Transceiver Basic Control Register
#define PHY_REG_BSR   	0x01    //!< Transceiver Basic Status Register

///  PHY_REG_BCR:      (phy Offset: 0x00) Transceiver Basic Control Register
#define PHY_REG_BCR_RESET                   0x8000  //!< PHY Reset
#define PHY_REG_BCR_LOOPBACK                0x4000  //!< Select loop-back mode
#define PHY_REG_BCR_FULLDUPLEX_100M         0x2100  //!< Set the full-duplex mode at 100 Mb/s
#define PHY_REG_BCR_HALFDUPLEX_100M         0x2000  //!< Set the half-duplex mode at 100 Mb/s
#define PHY_REG_BCR_FULLDUPLEX_10M          0x0100  //!< Set the full-duplex mode at 10 Mb/s
#define PHY_REG_BCR_HALFDUPLEX_10M          0x0000  //!< Set the half-duplex mode at 10 Mb/s
#define PHY_REG_BCR_AUTONEGOTIATION         0x1000  //!< Enable auto-negotiation function
#define PHY_REG_BCR_RESTART_AUTONEGO		0x0200  //!< Restart auto-negotiation function
#define PHY_REG_BCR_POWERDOWN               0x0800  //!< Select the power down mode
#define PHY_REG_BCR_ISOLATE                 0x0400  //!< Isolate PHY from MII

///  PHY_REG_SR:      (phy Offset: 0x01) Transceiver Basic Status Register
#define PHY_REG_BSR_AUTONEGO_COMPLETE     	0x0020  //!< Auto-Negotiation process completed
#define PHY_REG_BSR_LINKED_STATUS         	0x0004  //!< Valid link established
#define PHY_REG_BSR_JABBER_DETECTION      	0x0002  //!< Jabber condition detected


//// Extended PHY Registers
#ifdef ETH_PHY_DP83848
#define PHY_REG_SR      0x10    //!< PHY status register Offset
#define PHY_REG_MICR    0x11    //!< MII Interrupt Control Register
#define PHY_REG_MISR    0x12    //!< MII Interrupt Status and Misc. Control Register


///  PHY_REG_SR:      (phy Offset: 0x10) PHY status register
#define PHY_REG_SR_LINK_STATUS              0x0001  //!<  PHY Link mask
#define PHY_REG_SR_SPEED_STATUS             0x0002  //!<  PHY Speed mask
#define PHY_REG_SR_DUPLEX_STATUS            0x0004  //!<  PHY Duplex mask

///  PHY_REG_MICR:    (phy Offset: 0x11)  MII Interrupt Control Register
#define PHYREG_MICR_MICR_INT_EN             0x0002  //!<  PHY Enable interrupts
#define PHYREG_MICR_MICR_INT_OE             0x0001  //!<  PHY Enable output interrupt events

///  PHY_REG_MISR:    (phy Offset: 0x12) MII Interrupt Status and Misc. Control Register
#define PHY_REG_MISR_MISR_LINK_INT_EN       0x0020  //!<  Enable Interrupt on change of link status
#define PHY_REG_MISR_LINK_INTERRUPT         0x2000  //!<  PHY link status interrupt mask
#endif

#define ETH_PHY_LAN8700
#ifdef ETH_PHY_LAN8700

//// Extended PHY Registers
#define PHY_REG_IMR     0x1E    //!< PHY Interrupt Mask Register
#define PHY_REG_SR      0x1F    //!< PHY status register Offset

///  PHY_REG_IMR:      (phy Offset: 0x1E) Interrupt Mask Register
#define PHY_REG_IMR_ENERGYON     	0x80    //!< ENERGYON generated
#define PHY_REG_IMR_AUTONEGO_DONE	0x40    //!< Auto-Negotiation complete
#define PHY_REG_IMR_RFD			    0x20    //!< Remote Fault Detected
#define PHY_REG_IMR_LINK_STATUS     0x10    //!< Link Down (link status negated)
#define PHY_REG_IMR_AUTONEGO_ACK	0x08    //!< Auto-Negotiation LP Acknowledge
#define PHY_REG_IMR_PDF			    0x04    //!< Parallel Detection Fault
#define PHY_REG_IMR_PAGE 		    0x02    //!< Auto-Negotiation Page Received

///  PHY_REG_SR:      (phy Offset: 0x1F) PHY status register
#define PHY_REG_SR_SPEED_STATUS             0x0004  //!<  PHY Speed mask
#define PHY_REG_SR_DUPLEX_STATUS            0x0010  //!<  PHY Duplex mask
#endif

/// ---------------- TLK110 Registers -----------------------------------------
#define PHY_TLK110_BMCR 		0x00  //!< RW  Basic Mode Control Register
#define PHY_TLK110_BMSR 		0x01  //!< RO  Basic Mode Status Register
#define PHY_TLK110_PHYIDR1 		0x02  //!< RO  PHY Identifier Register 1
#define PHY_TLK110_PHYIDR2 		0x03  //!< RO  PHY Identifier Register 2
#define PHY_TLK110_ANAR 		0x04  //!< RW  Auto-Negotiation Advertisement Register
#define PHY_TLK110_ANLPAR 		0x05  //!< RO  Auto-Negotiation Link Partner Ability Register
#define PHY_TLK110_ANER 		0x06  //!< RO  Auto-Negotiation Expansion Register
#define PHY_TLK110_ANNPTR 		0x07  //!< RW  Auto-Negotiation Next Page TX
#define PHY_TLK110_ANLNPTR 		0x08  //!< RO  Auto-Negotiation Link Partner Ability Next Page Register
#define PHY_TLK110_SWSCR1 		0x09  //!< RW  Software Strap Control Register 1
#define PHY_TLK110_SWSCR2 		0x0A  //!< RW  Software Strap Control Register 2
#define PHY_TLK110_SWSCR3 		0x0B  //!< RW  Software Strap Control Register 3
#define PHY_TLK110_REGCR 		0x0D  //!< RW  Register control register
#define PHY_TLK110_ADDAR 		0x0E  //!< RW  Address or Data register
#define PHY_TLK110_PHYSTS 		0x10  //!< RO  PHY Status Register
#define PHY_TLK110_PHYSCR 		0x11  //!< RW  PHY Specific Control Register
#define PHY_TLK110_MISR1 		0x12  //!< RW  MII Interrupt Status Register 1
#define PHY_TLK110_MISR2 		0x13  //!< RW  MII Interrupt Status Register 2
#define PHY_TLK110_FCSCR 		0x14  //!< RO  False Carrier Sense Counter Register
#define PHY_TLK110_RECR 		0x15  //!< RO  Receive Error Count Register
#define PHY_TLK110_BISCR 		0x16  //!< RW  BIST Control Register
#define PHY_TLK110_RBR 			0x17  //!< RO  RMII and Status Register
#define PHY_TLK110_LEDCR 		0x18  //!< RW  LED Control Register
#define PHY_TLK110_PHYCR 		0x19  //!< RW  PHY Control Register
#define PHY_TLK110_10BTSCR 		0x1A  //!< RW  10Base-T Status/Control Register
#define PHY_TLK110_BICSR1 		0x1B  //!< RW  BIST Control and Status Register 1
#define PHY_TLK110_BICSR2 		0x1C  //!< RO  BIST Control and Status Register 2
#define PHY_TLK110_CDCR 		0x1E  //!< RW  Cable Diagnostic Control Register
#define PHY_TLK110_PHYRCR 		0x1F  //!< RW  PHY Reset Control Register

///  PHY_TLK110_SWSCR1:      (phy Offset: 0x09) RW  Software Strap Control Register 1
#define PHY_TLK110_SWSCR1_SWCFGDNE		0x8000  //!<  Software Strap Configuration Done

///  PHY_TLK110_PHYSTS:      (phy Offset: 0x10) RO  PHY Status Register
#define PHY_TLK110_PHYSTS_MDIX	 		0x4000  //!< MDI pairs swapped (Receive on TD pair, Transmit on RD pair)
#define PHY_TLK110_PHYSTS_RXERR	 		0x2000  //!< Receive error event has occurred since last read of RXERCNT register (0x0015)
#define PHY_TLK110_PHYSTS_POLARITY 		0x1000  //!< Inverted Polarity detected
#define PHY_TLK110_PHYSTS_FCSL	 		0x0800  //!< False Carrier event has occurred since last read of FCSCR register (0x0014)
#define PHY_TLK110_PHYSTS_SIGNAL 		0x0400  //!< Active high 100Base-TX unconditional Signal Detect indication from PMD
#define PHY_TLK110_PHYSTS_DESCRAMBLER	0x0200  //!< Active high 100Base-TX Descrambler Lock indication from PMD
#define PHY_TLK110_PHYSTS_PAGE			0x0100  //!< A new Link Code Word Page has been received. This bit is a duplicate of
												//!<  Page Received (bit 1) in the ANER register and it is cleared on read of
												//!<  the ANER register (0x0006).
#define PHY_TLK110_PHYSTS_MI_INT		0x0080  //!< Indicates that an internal interrupt is pending. Interrupt source can be
												//!<  determined by reading the MISR Register (0x0012). Reading the MISR will
												//!<  clear this Interrupt bit indication
#define PHY_TLK110_PHYSTS_RFAULT		0x0040  //!< Remote Fault condition detected. Fault criteria: notification from Link
												//!<  Partner of Remote Fault via Auto-Negotiation. Cleared on read of BMSR
												//!<  register (0x0001) or by reset
#define PHY_TLK110_PHYSTS_JABBER		0x0020  //!< Jabber condition detected. This bit has meaning only in 10 Mb/s mode.
												//!<  This bit is a duplicate of the Jabber Detect bit in the BMSR register (0x0001
#define PHY_TLK110_PHYSTS_ANEG_STAT		0x0010  //!< Auto-Negotiation complete
#define PHY_TLK110_PHYSTS_MII_LOOPBACK	0x0008  //!< MII Loopback
#define PHY_TLK110_PHYSTS_DUPLEX		0x0004  //!< Duplex Status: 1 = Full duplex mode
#define PHY_TLK110_PHYSTS_SPEED			0x0002  //!< Speed Status: 1 = 10 Mb/s mode 0 = 100 Mb/s mode
#define PHY_TLK110_PHYSTS_LINK			0x0001  //!< Link Status. This bit is a duplicate of the Link Status bit in the BMSR register (0x0001)

RES_CODE HAL_ETH_Init_PHY(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);

RES_CODE HAL_ETH_PHY_Init_LAN8720(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);
RES_CODE HAL_ETH_PHY_Init_TLK110(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);



#endif /* HARDWARE_STM32_STM32F2_ETH_PHY_F2_H_ */

/** @} */
