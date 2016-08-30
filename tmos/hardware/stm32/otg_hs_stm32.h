/*
 * otg_hs_stm32.h
 *
 *  Created on: Nov 30, 2012
 *      Author: miro
 */

#ifndef OTG_HS_STM32_H_
#define OTG_HS_STM32_H_

#include <mcu_inc.h>
#include <tmos_types.h>

#ifndef OTG_MAX_FIFOS
#define OTG_MAX_FIFOS	15	//!< max number of FIFOs
#endif

/*******************************************************************************
 *  USB OTG HS Registers
 ******************************************************************************/

/// Core global registers 0x0000
typedef struct
{
  __IO uint32_t GOTGCTL;      		//!< (otg Offset: 0x0000) USB_OTG Control and Status Register
  __IO uint32_t GOTGINT;      		//!< (otg Offset: 0x0004) USB_OTG Interrupt Register
  __IO uint32_t GAHBCFG;      		//!< (otg Offset: 0x0008) Core AHB Configuration Register
  __IO uint32_t GUSBCFG;      		//!< (otg Offset: 0x000C) Core USB Configuration Register
  __IO uint32_t GRSTCTL;      		//!< (otg Offset: 0x0010) Core Reset Register
  __IO uint32_t GINTSTS;      		//!< (otg Offset: 0x0014) Core Interrupt Register
  __IO uint32_t GINTMSK;      		//!< (otg Offset: 0x0018) Core Interrupt Mask Register
  __IO uint32_t GRXSTSR;      		//!< (otg Offset: 0x001C) Receive Sts Q Read Register
  __IO uint32_t GRXSTSP;      		//!< (otg Offset: 0x0020) Receive Sts Q Read & POP Register
  __IO uint32_t GRXFSIZ;      		//!< (otg Offset: 0x0024) Receive FIFO Size Register
  __IO uint32_t DIEPTXF0_HNPTXFSIZ; //!< (otg Offset: 0x0028) EP0 / Non Periodic Tx FIFO Size Register
  __IO uint32_t GNPTXSTS;   	 	//!< (otg Offset: 0x002C) Non Periodic Tx FIFO/Queue Sts reg
  __IO uint32_t reserved1[2];    	//!< (otg Offset: 0x0030) Reserved
  __IO uint32_t GCCFG;        		//!< (otg Offset: 0x0038) General Purpose IO Register
  __IO uint32_t CID;          		//!< (otg Offset: 0x003C) User ID Register
  __IO uint32_t reserved2[48];   	//!< (otg Offset: 0x0040) Reserved
  __IO uint32_t HPTXFSIZ; 			//!< (otg Offset: 0x0100) Host periodic transmit FIFO size register
  __IO uint32_t DIEPTXF[OTG_MAX_FIFOS];//!< (otg Offset: 0x0104) device IN endpoint transmit FIFO size register
} OTG_CORE_REGS;


/// Host-mode registers 0x0400
typedef struct
{
  __IO uint32_t HCFG;             	//!< (otg Offset: 0x0400) Host Configuration Register
  __IO uint32_t HFIR;      			//!< (otg Offset: 0x0404) Host Frame Interval Register
  __IO uint32_t HFNUM;         		//!< (otg Offset: 0x0408) Host Frame Nbr/Frame Remaining
  __IO uint32_t reserved; 	        //!< (otg Offset: 0x040C) Reserved
  __IO uint32_t HPTXSTS;   			//!< (otg Offset: 0x0410) Host Periodic Tx FIFO/ Queue Status
  __IO uint32_t HAINT;   			//!< (otg Offset: 0x0414) Host All Channels Interrupt Register
  __IO uint32_t HAINTMSK;   		//!< (otg Offset: 0x0418) Host All Channels Interrupt Mask
} OTG_HOST_REGS;


/// Host channel-specific registers  0x0500, 0x0520... +x*0x20
typedef struct
{
  __IO uint32_t HCCHAR;   			//!< (otg Offset: 0x0500+) Host channel-x characteristics register
  __IO uint32_t HCSPLT;             //!< (otg Offset: 0x0504+) host channel-x split control register
  __IO uint32_t HCINT;              //!< (otg Offset: 0x0508+) Host channel-x interrupt register
  __IO uint32_t HCINTMSK;           //!< (otg Offset: 0x050C+) Host channel-x interrupt mask register
  __IO uint32_t HCTSIZ;             //!< (otg Offset: 0x0510+) Host channel-x transfer size register
  __IO uint32_t HCDMA;              //!< (otg Offset: 0x0514+) host channel-x DMA address register
  __IO uint32_t reserved[2];        //!< (otg Offset: 0x051C+) reserved
} OTG_HC_REGS;


/// Device mode CSRs 0x0800
typedef struct
{
  __IO uint32_t DCFG;         		//!< (otg Offset: 0x0800) device Configuration Register
  __IO uint32_t DCTL;         		//!< (otg Offset: 0x0804) device Control Register
  __IO uint32_t DSTS;         		//!< (otg Offset: 0x0808) device Status Register (RO)
  __IO uint32_t reserved1;          //!< (otg Offset: 0x080C) Reserved
  __IO uint32_t DIEPMSK;   			//!< (otg Offset: 0x0810) device IN Endpoint Mask
  __IO uint32_t DOEPMSK;  			//!< (otg Offset: 0x0814) device OUT Endpoint Mask
  __IO uint32_t DAINT;     			//!< (otg Offset: 0x0818) device All Endpoints Itr Reg
  __IO uint32_t DAINTMSK; 			//!< (otg Offset: 0x081C) device All Endpoints Itr Mask
  __IO uint32_t reserved2[2];       //!< (otg Offset: 0x0820) Reserved
  __IO uint32_t DVBUSDIS;    		//!< (otg Offset: 0x0828) device VBUS discharge Register
  __IO uint32_t DVBUSPULSE;  		//!< (otg Offset: 0x082C) device VBUS Pulse Register
  __IO uint32_t DTHRCTL;     		//!< (otg Offset: 0x0830) device thr
  __IO uint32_t DIEPEMPMSK; 		//!< (otg Offset: 0x0834) device empty msk
  __IO uint32_t DEACHINT;    		//!< (otg Offset: 0x0838) dedicated EP interrupt
  __IO uint32_t DEACHINTMSK;    	//!< (otg Offset: 0x083C) dedicated EP msk
  __IO uint32_t DIEPEACHMSK1;      	//!< (otg Offset: 0x0840) dedicated in EP-1 interrupt
  __IO uint32_t DINEP1MSK;  		//!< (otg Offset: 0x0844) dedicated EP mask (as DIEPMSK)
  __IO uint32_t reserved3[14];    	//!< (otg Offset: 0x0848) Reserved
  __IO uint32_t DOEPEACHMSK1; 		//!< (otg Offset: 0x0880) device each OUT endpoint-1 interrupt register
  __IO uint32_t DOUTEP1MSK; 		//!< (otg Offset: 0x0884) dedicated EP msk (as DOEPMSK)
} OTG_DEVICE_REGS;


/// Device IN Endpoint CSRs 0x0900, 0x0920... + (ep_num * 0x20)
typedef struct
{
  __IO uint32_t DIEPCTL; 			//!< (otg Offset: 0x0900+) dev IN Endpoint Control Reg
  __IO uint32_t reserved1;          //!< (otg Offset: 0x0904+) Reserved
  __IO uint32_t DIEPINT; 			//!< (otg Offset: 0x0908+) dev IN Endpoint Itr Reg
  __IO uint32_t reserved2;          //!< (otg Offset: 0x090C+) Reserved
  __IO uint32_t DIEPTSIZ; 			//!< (otg Offset: 0x0910+) IN Endpoint Txfer Size
  __IO uint32_t DIEPDMA; 			//!< (otg Offset: 0x0914+) IN Endpoint DMA Address Reg
  __IO uint32_t DTXFSTS;			//!< (otg Offset: 0x0918+) IN Endpoint Tx FIFO Status Reg
  __IO uint32_t reserved3;          //!< (otg Offset: 0x091C+) Reserved
} OTG_IN_EPT_REGS;


