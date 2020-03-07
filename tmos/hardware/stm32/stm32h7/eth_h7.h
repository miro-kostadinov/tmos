/**************************************************************************//**
 * @ingroup	hardware_stm32h7_eth
 * @file	hardware/stm32/stm32h7/eth_h7.h
 * @brief  	STM32H7 ETH
 * @date    30. December 2019
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_stm32h7_eth  Ethernet (ETH): media access control (MAC) with DMA controller
 * @ingroup	 hardware_stm32h7
 * Source and definitions for STM32H7 Ethernet (ETH): media access control (MAC) with DMA controller
 * @{
 *
 ******************************************************************************/

#ifndef HARDWARE_STM32_STM32H7_ETH_H7_H_
#define HARDWARE_STM32_STM32H7_ETH_H7_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/***************************************************************************//**
 *  Ethernet (ETH): media access control (MAC) with DMA controller
 ******************************************************************************/
typedef struct
{
//--------------------- MAC Registers   ---------------------------------------/
  __IO uint32_t ETH_MACCR;			//!< (eth Offset: 0x0000) Operating mode configuration register
  __IO uint32_t ETH_MACECR;         //!< (eth Offset: 0x0004) Extended operating mode configuration register
  __IO uint32_t ETH_MACPFR;         //!< (eth Offset: 0x0008) Packet filtering control register
  __IO uint32_t ETH_MACWTR;         //!< (eth Offset: 0x000C) Watchdog timeout register
  __IO uint32_t ETH_MACHT0R;        //!< (eth Offset: 0x0010) Hash Table 0 register
  __IO uint32_t ETH_MACHT1R;        //!< (eth Offset: 0x0014) Hash Table 1 register
  __IO uint32_t reserved1[14];      //!< (eth Offset: 0x0018)
  __IO uint32_t ETH_MACVTR;         //!< (eth Offset: 0x0050) VLAN tag register
  __IO uint32_t reserved2;          //!< (eth Offset: 0x0054)
  __IO uint32_t ETH_MACVHTR;        //!< (eth Offset: 0x0058) VLAN Hash table register
  __IO uint32_t reserved3;		    //!< (eth Offset: 0x005C)
  __IO uint32_t ETH_MACVIR;         //!< (eth Offset: 0x0060) VLAN inclusion register
  __IO uint32_t ETH_MACIVIR;        //!< (eth Offset: 0x0064) Inner VLAN inclusion register
  __IO uint32_t reserved4[2];       //!< (eth Offset: 0x0068)
  __IO uint32_t ETH_MACTFCR;        //!< (eth Offset: 0x0070) Tx Queue flow control register
  __IO uint32_t reserved5[7];       //!< (eth Offset: 0x0074)
  __IO uint32_t ETH_MACRFCR;        //!< (eth Offset: 0x0090) Rx flow control register
  __IO uint32_t reserved6[7];       //!< (eth Offset: 0x0094)
  __IO uint32_t ETH_MACISR;         //!< (eth Offset: 0x00B0) Interrupt status register
  __IO uint32_t ETH_MACIER;         //!< (eth Offset: 0x00B4) Interrupt enable register
  __IO uint32_t ETH_MACRXTXSR;      //!< (eth Offset: 0x00B8) Rx Tx status register
  __IO uint32_t reserved7;          //!< (eth Offset: 0x00BC)
  __IO uint32_t ETH_MACPCSR;        //!< (eth Offset: 0x00C0) PMT control status register
  __IO uint32_t ETH_MACRWKPFR;      //!< (eth Offset: 0x00C4) Remote wakeup packet filter register
  __IO uint32_t reserved8[2];       //!< (eth Offset: 0x00C8)
  __IO uint32_t ETH_MACLCSR;        //!< (eth Offset: 0x00D0) LPI control status register
  __IO uint32_t ETH_MACLTCR;        //!< (eth Offset: 0x00D4) LPI timers control register
  __IO uint32_t ETH_MACLETR;        //!< (eth Offset: 0x00D8) LPI entry timer register
  __IO uint32_t ETH_MAC1USTCR;      //!< (eth Offset: 0x00DC) 1-microsecond-tick counter register
  __IO uint32_t reserved9[12];      //!< (eth Offset: 0x00E0)
  __IO uint32_t ETH_MACVR;          //!< (eth Offset: 0x0110) Version register
  __IO uint32_t ETH_MACDR;          //!< (eth Offset: 0x0114) Debug register
  __IO uint32_t reserved10;         //!< (eth Offset: 0x0118)
  __IO uint32_t ETH_MACHWF0R;       //!< (eth Offset: 0x011C)
  __IO uint32_t ETH_MACHWF1R;       //!< (eth Offset: 0x0120) HW feature 1 register
  __IO uint32_t ETH_MACHWF2R;       //!< (eth Offset: 0x0124) HW feature 2 register
  __IO uint32_t reserved11[54];     //!< (eth Offset: 0x0128)
  __IO uint32_t ETH_MACMDIOAR;      //!< (eth Offset: 0x0200) MDIO address register
  __IO uint32_t ETH_MACMDIODR;      //!< (eth Offset: 0x0204) MDIO data register
  __IO uint32_t reserved12[2];      //!< (eth Offset: 0x0208)
  __IO uint32_t ETH_MACARPAR;       //!< (eth Offset: 0x0210) ARP address register
  __IO uint32_t reserved13[59];     //!< (eth Offset: 0x0214)
  __IO uint32_t ETH_MACA0HR;        //!< (eth Offset: 0x0300) Address 0 high register
  __IO uint32_t ETH_MACA0LR;        //!< (eth Offset: 0x0304) Address 0 low register
  __IO uint32_t ETH_MACA1HR;        //!< (eth Offset: 0x0308) Address 1 high register
  __IO uint32_t ETH_MACA1LR;        //!< (eth Offset: 0x030C) Address 1 low register
  __IO uint32_t ETH_MACA2HR;        //!< (eth Offset: 0x0310) Address 1 high register
  __IO uint32_t ETH_MACA2LR;        //!< (eth Offset: 0x0314) Address 2 low register
  __IO uint32_t ETH_MACA3HR;        //!< (eth Offset: 0x0318) Address 3 high register
  __IO uint32_t ETH_MACA3LR;        //!< (eth Offset: 0x031C) Address 3 low register
  __IO uint32_t reserved14[248];    //!< (eth Offset: 0x0320)
  __IO uint32_t ETH_MMCCR;          //!< (eth Offset: 0x0700) MMC control register
  __IO uint32_t ETH_MMCRIR;         //!< (eth Offset: 0x0704) MMC Rx interrupt register
  __IO uint32_t ETH_MMCTIR;         //!< (eth Offset: 0x0708) MMC Tx interrupt register
  __IO uint32_t ETH_MMCRIMR;        //!< (eth Offset: 0x070C) MMC Rx interrupt mask register
  __IO uint32_t ETH_MMCTIMR;        //!< (eth Offset: 0x0710) MMC Tx interrupt mask register
  __IO uint32_t reserved15[14];     //!< (eth Offset: 0x0714)
  __IO uint32_t ETH_MMCTSCGPR;      //!< (eth Offset: 0x074C) Tx single collision good packets register
  __IO uint32_t ETH_MMCTMCGPR;      //!< (eth Offset: 0x0750) Tx multiple collision good packets register
  __IO uint32_t reserved16[5];      //!< (eth Offset: 0x0754)
  __IO uint32_t ETH_MMCTPCGR;       //!< (eth Offset: 0x0768) Tx packet count good register
  __IO uint32_t reserved17[10];     //!< (eth Offset: 0x076C)
  __IO uint32_t ETH_MMCRCRCEPR;     //!< (eth Offset: 0x0794) Rx CRC error packets register
  __IO uint32_t ETH_MMCRAEPR;       //!< (eth Offset: 0x0798) Rx alignment error packets register
  __IO uint32_t reserved18[10];     //!< (eth Offset: 0x079C)
  __IO uint32_t ETH_MMCRUPGR;       //!< (eth Offset: 0x07C4) Rx unicast packets good register
  __IO uint32_t reserved19[9];      //!< (eth Offset: 0x07C8)
  __IO uint32_t ETH_MMCTLPIMSTR;    //!< (eth Offset: 0x07EC) Tx LPI microsecond timer register
  __IO uint32_t ETH_MMCTLPITCR;     //!< (eth Offset: 0x07F0) Tx LPI transition counter register
  __IO uint32_t ETH_MMCRLPIMSTR;    //!< (eth Offset: 0x07F4) Rx LPI microsecond counter register
  __IO uint32_t ETH_MMCRLPITCR;     //!< (eth Offset: 0x07F8) Rx LPI transition counter register
  __IO uint32_t reserved20[65];     //!< (eth Offset: 0x07FC)
  __IO uint32_t ETH_MACL3L4C0R;     //!< (eth Offset: 0x0900) L3 and L4 control 0 register
  __IO uint32_t ETH_MACL4A0R;       //!< (eth Offset: 0x0904) Layer 4 address filter 0 register
  __IO uint32_t reserved21[2];      //!< (eth Offset: 0x0908)
  __IO uint32_t ETH_MACL3A0R0R;     //!< (eth Offset: 0x0910) Layer 3 address 0 filter 0 register
  __IO uint32_t ETH_MACL3A1R0R;     //!< (eth Offset: 0x0914) Layer 3 address 1 filter 0 register
  __IO uint32_t ETH_MACL3A2R0R;     //!< (eth Offset: 0x0918) Layer 3 Address 2 filter 0 register
  __IO uint32_t ETH_MACL3A3R0R;     //!< (eth Offset: 0x091C) Layer 3 Address 3 filter 0 register
  __IO uint32_t reserved22[4];      //!< (eth Offset: 0x0920)
  __IO uint32_t ETH_MACL3L4C1R;     //!< (eth Offset: 0x0930) L3 and L4 control 1 register
  __IO uint32_t ETH_MACL4A1R;       //!< (eth Offset: 0x0934) Layer 4 address filter 1 register
  __IO uint32_t reserved23[2];      //!< (eth Offset: 0x0938)
  __IO uint32_t ETH_MACL3A0R1R;     //!< (eth Offset: 0x0940) Layer 3 address 0 filter 1 register
  __IO uint32_t ETH_MACL3A1R1R;     //!< (eth Offset: 0x0944) Layer 3 address 1 filter 1 register
  __IO uint32_t ETH_MACL3A2R1R;     //!< (eth Offset: 0x0948) Layer 3 address 2 filter 1 register
  __IO uint32_t ETH_MACL3A3R1R;     //!< (eth Offset: 0x094C) Layer 3 address 3 filter 1 register
  __IO uint32_t reserved24[108];    //!< (eth Offset: 0x0950)
  __IO uint32_t ETH_MACTSCR;        //!< (eth Offset: 0x0B00) Timestamp control Register
  __IO uint32_t ETH_MACSSIR;        //!< (eth Offset: 0x0B04) Sub-second increment register
  __IO uint32_t ETH_MACSTSR;        //!< (eth Offset: 0x0B08) System time seconds register
  __IO uint32_t ETH_MACSTNR;        //!< (eth Offset: 0x0B0C) System time nanoseconds register
  __IO uint32_t ETH_MACSTSUR;       //!< (eth Offset: 0x0B10) System time seconds update register
  __IO uint32_t ETH_MACSTNUR;       //!< (eth Offset: 0x0B14) System time nanoseconds update register
  __IO uint32_t ETH_MACTSAR;        //!< (eth Offset: 0x0B18) Timestamp addend register
  __IO uint32_t reserved25;         //!< (eth Offset: 0x0B1C)
  __IO uint32_t ETH_MACTSSR;        //!< (eth Offset: 0x0B20) Timestamp status register
  __IO uint32_t reserved26[3];      //!< (eth Offset: 0x0B24)
  __IO uint32_t ETH_MACTTSSNR;      //!< (eth Offset: 0x0B30) Tx timestamp status nanoseconds register
  __IO uint32_t ETH_MACTTSSSR;      //!< (eth Offset: 0x0B34) Tx timestamp status seconds register
  __IO uint32_t reserved27[2];      //!< (eth Offset: 0x0B38)
  __IO uint32_t ETH_MACACR;         //!< (eth Offset: 0x0B40) Auxiliary control register
  __IO uint32_t reserved28;         //!< (eth Offset: 0x0B44)
  __IO uint32_t ETH_MACATSNR;       //!< (eth Offset: 0x0B48) Auxiliary timestamp nanoseconds register
  __IO uint32_t ETH_MACATSSR;       //!< (eth Offset: 0x0B4C) Auxiliary timestamp seconds register
  __IO uint32_t ETH_MACTSIACR;      //!< (eth Offset: 0x0B50) Timestamp Ingress asymmetric correction register
  __IO uint32_t ETH_MACTSEACR;      //!< (eth Offset: 0x0B54) Timestamp Egress asymmetric correction register
  __IO uint32_t ETH_MACTSICNR;      //!< (eth Offset: 0x0B58) Timestamp Ingress correction nanosecond register
  __IO uint32_t ETH_MACTSECNR;      //!< (eth Offset: 0x0B5C) Timestamp Egress correction nanosecond register
  __IO uint32_t reserved29[4];      //!< (eth Offset: 0x0B60)
  __IO uint32_t ETH_MACPPSCR;       //!< (eth Offset: 0x0B70) PPS control register [alternate]
  __IO uint32_t reserved30[3];      //!< (eth Offset: 0x0B74)
  __IO uint32_t ETH_MACPPSTTSR;     //!< (eth Offset: 0x0B80) PPS target time seconds register
  __IO uint32_t ETH_MACPPSTTNR;     //!< (eth Offset: 0x0B84) PPS target time nanoseconds register
  __IO uint32_t ETH_MACPPSIR;       //!< (eth Offset: 0x0B88) PPS interval register
  __IO uint32_t ETH_MACPPSWR;       //!< (eth Offset: 0x0B8C) PPS width register
  __IO uint32_t reserved31[12];     //!< (eth Offset: 0x0B90)
  __IO uint32_t ETH_MACPOCR;        //!< (eth Offset: 0x0BC0) PTP Offload control register
  __IO uint32_t ETH_MACSPI0R;       //!< (eth Offset: 0x0BC4) PTP Source Port Identity 0 Register
  __IO uint32_t ETH_MACSPI1R;       //!< (eth Offset: 0x0BC8) PTP Source port identity 1 register
  __IO uint32_t ETH_MACSPI2R;       //!< (eth Offset: 0x0BCC) PTP Source port identity 2 register
  __IO uint32_t ETH_MACLMIR;        //!< (eth Offset: 0x0BD0) Log message interval register
  __IO uint32_t reserved32[11];     //!< (eth Offset: 0x0BD4)
//--------------------- MTL Registers   ---------------------------------------/
  __IO uint32_t ETH_MTLOMR;         //!< (eth Offset: 0x0C00) Operating mode Register
  __IO uint32_t reserved33[7];      //!< (eth Offset: 0x0C04)
  __IO uint32_t ETH_MTLISR;         //!< (eth Offset: 0x0C20) Interrupt status Register
  __IO uint32_t reserved34[55];     //!< (eth Offset: 0x0C24)
  __IO uint32_t ETH_MTLTQOMR;       //!< (eth Offset: 0x0D00) Tx queue operating mode Register
  __IO uint32_t ETH_MTLTQUR;        //!< (eth Offset: 0x0D04) Tx queue underflow register
  __IO uint32_t ETH_MTLTQDR;        //!< (eth Offset: 0x0D08) Tx queue debug Register
  __IO uint32_t reserved35[8];      //!< (eth Offset: 0x0D0C)
  __IO uint32_t ETH_MTLQICSR;       //!< (eth Offset: 0x0D2C) Queue interrupt control status Register
  __IO uint32_t ETH_MTLRQOMR;       //!< (eth Offset: 0x0D30) Rx queue operating mode register
  __IO uint32_t ETH_MTLRQMPOCR;     //!< (eth Offset: 0x0D34) Rx queue missed packet and overflow counter register
  __IO uint32_t ETH_MTLRQDR;        //!< (eth Offset: 0x0D38) Rx queue debug register
  __IO uint32_t reserved36[177];    //!< (eth Offset: 0x0D3C)
//--------------------- DMA Registers   ---------------------------------------/
  __IO uint32_t ETH_DMAMR;          //!< (eth Offset: 0x1000) DMA mode register
  __IO uint32_t ETH_DMASBMR;        //!< (eth Offset: 0x1004) System bus mode register
  __IO uint32_t ETH_DMAISR;         //!< (eth Offset: 0x1008) Interrupt status register
  __IO uint32_t ETH_DMADSR;         //!< (eth Offset: 0x100C) Debug status register
  __IO uint32_t reserved37[60];     //!< (eth Offset: 0x1010)
  __IO uint32_t ETH_DMACCR;         //!< (eth Offset: 0x1100) Channel control register
  __IO uint32_t ETH_DMACTCR;        //!< (eth Offset: 0x1104) Channel transmit control register
  __IO uint32_t ETH_DMACRCR;        //!< (eth Offset: 0x1108) Channel receive control register
  __IO uint32_t reserved38[2];      //!< (eth Offset: 0x110C)
  __IO uint32_t ETH_DMACTDLAR;      //!< (eth Offset: 0x1114) Channel Tx descriptor list address register
  __IO uint32_t reserved39;         //!< (eth Offset: 0x1118)
  __IO uint32_t ETH_DMACRDLAR;      //!< (eth Offset: 0x111C) Channel Rx descriptor list address register
  __IO uint32_t ETH_DMACTDTPR;      //!< (eth Offset: 0x1120) Channel Tx descriptor tail pointer register
  __IO uint32_t reserved40;         //!< (eth Offset: 0x1124)
  __IO uint32_t ETH_DMACRDTPR;      //!< (eth Offset: 0x1128) Channel Rx descriptor tail pointer register
  __IO uint32_t ETH_DMACTDRLR;      //!< (eth Offset: 0x112C) Channel Tx descriptor ring length register
  __IO uint32_t ETH_DMACRDRLR;      //!< (eth Offset: 0x1130) Channel Rx descriptor ring length register
  __IO uint32_t ETH_DMACIER;        //!< (eth Offset: 0x1134) Channel interrupt enable register
  __IO uint32_t ETH_DMACRIWTR;      //!< (eth Offset: 0x1138) Channel Rx interrupt watchdog timer register
  __IO uint32_t ETH_DMACSFCSR;      //!< (eth Offset: 0x113C) ---
  __IO uint32_t reserved41;         //!< (eth Offset: 0x1140)
  __IO uint32_t ETH_DMACCATDR;      //!< (eth Offset: 0x1144) Channel current application transmit descriptor register
  __IO uint32_t reserved42;         //!< (eth Offset: 0x1148)
  __IO uint32_t ETH_DMACCARDR;      //!< (eth Offset: 0x114C) Channel current application receive descriptor register
  __IO uint32_t reserved43;         //!< (eth Offset: 0x1150)
  __IO uint32_t ETH_DMACCATBR;      //!< (eth Offset: 0x1154) Channel current application transmit buffer register
  __IO uint32_t reserved44;         //!< (eth Offset: 0x1158)
  __IO uint32_t ETH_DMACCARBR;      //!< (eth Offset: 0x115C) Channel current application receive buffer register
  __IO uint32_t ETH_DMACSR;         //!< (eth Offset: 0x1160) Channel status register
  __IO uint32_t reserved45[2];      //!< (eth Offset: 0x1164)
  __IO uint32_t ETH_DMACMFCR;       //!< (eth Offset: 0x116C) Channel missed frame count register
} ETH_TypeDef;


