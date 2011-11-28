/*
 * usb_interface.h
 *
 *  Created on: 12.07.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef USB_INTERFACE_H_
#define USB_INTERFACE_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_descriptors.h>
#include <usb_requests.h>

/** USB interface (or class) structure
 *
 */
struct usb_interface
{
	/// current alternative settins.
	unsigned char bAlternativeSetting;

	usb_interface(unsigned char bAS): bAlternativeSetting(bAS){};

	virtual void Initialize(const USBInterfaceDescriptor* ds, int size) =0;
	virtual void RequestHandler(const void* drv,
			const USBGenericRequest* pRequest, HANDLE hnd) = 0;

};


#endif /* USB_INTERFACE_H_ */
