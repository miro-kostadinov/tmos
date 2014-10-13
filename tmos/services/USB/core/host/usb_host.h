/*
 * usb_host.h
 *
 *  Created on: Apr 25, 2013
 *      Author: miro
 */

#ifndef USB_HOST_H_
#define USB_HOST_H_


#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>
#include <usb_std_reqs.h>

#define USB_HOST_FLG_LOW_SPPED		0x01
#define USB_HOST_FLG_CFG_READ		0x02	//!<flag whether we have read the device configuration
#define USB_HOST_FLG_NOTIFY			0x04

RES_CODE hdc_request(USBGenericRequest* req, HANDLE hnd, void* ptr);
RES_CODE usb_host_reset_bus(USB_DRV_INFO drv_info, HANDLE hnd);

void usb_get_class_name(CSTRING& s, USBClassCode code);
USBInterfaceDescriptor* usb_get_interface(const USBConfigurationDescriptor* des,
		int interface);
USBEndpointDescriptor* usb_get_enpoint(const USBConfigurationDescriptor* des,
		int indx);
RES_CODE  usb_get_interface_class_name(CSTRING& s,
		const USBConfigurationDescriptor* des, int interface);

int trace_usb_descriptor(USBGenericDescriptor* des);

#endif /* USB_HOST_H_ */