/***************************************************************************//**
 * @relates ETH_TypeDef
 * @{
 ******************************************************************************/

//--------------------- MAC Registers   ---------------------------------------/
/** @name  ETH_MACCR:		(eth Offset: 0x0000) Operating mode configuration register	*/
/** @{ */
#define ETH_MACCR_ARPEN    		0x80000000  //!< ARP Offload Enable
#define ETH_MACCR_SARC     		0x70000000  //!< Source Address Insertion or Replacement Control (in the SA field of all transmitted packets)
#define ETH_MACCR_SARC_INS_A0	0x20000000  //!<  Inserts the content of the MAC Address 0 registers
#define ETH_MACCR_SARC_REP_A0	0x30000000  //!<  Replaces the content of the MAC Address 0 registers
#define ETH_MACCR_SARC_INS_A1	0x60000000  //!<  Inserts the content of the MAC Address 1 registers
#define ETH_MACCR_SARC_REP_A1	0x70000000  //!<  Replaces the content of the MAC Address 1 registers
#define ETH_MACCR_IPC     		0x08000000  //!< Checksum Offload
#define ETH_MACCR_IPG     		0x07000000  //!< Inter-Packet Gap
#define ETH_MACCR_IPG_96   		0x00000000  //!<  96 bit times
#define ETH_MACCR_IPG_88   		0x01000000  //!<  88 bit times
#define ETH_MACCR_IPG_80   		0x02000000  //!<  80 bit times
#define ETH_MACCR_IPG_72   		0x03000000  //!<  72 bit times
#define ETH_MACCR_IPG_64   		0x04000000  //!<  64 bit times
#define ETH_MACCR_IPG_56   		0x05000000  //!<  56 bit times
#define ETH_MACCR_IPG_48   		0x06000000  //!<  48 bit times
#define ETH_MACCR_IPG_40   		0x07000000  //!<  40 bit times
#define ETH_MACCR_GPSLCE   		0x00800000  //!< Giant Packet Size Limit Control Enable
#define ETH_MACCR_S2KP     		0x00400000  //!< IEEE 802.3as Support for 2K Packets
#define ETH_MACCR_CST    		0x00200000  //!< CRC stripping for Type packets
#define ETH_MACCR_ACS     		0x00100000  //!< Automatic Pad or CRC Stripping
#define ETH_MACCR_WD      		0x00080000  //!< Watchdog disable
#define ETH_MACCR_JD      		0x00020000  //!< Jabber disable
#define ETH_MACCR_JE    		0x00010000  //!< Jumbo Packet Enable
#define ETH_MACCR_FES     		0x00004000  //!< MAC Speed
#define ETH_MACCR_FES_10Mbps	0x00000000  //!<  10 Mbps
#define ETH_MACCR_FES_100Mbps 	0x00004000  //!<  100 Mbps
#define ETH_MACCR_DM      		0x00002000  //!< Duplex mode
#define ETH_MACCR_LM      		0x00001000  //!< loopback mode
#define ETH_MACCR_ECRSFD   		0x00000800  //!< Enable Carrier Sense Before Transmission in Full-Duplex Mode
#define ETH_MACCR_DO     		0x00000400  //!< Disable Receive Own
#define ETH_MACCR_DCRS     		0x00000200  //!< Disable Carrier Sense During Transmission
#define ETH_MACCR_DR     		0x00000100  //!< Disable Retry
#define ETH_MACCR_BL      		0x00000060  //!< Back-off limit: random integer number (r) of slot time delays before rescheduling a transmission attempt during retries after a collision: 0 =< r <2^k
#define ETH_MACCR_BL_10    		0x00000000  //!<  k = min (n, 10)
#define ETH_MACCR_BL_8     		0x00000020  //!<  k = min (n, 8)
#define ETH_MACCR_BL_4     		0x00000040  //!<  k = min (n, 4)
#define ETH_MACCR_BL_1     		0x00000060  //!<  k = min (n, 1)
#define ETH_MACCR_DC      		0x00000010  //!< Deferral check
#define ETH_MACCR_PRELEN   		0x0000000C  //!< Preamble Length for Transmit packets
#define ETH_MACCR_PRELEN_7 		0x00000000  //!<  7 bytes of preamble
#define ETH_MACCR_PRELEN_5 		0x00000004  //!<  5 bytes of preamble
#define ETH_MACCR_PRELEN_3 		0x00000008  //!<  3 bytes of preamble
#define ETH_MACCR_TE      		0x00000002  //!< Transmitter enable
#define ETH_MACCR_RE      		0x00000001  //!< Receiver enable
/** @} */

/** @name  ETH_MACECR:      (eth Offset: 0x0004) Extended operating mode configuration register	*/
/** @{ */
#define ETH_MACECR_EIPG    		0x3E000000  //!< Extended Inter-Packet Gap
#define ETH_MACECR_EIPGEN  		0x01000000  //!< Extended Inter-Packet Gap Enable
#define ETH_MACECR_USP     		0x00040000  //!< Unicast Slow Protocol Packet Detect
#define ETH_MACECR_SPEN    		0x00020000  //!< Slow Protocol Detection Enable
#define ETH_MACECR_DCRCC   		0x00010000  //!< Disable CRC Checking for Received Packets
#define ETH_MACECR_GPSL    		0x00003FFF  //!< Giant Packet Size Limit
/** @} */

/** @name  ETH_MACPFR:      (eth Offset: 0x0008) Packet filtering control register	*/
/** @{ */
#define ETH_MACFFR_RA     		0x80000000  //!< Receive all
#define ETH_MACFFR_DNTU    		0x00200000  //!< Drop Non-TCP/UDP over IP Packets
#define ETH_MACFFR_IPFE    		0x00100000  //!< Layer 3 and Layer 4 Filter Enable
#define ETH_MACFFR_VTFE   		0x00010000  //!< VLAN Tag Filter Enable
#define ETH_MACFFR_HPF    		0x00000400  //!< Hash or perfect filter
#define ETH_MACFFR_SAF    		0x00000200  //!< Source address filter
#define ETH_MACFFR_SAIF   		0x00000100  //!< Source address inverse filtering
#define ETH_MACFFR_PCF    		  0x000000C0  //!< Pass control frames: 3 cases
#define ETH_MACFFR_PCF_BlockAll   0x00000000  //!<  MAC filters all control frames from reaching the application
#define ETH_MACFFR_PCF_BlockPause 0x00000040  //!<  MAC forwards all control packets except Pause packets to the application even if they fail the Address filter.
#define ETH_MACFFR_PCF_ForwardAll 0x00000080  //!<  MAC forwards all control frames to application even if they fail the Address Filter
#define ETH_MACFFR_PCF_AddrFilter 0x000000C0  //!<  MAC forwards control frames that pass the Address Filter. *
#define ETH_MACFFR_DBF    		0x00000020  //!< Disable Broadcast Packets
#define ETH_MACFFR_PM    		0x00000010  //!< Pass All Multicast
#define ETH_MACFFR_DAIF   		0x00000008  //!< Destination address inverse filtering
#define ETH_MACFFR_HM     		0x00000004  //!< Hash multicast
#define ETH_MACFFR_HU     		0x00000002  //!< Hash unicast
#define ETH_MACFFR_PM     		0x00000001  //!< Promiscuous mode
/** @} */


/** @name  ETH_MACWTR:      (eth Offset: 0x000C) Watchdog timeout register	  */
/** @{ */
#define ETH_MACWTR_PWE			0x00000100  //!< Programmable Watchdog Enable
#define ETH_MACWTR_WTO			0x0000000F  //!< Watchdog Timeout
/** @} */

/** @name  ETH_MACHT0R:     (eth Offset: 0x0010) Hash Table 0 register	      */
/** @{ */
#define ETH_MACHT0R_HT31T0		0xFFFFFFFF  //!< MAC Hash Table First 32 Bits
/** @} */