/// Device OUT Endpoint CSRs 0x0B00, 0x0B20... + (ep_num * 0x20)
typedef struct
{
  __IO uint32_t DOEPCTL;       		//!< (otg Offset: 0x0B00+) dev OUT Endpoint Control Reg
  __IO uint32_t reserved1;          //!< (otg Offset: 0x0B04+) Reserved
  __IO uint32_t DOEPINT;       		//!< (otg Offset: 0x0B08+) dev OUT Endpoint Itr Reg
  __IO uint32_t reserved2;          //!< (otg Offset: 0x0B0C+) Reserved
  __IO uint32_t DOEPTSIZ;      		//!< (otg Offset: 0x0B10+) dev OUT Endpoint Txfer Size
  __IO uint32_t DOEPDMA;       		//!< (otg Offset: 0x0B14+) dev OUT Endpoint DMA Address
  __IO uint32_t reserved3[2];       //!< (otg Offset: 0x0B18+) Reserved
} OTG_OUT_EPT_REGS;


/// Data FIFO (DFIFO) access register 0x1000
typedef struct
{
  __IO uint32_t DFIFO[1024];   		//!< (otg Offset: 0x1000+) Data FIFO (DFIFO) access
} OTG_DFIFO_REGS;


typedef struct
{
  OTG_CORE_REGS		core_regs;						//!< (otg Offset: 0x0000) Core global CSRs
  uint32_t			reserved1[191 - OTG_MAX_FIFOS];	//!< (otg Offset: 0x0104+x) reserved
  OTG_HOST_REGS     host_regs;						//!< (otg Offset: 0x0400) Host mode CSRs
  uint32_t			reserved2[9];					//!< (otg Offset: 0x041C) reserved
  __IO uint32_t     HPRT;							//!< (otg Offset: 0x0440) Host port control and status register
  uint32_t			reserved3[47];					//!< (otg Offset: 0x0444) reserved
  OTG_HC_REGS		HC_REGS[OTG_MAX_FIFOS];			//!< (otg Offset: 0x0500) Host Channel CSRs
  uint32_t			reserved4[192- OTG_MAX_FIFOS*8];//!< (otg Offset: 0x0500+x*8) reserved
  OTG_DEVICE_REGS   device_regs;					//!< (otg Offset: 0x0800) Device mode CSRs
  uint32_t			reserved5[30];					//!< (otg Offset: 0x0888) reserved
  OTG_IN_EPT_REGS   in_ept_regs[OTG_MAX_FIFOS];		//!< (otg Offset: 0x0900) device IN endpoint CSRs
  uint32_t			reserved6[128- OTG_MAX_FIFOS*8];//!< (otg Offset: 0x0900+x*8) reserved
  OTG_OUT_EPT_REGS  out_ept_regs[OTG_MAX_FIFOS];	//!< (otg Offset: 0x0B00) device OUT endpoint CSRs
  uint32_t			reserved7[192- OTG_MAX_FIFOS*8];//!< (otg Offset: 0x0B00+x*8) reserved
  __IO uint32_t     PCGCCTL; 						//!< (otg Offset: 0x0E00) power and clock gating control register
  uint32_t			reserved8[127];					//!< (otg Offset: 0x0E04) reserved
  OTG_DFIFO_REGS    DFIFO[OTG_MAX_FIFOS];			//!< (otg Offset: 0x1000) Data FIFO (DFIFO) access registers
} USB_TypeDef;


/* ---------- Core global registers 0x0000  -----------------------------------*/

/** @defgroup GOTGCTL:	(otg Offset: 0x0000) USB_OTG Control and Status Register*/
#define OTG_GOTGCTL_BSVLD           (1u<<19)    //!< B-Session Valid
#define OTG_GOTGCTL_ASVLD           (1u<<18)    //!< A-Session Valid
#define OTG_GOTGCTL_DBCT            (1u<<17)    //!< Long/Short debounce time
#define OTG_GOTGCTL_CIDSTS          (1u<<16)    //!< Connector ID status
#define OTG_GOTGCTL_DHNPEN          (1u<<11)    //!< Device HNP enabled
#define OTG_GOTGCTL_HSHNPEN         (1u<<10)    //!< Host Set HNP enable
#define OTG_GOTGCTL_HNPRQ           (1u<<9)     //!< HNP request
#define OTG_GOTGCTL_HNGSCS          (1u<<8)     //!< Host negotiation success
#define OTG_GOTGCTL_SRQ             (1u<<1)     //!< Session request
#define OTG_GOTGCTL_SRQSCS          (1u<<0)     //!< Session request success
/** @} */

/** @defgroup GOTGINT;  (otg Offset: 0x0004) USB_OTG Interrupt Register			*/
#define OTG_GOTGINT_DBCDNE          (1u<<19)    //!< Debounce done
#define OTG_GOTGINT_ADTOCHG         (1u<<18)    //!< A-Device timeout change
#define OTG_GOTGINT_HNGDET          (1u<<17)    //!< Host negotiation detected
#define OTG_GOTGINT_HNSSCHG         (1u<<9)     //!< Host negotiation success status change
#define OTG_GOTGINT_SRSSCHG         (1u<<8)     //!< Session request success status change
#define OTG_GOTGINT_SEDET           (1u<<2)     //!< Session end detected
/** @} */

/** @defgroup GAHBCFG;  (otg Offset: 0x0008) Core AHB Configuration Register	*/
#define OTG_GAHBCFG_PTXFELVL        (1u<<8)     //!< Periodic TxFIFO empty level
#define OTG_GAHBCFG_TXFELVL         (1u<<7)     //!< Non-periodic TxFIFO empty level
#define OTG_GAHBCFG_DMAEN           (1u<<5)     //!< DMA enable (HS only
#define OTG_GAHBCFG_HBSTLEN_Msk     (15u<<1)    //!< Burst length/type mask (HS only)
#define OTG_GAHBCFG_HBSTLEN(n)      ((n)<<1)    //!< Burst length/type (HS only)
#define OTG_GAHBCFG_GINTMSK         (1u<<0)     //!< Global interrupt mask
/** @} */

/** @defgroup GUSBCFG;  (otg Offset: 0x000C) Core USB Configuration Register	*/
#define OTG_GUSBCFG_CTXPKT          (1u<<31)    //!< Corrupt Tx packet
#define OTG_GUSBCFG_FDMOD           (1u<<30)    //!< Force Device Mode
#define OTG_GUSBCFG_FHMOD           (1u<<29)    //!< Force Host Mode
#define OTG_GUSBCFG_ULPIIPD         (1u<<25)    //!< ULPI interface protect disable
#define OTG_GUSBCFG_PTCI            (1u<<24)    //!< Indicator pass through
#define OTG_GUSBCFG_PCCI            (1u<<23)    //!< Indicator complement
#define OTG_GUSBCFG_TSDPS           (1u<<22)    //!< TermSel DLine pulsing selection
#define OTG_GUSBCFG_ULPIEVBUSI      (1u<<21)    //!< ULPI external VBUS indicator
#define OTG_GUSBCFG_ULPIEVBUSD      (1u<<20)    //!< ULPI External VBUS Drive
#define OTG_GUSBCFG_ULPICSM         (1u<<19)    //!< ULPI Clock SuspendM
#define OTG_GUSBCFG_ULPIAR          (1u<<18)    //!< ULPI Auto-resume
#define OTG_GUSBCFG_ULPIFSLS	    (1u<<17)    //!< ULPI FS/LS select
#define OTG_GUSBCFG_TRDT_Msk        (15u<<10)   //!< USB Turnaround time field mask
#define OTG_GUSBCFG_TRDT(n)         ((n)<<10)   //!< USB Turnaround time field value
#define OTG_GUSBCFG_HNPCAP          (1u<<9)     //!< HNP-Capable
#define OTG_GUSBCFG_SRPCAP          (1u<<8)     //!< SRP-Capable
#define OTG_GUSBCFG_PHYSEL          (1u<<6)     //!< USB 2.0 High-Speed PHY or USB 1.1 Full-Speed serial transceiver Select
#define OTG_GUSBCFG_PHYSEL_ULPI		(0u<<6)     //!<  USB 2.0 high-speed ULPI PHY
#define OTG_GUSBCFG_PHYSEL_FSSER    (1u<<6)     //!<  USB 1.1 full-speed serial transceiver
#define OTG_GUSBCFG_TOCAL_Msk       (7u<<0)     //!< HS/FS timeout calibration field mask
#define OTG_GUSBCFG_TOCAL(n)        ((n)<<0)    //!< HS/FS timeout calibration
/** @} */

