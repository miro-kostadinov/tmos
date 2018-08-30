/*
 * usbh_vid-cdc.h
 *
 *  Created on: Aug 22, 2018
 *      Author: miro
 */

#ifndef SERVICES_USB_HOST_USBH_VID_CDC_H_
#define SERVICES_USB_HOST_USBH_VID_CDC_H_

#include <tmos.h>
#include <usb_host.h>

#define USBMSC_WRITE_TOUT	500
#define USBMSC_READ_TOUT	500

struct usb_vid_cdc_t: public usb_remote_dev_t
{
	HANDLE 	cdc_hnd;


	usb_vid_cdc_t(HANDLE h1, HANDLE h2): usb_remote_dev_t(h1),
			cdc_hnd(h2){};


	RES_CODE scan_vid(uint32_t port_indx, uint32_t vid, uint32_t pid);
};




#endif /* SERVICES_USB_HOST_USBH_VID_CDC_H_ */
