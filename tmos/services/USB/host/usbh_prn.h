/*
 * usbh_prn.h
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#ifndef USBH_PRN_H_
#define USBH_PRN_H_


#include <tmos.h>
#include <usb_host.h>

struct usb_remote_prn_t: public usb_remote_dev_t
{
	HANDLE 				epi_hnd;
	CSTRING 			device_id;
	uint16_t 			port_status;
	USBInterfaceDescriptor* pid;

	usb_remote_prn_t(HANDLE h1, HANDLE hi1): usb_remote_dev_t(h1),
			epi_hnd(hi1), port_status(0), pid(nullptr) {};

	void clean() override;

	RES_CODE get_device_id();
	RES_CODE get_port_status();
	RES_CODE soft_reset();
	RES_CODE scan_printer(uint32_t port_indx);
};

bool parse_ieee1284_id(CSTRING& dst, const char* src, const char* id);

#endif /* USBH_PRN_H_ */