/** @defgroup GRSTCTL;  (otg Offset: 0x0010) Core Reset Register				*/
#define OTG_GRSTCTL_AHBIDL          (1u<<31)    //!< AHB Master Idle
#define OTG_GRSTCTL_TXFNUM_Msk      (31u<<6)    //!< TxFIFO number field mask
#define OTG_GRSTCTL_TXFNUM(n)       ((n)<<6)    //!< TxFIFO number field value
#define OTG_GRSTCTL_TXFFLSH         (1u<<5)     //!< TxFIFO flush
#define OTG_GRSTCTL_RXFFLSH         (1u<<4)     //!< RxFIFO flush
#define OTG_GRSTCTL_FCRST           (1u<<2)     //!< Host frame counter reset
#define OTG_GRSTCTL_HSRST           (1u<<1)     //!< HClk soft reset
#define OTG_GRSTCTL_CSRST           (1u<<0)     //!< Core soft reset
/** @} */

/** @defgroup GINTSTS;  (otg Offset: 0x0014) Core Interrupt Register			*/
#define OTG_GINTSTS_WKUPINT         (1u<<31)    //!< Resume/Remote wakeup detected interrupt
#define OTG_GINTSTS_SRQINT          (1u<<30)    //!< Session request/New session detected interrupt
#define OTG_GINTSTS_DISCINT         (1u<<29)    //!< Disconnect detected interrupt
#define OTG_GINTSTS_CIDSCHG         (1u<<28)    //!< Connector ID status change
#define OTG_GINTSTS_PTXFE           (1u<<26)    //!< Periodic TxFIFO empty
#define OTG_GINTSTS_HCINT           (1u<<25)    //!< Host channels interrupt
#define OTG_GINTSTS_HPRTINT         (1u<<24)    //!< Host port interrupt
#define OTG_GINTSTS_IPXFR           (1u<<21)    //!< Incomplete periodic transfer
#define OTG_GINTSTS_IISOOXFR        (1u<<21)    //!< Incomplete isochronous OUT transfer
#define OTG_GINTSTS_IISOIXFR        (1u<<20)    //!< Incomplete isochronous IN transfer
#define OTG_GINTSTS_OEPINT          (1u<<19)    //!< OUT endpoints interrupt
#define OTG_GINTSTS_IEPINT          (1u<<18)    //!< IN endpoints interrupt
#define OTG_GINTSTS_EOPF            (1u<<15)    //!< End of periodic frame interrupt
#define OTG_GINTSTS_ISOODRP         (1u<<14)    //!< Isochronous OUT packet dropped interrupt
#define OTG_GINTSTS_ENUMDNE         (1u<<13)    //!< Enumeration done
#define OTG_GINTSTS_USBRST          (1u<<12)    //!< USB reset
#define OTG_GINTSTS_USBSUSP         (1u<<11)    //!< USB suspend
#define OTG_GINTSTS_ESUSP           (1u<<10)    //!< Early suspend
#define OTG_GINTSTS_GONAKEFF        (1u<<7)     //!< Global OUT NAK effective
#define OTG_GINTSTS_GINAKEFF        (1u<<6)     //!< Global IN non-periodic NAK effective
#define OTG_GINTSTS_NPTXFE          (1u<<5)     //!< Non-periodic TxFIFO empty
#define OTG_GINTSTS_RXFLVL          (1u<<4)     //!< RxFIFO non-empty
#define OTG_GINTSTS_SOF             (1u<<3)     //!< Start of frame
#define OTG_GINTSTS_OTGINT          (1u<<2)     //!< OTG interrupt
#define OTG_GINTSTS_MMIS            (1u<<1)     //!< Mode Mismatch interrupt
#define OTG_GINTSTS_CMOD            (1u<<0)     //!< Current mode of operation
/** @} */

/** @defgroup GINTMSK;  (otg Offset: 0x0018) Core Interrupt Mask Register		*/
#define OTG_GINTMSK_WUIM            (1u<<31)    //!< Resume/remote wakeup detected interrupt mask
#define OTG_GINTMSK_SRQIM           (1u<<30)    //!< Session request/New session detected interrupt mask
#define OTG_GINTMSK_DISCINT         (1u<<29)    //!< Disconnect detected interrupt mask
#define OTG_GINTMSK_CIDSCHGM        (1u<<28)    //!< Connector ID status change mask
#define OTG_GINTMSK_PTXFEM          (1u<<26)    //!< Periodic TxFIFO empty mask
#define OTG_GINTMSK_HCIM            (1u<<25)    //!< Host channels interrupt mask
#define OTG_GINTMSK_PRTIM           (1u<<24)    //!< Host port interrupt mask
#define OTG_GINTMSK_IPXFRM          (1u<<21)    //!<  Host Incomplete periodic transfer mask
#define OTG_GINTMSK_IISOOXFRM       (1u<<21)    //!<  Device Incomplete isochronous OUT transfer mask
#define OTG_GINTMSK_IISOIXFRM       (1u<<20)    //!< Incomplete isochronous IN transfer mask
#define OTG_GINTMSK_OEPINT          (1u<<19)    //!< OUT endpoints interrupt mask
#define OTG_GINTMSK_IEPINT          (1u<<18)    //!< IN endpoints interrupt mask
#define OTG_GINTMSK_EPMISM          (1u<<17)    //!< Endpoint mismatch interrupt mask
#define OTG_GINTMSK_EOPFM           (1u<<15)    //!< End of periodic frame interrupt mask
#define OTG_GINTMSK_ISOODRPM        (1u<<14)    //!< Isochronous OUT packet dropped interrupt mask
#define OTG_GINTMSK_ENUMDNEM        (1u<<13)    //!< Enumeration done mask
#define OTG_GINTMSK_USBRST          (1u<<12)    //!< USB reset mask
#define OTG_GINTMSK_USBSUSPM        (1u<<11)    //!< USB suspend mask
#define OTG_GINTMSK_ESUSPM          (1u<<10)    //!< Early suspend mask
#define OTG_GINTMSK_GONAKEFFM       (1u<<7)     //!< Global OUT NAK effective mask
#define OTG_GINTMSK_GINAKEFFM       (1u<<6)     //!< Global non-periodic IN NAK effective mask
#define OTG_GINTMSK_NPTXFEM         (1u<<5)     //!< Non-periodic TxFIFO empty mask
#define OTG_GINTMSK_RXFLVLM         (1u<<4)     //!< Receive FIFO non-empty mask
#define OTG_GINTMSK_SOFM            (1u<<3)     //!< Start of (micro)frame mask
#define OTG_GINTMSK_OTGINT          (1u<<2)     //!< OTG interrupt mask
#define OTG_GINTMSK_MMISM           (1u<<1)     //!< Mode Mismatch interrupt
/** @} */

