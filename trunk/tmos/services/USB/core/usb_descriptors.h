/*
 * usb_descriptors.h
 *
 *  Created on: 21.06.2011
 *      Author: Miroslav Kostadinov
 *
 *	NOTE: This file is based on some Atmel sources, so the original copyright
 *	follows
 *
 */

/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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
 \unit
 !!!Purpose

    Class for manipulating USB device descriptors.

 !!!Usage

    - Declare a USBDeviceDescriptor instance as the device descriptor of a
      USB device.
*/

#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include <tmos.h>
#include <usb_opt.h>

#ifdef __ICCARM__          // IAR
#pragma pack(1)            // IAR
#define __attribute__(...) // IAR
#endif                     // IAR

/// USB Endpints
enum USB_EPT : unsigned char
{
	EPT_0,
	EPT_1,
	EPT_2,
	EPT_3,
	EPT_4,
	EPT_5,
	EPT_6,
	EPT_7,
	EPT_8,
	EPT_9,
	EPT_10,
	EPT_11,
	EPT_12,
	EPT_13,
	EPT_14,
	EPT_15
};

//------------------------------------------------------------------------------
//         				USBGenericDescriptor
//------------------------------------------------------------------------------
/// USB descriptor types
enum USBDescriptorType : unsigned char
{
	DEVICE_DESCRIPTOR   				= 0x01, //!< Device descriptor type
	CONFIGURATION_DESCRIPTOR 			= 0x02, //!< Configuration descriptor type
	STRING_DESCRIPTOR					= 0x03, //!< String descriptor type
	INTERFACE_DESCRIPTOR				= 0x04, //!< Interface descriptor type
	ENDPOINT_DESCRIPTOR					= 0x05, //!< Endpoint descriptor type
	DEVICEQUALIFIER_DESCRIPTOR			= 0x06, //!< Device qualifier descriptor type
	OTHERSPEEDCONFIGURATION_DESCRIPTOR 	= 0x07, //!< Other speed configuration descriptor type
	INTERFACEPOWER_DESCRIPTOR			= 0x08, //!< Interface power descriptor type
	OTG_DESCRIPTOR						= 0x09, //!< On-The-Go descriptor type
	DEBUG_DESCRIPTOR					= 0x0A, //!< Debug descriptor type
	INTERFACEASSOCIATION_DESCRIPTOR		= 0x0B, //!< Interface association descriptor type
	HID_DESCRIPTOR						= 0x21, //!< HID descriptor
	HIDREPORT_DESCRIPTOR				= 0x22, //!< HID Report descriptor
	PHYSICAL_DESCRIPTOR					= 0x23, //!< Physical descriptor
	CDC_SPECIFIC_DESCRIPTOR				= 0x24, //!< Class specific function descriptor
	HUB_DESCRIPTOR						= 0x29, //!< Hub descriptor
};

/// Holds the few fields shared by all USB descriptors.
struct USBGenericDescriptor
{
	unsigned char 		bLength; 		//!< Length of the descriptor in bytes.
	USBDescriptorType 	bDescriptorType;//!< Descriptor type

	//--------------------------------------------------------------------------
	/// Returns the length of a descriptor.
	/// \return Length of descriptor in bytes.
	//--------------------------------------------------------------------------
	unsigned int GetLength() const { return bLength;}

	//--------------------------------------------------------------------------
	/// Returns the type of a descriptor.
	/// \return Type of descriptor.
	//--------------------------------------------------------------------------
	USBDescriptorType GetType()	const { return bDescriptorType; }

	//--------------------------------------------------------------------------
	/// Returns a pointer to the descriptor right after the given one, when
	/// parsing a Configuration descriptor.
	/// \return Pointer to the next descriptor.
	//--------------------------------------------------------------------------
	USBGenericDescriptor* GetNextDescriptor() const
	{
	    return (USBGenericDescriptor *) (((char *) this) + GetLength());
	}

