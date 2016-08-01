/*
 * eth_f2.h
 *
 *  Created on: May 20, 2015
 *      Author: miro
 */

#ifndef HARDWARE_STM32_STM32F2_ETH_F2_H_
#define HARDWARE_STM32_STM32F2_ETH_F2_H_


#include <mcu_inc.h>
#include <tmos_types.h>

/*******************************************************************************
 *  Ethernet (ETH): media access control (MAC) with DMA controller
 ******************************************************************************/
typedef struct
{
  __IO uint32_t ETH_MACCR;			//!< (eth Offset: 0x0000) Ethernet MAC configuration register
  __IO uint32_t ETH_MACFFR;         //!< (eth Offset: 0x0004) Ethernet MAC frame filter register
  __IO uint32_t ETH_MACHTHR;        //!< (eth Offset: 0x0008) Ethernet MAC hash table high register
  __IO uint32_t ETH_MACHTLR;        //!< (eth Offset: 0x000C) Ethernet MAC hash table low register
  __IO uint32_t ETH_MACMIIAR;       //!< (eth Offset: 0x0010) Ethernet MAC MII address register
  __IO uint32_t ETH_MACMIIDR;       //!< (eth Offset: 0x0014) Ethernet MAC MII data register
  __IO uint32_t ETH_MACFCR;         //!< (eth Offset: 0x0018) Ethernet MAC flow control register
  __IO uint32_t ETH_MACVLANTR;      //!< (eth Offset: 0x001C) Ethernet MAC VLAN tag register
  	   uint32_t reserved0[2];       //!< (eth Offset: 0x0020)
  __IO uint32_t ETH_MACRWUFFR;      //!< (eth Offset: 0x0028) Ethernet MAC remote wakeup frame filter register
  __IO uint32_t ETH_MACPMTCSR;      //!< (eth Offset: 0x002C) Ethernet MAC PMT control and status register
  	   uint32_t reserved1;			//!< (eth Offset: 0x0030)
  __IO uint32_t ETH_MACDBGR;        //!< (eth Offset: 0x0034) Ethernet MAC debug register
  __IO uint32_t ETH_MACSR;          //!< (eth Offset: 0x0038) Ethernet MAC interrupt status register
  __IO uint32_t ETH_MACIMR;         //!< (eth Offset: 0x003C) Ethernet MAC interrupt mask register
  __IO uint32_t ETH_MACA0HR;        //!< (eth Offset: 0x0040) Ethernet MAC address 0 high register
  __IO uint32_t ETH_MACA0LR;        //!< (eth Offset: 0x0044) Ethernet MAC address 0 low register
  __IO uint32_t ETH_MACA1HR;        //!< (eth Offset: 0x0048) Ethernet MAC address 1 high register
  __IO uint32_t ETH_MACA1LR;        //!< (eth Offset: 0x004C) Ethernet MAC address 1 low register
  __IO uint32_t ETH_MACA2HR;        //!< (eth Offset: 0x0050) Ethernet MAC address 2 high register
  __IO uint32_t ETH_MACA2LR;        //!< (eth Offset: 0x0054) Ethernet MAC address 2 low register
  __IO uint32_t ETH_MACA3HR;        //!< (eth Offset: 0x0058) Ethernet MAC address 3 high register
  __IO uint32_t ETH_MACA3LR;        //!< (eth Offset: 0x005C) Ethernet MAC address 3 low register
  	   uint32_t reserved2[40]; 		//!< (eth Offset: 0x0060)
  __IO uint32_t ETH_MMCCR;          //!< (eth Offset: 0x0100) Ethernet MMC control register
  __IO uint32_t ETH_MMCRIR;         //!< (eth Offset: 0x0104) Ethernet MMC receive interrupt register
  __IO uint32_t ETH_MMCTIR;         //!< (eth Offset: 0x0108) Ethernet MMC transmit interrupt register
  __IO uint32_t ETH_MMCRIMR;        //!< (eth Offset: 0x010C) Ethernet MMC receive interrupt mask register
  __IO uint32_t ETH_MMCTIMR;        //!< (eth Offset: 0x0110) Ethernet MMC transmit interrupt mask register
  	   uint32_t reserved3[14];  	//!< (eth Offset: 0x0114)
  __IO uint32_t ETH_MMCTGFSCCR;     //!< (eth Offset: 0x014C) Ethernet MMC transmitted good frames after a single collision counter register
  __IO uint32_t ETH_MMCTGFMSCCR;    //!< (eth Offset: 0x0150) Ethernet MMC transmitted good frames after more than a single collision counter register
  	   uint32_t reserved4[5];  		//!< (eth Offset: 0x0154)
  __IO uint32_t ETH_MMCTGFCR;       //!< (eth Offset: 0x0168) Ethernet MMC transmitted good frames counter register
  	   uint32_t reserved5[10];		//!< (eth Offset: 0x016C)
  __IO uint32_t ETH_MMCRFCECR;      //!< (eth Offset: 0x0194) Ethernet MMC received frames with CRC error counter register
  __IO uint32_t ETH_MMCRFAECR;      //!< (eth Offset: 0x0198) Ethernet MMC received frames with alignment error counter register
  	   uint32_t reserved6[10]; 		//!< (eth Offset: 0x019C)
  __IO uint32_t ETH_MMCRGUFCR;      //!< (eth Offset: 0x01C4) MMC received good unicast frames counter register
  	   uint32_t reserved7[334]; 	//!< (eth Offset: 0x01C8)
  __IO uint32_t ETH_PTPTSCR;        //!< (eth Offset: 0x0700) Ethernet PTP time stamp control register
  __IO uint32_t ETH_PTPSSIR;        //!< (eth Offset: 0x0704) Ethernet PTP subsecond increment register
  __IO uint32_t ETH_PTPTSHR;        //!< (eth Offset: 0x0708) Ethernet PTP time stamp high register
  __IO uint32_t ETH_PTPTSLR;        //!< (eth Offset: 0x070C) Ethernet PTP time stamp low register
  __IO uint32_t ETH_PTPTSHUR;       //!< (eth Offset: 0x0710) Ethernet PTP time stamp high update register
  __IO uint32_t ETH_PTPTSLUR;       //!< (eth Offset: 0x0714) Ethernet PTP time stamp low update register
  __IO uint32_t ETH_PTPTSAR;        //!< (eth Offset: 0x0718) Ethernet PTP time stamp addend register
  __IO uint32_t ETH_PTPTTHR;        //!< (eth Offset: 0x071C) Ethernet PTP target time high register
  __IO uint32_t ETH_PTPTTLR;        //!< (eth Offset: 0x0720) Ethernet PTP target time low register
  	   uint32_t reserved8;     		//!< (eth Offset: 0x0724)
  __IO uint32_t ETH_PTPTSSR;  		//!< (eth Offset: 0x0728) Ethernet PTP time stamp status register
  __IO uint32_t ETH_PTPPPSCR;  		//!< (eth Offset: 0x072C) Ethernet PTP PPS control register
  	   uint32_t reserved9[564]; 	//!< (eth Offset: 0x0730)
  __IO uint32_t ETH_DMABMR;         //!< (eth Offset: 0x1000) Ethernet DMA bus mode register
  __IO uint32_t ETH_DMATPDR;        //!< (eth Offset: 0x1004) Ethernet DMA transmit poll demand register
  __IO uint32_t ETH_DMARPDR;        //!< (eth Offset: 0x1008) EHERNET DMA receive poll demand register
  __IO uint32_t ETH_DMARDLAR;       //!< (eth Offset: 0x100C) Ethernet DMA receive descriptor list address register
  __IO uint32_t ETH_DMATDLAR;       //!< (eth Offset: 0x1010) Ethernet DMA transmit descriptor list address register
  __IO uint32_t ETH_DMASR;          //!< (eth Offset: 0x1014) Ethernet DMA status register
  __IO uint32_t ETH_DMAOMR;         //!< (eth Offset: 0x1018) Ethernet DMA operation mode register
  __IO uint32_t ETH_DMAIER;         //!< (eth Offset: 0x101C) Ethernet DMA interrupt enable register
  __IO uint32_t ETH_DMAMFBOCR;      //!< (eth Offset: 0x1020) Ethernet DMA missed frame and buffer overflow counter register
  __IO uint32_t ETH_DMARSWTR;  		//!< (eth Offset: 0x1024) Ethernet DMA receive status watchdog timer register
  	   uint32_t reserved10[8]; 		//!< (eth Offset: 0x1028)
  __IO uint32_t ETH_DMACHTDR;       //!< (eth Offset: 0x1048) Ethernet DMA current host transmit descriptor register
  __IO uint32_t ETH_DMACHRDR;       //!< (eth Offset: 0x104C) Ethernet DMA current host receive descriptor register
  __IO uint32_t ETH_DMACHTBAR;      //!< (eth Offset: 0x1050) Ethernet DMA current host transmit buffer address register
  __IO uint32_t ETH_DMACHRBAR;      //!< (eth Offset: 0x1054) Ethernet DMA current host receive buffer address register

} ETH_TypeDef;