/** @name  ETH_MACHT1R:     (eth Offset: 0x0014) Hash Table 1 register		  */
/** @{ */
#define ETH_MACHT1R_HT63T32		0xFFFFFFFF  //!< MAC Hash Table Second 32 Bits
/** @} */

/** @name  ETH_MACVTR:      (eth Offset: 0x0050) VLAN tag register			  */
/** @{ */
#define ETH_MACVTR_EIVLRXS		0x80000000  //!< Enable Inner VLAN Tag in Rx Status
#define ETH_MACVTR_EIVLS		0x30000000  //!< Enable Inner VLAN Tag Stripping on Receive
#define ETH_MACVTR_EIVLS_NO		0x00000000  //!<  Do not strip
#define ETH_MACVTR_EIVLS_FP		0x10000000  //!<  Strip if VLAN filter passes
#define ETH_MACVTR_EIVLS_FF		0x20000000  //!<  Strip if VLAN filter fails
#define ETH_MACVTR_EIVLS_ALL	0x30000000  //!<  Always strip
#define ETH_MACVTR_ERIVLT		0x08000000  //!< Enable Inner VLAN Tag
#define ETH_MACVTR_EDVLP		0x04000000  //!< Enable Double VLAN Processing
#define ETH_MACVTR_VTHM			0x02000000  //!< VLAN Tag Hash Table Match Enable
#define ETH_MACVTR_EVLRXS		0x01000000  //!< Enable VLAN Tag in Rx status
#define ETH_MACVTR_EVLS			0x00600000  //!< Enable VLAN Tag Stripping on Receive
#define ETH_MACVTR_EVLS_NO		0x00000000  //!<  Do not strip
#define ETH_MACVTR_EVLS_FP		0x00200000  //!<  Strip if VLAN filter passes
#define ETH_MACVTR_EVLS_FF		0x00400000  //!<  Strip if VLAN filter fails
#define ETH_MACVTR_EVLS_ALL		0x00600000  //!<  Always strip
#define ETH_MACVTR_DOVLTC		0x00100000  //!< Disable VLAN Type Check
#define ETH_MACVTR_ERSVLM		0x00080000  //!< Enable Receive S-VLAN Match
#define ETH_MACVTR_ESVL			0x00040000  //!< Enable S-VLAN
#define ETH_MACVTR_VTIM			0x00020000  //!< VLAN Tag Inverse Match Enable
#define ETH_MACVTR_ETV			0x00010000  //!< Enable 12-Bit VLAN Tag Comparison
#define ETH_MACVTR_VL			0x0000FFFF  //!< VLAN Tag Identifier for Receive Packets
/** @} */

/** @name  ETH_MACVHTR:     (eth Offset: 0x0058) VLAN Hash table register	  */
/** @{ */
#define ETH_MACVHTR_VLHT		0x0000FFFF  //!< VLAN Hash Table
/** @} */

/** @name  ETH_MACVIR:      (eth Offset: 0x0060) VLAN inclusion register	  */
/** @{ */
#define ETH_MACVIR_VLTI			0x00100000  //!< VLAN Tag Input
#define ETH_MACVIR_CSVL			0x00080000  //!< C-VLAN or S-VLAN
#define ETH_MACVIR_VLP			0x00040000  //!< VLAN Priority Control
#define ETH_MACVIR_VLC			0x00030000  //!< VLAN Tag Control in Transmit Packets
#define ETH_MACVIR_VLT			0x0000FFFF  //!< VLAN Tag for Transmit Packets
/** @} */

/** @name  ETH_MACIVIR:     (eth Offset: 0x0064) Inner VLAN inclusion register  */
/** @{ */
#define ETH_MACIVIR_VLTI		0x00100000  //!< VLAN Tag Input
#define ETH_MACIVIR_CSVL		0x00080000  //!< C-VLAN or S-VLAN
#define ETH_MACIVIR_VLP			0x00040000  //!< VLAN Priority Control
#define ETH_MACIVIR_VLC			0x00030000  //!< VLAN Tag Control in Transmit Packets
#define ETH_MACIVIR_VLT			0x0000FFFF  //!< VLAN Tag for Transmit Packets
/** @} */

/** @name  ETH_MACTFCR:     (eth Offset: 0x0070) Tx Queue flow control register  */
/** @{ */
#define ETH_MACTFCR_PT			0xFFFF0000  //!< Pause Time
#define ETH_MACTFCR_DZPQ		0x00000080  //!< Disable Zero-Quanta Pause
#define ETH_MACTFCR_PLT			0x00000070  //!< Pause Low Threshold
#define ETH_MACTFCR_TFE			0x00000002  //!< Transmit Flow Control Enable
#define ETH_MACTFCR_FCB_BPA		0x00000001  //!< Flow Control Busy or Backpressure Activate
/** @} */

/** @name  ETH_MACRFCR:     (eth Offset: 0x0090) Rx flow control register	  */
/** @{ */
#define ETH_MACRFCR_UP			0x00000002  //!< Unicast Pause Packet Detect
#define ETH_MACRFCR_RFE			0x00000001  //!< Receive Flow Control Enable
/** @} */

/** @name  ETH_MACISR:      (eth Offset: 0x00B0) Interrupt status register	  */
/** @{ */
#define ETH_MACISR_RXSTSIS		0x00004000  //!< Receive Status Interrupt
#define ETH_MACISR_TXSTSIS		0x00002000  //!< Transmit Status Interrupt
#define ETH_MACISR_TSIS			0x00001000  //!< Timestamp Interrupt Status
#define ETH_MACISR_MMCTXIS		0x00000400  //!< MMC Transmit Interrupt Status
#define ETH_MACISR_MMCRXIS		0x00000200  //!< MMC Receive Interrupt Status
#define ETH_MACISR_MMCIS		0x00000100  //!< MMC Interrupt Status
#define ETH_MACISR_LPIIS		0x00000020  //!< LPI Interrupt Status
#define ETH_MACISR_PMTIS		0x00000010  //!< PMT Interrupt Status
#define ETH_MACISR_PHYIS		0x00000008  //!< PHY Interrupt
/** @} */

/** @name  ETH_MACIER:      (eth Offset: 0x00B4) Interrupt enable register	  */
/** @{ */
#define ETH_MACIER_RXSTSIE		0x00004000  //!< Receive Status Interrupt Enable
#define ETH_MACIER_TXSTSIE		0x00002000  //!< Transmit Status Interrupt Enable
#define ETH_MACIER_TSIE			0x00001000  //!< Timestamp Interrupt Enable
#define ETH_MACIER_LPIIE		0x00000020  //!< LPI Interrupt Enable
#define ETH_MACIER_PMTIE		0x00000010  //!< PMT Interrupt Enable
#define ETH_MACIER_PHYIE		0x00000008  //!< PHY Interrupt Enable
/** @} */

/** @name  ETH_MACRXTXSR:   (eth Offset: 0x00B8) Rx Tx status register		  */
/** @{ */
#define ETH_MACRXTXSR_RWT		0x00000100  //!< Receive Watchdog Timeout
#define ETH_MACRXTXSR_EXCOL		0x00000020  //!< Excessive Collisions
#define ETH_MACRXTXSR_LCOL		0x00000010  //!< Late Collision
#define ETH_MACRXTXSR_EXDEF		0x00000008  //!< Excessive Deferral
#define ETH_MACRXTXSR_LCARR		0x00000004  //!< Loss of Carrier
#define ETH_MACRXTXSR_NCARR		0x00000002  //!< No Carrier
#define ETH_MACRXTXSR_TJT		0x00000001  //!< Transmit Jabber Timeout
/** @} */

/** @name  ETH_MACPCSR:     (eth Offset: 0x00C0) PMT control status register  */
/** @{ */
#define ETH_MACPCSR_RWKFILTRST	0x80000000  //!< Remote wakeup Packet Filter Register Pointer Reset
#define ETH_MACPCSR_RWKPTR		0x1F000000  //!< Remote wakeup FIFO Pointer
#define ETH_MACPCSR_RWKPFE		0x00000400  //!< Remote wakeup Packet Forwarding Enable
#define ETH_MACPCSR_GLBLUCAST	0x00000200  //!< Global Unicast
#define ETH_MACPCSR_RWKPRCVD	0x00000040  //!< Remote wakeup Packet Received
#define ETH_MACPCSR_MGKPRCVD	0x00000020  //!< Magic Packet Received
#define ETH_MACPCSR_RWKPKTEN	0x00000004  //!< Remote wakeup Packet Enable
#define ETH_MACPCSR_MGKPKTEN	0x00000002  //!< Magic Packet Enable
#define ETH_MACPCSR_PWRDWN		0x00000001  //!< Power Down
/** @} */

/** @name  ETH_MACRWKPFR:   (eth Offset: 0x00C4) Remote wakeup packet filter register  */
/** @{ */
#define ETH_MACRWKPFR_MACRWKPFR	0xFFFFFFFF  //!< Remote wakeup packet filter
/** @} */

/** @name  ETH_MACLCSR:     (eth Offset: 0x00D0) LPI control status register  */
/** @{ */
#define ETH_MACLCSR_LPITE		0x00100000  //!< LPI Timer Enable
#define ETH_MACLCSR_LPITXA		0x00080000  //!< LPI Tx Automate
#define ETH_MACLCSR_PLSEN		0x00040000  //!< PHY Link Status Enable
#define ETH_MACLCSR_PLS			0x00020000  //!< PHY Link Status
#define ETH_MACLCSR_LPIEN		0x00010000  //!< LPI Enable
#define ETH_MACLCSR_RLPIST		0x00000200  //!< Receive LPI State
#define ETH_MACLCSR_TLPIST		0x00000100  //!< Transmit LPI State
#define ETH_MACLCSR_RLPIEX		0x00000008  //!< Receive LPI Exit
#define ETH_MACLCSR_RLPIEN		0x00000004  //!< Receive LPI Entry
#define ETH_MACLCSR_TLPIEX		0x00000002  //!< Transmit LPI Exit
#define ETH_MACLCSR_TLPIEN		0x00000001  //!< Transmit LPI Entry
/** @} */

/** @name  ETH_MACLTCR:     (eth Offset: 0x00D4) LPI timers control register  */
/** @{ */
#define ETH_MACLTCR_LST			0x03FF0000  //!< LPI LS Timer
#define ETH_MACLTCR_TWT			0x0000FFFF  //!< LPI TW Timer
/** @} */

/** @name  ETH_MACLETR:     (eth Offset: 0x00D8) LPI entry timer register	  */
/** @{ */
#define ETH_MACLETR_LPIET		0x000FFFF0  //!< LPI Entry Timer
/** @} */

/** @name  ETH_MAC1USTCR:   (eth Offset: 0x00DC) 1-microsecond-tick counter register  */
/** @{ */
#define ETH_MAC1USTCR_TIC_1US_CNTR	0x00000FFF  //!< 1 us tick Counter
/** @} */

/** @name  ETH_MACVR:       (eth Offset: 0x0110) Version register			  */
/** @{ */
#define ETH_MACVR_USERVER		0x0000FF00  //!< ST-defined version
#define ETH_MACVR_SNPSVER		0x000000FF  //!< IP version
/** @} */

/** @name  ETH_MACDR:       (eth Offset: 0x0114) Debug register				  */
/** @{ */
#define ETH_MACDR_TFCSTS		0x00060000  //!< MAC Transmit Packet Controller Status
#define ETH_MACDR_TFCSTS_IDLE	0x00000000  //!<  Idle state
#define ETH_MACDR_TFCSTS_WAIT	0x00020000  //!<  Waiting for one of the following:
 	 	 	 	 	 	 	 	 	 	 	//!<   – Status of the previous packet OR
 	 	 	 	 	 	 	 	 	 	 	//!<   – IPG or backoff period to be over
#define ETH_MACDR_TFCSTS_PAUSE	0x00040000  //!<  Generating and transmitting a Pause control packet (in full-duplex mode)
#define ETH_MACDR_TFCSTS_INPUT	0x00060000  //!<  Transferring input packet for transmission
#define ETH_MACDR_TPESTS		0x00010000  //!< MAC MII Transmit Protocol Engine Status
#define ETH_MACDR_RFCFCSTS		0x00000006  //!< MAC Receive Packet Controller FIFO Status
#define ETH_MACDR_RPESTS		0x00000001  //!< MAC MII Receive Protocol Engine Status
/** @} */

/** @name  ETH_MACHWF0R:    (eth Offset: 0x011C) ---						  */
/** @{ */
#define ETH_MACHWF0R_			0x00000000  //!<
/** @} */

/** @name  ETH_MACHWF1R:    (eth Offset: 0x0120) HW feature 1 register		  */
/** @{ */
#define ETH_MACHWF1R_L3L4FNUM	0xE8000000  //!< Total number of L3 or L4 Filters
#define ETH_MACHWF1R_HASHTBLSZ	0x03000000  //!< Hash Table Size
#define ETH_MACHWF1R_AVSEL		0x00100000  //!< AV Feature Enable
#define ETH_MACHWF1R_DBGMEMA	0x00080000  //!< DMA Debug Registers Enable
#define ETH_MACHWF1R_TSOEN		0x00040000  //!< TCP Segmentation Offload Enable
#define ETH_MACHWF1R_SPHEN		0x00020000  //!< Split Header Feature Enable
#define ETH_MACHWF1R_DCBEN		0x00010000  //!< DCB Feature Enable
#define ETH_MACHWF1R_ADDR64		0x0000C000  //!< Address width
#define ETH_MACHWF1R_ADVTHWORD	0x00002000  //!< IEEE 1588 High Word Register Enable
#define ETH_MACHWF1R_PTOEN		0x00001000  //!< PTP Offload Enable
#define ETH_MACHWF1R_OSTEN		0x00000800  //!< One-Step Timestamping Enable
#define ETH_MACHWF1R_TXFIFOSIZE	0x000007C0  //!< MTL Transmit FIFO Size
#define ETH_MACHWF1R_RXFIFOSIZE	0x0000001F  //!< MTL Receive FIFO Size
/** @} */