/** @defgroup GRXSTSR;  (otg Offset: 0x001C) Receive Sts Q Read Register		*/
#define OTG_GRXSTSR_PKTSTS_Msk      (15u<<17)   //!< Packet status mask
#define OTG_GRXSTSR_PKTSTS(n)       ((n)<<17)   //!< Packet status value
#define OTG_GRXSTSR_OUT_GLOBAL_NAK  GRXSTSR_PKTSTS(1)
#define OTG_GRXSTSR_OUT_DATA        GRXSTSR_PKTSTS(2)
#define OTG_GRXSTSR_OUT_COMP        GRXSTSR_PKTSTS(3)
#define OTG_GRXSTSR_SETUP_COMP      GRXSTSR_PKTSTS(4)
#define OTG_GRXSTSR_SETUP_DATA      GRXSTSR_PKTSTS(6)
#define OTG_GRXSTSR_DPID_Msk        (3u<<15)    //!< Data PID mask
#define OTG_GRXSTSR_DPID(n)         ((n)<<15)   //!< Data PID value
#define OTG_GRXSTSR_BCNT_Msk        (0x7FF<<4)  //!< Byte count mask
#define OTG_GRXSTSR_BCNT(n)         ((n)<<4)    //!< Byte count value
#define OTG_GRXSTSR_CHNUM_Msk       (15u<<0)    //!< Channel number mask
#define OTG_GRXSTSR_CHNUM(n)        ((n)<<0)    //!< Channel number value
#define OTG_GRXSTSR_EPNUM_Msk       (15u<<0)    //!< Endpoint number mask
#define OTG_GRXSTSR_EPNUM(n)        ((n)<<0)    //!< Endpoint number value
/** @} */

/** @defgroup GRXSTSP;  (otg Offset: 0x0020) Receive Sts Q Read & POP Register	*/
#define OTG_GRXSTSP_PKTSTS_Msk      (15<<17)    //!< Packet status mask
#define OTG_GRXSTSP_PKTSTS(n)       ((n)<<17)   //!< Packet status value
#define OTG_GRXSTSP_PKTSTS_NAK 		  (1 << 17)	//!<  Dev: Global OUT NAK (triggers an interrupt)
#define OTG_GRXSTSP_PKTSTS_OUT_DATA   (2 << 17)	//!<  Dev: OUT data packet received
#define OTG_GRXSTSP_PKTSTS_OUT_COMP   (3 << 17)	//!<  Dev: OUT transfer completed (triggers an interrupt)
#define OTG_GRXSTSP_PKTSTS_SETUP_COMP (4 << 17)	//!<  Dev: SETUP transaction completed (triggers an interrupt)
#define OTG_GRXSTSP_PKTSTS_SETUP_DATA (6 << 17)	//!<  Dev: SETUP data packet received

#define OTG_GRXSTSP_PKTSTS_IN_DATA    (2 << 17)	//!<  Host: IN data packet received
#define OTG_GRXSTSP_PKTSTS_IN_COMP    (3 << 17)	//!<  Host: IN transfer completed (triggers an interrupt)
#define OTG_GRXSTSP_PKTSTS_TOGGLE_ERR (5 << 17)	//!<  Host: Data toggle error (triggers an interrupt)
#define OTG_GRXSTSP_PKTSTS_EP_HALT	  (7 << 17)	//!<  Host: Channel halted (triggers an interrupt)

#define OTG_GRXSTSP_DPID_Msk        (3u<<15)    //!< Data PID mask
#define OTG_GRXSTSP_DPID(n)         ((n)<<15)   //!< Data PID value
#define OTG_GRXSTSP_BCNT_Msk        (0x7FF<<4)  //!< Byte count mask
#define OTG_GRXSTSP_BCNT_Get(x) (((x)>>4)&0x7FF)//!< Byte count get
#define OTG_GRXSTSP_BCNT(n)         ((n)<<4)    //!< Byte count value
#define OTG_GRXSTSP_CHNUM_Msk       (15u<<0)    //!< Channel number mask
#define OTG_GRXSTSP_CHNUM(n)        ((n)<<0)    //!< Channel number value
#define OTG_GRXSTSP_EPNUM_Msk       (15u<<0)    //!< Endpoint number mask
#define OTG_GRXSTSP_EPNUM(n)        ((n)<<0)    //!< Endpoint number value
/** @} */

/** @defgroup GRXFSIZ;  (otg Offset: 0x0024) Receive FIFO Size Register			*/
#define OTG_GRXFSIZ_RXFD_Msk        (0xFFFF<<0) //!< RxFIFO depth mask
#define OTG_GRXFSIZ_RXFD(n)         ((n)<<0)    //!< RxFIFO depth value
/** @} */

/** @defgroup DIEPTXF0_HNPTXFSIZ; (otg Offset: 0x0028) EP0 / Non Periodic Tx FIFO Size Register*/
#define OTG_DIEPTXF0_TX0FD_Msk     (0xFFFFu<<16)//!< Device Endpoint 0 TxFIFO depth mask
#define OTG_DIEPTXF0_TX0FD(n)       ((n)<<16)   //!< Device Endpoint 0 TxFIFO dept value
#define OTG_DIEPTXF0_TX0FSA_Msk     (0xFFFF<<0) //!< Device Endpoint 0 transmit RAM start address mask
#define OTG_DIEPTXF0_TX0FSA(n)      ((n)<<0)    //!< Device Endpoint 0 transmit RAM start address

#define OTG_HNPTXFSIZ_NPTXFD_Msk   (0xFFFFu<<16)//!< Host Non-periodic TxFIFO depth mask
#define OTG_HNPTXFSIZ_NPTXFD(n)     ((n)<<16)   //!< Host Non-periodic TxFIFO depth value
#define OTG_HNPTXFSIZ_NPTXFSA_Msk   (0xFFFF<<0) //!< Host Non-periodic transmit RAM start address mask
#define OTG_HNPTXFSIZ_NPTXFSA(n)    ((n)<<0)    //!< Host Non-periodic transmit RAM start address
/** @} */

/** @defgroup GNPTXSTS; (otg Offset: 0x002C) Non Periodic Tx FIFO/Queue Sts reg	*/
#define OTG_GNPTXSTS_NPTXQTOP_Get(x) (((x)>>24) & 0x7Fu) //!< Top of the nonperiodic transmit request queue get
#define OTG_GNPTXSTS_NPTXQTOP_CH_Get(x) (((x)>>27) & 0xFu) //!< Top of the nonperiodic transmit request queue channel get
#define OTG_GNPTXSTS_NPTQXSAV_Msk   (0xFFu<<16) //!< Nonperiodic transmit request queue space available mask
#define OTG_GNPTXSTS_NPTQXSAV_Get(x) (((x)>>16) & 0xFFu) //!< Nonperiodic transmit request queue space available get
#define OTG_GNPTXSTS_NPTXFSAV_Msk   (0xFFFF)	//!< Nonperiodic TxFIFO space available mask
#define OTG_GNPTXSTS_NPTXFSAV(n)    (n)			//!< Nonperiodic TxFIFO space available value
/** @} */

/** @defgroup GCCFG;    (otg Offset: 0x0038) General Purpose IO Register		*/
#define OTG_GCCFG_NOVBUSSENS        (1u<<21)    //!< VBUS sensing disable
#define OTG_GCCFG_SOFOUTEN          (1u<<20)    //!< SOF output enable
#define OTG_GCCFG_VBUSBSEN          (1u<<19)    //!< Enable the VBUS sensing "B" device
#define OTG_GCCFG_VBUSASEN          (1u<<18)    //!< Enable the VBUS sensing "A" device
#define OTG_GCCFG_PWRDWN            (1u<<16)    //!< Power down
/** @} */

/** @defgroup CID;      (otg Offset: 0x003C) User ID Register					*/
/** @} */