/*******************************************************************************
 * @defgroup ETH_regs_define
 * @{
 ******************************************************************************/

/** @defgroup  ETH_MACCR:		(eth Offset: 0x0000) Ethernet MAC configuration registe	*/

#define ETH_MACCR_CSTF     		0x02000000  //!< CRC stripping for Type frames
#define ETH_MACCR_WD      		0x00800000  //!< Watchdog disable
#define ETH_MACCR_JD      		0x00400000  //!< Jabber disable
#define ETH_MACCR_IFG     		0x000E0000  //!< Inter-frame gap
#define ETH_MACCR_IFG_96Bit     0x00000000  //!<  Minimum IFG between frames during transmission is 96Bit
#define ETH_MACCR_IFG_88Bit     0x00020000  //!<  Minimum IFG between frames during transmission is 88Bit
#define ETH_MACCR_IFG_80Bit     0x00040000  //!<  Minimum IFG between frames during transmission is 80Bit
#define ETH_MACCR_IFG_72Bit     0x00060000  //!<  Minimum IFG between frames during transmission is 72Bit
#define ETH_MACCR_IFG_64Bit     0x00080000  //!<  Minimum IFG between frames during transmission is 64Bit
#define ETH_MACCR_IFG_56Bit     0x000A0000  //!<  Minimum IFG between frames during transmission is 56Bit
#define ETH_MACCR_IFG_48Bit     0x000C0000  //!<  Minimum IFG between frames during transmission is 48Bit
#define ETH_MACCR_IFG_40Bit     0x000E0000  //!<  Minimum IFG between frames during transmission is 40Bit
#define ETH_MACCR_CSD     		0x00010000  //!< Carrier sense disable (during transmission)
#define ETH_MACCR_FES     		0x00004000  //!< Fast ethernet speed
#define ETH_MACCR_ROD     		0x00002000  //!< Receive own disable
#define ETH_MACCR_LM      		0x00001000  //!< loopback mode
#define ETH_MACCR_DM      		0x00000800  //!< Duplex mode
#define ETH_MACCR_IPCO    		0x00000400  //!< IPv4 Checksum offload
#define ETH_MACCR_RD      		0x00000200  //!< Retry disable
#define ETH_MACCR_APCS    		0x00000080  //!< Automatic Pad/CRC stripping
#define ETH_MACCR_BL      		0x00000060  //!< Back-off limit: random integer number (r) of slot time delays before rescheduling
                                            //!<       a transmission attempt during retries after a collision: 0 =< r <2^k
#define ETH_MACCR_BL_10    		0x00000000  //!<  k = min (n, 10)
#define ETH_MACCR_BL_8     		0x00000020  //!<  k = min (n, 8)
#define ETH_MACCR_BL_4     		0x00000040  //!<  k = min (n, 4)
#define ETH_MACCR_BL_1     		0x00000060  //!<  k = min (n, 1)
#define ETH_MACCR_DC      		0x00000010  //!< Deferral check
#define ETH_MACCR_TE      		0x00000008  //!< Transmitter enable
#define ETH_MACCR_RE      		0x00000004  //!< Receiver enable
/** @} */

/** @defgroup  ETH_MACFFR:      (eth Offset: 0x0004) Ethernet MAC frame filter register	*/
#define ETH_MACFFR_RA     		0x80000000  //!< Receive all
#define ETH_MACFFR_HPF    		0x00000400  //!< Hash or perfect filter
#define ETH_MACFFR_SAF    		0x00000200  //!< Source address filter
#define ETH_MACFFR_SAIF   		0x00000100  //!< Source address inverse filtering
#define ETH_MACFFR_PCF    		  0x000000C0  //!< Pass control frames: 3 cases
#define ETH_MACFFR_PCF_BlockAll   0x00000040  //!<  MAC filters all control frames from reaching the application
#define ETH_MACFFR_PCF_ForwardAll 0x00000080  //!<  MAC forwards all control frames to application even if they fail the Address Filter
#define ETH_MACFFR_PCF_AddrFilter 0x000000C0  //!<  MAC forwards control frames that pass the Address Filter. *
#define ETH_MACFFR_BFD    		0x00000020  //!< Broadcast frame disable
#define ETH_MACFFR_PAM    		0x00000010  //!< Pass all mutlicast
#define ETH_MACFFR_DAIF   		0x00000008  //!< Destination address inverse filtering
#define ETH_MACFFR_HM     		0x00000004  //!< Hash multicast
#define ETH_MACFFR_HU     		0x00000002  //!< Hash unicast
#define ETH_MACFFR_PM     		0x00000001  //!< Promiscuous mode
/** @} */

/** @defgroup  ETH_MACHTHR:     (eth Offset: 0x0008) Ethernet MAC hash table high register	*/
/** @} */

/** @defgroup  ETH_MACHTLR:     (eth Offset: 0x000C) Ethernet MAC hash table low register	*/
/** @} */