	const USBGenericDescriptor* FindNext(USBDescriptorType from,
			USBDescriptorType to, int& size) const
	{
		   // Start parsing descriptors
		   const USBGenericDescriptor *descriptor = this;
		   while (size > (int)descriptor->GetLength())
		   {
			   size -= descriptor->GetLength();
			   // Get next descriptor
			   descriptor = descriptor->GetNextDescriptor();

			   if(descriptor->GetType() == from)
					return descriptor;

			   if(descriptor->GetType() == to)
					break;
		   }
		return NULL;
	}
} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBDeviceDescriptor (0x01)
//------------------------------------------------------------------------------

/// USB Specification Release Number in Binary-Coded Decimal
enum USBBCDReleaseCode : unsigned short
{
	USBDeviceDescriptor_USB2_00		= 0x0200 //!< The device supports USB 2.00.
};

/// USB Class Codes
enum USBClassCode : unsigned char
{
	INTERFACE_DEFINED_CLASS 	= 0x00,	//!< The class is defined in the interface(s)
	AUDIO_DEVICE_CLASS			= 0x01, //!< Audio device
	CDC_DEVICE_CLASS			= 0x02, //!< Communication device class
	HID_DEVICE_CLASS			= 0x03, //!< HID device class
	PHYSICAL_DEVICE_CLASS		= 0x05, //!< Physical device class
	IMAGE_DEVICE_CLASS			= 0x06, //!< Still Imaging device
	PRINTER_DEVICE_CLASS		= 0x07, //!< Printer device
	MASSSTORAGE_DEVICE_CLASS	= 0x08, //!< Mass Storage device
	HUB_DEVICE_CLASS			= 0x09, //!< Hub
	CDCDATA_DEVICE_CLASS		= 0x0A, //!< CDC data device
	SMARTCARD_DEVICE_CLASS		= 0x0B, //!< Smart Card device
	CONTENTSECURITY_CLASS		= 0x0D, //!< Content Security device
	VIDEO_DEVICE_CLASS			= 0x0E, //!< Video device
	DIAGNOSTIC_DEVICE_CLASS		= 0xDC, //!< Diagnostic Device
	WIRELESSCONTROLLER_CLASS	= 0xE0, //!< Wireless Controller
	MISCELLANEOUS_CLASS			= 0xEF, //!< Miscellaneous
	APPLICATION_SPECIFIC_CLASS	= 0xFE, //!< Application Specific
	VENDOR_SPECIFIC_CLASS		= 0xFF, //!< Vendor Specific
};

/// USB Sub Class Codes
enum USBSubClassCode : unsigned char
{
	INTERFACE_DEFINED_SUBCLASS 	= 0x00,	//!< The sub class is defined in the interface(s)
	CDC_DLCM_SUBCLASS		 	= 0x01,	//!< CDC Direct Line Control Model
	CDC_ACM_SUBCLASS 			= 0x02,	//!< CDC Abstract Control Model
	CDC_TCM_SUBCLASS 			= 0x03,	//!< CDC Telephone Control Model
	CDC_MCCM_SUBCLASS 			= 0x04,	//!< CDC Multi-Channel Control Model
	CDC_CAPI_SUBCLASS 			= 0x05,	//!< CDC CAPI Control Model
	CDC_ETHERNET_SUBCLASS 		= 0x06,	//!< CDC Ethernet Networking Control Model
	CDC_ATM_SUBCLASS 			= 0x07,	//!< CDC ATM Networking Control Model
	VENDOR_SPECIFIC_SUBCLASS	= 0xFF, //!< Vendor Specific
};

/// USB Protocol Codes
enum USBProtocolCode : unsigned char
{
	INTERFACE_DEFINED_PROTOCOL 	= 0x00,	//!< The sub class is defined in the interface(s)
	PROTOCOL_01					= 0x01,
	PROTOCOL_02					= 0x02,
	PROTOCOL_03					= 0x03,
	PROTOCOL_04					= 0x04,
	PROTOCOL_05					= 0x05,
	VENDOR_SPECIFIC_PROTOCOL	= 0xFF, //!< Vendor Specific
};

