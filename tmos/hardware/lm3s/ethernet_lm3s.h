/**************************************************************************//**
 * @ingroup	 lm3s_ethernet
 * @{
 * @file     lm3s/ethernet_lm3s.h
 * @brief    Macros used when accessing the Ethernet MAC hardware.
 *
 * Copyright (c) 2005-2009 Luminary Micro, Inc.  All rights reserved.
 * Software License Agreement
 *
 * Luminary Micro, Inc. (LMI) is supplying this software for use solely and
 * exclusively on LMI's microcontroller products.
 *
 * The software is owned by LMI and/or its suppliers, and is protected under
 * applicable copyright laws.  All rights are reserved.  You may not combine
 * this software with "viral" open-source software in order to form a larger
 * program.  Any use in violation of the foregoing restrictions may subject
 * the user to criminal sanctions under applicable laws, as well as to civil
 * liability for the breach of the terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * LMI SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * This is part of revision 5228 of the Stellaris Firmware Development Package.
 *
 *
 ******************************************************************************/
#ifndef ETHERNET_LM3S_H_
#define ETHERNET_LM3S_H_

#include <tmos_types.h>
#include <ethernet_inc.h>

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

/*----------       Ethernet Controller (MAC)                      -----------*/
// <g> Ethernet Controller (MAC)
typedef struct
{
    __IO uint32_t MACRIS;       //!< 00 (__I) Ethernet MAC Raw Interrupt Status
#define MACIACK MACRIS   	 	//!< 00 (__O)Interrupt Acknowledge Register
    __IO uint32_t MACIM;		//!< 04 Interrupt Mask Register
    __IO uint32_t MACRCTL;  	//!< 08 Receive Control Register
    __IO uint32_t MACTCTL;		//!< 0C Transmit Control Register
    __IO uint32_t MACDATA;		//!< 10 Data Register
    __IO uint32_t MACIA0;		//!< 14 Individual Address Register 0
    __IO uint32_t MACIA1;		//!< 18 Individual Address Register 1
    __IO uint32_t MACTHR;		//!< 1C Threshold Register
    __IO uint32_t MACMCTL;		//!< 20 Management Control Register
    __IO uint32_t MACMDV;       //!< 24 Management Divider Register
    uint32_t  RESERVED1;		//!< 28
    __IO uint32_t MACMTXD;      //!< 2C Management Transmit Data Reg
    __IO uint32_t MACMRXD;      //!< 30 Management Receive Data Reg
    __I  uint32_t MACNP;        //!< 34 Number of Packets Register
    __IO uint32_t MACTR;        //!< 38 Transmission Request Register
    __IO uint32_t MACTS;        //!< 3C Timer Support Register
    __IO uint32_t MACLED;       //!< 40 LED Encoding
    __IO uint32_t MDIX;         //!< 44 Ethernet PHY MDIX
} MAC_Type;
// </g>


//*****************************************************************************
//
// Values that can be passed to EthernetConfigSet as the ulConfig value, and
// returned from EthernetConfigGet.
//
//*****************************************************************************
#define ETH_CFG_TS_TSEN         0x010000    // Enable Timestamp (CCP)
#define ETH_CFG_RX_BADCRCDIS    0x000800    // Disable RX BAD CRC Packets
#define ETH_CFG_RX_PRMSEN       0x000400    // Enable RX Promiscuous
#define ETH_CFG_RX_AMULEN       0x000200    // Enable RX Multicast
#define ETH_CFG_TX_DPLXEN       0x000010    // Enable TX Duplex Mode
#define ETH_CFG_TX_CRCEN        0x000004    // Enable TX CRC Generation
#define ETH_CFG_TX_PADEN        0x000002    // Enable TX Padding

//*****************************************************************************
//
// Values that can be passed to EthernetIntEnable, EthernetIntDisable, and
// EthernetIntClear as the ulIntFlags parameter, and returned from
// EthernetIntStatus.
//
//*****************************************************************************
#define ETH_INT_PHY             0x040       // PHY Event/Interrupt
#define ETH_INT_MDIO            0x020       // Management Transaction
#define ETH_INT_RXER            0x010       // RX Error
#define ETH_INT_RXOF            0x008       // RX FIFO Overrun
#define ETH_INT_TX              0x004       // TX Complete
#define ETH_INT_TXER            0x002       // TX Error
#define ETH_INT_RX              0x001       // RX Complete

//*****************************************************************************
//
// Helper Macros for Ethernet Processing
//
//*****************************************************************************
//
// htonl/ntohl - big endian/little endian byte swapping macros for
// 32-bit (long) values
//
//*****************************************************************************
#ifndef htonl
    #define htonl(a)                    \
        ((((a) >> 24) & 0x000000ff) |   \
         (((a) >>  8) & 0x0000ff00) |   \
         (((a) <<  8) & 0x00ff0000) |   \
         (((a) << 24) & 0xff000000))
#endif

#ifndef ntohl
    #define ntohl(a)    htonl((a))
#endif

//*****************************************************************************
//
// htons/ntohs - big endian/little endian byte swapping macros for
// 16-bit (short) values
//
//*****************************************************************************
#ifndef htons
    #define htons(a)                \
        ((((a) >> 8) & 0x00ff) |    \
         (((a) << 8) & 0xff00))
#endif

#ifndef ntohs
    #define ntohs(a)    htons((a))
#endif

typedef bool tBoolean;
//*****************************************************************************
//
// API Function prototypes
//
//*****************************************************************************
extern void EthernetInitExpClk(MAC_Type* mac, unsigned long ulEthClk);
extern void EthernetConfigSet(MAC_Type* mac, unsigned long ulConfig);
extern unsigned long EthernetConfigGet(MAC_Type* mac);
extern void EthernetMACAddrSet(MAC_Type* mac, unsigned char *pucMACAddr);
extern void EthernetMACAddrGet(MAC_Type* mac, unsigned char *pucMACAddr);
extern void EthernetEnable(MAC_Type* mac);
extern void EthernetDisable(MAC_Type* mac);
extern tBoolean EthernetPacketAvail(MAC_Type* mac);
extern tBoolean EthernetSpaceAvail(MAC_Type* mac);
extern long EthernetPacketGetNonBlocking(MAC_Type* mac, unsigned char *pucBuf,
		long lBufLen);
extern long EthernetPacketGet(MAC_Type* mac, unsigned char *pucBuf,
		long lBufLen);
extern long EthernetPacketPutNonBlocking(MAC_Type* mac, unsigned char *pucBuf,
		long lBufLen);
extern long EthernetPacketPut(MAC_Type* mac, unsigned char *pucBuf,
		long lBufLen);
//extern void EthernetIntRegister(MAC_Type* mac, void(*pfnHandler)(void));
//extern void EthernetIntUnregister(MAC_Type* mac);
extern void EthernetIntEnable(MAC_Type* mac, unsigned long ulIntFlags);
extern void EthernetIntDisable(MAC_Type* mac, unsigned long ulIntFlags);
extern unsigned long EthernetIntStatus(MAC_Type* mac, tBoolean bMasked);
extern void EthernetIntClear(MAC_Type* mac, unsigned long ulIntFlags);
extern void EthernetPHYWrite(MAC_Type* mac, unsigned char ucRegAddr,
		unsigned long ulData);
extern unsigned long EthernetPHYRead(MAC_Type* mac, unsigned char ucRegAddr);
extern void EthernetPHYPowerOff(MAC_Type* mac);
extern void EthernetPHYPowerOn(MAC_Type* mac);


//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif


#endif /* ETHERNET_LM3S_H_ */
