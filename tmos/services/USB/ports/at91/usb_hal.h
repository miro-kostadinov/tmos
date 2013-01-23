
#ifndef USB_HAL_H_
#define USB_HAL_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_drv.h>


/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * for now since high-speed mode requires external PHY...
 */
#define usb_hal_IsHighSpeed() 0


void SET_CSR(RwReg* csr, uint32_t flags);
void CLEAR_CSR(RwReg* csr, uint32_t flags);

extern "C" void usb_drv_event(USB_DRV_INFO drv_info, USB_EVENT event);
void usb_drv_reset(USB_DRV_INFO drv_info);
void usb_drv_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_drv_start_tx(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_drv_start_rx(USB_DRV_INFO drv_info, HANDLE hnd);
HANDLE usb_drv_end_transfers(Endpoint *endpoint, unsigned int status);

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t ept_num, uint16_t* data);
void usb_hal_stall(Udp* hw_base, unsigned int ept_num);
void usb_hal_stall_clear(Udp* hw_base, unsigned int ept_num);
void usb_hal_configure(USB_DRV_INFO drv_info);
void usb_hal_ept_reset(USB_DRV_INFO drv_info, unsigned int ept_indx);
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor);

#define usb_hal_config_fifo(drv_info) while(0)

void USBD_ISR(USB_DRV_INFO drv_info);


#endif /* USB_HAL_H_ */
