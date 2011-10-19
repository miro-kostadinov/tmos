/**************************************************************************//**
 * @ingroup	 hardware_lm3s
 * @defgroup lm3s_usb_api USB
 * @{
 * @file     hardware/lm3s/usb_lm3s.h
 * @brief    LM3S USB Header File
 * @version  V3.00
 * @date     30. May 2011
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#ifndef USB_LM3S_H_
#define USB_LM3S_H_


//==============================================================================
// C/C++ definitions
//==============================================================================
#ifndef __ASSEMBLY__
#ifdef __cplusplus
 extern "C" {
#endif

#include <tmos_types.h>

/// Internal struct for USB host device to endpoint mapping
typedef struct
{
    __IO uint8_t  USBTXFUNCADDR;	//!< (off 0x0) USB Transmit Functional Address Endpoint 0-15
         uint8_t  reserved1;		//!< reserved
    __IO uint8_t  USBTXHUBADDR;	   	//!< (off 0x2) USB Transmit Hub Address Endpoint 0-15
    __IO uint8_t  USBTXHUBPORT;	   	//!< (off 0x3) USB Transmit Hub Port Endpoint 0-15
    __IO uint8_t  USBRXFUNCADDR;	//!< (off 0x4) USB Receive Functional Address Endpoint 1-15
         uint8_t  reserved2;		//!< reserved
    __IO uint8_t  USBRXHUBADDR;	   	//!< (off 0x6) USB Receive Hub Address Endpoint 1-15
    __IO uint8_t  USBRXHUBPORT;	   	//!< (off 0x7) USB Receive Hub Port Endpoint 1-15
} USBHOSTFUNS_t;

/// Internal struct for USB device endpoint mapping
typedef struct
{
/*
 * 	NOTE: For unknown reason the datasheet is using different names for endpoint 0
 *	Since the functionality is the same and to make all endpoints to look like
 *	array we use
 *
 *
         uint8_t  reserved1[2];
	__O  uint8_t  USBCSRL0;     	//!< (off 0x2) USB Control and Status Endpoint 0 Low
	__O  uint8_t  USBCSRH0;         //!< (off 0x3) USB Control and Status Endpoint 0 High
		 uint8_t  reserved2[4];
	__I  uint8_t  USBCOUNT0;        //!< (off 0x8) USB Receive Byte Count Endpoint 0
 		 uint8_t  reserved3[1];
	__IO uint8_t  USBTYPE0;         //!< (off 0xA) USB Type Endpoint 0
	__IO uint8_t  USBNAKLMT;        //!< (off 0xB) USB NAK Limit
		 uint8_t  reserved4[4];
*/
	__IO uint16_t USBTXMAXP;  		//!< (off 0x0) USB Maximum Transmit Data Endpoint 1-15
	__IO uint8_t  USBTXCSRL;	   	//!< (off 0x2) USB Transmit Control and Status Endpoint 0-15 Low
	__IO uint8_t  USBTXCSRH;    	//!< (off 0x3) USB Transmit Control and Status Endpoint 0-15 High
	__IO uint16_t USBRXMAXP;		//!< (off 0x4) USB Maximum Receive Data Endpoint 1-15
	__IO uint8_t  USBRXCSRL;     	//!< (off 0x6) USB Receive Control and Status Endpoint 1-15 Low
	__IO uint8_t  USBRXCSRH;		//!< (off 0x7) USB Receive Control and Status Endpoint 1-15 High
	__I  uint16_t USBRXCOUNT;		//!< (off 0x8) USB Receive Byte Count Endpoint 0-15
	__IO uint8_t  USBTXTYPE;		//!< (off 0xA) USB Host Transmit Configure Type Endpoint 0-15
	__IO uint8_t  USBTXINTERVAL;	//!< (off 0xB) USB Host Transmit Interval Endpoint 0-15
	__IO uint8_t  USBRXTYPE;		//!< (off 0xC) USB Host Configure Receive Type Endpoint 1-15
	__IO uint8_t  USBRXINTERVAL;	//!< (off 0xD) USB Host Receive Polling Interval Endpoint 1-15
		uint8_t   reserved[2];		//!< reserved

} USBDEVICEEP_t;

/// Internal struct for USB request packet counters
typedef struct
{
/*
 * 	NOTE: Endpoint 0 has no counter
 *
*/
    __IO uint16_t USBRQPKTCOUNT;   	//!< (off 0x0) USB Request Packet Count in Block Transfer Endpoint 1-15
         uint8_t  reserved[2];		//!< reserved

} USBRPCNT_t;

