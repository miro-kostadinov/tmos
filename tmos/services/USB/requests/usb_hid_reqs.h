/*
 * usb_hid_reqs.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_HID_REQS_H_
#define USB_HID_REQS_H_

#include <tmos.h>
#include <usb_opt.h>


/// HID class specific request codes
enum HIDGenericRequests : unsigned char
{
	HIDRequest_GET_REPORT			= 0x01,
	HIDRequest_GET_IDLE				= 0x02,
	HIDRequest_GET_PROTOCOL			= 0x03,
	HIDRequest_SET_REPORT			= 0x09,
	HIDRequest_SET_IDLE				= 0x0A,
	HIDRequest_SET_PROTOCOL			= 0x0B
};


/// Report types for get/set report request (top byte of wValue)
#define HID_REPORTTYPE_INPUT           0x01
#define HID_REPORTTYPE_OUTPUT          0x02
#define HID_REPORTTYPE_FEATURE         0x03



#endif /* USB_HID_REQS_H_ */
