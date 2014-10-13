/*
 * usb_hid_dtors.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_HID_DTORS_H_
#define USB_HID_DTORS_H_

#include <usb_std_dtors.h>

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