/** @defgroup  ETH_MACMIIAR:    (eth Offset: 0x0010) Ethernet MAC MII address register	*/
#define ETH_MACMIIAR_PA   		0x0000F800  //!< Physical layer address
#define ETH_MACMIIAR_MR   		0x000007C0  //!< MII register in the selected PHY
#define ETH_MACMIIAR_CR   		0x0000001C  //!< CR clock range: 6 cases
#define ETH_MACMIIAR_CR_Div42   0x00000000  //!<  HCLK:60-100 MHz; MDC clock= HCLK/42
#define ETH_MACMIIAR_CR_Div62   0x00000004  //!<  HCLK:100-120 MHz; MDC clock= HCLK/62
#define ETH_MACMIIAR_CR_Div16   0x00000008  //!<  HCLK:20-35 MHz; MDC clock= HCLK/16
#define ETH_MACMIIAR_CR_Div26   0x0000000C  //!<  HCLK:35-60 MHz; MDC clock= HCLK/42
#define ETH_MACMIIAR_MW   		0x00000002  //!< MII write
#define ETH_MACMIIAR_MB   		0x00000001  //!< MII busy
/** @} */

/** @defgroup  ETH_MACMIIDR:    (eth Offset: 0x0014) Ethernet MAC MII data register	*/
/** @} */

/** @defgroup  ETH_MACFCR:      (eth Offset: 0x0018) Ethernet MAC flow control register	*/
#define ETH_MACFCR_PT     		0xFFFF0000  //!< Pause time
#define ETH_MACFCR_ZQPD   		0x00000080  //!< Zero-quanta pause disable
#define ETH_MACFCR_PLT    		0x00000030  //!< Pause low threshold: 4 cases
#define ETH_MACFCR_PLT_Minus4   0x00000000  //!<  Pause time minus 4 slot times
#define ETH_MACFCR_PLT_Minus28  0x00000010  //!<  Pause time minus 28 slot times
#define ETH_MACFCR_PLT_Minus144 0x00000020  //!<  Pause time minus 144 slot times
#define ETH_MACFCR_PLT_Minus256 0x00000030  //!<  Pause time minus 256 slot times
#define ETH_MACFCR_UPFD   		0x00000008  //!< Unicast pause frame detect
#define ETH_MACFCR_RFCE   		0x00000004  //!< Receive flow control enable
#define ETH_MACFCR_TFCE   		0x00000002  //!< Transmit flow control enable
#define ETH_MACFCR_FCBBPA 		0x00000001  //!< Flow control busy/back pressure activate
/** @} */

/** @defgroup  ETH_MACVLANTR:   (eth Offset: 0x001C) Ethernet MAC VLAN tag register	*/
#define ETH_MACVLANTR_VLANTC 	0x00010000  //!< 12-bit VLAN tag comparison
#define ETH_MACVLANTR_VLANTI 	0x0000FFFF  //!< VLAN tag identifier (for receive frames)
/** @} */

/** @defgroup  ETH_MACRWUFFR:   (eth Offset: 0x0028) Ethernet MAC remote wakeup frame filter register */
/** @} */

/** @defgroup  ETH_MACPMTCSR:   (eth Offset: 0x002C) Ethernet MAC PMT control and status register */
#define ETH_MACPMTCSR_WFFRPR 	0x80000000  //!< Wake-Up Frame Filter Register Pointer Reset
#define ETH_MACPMTCSR_GU     	0x00000200  //!< Global Unicast
#define ETH_MACPMTCSR_WFR    	0x00000040  //!< Wake-Up Frame Received
#define ETH_MACPMTCSR_MPR    	0x00000020  //!< Magic Packet Received
#define ETH_MACPMTCSR_WFE    	0x00000004  //!< Wake-Up Frame Enable
#define ETH_MACPMTCSR_MPE    	0x00000002  //!< Magic Packet Enable
#define ETH_MACPMTCSR_PD     	0x00000001  //!< Power Down
/** @} */

/** @defgroup  ETH_MACDBGR:     (eth Offset: 0x0034) Ethernet MAC debug register	*/
#define ETH_MACDBGR_TFF     	0x02000000  //!< Tx FIFO full
#define ETH_MACDBGR_TFNE     	0x01000000  //!< Tx FIFO not empty
#define ETH_MACDBGR_TFWA     	0x00400000  //!< Tx FIFO write active
#define ETH_MACDBGR_TFRS     	0x00300000  //!< Tx FIFO read status
#define ETH_MACDBGR_MTP     	0x00080000  //!< MAC transmitter in pause
#define ETH_MACDBGR_MTFCS     	0x00060000  //!< MAC transmit frame controller status
#define ETH_MACDBGR_MMTEA     	0x00010000  //!< MAC MII transmit engine active
#define ETH_MACDBGR_RFFL     	0x00000300  //!< Rx FIFO fill level
#define ETH_MACDBGR_RFRCS     	0x00000060  //!< Rx FIFO read controller status
#define ETH_MACDBGR_RFWRA     	0x00000010  //!< Rx FIFO write controller active
#define ETH_MACDBGR_MSFRWCS    	0x00000006  //!< MAC small FIFO read / write controllers status
#define ETH_MACDBGR_MMRPEA     	0x00000001  //!< MAC MII receive protocol engine active
/** @} */

/** @defgroup  ETH_MACSR:       (eth Offset: 0x0038) Ethernet MAC interrupt status register	*/
#define ETH_MACSR_TSTS      	0x00000200  //!< Time stamp trigger status
#define ETH_MACSR_MMCTS     	0x00000040  //!< MMC transmit status
#define ETH_MACSR_MMMCRS    	0x00000020  //!< MMC receive status
#define ETH_MACSR_MMCS      	0x00000010  //!< MMC status
#define ETH_MACSR_PMTS      	0x00000008  //!< PMT status
/** @} */

/** @defgroup  ETH_MACIMR:      (eth Offset: 0x003C) Ethernet MAC interrupt mask register	*/
#define ETH_MACIMR_TSTIM     	0x00000200  //!< Time stamp trigger interrupt mask
#define ETH_MACIMR_PMTIM     	0x00000008  //!< PMT interrupt mask
/** @} */

/** @defgroup  ETH_MACA0HR:     (eth Offset: 0x0040) Ethernet MAC address 0 high register	*/
/** @} */

/** @defgroup  ETH_MACA0LR:     (eth Offset: 0x0044) Ethernet MAC address 0 low register	*/
/** @} */

/** @defgroup  ETH_MACA1HR:     (eth Offset: 0x0048) Ethernet MAC address 1 high register	*/
#define ETH_MACA1HR_AE       	0x80000000  //!< Address enable
#define ETH_MACA1HR_SA       	0x40000000  //!< Source address
#define ETH_MACA1HR_MBC      	0x3F000000  //!< Mask byte control: bits to mask for comparison of the MAC Address bytes
#define ETH_MACA1HR_MBC_HBits15_8    0x20000000  //!< Mask MAC Address high reg bits [15:8]
#define ETH_MACA1HR_MBC_HBits7_0     0x10000000  //!< Mask MAC Address high reg bits [7:0]
#define ETH_MACA1HR_MBC_LBits31_24   0x08000000  //!< Mask MAC Address low reg bits [31:24]
#define ETH_MACA1HR_MBC_LBits23_16   0x04000000  //!< Mask MAC Address low reg bits [23:16]
#define ETH_MACA1HR_MBC_LBits15_8    0x02000000  //!< Mask MAC Address low reg bits [15:8]
#define ETH_MACA1HR_MBC_LBits7_0     0x01000000  //!< Mask MAC Address low reg bits [7:0]
#define ETH_MACA1HR_MACA1H   	0x0000FFFF  //!< MAC address1 high
/** @} */