/** @name  ETH_MACHWF2R:    (eth Offset: 0x0124) HW feature 2 register		  */
/** @{ */
#define ETH_MACHWF2R_AUXSNAPNUM	0x70000000  //!< Number of Auxiliary Snapshot Inputs
#define ETH_MACHWF2R_PPSOUTNUM	0x07000000  //!< Number of PPS Outputs
#define ETH_MACHWF2R_TXCHCNT	0x003C0000  //!< Number of DMA Transmit Channels
#define ETH_MACHWF2R_RXCHCNT	0x0000F000  //!< Number of DMA Receive Channels
#define ETH_MACHWF2R_TXQCNT		0x000003C0  //!< Number of MTL Transmit Queues
#define ETH_MACHWF2R_RXQCNT		0x0000000F  //!< Number of MTL Receive Queues
/** @} */

/** @name  ETH_MACMDIOAR:   (eth Offset: 0x0200) MDIO address register		  */
/** @{ */
#define ETH_MACMDIOAR_PSE		0x08000000  //!< Preamble Suppression Enable
#define ETH_MACMDIOAR_BTB		0x04000000  //!< Back to Back transactions
#define ETH_MACMDIOAR_PA		0x03E00000  //!< Physical Layer Address
#define ETH_MACMDIOAR_RDA		0x001F0000  //!< Register/Device Address
#define ETH_MACMDIOAR_NTC		0x00007000  //!< Number of Training Clocks
#define ETH_MACMDIOAR_CR		0x00000F00  //!< CSR Clock Range
#define ETH_MACMDIOAR_SKAP		0x00000010  //!< Skip Address Packet
#define ETH_MACMDIOAR_GOC		0x0000000C  //!< MII Operation Command
#define ETH_MACMDIOAR_C45E		0x00000002  //!< Clause 45 PHY Enable
#define ETH_MACMDIOAR_MB		0x00000001  //!< MII Busy
/** @} */

/** @name  ETH_MACMDIODR:   (eth Offset: 0x0204) MDIO data register           */
/** @{ */
#define ETH_MACMDIODR_RA		0xFFFF0000  //!< Register Address
#define ETH_MACMDIODR_MD		0x0000FFFF  //!< MII Data
/** @} */

/** @name  ETH_MACARPAR:    (eth Offset: 0x0210) ARP address register         */
/** @{ */
#define ETH_MACARPAR_ARPPA		0xFFFFFFFF  //!< ARP Protocol Address
/** @} */

/** @name  ETH_MACA0HR:     (eth Offset: 0x0300) Address 0 high register      */
/** @{ */
#define ETH_MACA0HR_AE			0x80000000  //!< Address Enable
#define ETH_MACA0HR_ADDRHI		0x0000FFFF  //!< MAC Address0[47:32]
/** @} */

/** @name  ETH_MACA0LR:     (eth Offset: 0x0304) Address 0 low register       */
/** @{ */
#define ETH_MACA0LR_ADDRLO		0xFFFFFFFF  //!< MAC Address x [31:0] (x = 0 to 3)
/** @} */

/** @name  ETH_MACA1HR:     (eth Offset: 0x0308) Address 1 high register      */
/** @{ */
#define ETH_MACA1HR_AE			0x80000000  //!< Address Enable
#define ETH_MACA1HR_SA			0x40000000  //!< Source Address
#define ETH_MACA1HR_MBC			0x3F000000  //!< Mask Byte Control
#define ETH_MACA1HR_ADDRHI		0x0000FFFF  //!< MAC Address1 [47:32]
/** @} */

/** @name  ETH_MACA1LR:     (eth Offset: 0x030C) Address 1 low register       */
/** @{ */
#define ETH_MACA1LR_ADDRLO		0xFFFFFFFF  //!< MAC Address x [31:0] (x = 0 to 3)
/** @} */

/** @name  ETH_MACA2HR:     (eth Offset: 0x0310) Address 1 high register      */
/** @{ */
#define ETH_MACA2HR_AE			0x80000000  //!< Address Enable
#define ETH_MACA2HR_SA			0x40000000  //!< Source Address
#define ETH_MACA2HR_MBC			0x3F000000  //!< Mask Byte Control
#define ETH_MACA2HR_ADDRHI		0x0000FFFF  //!< MAC Address2 [47:32]
/** @} */

/** @name  ETH_MACA2LR:     (eth Offset: 0x0314) Address 2 low register       */
/** @{ */
#define ETH_MACA2LR_ADDRLO		0xFFFFFFFF  //!< MAC Address x [31:0] (x = 0 to 3)
/** @} */

/** @name  ETH_MACA3HR:     (eth Offset: 0x0318) Address 3 high register      */
/** @{ */
#define ETH_MACA3HR_AE			0x80000000  //!< Address Enable
#define ETH_MACA3HR_SA			0x40000000  //!< Source Address
#define ETH_MACA3HR_MBC			0x3F000000  //!< Mask Byte Control
#define ETH_MACA3HR_ADDRHI		0x0000FFFF  //!< MAC Address3 [47:32]
/** @} */

/** @name  ETH_MACA3LR:     (eth Offset: 0x031C) Address 3 low register       */
/** @{ */
#define ETH_MACA3LR_ADDRLO		0xFFFFFFFF  //!< MAC Address x [31:0] (x = 0 to 3)
/** @} */

/** @name  ETH_MMCCR:       (eth Offset: 0x0700) MMC control register         */
/** @{ */
#define ETH_MMCCR_UCDBC			0x00000100  //!< Update MMC Counters for Dropped Broadcast Packets
#define ETH_MMCCR_CNTPRSTLVL	0x00000020  //!< Full-Half Preset
#define ETH_MMCCR_CNTPRST		0x00000010  //!< Counters Preset
#define ETH_MMCCR_CNTFREEZ		0x00000008  //!< MMC Counter Freeze
#define ETH_MMCCR_RSTONRD		0x00000004  //!< Reset on Read
#define ETH_MMCCR_CNTSTOPRO		0x00000002  //!< Counter Stop Rollover
#define ETH_MMCCR_CNTRST		0x00000001  //!< Counters Reset
/** @} */

/** @name  ETH_MMCRIR:      (eth Offset: 0x0704) MMC Rx interrupt register    */
/** @{ */
#define ETH_MMCRIR_RXLPITRCIS	0x08000000  //!< MMC Receive LPI transition counter interrupt status
#define ETH_MMCRIR_RXLPIUSCIS	0x04000000  //!< MMC Receive LPI microsecond counter interrupt status
#define ETH_MMCRIR_RXUCGPIS		0x00020000  //!< MMC Receive Unicast Good Packet Counter Interrupt Status
#define ETH_MMCRIR_RXALGNERPIS	0x00000040  //!< MMC Receive Alignment Error Packet Counter Interrupt Status
#define ETH_MMCRIR_RXCRCERPIS	0x00000020  //!< MMC Receive CRC Error Packet Counter Interrupt Status
/** @} */

/** @name  ETH_MMCTIR:      (eth Offset: 0x0708) MMC Tx interrupt register    */
/** @{ */
#define ETH_MMCTIR_TXLPITRCIS	0x08000000  //!< MMC Transmit LPI transition counter interrupt status
#define ETH_MMCTIR_TXLPIUSCIS	0x04000000  //!< MMC Transmit LPI microsecond counter interrupt status
#define ETH_MMCTIR_TXGPKTIS		0x00200000  //!< MMC Transmit Good Packet Counter Interrupt Status
#define ETH_MMCTIR_TXMCOLGPIS	0x00008000  //!< MMC Transmit Multiple Collision Good Packet Counter Interrupt Status
#define ETH_MMCTIR_TXSCOLGPIS	0x00004000  //!< MMC Transmit Single Collision Good Packet Counter Interrupt Status
/** @} */

/** @name  ETH_MMCRIMR:     (eth Offset: 0x070C) MMC Rx interrupt mask register  */
/** @{ */
#define ETH_MMCRIMR_RXLPITRCIM	0x08000000  //!< MMC Receive LPI transition counter interrupt Mask
#define ETH_MMCRIMR_RXLPIUSCIM	0x04000000  //!< MMC Receive LPI microsecond counter interrupt Mask
#define ETH_MMCRIMR_RXUCGPIM	0x00020000  //!< MMC Receive Unicast Good Packet Counter Interrupt Mask
#define ETH_MMCRIMR_RXALGNERPIM	0x00000040  //!< MMC Receive Alignment Error Packet Counter Interrupt Mask
#define ETH_MMCRIMR_RXCRCERPIM	0x00000020  //!< MMC Receive CRC Error Packet Counter Interrupt Mask
/** @} */

/** @name  ETH_MMCTIMR:     (eth Offset: 0x0710) MMC Tx interrupt mask register  */
/** @{ */
#define ETH_MMCTIMR_TXLPITRCIM	0x08000000  //!< MMC Transmit LPI transition counter interrupt Mask
#define ETH_MMCTIMR_TXLPIUSCIM	0x04000000  //!< MMC Transmit LPI microsecond counter interrupt Mask
#define ETH_MMCTIMR_TXGPKTIM	0x00200000  //!< MMC Transmit Good Packet Counter Interrupt Mask
#define ETH_MMCTIMR_TXMCOLGPIM	0x00008000  //!< MMC Transmit Multiple Collision Good Packet Counter Interrupt Mask
#define ETH_MMCTIMR_TXSCOLGPIM	0x00004000  //!< MMC Transmit Single Collision Good Packet Counter Interrupt Mask
/** @} */

/** @name  ETH_MMCTSCGPR:   (eth Offset: 0x074C) Tx single collision good packets register  */
/** @{ */
#define ETH_MMCTSCGPR_TXSNGLCOLG 0xFFFFFFFF  //!< Tx Single Collision Good Packets
/** @} */

/** @name  ETH_MMCTMCGPR:   (eth Offset: 0x0750) Tx multiple collision good packets register  */
/** @{ */
#define ETH_MMCTMCGPR_TXMULTCOLG 0xFFFFFFFF  //!< Tx Multiple Collision Good Packets
/** @} */

/** @name  ETH_MMCTPCGR:    (eth Offset: 0x0768) Tx packet count good register  */
/** @{ */
#define ETH_MMCTPCGR_TXPKTG		0x00000000  //!< Tx Packet Count Good
/** @} */

/** @name  ETH_MMCRCRCEPR:  (eth Offset: 0x0794) Rx CRC error packets register  */
/** @{ */
#define ETH_MMCRCRCEPR_RXCRCERR	0xFFFFFFFF  //!< Rx CRC Error Packets
/** @} */

/** @name  ETH_MMCRAEPR:    (eth Offset: 0x0798) Rx alignment error packets register  */
/** @{ */
#define ETH_MMCRAEPR_RXALGNERR	0xFFFFFFFF  //!< Rx Alignment Error Packets
/** @} */

/** @name  ETH_MMCRUPGR:    (eth Offset: 0x07C4) Rx unicast packets good register  */
/** @{ */
#define ETH_MMCRUPGR_RXUCASTG	0xFFFFFFFF  //!< Rx Unicast Packets Good
/** @} */

/** @name  ETH_MMCTLPIMSTR: (eth Offset: 0x07EC) Tx LPI microsecond timer register  */
/** @{ */
#define ETH_MMCTLPIMSTR_TXLPIUSC 0xFFFFFFFF  //!< Tx LPI Microseconds Counter
/** @} */

/** @name  ETH_MMCTLPITCR:  (eth Offset: 0x07F0) Tx LPI transition counter register  */
/** @{ */
#define ETH_MMCTLPITCR_TXLPITRC	0xFFFFFFFF  //!< Tx LPI Transition counter
/** @} */

/** @name  ETH_MMCRLPIMSTR: (eth Offset: 0x07F4) Rx LPI microsecond counter register  */
/** @{ */
#define ETH_MMCRLPIMSTR_RXLPIUSC 0xFFFFFFFF  //!< Rx LPI Microseconds Counter
/** @} */

/** @name  ETH_MMCRLPITCR:  (eth Offset: 0x07F8) Rx LPI transition counter register  */
/** @{ */
#define ETH_MMCRLPITCR_RXLPITRC	0xFFFFFFFF  //!< Rx LPI Transition counter
/** @} */

/** @name  ETH_MACL3L4C0R:  (eth Offset: 0x0900) L3 and L4 control 0 register  */
/** @{ */
#define ETH_MACL3L4C0R_L4DPIM0	0x00200000  //!< Layer 4 Destination Port Inverse Match Enable
#define ETH_MACL3L4C0R_L4DPM0	0x00100000  //!< Layer 4 Destination Port Match Enable
#define ETH_MACL3L4C0R_L4SPIM0	0x00080000  //!< Layer 4 Source Port Inverse Match Enable
#define ETH_MACL3L4C0R_L4SPM0	0x00040000  //!< Layer 4 Source Port Match Enable
#define ETH_MACL3L4C0R_L4PEN0	0x00010000  //!< Layer 4 Protocol Enable
#define ETH_MACL3L4C0R_L3HDBM0	0x0000F800  //!< Layer 3 IP DA Higher Bits Match
#define ETH_MACL3L4C0R_L3HSBM0	0x000007C0  //!< Layer 3 IP SA Higher Bits Match
#define ETH_MACL3L4C0R_L3DAIM0	0x00000020  //!< Layer 3 IP DA Inverse Match Enable
#define ETH_MACL3L4C0R_L3DAM0	0x00000010  //!< Layer 3 IP DA Match Enable
#define ETH_MACL3L4C0R_L3SAIM0	0x00000008  //!< Layer 3 IP SA Inverse Match Enable
#define ETH_MACL3L4C0R_L3SAM0	0x00000004  //!< Layer 3 IP SA Match Enable
#define ETH_MACL3L4C0R_L3PEN0	0x00000001  //!< Layer 3 Protocol Enable
/** @} */

