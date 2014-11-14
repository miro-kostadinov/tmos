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


#define ENTPOINT_FIFO(hw_base, eptnum) &hw_base->DFIFO[eptnum].DFIFO[0]

/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * for now since high-speed mode requires external PHY...
 */
#define usb_hal_IsHighSpeed() 0

extern "C" void usb_drv_event(USB_DRV_INFO drv_info, USB_EVENT event);
void usb_drv_reset(USB_DRV_INFO drv_info);
void usb_drv_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_drv_start_tx(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_drv_start_rx(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_drv_end_transfers(ep_dir_state_t* epdir, unsigned int status);

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t ept_num, uint16_t* data);
void usb_hal_stall(USB_TypeDef* hw_base, unsigned int ept_num);
void usb_hal_stall_clear(USB_TypeDef* hw_base, unsigned int ept_num);
void usb_hal_device_start(USB_DRV_INFO drv_info);
void usb_hal_ept_reset(USB_DRV_INFO drv_info, unsigned int ept_num);
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor);
void usb_hal_config_fifo(USB_DRV_INFO drv_info);
#if USB_ENABLE_HOST
EXTERN_C void usb_hal_host_power(USB_DRV_INFO drv_info, bool enable);
RES_CODE usb_hal_host_start(USB_DRV_INFO drv_info);
RES_CODE usb_hal_host_bus_reset(USB_DRV_INFO drv_info);
void usb_hal_host_resume(USB_DRV_INFO drv_info);
void usb_hal_host_ept_cfg(USB_DRV_INFO drv_info, const USBEndpointDescriptor* pDescriptor);
#endif

void USB_HS_WKUP_ISR(USB_DRV_INFO drv_info);
void USB_OTG_ISR(USB_DRV_INFO drv_info);
#if USB_STM32_DEDICATED_EP1
void USB_EP1_IN_ISR(USB_DRV_INFO drv_info);
void USB_EP1_OUT_ISR(USB_DRV_INFO drv_info);
#endif

#if USB_ENABLE_HOST
void usb_otg_set_flags(USB_DRV_INFO drv_info, uint32_t flags);
void usb_otg_clr_flags(USB_DRV_INFO drv_info, uint32_t flags);
#endif


#endif /* USB_HAL_H_ */