//=============================================================================
//			       Universal Serial Bus Controller (USB)
//=============================================================================
/** USB controller **/
typedef struct
{
    __IO uint8_t  USBFADDR;			//!< (off 0x000) USB Device Functional Address
    __IO uint8_t  USBPOWER;			//!< (off 0x001) USB Power
    __I  uint16_t USBTXIS;			//!< (off 0x002) USB Transmit Interrupt Status
    __I  uint16_t USBRXIS;			//!< (off 0x004) USB Receive Interrupt Status
    __IO uint16_t USBTXIE;			//!< (off 0x006) USB Transmit Interrupt Enable
    __IO uint16_t USBRXIE;			//!< (off 0x008) USB Receive Interrupt Enable
    __I  uint8_t  USBIS;			//!< (off 0x00A) USB General Interrupt Status
    __IO uint8_t  USBIE;			//!< (off 0x00B) USB Interrupt Enable
    __I  uint16_t USBFRAME;			//!< (off 0x00C) USB Frame Value
    __IO uint8_t  USBEPIDX;			//!< (off 0x00E) USB Endpoint Index
    __IO uint8_t  USBTEST;			//!< (off 0x00F) USB Test Mode
         uint8_t  reserved1[16];	//!< reserved
    __IO uint32_t USBFIFO[16];		//!< (off 0x020) USB FIFO Endpoint 0-15
    __IO uint8_t  USBDEVCTL;		//!< (off 0x060) USB Device Control
         uint8_t  reserved2;		//!< reserved
    __IO uint8_t  USBTXFIFOSZ;		//!< (off 0x062) USB Transmit Dynamic FIFO Sizing
    __IO uint8_t  USBRXFIFOSZ;		//!< (off 0x063) USB Receive Dynamic FIFO Sizing
    __IO uint16_t USBTXFIFOADD; 	//!< (off 0x064) USB Transmit FIFO Start Address
    __IO uint16_t USBRXFIFOADD; 	//!< (off 0x066) USB Receive FIFO Start Address
         uint8_t  reserved3[18];	//!< reserved
    __IO uint8_t  USBCONTIM;		//!< (off 0x07A) USB Connect Timing
    __IO uint8_t  USBVPLEN;			//!< (off 0x07B) USB OTG VBus Pulse Timing
         uint8_t  reserved4;		//!< reserved
    __IO uint8_t  USBFSEOF;			//!< (off 0x07D) USB Full-Speed Last Transaction to End of Frame Timing
    __IO uint8_t  USBLSEOF;			//!< (off 0x07E) USB Low-Speed Last Transaction to End of Frame Timing
         uint8_t  reserved5;		//!< reserved
    USBHOSTFUNS_t HOST_EP[16];		//!< (0ff 0x080) USB Host endpoints mapping
    USBDEVICEEP_t DEVICE_EP[16];	//!< (0ff 0x100) USB Device endpoints mapping
         uint8_t  reserved6[256];	//!< reserved
    USBRPCNT_t	  HOST_PC[16];		//!< (0ff 0x300) USB Request Packet Count in Block Transfer
    __IO uint16_t USBRXDPKTBUFDIS;  //!< (off 0x340) USB Receive Double Packet Buffer Disable
    __IO uint16_t USBTXDPKTBUFDIS;  //!< (off 0x342) USB Transmit Double Packet Buffer Disable
         uint8_t  reserved7[188];	//!< reserved
    __IO uint32_t USBEPC;			//!< (off 0x400) USB External Power Control
    __I  uint32_t USBEPCRIS;		//!< (off 0x404) USB External Power Control Raw Interrupt Status
    __IO uint32_t USBEPCIM;			//!< (off 0x408) USB External Power Control Interrupt Mask
    __IO uint32_t USBEPCISC; 		//!< (off 0x40C) USB External Power Control Interrupt Status and Clear
    __I  uint32_t USBDRRIS; 		//!< (off 0x410) USB Device RESUME Raw Interrupt Status
    __IO uint32_t USBDRIM; 			//!< (off 0x414) USB Device RESUME Interrupt Mask
    __O  uint32_t USBDRISC; 		//!< (off 0x418) USB Device RESUME Interrupt Status and Clear
    __IO uint32_t USBGPCS; 			//!< (off 0x41C) USB General-Purpose Control and Status
    	 uint32_t reserved8[4];		//!< reserved
    __IO uint32_t USBVDC; 			//!< (off 0x430) USB VBUS Droop Control
    __I  uint32_t USBVDCRIS; 		//!< (off 0x434) USB VBUS Droop Control Raw Interrupt Status
    __IO uint32_t USBVDCIM; 		//!< (off 0x438) USB VBUS Droop Control Interrupt Mask
    __IO uint32_t USBVDCISC; 		//!< (off 0x43C) USB VBUS Droop Control Interrupt Status and Clear
    	 uint32_t reserved9;		//!< reserved
    __I  uint32_t USBIDVRIS; 		//!< (off 0x444) USB ID Valid Detect Raw Interrupt Status
    __IO uint32_t USBIDVIM; 		//!< (off 0x448) USB ID Valid Detect Interrupt Mask
    __IO uint32_t USBIDVISC; 		//!< (off 0x44C) USB ID Valid Detect Interrupt Status and Clear
    __IO uint32_t USBDMASEL; 		//!< (off 0x450) USB DMA Select
} USB_Type;


#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */


//==============================================================================
//				USB registers
//==============================================================================


//-- USB off 0x000 USBFADDR: 		Device Functional Address  -
#define USB_USBFADDR_FUNCADDR_Msk	0x0000007F  //!< Function Address of Device as received through SET_ADDRESS

//-- USB off 0x001 USBPOWER:		Power
#define USB_USBPOWER_ISOUP         0x00000080  //!< Isochronous Update (device mode)
#define USB_USBPOWER_SOFTCONN      0x00000040  //!< Soft Connect/Disconnect (device mode)
#define USB_USBPOWER_RESET         0x00000008  //!< RESET Signaling
#define USB_USBPOWER_RESUME        0x00000004  //!< Resume Signaling
#define USB_USBPOWER_SUSPEND       0x00000002  //!< SUSPEND Mode
#define USB_USBPOWER_PWRDNPHY      0x00000001  //!< Power Down PHY

//-- USB off 0x002 USBTXIS:			Transmit Interrupt Status
#define USB_USBTXIS_EP15           0x00008000  //!< TX Endpoint 15 Interrupt
#define USB_USBTXIS_EP14           0x00004000  //!< TX Endpoint 14 Interrupt
#define USB_USBTXIS_EP13           0x00002000  //!< TX Endpoint 13 Interrupt
#define USB_USBTXIS_EP12           0x00001000  //!< TX Endpoint 12 Interrupt
#define USB_USBTXIS_EP11           0x00000800  //!< TX Endpoint 11 Interrupt
#define USB_USBTXIS_EP10           0x00000400  //!< TX Endpoint 10 Interrupt
#define USB_USBTXIS_EP9            0x00000200  //!< TX Endpoint 9 Interrupt
#define USB_USBTXIS_EP8            0x00000100  //!< TX Endpoint 8 Interrupt
#define USB_USBTXIS_EP7            0x00000080  //!< TX Endpoint 7 Interrupt
#define USB_USBTXIS_EP6            0x00000040  //!< TX Endpoint 6 Interrupt
#define USB_USBTXIS_EP5            0x00000020  //!< TX Endpoint 5 Interrupt
#define USB_USBTXIS_EP4            0x00000010  //!< TX Endpoint 4 Interrupt
#define USB_USBTXIS_EP3            0x00000008  //!< TX Endpoint 3 Interrupt
#define USB_USBTXIS_EP2            0x00000004  //!< TX Endpoint 2 Interrupt
#define USB_USBTXIS_EP1            0x00000002  //!< TX Endpoint 1 Interrupt
#define USB_USBTXIS_EP0            0x00000001  //!< TX and RX Endpoint 0 Interrupt

