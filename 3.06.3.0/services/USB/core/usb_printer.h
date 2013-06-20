/*
 * usb_printer.h
 *
 *  Created on: May 13, 2013
 *      Author: miro
 */

#ifndef USB_PRINTER_H_
#define USB_PRINTER_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_interface.h>


/// USB Sub Class Codes
#define INTERFACE_PRINTER_SUBCLASS		0x01

/// USB Protocol Codes
#define PRINTER_UNIDIRECTIONAL_PROTOCOL     0x01    //!< unidirectional interface
#define PRINTER_BIDIRECTIONAL_PROTOCOL      0x02    //!< bidirectional interface
#define PRINTER_IEEE1284_4_PROTOCOL         0x03    //!< IEEE 1284.4 interface

#define PORT_STATUS_PAPER_EMPTY				0x20
#define PORT_STATUS_SELECT					0x10
#define PORT_STATUS_NOT_ERROR				0x08

#define PORT_STATUS_OK	(PORT_STATUS_SELECT | PORT_STATUS_NOT_ERROR)

enum USBPrinterRequests : unsigned char
{
	USBPrinter_GET_DEVICE_ID 		= 0x00,	//!< returns a device ID string that is compatible with IEEE 1284
	USBPrinter_GET_PORT_STATUS 		= 0x01,	//!< returns the printer’s current status, in a format which is compatible with the status register of a standard PC parallel port
	USBPrinter_SOFT_RESET			= 0x02	//!< flushes all buffers and resets the Bulk OUT and Bulk IN pipes to their default states
};


#endif /* USB_PRINTER_H_ */