/** @defgroup  ETH_MACA1LR:     (eth Offset: 0x004C) Ethernet MAC address 1 low register	*/
/** @} */

/** @defgroup  ETH_MACA2HR:     (eth Offset: 0x0050) Ethernet MAC address 2 high register	*/
#define ETH_MACA2HR_AE       	0x80000000  //!< Address enable
#define ETH_MACA2HR_SA       	0x40000000  //!< Source address
#define ETH_MACA2HR_MBC      	0x3F000000  //!< Mask byte control
#define ETH_MACA2HR_MBC_HBits15_8    0x20000000  //!< Mask MAC Address high reg bits [15:8]
#define ETH_MACA2HR_MBC_HBits7_0     0x10000000  //!< Mask MAC Address high reg bits [7:0]
#define ETH_MACA2HR_MBC_LBits31_24   0x08000000  //!< Mask MAC Address low reg bits [31:24]
#define ETH_MACA2HR_MBC_LBits23_16   0x04000000  //!< Mask MAC Address low reg bits [23:16]
#define ETH_MACA2HR_MBC_LBits15_8    0x02000000  //!< Mask MAC Address low reg bits [15:8]
#define ETH_MACA2HR_MBC_LBits7_0     0x01000000  //!< Mask MAC Address low reg bits [70]
#define ETH_MACA2HR_MACA2H   	0x0000FFFF  //!< MAC address1 high
/** @} */

/** @defgroup  ETH_MACA2LR:     (eth Offset: 0x0054) Ethernet MAC address 2 low register	*/
/** @} */

/** @defgroup  ETH_MACA3HR:     (eth Offset: 0x0058) Ethernet MAC address 3 high register	*/
#define ETH_MACA3HR_AE       	0x80000000  //!< Address enable
#define ETH_MACA3HR_SA       	0x40000000  //!< Source address
#define ETH_MACA3HR_MBC      	0x3F000000  //!< Mask byte control
#define ETH_MACA3HR_MBC_HBits15_8   0x20000000  //!< Mask MAC Address high reg bits [15:8]
#define ETH_MACA3HR_MBC_HBits7_0    0x10000000  //!< Mask MAC Address high reg bits [7:0]
#define ETH_MACA3HR_MBC_LBits31_24  0x08000000  //!< Mask MAC Address low reg bits [31:24]
#define ETH_MACA3HR_MBC_LBits23_16  0x04000000  //!< Mask MAC Address low reg bits [23:16]
#define ETH_MACA3HR_MBC_LBits15_8   0x02000000  //!< Mask MAC Address low reg bits [15:8]
#define ETH_MACA3HR_MBC_LBits7_0    0x01000000  //!< Mask MAC Address low reg bits [70]
#define ETH_MACA3HR_MACA3H   	0x0000FFFF  //!< address3 high
/** @} */

/** @defgroup  ETH_MACA3LR:     (eth Offset: 0x005C) Ethernet MAC address 3 low register	*/
/** @} */

/** @defgroup  ETH_MMCCR:       (eth Offset: 0x0100) Ethernet MMC control register	*/
#define ETH_MMCCR_MCFHP      	0x00000020  //!< MMC counter Full-Half preset (Only in STM32F2xx)
#define ETH_MMCCR_MCP        	0x00000010  //!< MMC counter preset (Only in STM32F2xx)
#define ETH_MMCCR_MCF        	0x00000008  //!< MMC Counter Freeze
#define ETH_MMCCR_ROR        	0x00000004  //!< Reset on Read
#define ETH_MMCCR_CSR        	0x00000002  //!< Counter Stop Rollover
#define ETH_MMCCR_CR         	0x00000001  //!< Counters Reset
/** @} */

/** @defgroup  ETH_MMCRIR:      (eth Offset: 0x0104) Ethernet MMC receive interrupt register	*/
#define ETH_MMCRIR_RGUFS     	0x00020000  //!< Set when Rx good unicast frames counter reaches half the maximum value
#define ETH_MMCRIR_RFAES     	0x00000040  //!< Set when Rx alignment error counter reaches half the maximum value
#define ETH_MMCRIR_RFCES     	0x00000020  //!< Set when Rx crc error counter reaches half the maximum value
/** @} */

/** @defgroup  ETH_MMCTIR:      (eth Offset: 0x0108) Ethernet MMC transmit interrupt register	*/
#define ETH_MMCTIR_TGFS      	0x00200000  //!< Set when Tx good frame count counter reaches half the maximum value
#define ETH_MMCTIR_TGFMSCS   	0x00008000  //!< Set when Tx good multi col counter reaches half the maximum value
#define ETH_MMCTIR_TGFSCS    	0x00004000  //!< Set when Tx good single col counter reaches half the maximum value
/** @} */

/** @defgroup  ETH_MMCRIMR:     (eth Offset: 0x010C) Ethernet MMC receive interrupt mask register	*/
#define ETH_MMCRIMR_RGUFM    	0x00020000  //!< Mask the interrupt when Rx good unicast frames counter reaches half the maximum value
#define ETH_MMCRIMR_RFAEM    	0x00000040  //!< Mask the interrupt when when Rx alignment error counter reaches half the maximum value
#define ETH_MMCRIMR_RFCEM    	0x00000020  //!< Mask the interrupt when Rx crc error counter reaches half the maximum value
/** @} */

/** @defgroup  ETH_MMCTIMR:     (eth Offset: 0x0110) Ethernet MMC transmit interrupt mask register	*/
#define ETH_MMCTIMR_TGFM     	0x00200000  //!< Mask the interrupt when Tx good frame count counter reaches half the maximum value
#define ETH_MMCTIMR_TGFMSCM  	0x00008000  //!< Mask the interrupt when Tx good multi col counter reaches half the maximum value
#define ETH_MMCTIMR_TGFSCM   	0x00004000  //!< Mask the interrupt when Tx good single col counter reaches half the maximum value
/** @} */

/** @defgroup  ETH_MMCTGFSCCR:  (eth Offset: 0x014C) Ethernet MMC transmitted good frames after a single collision counter register	*/
/** @} */

/** @defgroup  ETH_MMCTGFMSCCR: (eth Offset: 0x0150) Ethernet MMC transmitted good frames after more than a single collision counter register	*/
/** @} */

/** @defgroup  ETH_MMCTGFCR:    (eth Offset: 0x0168) Ethernet MMC transmitted good frames counter register	*/
/** @} */

/** @defgroup  ETH_MMCRFCECR:   (eth Offset: 0x0194) Ethernet MMC received frames with CRC error counter register	*/
/** @} */

/** @defgroup  ETH_MMCRFAECR:   (eth Offset: 0x0198) Ethernet MMC received frames with alignment error counter register	*/
/** @} */

/** @defgroup  ETH_MMCRGUFCR:   (eth Offset: 0x01C4) MMC received good unicast frames counter register	*/
/** @} */