//-- USB off 0x004 USBRXIS:			Receive Interrupt Status
#define USB_USBRXIS_EP15           0x00008000  //!< RX Endpoint 15 Interrupt
#define USB_USBRXIS_EP14           0x00004000  //!< RX Endpoint 14 Interrupt
#define USB_USBRXIS_EP13           0x00002000  //!< RX Endpoint 13 Interrupt
#define USB_USBRXIS_EP12           0x00001000  //!< RX Endpoint 12 Interrupt
#define USB_USBRXIS_EP11           0x00000800  //!< RX Endpoint 11 Interrupt
#define USB_USBRXIS_EP10           0x00000400  //!< RX Endpoint 10 Interrupt
#define USB_USBRXIS_EP9            0x00000200  //!< RX Endpoint 9 Interrupt
#define USB_USBRXIS_EP8            0x00000100  //!< RX Endpoint 8 Interrupt
#define USB_USBRXIS_EP7            0x00000080  //!< RX Endpoint 7 Interrupt
#define USB_USBRXIS_EP6            0x00000040  //!< RX Endpoint 6 Interrupt
#define USB_USBRXIS_EP5            0x00000020  //!< RX Endpoint 5 Interrupt
#define USB_USBRXIS_EP4            0x00000010  //!< RX Endpoint 4 Interrupt
#define USB_USBRXIS_EP3            0x00000008  //!< RX Endpoint 3 Interrupt
#define USB_USBRXIS_EP2            0x00000004  //!< RX Endpoint 2 Interrupt
#define USB_USBRXIS_EP1            0x00000002  //!< RX Endpoint 1 Interrupt

//-- USB off 0x006 USBTXIE:			Transmit Interrupt Enable
#define USB_USBTXIE_EP15           0x00008000  //!< TX Endpoint 15 Interrupt Enable
#define USB_USBTXIE_EP14           0x00004000  //!< TX Endpoint 14 Interrupt Enable
#define USB_USBTXIE_EP13           0x00002000  //!< TX Endpoint 13 Interrupt Enable
#define USB_USBTXIE_EP12           0x00001000  //!< TX Endpoint 12 Interrupt Enable
#define USB_USBTXIE_EP11           0x00000800  //!< TX Endpoint 11 Interrupt Enable
#define USB_USBTXIE_EP10           0x00000400  //!< TX Endpoint 10 Interrupt Enable
#define USB_USBTXIE_EP9            0x00000200  //!< TX Endpoint 9 Interrupt Enable
#define USB_USBTXIE_EP8            0x00000100  //!< TX Endpoint 8 Interrupt Enable
#define USB_USBTXIE_EP7            0x00000080  //!< TX Endpoint 7 Interrupt Enable
#define USB_USBTXIE_EP6            0x00000040  //!< TX Endpoint 6 Interrupt Enable
#define USB_USBTXIE_EP5            0x00000020  //!< TX Endpoint 5 Interrupt Enable
#define USB_USBTXIE_EP4            0x00000010  //!< TX Endpoint 4 Interrupt Enable
#define USB_USBTXIE_EP3            0x00000008  //!< TX Endpoint 3 Interrupt Enable
#define USB_USBTXIE_EP2            0x00000004  //!< TX Endpoint 2 Interrupt Enable
#define USB_USBTXIE_EP1            0x00000002  //!< TX Endpoint 1 Interrupt Enable
#define USB_USBTXIE_EP0            0x00000001  //!< TX and RX Endpoint 0 Interrupt Enable
#define USB_USBTXIE_ALL            0x0000FFFF  //!< TX and RX Endpoint 0 Interrupt Enable

//-- USB off 0x008 USBRXIE:			Receive Interrupt Enable
#define USB_USBRXIE_EP15           0x00008000  //!< RX Endpoint 15 Interrupt Enable
#define USB_USBRXIE_EP14           0x00004000  //!< RX Endpoint 14 Interrupt Enable
#define USB_USBRXIE_EP13           0x00002000  //!< RX Endpoint 13 Interrupt Enable
#define USB_USBRXIE_EP12           0x00001000  //!< RX Endpoint 12 Interrupt Enable
#define USB_USBRXIE_EP11           0x00000800  //!< RX Endpoint 11 Interrupt Enable
#define USB_USBRXIE_EP10           0x00000400  //!< RX Endpoint 10 Interrupt Enable
#define USB_USBRXIE_EP9            0x00000200  //!< RX Endpoint 9 Interrupt Enable
#define USB_USBRXIE_EP8            0x00000100  //!< RX Endpoint 8 Interrupt Enable
#define USB_USBRXIE_EP7            0x00000080  //!< RX Endpoint 7 Interrupt Enable
#define USB_USBRXIE_EP6            0x00000040  //!< RX Endpoint 6 Interrupt Enable
#define USB_USBRXIE_EP5            0x00000020  //!< RX Endpoint 5 Interrupt Enable
#define USB_USBRXIE_EP4            0x00000010  //!< RX Endpoint 4 Interrupt Enable
#define USB_USBRXIE_EP3            0x00000008  //!< RX Endpoint 3 Interrupt Enable
#define USB_USBRXIE_EP2            0x00000004  //!< RX Endpoint 2 Interrupt Enable
#define USB_USBRXIE_EP1            0x00000002  //!< RX Endpoint 1 Interrupt Enable
#define USB_USBRXIE_ALL            0x0000FFFE  //!< RX Endpoint 1 Interrupt Enable

//-- USB off 0x00A USBIS:			General Interrupt Status
#define USB_USBIS_VBUSERR          0x00000080  //!< VBus Error (host mode)
#define USB_USBIS_SESREQ           0x00000040  //!< Session Request (host mode)
#define USB_USBIS_DISCON           0x00000020  //!< Session Disconnect
#define USB_USBIS_CONN             0x00000010  //!< Session Connect (host mode)
#define USB_USBIS_SOF              0x00000008  //!< Start of Frame
#define USB_USBIS_BABBLE           0x00000004  //!< Babble Detected
#define USB_USBIS_RESET            0x00000004  //!< Reset Signal Detected
#define USB_USBIS_RESUME           0x00000002  //!< Resume Signal Detected
#define USB_USBIS_SUSPEND          0x00000001  //!< Suspend Signal Detected (device mode)

//-- USB off 0x00B USBIE:			Interrupt Enable
#define USB_USBIE_VBUSERR          0x00000080  //!< Enable VBUS Error Interrupt (host mode)
#define USB_USBIE_SESREQ           0x00000040  //!< Enable Session Request Interrupt (host mode)
#define USB_USBIE_DISCON           0x00000020  //!< Enable Disconnect Interrupt
#define USB_USBIE_CONN             0x00000010  //!< Enable Connect Interrupt (host mode)
#define USB_USBIE_SOF              0x00000008  //!< Enable Start-of-Frame Interrupt
#define USB_USBIE_BABBLE           0x00000004  //!< Enable Babble Interrupt
#define USB_USBIE_RESET            0x00000004  //!< Enable Reset Interrupt
#define USB_USBIE_RESUME           0x00000002  //!< Enable Resume Interrupt
#define USB_USBIE_SUSPND           0x00000001  //!< Enable Suspend Interrupt (device mode

