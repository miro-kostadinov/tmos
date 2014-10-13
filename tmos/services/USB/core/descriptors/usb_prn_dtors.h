/*
 * usb_prn_dtors.h
 *
 *  Created on: Oct 13, 2014
 *      Author: miro
 */

#ifndef USB_PRN_DTORS_H_
#define USB_PRN_DTORS_H_


/// USB Printer Class Codes
enum USBPrnSubClassCode : unsigned char
{
	INTERFACE_PRINTER_SUBCLASS 	= 0x01	//!<
};

/// USB Printer Protocol Codes
enum USBPrnProtocolCode : unsigned char
{
	PRINTER_UNIDIRECTIONAL_PROTOCOL 	= 0x01,	//!< unidirectional interface
	PRINTER_BIDIRECTIONAL_PROTOCOL		= 0x02, //!< bidirectional interface
	PRINTER_IEEE1284_4_PROTOCOL			= 0x03  //!< IEEE 1284.4 interface
};


#define PORT_STATUS_PAPER_EMPTY				0x20
#define PORT_STATUS_SELECT					0x10
#define PORT_STATUS_NOT_ERROR				0x08

#define PORT_STATUS_OK	(PORT_STATUS_SELECT | PORT_STATUS_NOT_ERROR)

#endif /* USB_PRN_DTORS_H_ */