/** @name  ETH_MACL4A0R:    (eth Offset: 0x0904) Layer 4 address filter 0 register  */
/** @{ */
#define ETH_MACL4A0R_L4DP0		0xFFFF0000  //!< Layer 4 Destination Port Number Field
#define ETH_MACL4A0R_L4SP0		0x0000FFFF  //!< Layer 4 Source Port Number Field
/** @} */

/** @name  ETH_MACL3A0R0R:  (eth Offset: 0x0910) Layer 3 address 0 filter 0 register  */
/** @{ */
#define ETH_MACL3A0R0R_L3A00	0xFFFFFFFF  //!< Layer 3 Address 0 Field
/** @} */

/** @name  ETH_MACL3A1R0R:  (eth Offset: 0x0914) Layer 3 address 1 filter 0 register  */
/** @{ */
#define ETH_MACL3A1R0R_L3A10	0xFFFFFFFF  //!< Layer 3 Address 1 Field
/** @} */

/** @name  ETH_MACL3A2R0R:  (eth Offset: 0x0918) Layer 3 Address 2 filter 0 register  */
/** @{ */
#define ETH_MACL3A2R0R_L3A20	0xFFFFFFFF  //!< Layer 3 Address 2 Field
/** @} */

/** @name  ETH_MACL3A3R0R:  (eth Offset: 0x091C) Layer 3 Address 3 filter 0 register  */
/** @{ */
#define ETH_MACL3A3R0R_L3A30	0xFFFFFFFF  //!< Layer 3 Address 3 Field
/** @} */

/** @name  ETH_MACL3L4C1R:  (eth Offset: 0x0930) L3 and L4 control 1 register  */
/** @{ */
#define ETH_MACL3L4C1R_L4DPIM1	0x00200000  //!< Layer 4 Destination Port Inverse Match Enable
#define ETH_MACL3L4C1R_L4DPM1	0x00100000  //!< Layer 4 Destination Port Match Enable
#define ETH_MACL3L4C1R_L4SPIM1	0x00080000  //!< Layer 4 Source Port Inverse Match Enable
#define ETH_MACL3L4C1R_L4SPM1	0x00040000  //!< Layer 4 Source Port Match Enable
#define ETH_MACL3L4C1R_L4PEN1	0x00010000  //!< Layer 4 Protocol Enable
#define ETH_MACL3L4C1R_L3HDBM1	0x0000F800  //!< Layer 3 IP DA Higher Bits Match
#define ETH_MACL3L4C1R_L3HSBM1	0x000007C0  //!< Layer 3 IP SA Higher Bits Match
#define ETH_MACL3L4C1R_L3DAIM1	0x00000020  //!< Layer 3 IP DA Inverse Match Enable
#define ETH_MACL3L4C1R_L3DAM1	0x00000010  //!< Layer 3 IP DA Match Enable
#define ETH_MACL3L4C1R_L3SAIM1	0x00000008  //!< Layer 3 IP SA Inverse Match Enable
#define ETH_MACL3L4C1R_L3SAM1	0x00000004  //!< Layer 3 IP SA Match Enable
#define ETH_MACL3L4C1R_L3PEN1	0x00000001  //!< Layer 3 Protocol Enable
/** @} */

/** @name  ETH_MACL4A1R:    (eth Offset: 0x0934) Layer 4 address filter 1 register  */
/** @{ */
#define ETH_MACL4A1R_L4DP1		0xFFFF0000  //!< Layer 4 Destination Port Number Field
#define ETH_MACL4A1R_L4SP1		0x0000FFFF  //!< Layer 4 Source Port Number Field
/** @} */

/** @name  ETH_MACL3A0R1R:  (eth Offset: 0x0940) Layer 3 address 0 filter 1 register  */
/** @{ */
#define ETH_MACL3A0R1R_L3A01	0xFFFFFFFF  //!< Layer 3 Address 0 Field
/** @} */

/** @name  ETH_MACL3A1R1R:  (eth Offset: 0x0944) Layer 3 address 1 filter 1 register  */
/** @{ */
#define ETH_MACL3A1R1R_L3A11	0xFFFFFFFF  //!< Layer 3 Address 1 Field
/** @} */

/** @name  ETH_MACL3A2R1R:  (eth Offset: 0x0948) Layer 3 address 2 filter 1 register  */
/** @{ */
#define ETH_MACL3A2R1R_L3A21	0xFFFFFFFF  //!< Layer 3 Address 2 Field
/** @} */

/** @name  ETH_MACL3A3R1R:  (eth Offset: 0x094C) Layer 3 address 3 filter 1 register  */
/** @{ */
#define ETH_MACL3A3R1R_L3A31	0xFFFFFFFF  //!< Layer 3 Address 3 Field
/** @} */

/** @name  ETH_MACTSCR:     (eth Offset: 0x0B00) Timestamp control Register  */
/** @{ */
#define ETH_MACTSCR_TXTSSTSM	0x01000000  //!< Transmit Timestamp Status Mode
#define ETH_MACTSCR_CSC			0x00080000  //!< Enable checksum correction during OST for PTP over UDP/IPv4 packets
#define ETH_MACTSCR_TSENMACADDR	0x00040000  //!< Enable MAC Address for PTP Packet Filtering
#define ETH_MACTSCR_SNAPTYPSEL	0x00030000  //!< Select PTP packets for Taking Snapshots
#define ETH_MACTSCR_TSMSTRENA	0x00008000  //!< Enable Snapshot for Messages Relevant to Master
#define ETH_MACTSCR_TSEVNTENA	0x00004000  //!< Enable Timestamp Snapshot for Event Messages
#define ETH_MACTSCR_TSIPV4ENA	0x00002000  //!< Enable Processing of PTP Packets Sent over IPv4-UDP
#define ETH_MACTSCR_TSIPV6ENA	0x00001000  //!< Enable Processing of PTP Packets Sent over IPv6-UDP
#define ETH_MACTSCR_TSIPENA		0x00000800  //!< Enable Processing of PTP over Ethernet Packets
#define ETH_MACTSCR_TSVER2ENA	0x00000400  //!< Enable PTP Packet Processing for Version 2 Format
#define ETH_MACTSCR_TSCTRLSSR	0x00000200  //!< Timestamp Digital or Binary Rollover Control
#define ETH_MACTSCR_TSENALL		0x00000100  //!< Enable Timestamp for All Packets
#define ETH_MACTSCR_TSADDREG	0x00000020  //!< Update Addend Register
#define ETH_MACTSCR_TSUPDT		0x00000008  //!< Update Timestamp
#define ETH_MACTSCR_TSINIT		0x00000004  //!< Initialize Timestamp
#define ETH_MACTSCR_TSCFUPDT	0x00000002  //!< Fine or Coarse Timestamp Update
#define ETH_MACTSCR_TSENA		0x00000001  //!< Enable Timestamp
/** @} */

/** @name  ETH_MACSSIR:     (eth Offset: 0x0B04) Sub-second increment register  */
/** @{ */
#define ETH_MACSSIR_SSINC		0x00FF0000  //!< Sub-second Increment Value
#define ETH_MACSSIR_SNSINC		0x0000FF00  //!< Sub-nanosecond Increment Value
/** @} */

/** @name  ETH_MACSTSR:     (eth Offset: 0x0B08) System time seconds register  */
/** @{ */
#define ETH_MACSTSR_TSS			0xFFFFFFFF  //!< Timestamp Second
/** @} */

/** @name  ETH_MACSTNR:     (eth Offset: 0x0B0C) System time nanoseconds register  */
/** @{ */
#define ETH_MACSTNR_TSSS		0x7FFFFFFF  //!< Timestamp Sub-seconds
/** @} */

/** @name  ETH_MACSTSUR:    (eth Offset: 0x0B10) System time seconds update register  */
/** @{ */
#define ETH_MACSTSUR_TSS		0xFFFFFFFF  //!< Timestamp Seconds
/** @} */

/** @name  ETH_MACSTNUR:    (eth Offset: 0x0B14) System time nanoseconds update register  */
/** @{ */
#define ETH_MACSTNUR_ADDSUB		0x80000000  //!< Add or Subtract Time
#define ETH_MACSTNUR_TSSS		0x7FFFFFFF  //!< Timestamp Sub-seconds
/** @} */

/** @name  ETH_MACTSAR:     (eth Offset: 0x0B18) Timestamp addend register	  */
/** @{ */
#define ETH_MACTSAR_TSAR		0xFFFFFFFF  //!< Timestamp Addend Register
/** @} */

/** @name  ETH_MACTSSR:     (eth Offset: 0x0B20) Timestamp status register    */
/** @{ */
#define ETH_MACTSSR_ATSNS		0x3E000000  //!< Number of Auxiliary Timestamp Snapshots
#define ETH_MACTSSR_ATSSTM		0x01000000  //!< Auxiliary Timestamp Snapshot Trigger Missed
#define ETH_MACTSSR_ATSSTN		0x000F0000  //!< Auxiliary Timestamp Snapshot Trigger Identifier
#define ETH_MACTSSR_TXTSSIS		0x00008000  //!< Tx Timestamp Status Interrupt Status
#define ETH_MACTSSR_TSTRGTERR0	0x00000008  //!< Timestamp Target Time Error
#define ETH_MACTSSR_AUXTSTRIG	0x00000004  //!< Auxiliary Timestamp Trigger Snapshot
#define ETH_MACTSSR_TSTARGT0	0x00000002  //!< Timestamp Target Time Reached
#define ETH_MACTSSR_TSSOVF		0x00000001  //!< Timestamp Seconds Overflow
/** @} */

/** @name  ETH_MACTTSSNR:   (eth Offset: 0x0B30) Tx timestamp status nanoseconds register  */
/** @{ */
#define ETH_MACTTSSNR_TXTSSMIS	0x80000000  //!< Transmit Timestamp Status Missed
#define ETH_MACTTSSNR_TXTSSLO	0x7FFFFFFF  //!< Transmit Timestamp Status Low
/** @} */

/** @name  ETH_MACTTSSSR:   (eth Offset: 0x0B34) Tx timestamp status seconds register  */
/** @{ */
#define ETH_MACTTSSSR_TXTSSHI	0xFFFFFFFF  //!< Transmit Timestamp Status High
/** @} */

/** @name  ETH_MACACR:      (eth Offset: 0x0B40) Auxiliary control register	  */
/** @{ */
#define ETH_MACACR_ATSEN3		0x00000080  //!< Auxiliary Snapshot 3 Enable
#define ETH_MACACR_ATSEN2		0x00000040  //!< Auxiliary Snapshot 2 Enable
#define ETH_MACACR_ATSEN1		0x00000020  //!< Auxiliary Snapshot 1 Enable
#define ETH_MACACR_ATSEN0		0x00000010  //!< Auxiliary Snapshot 0 Enable
#define ETH_MACACR_ATSFC		0x00000001  //!< Auxiliary Snapshot FIFO Clear
/** @} */

/** @name  ETH_MACATSNR:    (eth Offset: 0x0B48) Auxiliary timestamp nanoseconds register  */
/** @{ */
#define ETH_MACATSNR_AUXTSLO	0x7FFFFFFF  //!< Auxiliary Timestamp
/** @} */

/** @name  ETH_MACATSSR:    (eth Offset: 0x0B4C) Auxiliary timestamp seconds register  */
/** @{ */
#define ETH_MACATSSR_AUXTSHI	0xFFFFFFFF  //!< Auxiliary Timestamp
/** @} */

/** @name  ETH_MACTSIACR:   (eth Offset: 0x0B50) Timestamp Ingress asymmetric correction register  */
/** @{ */
#define ETH_MACTSIACR_OSTIAC	0xFFFFFFFF  //!< One-Step Timestamp Ingress Asymmetry Correction
/** @} */

/** @name  ETH_MACTSEACR:   (eth Offset: 0x0B54) Timestamp Egress asymmetric correction register  */
/** @{ */
#define ETH_MACTSEACR_OSTEAC	0xFFFFFFFF  //!< One-Step Timestamp Egress Asymmetry Correction
/** @} */

/** @name  ETH_MACTSICNR:   (eth Offset: 0x0B58) Timestamp Ingress correction nanosecond register  */
/** @{ */
#define ETH_MACTSICNR_TSIC		0xFFFFFFFF  //!< Timestamp Ingress Correction
/** @} */

/** @name  ETH_MACTSECNR:   (eth Offset: 0x0B5C) Timestamp Egress correction nanosecond register  */
/** @{ */
#define ETH_MACTSECNR_TSEC		0x00000000  //!< Timestamp Egress Correction
/** @} */

/** @name  ETH_MACPPSCR:    (eth Offset: 0x0B70) PPS control register [alternate]  */
/** @{ */
#define ETH_MACPPSCR_TRGTMODSEL0 0x00000060 //!< Target Time Register Mode for PPS Output
#define ETH_MACPPSCR_PPSEN0		0x00000010  //!< Flexible PPS Output Mode Enable
#define ETH_MACPPSCR_PPSCTRL	0x0000000F  //!< PPS Output Frequency Control
/** @} */

/** @name  ETH_MACPPSTTSR:  (eth Offset: 0x0B80) PPS target time seconds register  */
/** @{ */
#define ETH_MACPPSTTSR_TSTRH0	0xFFFFFFFF  //!< PPS Target Time Seconds Register
/** @} */