//-- USB off 0x00C USBFRAME:		Frame Value
#define USB_USBFRAME_Msk           0x000007FF  //!< Frame Number

//-- USB off 0x00E USBEPIDX:		Endpoint Index
#define USB_USBEPIDX_EPIDX_Msk     0x0000000F  //!< Endpoint Index

//-- USB off 0x00F USBTEST:			Test Mode
#define USB_USBTEST_FORCEH         0x00000080  //!< Force Host Mode (host mode)
#define USB_USBTEST_FIFOACC        0x00000040  //!< FIFO Access
#define USB_USBTEST_FORCEFS        0x00000020  //!< Force Full Speed

//-- USB off 0x020 USBFIFO[16]:		FIFO Endpoint 0-15
#define USB_USBFIFO_EPDATA_Msk      0xFFFFFFFF  //!< Endpoint Data

//-- USB off 0x060 USBDEVCTL:		Device Control
#define USB_USBDEVCTL_DEV          0x00000080  //!< Device Mode
#define USB_USBDEVCTL_FSDEV        0x00000040  //!< Full-Speed Device Detected
#define USB_USBDEVCTL_LSDEV        0x00000020  //!< Low-Speed Device Detected
#define USB_USBDEVCTL_VBUS_Msk     0x00000018  //!< VBus Level
#define USB_USBDEVCTL_VBUS_NONE    0x00000000  //!< Below SessionEnd
#define USB_USBDEVCTL_VBUS_SEND    0x00000008  //!< Above SessionEnd, below AValid
#define USB_USBDEVCTL_VBUS_AVALID  0x00000010  //!< Above AValid, below VBusValid
#define USB_USBDEVCTL_VBUS_VALID   0x00000018  //!< Above VBusValid
#define USB_USBDEVCTL_HOST         0x00000004  //!< Host Mode
#define USB_USBDEVCTL_HOSTREQ      0x00000002  //!< Host Request
#define USB_USBDEVCTL_SESSION      0x00000001  //!< Session Start/End

//-- USB off 0x062 USBTXFIFOSZ:		Transmit Dynamic FIFO Sizing
#define USB_USBTXFIFOSZ_DPB        0x00000010  //!< Double Packet Buffer Support
#define USB_USBTXFIFOSZ_SIZE_Msk   0x0000000F  //!< Max Packet Size
#define USB_USBTXFIFOSZ_SIZE_8     0x00000000  //!< 8
#define USB_USBTXFIFOSZ_SIZE_16    0x00000001  //!< 16
#define USB_USBTXFIFOSZ_SIZE_32    0x00000002  //!< 32
#define USB_USBTXFIFOSZ_SIZE_64    0x00000003  //!< 64
#define USB_USBTXFIFOSZ_SIZE_128   0x00000004  //!< 128
#define USB_USBTXFIFOSZ_SIZE_256   0x00000005  //!< 256
#define USB_USBTXFIFOSZ_SIZE_512   0x00000006  //!< 512
#define USB_USBTXFIFOSZ_SIZE_1024  0x00000007  //!< 1024
#define USB_USBTXFIFOSZ_SIZE_2048  0x00000008  //!< 2048

//-- USB off 0x063 USBRXFIFOSZ:		Receive Dynamic FIFO Sizing
#define USB_USBRXFIFOSZ_DPB        0x00000010  //!< Double Packet Buffer Support
#define USB_USBRXFIFOSZ_SIZE_Msk   0x0000000F  //!< Max Packet Size
#define USB_USBRXFIFOSZ_SIZE_8     0x00000000  //!< 8
#define USB_USBRXFIFOSZ_SIZE_16    0x00000001  //!< 16
#define USB_USBRXFIFOSZ_SIZE_32    0x00000002  //!< 32
#define USB_USBRXFIFOSZ_SIZE_64    0x00000003  //!< 64
#define USB_USBRXFIFOSZ_SIZE_128   0x00000004  //!< 128
#define USB_USBRXFIFOSZ_SIZE_256   0x00000005  //!< 256
#define USB_USBRXFIFOSZ_SIZE_512   0x00000006  //!< 512
#define USB_USBRXFIFOSZ_SIZE_1024  0x00000007  //!< 1024
#define USB_USBRXFIFOSZ_SIZE_2048  0x00000008  //!< 2048

//-- USB off 0x064 USBTXFIFOADD: 	Transmit FIFO Start Address
#define USB_USBTXFIFOADD_ADDR_Msk  0x00001FFF  //!< Transmit Start Address
#define USB_USBTXFIFOADD_ADDR_0    0x00000000  //!< 0
#define USB_USBTXFIFOADD_ADDR_8    0x00000001  //!< 8
#define USB_USBTXFIFOADD_ADDR_16   0x00000002  //!< 16
#define USB_USBTXFIFOADD_ADDR_32   0x00000003  //!< 32
#define USB_USBTXFIFOADD_ADDR_64   0x00000004  //!< 64
#define USB_USBTXFIFOADD_ADDR_128  0x00000005  //!< 128
#define USB_USBTXFIFOADD_ADDR_256  0x00000006  //!< 256
#define USB_USBTXFIFOADD_ADDR_512  0x00000007  //!< 512
#define USB_USBTXFIFOADD_ADDR_1024 0x00000008  //!< 1024
#define USB_USBTXFIFOADD_ADDR_2048 0x00000009  //!< 2048

//-- USB off 0x066 USBRXFIFOADD: 	Receive FIFO Start Address
#define USB_USBRXFIFOADD_ADDR_Msk  0x00001FFF  //!< Receive Start Address
#define USB_USBRXFIFOADD_ADDR_0    0x00000000  //!< 0
#define USB_USBRXFIFOADD_ADDR_8    0x00000001  //!< 8
#define USB_USBRXFIFOADD_ADDR_16   0x00000002  //!< 16
#define USB_USBRXFIFOADD_ADDR_32   0x00000003  //!< 32
#define USB_USBRXFIFOADD_ADDR_64   0x00000004  //!< 64
#define USB_USBRXFIFOADD_ADDR_128  0x00000005  //!< 128
#define USB_USBRXFIFOADD_ADDR_256  0x00000006  //!< 256
#define USB_USBRXFIFOADD_ADDR_512  0x00000007  //!< 512
#define USB_USBRXFIFOADD_ADDR_1024 0x00000008  //!< 1024
#define USB_USBRXFIFOADD_ADDR_2048 0x00000009  //!< 2048