/// USB standard device descriptor structure.
struct USBDeviceDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	USBBCDReleaseCode bcdUSB;			//!< USB specification release number in BCD format.
	USBClassCode bDeviceClass;			//!< Device class code
	USBSubClassCode bDeviceSubClass;	//!< Device subclass code.
	USBProtocolCode bDeviceProtocol;	//!< Device protocol code
	unsigned char bMaxPacketSize0;		//!< Maximum packet size of endpoint 0 (in bytes)
	unsigned short idVendor;			//!< Vendor ID
	unsigned short idProduct;			//!< Product ID
	unsigned short bcdDevice;			//!< Device release number in BCD format
	unsigned char iManufacturer;		//!< Index of the manufacturer string descriptor
	unsigned char iProduct;				//!< Index of the product string descriptor
	unsigned char iSerialNumber;		//!< Index of the serial number string descriptor
	unsigned char bNumConfigurations;	//!< Number of possible configurations for the device
} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBStringDescriptor (0x03)
//------------------------------------------------------------------------------
/**
 *  Calculates the length of a string descriptor given the number of ascii
 *  characters/language IDs in it.
 *  \param length The ascii format string length.
 *  \return The actual data length in bytes.
 */
#define USBStringDescriptor_LENGTH(length)      ((length) * 2 + 2)

/** Language ID for US english. */
#define USBStringDescriptor_ENGLISH_US          0x09, 0x04

/**
 *  Converts an ascii character to its unicode representation.
 *  \param ascii The ASCII character to convert
 *  \return A 2-byte-array for the UNICODE based on given ASCII
 */
#define USBStringDescriptor_UNICODE(ascii)      (ascii), 0

/// USB standard string descriptor structure.
struct USBStringDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned short wLANGID[];			//!< Supported Language Code
} __attribute__ ((packed)); // GCC

//------------------------------------------------------------------------------
//         				USBInterfaceDescriptor (0x04)
//------------------------------------------------------------------------------
/// USB standard interface descriptor structure.
struct USBInterfaceDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned char bInterfaceNumber;		//!< Number of the interface in its configuration
	unsigned char bAlternateSetting;	//!< Value to select this alternate interface setting
	unsigned char bNumEndpoints;		//!< Number of endpoints used by the inteface (excluding endpoint 0)
	USBClassCode bDeviceClass;			//!< Interface class code
	USBSubClassCode bDeviceSubClass;	//!< Interface subclass code.
	USBProtocolCode bDeviceProtocol;	//!< Interface protocol code
	unsigned char iInterface;			//!< Index of the interface string descriptor

} __attribute__ ((packed)); // GCC

//------------------------------------------------------------------------------
//         				USBEndpointDescriptor (0x05)
//------------------------------------------------------------------------------
enum EndpointDirection
{
	ENDPOINT_DIRECTION_IN 	= 1, //!< Endpoint receives data from the host
	ENDPOINT_DIRECTION_OUT 	= 0	 //!< Endpoint sends data to the host
};

/// Calculates the address of an endpoint given its number and direction
/// \param direction USB endpoint direction definition
/// \param number USB endpoint number
/// \return The value used to set the endpoint descriptor based on input number
///         and direction
#define USBEndpointDescriptor_ADDRESS(direction, number) \
    (((direction & 0x01) << 7) | (number & 0xF))

/// Endpoint transfer type (bmAttributes bits 1-0)
enum EndpointAttribute_TransferType
{
	ENDPOINT_TYPE_CONTROL 		= 0,	//!< Control endpoint type
	ENDPOINT_TYPE_ISOCHRONOUS 	= 1,	//!< Isochronous endpoint type
	ENDPOINT_TYPE_BULK			= 2,	//!< Bulk endpoint type
	ENDPOINT_TYPE_INTERRUPT 	= 3		//!< Interrupt endpoint type
};