/** @defgroup  ETH_PTPTSCR:     (eth Offset: 0x0700) Ethernet PTP time stamp control register	*/
#define ETH_PTPTSCR_TSPFFMAE   	0x00040000  //!< Time stamp PTP frame filtering MAC address enable
#define ETH_PTPTSCR_TSCNT     	0x00030000  //!< Time stamp clock node type
#define ETH_PTPTSSR_TSSMRME   	0x00008000  //!< Time stamp snapshot for message relevant to master enable
#define ETH_PTPTSSR_TSSEME    	0x00004000  //!< Time stamp snapshot for event message enable
#define ETH_PTPTSSR_TSSIPV4FE 	0x00002000  //!< Time stamp snapshot for IPv4 frames enable
#define ETH_PTPTSSR_TSSIPV6FE 	0x00001000  //!< Time stamp snapshot for IPv6 frames enable
#define ETH_PTPTSSR_TSSPTPOEFE	0x00000800  //!< Time stamp snapshot for PTP over ethernet frames enable
#define ETH_PTPTSSR_TSPTPPSV2E	0x00000400  //!< Time stamp PTP packet snooping for version2 format enable
#define ETH_PTPTSSR_TSSSR     	0x00000200  //!< Time stamp Sub-seconds rollover
#define ETH_PTPTSSR_TSSARFE   	0x00000100  //!< Time stamp snapshot for all received frames enable
#define ETH_PTPTSCR_TSARU    	0x00000020  //!< Addend register update
#define ETH_PTPTSCR_TSITE    	0x00000010  //!< Time stamp interrupt trigger enable
#define ETH_PTPTSCR_TSSTU    	0x00000008  //!< Time stamp update
#define ETH_PTPTSCR_TSSTI    	0x00000004  //!< Time stamp initialize
#define ETH_PTPTSCR_TSFCU    	0x00000002  //!< Time stamp fine or coarse update
#define ETH_PTPTSCR_TSE      	0x00000001  //!< Time stamp enable
/** @} */

/** @defgroup  ETH_PTPSSIR:     (eth Offset: 0x0704) Ethernet PTP subsecond increment register	*/
/** @} */

/** @defgroup  ETH_PTPTSHR:     (eth Offset: 0x0708) Ethernet PTP time stamp high register	*/
/** @} */

/** @defgroup  ETH_PTPTSLR:     (eth Offset: 0x070C) Ethernet PTP time stamp low register	*/
#define ETH_PTPTSLR_STPNS    	0x80000000  //!< System Time Positive or negative time
#define ETH_PTPTSLR_STSS     	0x7FFFFFFF  //!< System Time sub-seconds
/** @} */

/** @defgroup  ETH_PTPTSHUR:    (eth Offset: 0x0710) Ethernet PTP time stamp high update register	*/
/** @} */

/** @defgroup  ETH_PTPTSLUR:    (eth Offset: 0x0714) Ethernet PTP time stamp low update register	*/
#define ETH_PTPTSLUR_TSUPNS  	0x80000000  //!< Time stamp update Positive or negative time
#define ETH_PTPTSLUR_TSUSS   	0x7FFFFFFF  //!< Time stamp update sub-seconds
/** @} */

/** @defgroup  ETH_PTPTSAR:     (eth Offset: 0x0718) Ethernet PTP time stamp addend register	*/
/** @} */

/** @defgroup  ETH_PTPTTHR:     (eth Offset: 0x071C) Ethernet PTP target time high register	*/
/** @} */

/** @defgroup  ETH_PTPTTLR:     (eth Offset: 0x0720) Ethernet PTP target time low register	*/
/** @} */

/** @defgroup  ETH_PTPTSSR:  	(eth Offset: 0x0728) Ethernet PTP time stamp status register	*/
#define ETH_PTPTSSR_TSTTR    	0x00000002  //!< Time stamp target time reached
#define ETH_PTPTSSR_TSSO     	0x00000001  //!< Time stamp seconds overflow
/** @} */

/** @defgroup  ETH_PTPPPSCR:  	(eth Offset: 0x072C) Ethernet PTP PPS control register	*/
/** @} */

/** @defgroup  ETH_DMABMR:      (eth Offset: 0x1000) Ethernet DMA bus mode register 	*/
#define ETH_DMABMR_MB       			0x04000000  //!< Mixed burst
#define ETH_DMABMR_AAB       			0x02000000  //!< Address-Aligned beats
#define ETH_DMABMR_FPM        			0x01000000  //!< 4xPBL mode
#define ETH_DMABMR_USP       			0x00800000  //!< Use separate PBL
#define ETH_DMABMR_RDP       			0x007E0000  //!< RxDMA PBL
#define ETH_DMABMR_RDP_1Beat   			0x00020000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 1
#define ETH_DMABMR_RDP_2Beat   			0x00040000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 2
#define ETH_DMABMR_RDP_4Beat   			0x00080000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 4
#define ETH_DMABMR_RDP_8Beat   			0x00100000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 8
#define ETH_DMABMR_RDP_16Beat  			0x00200000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 16
#define ETH_DMABMR_RDP_32Beat  			0x00400000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 32
#define ETH_DMABMR_RDP_4xPBL_4Beat   	0x01020000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 4
#define ETH_DMABMR_RDP_4xPBL_8Beat   	0x01040000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 8
#define ETH_DMABMR_RDP_4xPBL_16Beat  	0x01080000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 16
#define ETH_DMABMR_RDP_4xPBL_32Beat  	0x01100000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 32
#define ETH_DMABMR_RDP_4xPBL_64Beat  	0x01200000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 64
#define ETH_DMABMR_RDP_4xPBL_128Beat 	0x01400000  //!<  maximum number of beats to be transferred in one RxDMA transaction is 128
#define ETH_DMABMR_FB        			0x00010000  //!< Fixed Burst
#define ETH_DMABMR_RTPR      			0x0000C000  //!< Rx Tx priority ratio
#define ETH_DMABMR_RTPR_1_1  			0x00000000  //!<  Rx Tx priority ratio 1:1
#define ETH_DMABMR_RTPR_2_1  			0x00004000  //!<  Rx Tx priority ratio 2:1
#define ETH_DMABMR_RTPR_3_1  			0x00008000  //!<  Rx Tx priority ratio 3:1
#define ETH_DMABMR_RTPR_4_1  			0x0000C000  //!<  Rx Tx priority ratio 4:1
#define ETH_DMABMR_PBL    				0x00003F00  //!< Programmable burst length
#define ETH_DMABMR_PBL_1Beat    		0x00000100  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 1
#define ETH_DMABMR_PBL_2Beat    		0x00000200  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 2
#define ETH_DMABMR_PBL_4Beat    		0x00000400  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 4
#define ETH_DMABMR_PBL_8Beat    		0x00000800  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 8
#define ETH_DMABMR_PBL_16Beat   		0x00001000  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 16
#define ETH_DMABMR_PBL_32Beat   		0x00002000  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 32
#define ETH_DMABMR_PBL_4xPBL_4Beat   	0x01000100  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 4
#define ETH_DMABMR_PBL_4xPBL_8Beat   	0x01000200  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 8
#define ETH_DMABMR_PBL_4xPBL_16Beat  	0x01000400  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 16
#define ETH_DMABMR_PBL_4xPBL_32Beat  	0x01000800  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 32
#define ETH_DMABMR_PBL_4xPBL_64Beat  	0x01001000  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 64
#define ETH_DMABMR_PBL_4xPBL_128Beat 	0x01002000  //!<  maximum number of beats to be transferred in one TxDMA (or both) transaction is 128
#define ETH_DMABMR_EDE       			0x00000080  //!< Enhanced Descriptor Enable
#define ETH_DMABMR_DSL       			0x0000007C  //!< Descriptor Skip Length
#define ETH_DMABMR_DA        			0x00000002  //!< DMA arbitration scheme
#define ETH_DMABMR_SR        			0x00000001  //!< Software reset
/** @} */