//-- USB off 0x07A USBCONTIM:		Connect Timing
#define USB_USBCONTIM_WTCON_Msk    0x000000F0  //!< Connect Wait
#define USB_USBCONTIM_WTID_Msk     0x0000000F  //!< Wait ID

//-- USB off 0x07B USBVPLEN:		OTG VBus Pulse Timing
#define USB_USBVPLEN_VPLEN_Msk     0x000000FF  //!< VBus Pulse Length

//-- USB off 0x07D USBFSEOF:		Full-Speed Last Transaction to End of Frame Timing
#define USB_USBFSEOF_FSEOFG_Msk    0x000000FF  //!< Full-Speed End-of-Frame Gap

//-- USB off 0x07E USBLSEOF:		Low-Speed Last Transaction to End of Frame Timing
#define USB_USBLSEOF_LSEOFG_Msk    0x000000FF  //!< Low-Speed End-of-Frame Gap


//-- USB 0ff 0x080 (+ 0x0) USBTXFUNCADDR: Transmit Functional Address Endpoint 0-15
#define USB_USBTXFUNCADDR_ADDR_Msk 0x0000007F  //!< Device Address

//-- USB 0ff 0x080 (+ 0x2) USBTXHUBADDR:  Transmit Hub Address Endpoint 0-15
#define USB_USBTXHUBADDR_MULTTRAN  0x00000080  //!< Multiple Translators
#define USB_USBTXHUBADDR_ADDR_Msk  0x0000007F  //!< Hub Address

//-- USB 0ff 0x080 (+ 0x3) USBTXHUBPORT:  Transmit Hub Port Endpoint 0-15
#define USB_USBTXHUBPORT_PORT_Msk  0x0000007F  //!< Hub Port

//-- USB 0ff 0x080 (+ 0x4) USBRXFUNCADDR: Receive Functional Address Endpoint 1-15
#define USB_USBRXFUNCADDR_ADDR_Msk 0x0000007F  //!< Device Address

//-- USB 0ff 0x080 (+ 0x6) USBRXHUBADDR:  Receive Hub Address Endpoint 1-15
#define USB_USBRXHUBADDR_MULTTRAN  0x00000080  //!< Multiple Translators
#define USB_USBRXHUBADDR_ADDR_Msk  0x0000007F  //!< Hub Address

//-- USB 0ff 0x080 (+ 0x7) USBRXHUBPORT:  Receive Hub Port Endpoint 1-15
#define USB_USBRXHUBPORT_PORT_Msk  0x0000007F  //!< Hub Port



//-- USB 0ff 0x100 (+ 0x0)	USBTXMAXP:  	 Maximum Transmit Data Endpoint 1-15
#define USB_USBTXMAXP_MAXLOAD_Msk  0x000007FF  //!< Maximum Payload

//-- USB 0ff 0x100 (+ 0x2)	USBTXCSRL:	   	 Transmit Control and Status Endpoint 0-15 Low
/** NAK Timeout
 * host mode ep 0:
 * 		0	No timeout.
 *		1 	Indicates that endpoint 0 is halted following the receipt of NAK
 *		 	responses for longer than the time set by the USBNAKLMT register.
 * host mode bulk endpoints:
 * 		0	No timeout.
 * 		1	Indicates that the transmit endpoint is halted following the receipt
 * 			of NAK responses for longer than the time set by the NAKLMT field in
 * 			the USBTXINTERVALn register.
 * 			Software must clear this bit to allow the endpoint to continue
 */
#define USB_USBTXCSRL_NAKTO        0x00000080

/** Setup End Clear
 * device mode ep 0:
 * 		Writing a 1 to this bit clears the SETEND bit.
 */
#define USB_USBTXCSRL_SETENDC      0x00000080



/** Status Packet
 * host mode ep 0:
 * 		0 	No transaction.
 * 		1	Initiates a STATUS stage transaction. This bit must be set at the
 * 			same time as the TXRDY or REQPKT bit is set.
 */
#define USB_USBTXCSRL0_STATUS      0x00000040

/** RXRDY Clear
 * device mode ep 0:
 *		Writing a 1 to this bit clears the RXRDY bit.
 */
#define USB_USBTXCSRL0_RXRDYC      0x00000040

/** Clear Data Toggle
 * ep 1-15:
 * 		Writing a 1 to this bit clears the DT bit in the USBTXCSRHn register
 */
#define USB_USBTXCSRL_CLRDT        0x00000040



/** Request Packet
 * host mode ep 0:
 * 		0 	No request
 * 		1 	Requests an IN transaction
 */
#define USB_USBTXCSRL0_REQPKT      0x00000020

/** Send Stall
 * device mode ep 0:
 *		0 	No effect
 *		1	Terminates the current transaction and transmits the STALL handshake
 */
#define USB_USBTXCSRL0_STALL       0x00000020

/** Endpoint Stalled
 * host mode ep 1-15:
 * 		0 	A STALL handshake has not been received.
 * 		1	Indicates that a STALL handshake has been received. When this bit is
 * 			set, any μDMA request that is in progress is stopped, the FIFO is
 * 			completely flushed, and the TXRDY bit is cleared.
 * 		Software must clear this bit.
 * device mode ep 1-15:
 * 		0 	A STALL handshake has not been transmitted.
 * 		1	A STALL handshake has been transmitted. The FIFO is flushed and the
 * 			TXRDY bit is cleared.
 * 		Software must clear this bit.
 */
#define USB_USBTXCSRL_STALLED      0x00000020



/** Error
 * host mode ep 0:
 * 		0 	No error.
 * 		1	Three attempts have been made to perform a transaction with no
 * 			response from the peripheral. The EP0 bit in the USBTXIS register is
 * 			 also set in this situation.
 */
#define USB_USBTXCSRL0_ERROR       0x00000010

/** Setup End
 * device mode ep 0 (RO):
 *		0	A control transaction has not ended or ended after the DATAEND bit
 *			was set.
 *		1	A control transaction has ended before the DATAEND bit has been set.
 *			The EP0 bit in the USBTXIS register is also set in this situation.
 */