/** @name  ETH_MACPPSTTNR:  (eth Offset: 0x0B84) PPS target time nanoseconds register  */
/** @{ */
#define ETH_MACPPSTTNR_TRGTBUSY0 0x80000000 //!< PPS Target Time Register Busy
#define ETH_MACPPSTTNR_TTSL0 	 0x7FFFFFFF //!< Target Time Low for PPS Registe
/** @} */

/** @name  ETH_MACPPSIR:    (eth Offset: 0x0B88) PPS interval register		  */
/** @{ */
#define ETH_MACPPSIR_PPSINT0	0xFFFFFFFF  //!< PPS Output Signal Interval
/** @} */

/** @name  ETH_MACPPSWR:    (eth Offset: 0x0B8C) PPS width register           */
/** @{ */
#define ETH_MACPPSWR_PPSWIDTH0	0xFFFFFFFF  //!< PPS Output Signal Width
/** @} */

/** @name  ETH_MACPOCR:     (eth Offset: 0x0BC0) PTP Offload control register  */
/** @{ */
#define ETH_MACPOCR_DN			0x0000FF00  //!< Domain Number
#define ETH_MACPOCR_DRRDIS		0x00000040  //!< Disable PTO Delay Request/Response response generation
#define ETH_MACPOCR_APDREQTRIG	0x00000020  //!< Automatic PTP Pdelay_Req message Trigger
#define ETH_MACPOCR_ASYNCTRIG	0x00000010  //!< Automatic PTP SYNC message Trigger
#define ETH_MACPOCR_APDREQEN	0x00000004  //!< Automatic PTP Pdelay_Req message Enable
#define ETH_MACPOCR_ASYNCEN		0x00000002  //!< Automatic PTP SYNC message Enable
#define ETH_MACPOCR_PTOEN		0x00000001  //!< PTP Offload Enable
/** @} */

/** @name  ETH_MACSPI0R:    (eth Offset: 0x0BC4) PTP Source Port Identity 0 Register  */
/** @{ */
#define ETH_ETH_MACSPI0R_SPI0	0xFFFFFFFF  //!< Source Port Identity 0
/** @} */

/** @name  ETH_MACSPI1R:    (eth Offset: 0x0BC8) PTP Source port identity 1 register  */
/** @{ */
#define ETH_MACSPI1R_SPI1		0xFFFFFFFF  //!< Source Port Identity 1
/** @} */

/** @name  ETH_MACSPI2R:    (eth Offset: 0x0BCC) PTP Source port identity 2 register  */
/** @{ */
#define ETH_MACSPI2R_SPI2		0x0000FFFF  //!< Source Port Identity 2
/** @} */

/** @name  ETH_MACLMIR:     (eth Offset: 0x0BD0) Log message interval register  */
/** @{ */
#define ETH_MACLMIR_LMPDRI		0xFF000000  //!< Log Min Pdelay_Req Interval
#define ETH_MACLMIR_DRSYNCR		0x00000700  //!< Delay_Req to SYNC Ratio
#define ETH_MACLMIR_LSI			0x000000FF  //!< Log Sync Interval
/** @} */

//--------------------- MTL Registers   ---------------------------------------/
/** @name  ETH_MTLOMR:      (eth Offset: 0x0C00) Operating mode Register	  */
/** @{ */
#define ETH_MTLOMR_CNTCLR		0x00000200  //!< Counters Reset
#define ETH_MTLOMR_CNTPRST		0x00000100  //!< Counters Preset
#define ETH_MTLOMR_DTXSTS		0x00000002  //!< Drop Transmit Status
/** @} */

/** @name  ETH_MTLISR:      (eth Offset: 0x0C20) Interrupt status Register    */
/** @{ */
#define ETH_MTLISR_Q0IS			0x00000001  //!< Queue interrupt status
/** @} */

/** @name  ETH_MTLTQOMR:    (eth Offset: 0x0D00) Tx queue operating mode Register  */
/** @{ */
#define ETH_MTLTQOMR_TQS		0x01FF0000  //!< Transmit Queue Size
#define ETH_MTLTQOMR_TTC		0x00000070  //!< Transmit Threshold Control
#define ETH_MTLTQOMR_TXQEN		0x0000000C  //!< Transmit Queue Enable
#define ETH_MTLTQOMR_TSF		0x00000002  //!< Transmit Store and Forward
#define ETH_MTLTQOMR_FTQ		0x00000001  //!< Flush Transmit Queue
/** @} */

/** @name  ETH_MTLTQUR:     (eth Offset: 0x0D04) Tx queue underflow register  */
/** @{ */
#define ETH_MTLTQUR_UFCNTOVF	0x00000800  //!< Overflow Bit for Underflow Packet Counter
#define ETH_MTLTQUR_UFFRMCNT	0x000007FF  //!< Underflow Packet Counter
/** @} */

/** @name  ETH_MTLTQDR:     (eth Offset: 0x0D08) Tx queue debug Register	  */
/** @{ */
#define ETH_MTLTQDR_STXSTSF		0x00700000  //!< Number of Status Words in Tx Status FIFO of Queue
#define ETH_MTLTQDR_PTXQ		0x00070000  //!< Number of Packets in the Transmit Queue
#define ETH_MTLTQDR_TXSTSFSTS	0x00000020  //!< MTL Tx Status FIFO Full Status
#define ETH_MTLTQDR_TXQSTS		0x00000010  //!< MTL Tx Queue Not Empty Status
#define ETH_MTLTQDR_TWCSTS		0x00000008  //!< MTL Tx Queue Write Controller Status
#define ETH_MTLTQDR_TRCSTS		0x00000006  //!< MTL Tx Queue Read Controller Status
#define ETH_MTLTQDR_TXQPAUSED	0x00000001  //!< Transmit Queue in Pause
/** @} */

/** @name  ETH_MTLQICSR:    (eth Offset: 0x0D2C) Queue interrupt control status Register  */
/** @{ */
#define ETH_MTLQICSR_RXOIE		0x01000000  //!< Receive Queue Overflow Interrupt Enable
#define ETH_MTLQICSR_RXOVFIS	0x00010000  //!< Receive Queue Overflow Interrupt Status
#define ETH_MTLQICSR_TXUIE		0x00000100  //!< Transmit Queue Underflow Interrupt Enable
#define ETH_MTLQICSR_TXUNFIS	0x00000001  //!< Transmit Queue Underflow Interrupt Status
/** @} */

/** @name  ETH_MTLRQOMR:    (eth Offset: 0x0D30) Rx queue operating mode register  */
/** @{ */
#define ETH_MTLRQOMR_RQS		0x00700000  //!< Receive Queue Size
#define ETH_MTLRQOMR_RFD		0x0001C000  //!< Threshold for Deactivating Flow Control
#define ETH_MTLRQOMR_RFA		0x00000700  //!< Threshold for Activating Flow Control (in half-duplex and full-duplex
#define ETH_MTLRQOMR_EHFC		0x00000080  //!< Enable Hardware Flow Control
#define ETH_MTLRQOMR_DIS_TCP_EF	0x00000040  //!< Disable Dropping of TCP/IP Checksum Error Packets
#define ETH_MTLRQOMR_RSF		0x00000020  //!< Receive Queue Store and Forward
#define ETH_MTLRQOMR_FEP		0x00000010  //!< Forward Error Packets
#define ETH_MTLRQOMR_FUP		0x00000008  //!< Forward Undersized Good Packets
#define ETH_MTLRQOMR_RTC		0x00000003  //!< Receive Queue Threshold Control
/** @} */

/** @name  ETH_MTLRQMPOCR:  (eth Offset: 0x0D34) Rx queue missed packet and overflow counter register  */
/** @{ */
#define ETH_MTLRQMPOCR_MISCNTOVF 0x08000000 //!< Missed Packet Counter Overflow Bit
#define ETH_MTLRQMPOCR_MISPKTCNT 0x07FF0000 //!< Missed Packet Counter
#define ETH_MTLRQMPOCR_OVFCNTOVF 0x00000800 //!< Overflow Counter Overflow Bit
#define ETH_MTLRQMPOCR_OVFPKTCNT 0x000007FF //!< Overflow Packet Counter
/** @} */

/** @name  ETH_MTLRQDR:     (eth Offset: 0x0D38) Rx queue debug register	  */
/** @{ */
#define ETH_MTLRQDR_PRXQ		0x3FFF0000  //!< Number of Packets in Receive Queue
#define ETH_MTLRQDR_RXQSTS		0x00000030  //!< MTL Rx Queue Fill-Level Status
#define ETH_MTLRQDR_RRCSTS		0x00000006  //!< MTL Rx Queue Read Controller State
#define ETH_MTLRQDR_RWCSTS		0x00000001  //!< MTL Rx Queue Write Controller Active Status
/** @} */

//--------------------- DMA Registers   ---------------------------------------/
/** @name  ETH_DMAMR:       (eth Offset: 0x1000) DMA mode register			  */
/** @{ */
#define ETH_DMAMR_INTM			0x00030000  //!< Interrupt Mode
#define ETH_DMAMR_PR			0x00007000  //!< Priority ratio
#define ETH_DMAMR_TXPR			0x00000800  //!< Transmit priority
#define ETH_DMAMR_DA			0x00000002  //!< DMA Tx or Rx Arbitration Scheme
#define ETH_DMAMR_SWR			0x00000001  //!< Software Reset
/** @} */

/** @name  ETH_DMASBMR:     (eth Offset: 0x1004) System bus mode register     */
/** @{ */
#define ETH_DMASBMR_RB			0x00008000  //!< Rebuild INCRx Burst
#define ETH_DMASBMR_MB			0x00004000  //!< Mixed Burst
#define ETH_DMASBMR_AAL			0x00001000  //!< Address-Aligned Beats
#define ETH_DMASBMR_FB			0x00000001  //!< Fixed Burst Length
/** @} */

/** @name  ETH_DMAISR:      (eth Offset: 0x1008) Interrupt status register    */
/** @{ */
#define ETH_DMAISR_MACIS		0x00020000  //!< MAC Interrupt Status
#define ETH_DMAISR_MTLIS		0x00010000  //!< MTL Interrupt Status
#define ETH_DMAISR_DC0IS		0x00000001  //!< DMA Channel Interrupt Status
/** @} */

/** @name  ETH_DMADSR:      (eth Offset: 0x100C) Debug status register        */
/** @{ */
#define ETH_DMADSR_TPS0			0x0000F000  //!< DMA Channel Transmit Process State
#define ETH_DMADSR_RPS0			0x00000F00  //!< DMA Channel Receive Process State
#define ETH_DMADSR_AXWHSTS		0x00000001  //!< AHB Master Write Channel
/** @} */

/** @name  ETH_DMACCR:      (eth Offset: 0x1100) Channel control register     */
/** @{ */
#define ETH_DMACCR_DSL			0x001C0000  //!< Descriptor Skip Length
#define ETH_DMACCR_PBLX8		0x00010000  //!< 8xPBL mode
#define ETH_DMACCR_MSS			0x00003FFF  //!< Maximum Segment Size
/** @} */

/** @name  ETH_DMACTCR:     (eth Offset: 0x1104) Channel transmit control register  */
/** @{ */
#define ETH_DMACTCR_TXPBL		0x003F0000  //!< Transmit Programmable Burst Length
#define ETH_DMACTCR_TSE			0x00001000  //!< TCP Segmentation Enabled
#define ETH_DMACTCR_OSF			0x00000010  //!< Operate on Second Packet
#define ETH_DMACTCR_ST			0x00000001  //!< Start or Stop Transmission Command
/** @} */

/** @name  ETH_DMACRCR:     (eth Offset: 0x1108) Channel receive control register  */
/** @{ */
#define ETH_DMACRCR_RPF			0x80000000  //!< DMA Rx Channel Packet Flush
#define ETH_DMACRCR_RXPBL		0x003F0000  //!< Receive Programmable Burst Length
#define ETH_DMACRCR_RBSZ		0x00007FFE  //!< Receive Buffer size
#define ETH_DMACRCR_SR			0x00000001  //!< Start or Stop Receive
/** @} */

/** @name  ETH_DMACTDLAR:   (eth Offset: 0x1114) Channel Tx descriptor list address register  */
/** @{ */
#define ETH_DMACTDLAR_TDESLA	0xFFFFFFFC  //!< Start of Transmit List
/** @} */

/** @name  ETH_DMACRDLAR:   (eth Offset: 0x111C) Channel Rx descriptor list address register  */
/** @{ */
#define ETH_DMACRDLAR_RDESLA	0xFFFFFFFC  //!< Start of Receive List
/** @} */

/** @name  ETH_DMACTDTPR:   (eth Offset: 0x1120) Channel Tx descriptor tail pointer register  */
/** @{ */
#define ETH_DMACTDTPR_TDT		0xFFFFFFFC  //!< Transmit Descriptor Tail Pointer
/** @} */

/** @name  ETH_DMACRDTPR:   (eth Offset: 0x1128) Channel Rx descriptor tail pointer register  */
/** @{ */
#define ETH_DMACRDTPR_RDT		0xFFFFFFFC  //!< Receive Descriptor Tail Pointer
/** @} */

/** @name  ETH_DMACTDRLR:   (eth Offset: 0x112C) Channel Tx descriptor ring length register  */
/** @{ */
#define ETH_DMACTDRLR_TDRL		0x000003FF  //!< Transmit Descriptor Ring Length
/** @} */

/** @name  ETH_DMACRDRLR:   (eth Offset: 0x1130) Channel Rx descriptor ring length register  */
/** @{ */
#define ETH_DMACRDRLR_RDRL		0x000003FF  //!< Receive Descriptor Ring Length
/** @} */

