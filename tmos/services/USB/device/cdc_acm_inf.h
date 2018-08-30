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
			bControlLineState(0), wSerialState(0), descriptor(NULL)

	{
		lineCoding.dwDTERate = 9600;
		lineCoding.bCharFormat = 1;
		lineCoding.bParityType = CDCLineCoding_NOPARITY;
		lineCoding.bDataBits = 8;
	}
	;

	void Initialize(const USBInterfaceDescriptor* ds, int size) override;
	void RequestHandler(const void* drv,
			const USBGenericRequest *pRequest, HANDLE hnd) override;

	virtual void set_line_coding(const void* drv) {};
	virtual void set_control_line_state(const void* drv) {};
};

#endif /* CDC_ACM_INF_H_ */
