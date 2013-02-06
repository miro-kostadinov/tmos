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


typedef RES_CODE (*USB_API_FUNC)(USB_DRV_INFO drv_info, HANDLE client);


RES_CODE usb_api_device_config(USB_DRV_INFO drv_info, HANDLE client);


#define USB_CMD_DEVICE_CONFIG 	((void*)usb_api_device_config)


#endif /* USB_API_H_ */