/** @defgroup HPTXFSIZ; (otg Offset: 0x0100) Host periodic transmit FIFO size register */
#define OTG_HPTXFSIZ_PTXFD_Msk     (0xFFFFu<<16)//!< Host periodic TxFIFO depth mask
#define OTG_HPTXFSIZ_PTXFD(n)       ((n)<<16)   //!< Host periodic TxFIFO depth value
#define OTG_HPTXFSIZ_PTXSA_Msk      (0xFFFFu<<0)//!< Host periodic TxFIFO Start address mask
#define OTG_HPTXFSIZ_PTXSA(n)       ((n)<<0)    //!< Host periodic TxFIFO start address value
/** @} */

/** @defgroup DIEPTXF[OTG_MAX_FIFOS]; (otg Offset: 0x0104) device IN endpoint transmit FIFO size register */
/** @} */



/* ---------- Host-mode registers 0x0400 --------------------------------------*/

/** @defgroup HCFG; 	(otg Offset: 0x0400) Host Configuration Register		*/
#define OTG_HCFG_FSLSS              (1u<<2)     //!< FS- and LS-only support
#define OTG_HCFG_FSLSPCS_Msk        (3u<<0)     //!< FS/LS PHY clock select mask
#define OTG_HCFG_FSLSPCS_48         (1u<<0)     //!< PHY clock is running at 48 MHz
#define OTG_HCFG_FSLSPCS_6          (2u<<0)     //!< PHY clock is running at 6 MHz
/** @} */

/** @defgroup HFIR;     (otg Offset: 0x0404) Host Frame Interval Register		*/
#define OTG_HFIR_FRIVL_Msk          (0xFFFFu<<0)//!< Frame interval mask
#define OTG_HFIR_FRIVL(n)           ((n)<<0)    //!< Frame interval value
/** @} */

/** @defgroup HFNUM;    (otg Offset: 0x0408) Host Frame Nbr/Frame Remaining		*/
#define OTG_HFNUM_FTREM_Msk        (0xFFFFu<<16)//!< Frame time Remaining mask
#define OTG_HFNUM_FTREM(n)          ((n)<<16)   //!< Frame time Remaining value
#define OTG_HFNUM_FRNUM_Msk         (0xFFFFu<<0)//!< Frame number mask
#define OTG_HFNUM_FRNUM(n)          ((n)<<0)    //!< Frame number value
/** @} */

/** @defgroup HPTXSTS;  (otg Offset: 0x0410) Host Periodic Tx FIFO/ Queue Status*/
#define OTG_HPTXSTS_PTXQTOP_Msk     (0xFFu<<24) //!< Top of the periodic transmit request queue mask
#define OTG_HPTXSTS_PTXQTOP(n)      ((n)<<24)   //!< Top of the periodictransmit request queue value
#define OTG_HPTXSTS_PTXQSAV_Msk     (0xFF<<16)  //!< Periodic transmit request queue Space Available mask
#define OTG_HPTXSTS_PTXQSAV(n)      ((n)<<16)   //!< Periodic transmit request queue Space Available value
#define OTG_HPTXSTS_PTXFSAVL_Msk    (0xFFFF<<0) //!< Periodic transmit Data FIFO Space Available mask
#define OTG_HPTXSTS_PTXFSAVL(n)     ((n)<<0)    //!< Periodic transmit Data FIFO Space Available value
/** @} */

/** @defgroup HAINT;   	(otg Offset: 0x0414) Host All Channels Interrupt Register*/
#define OTG_HAINT_HAINT_Msk         (0xFFFFu<<0)//!< Channel interrupts mask
#define OTG_HAINT_HAINT(n)          ((n)<<0)    //!< Channel interrupts value
/** @} */

/** @defgroup HAINTMSK; (otg Offset: 0x0418) Host All Channels Interrupt Mask	*/
#define OTG_HAINTMSK_HAINTM_Msk     (0xFFFFu<<0)//!< Channel interrupt mask mask
#define OTG_HAINTMSK_HAINTM(n)      ((n)<<0)    //!< Channel interrupt mask
/** @} */


/* ---------- Host port control and status register  0x0440  ------------------*/
/** @defgroup HPRT;		(otg Offset: 0x0440) Host port control and status register */
#define OTG_HPRT_PSPD_Msk           (3u<<17)    //!< Port speed mask
#define OTG_HPRT_PSPD_FS            (1u<<17)    //!< Full speed value
#define OTG_HPRT_PSPD_LS            (2u<<17)    //!< Low speed value
#define OTG_HPRT_PTCTL_Msk          (15<<13)    //!< Port Test control mask
#define OTG_HPRT_PTCTL(n)           ((n)<<13)   //!< Port Test control value
#define OTG_HPRT_PPWR               (1u<<12)    //!< Port power
#define OTG_HPRT_PLSTS_Msk          (3u<<11)    //!< Port Line status mask
#define OTG_HPRT_PLSTS_DM           (1u<<11)    //!< Logic level of D-
#define OTG_HPRT_PLSTS_DP           (1u<<10)    //!< Logic level of D+
#define OTG_HPRT_PRST               (1u<<8)     //!< Port reset
#define OTG_HPRT_PSUSP              (1u<<7)     //!< Port suspend
#define OTG_HPRT_PRES               (1u<<6)     //!< Port Resume
#define OTG_HPRT_POCCHNG            (1u<<5)     //!< Port overcurrent change
#define OTG_HPRT_POCA               (1u<<4)     //!< Port overcurrent active
#define OTG_HPRT_PENCHNG            (1u<<3)     //!< Port enable/disable change
#define OTG_HPRT_PENA               (1u<<2)     //!< Port enable
#define OTG_HPRT_PCDET              (1u<<1)     //!< Port Connect detected
#define OTG_HPRT_PCSTS              (1u<<0)     //!< Port connect status
#define OTG_HPRT_rc_w1_bits         (OTG_HPRT_PENA    | OTG_HPRT_PCDET |\
	    OTG_HPRT_PENCHNG | OTG_HPRT_POCCHNG)     //!< rc_w1 bits (PENA is rc_w0 according to the RM v.6 but more likely is not)
/** @} */


/* ---------- Host channel-specific registers  0x0500, 0x0520... --------------*/

/** @defgroup HCCHAR;   (otg Offset: 0x0500+) Host channel-x characteristics register */
#define OTG_HCCHAR_CHENA            (1u<<31)    //!< Channel enable
#define OTG_HCCHAR_CHDIS            (1u<<30)    //!< Channel Disable
#define OTG_HCCHAR_ODDFRM           (1u<<29)    //!< Odd frame
#define OTG_HCCHAR_DAD_Msk          (0x7Fu<<22) //!< Device Address mask
#define OTG_HCCHAR_DAD(n)           ((n)<<22)   //!< Device Address value
#define OTG_HCCHAR_MCNT_Msk         (3u<<20)    //!< Multicount mask
#define OTG_HCCHAR_MCNT(n)          ((n)<<20)   //!< Multicount value
#define OTG_HCCHAR_EPTYP_Msk        (3u<<18)    //!< Endpoint type mask
#define OTG_HCCHAR_EPTYP(n)         ((n)<<18)   //!< Endpoint type value
#define OTG_HCCHAR_EPTYP_CTL        (0u<<18)    //!< Control endpoint value
#define OTG_HCCHAR_EPTYP_ISO        (1u<<18)    //!< Isochronous endpoint value
#define OTG_HCCHAR_EPTYP_BULK       (2u<<18)    //!< Bulk endpoint value
#define OTG_HCCHAR_EPTYP_INTR       (3u<<18)    //!< Interrupt endpoint value
#define OTG_HCCHAR_LSDEV            (1u<<17)    //!< Low-Speed device
#define OTG_HCCHAR_EPDIR            (1u<<15)    //!< Endpoint direction
#define OTG_HCCHAR_EPNUM_Msk        (15u<<11)   //!< Endpoint number mask
#define OTG_HCCHAR_EPNUM(n)         ((n)<<11)   //!< Endpoint number value
#define OTG_HCCHAR_MPS_Msk          (11u<<0)    //!< Maximum packet size mask
#define OTG_HCCHAR_MPS(n)           ((n)<<0)    //!< Maximum packet size value
/** @} */