/** @name  ETH_DMACIER:     (eth Offset: 0x1134) Channel interrupt enable register  */
/** @{ */
#define ETH_DMACIER_NIE			0x00008000  //!< Normal Interrupt Summary Enable
#define ETH_DMACIER_AIE			0x00004000  //!< Abnormal Interrupt Summary Enable
#define ETH_DMACIER_CDEE		0x00002000  //!< Context Descriptor Error Enable
#define ETH_DMACIER_FBEE		0x00001000  //!< Fatal Bus Error Enable
#define ETH_DMACIER_ERIE		0x00000800  //!< Early Receive Interrupt Enable
#define ETH_DMACIER_ETIE		0x00000400  //!< Early Transmit Interrupt Enable
#define ETH_DMACIER_RWTE		0x00000200  //!< Receive Watchdog Timeout Enable
#define ETH_DMACIER_RSE			0x00000100  //!< Receive Stopped Enable
#define ETH_DMACIER_RBUE		0x00000080  //!< Receive Buffer Unavailable Enable
#define ETH_DMACIER_RIE			0x00000040  //!< Receive Interrupt Enable
#define ETH_DMACIER_TBUE		0x00000004  //!< Transmit Buffer Unavailable Enable
#define ETH_DMACIER_TXSE		0x00000002  //!< Transmit Stopped Enable
#define ETH_DMACIER_TIE			0x00000001  //!< Transmit Interrupt Enable
/** @} */

/** @name  ETH_DMACRIWTR:   (eth Offset: 0x1138) Channel Rx interrupt watchdog timer register  */
/** @{ */
#define ETH_DMACRIWTR_RWT		0x000000FF  //!< Receive Interrupt Watchdog Timer Count
/** @} */

/** @name  ETH_DMACSFCSR:   (eth Offset: 0x113C) ---						  */
/** @{ */
#define ETH_DMACSFCSR_			0x00000000  //!<
/** @} */

/** @name  ETH_DMACCATDR:   (eth Offset: 0x1144) Channel current application transmit descriptor register  */
/** @{ */
#define ETH_DMACCATDR_CURTDESAPTR 0xFFFFFFFF //!< Application Transmit Descriptor Address Pointer
/** @} */

/** @name  ETH_DMACCARDR:   (eth Offset: 0x114C) Channel current application receive descriptor register  */
/** @{ */
#define ETH_DMACCARDR_CURRDESAPTR 0xFFFFFFFF //!< Application Receive Descriptor Address Pointer
/** @} */

/** @name  ETH_DMACCATBR:   (eth Offset: 0x1154) Channel current application transmit buffer register  */
/** @{ */
#define ETH_DMACCATBR_CURTBUFAPTR 0xFFFFFFFF //!< Application Transmit Buffer Address Pointer
/** @} */

/** @name  ETH_DMACCARBR:   (eth Offset: 0x115C) Channel current application receive buffer register  */
/** @{ */
#define ETH_DMACCARBR_CURRBUFAPTR 0xFFFFFFFF //!< Application Receive Buffer Address Pointer
/** @} */

/** @name  ETH_DMACSR:      (eth Offset: 0x1160) Channel status register	  */
/** @{ */
#define ETH_ETH_DMACSR_REB 		0x00380000  //!< Rx DMA Error Bits
#define ETH_ETH_DMACSR_TEB 		0x00070000  //!< Tx DMA Error Bits
#define ETH_ETH_DMACSR_NIS 		0x00008000  //!< Normal Interrupt Summary
#define ETH_ETH_DMACSR_AIS 		0x00004000  //!< Abnormal Interrupt Summary
#define ETH_ETH_DMACSR_CDE 		0x00002000  //!< Context Descriptor Error
#define ETH_ETH_DMACSR_FBE 		0x00001000  //!< Fatal Bus Error
#define ETH_ETH_DMACSR_ERI		0x00000800  //!< Early Receive Interrupt
#define ETH_ETH_DMACSR_ETI 		0x00000400  //!< Early Transmit Interrupt
#define ETH_ETH_DMACSR_RWT 		0x00000200  //!< Receive Watchdog Timeout
#define ETH_ETH_DMACSR_RPS 		0x00000100  //!< Receive Process Stopped
#define ETH_ETH_DMACSR_RBU 		0x00000080  //!< Receive Buffer Unavailable
#define ETH_ETH_DMACSR_RI 		0x00000040  //!< Receive Interrupt
#define ETH_ETH_DMACSR_TBU 		0x00000004  //!< Transmit Buffer Unavailable
#define ETH_ETH_DMACSR_TPS 		0x00000002  //!< Transmit Process Stopped
#define ETH_ETH_DMACSR_TI 		0x00000001  //!< Transmit Interrupt
/** @} */

/** @name  ETH_DMACMFCR:    (eth Offset: 0x116C) Channel missed frame count register  */
/** @{ */
#define ETH_DMACMFCR_MFCO		0x00008000  //!< Overflow status of the MFC Counter
#define ETH_DMACMFCR_MFC		0x000007FF  //!< Dropped Packet Counters
/** @} */



/** @} */ // @relates ETH_TypeDef


/***************************************************************************//**
 *  @name Ethernet MAC configuration
 ******************************************************************************/

/** @{ */
#ifndef PHY_WRITE_TOT
#define PHY_WRITE_TOT 	255		//!< PHY WRITE TOUT
#endif
#ifndef PHY_READ_TOT
#define PHY_READ_TOT 	255		//!< PHY READ TOUT
#endif

#define PHY_RESET_DELAY 	255
#define PHY_CONFIG_DELAY	255
#define ETH_REG_WRITE_DELAY	2
/** @} */


/**--------------------------------------------------------------------------**/
/**
  *                           ETH Frames defines
  */
/**--------------------------------------------------------------------------**/

/** @name ENET_Buffers_setting
  * @{
  */
#define ETH_MAX_PACKET_SIZE    1524    /*!< ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC */
#define ETH_HEADER               14    /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETH_CRC                   4    /*!< Ethernet CRC */
#define ETH_EXTRA                 2    /*!< Extra bytes in some cases */
#define VLAN_TAG                  4    /*!< optional 802.1q VLAN Tag */
#define MIN_ETH_PAYLOAD          46    /*!< Minimum Ethernet payload size */
#define MAX_ETH_PAYLOAD        1500    /*!< Maximum Ethernet payload size */
#define JUMBO_FRAME_PAYLOAD    9000    /*!< Jumbo frame payload size */
/** @} */

/** Here we configure each Ethernet driver transmit buffer to fit the Max size Ethernet packet */
#ifndef ETH_TX_BUF_SIZE
 #define ETH_TX_BUF_SIZE         ETH_MAX_PACKET_SIZE
#endif

/** Here we configure each Ethernet driver receive buffer to fit the Max size Ethernet packet */
#ifndef ETH_RX_BUF_SIZE
 #define ETH_RX_BUF_SIZE         ETH_MAX_PACKET_SIZE
#endif

/// ETH DMA Descriptors data structure definition
struct ETH_DMADescTypeDef
{
  __IO uint32_t   Status;           		/*!< Status */
  uint32_t   ControlBufferSize;     		/*!< Control and Buffer1, Buffer2 lengths */
  uint8_t*   Buffer1Addr;           		/*!< Buffer1 address pointer */
  ETH_DMADescTypeDef* Buffer2NextDescAddr;  /*!< Buffer2 or next descriptor address pointer */
  /*!< Enhanced ETHERNET DMA PTP Descriptors */
  uint32_t   ExtendedStatus;        		/*!< Extended status for PTP receive descriptor */
  uint32_t   Reserved1;						/*!< reserved */
  uint32_t   TimeStampLow;          		/*!< Time Stamp Low value for transmit and receive */
  uint32_t   TimeStampHigh;         		/*!< Time Stamp High value for transmit and receive */
  uint8_t	 Buff[ETH_TX_BUF_SIZE] __attribute__ ((aligned (4)));	/*!< buffer */

};

/*
   DMA Tx Desciptor
  -----------------------------------------------------------------------------------------------
  TDES0 | OWN(31) | CTRL[30:26] | Reserved[25:24] | CTRL[23:20] | Reserved[19:17] | Status[16:0] |
  -----------------------------------------------------------------------------------------------
  TDES1 | Reserved[31:29] | Buffer2 ByteCount[28:16] | Reserved[15:13] | Buffer1 ByteCount[12:0] |
  -----------------------------------------------------------------------------------------------
  TDES2 |                         Buffer1 Address [31:0]                                         |
  -----------------------------------------------------------------------------------------------
  TDES3 |                   Buffer2 Address [31:0] / Next Descriptor Address [31:0]              |
  -----------------------------------------------------------------------------------------------
*/

/**
  * @name  Bit definition of TDES0 register: DMA Tx descriptor status register
  */
/** @{ */
#define ETH_DMATXDESC_OWN                     0x80000000  //!< OWN bit: descriptor is owned by DMA engine
#define ETH_DMATXDESC_IC                      0x40000000  //!< Interrupt on Completion
#define ETH_DMATXDESC_LS                      0x20000000  //!< Last Segment
#define ETH_DMATXDESC_FS                      0x10000000  //!< First Segment
#define ETH_DMATXDESC_DC                      0x08000000  //!< Disable CRC
#define ETH_DMATXDESC_DP                      0x04000000  //!< Disable Padding
#define ETH_DMATXDESC_TTSE                    0x02000000  //!< Transmit Time Stamp Enable
#define ETH_DMATXDESC_CIC                     0x00C00000  //!< Checksum Insertion Control: 4 cases
#define ETH_DMATXDESC_CIC_BYPASS              0x00000000  //!< Do Nothing: Checksum Engine is bypassed
#define ETH_DMATXDESC_CIC_IPV4HEADER          0x00400000  //!< IPV4 header Checksum Insertion
#define ETH_DMATXDESC_CIC_TCPUDPICMP_SEGMENT  0x00800000  //!< TCP/UDP/ICMP Checksum Insertion calculated over segment only
#define ETH_DMATXDESC_CIC_TCPUDPICMP_FULL     0x00C00000  //!< TCP/UDP/ICMP Checksum Insertion fully calculated
#define ETH_DMATXDESC_TER                     0x00200000  //!< Transmit End of Ring
#define ETH_DMATXDESC_TCH                     0x00100000  //!< Second Address Chained
#define ETH_DMATXDESC_TTSS                    0x00020000  //!< Tx Time Stamp Status
#define ETH_DMATXDESC_IHE                     0x00010000  //!< IP Header Error
#define ETH_DMATXDESC_ES                      0x00008000  //!< Error summary: OR of the following bits: UE || ED || EC || LCO || NC || LCA || FF || JT
#define ETH_DMATXDESC_JT                      0x00004000  //!< Jabber Timeout
#define ETH_DMATXDESC_FF                      0x00002000  //!< Frame Flushed: DMA/MTL flushed the frame due to SW flush
#define ETH_DMATXDESC_PCE                     0x00001000  //!< Payload Checksum Error
#define ETH_DMATXDESC_LCA                     0x00000800  //!< Loss of Carrier: carrier lost during transmission
#define ETH_DMATXDESC_NC                      0x00000400  //!< No Carrier: no carrier signal from the transceiver
#define ETH_DMATXDESC_LCO                     0x00000200  //!< Late Collision: transmission aborted due to collision
#define ETH_DMATXDESC_EC                      0x00000100  //!< Excessive Collision: transmission aborted after 16 collisions
#define ETH_DMATXDESC_VF                      0x00000080  //!< VLAN Frame
#define ETH_DMATXDESC_CC                      0x00000078  //!< Collision Count
#define ETH_DMATXDESC_ED                      0x00000004  //!< Excessive Deferral
#define ETH_DMATXDESC_UF                      0x00000002  //!< Underflow Error: late data arrival from the memory
#define ETH_DMATXDESC_DB                      0x00000001  //!< Deferred Bit
/** @} */

/**
  * @name  Bit definition of TDES1 register
  */
/** @{ */
#define ETH_DMATXDESC_TBS2  ((uint32_t)0x1FFF0000)  //!< Transmit Buffer2 Size
#define ETH_DMATXDESC_TBS1  ((uint32_t)0x00001FFF)  //!< Transmit Buffer1 Size
/** @} */

/**
  * @name  Bit definition of TDES2 register
  */
#define ETH_DMATXDESC_B1AP  ((uint32_t)0xFFFFFFFF)  //!< Buffer1 Address Pointer

/**
  * @name  Bit definition of TDES3 register
  */
#define ETH_DMATXDESC_B2AP  ((uint32_t)0xFFFFFFFF)  //!< Buffer2 Address Pointer

  /*---------------------------------------------------------------------------------------------
  TDES6 |                         Transmit Time Stamp Low [31:0]                                 |
  -----------------------------------------------------------------------------------------------
  TDES7 |                         Transmit Time Stamp High [31:0]                                |
  ----------------------------------------------------------------------------------------------*/

/** @name Bit definition of TDES6 register */
 #define ETH_DMAPTPTXDESC_TTSL  ((uint32_t)0xFFFFFFFF)  //!< Transmit Time Stamp Low

/** @name Bit definition of TDES7 register */
 #define ETH_DMAPTPTXDESC_TTSH  ((uint32_t)0xFFFFFFFF)  //!< Transmit Time Stamp High