#define USB_USBTXCSRL0_SETEND      0x00000010

/** Setup Packet
 * host mode ep 1-15:
 *		0 	No SETUP token is sent.
 *		1	Sends a SETUP token instead of an OUT token for the transaction.
 *			This bit should be set at the same time as the TXRDY bit is set.
 *	Note: Setting this bit also clears the DT bit in the USBTXCSRHn register.
 */
#define USB_USBTXCSRL_SETUP        0x00000010

/** Send Stall
 * device mode ep 1-15:
 * 		0 	No effect.
 * 		1 	Issues a STALL handshake to an IN token.
 * 		Software clears this bit to terminate the STALL condition.
 * 		Note: This bit has no effect in isochronous transfers.
 */
#define USB_USBTXCSRL_STALL        0x00000010



/** Setup Packet
 * host mode ep 0
 *		0	Sends an OUT token
 *		1 	Sends a SETUP token instead of an OUT token for the transaction.
 *			This bit should be set at the same time as the TXRDY bit is set.
 */
#define USB_USBTXCSRL0_SETUP       0x00000008

/** Data End
 * device mode ep 0
 *		0 	No effect
 *		1	Set this bit in the following situations:
 *			■ When setting TXRDY for the last data packet
 *			■ When clearing RXRDY after unloading the last data packet
 *			■ When setting TXRDY for a zero-length data packet
 */
#define USB_USBTXCSRL0_DATAEND     0x00000008

/** Flush FIFO
 * ep 1-15:
 * 		0	No effect.
 * 		1	Flushes the latest packet from the endpoint transmit FIFO. The FIFO
 * 			pointer is reset and the TXRDY bit is cleared. The EPn bit in the
 * 			USBTXIS register is also set in this situation.
 * 		This bit may be set simultaneously with the TXRDY bit to abort the
 * 		packet that is currently being loaded into the FIFO. Note that if the
 * 		FIFO is double-buffered, FLUSH may have to be set twice to completely
 * 		clear the FIFO.
 * 		Important: This bit should only be set when the TXRDY bit is set. At
 * 		other times, it may cause data to be corrupted.
 */
#define USB_USBTXCSRL_FLUSH        0x00000008



/** Endpoint Stalled
 * host mode ep 0:
 * 		0 	No handshake has been received.
 * 		1 	A STALL handshake has been received.
 * device mode ep 0:
 * 		0 	A STALL handshake has not been transmitted
 * 		1 	A STALL handshake has been transmitted.
 */
#define USB_USBTXCSRL0_STALLED     0x00000004

/** Error
 * host mode ep 1-15:
 * 		0	No error
 * 		1	Three attempts have been made to send a packet and no handshake
 * 			packet has been received. The TXRDY bit is cleared, the EPn bit in
 * 			the USBTXIS register is set, and the FIFO is completely flushed in
 * 			this situation.
 * 		Software must clear this bit.
 * 		Note: This is valid only when the endpoint is operating in Bulk or
 * 		Interrupt mode
 */
#define USB_USBTXCSRL_ERROR        0x00000004

/** Underrun
 * device mode ep 1-15:
 *		0 	No underrun
 *		1 	An IN token has been received when TXRDY is not set.
 *		Software must clear this bit.
 */
#define USB_USBTXCSRL_UNDRN        0x00000004



/** Transmit Packet Ready
 * host mode ep 0:
 * 		0 	No transmit packet is ready.
 * 		1	Software sets this bit after loading a data packet into the TX FIFO.
 * 			The EP0 bit in the USBTXIS register is also set in this situation.
 * 			If both the TXRDY and SETUP bits are set, a setup packet is sent.
 * 			If just TXRDY is set, an OUT packet is sent.
 * device mode ep 0:
 * 		0 	No transmit packet is ready.
 * 		1	Software sets this bit after loading an IN data packet into the
 * 			TX FIFO. The EP0 bit in the USBTXIS register is also set in this
 * 			situation.
 */
#define USB_USBTXCSRL0_TXRDY       0x00000002

/** FIFO Not Empty
 * ep 1-15:
 * 		0 	The FIFO is empty
 * 		1	At least one packet is in the transmit FIFO
 */
#define USB_USBTXCSRL_FIFONE       0x00000002



/** Receive Packet Ready
 * host mode ep 0:
 * 		0 	No received packet has been received.
 * 		1	Indicates that a data packet has been received in the RX FIFO.
 * 			The EP0 bit in the USBTXIS register is also set in this situation.
 */
#define USB_USBTXCSRL0_RXRDY       0x00000001

/** Transmit Packet Ready
 * ep 1-15:
 *		0 	No transmit packet is ready.
 *		1	Software sets this bit after loading a data packet into the TX FIFO.
 */
#define USB_USBTXCSRL_TXRDY        0x00000001



//-- USB 0ff 0x100 (+ 0x3)	USBTXCSRH:    	 Transmit Control and Status Endpoint 0-15 High
#define USB_USBTXCSRH0_DTWE        0x00000004  //!< Data Toggle Write Enable (host)
#define USB_USBTXCSRH0_DT          0x00000002  //!< Data Toggle (host)
#define USB_USBTXCSRH0_FLUSH       0x00000001  //!< Flush FIFO

#define USB_USBTXCSRH_AUTOSET      0x00000080  //!< Auto Set
#define USB_USBTXCSRH_ISO          0x00000040  //!< ISO	(device)
#define USB_USBTXCSRH_MODE         0x00000020  //!< Mode
#define USB_USBTXCSRH_DMAEN        0x00000010  //!< DMA Request Enable
#define USB_USBTXCSRH_FDT          0x00000008  //!< Force Data Toggle
#define USB_USBTXCSRH_DMAMOD       0x00000004  //!< DMA Request Mode
#define USB_USBTXCSRH_DTWE         0x00000002  //!< Data Toggle Write Enable (host)
#define USB_USBTXCSRH_DT           0x00000001  //!< Data Toggle (host)

//-- USB 0ff 0x100 (+ 0x4)	USBRXMAXP:		 Maximum Receive Data Endpoint 1-15
#define USB_USBRXMAXP_MAXLOAD_Msk  0x000007FF  //!< Maximum Payload