/** @defgroup  ETH_DMATPDR:     (eth Offset: 0x1004) Ethernet DMA transmit poll demand register	*/
/** @} */

/** @defgroup  ETH_DMARPDR:     (eth Offset: 0x1008) EHERNET DMA receive poll demand register	*/
/** @} */

/** @defgroup  ETH_DMARDLAR:    (eth Offset: 0x100C) Ethernet DMA receive descriptor list address register	*/
/** @} */

/** @defgroup  ETH_DMATDLAR:    (eth Offset: 0x1010) Ethernet DMA transmit descriptor list address register 	*/
/** @} */

/** @defgroup  ETH_DMASR:       (eth Offset: 0x1014) Ethernet DMA status register 	*/
#define ETH_DMASR_TSTS       		0x20000000  //!< Time-stamp trigger status
#define ETH_DMASR_PMTS       		0x10000000  //!< PMT status
#define ETH_DMASR_MMCS       		0x08000000  //!< MMC status
#define ETH_DMASR_EBS        		0x03800000  //!< Error bits status
#define ETH_DMASR_EBS_DescAccess   	0x02000000  //!<  Error bits 0-data buffer, 1-desc. access
#define ETH_DMASR_EBS_ReadTransf   	0x01000000  //!<  Error bits 0-write transfer, 1-read transfer
#define ETH_DMASR_EBS_DataTransfTx 	0x00800000  //!<  Error bits 0-Rx DMA, 1-Tx DMA
#define ETH_DMASR_TPS         		0x00700000  //!< Transmit process state
#define ETH_DMASR_TPS_Stopped       0x00000000  //!<  Stopped - Reset or Stop Tx Command issued
#define ETH_DMASR_TPS_Fetching      0x00100000  //!<  Running - fetching the Tx descriptor
#define ETH_DMASR_TPS_Waiting       0x00200000  //!<  Running - waiting for status
#define ETH_DMASR_TPS_Reading       0x00300000  //!<  Running - reading the data from host memory
#define ETH_DMASR_TPS_Suspended     0x00600000  //!<  Suspended - Tx Descriptor unavailabe
#define ETH_DMASR_TPS_Closing       0x00700000  //!<  Running - closing Rx descriptor
#define ETH_DMASR_RPS         		0x000E0000  //!< Receive process state
#define ETH_DMASR_RPS_Stopped   	0x00000000  //!<  Stopped - Reset or Stop Rx Command issued
#define ETH_DMASR_RPS_Fetching  	0x00020000  //!<  Running - fetching the Rx descriptor
#define ETH_DMASR_RPS_Waiting   	0x00060000  //!<  Running - waiting for packet
#define ETH_DMASR_RPS_Suspended 	0x00080000  //!<  Suspended - Rx Descriptor unavailable
#define ETH_DMASR_RPS_Closing   	0x000A0000  //!<  Running - closing descriptor
#define ETH_DMASR_RPS_Queuing   	0x000E0000  //!<  Running - queuing the recieve frame into host memory
#define ETH_DMASR_NIS        		0x00010000  //!< Normal interrupt summary
#define ETH_DMASR_AIS        		0x00008000  //!< Abnormal interrupt summary
#define ETH_DMASR_ERS        		0x00004000  //!< Early receive status
#define ETH_DMASR_FBES       		0x00002000  //!< Fatal bus error status
#define ETH_DMASR_ETS        		0x00000400  //!< Early transmit status
#define ETH_DMASR_RWTS       		0x00000200  //!< Receive watchdog timeout status
#define ETH_DMASR_RPSS       		0x00000100  //!< Receive process stopped status
#define ETH_DMASR_RBUS       		0x00000080  //!< Receive buffer unavailable status
#define ETH_DMASR_RS         		0x00000040  //!< Receive status
#define ETH_DMASR_TUS        		0x00000020  //!< Transmit underflow status
#define ETH_DMASR_ROS        		0x00000010  //!< Receive overflow status
#define ETH_DMASR_TJTS       		0x00000008  //!< Transmit jabber timeout status
#define ETH_DMASR_TBUS       		0x00000004  //!< Transmit buffer unavailable status
#define ETH_DMASR_TPSS       		0x00000002  //!< Transmit process stopped status
#define ETH_DMASR_TS         		0x00000001  //!< Transmit status
/** @} */

/** @defgroup  ETH_DMAOMR:      (eth Offset: 0x1018) Ethernet DMA operation mode register	*/
#define ETH_DMAOMR_DTCEFD    	0x04000000  //!< Disable Dropping of TCP/IP checksum error frames
#define ETH_DMAOMR_RSF       	0x02000000  //!< Receive store and forward
#define ETH_DMAOMR_DFRF      	0x01000000  //!< Disable flushing of received frames
#define ETH_DMAOMR_TSF       	0x00200000  //!< Transmit store and forward
#define ETH_DMAOMR_FTF       	0x00100000  //!< Flush transmit FIFO
#define ETH_DMAOMR_TTC       	0x0001C000  //!< Transmit threshold control
#define ETH_DMAOMR_TTC_64Bytes  0x00000000  //!<  threshold level of the MTL Transmit FIFO is 64 Bytes
#define ETH_DMAOMR_TTC_128Bytes 0x00004000  //!<  threshold level of the MTL Transmit FIFO is 128 Bytes
#define ETH_DMAOMR_TTC_192Bytes 0x00008000  //!<  threshold level of the MTL Transmit FIFO is 192 Bytes
#define ETH_DMAOMR_TTC_256Bytes 0x0000C000  //!<  threshold level of the MTL Transmit FIFO is 256 Bytes
#define ETH_DMAOMR_TTC_40Bytes  0x00010000  //!<  threshold level of the MTL Transmit FIFO is 40 Bytes
#define ETH_DMAOMR_TTC_32Bytes  0x00014000  //!<  threshold level of the MTL Transmit FIFO is 32 Bytes
#define ETH_DMAOMR_TTC_24Bytes  0x00018000  //!<  threshold level of the MTL Transmit FIFO is 24 Bytes
#define ETH_DMAOMR_TTC_16Bytes  0x0001C000  //!<  threshold level of the MTL Transmit FIFO is 16 Bytes
#define ETH_DMAOMR_ST       	0x00002000  //!< Start/stop transmission command
#define ETH_DMAOMR_FEF      	0x00000080  //!< Forward error frames
#define ETH_DMAOMR_FUGF     	0x00000040  //!< Forward undersized good frames
#define ETH_DMAOMR_RTC      	0x00000018  //!< receive threshold control
#define ETH_DMAOMR_RTC_64Bytes  0x00000000  //!<  threshold level of the MTL Receive FIFO is 64 Bytes
#define ETH_DMAOMR_RTC_32Bytes  0x00000008  //!<  threshold level of the MTL Receive FIFO is 32 Bytes
#define ETH_DMAOMR_RTC_96Bytes  0x00000010  //!<  threshold level of the MTL Receive FIFO is 96 Bytes
#define ETH_DMAOMR_RTC_128Bytes 0x00000018  //!<  threshold level of the MTL Receive FIFO is 128 Bytes
#define ETH_DMAOMR_OSF       	0x00000004  //!< operate on second frame
#define ETH_DMAOMR_SR        	0x00000002  //!< Start/stop receive
/** @} */

