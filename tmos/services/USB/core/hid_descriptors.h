/*
 * hid_descriptors.h
 *
 *  Created on: Oct 10, 2014
 *      Author: miro
 */

#ifndef HID_DESCRIPTORS_H_
#define HID_DESCRIPTORS_H_


#include <usb_descriptors.h>

/// HID class specific requests
enum HIDGenericRequests : unsigned char
{
	HIDRequest_GET_REPORT			= 0x01,
	HIDRequest_GET_IDLE				= 0x02,
	HIDRequest_GET_PROTOCOL			= 0x03,
	HIDRequest_SET_REPORT			= 0x09,
	HIDRequest_SET_IDLE				= 0x0A,
	HIDRequest_SET_PROTOCOL			= 0x0B
};

#define HID_REPORTTYPE_INPUT           0x01
#define HID_REPORTTYPE_OUTPUT          0x02
#define HID_REPORTTYPE_FEATURE         0x03


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



#endif /* HID_DESCRIPTORS_H_ */