/** @defgroup HCSPLT;   (otg Offset: 0x0504+) host channel-x split control register */
/** @} */

/** @defgroup HCINT;    (otg Offset: 0x0508+) Host channel-x interrupt register */
#define OTG_HCINT_DTERR             (1u<<10)    //!< Data toggle error
#define OTG_HCINT_FRMOR             (1u<<9)     //!< Frame overrun
#define OTG_HCINT_BBERR             (1u<<8)     //!< Babble error
#define OTG_HCINT_TRERR             (1u<<7)     //!< Transaction Error
#define OTG_HCINT_NYET              (1u<<6)     //!< Response received
#define OTG_HCINT_ACK               (1u<<5)     //!< ACK response received/transmitted interrupt
#define OTG_HCINT_NAK               (1u<<4)     //!< NAK response received interrupt
#define OTG_HCINT_STALL             (1u<<3)     //!< STALL response received interrupt
#define OTG_HCINT_AHBERR            (1u<<2)     //!< AHB error
#define OTG_HCINT_CHH               (1u<<1)     //!< Channel halted
#define OTG_HCINT_XFRC              (1u<<0)     //!< Transfer completed
/** @} */

/** @defgroup HCINTMSK; (otg Offset: 0x050C+) Host channel-x interrupt mask register */
#define OTG_HCINTMSK_DTERRM         (1u<<10)    //!< Data toggle error mask
#define OTG_HCINTMSK_FRMORM         (1u<<9)     //!< Frame overrun mask
#define OTG_HCINTMSK_BBERRM         (1u<<8)     //!< Babble error mask
#define OTG_HCINTMSK_TRERRM         (1u<<7)     //!< Transaction error mask
#define OTG_HCINTMSK_NYET           (1u<<6)     //!< NYET response received interrupt mask
#define OTG_HCINTMSK_ACKM           (1u<<5)     //!< ACK Response received/transmitted interrupt mask
#define OTG_HCINTMSK_NAKM           (1u<<4)     //!< NAK response received interrupt mask
#define OTG_HCINTMSK_STALLM         (1u<<3)     //!< STALL response received interrupt mask
#define OTG_HCINTMSK_CHHM           (1u<<1)     //!< Channel halted mask
#define OTG_HCINTMSK_XFRCM          (1u<<0)     //!< Transfer completed mask
/** @} */

/** @defgroup HCTSIZ;   (otg Offset: 0x0510+) Host channel-x transfer size register */
#define OTG_HCTSIZ_DPID_Msk         (3u<<29)    //!< PID mask
#define OTG_HCTSIZ_DPID_DATA0       (0u<<29)    //!< DATA0
#define OTG_HCTSIZ_DPID_DATA2       (1u<<29)    //!< DATA2
#define OTG_HCTSIZ_DPID_DATA1       (2u<<29)    //!< DATA1
#define OTG_HCTSIZ_DPID_MDATA       (3u<<29)    //!< MDATA
#define OTG_HCTSIZ_PKTCNT_Msk       (0x3FFu<<19)//!< Packet count mask
#define OTG_HCTSIZ_PKTCNT(n)        ((n)<<19)   //!< Packet count value
#define OTG_HCTSIZ_XFRSIZ_Msk       (0x7FFFF<<0)//!< Transfer size mask
#define OTG_HCTSIZ_XFRSIZ(n)        ((n)<<0)    //!< Transfer size value
/** @} */

/** @defgroup HCDMA;    (otg Offset: 0x0514+) host channel-x DMA address register */
/** @} */



/* ---------- Device mode CSRs 0x0800 -----------------------------------------*/

/** @defgroup DCFG;     (otg Offset: 0x0800) device Configuration Register		*/
#define OTG_DCFG_PFIVL_Msk          (3u<<11)    //!< Periodic frame interval mask
#define OTG_DCFG_PFIVL(n)           ((n)<<11)   //!< Periodic frame interval value
#define OTG_DCFG_PFIVL_80      		(0u<<11)    //!<  80% of the frame interval
#define OTG_DCFG_PFIVL_85       	(1u<<11)    //!<  85% of the frame interval
#define OTG_DCFG_PFIVL_90   	    (1u<<11)    //!<  80% of the frame interval
#define OTG_DCFG_PFIVL_95	        (3u<<11)    //!<  95% of the frame interval
#define OTG_DCFG_DAD_Msk            (0x7Fu<<4)  //!< Device address mask
#define OTG_DCFG_DAD(n)             ((n)<<4)    //!< Device address value
#define OTG_DCFG_NZLSOHSK           (1u<<2)     //!< Non-Zero-Length status OUT handshake
#define OTG_DCFG_DSPD_Msk	        (3u<<0)     //!< Device speed mask
#define OTG_DCFG_DSPD_HS            (0u<<0)     //!< High speed (USB 2.0)
#define OTG_DCFG_DSPD_HS_FS         (1u<<0)     //!< High speed (USB 2.0) in FS mode
#define OTG_DCFG_DSPD_FS11          (3u<<0)     //!< Full speed (USB 1.1 transceiver clock is 48 MHz)
/** @} */

/** @defgroup DCTL;     (otg Offset: 0x0804) device Control Register			*/
#define OTG_DCTL_POPRGDNE           (1u<<11)    //!< Power-on programming done
#define OTG_DCTL_CGONAK             (1u<<10)    //!< Clear global OUT NAK
#define OTG_DCTL_SGONAK             (1u<<9)     //!< Set global OUT NAK
#define OTG_DCTL_CGINAK             (1u<<8)     //!< Clear global non-periodic IN NAK
#define OTG_DCTL_SGINAK             (1u<<7)     //!< Set global non-periodic IN NAK
#define OTG_DCTL_TCTL_Msk           (7u<<4)     //!< Test control mask
#define OTG_DCTL_TCTL(n)            ((n)<<4     //!< Test control value
#define OTG_DCTL_GONSTS             (1u<<3)     //!< Global OUT NAK status
#define OTG_DCTL_GINSTS             (1u<<2)     //!< Global non-periodic IN NAK status
#define OTG_DCTL_SDIS               (1u<<1)     //!< Soft disconnect
#define OTG_DCTL_RWUSIG             (1u<<0)     //!< Remote wakeup signaling
/** @} */

/** @defgroup DSTS;     (otg Offset: 0x0808) device Status Register (RO)		*/
#define OTG_DSTS_FNSOF_Msk          (0x3FFu<<8) //!< Frame number of the received SOF mask
#define OTG_DSTS_FNSOF(n)           ((n)<<8)    //!< Frame number of the received SOF value
#define OTG_DSTS_FNSOF_ODD          ((1)<<8)    //!<  Frame number of the received SOF value
#define OTG_DSTS_EERR               (1u<<3)     //!< Erratic error
#define OTG_DSTS_ENUMSPD_Msk        (3u<<1)     //!< Enumerated speed mask
#define OTG_DSTS_ENUMSPD_HS_30_60   (0u<<1)     //!<  High speed (PHY is running at 30 or 60 MHz)
#define OTG_DSTS_ENUMSPD_FS_30_60   (1u<<1)     //!<  Full speed (PHY is running at 30 or 60 MHz)
#define OTG_DSTS_ENUMSPD_LS_6       (2u<<1)     //!<  Low speed (PHY is running at 6 MHz)
#define OTG_DSTS_ENUMSPD_FS_48      (3u<<1)     //!<  Full speed (PHY is running at 48 MHz)
#define OTG_DSTS_SUSPSTS            (1u<<0)     //!< Suspend status
/** @} */

