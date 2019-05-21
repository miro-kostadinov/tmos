/*
 * usb_msc_dtors.h
 *
 *  Created on: Aug 9, 2016
 *      Author: miro
 */

#ifndef SERVICES_USB_DESCRIPTORS_USB_MSC_DTORS_H_
#define SERVICES_USB_DESCRIPTORS_USB_MSC_DTORS_H_


#include <usb_std_dtors.h>

/// USB Mass Storage Subclass Codes	(bDeviceSubClass)
enum USBMSCSubClassCode : unsigned char
{
	INTERFACE_MSC_SUBCLASS_SCSI_NR 	= 0x00,	//!< SCSI command set not reported; De facto use
	INTERFACE_MSC_SUBCLASS_RBC	 	= 0x01,	//!< RBC;							Allocated by USB-IF for RBC. RBC is defined outside of USB.
	INTERFACE_MSC_SUBCLASS_MMC5 	= 0x02,	//!< MMC-5 (ATAPI);					Allocated by USB-IF for MMC-5. MMC-5 is defined outside of USB.
	INTERFACE_MSC_SUBCLASS_QIC157	= 0x03,	//!< Obsolete;						Was QIC-157
	INTERFACE_MSC_SUBCLASS_UFI	 	= 0x04,	//!< UFI;							Specifies how to interface Floppy Disk Drives to USB
	INTERFACE_MSC_SUBCLASS_SFF	 	= 0x05,	//!< Obsolete;						Was SFF-8070i
	INTERFACE_MSC_SUBCLASS_SCSI_T 	= 0x06,	//!< SCSI transparent command set;	Allocated by USB-IF for SCSI. SCSI standards are defined outside of USB
	INTERFACE_MSC_SUBCLASS_LSDFS 	= 0x07,	//!< LSD FS;						LSDFS specifies how host has to negotiate access before trying SCSI
	INTERFACE_MSC_SUBCLASS_IEEE_1667= 0x08,	//!< IEEE 1667;						Allocated by USB-IF for IEEE 1667. IEEE 1667 is defined outside of USB.
	INTERFACE_MSC_SUBCLASS_VENDOR 	= 0xFF	//!< Specific to device vendor; 	De facto use
};

/// USB Mass Storage Transport Protocol Codes (bInterfaceProtocol)
enum USBMSCProtocolCode : unsigned char
{
	INTERFACE_MSC_PROTOCOL_CBI_W_CCI	= 0x00,	//!< Control/Bulk/Interrupt (CBI) Transport with command completion interrupt
	INTERFACE_MSC_PROTOCOL_CBI_WO_CCI	= 0x01,	//!< Control/Bulk/Interrupt (CBI) Transport with no command completion interrupt
	INTERFACE_MSC_PROTOCOL_BBB			= 0x50, //!< USB Mass Storage Class Bulk-Only (BBB) Transport
	INTERFACE_MSC_PROTOCOL_UAS			= 0x62, //!< Allocated by USB-IF for UAS.
	INTERFACE_MSC_PROTOCOL_VENDOR		= 0xFF  //!< Specific to device vendor
};

//------------------------------------------------------------------------------
//         				USBMSCDescriptor (0x24)
//------------------------------------------------------------------------------


///Pipe Usage Class Specific Descriptor
struct USBMSCPipeDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
										//!< Allocated by USB-IF for UAS. UAS is defined outside of USB
} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				Common Block Wrapper (CBW)
//------------------------------------------------------------------------------

#define USBMSC_CBW_SIZEOF            31
#define USBMSC_CBW_SIGNATURE         0x43425355   	//!< Signature that helps identify this data packet as a CBW

#define USBMSC_MAX_CDBLEN            16		        //!< Max length of length of the CBWCB in bytes

struct usbmsc_cbw_t
{
	uint32_t dCBWSignature;           //!< Signature that helps identify this data packet as a CBW (0x43425355)
	uint32_t dCBWTag;                 //!< A Command Block Tag sent by the host
	uint32_t dCBWDataTransferLength;  //!< The number of bytes of data that the host expects to transfer
	uint8_t bmCBWFlags;               //!< Bit 7: Direction=IN
	uint8_t bCBWLUN;                  //!< The device Logical Unit Number (LUN) to which the command block is being sent.
	uint8_t bCBWCBLength;             //!< The valid length of the CBWCB in bytes
	uint8_t CBWCB[USBMSC_MAX_CDBLEN]; //!< The command block to be executed by the device
};

#define USBMSC_CBW_FLAGS_OUT		0x00
#define USBMSC_CBW_FLAGS_IN			0x80

//------------------------------------------------------------------------------
//         				Command Status Wrapper (CSW)
//------------------------------------------------------------------------------

#define USBMSC_CSW_SIZEOF            13
#define USBMSC_CSW_SIGNATURE         0x53425355

#define USBMSC_CSW_STATUS_PASS        0		//!< Command Passed ("good status")
#define USBMSC_CSW_STATUS_FAIL        1		//!< Command Failed
#define USBMSC_CSW_STATUS_PHASEERROR  2		//!< Phase Error


struct usbmsc_csw_t
{
	uint32_t dCSWSignature;           //!< Signature that helps identify this data packet as a CSW
	uint32_t dCSWTag;                 //!< The device shall set this field to the value received in the dCBWTag of the associated CBW
	uint32_t dCSWDataResidue;         //!< The difference between the amount of data expected, and the actual amount of data processed by the device
	uint8_t bCSWStatus;               //!< success or failure of the command
} __attribute__ ((packed)); // GCC

// Usb BOT command block status values
//
#define USBMSC_BOT_COMMAND_OK       0x00 ///< Command passed, good status
#define USBMSC_BOT_COMMAND_FAILED   0x01 ///< Command failed
#define USBMSC_BOT_COMMAND_ERROR    0x02 ///< Phase error, need to reset the device

struct usbmsc_cs_t
{
	usbmsc_cbw_t cbw;				//!< command block wrapper
	usbmsc_csw_t csw;				//!< command status wrapper
};

#endif /* SERVICES_USB_DESCRIPTORS_USB_MSC_DTORS_H_ */
