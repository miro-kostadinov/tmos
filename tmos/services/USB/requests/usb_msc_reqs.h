/*
 * usb_msc_reqs.h
 *
 *  Created on: Aug 9, 2016
 *      Author: miro
 */

#ifndef SERVICES_USB_REQUESTS_USB_MSC_REQS_H_
#define SERVICES_USB_REQUESTS_USB_MSC_REQS_H_

#include <tmos.h>
#include <usb_opt.h>


/// Mass storage class specific request codes
enum MSCGenericRequests : unsigned char
{
	MSCRequest_ADSC					= 0x00,		//!< Accept Device-Specific Command (ADSC), also aliases core USB request 00h Get Status.
	MSCRequest_GET_REQUEST			= 0xFC,
	MSCRequest_PUT_REQUEST			= 0xFD,
	MSCRequest_GET_MAX_LUN			= 0xFE,
	MSCRequest_BOMSR				= 0xFF		//!< Bulk-Only Mass Storage Reset (BOMSR)
};



#endif /* SERVICES_USB_REQUESTS_USB_MSC_REQS_H_ */
