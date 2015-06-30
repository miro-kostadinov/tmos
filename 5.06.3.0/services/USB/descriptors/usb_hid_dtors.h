/*
 * usb_hid_dtors.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_HID_DTORS_H_
#define USB_HID_DTORS_H_

#include <usb_std_dtors.h>

/// USB HID Interface Class Codes	(bDeviceSubClass)
enum USBHIDSubClassCode : unsigned char
{
	INTERFACE_HID_SUBCLASS_NONE 	= 0x00,	//!< No Subclass
	INTERFACE_HID_SUBCLASS_BOOT 	= 0x01	//!< Boot Interface Subclass
};

/// USB HID Interface Protocol Codes (bDeviceProtocol)
enum USBHIDProtocolCode : unsigned char
{
	INTERFACE_HID_PROTOCOL_NONE			= 0x00,	//!< None
	INTERFACE_HID_PROTOCOL_KEYBOARD		= 0x01, //!< Keyboard
	INTERFACE_HID_PROTOCOL_MOUSE		= 0x02  //!< Mouse
};

/// USB HID Report Types
enum USBHIDReportType : unsigned char
{
	HID_REPORT_ALL			= 0x00,	//!< Report IDs are not used or all input
	HID_REPORT_INPUT		= 0x01, //!< Input
	HID_REPORT_OUTPUT		= 0x02, //!< Output
	HID_REPORT_FEATURE		= 0x03  //!< Feature
};

//------------------------------------------------------------------------------
//         				USBHIDDescriptor (0x21)
//------------------------------------------------------------------------------

/// Optional HID descriptors
struct USBOptHIDDescriptors
{
	USBDescriptorType bDescriptorType;	//!< Constant name identifying type of class descriptor.
										//!<  See Section 7.1.2: Set_Descriptor Request for a
										//!<  table of class descriptor constants.

	unsigned short wDescriptorLength;	//!< Numeric expression that is the total size
										//!<  of the Report descriptor.
} __attribute__ ((packed)); // GCC

///Interface association descriptor
struct USBHIDDescriptor
{
	USBGenericDescriptor as_generic;	//!< Generic Descriptor
	unsigned short bcdHID;				//!< Numeric expression identifying the
										//!<  HID Class Specification release

	unsigned char bCountryCode;			//!< Numeric expression identifying country
										//!<  code of the localized hardware

	unsigned char bNumDescriptors;		//!< Numeric expression specifying the number
										//!<  of class descriptors (always at least one
										//!<  i.e. Report descriptor.)

	USBOptHIDDescriptors hid_descriptors[1]; //!< 1 (Report) or more optional descriptors
} __attribute__ ((packed)); // GCC


#endif /* USB_HID_DTORS_H_ */