//-- USB 0ff 0x100 (+ 0x6)	USBRXCSRL:     	 Receive Control and Status Endpoint 1-15 Low
#define USB_USBRXCSRL_CLRDT        0x00000080  //!< Clear Data Toggle
#define USB_USBRXCSRL_STALLED      0x00000040  //!< Endpoint Stalled
#define USB_USBRXCSRL_STALL        0x00000020  //!< Send Stall (device)
#define USB_USBRXCSRL_REQPKT       0x00000020  //!< Request Packet (host)
#define USB_USBRXCSRL_FLUSH        0x00000010  //!< Flush FIFO
#define USB_USBRXCSRL_DATAERR      0x00000008  //!< Data Error (device)
#define USB_USBRXCSRL_NAKTO        0x00000008  //!< NAK Timeout (host)
#define USB_USBRXCSRL_OVER         0x00000004  //!< Overrun (device)
#define USB_USBRXCSRL_ERROR        0x00000004  //!< Error (host)
#define USB_USBRXCSRL_FULL         0x00000002  //!< FIFO Full
#define USB_USBRXCSRL_RXRDY        0x00000001  //!< Receive Packet Ready

//-- USB 0ff 0x100 (+ 0x7)	USBRXCSRH:		 Receive Control and Status Endpoint 1-15 High
#define USB_USBRXCSRH_AUTOCL       0x00000080  //!< Auto Clear
#define USB_USBRXCSRH_AUTORQ       0x00000040  //!< Auto Request (host)
#define USB_USBRXCSRH_ISO          0x00000040  //!< ISO (device)
#define USB_USBRXCSRH_DMAEN        0x00000020  //!< DMA Request Enable
#define USB_USBRXCSRH_DISNYET      0x00000010  //!< Disable NYET (device)
#define USB_USBRXCSRH_PIDERR       0x00000010  //!< PID Error (host)
#define USB_USBRXCSRH_DMAMOD       0x00000008  //!< DMA Request Mode
#define USB_USBRXCSRH_DTWE         0x00000004  //!< Data Toggle Write Enable (host)
#define USB_USBRXCSRH_DT           0x00000002  //!< Data Toggle (host)

//-- USB 0ff 0x100 (+ 0x8)	USBRXCOUNT:		 Receive Byte Count Endpoint 0-15
#define USB_USBRXCOUNT_COUNT_Msk   0x00001FFF  //!< Receive Packet Count
#define USB_USBRXCOUNT0_COUNT_Msk  0x0000007F  //!< Count

//-- USB 0ff 0x100 (+ 0xA)	USBTXTYPE:		 Host Transmit Configure Type Endpoint 0-15
#define USB_USBTXTYPE_SPEED_Msk    0x000000C0  //!< Operating Speed
#define USB_USBTXTYPE_SPEED_DFLT   0x00000000  //!< Default
#define USB_USBTXTYPE_SPEED_FULL   0x00000080  //!< Full
#define USB_USBTXTYPE_SPEED_LOW    0x000000C0  //!< Low
#define USB_USBTXTYPE_PROTO_Msk    0x00000030  //!< Protocol
#define USB_USBTXTYPE_PROTO_CTRL   0x00000000  //!< Control
#define USB_USBTXTYPE_PROTO_ISOC   0x00000010  //!< Isochronous
#define USB_USBTXTYPE_PROTO_BULK   0x00000020  //!< Bulk
#define USB_USBTXTYPE_PROTO_INT    0x00000030  //!< Interrupt
#define USB_USBTXTYPE_TEP_Msk      0x0000000F  //!< Target Endpoint Number


//-- USB 0ff 0x100 (+ 0xB)	USBTXINTERVAL:	 Host Transmit Interval Endpoint 0-15
#define USB_USBTXINTERVAL_NAKLMT_Msk  	  0x000000FF  //!< NAK Limit is 2(m-1) frames (bulk transfers)
#define USB_USBTXINTERVAL_TXPOLL_INT_Msk  0x000000FF  //!< TX polling interval is m frames. (interrupt transfers)
#define USB_USBTXINTERVAL_TXPOLL_ISO_Msk  0x0000001F  //!< TX polling interval is 2^(m-1) frames. (isochronious transfers)

//-- USB 0ff 0x100 (+ 0xC)	USBRXTYPE:		 Host Configure Receive Type Endpoint 1-15
#define USB_USBRXTYPE_SPEED_Msk    0x000000C0  //!< Operating Speed
#define USB_USBRXTYPE_SPEED_DFLT   0x00000000  //!< Default
#define USB_USBRXTYPE_SPEED_FULL   0x00000080  //!< Full
#define USB_USBRXTYPE_SPEED_LOW    0x000000C0  //!< Low
#define USB_USBRXTYPE_PROTO_Msk    0x00000030  //!< Protocol
#define USB_USBRXTYPE_PROTO_CTRL   0x00000000  //!< Control
#define USB_USBRXTYPE_PROTO_ISOC   0x00000010  //!< Isochronous
#define USB_USBRXTYPE_PROTO_BULK   0x00000020  //!< Bulk
#define USB_USBRXTYPE_PROTO_INT    0x00000030  //!< Interrupt
#define USB_USBRXTYPE_TEP_Msk      0x0000000F  //!< Target Endpoint Number

//-- USB 0ff 0x100 (+ 0xD)	USBRXINTERVAL:	 Host Receive Polling Interval Endpoint 1-15
#define USB_USBRXINTERVAL_TXPOLL_Msk 0x000000FF  //!< RX Polling
#define USB_USBRXINTERVAL_NAKLMT_Msk 0x000000FF  //!< NAK Limit



//-- USB 0ff 0x300 (+ 0x0) USBRQPKTCOUNT:		Request Packet Count in Block Transfer Endpoint 1-15
#define USB_USBRQPKTCOUNT_COUNT_Msk  0x0000FFFF  //!< Block Transfer Packet Count

//-- USB off 0x340 USBRXDPKTBUFDIS: Receive Double Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP15   0x00008000  //!< EP15 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP14   0x00004000  //!< EP14 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP13   0x00002000  //!< EP13 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP12   0x00001000  //!< EP12 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP11   0x00000800  //!< EP11 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP10   0x00000400  //!< EP10 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP9    0x00000200  //!< EP9 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP8    0x00000100  //!< EP8 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP7    0x00000080  //!< EP7 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP6    0x00000040  //!< EP6 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP5    0x00000020  //!< EP5 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP4    0x00000010  //!< EP4 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP3    0x00000008  //!< EP3 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP2    0x00000004  //!< EP2 RX Double-Packet Buffer Disable
#define USB_USBRXDPKTBUFDIS_EP1    0x00000002  //!< EP1 RX Double-Packet Buffer Disable

