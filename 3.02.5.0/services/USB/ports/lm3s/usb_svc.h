/*
 * usb_svc.h
 *
 *  Created on: 07.07.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef USB_SVC_H_
#define USB_SVC_H_

#include <tmos.h>
#include <usb_drv.h>

typedef void (*USB_SVC_HOOK)(USB_DRV_INFO drv_info, HANDLE hnd);

void usb_svc_stall(HANDLE hnd);
void usb_svc_setconfiguration(HANDLE hnd, unsigned int cfg);
void usb_svc_configendpoint(HANDLE hnd, const USBGenericDescriptor* ds);
void usb_svc_setaddress(HANDLE hnd, unsigned int adr);
void usb_svc_halt(HANDLE hnd, unsigned int eptnum);
void usb_svc_unhalt(HANDLE hnd, unsigned int eptnum);


/**
 * This is suppose to send zero length packet for the status stage of control
 * transaction.
 * In fact it doesn't do anything... (LM3S specific )
 *
 */
#define usb_svc_send_control_status(hnd) while(0)

#endif /* USB_SVC_H_ */
