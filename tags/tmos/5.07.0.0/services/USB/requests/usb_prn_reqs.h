/*
 * usb_prn_reqs.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_PRN_REQS_H_
#define USB_PRN_REQS_H_


#include <tmos.h>
#include <usb_opt.h>


//------------------------------------------------------------------------------
//         				USB Printer Request codes
//------------------------------------------------------------------------------
enum USBPrinterRequests : unsigned char
{
	USBPrinter_GET_DEVICE_ID 		= 0x00,	//!< returns a device ID string that is compatible with IEEE 1284
	USBPrinter_GET_PORT_STATUS 		= 0x01,	//!< returns the printer’s current status, in a format which is compatible with the status register of a standard PC parallel port
	USBPrinter_SOFT_RESET			= 0x02	//!< flushes all buffers and resets the Bulk OUT and Bulk IN pipes to their default states
};


#endif /* USB_PRN_REQS_H_ */
