/*
 * usb_api.h
 *
 *  Created on: 20.06.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef USB_API_H_
#define USB_API_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_device.h>
#include <usb_drv.h>

/**
 * Handle for USB operations
 *
 */
class usb_handle: public CHandle
{
	public:
//	usb_handle(): {};

	RES_CODE device_config(const USBDDriverDescriptors* drv_descriptors);
//	RES_CODE device_shutdown(const USB_CONFIGS* cfgs);
//	RES_CODE host_config(const USB_CONFIGS* cfgs);
//	RES_CODE host_shutdown(const USB_CONFIGS* cfgs);
//
//	RES_CODE device_connect(const USB_INTERFACE* usb_if);
};

#define USB_CMD_DEVICE_CONFIG 	(( 0 <<4)+CMD_COMMAND)
#define MAX_USBCALLBACK 		0
typedef RES_CODE (*USB_API_FUNC)(usb_handle* client, USB_DRV_INFO drv_info);
EXTERN_C const USB_API_FUNC usb_api_functions[MAX_USBCALLBACK+1];

#endif /* USB_API_H_ */