/// Synchronization type for isochronous endpoints (bmAttributes bits 3-2)
enum EndpointAttribute_ISO_SyncType
{
	ENDPOINT_ISO_SYNC_NONE 		= 0,	//!< No synchronization
	ENDPOINT_ISO_SYNC_ASYNC 	= 1,	//!< Asynchronous
	ENDPOINT_ISO_SYNC_ADAPTIVE 	= 2,	//!< Adaptive
	ENDPOINT_ISO_SYNC_SYNC 		= 3		//!< Synchronous
};

/// Usage type for isochronous endpoints (bmAttributes bits 5-4)
enum EndpointAttribute_ISO_UsageType
{
	ENDPOINT_ISO_USAGE_DATA 	= 0,	//!< Data endpoint
	ENDPOINT_ISO_USAGE_FEEDBACK	= 1,	//!< Feedback endpoint
	ENDPOINT_ISO_USAGE_IMPLICIT	= 2		//!< Implicit feedback Data endpoint
};

/**  Maximum size for a full-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_FS                64
/**  Maximum size for a high-speed control endpoint. */
#define USBEndpointDescriptor_MAXCTRLSIZE_HS                64
/**  Maximum size for a full-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_FS                64
/**  Maximum size for a high-speed bulk endpoint. */
#define USBEndpointDescriptor_MAXBULKSIZE_HS                512
/**  Maximum size for a full-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_FS           64
/**  Maximum size for a high-speed interrupt endpoint. */
#define USBEndpointDescriptor_MAXINTERRUPTSIZE_HS           1024
/**  Maximum size for a full-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_FS         1023
/**  Maximum size for a high-speed isochronous endpoint. */
#define USBEndpointDescriptor_MAXISOCHRONOUSSIZE_HS         1024

/// USB standard endpoint descriptor structure.
struct USBEndpointDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned char bEndpointAddress;		//!< Address (0-3) and direction (7) of the endpoint
	unsigned char bmAttributes;			//!< Endpoint type and additional characteristics (for isochronous endpoints)
	unsigned short wMaxPacketSize;		//!< Maximum packet size (in bytes) of the endpoint
	unsigned char bInterval;			//!< Polling rate of the endpoint

	//--------------------------------------------------------------------------
	/// Returns the number of an endpoint given its descriptor.
	/// \return Endpoint number.
	//--------------------------------------------------------------------------
	unsigned char GetNumber() const {	return bEndpointAddress & 0xF;}

	//--------------------------------------------------------------------------
	/// Returns the direction of an endpoint given its descriptor.
	/// \return Endpoint direction (see <Endpoint directions>).
	//--------------------------------------------------------------------------
	EndpointDirection GetDirection() const
	{
		return (EndpointDirection)(bEndpointAddress >> 7 );
	}

	//--------------------------------------------------------------------------
	/// Returns the type of an endpoint given its descriptor.
	/// \return Endpoint type (see <Endpoint types>).
	//--------------------------------------------------------------------------
	EndpointAttribute_TransferType GetType() const
	{
		return (EndpointAttribute_TransferType) (bmAttributes & 0x3);
	}

	//--------------------------------------------------------------------------
	/// Returns the maximum size of a packet (in bytes) on an endpoint given
	/// its descriptor.
	/// \return Maximum packet size of endpoint.
	//--------------------------------------------------------------------------
	unsigned short GetMaxPacketSize() const {	return wMaxPacketSize; }

	//--------------------------------------------------------------------------
	/// Returns the polling interval on an endpoint given its descriptor.
	/// \return Polling interval of endpoint.
	//--------------------------------------------------------------------------
	unsigned char GetInterval() const {	return bInterval;	}

} __attribute__ ((packed)); // GCC
//------------------------------------------------------------------------------
//         				USBDeviceQualifierDescriptor (0x06)
//------------------------------------------------------------------------------

