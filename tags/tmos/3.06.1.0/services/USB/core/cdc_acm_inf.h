/*
 * cdc_acm_inf.h
 *
 *  Created on: 13.07.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef CDC_ACM_INF_H_
#define CDC_ACM_INF_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_interface.h>

enum CDCGenericRequests : unsigned char
{
	CDCRequest_SEND_ENCAPSULATED_COMMAND= 0x00,	//!< Issues a command in the format of the supported control protocol
	CDCRequest_GET_ENCAPSULATED_RESPONSE= 0x01,	//!< Requests a response in the format of the supported control protocol
	CDCRequest_SET_COMM_FEATURE			= 0x02,	//!< Controls the settings for a particular communication feature
	CDCRequest_GET_COMM_FEATURE			= 0x03,	//!< Returns the current settings for the communication feature
	CDCRequest_CLEAR_COMM_FEATURE		= 0x04,	//!< Clears the settings for a particular communication feature
	CDCRequest_SET_LINE_CODING			= 0x20,	//!< Configures DTE rate, stop-bits, parity, and number-of-character bits
	CDCRequest_GET_LINE_CODING			= 0x21,	//!< Requests current DTE rate, stop-bits, parity, and number-of-character bits
	CDCRequest_SET_CONTROL_LINE_STATE	= 0x22,	//!< RS-232 signal used to tell the DCE device the DTE device is now present
	CDCRequest_SEND_BREAK				= 0x23	//!< Sends special carrier modulation used to specify RS-232 style break
};

enum CDCLineCodingParity : unsigned char
{
	CDCLineCoding_NOPARITY              = 0x00, //!< No parity checking
	CDCLineCoding_ODDPARITY             = 0x01,	//!< Odd parity checking
	CDCLineCoding_EVENPARITY            = 0x02,	//!< Even parity checking
	CDCLineCoding_MARKPARITY            = 0x03,	//!< Mark parity checking
	CDCLineCoding_SPACEPARITY           = 0x04	//!< Space parity checking
};

/**
 * CDCLineCoding
 * 	Format of the data returned when a GET_LINE_CODING request is received.
 */
struct CDCLineCoding
{
    uint32_t dwDTERate;					//!< Data terminal rate in bits per second
    uint8_t bCharFormat;				//!< Number of stop bits
    CDCLineCodingParity bParityType;	//!< Type of parity checking used
    uint8_t bDataBits;					//!< Number of data bits (5, 6, 7, 8 or 16)
} __attribute__ ((packed));

/** bControlLineState
 *  USB CDC ControlLineState bitmap
 */
/** Indicates to DCE if DTE is present or not. */
#define CDCControlLineState_DTE_PRESENT                     (1 << 0)
/** RS232 signal DTR: Data Terminal Ready. */
#define CDCControlLineState_DTR                             (1 << 0)
/** Carrier control for half duplex modems. */
#define CDCControlLineState_CARRIER_ON                      (1 << 1)
/** RS232 signal RTS: Request to send. */
#define CDCControlLineState_RTS                             (1 << 1)

/** USB CDC Abstract Control Model interface structure
 * http://www.usb.org/developers/devclass_docs/usbcdc11.pdf
 */
struct usb_cdc_acm_interface: usb_interface
{
    /** Serial port ControlLineState */
    uint8_t        bControlLineState;
    /** Serial port SerialState */
    uint16_t       wSerialState;
    /** Serial port linecoding */
    CDCLineCoding  lineCoding;
    /** interface descriptor **/
	const USBInterfaceDescriptor* descriptor;

	usb_cdc_acm_interface(unsigned char bAS):	usb_interface(bAS),
			bControlLineState(), wSerialState(), descriptor(NULL)

	{
	}
	;

	virtual void Initialize(const USBInterfaceDescriptor* ds, int size);
	virtual void RequestHandler(const void* drv,
			const USBGenericRequest *pRequest, HANDLE hnd);

	virtual void set_line_coding(const void* drv) {};
	virtual void set_control_line_state(const void* drv) {};
};

#endif /* CDC_ACM_INF_H_ */
