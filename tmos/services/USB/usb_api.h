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
//#include <usb_device.h>
#include <usb_drv.h>


typedef RES_CODE (*USB_API_FUNC)(USB_DRV_INFO drv_info, HANDLE client);

#if USB_ENABLE_DEVICE
RES_CODE usb_api_device_config(USB_DRV_INFO drv_info, HANDLE client);
#endif

#if USB_ENABLE_HOST
RES_CODE usb_api_otg_config(USB_DRV_INFO drv_info, HANDLE client);
RES_CODE usb_api_otg_config2(USB_DRV_INFO drv_info, HANDLE client);
RES_CODE usb_api_otg_off(USB_DRV_INFO drv_info, HANDLE client);
RES_CODE usb_api_get_hdev(USB_DRV_INFO drv_info, HANDLE client);
RES_CODE usb_api_host_resume(USB_DRV_INFO drv_info, HANDLE client);
#endif


#define USB_CMD_DEVICE_CONFIG 	((void*)usb_api_device_config)

#if USB_ENABLE_HOST
#define USB_CMD_OTG_CONFIG 		((void*)usb_api_otg_config)
#define USB_CMD_OTG_CONFIG2 	((void*)usb_api_otg_config2)
#define USB_CMD_OTG_OFF			((void*)usb_api_otg_off)
#define USB_CMD_GET_HDEV		((void*)usb_api_get_hdev)
#define USB_CMD_HOST_RESUME		((void*)usb_api_host_resume)
#endif


#endif /* USB_API_H_ */
