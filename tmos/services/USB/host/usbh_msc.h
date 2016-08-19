/*
 * usbh_msc.h
 *
 *  Created on: Aug 8, 2016
 *      Author: miro
 */

#ifndef SERVICES_USB_HOST_USBH_MSC_H_
#define SERVICES_USB_HOST_USBH_MSC_H_

#include <tmos.h>
#include <usb_host.h>
#include <usb_msc_dtors.h>

#define USBMSC_WRITE_TOUT	500
#define USBMSC_READ_TOUT	500

struct usb_remote_msc_t: public usb_remote_dev_t
{
	HANDLE 	msc_hnd;
	USBInterfaceDescriptor* pid;

	uint32_t cb_tag;
	uint32_t lun;
	uint32_t blk_size;
	uint32_t capacity;
	uint8_t	msc_max_lun;

	usb_remote_msc_t(HANDLE h1, HANDLE h2): usb_remote_dev_t(h1),
			msc_hnd(h2), pid(NULL), cb_tag(1), lun(0), blk_size(0), capacity(0), msc_max_lun(0) {};


	RES_CODE get_max_lun();

	RES_CODE msc_command(usbmsc_cs_t* transaction, void* buf, uint32_t len);
	RES_CODE cmd_test_unit_ready(usbmsc_cs_t* transaction);
	RES_CODE cmd_read_capcity(usbmsc_cs_t* transaction);
	RES_CODE cmd_inquiry(usbmsc_cs_t* transaction);
	RES_CODE init_msd();
	RES_CODE msc_read(void* buf, uint32_t sector, uint32_t count);
	RES_CODE msc_write(const void* buf, uint32_t sector, uint32_t count);
	RES_CODE scan_msc(uint32_t port_indx, USBSubClassCode subcls, USBProtocolCode proto);
};


#endif /* SERVICES_USB_HOST_USBH_MSC_H_ */