/// Alternate device descriptor indicating the capabilities of the device
/// in full-speed, if currently in high-speed; or in high-speed, if it is
/// currently in full-speed. Only relevant for devices supporting the
/// high-speed mode.
struct USBDeviceQualifierDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	USBBCDReleaseCode bcdUSB;			//!< USB specification release number in BCD format.
	USBClassCode bDeviceClass;			//!< Device class code
	USBSubClassCode bDeviceSubClass;	//!< Device subclass code.
	USBProtocolCode bDeviceProtocol;	//!< Device protocol code
	unsigned char bMaxPacketSize0;		//!< Maximum packet size of endpoint 0 (in bytes)
	unsigned char bNumConfigurations;	//!< Number of possible configurations for the device
	unsigned char bReserved;			//!< Reserved

} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBOtgDescriptor (0x09)
//------------------------------------------------------------------------------

/// OTG Attribute Fields D7…2: Reserved D1: HNP support D0: SRP support
enum USBOTGAttributes : unsigned char
{
	NO_OTG_SUPPORT	 	= 0x00,	//!< No OTG feature supported
	HNP_SUPPORT			= 0x01, //!< Host Negotiation Protocol (HNP) is supported
	SRP_SUPPORT			= 0x02, //!< Session Request Protocol (SRP)is supported
	HNP_SRP_SUPPORT		= 0x03, //!< HNP and SRP are supported
};

struct USBOtgDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	USBOTGAttributes bmAttributes;		//!< OTG features supported
} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBInterfaceAssociationDescriptor (0x0B)
//------------------------------------------------------------------------------
///Interface association descriptor
struct USBInterfaceAssociationDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned char bFirstInterface;		//!< Interface number of the first interface that is associated with this function.
	unsigned char bInterfaceCount;		//!< Number of contiguous interfaces
	USBClassCode bFunctionClass;		//!< Class code
	USBSubClassCode bFunctionSubClass;	//!< Subclass code
	USBProtocolCode bFunctionProtocol;	//!< Protocol code
	unsigned char iFunction;			//!< Index of string descriptor describing this function
} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBConfigurationDescriptor (0x02)
//------------------------------------------------------------------------------

/// Device is bus-powered and not support remote wake-up.
#define USBConfigurationDescriptor_BUSPOWERED_NORWAKEUP  0x80
/// Device is self-powered and not support remote wake-up.
#define USBConfigurationDescriptor_SELFPOWERED_NORWAKEUP 0xC0
/// Device is bus-powered  and supports remote wake-up.
#define USBConfigurationDescriptor_BUSPOWERED_RWAKEUP    0xA0
/// Device is self-powered and supports remote wake-up.
#define USBConfigurationDescriptor_SELFPOWERED_RWAKEUP   0xE0

/// Calculates the value of the power consumption field given the value in mA.
/// \param power The power consumption value in mA
/// \return The value that should be set to the field in descriptor
#define USBConfigurationDescriptor_POWER(power)     (power / 2)