/** @defgroup DIEPMSK;  (otg Offset: 0x0810) device IN Endpoint Mask			*/
#define OTG_DIEPMSK_BIM             (1u<<9)     //!< BNA interrupt mask
#define OTG_DIEPMSK_TXFURM          (1u<<8)     //!< FIFO underrun mask
#define OTG_DIEPMSK_TXFEM           (1u<<6)     //!< Transmit FIFO empty mask
#define OTG_DIEPMSK_INEPNEM         (1u<<6)     //!< IN endpoint NAK effective mask
#define OTG_DIEPMSK_ITTXFEMSK       (1u<<4)     //!< IN token received when TxFIFO empty mask
#define OTG_DIEPMSK_TOCM            (1u<<3)     //!< Timeout condition mask
#define OTG_DIEPMSK_EPDM            (1u<<1)     //!< Endpoint disabled interrupt mask
#define OTG_DIEPMSK_XFRCM           (1u<<0)     //!< Transfer completed
/** @} */

/** @defgroup DOEPMSK;  (otg Offset: 0x0814) device OUT Endpoint Mask			*/
#define OTG_DOEPMSK_BOIM            (1u<<9)     //!< BNA interrupt mask
#define OTG_DOEPMSK_OPEM            (1u<<8)     //!< OUT packet error mask
#define OTG_DOEPMSK_B2BSTUP         (1u<<6)     //!< Back-to-back SETUP packets received mask
#define OTG_DOEPMSK_OTEPDM          (1u<<4)     //!< OUT token received when endpoint disabled mask
#define OTG_DOEPMSK_STUPM           (1u<<3)     //!< SETUP phase done mask
#define OTG_DOEPMSK_EPDM            (1u<<1)     //!< Endpoint disabled interrupt mask
#define OTG_DOEPMSK_XFRCM           (1u<<0)     //!< Transfer completed interrupt mask
/** @} */

/** @defgroup DAINT;    (otg Offset: 0x0818) device All Endpoints Itr Reg		*/
#define OTG_DAINT_OEPINT_Msk        (0xFFFFu<<16)//!< OUT endpoint interrupt bits mask
#define OTG_DAINT_OEPINT(n)         ((n)<<16)   //!< OUT endpoint interrupt bits value
#define OTG_DAINT_IEPINT_Msk        (0xFFFFu<<0)//!< IN endpoint interrupt bits mask
#define OTG_DAINT_IEPINT(n)         ((n)<<0)    //!< IN endpoint interrupt bits value
/** @} */

/** @defgroup DAINTMSK; (otg Offset: 0x081C) device All Endpoints Itr Mask		*/
#define OTG_DAINTMSK_OEPM_Msk       (0xFFFFu<<16)//!< OUT EP interrupt mask bits mask
#define OTG_DAINTMSK_OEPM(n)        (1u<<(16+(n)))//!< OUT EP interrupt mask bits value
#define OTG_DAINTMSK_IEPM_Msk       (0xFFFFu<<0)//!< IN EP interrupt mask bits mask
#define OTG_DAINTMSK_IEPM(n)        (1u<<(n))   //!< IN EP interrupt mask bits value
/** @} */

/** @defgroup DVBUSDIS; (otg Offset: 0x0828) device VBUS discharge Register		*/
#define OTG_DVBUSDIS_VBUSDT_Msk     (0xFFFFu<<0)//!< Device VBUS discharge time mask
#define OTG_DVBUSDIS_VBUSDT(n)      ((n)<<0)    //!< Device VBUS discharge time value
/** @} */

/** @defgroup DVBUSPULSE   (otg Offset: 0x082C) device VBUS Pulse Register		*/
#define OTG_DVBUSPULSE_DVBUSP_Msk   (0xFFFu<<0) //!< Device VBUSpulsing time mask
#define OTG_DVBUSPULSE_DVBUSP(n)    ((n)<<0)    //!< Device VBUS pulsing time value
/** @} */

/** @defgroup DTHRCTL;     (otg Offset: 0x0830) device thr						*/
#define OTG_DTHRCTL_ARPEN           (1u<<27)    //!< Arbiter parking enable
#define OTG_DTHRCTL_RXTHRLEN_Msk    (0x1FFu<<17)//!< Receive threshold length mask
#define OTG_DTHRCTL_RXTHRLEN(n)     ((n)<<17)   //!< Receive threshold length set
#define OTG_DTHRCTL_RXTHREN         (1u<<16)    //!< Receive threshold enable
#define OTG_DTHRCTL_TXTHRLEN_Msk    (0x1FFu<<2) //!< Transmit threshold length mask
#define OTG_DTHRCTL_TXTHRLEN(n)     ((n)<<2)    //!< Transmit threshold length set
#define OTG_DTHRCTL_ISOTHREN        (1u<<1)     //!< ISO IN endpoint threshold enable
#define OTG_DTHRCTL_NONISOTHREN     (1u<<0)     //!< Nonisochronous IN endpoints threshold enable
/** @} */

/** @defgroup DIEPEMPMSK;  (otg Offset: 0x0834) device empty msk				*/
#define OTG_DIEPEMPMSK_INEPTXFEM(n) (1u<<(n))   //!< IN EP Tx FIFO empty interrupt mask bit
/** @} */

/** @defgroup DEACHINT;    (otg Offset: 0x0838) dedicated EP interrupt			*/
#define OTG_DEACHINT_OEP1INT        (1u<<17)    //!< OUT endpoint 1 interrupt bit
#define OTG_DEACHINT_IEP1INT        (1u<<1)     //!< IN endpoint 1interrupt bit
/** @} */

/** @defgroup DEACHINTMSK; (otg Offset: 0x083C) dedicated EP msk				*/
#define OTG_DEACHINTMSK_OEP1INTM    (1u<<17)    //!< OUT endpoint 1 interrupt mask bit
#define OTG_DEACHINTMSK_IEP1INT     (1u<<1)     //!< IN endpoint 1interrupt mask bit
/** @} */

/** @defgroup DIEPEACHMSK1;(otg Offset: 0x0840) dedicated in EP-1 interrupt		*/
/** @} */

/** @defgroup DINEP1MSK;   (otg Offset: 0x0844) dedicated EP mask				*/
/** @} */

/** @defgroup DOEPEACHMSK1;(otg Offset: 0x0880) device each OUT endpoint-1 interrupt register */
/** @} */

/** @defgroup DOUTEP1MSK;  (otg Offset: 0x0884) dedicated EP msk				*/
/** @} */



/* ---------- Device IN Endpoint CSRs 0x0900, 0x0920... -----------------------*/

/** @defgroup DIEPCTL: 	(otg Offset: 0x0900+) dev IN Endpoint Control Reg		*/
#define OTG_DIEPCTL_EPENA           (1u<<31)    //!< Endpoint enable
#define OTG_DIEPCTL_EPDIS           (1u<<30)    //!< Endpoint disable
#define OTG_DIEPCTL_SD1PID          (1u<<29)    //!< Set DATA1 PID	(ep 1..7)
#define OTG_DIEPCTL_SODDFRM         (1u<<29)    //!< Set odd frame	(ep 1..7)
#define OTG_DIEPCTL_SD0PID          (1u<<28)    //!< Set DATA0 PID	(ep 1..7)
#define OTG_DIEPCTL_SEVNFRM         (1u<<28)    //!< Set even frame	(ep 1..7)
#define OTG_DIEPCTL_SNAK            (1u<<27)    //!< Set NAK.
#define OTG_DIEPCTL_CNAK            (1u<<26)    //!< Clear NAK
#define OTG_DIEPCTL_TXFNUM_Msk	    (0xFu<<22)  //!< TxFIFO number mask.  (ep 1..7)
#define OTG_DIEPCTL_TXFNUM(n)       ((n)<<22)   //!< TxFIFO number value. (ep 1..7)
#define OTG_DIEPCTL_STALL           (1u<<21)    //!< STALL handshake.
#define OTG_DIEPCTL_SNPM            (1u<<20)    //!< Snoop mode.
#define OTG_DIEPCTL_EPTYP_Msk       (3<<18)     //!< Endpoint type mask.
#define OTG_DIEPCTL_EPTYP(n)        ((n)<<18)   //!<  Endpoint type value.
#define OTG_DIEPCTL_EPTYP_CTRL      (0u<<18)    //!<  Control.
#define OTG_DIEPCTL_EPTYP_ISO       (1u<<18)    //!<  Isochronous.
#define OTG_DIEPCTL_EPTYP_BULK      (2u<<18)    //!<  Bulk.
#define OTG_DIEPCTL_EPTYP_INTR      (3u<<18)    //!<  Interrupt.
#define OTG_DIEPCTL_NAKSTS          (1u<<17)    //!< NAK status.
#define OTG_DIEPCTL_EONUM           (1u<<16)    //!< Even/odd frame.		(ep 1..7)
#define OTG_DIEPCTL_DPID            (1u<<16)    //!< Endpoint data PID.		(ep 1..7)
#define OTG_DIEPCTL_USBAEP          (1u<<15)    //!< USB active endpoint.
#define OTG_DIEPCTL_MPSIZ_Msk       (0x3FFu<<0) //!< Maximum Packet size mask. (0x3 for ep0)
#define OTG_DIEPCTL_MPSIZ(n)        ((n)<<0)    //!< Maximum Packet size value.
/** @} */