/*
  DMA Rx Descriptor
  --------------------------------------------------------------------------------------------------------------------
  RDES0 | OWN(31) |                                             Status [30:0]                                          |
  ---------------------------------------------------------------------------------------------------------------------
  RDES1 | CTRL(31) | Reserved[30:29] | Buffer2 ByteCount[28:16] | CTRL[15:14] | Reserved(13) | Buffer1 ByteCount[12:0] |
  ---------------------------------------------------------------------------------------------------------------------
  RDES2 |                                       Buffer1 Address [31:0]                                                 |
  ---------------------------------------------------------------------------------------------------------------------
  RDES3 |                          Buffer2 Address [31:0] / Next Descriptor Address [31:0]                             |
  ---------------------------------------------------------------------------------------------------------------------
*/

/**
  * @name  Bit definition of RDES0 register: DMA Rx descriptor status register
  */
/** @{ */
#define ETH_DMARXDESC_OWN         0x80000000  //!< OWN bit: descriptor is owned by DMA engine
#define ETH_DMARXDESC_AFM         0x40000000  //!< DA Filter Fail for the rx frame
#define ETH_DMARXDESC_FL          0x3FFF0000  //!< Receive descriptor frame length
#define ETH_DMARXDESC_ES          0x00008000  //!< Error summary: OR of the following bits: DE || OE || IPC || LC || RWT || RE || CE
#define ETH_DMARXDESC_DE          0x00004000  //!< Descriptor error: no more descriptors for receive frame
#define ETH_DMARXDESC_SAF         0x00002000  //!< SA Filter Fail for the received frame
#define ETH_DMARXDESC_LE          0x00001000  //!< Frame size not matching with length field
#define ETH_DMARXDESC_OE          0x00000800  //!< Overflow Error: Frame was damaged due to buffer overflow
#define ETH_DMARXDESC_VLAN        0x00000400  //!< VLAN Tag: received frame is a VLAN frame
#define ETH_DMARXDESC_FS          0x00000200  //!< First descriptor of the frame
#define ETH_DMARXDESC_LS          0x00000100  //!< Last descriptor of the frame
#define ETH_DMARXDESC_IPHCE		  0x00000080  //!< IPC Checksum Error: Rx Ipv4 header checksum error
#define ETH_DMARXDESC_LC          0x00000040  //!< Late collision occurred during reception
#define ETH_DMARXDESC_FT          0x00000020  //!< Frame type - Ethernet, otherwise 802.3
#define ETH_DMARXDESC_RWT         0x00000010  //!< Receive Watchdog Timeout: watchdog timer expired during reception
#define ETH_DMARXDESC_RE          0x00000008  //!< Receive error: error reported by MII interface
#define ETH_DMARXDESC_DBE         0x00000004  //!< Dribble bit error: frame contains non int multiple of 8 bits
#define ETH_DMARXDESC_CE          0x00000002  //!< CRC error
#define ETH_DMARXDESC_PCE	      0x00000001  //!< Rx MAC Address/Payload Checksum Error: Rx MAC address matched/ Rx Payload Checksum Error
/** @} */

/**
  * @name  Bit definition of RDES1 register
  */
/** @{ */
#define ETH_DMARXDESC_DIC   0x80000000  //!< Disable Interrupt on Completion
#define ETH_DMARXDESC_RBS2  0x1FFF0000  //!< Receive Buffer2 Size
#define ETH_DMARXDESC_RER   0x00008000  //!< Receive End of Ring
#define ETH_DMARXDESC_RCH   0x00004000  //!< Second Address Chained
#define ETH_DMARXDESC_RBS1  0x00001FFF  //!< Receive Buffer1 Size
/** @} */

/**
  * @name  Bit definition of RDES2 register
  */
#define ETH_DMARXDESC_B1AP  ((uint32_t)0xFFFFFFFF)  //!< Buffer1 Address Pointer

/**
  * @name  Bit definition of RDES3 register
  */
#define ETH_DMARXDESC_B2AP  ((uint32_t)0xFFFFFFFF)  //!< Buffer2 Address Pointer

/*---------------------------------------------------------------------------------------------------------------------
  RDES4 |                   Reserved[31:15]              |             Extended Status [14:0]                          |
  ---------------------------------------------------------------------------------------------------------------------
  RDES5 |                                            Reserved[31:0]                                                    |
  ---------------------------------------------------------------------------------------------------------------------
  RDES6 |                                       Receive Time Stamp Low [31:0]                                          |
  ---------------------------------------------------------------------------------------------------------------------
  RDES7 |                                       Receive Time Stamp High [31:0]                                         |
  --------------------------------------------------------------------------------------------------------------------*/

/** @name Bit definition of RDES4 register */
/** @{ */
#define ETH_DMAPTPRXDESC_PTPV     	0x00002000  //!< PTP Version
#define ETH_DMAPTPRXDESC_PTPFT    	0x00001000  //!< PTP Frame Type
#define ETH_DMAPTPRXDESC_PTPMT    	0x00000F00  //!< PTP Message Type *
#define ETH_DMAPTPRXDESC_PTPMT_SYNC                      0x00000100  //!< SYNC message (all clock types)
#define ETH_DMAPTPRXDESC_PTPMT_FOLLOWUP                  0x00000200  //!< FollowUp message (all clock types)
#define ETH_DMAPTPRXDESC_PTPMT_DELAYREQ                  0x00000300  //!< DelayReq message (all clock types)
#define ETH_DMAPTPRXDESC_PTPMT_DELAYRESP                 0x00000400  //!< DelayResp message (all clock types)
#define ETH_DMAPTPRXDESC_PTPMT_PDELAYREQ_ANNOUNCE        0x00000500  //!< PdelayReq message (peer-to-peer transparent clock) or Announce message (Ordinary or Boundary clock)
#define ETH_DMAPTPRXDESC_PTPMT_PDELAYRESP_MANAG          0x00000600  //!< PdelayResp message (peer-to-peer transparent clock) or Management message (Ordinary or Boundary clock)
#define ETH_DMAPTPRXDESC_PTPMT_PDELAYRESPFOLLOWUP_SIGNAL 0x00000700  //!< PdelayRespFollowUp message (peer-to-peer transparent clock) or Signaling message (Ordinary or Boundary clock)
#define ETH_DMAPTPRXDESC_IPV6PR   	0x00000080  //!< IPv6 Packet Received
#define ETH_DMAPTPRXDESC_IPV4PR   	0x00000040  //!< IPv4 Packet Received
#define ETH_DMAPTPRXDESC_IPCB     	0x00000020  //!< IP Checksum Bypassed
#define ETH_DMAPTPRXDESC_IPPE     	0x00000010  //!< IP Payload Error
#define ETH_DMAPTPRXDESC_IPHE     	0x00000008  //!< IP Header Error
#define ETH_DMAPTPRXDESC_IPPT     	0x00000007  //!< IP Payload Type
#define ETH_DMAPTPRXDESC_IPPT_UDP   0x00000001  //!<   UDP payload encapsulated in the IP datagram
#define ETH_DMAPTPRXDESC_IPPT_TCP   0x00000002  //!<   TCP payload encapsulated in the IP datagram
#define ETH_DMAPTPRXDESC_IPPT_ICMP  0x00000003  //!<   ICMP payload encapsulated in the IP datagram
/** @} */

/** @name Bit definition of RDES6 register */
#define ETH_DMAPTPRXDESC_RTSL  ((uint32_t)0xFFFFFFFF)  //!< Receive Time Stamp Low

/** @name Bit definition of RDES7 register */
#define ETH_DMAPTPRXDESC_RTSH  ((uint32_t)0xFFFFFFFF)  //!< Receive Time Stamp High


/// @name ETH MAC configuration
/** @{ */
#define MAC_CFG_FLG_IFCE_MII_RMII		0x01	//!< Enable RMII interface
#define MAC_CFG_FLG_AUTONEGOTIATION		0x02	//!< Enable autonegotiation
#define MAC_CFG_FLG_DUPLEX_MODE			0x04	//!< 1-Full duplex
#define MAC_CFG_FLG_100M				0x08	//!< Speed 100Mbit
#define MAC_CFG_FLG_MCO1				0x10	//!< MCO1 used for ref clk
#define MAC_CFG_FLG_MCO2				0x20	//!< MCO2 used for ref clk
/** @} */

/// MAC data
typedef struct
{
	uint32_t mac_cfg;			//!< MAC_CFG_FLG_XXX configuration flags
	ETH_DMADescTypeDef *TxDesc;	//!< First DMA Tx Descriptor
	ETH_DMADescTypeDef *RxDesc;	//!< First DMA Rx Descriptor
	uint32_t rx_len;			//!< Received frame length
	uint8_t  mac_adr[6];		//!< MAC address
} eth_mac_data_t;

/// MAC configuration
typedef struct
{
	uint32_t mac_cfg_flags;		//!< MAC_CFG_FLG_XXX configuration flags

	eth_mac_data_t* mac_data;	//!< pointer to MAC data

	/// ETH_MACCR configuration
	/** Set the WD bit according to ETH Watchdog value */
	/** Set the JD: bit according to ETH Jabber value */
	/** Set the IFG bit according to ETH InterFrameGap value */
	/** Set the DCRS bit according to ETH CarrierSense value */
	/** Set the FES bit according to --- mac_data ---- */
	/** Set the DO bit according to ETH ReceiveOwn value */
	/** Set the LM bit according to ETH LoopbackMode value */
	/** Set the DM bit according to --- mac_data ---- */
	/** Set the IPCO bit according to ETH ChecksumOffload value */
	/** Set the DR bit according to ETH RetryTransmission value */
	/** Set the ACS bit according to ETH AutomaticPadCRCStrip value */
	/** Set the BL bit according to ETH BackOffLimit value */
	/** Set the DC bit according to ETH DeferralCheck value */
	uint32_t ETH_MACCR;

	/// ETH_MACFFR configuration
	/** Set the RA bit according to ETH ReceiveAll value */
	/** Set the SAF and SAIF bits according to ETH SourceAddrFilter value */
	/** Set the PCF bit according to ETH PassControlFrames value */
	/** Set the DBF bit according to ETH BroadcastFramesReception value */
	/** Set the DAIF bit according to ETH DestinationAddrFilter value */
	/** Set the PR bit according to ETH PromiscuousMode value */
	/** Set the PM, HMC and HPF bits according to ETH MulticastFramesFilter value */
	/** Set the HUC and HPF bits according to ETH UnicastFramesFilter value */
	uint32_t ETH_MACFFR;

	/// ETH_MACFCR configuration
	/** Set the PT bit according to ETH PauseTime value */
	/** Set the DZPQ bit according to ETH ZeroQuantaPause value */
	/** Set the PLT bit according to ETH PauseLowThreshold value */
	/** Set the UP bit according to ETH UnicastPauseFrameDetect value */
	/** Set the RFE bit according to ETH ReceiveFlowControl value */
	/** Set the TFE bit according to ETH TransmitFlowControl value */
	uint32_t ETH_MACFCR;

	/// ETH_DMAOMR configuration
    /** Set the DT bit according to ETH DropTCPIPChecksumErrorFrame value */
    /** Set the RSF bit according to ETH ReceiveStoreForward value */
    /** Set the DFF bit according to ETH FlushReceivedFrame value */
    /** Set the TSF bit according to ETH TransmitStoreForward value */
    /** Set the TTC bit according to ETH TransmitThresholdControl value */
    /** Set the FEF bit according to ETH ForwardErrorFrames value */
    /** Set the FUF bit according to ETH ForwardUndersizedGoodFrames value */
    /** Set the RTC bit according to ETH ReceiveThresholdControl value */
    /** Set the OSF bit according to ETH SecondFrameOperate value */
	uint32_t ETH_DMAOMR;

	/// ETH_DMABMR configuration
    /** Set the AAL bit according to ETH AddressAlignedBeats value */
    /** Set the FB bit according to ETH FixedBurst value */
    /** Set the RPBL and 4*PBL bits according to ETH RxDMABurstLength value */
    /** Set the PBL and 4*PBL bits according to ETH TxDMABurstLength value */
    /** Set the Enhanced DMA descriptors bit according to ETH EnhancedDescriptorFormat value*/
    /** Set the DSL bit according to ETH DesciptorSkipLength value */
    /** Set the PR and DA bits according to ETH DMAArbitration value */
	uint32_t ETH_DMABMR;

	ETH_DMADescTypeDef* DMATxDescTab;	//!< DMA Tx Descriptor table
	ETH_DMADescTypeDef* DMARxDescTab;	//!< DMA Rx Descriptor table

	uint32_t RCC_CFGR;			//!< MCO1/MCO2 select and MCOxPRE prescaler
	uint8_t  phy_address;		//!< Ethernet PHY address (0-32)
	uint8_t ETH_RXBUFNB;      	//!< Number of Rx buffers
	uint8_t ETH_TXBUFNB;   		//!< Number of Tx buffers
} eth_mac_cfg_t;


extern "C" void EthernetMACAddrGet(const eth_mac_cfg_t* cfg);
RES_CODE HAL_ETH_WritePHYRegister(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, uint16_t PHYReg, uint32_t RegValue);
RES_CODE HAL_ETH_ReadPHYRegister(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, uint16_t PHYReg, uint32_t *RegValue);
void TRACEPHY(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg, const char* err);
RES_CODE HAL_ETH_Init(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);
void HAL_ETH_DMATxDescListInit(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);
void HAL_ETH_DMARxDescListInit(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);
void HAL_ETH_Start(ETH_TypeDef* mac);
void HAL_ETH_Stop(ETH_TypeDef* mac);
void HAL_ETH_TransmitFrame(ETH_TypeDef* mac, eth_mac_data_t* mac_data, uint32_t FrameLength);
void HAL_ETH_ReleaseFrame(eth_mac_data_t* mac_data, uint32_t descriptors);
uint32_t HAL_ETH_GetReceivedFrame_IT(const eth_mac_cfg_t* cfg);

#endif /* HARDWARE_STM32_STM32H7_ETH_H7_H_ */

/** @} */