/// USB standard configuration descriptor structure.
struct USBConfigurationDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned short wTotalLength;		//!< Length of all descriptors returned
   	   	   	   	   	   	   	   	   		//!< along with this configuration
	unsigned char bNumInterfaces;		//!< Number of interfaces in this configuration
	unsigned char bConfigurationValue;	//!< Value for selecting this configuration
	unsigned char iConfiguration;		//!< Index of the configuration string descriptor
	unsigned char bmAttributes;			//!< Configuration characteristics
	unsigned char bMaxPower;			//!< Maximum Power Consumption in 2mA units

	//--------------------------------------------------------------------------
	/// Returns the total length of a configuration, i.e. including the
	/// descriptors following it.
	/// \param configuration Pointer to a USBConfigurationDescriptor instance.
	/// \return Total length (in bytes) of the configuration.
	//--------------------------------------------------------------------------
	unsigned int GetTotalLength() const { return wTotalLength; }

	//--------------------------------------------------------------------------
	/// Returns the number of interfaces in a configuration.
	/// \param configuration Pointer to a USBConfigurationDescriptor instance.
	/// \return Number of interfaces in configuration.
	//--------------------------------------------------------------------------
	unsigned char GetNumInterfaces() const { return bNumInterfaces; }

	//--------------------------------------------------------------------------
	/// Indicates if the device is self-powered when in a given configuration.
	/// \param configuration Pointer to a USBConfigurationDescriptor instance.
	/// \return 1 if the device is self-powered when in the given configuration;
	///         otherwise 0.
	//--------------------------------------------------------------------------
	unsigned char IsSelfPowered() const { return (bmAttributes >> 6) & 1; }

	//--------------------------------------------------------------------------
	/// Parses the given Configuration descriptor (followed by relevant
	/// interface, endpoint and class-specific descriptors) into three arrays.
	/// *Each array must have its size equal or greater to the number of
	/// descriptors it stores plus one*. A null-value is inserted after the last
	/// descriptor of each type to indicate the array end.
	///
	/// Note that if the pointer to an array is null (0), nothing is stored in
	/// it.
	/// \param configuration Pointer to the start of the whole Configuration
	///                      descriptor.
	/// \param interfaces    Pointer to the Interface descriptor array.
	/// \param endpoints     Pointer to the Endpoint descriptor array.
	/// \param others        Pointer to the class-specific descriptor array.
	//--------------------------------------------------------------------------
	void Parse(const USBGenericDescriptor **desc, USBDescriptorType type) const
	{
	   // Get size of configuration to parse
	   int size = GetTotalLength();
	   size -= sizeof(USBConfigurationDescriptor);

	   // Start parsing descriptors
	   const USBGenericDescriptor *descriptor = &this->as_generic;
	   while (size > 0)
	   {

		   // Get next descriptor
		   descriptor = descriptor->GetNextDescriptor();
		   size -= descriptor->GetLength();

		   // Store descriptor
		   if(descriptor->GetType() == type)
				*desc++ = descriptor;
	   }

	   // Null-terminate arrays
	   *desc = 0;
	}

} __attribute__ ((packed)); // GCC


//------------------------------------------------------------------------------
//         				USBDDriverDescriptors
//------------------------------------------------------------------------------
/// List of all descriptors used by a USB device driver. Each descriptor can
/// be provided in two versions: full-speed and high-speed. Devices which are
/// not high-speed capable do not need to provided high-speed descriptors and
/// the full-speed qualifier & other speed descriptors.
struct USBDDriverDescriptors{

    /// Pointer to the full-speed device descriptor.
    const USBDeviceDescriptor *pFsDevice;
    /// Pointer to the full-speed configuration descriptor.
    const USBConfigurationDescriptor *pFsConfiguration;
    /// Pointer to the full-speed qualifier descriptor.
    const USBDeviceQualifierDescriptor *pFsQualifier;
    /// Pointer to the full-speed other speed configuration descriptor.
    const USBConfigurationDescriptor *pFsOtherSpeed;
    /// Pointer to the high-speed device descriptor.
    const USBDeviceDescriptor *pHsDevice;
    /// Pointer to the high-speed configuration descriptor.
    const USBConfigurationDescriptor *pHsConfiguration;
    /// Pointer to the high-speed qualifier descriptor.
    const USBDeviceQualifierDescriptor *pHsQualifier;
    /// Pointer to the high-speed other speed configuration descriptor.
    const USBConfigurationDescriptor *pHsOtherSpeed;
    /// Pointer to the list of string descriptors.
    const unsigned char **pStrings;
    /// Number of string descriptors in list.
    unsigned char numStrings;

};


#ifdef __ICCARM__          // IAR
#pragma pack()             // IAR
#endif                     // IAR

#endif //#ifndef USB_DESCRIPTORS_H