//-- USB off 0x342 USBTXDPKTBUFDIS: Transmit Double Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP15   0x00008000  //!< EP15 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP14   0x00004000  //!< EP14 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP13   0x00002000  //!< EP13 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP12   0x00001000  //!< EP12 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP11   0x00000800  //!< EP11 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP10   0x00000400  //!< EP10 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP9    0x00000200  //!< EP9 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP8    0x00000100  //!< EP8 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP7    0x00000080  //!< EP7 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP6    0x00000040  //!< EP6 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP5    0x00000020  //!< EP5 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP4    0x00000010  //!< EP4 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP3    0x00000008  //!< EP3 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP2    0x00000004  //!< EP2 TX Double-Packet Buffer Disable
#define USB_USBTXDPKTBUFDIS_EP1    0x00000002  //!< EP1 TX Double-Packet Buffer Disable

//-- USB off 0x400 USBEPC:			External Power Control
#define USB_USBEPC_PFLTACT_Msk     0x00000300  //!< Power Fault Action
#define USB_USBEPC_PFLTACT_UNCHG   0x00000000  //!< Unchanged
#define USB_USBEPC_PFLTACT_TRIS    0x00000100  //!< Tristate
#define USB_USBEPC_PFLTACT_LOW     0x00000200  //!< Low
#define USB_USBEPC_PFLTACT_HIGH    0x00000300  //!< High
#define USB_USBEPC_PFLTAEN         0x00000040  //!< Power Fault Action Enable
#define USB_USBEPC_PFLTSEN_HIGH    0x00000020  //!< Power Fault Sense
#define USB_USBEPC_PFLTEN          0x00000010  //!< Power Fault Input Enable
#define USB_USBEPC_EPENDE          0x00000004  //!< EPEN Drive Enable
#define USB_USBEPC_EPEN_Msk        0x00000003  //!< External Power Supply Enable Configuration
#define USB_USBEPC_EPEN_LOW        0x00000000  //!< Power Enable Active Low
#define USB_USBEPC_EPEN_HIGH       0x00000001  //!< Power Enable Active High
#define USB_USBEPC_EPEN_VBLOW      0x00000002  //!< Power Enable High if VBUS Low
#define USB_USBEPC_EPEN_VBHIGH     0x00000003  //!< Power Enable High if VBUS High

//-- USB off 0x404 USBEPCRIS:		External Power Control Raw Interrupt Status
#define USB_USBEPCRIS_PF           0x00000001  //!< USB Power Fault Interrupt Status

//-- USB off 0x408 USBEPCIM:		External Power Control Interrupt Mask
#define USB_USBEPCIM_PF            0x00000001  //!< USB Power Fault Interrupt Mask

//-- USB off 0x40C USBEPCISC: 		External Power Control Interrupt Status and Clear
#define USB_USBEPCISC_PF           0x00000001  //!< USB Power Fault Interrupt Status and Clear

//-- USB off 0x410 USBDRRIS: 		Device RESUME Raw Interrupt Status
#define USB_USBDRRIS_RESUME        0x00000001  //!< Resume Interrupt Status

//-- USB off 0x414 USBDRIM: 		Device RESUME Interrupt Mask
#define USB_USBDRIM_RESUME         0x00000001  //!< Resume Interrupt Mask

//-- USB off 0x418 USBDRISC: 		Device RESUME Interrupt Status and Clear
#define USB_USBDRISC_RESUME        0x00000001  //!< Resume Interrupt Status and Clear

//-- USB off 0x41C USBGPCS: 		General-Purpose Control and Status
#define USB_USBGPCS_DEVMOD         0x00000001  //!< Device Mode
#define USB_USBGPCS_DEVMODOTG      0x00000002  //!< Enable Device Mode

//-- USB off 0x430 USBVDC: 			VBUS Droop Control
#define USB_USBVDC_VBDEN           0x00000001  //!< VBUS Droop Enable

//-- USB off 0x434 USBVDCRIS: 		VBUS Droop Control Raw Interrupt Status
#define USB_USBVDCRIS_VD           0x00000001  //!< VBUS Droop Raw Interrupt Status

//-- USB off 0x438 USBVDCIM: 		VBUS Droop Control Interrupt Mask
#define USB_USBVDCIM_VD            0x00000001  //!< VBUS Droop Interrupt Mask

//-- USB off 0x43C USBVDCISC: 		VBUS Droop Control Interrupt Status and Clear
#define USB_USBVDCISC_VD           0x00000001  //!< VBUS Droop Interrupt Status and Clear

//-- USB off 0x444 USBIDVRIS: 		ID Valid Detect Raw Interrupt Status
#define USB_USBIDVRIS_ID           0x00000001  //!< ID Valid Detect Raw Interrupt Status

//-- USB off 0x448 USBIDVIM: 		ID Valid Detect Interrupt Mask
#define USB_USBIDVIM_ID            0x00000001  //!< ID Valid Detect Interrupt Mask

//-- USB off 0x44C USBIDVISC: 		ID Valid Detect Interrupt Status and Clear
#define USB_USBIDVISC_ID           0x00000001  //!< ID Valid Detect Interrupt Status and Clear

//-- USB off 0x450 USBDMASEL: 		DMA Select
#define USB_USBDMASEL_DMACTX_Msk   0x00F00000  //!< DMA C TX Select
#define USB_USBDMASEL_DMACRX_Msk   0x000F0000  //!< DMA C RX Select
#define USB_USBDMASEL_DMABTX_Msk   0x0000F000  //!< DMA B TX Select
#define USB_USBDMASEL_DMABRX_Msk   0x00000F00  //!< DMA B RX Select
#define USB_USBDMASEL_DMAATX_Msk   0x000000F0  //!< DMA A TX Select
#define USB_USBDMASEL_DMAARX_Msk   0x0000000F  //!< DMA A RX Select
#define USB_USBDMASEL_DMABTX_Pos   12		   //!< doc!
#define USB_USBDMASEL_DMABRX_Pos   8		   //!< doc!
#define USB_USBDMASEL_DMAATX_Pos   4		   //!< doc!
#define USB_USBDMASEL_DMAARX_Pos   0		   //!< doc!

#define USB0         ((USB_Type *)USB0_BASE)	//!< doc!

 /** @} defgroup lm3s_usb_api USB */

#endif /* USB_LM3S_H_ */
