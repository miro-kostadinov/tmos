/*
 * usb_hal.h
 *
 *  Created on: Dec 3, 2012
 *      Author: miro
 */

#ifndef USB_HAL_H_
#define USB_HAL_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>

/** Enable interrupt for endpoint
 *
 * Not implemented from STM32 ????
 */
#define ENTPOINT_ENABLE_INT(hw_base, eptnum) while(0)

#define ENTPOINT_FIFO(hw_base, eptnum) &hw_base->DFIFO[eptnum].DFIFO[0]

/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * for now since high-speed mode requires external PHY...
 */
#define usb_hal_IsHighSpeed() 0

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t eptnum, uint16_t* data);
void usb_end_transfers(ep_dir_state_t* epdir, unsigned int status);
void usb_hal_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd);

void usb_start_tx(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_start_rx(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, Endpoint *endpoint);

void usb_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum);
void usb_ack_packet(USB_TypeDef* hw_base, Endpoint* endpoint, unsigned int eptnum);
void usb_hal_reset(USB_DRV_INFO drv_info);
void usb_hal_configure(USB_DRV_INFO drv_info);
void usb_hal_stall(USB_TypeDef* hw_base, unsigned int eptnum);
void usb_hal_stall_clear(USB_TypeDef* hw_base, unsigned int eptnum);
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor);
void usb_hal_txpktrdy(USB_TypeDef* hw_base, unsigned int eptnum, int len);

void usb_hal_config_fifo(USB_DRV_INFO drv_info);

void USB_HS_WKUP_ISR(USB_DRV_INFO drv_info);
void USB_OTG_ISR(USB_DRV_INFO drv_info);
void USB_EP1_IN_ISR(USB_DRV_INFO drv_info);
void USB_EP1_OUT_ISR(USB_DRV_INFO drv_info);


#endif /* USB_HAL_H_ */
