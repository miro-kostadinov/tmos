/*
 * usbh_hid.h
 *
 *  Created on: Oct 14, 2014
 *      Author: miro
 */

#ifndef USBH_HID_H_
#define USBH_HID_H_


#include <tmos.h>
#include <usb_host.h>

struct usb_remote_hid_t: public usb_remote_dev_t
{
	HANDLE 	epi_hnd;
	USBInterfaceDescriptor* pid;
	uint8_t	hid_idle;

	usb_remote_hid_t(HANDLE h1, HANDLE h2): usb_remote_dev_t(h1),
			epi_hnd(h2), pid(NULL) , hid_idle(255) {};

	RES_CODE get_hid_report_descriptor(void* buf, uint32_t len);
	RES_CODE get_hid_descriptor(void* buf, uint32_t len);

	RES_CODE get_report(void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE set_report(const void* buf, uint32_t len, uint32_t rep_type, uint32_t rep_id);
	RES_CODE get_idle(uint32_t rep_id);
	RES_CODE set_idle(uint32_t duration, uint32_t rep_id);
	RES_CODE get_protocol(void* buf);
	RES_CODE set_protocol(uint32_t protocol);

	RES_CODE scan_hid(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto);
};



#endif /* USBH_HID_H_ */
