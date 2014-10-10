/*
 * usb_svc.h
 *
 *  Created on: Dec 3, 2012
 *      Author: miro
 */

#ifndef USB_SVC_H_
#define USB_SVC_H_

#include <tmos.h>
#include <usb_drv.h>

typedef void (*USB_SVC_HOOK)(USB_DRV_INFO drv_info, HANDLE hnd);

void usb_svc_stall(HANDLE hnd);
void usb_svc_setconfiguration(HANDLE hnd, unsigned int cfg);
void usb_svc_configendpoint(HANDLE hnd, const USBGenericDescriptor* ds, int bCfgFifo=0);
void usb_svc_setaddress(HANDLE hnd, unsigned int adr);
void usb_svc_halt(HANDLE hnd, unsigned int eptnum);
void usb_svc_unhalt(HANDLE hnd, unsigned int eptnum);

/**
 * Send zero length packet for the status stage of control transaction
 */
#define usb_svc_send_control_status(hnd) hnd->tsk_write(NULL, 0, USB_SETUP_WRITE_TOUT)

#endif /* USB_SVC_H_ */