/** @defgroup  ETH_DMAIER:      (eth Offset: 0x101C) Ethernet DMA interrupt enable register	*/
#define ETH_DMAIER_NISE      	0x00010000  //!< Normal interrupt summary enable
#define ETH_DMAIER_AISE      	0x00008000  //!< Abnormal interrupt summary enable
#define ETH_DMAIER_ERIE      	0x00004000  //!< Early receive interrupt enable
#define ETH_DMAIER_FBEIE     	0x00002000  //!< Fatal bus error interrupt enable
#define ETH_DMAIER_ETIE      	0x00000400  //!< Early transmit interrupt enable
#define ETH_DMAIER_RWTIE     	0x00000200  //!< Receive watchdog timeout interrupt enable
#define ETH_DMAIER_RPSIE     	0x00000100  //!< Receive process stopped interrupt enable
#define ETH_DMAIER_RBUIE     	0x00000080  //!< Receive buffer unavailable interrupt enable
#define ETH_DMAIER_RIE       	0x00000040  //!< Receive interrupt enable
#define ETH_DMAIER_TUIE      	0x00000020  //!< Transmit Underflow interrupt enable
#define ETH_DMAIER_ROIE      	0x00000010  //!< Receive Overflow interrupt enable
#define ETH_DMAIER_TJTIE     	0x00000008  //!< Transmit jabber timeout interrupt enable
#define ETH_DMAIER_TBUIE     	0x00000004  //!< Transmit buffer unavailable interrupt enable
#define ETH_DMAIER_TPSIE     	0x00000002  //!< Transmit process stopped interrupt enable
#define ETH_DMAIER_TIE       	0x00000001  //!< Transmit interrupt enable
/** @} */

/** @defgroup  ETH_DMAMFBOCR:   (eth Offset: 0x1020) Ethernet DMA missed frame and buffer overflow counter register	*/
#define ETH_DMAMFBOCR_OFOC   	0x10000000  //!< Overflow bit for FIFO overflow counter
#define ETH_DMAMFBOCR_MFA    	0x0FFE0000  //!< Number of frames missed by the application
#define ETH_DMAMFBOCR_OMFC   	0x00010000  //!< Overflow bit for missed frame counter
#define ETH_DMAMFBOCR_MFC    	0x0000FFFF  //!< Number of frames missed by the controller
/** @} */

/** @defgroup  ETH_DMARSWTR:  	(eth Offset: 0x1024) Ethernet DMA receive status watchdog timer register	*/
/** @} */

/** @defgroup  ETH_DMACHTDR:    (eth Offset: 0x1048) Ethernet DMA current host transmit descriptor register 	*/
/** @} */

/** @defgroup  ETH_DMACHRDR:    (eth Offset: 0x104C) Ethernet DMA current host receive descriptor register	*/
/** @} */

/** @defgroup  ETH_DMACHTBAR:   (eth Offset: 0x1050) Ethernet DMA current host transmit buffer address register	*/
/** @} */

/** @defgroup  ETH_DMACHRBAR:   (eth Offset: 0x1054) Ethernet DMA current host receive buffer address register	*/
/** @} */

/** @} */ // @defgroup ETH_regs_define


/*******************************************************************************
 *  Ethernet MAC configuration
 ******************************************************************************/

#ifndef PHY_WRITE_TOT
#define PHY_WRITE_TOT 	255		//!< PHY WRITE TOUT
#endif
#ifndef PHY_READ_TOT
#define PHY_READ_TOT 	255		//!< PHY READ TOUT
#endif

#define PHY_RESET_DELAY 	255
#define PHY_CONFIG_DELAY	255
#define ETH_REG_WRITE_DELAY	2


/**--------------------------------------------------------------------------**/
/**
  * @brief                          ETH Frames defines
  */
/**--------------------------------------------------------------------------**/

/** @defgroup ENET_Buffers_setting
  * @{
  */
#define ETH_MAX_PACKET_SIZE    1520    /*!< ETH_HEADER + ETH_EXTRA + MAX_ETH_PAYLOAD + ETH_CRC */
#define ETH_HEADER               14    /*!< 6 byte Dest addr, 6 byte Src addr, 2 byte length/type */
#define ETH_CRC                   4    /*!< Ethernet CRC */
#define ETH_EXTRA                 2    /*!< Extra bytes in some cases */
#define VLAN_TAG                  4    /*!< optional 802.1q VLAN Tag */
#define MIN_ETH_PAYLOAD          46    /*!< Minimum Ethernet payload size */
#define MAX_ETH_PAYLOAD        1500    /*!< Maximum Ethernet payload size */
#define JUMBO_FRAME_PAYLOAD    9000    /*!< Jumbo frame payload size */

/* Here we configure each Ethernet driver transmit buffer to fit the Max size Ethernet
   packet */
#ifndef ETH_TX_BUF_SIZE
 #define ETH_TX_BUF_SIZE         ETH_MAX_PACKET_SIZE
#endif

/* Here we configure each Ethernet driver receive buffer to fit the Max size Ethernet
   packet */
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
  uint32_t   Reserved1;
  uint32_t   TimeStampLow;          		/*!< Time Stamp Low value for transmit and receive */
  uint32_t   TimeStampHigh;         		/*!< Time Stamp High value for transmit and receive */
  uint8_t	 Buff[ETH_TX_BUF_SIZE] __attribute__ ((aligned (4)));

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
  * @brief  Bit definition of TDES0 register: DMA Tx descriptor status register
  */
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

/**
  * @brief  Bit definition of TDES1 register
  */
#define ETH_DMATXDESC_TBS2  ((uint32_t)0x1FFF0000)  /*!< Transmit Buffer2 Size */
#define ETH_DMATXDESC_TBS1  ((uint32_t)0x00001FFF)  /*!< Transmit Buffer1 Size */

/**
  * @brief  Bit definition of TDES2 register
  */
#define ETH_DMATXDESC_B1AP  ((uint32_t)0xFFFFFFFF)  /*!< Buffer1 Address Pointer */

/**
  * @brief  Bit definition of TDES3 register
  */
#define ETH_DMATXDESC_B2AP  ((uint32_t)0xFFFFFFFF)  /*!< Buffer2 Address Pointer */

  /*---------------------------------------------------------------------------------------------
  TDES6 |                         Transmit Time Stamp Low [31:0]                                 |
  -----------------------------------------------------------------------------------------------
  TDES7 |                         Transmit Time Stamp High [31:0]                                |
  ----------------------------------------------------------------------------------------------*/

/* Bit definition of TDES6 register */
 #define ETH_DMAPTPTXDESC_TTSL  ((uint32_t)0xFFFFFFFF)  /* Transmit Time Stamp Low */