/** @defgroup DIEPINT: 	(otg Offset: 0x0908+) dev IN Endpoint Itr Reg			*/
#define OTG_DIEPINT_TXFE            (1u<<7)     //!< Transmit FIFO empty
#define OTG_DIEPINT_INEPNE          (1u<<6)     //!< IN endpoint NAK effective
#define OTG_DIEPINT_ITTXFE          (1u<<4)     //!< IN Token received when TxFIFO is empty
#define OTG_DIEPINT_TOC             (1u<<3)     //!< Timeout condition
#define OTG_DIEPINT_EPDISD          (1u<<1)     //!< Endpoint disabled interrupt
#define OTG_DIEPINT_XFRC            (1u<<0)     //!< Transfer completed
/** @} */

/** @defgroup DIEPTSIZ: (otg Offset: 0x0910+) IN Endpoint Txfer Size			*/
#define OTG_DIEPTSIZ_MCNT_Msk       (3u<<29)  	//!< Multi count mask.
#define OTG_DIEPTSIZ_MCNT(n)        ((n)<<29)   //!< Multi count value.
#define OTG_DIEPTSIZ_PKTCNT_Msk    (0x3FFu<<19) //!< Packet count mask.
#define OTG_DIEPTSIZ_PKTCNT(n)      ((n)<<19)   //!< Packet count value.
#define OTG_DIEPTSIZ_XFRSIZ_Msk   (0x7FFFFu<<0) //!< Transfer size mask.
#define OTG_DIEPTSIZ_XFRSIZ(n)      ((n)<<0)    //!< Transfer size value.
/** @} */

/** @defgroup DTXFSTS:	(otg Offset: 0x0918+) IN Endpoint Tx FIFO Status Reg	*/
#define DTXFSTS_INEPTFSAV_Msk       (0xFFFFu<<0) //!< IN endpoint TxFIFO space  available.                 */
/** @} */


/* ---------- Device OUT Endpoint CSRs 0x0B00, 0x0B20... ----------------------*/

/** @defgroup DOEPCTL:   (otg Offset: 0x0B00+) dev OUT Endpoint Control Reg		*/
#define OTG_DOEPCTL_EPENA           (1u<<31)    //!< Endpoint enable.
#define OTG_DOEPCTL_EPDIS           (1u<<30)    //!< Endpoint disable.
#define OTG_DOEPCTL_SD1PID          (1u<<29)    //!< Set DATA1 PID.
#define OTG_DOEPCTL_SODDFRM         (1u<<29)    //!< Set odd frame.
#define OTG_DOEPCTL_SD0PID         	(1u<<28)    //!< Set DATA0 PID.
#define OTG_DOEPCTL_SEVNFRM         (1u<<28)    //!< Set even frame.
#define OTG_DOEPCTL_SNAK            (1u<<27)    //!< Set NAK.
#define OTG_DOEPCTL_CNAK            (1u<<26)    //!< Clear NAK.
#define OTG_DOEPCTL_STALL           (1u<<21)    //!< STALL handshake.
#define OTG_DOEPCTL_SNPM            (1u<<20)    //!< Snoop mode.
#define OTG_DOEPCTL_EPTYP_Msk	    (3u<<18)    //!< Endpoint type mask.
#define OTG_DOEPCTL_EPTYP(n)	    ((n)<<18)   //!<  Endpoint type value
#define OTG_DOEPCTL_EPTYP_CTRL      (0u<<18)    //!<  Control.
#define OTG_DOEPCTL_EPTYP_ISO       (1u<<18)    //!<  Isochronous.
#define OTG_DOEPCTL_EPTYP_BULK      (2u<<18)    //!<  Bulk.
#define OTG_DOEPCTL_EPTYP_INTR      (3u<<18)    //!<  Interrupt.
#define OTG_DOEPCTL_NAKSTS          (1u<<17)    //!< NAK status.
#define OTG_DOEPCTL_EONUM           (1u<<16)    //!< Even/odd frame.
#define OTG_DOEPCTL_DPID            (1u<<16)    //!< Endpoint data PID.
#define OTG_DOEPCTL_USBAEP          (1u<<15)    //!< USB active endpoint.
#define OTG_DOEPCTL_MPSIZ_Msk       (0x3FFu<<0) //!< Maximum Packet size mask.
#define OTG_DOEPCTL_MPSIZ(n)        ((n)<<0)    //!< Maximum Packet size value.
/** @} */

/** @defgroup DOEPINT:   (otg Offset: 0x0B08+) dev OUT Endpoint Itr Reg			*/
#define OTG_DOEPINT_B2BSTUP         (1u<<6)     //!< Back-to-back SETUP packets received
#define OTG_DOEPINT_OTEPDIS         (1u<<4)     //!< OUT token received when endpoint disabled
#define OTG_DOEPINT_STUP            (1u<<3)     //!< SETUP phase done
#define OTG_DOEPINT_EPDISD          (1u<<1)     //!< Endpoint disabled interrupt
#define OTG_DOEPINT_XFRC            (1u<<0)     //!< Transfer completed interrupt
/** @} */

/** @defgroup DOEPTSIZ:  (otg Offset: 0x0B10+) dev OUT Endpoint Txfer Size		*/
#define OTG_DOEPTSIZ_RXDPID_Msk     (3u<<29)    //!< Received data PID mask.
#define OTG_DOEPTSIZ_RXDPID(n)      ((n)<<29)   //!< Received data PID value.
#define OTG_DOEPTSIZ_STUPCNT_Msk    (3u<<29)    //!< SETUP packet count mask.
#define OTG_DOEPTSIZ_STUPCNT(n)     ((n)<<29)   //!< SETUP packet count value.
#define OTG_DOEPTSIZ_PKTCNT_Msk    (0x3FFu<<19) //!< Packet count mask.
#define OTG_DOEPTSIZ_PKTCNT(n)      ((n)<<19)   //!< Packet count value.
#define OTG_DOEPTSIZ_XFRSIZ_Msk    (0x7FFFFu<<0)//!< Transfer size mask.
#define OTG_DOEPTSIZ_XFRSIZ(n)      ((n)<<0)    //!< Transfer size value.
/** @} */

/** @defgroup DOEPDMA:   (otg Offset: 0x0B14+) dev OUT Endpoint DMA Address   */
/** @} */

/** @defgroup PCGCCTL: 	(otg Offset: 0x0E00) power and clock gating control register */
#define OTG_PCGCCTL_PHYSUSP         (1u<<4)     //!< PHY suspended
#define OTG_PCGCCTL_GATEHCLK        (1u<<1)     //!< Gate HCLK
#define OTG_PCGCCTL_STPPCLK         (1u<<0)     //!< Stop PHY clock
/** @} */

#endif /* OTG_HS_STM32_H_ */
