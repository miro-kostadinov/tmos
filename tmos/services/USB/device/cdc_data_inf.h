/*
 * cdc_data_inf.h
 *
 *  Created on: 13.07.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef CDC_DATA_INF_H_
#define CDC_DATA_INF_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_interface.h>

/** USB CDC data interface (or class) structure
 *
 */
struct usb_cdc_data_interface: usb_interface
{
	usb_cdc_data_interface(unsigned char bAS): usb_interface(bAS){};

	void Initialize(const USBInterfaceDescriptor* ds, int size) override;
	void RequestHandler(const void* drv,
			const USBGenericRequest *pRequest, HANDLE hnd) override;

};

#endif /* CDC_DATA_INF_H_ */