/* Bit definition of TDES7 register */
 #define ETH_DMAPTPTXDESC_TTSH  ((uint32_t)0xFFFFFFFF)  /* Transmit Time Stamp High */


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
  * @brief  Bit definition of RDES0 register: DMA Rx descriptor status register
  */
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

/**
  * @brief  Bit definition of RDES1 register
  */
#define ETH_DMARXDESC_DIC   0x80000000  //!< Disable Interrupt on Completion
#define ETH_DMARXDESC_RBS2  0x1FFF0000  //!< Receive Buffer2 Size
#define ETH_DMARXDESC_RER   0x00008000  //!< Receive End of Ring
#define ETH_DMARXDESC_RCH   0x00004000  //!< Second Address Chained
#define ETH_DMARXDESC_RBS1  0x00001FFF  //!< Receive Buffer1 Size

/**
  * @brief  Bit definition of RDES2 register
  */
#define ETH_DMARXDESC_B1AP  ((uint32_t)0xFFFFFFFF)  /*!< Buffer1 Address Pointer */

/**
  * @brief  Bit definition of RDES3 register
  */
#define ETH_DMARXDESC_B2AP  ((uint32_t)0xFFFFFFFF)  /*!< Buffer2 Address Pointer */

/*---------------------------------------------------------------------------------------------------------------------
  RDES4 |                   Reserved[31:15]              |             Extended Status [14:0]                          |
  ---------------------------------------------------------------------------------------------------------------------
  RDES5 |                                            Reserved[31:0]                                                    |
  ---------------------------------------------------------------------------------------------------------------------
  RDES6 |                                       Receive Time Stamp Low [31:0]                                          |
  ---------------------------------------------------------------------------------------------------------------------
  RDES7 |                                       Receive Time Stamp High [31:0]                                         |
  --------------------------------------------------------------------------------------------------------------------*/

/* Bit definition of RDES4 register */
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

/* Bit definition of RDES6 register */
#define ETH_DMAPTPRXDESC_RTSL  ((uint32_t)0xFFFFFFFF)  /* Receive Time Stamp Low */

/* Bit definition of RDES7 register */
#define ETH_DMAPTPRXDESC_RTSH  ((uint32_t)0xFFFFFFFF)  /* Receive Time Stamp High */


/// ETH MAC configuration
#define MAC_CFG_FLG_IFCE_MII_RMII		0x01
#define MAC_CFG_FLG_AUTONEGOTIATION		0x02
#define MAC_CFG_FLG_DUPLEX_MODE			0x04	//!< 1-Full duplex
#define MAC_CFG_FLG_100M				0x08	//!< Speed 100Mbit
#define MAC_CFG_FLG_MCO1				0x10	//!< MCO1 used for ref clk
#define MAC_CFG_FLG_MCO2				0x20	//!< MCO2 used for ref clk

typedef struct
{
	uint32_t mac_cfg;			//!< MAC_CFG_FLG_XXX configuration flags
	ETH_DMADescTypeDef *TxDesc;	//!< First DMA Tx Descriptor
	ETH_DMADescTypeDef *RxDesc;	//!< First DMA Rx Descriptor
	uint32_t rx_len;			//!< Received frame length
	uint8_t  mac_adr[6];		//!< MAC address
} eth_mac_data_t;

typedef struct
{
	uint32_t mac_cfg_flags;		//!< MAC_CFG_FLG_XXX configuration flags

	eth_mac_data_t* mac_data;

	/* Set the WD bit according to ETH Watchdog value */
	/* Set the JD: bit according to ETH Jabber value */
	/* Set the IFG bit according to ETH InterFrameGap value */
	/* Set the DCRS bit according to ETH CarrierSense value */
	/* Set the FES bit according to --- mac_data ---- */
	/* Set the DO bit according to ETH ReceiveOwn value */
	/* Set the LM bit according to ETH LoopbackMode value */
	/* Set the DM bit according to --- mac_data ---- */
	/* Set the IPCO bit according to ETH ChecksumOffload value */
	/* Set the DR bit according to ETH RetryTransmission value */
	/* Set the ACS bit according to ETH AutomaticPadCRCStrip value */
	/* Set the BL bit according to ETH BackOffLimit value */
	/* Set the DC bit according to ETH DeferralCheck value */
	uint32_t ETH_MACCR;

	/* Set the RA bit according to ETH ReceiveAll value */
	/* Set the SAF and SAIF bits according to ETH SourceAddrFilter value */
	/* Set the PCF bit according to ETH PassControlFrames value */
	/* Set the DBF bit according to ETH BroadcastFramesReception value */
	/* Set the DAIF bit according to ETH DestinationAddrFilter value */
	/* Set the PR bit according to ETH PromiscuousMode value */
	/* Set the PM, HMC and HPF bits according to ETH MulticastFramesFilter value */
	/* Set the HUC and HPF bits according to ETH UnicastFramesFilter value */
	uint32_t ETH_MACFFR;

	/* Set the PT bit according to ETH PauseTime value */
	/* Set the DZPQ bit according to ETH ZeroQuantaPause value */
	/* Set the PLT bit according to ETH PauseLowThreshold value */
	/* Set the UP bit according to ETH UnicastPauseFrameDetect value */
	/* Set the RFE bit according to ETH ReceiveFlowControl value */
	/* Set the TFE bit according to ETH TransmitFlowControl value */
	uint32_t ETH_MACFCR;

    /* Set the DT bit according to ETH DropTCPIPChecksumErrorFrame value */
    /* Set the RSF bit according to ETH ReceiveStoreForward value */
    /* Set the DFF bit according to ETH FlushReceivedFrame value */
    /* Set the TSF bit according to ETH TransmitStoreForward value */
    /* Set the TTC bit according to ETH TransmitThresholdControl value */
    /* Set the FEF bit according to ETH ForwardErrorFrames value */
    /* Set the FUF bit according to ETH ForwardUndersizedGoodFrames value */
    /* Set the RTC bit according to ETH ReceiveThresholdControl value */
    /* Set the OSF bit according to ETH SecondFrameOperate value */
	uint32_t ETH_DMAOMR;

    /* Set the AAL bit according to ETH AddressAlignedBeats value */
    /* Set the FB bit according to ETH FixedBurst value */
    /* Set the RPBL and 4*PBL bits according to ETH RxDMABurstLength value */
    /* Set the PBL and 4*PBL bits according to ETH TxDMABurstLength value */
    /* Set the Enhanced DMA descriptors bit according to ETH EnhancedDescriptorFormat value*/
    /* Set the DSL bit according to ETH DesciptorSkipLength value */
    /* Set the PR and DA bits according to ETH DMAArbitration value */
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
RES_CODE HAL_ETH_PHY_INTs(ETH_TypeDef* mac, const eth_mac_cfg_t* cfg);
void HAL_ETH_TransmitFrame(ETH_TypeDef* mac, eth_mac_data_t* mac_data, uint32_t FrameLength);
void HAL_ETH_ReleaseFrame(eth_mac_data_t* mac_data, uint32_t descriptors);
uint32_t HAL_ETH_GetReceivedFrame_IT(const eth_mac_cfg_t* cfg);

#endif /* HARDWARE_STM32_STM32F2_ETH_F2_H_ */
