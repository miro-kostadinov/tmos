
#ifndef USB_HAL_H_
#define USB_HAL_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_lm3s.h>
#include <usb_drv.h>


/** Enable interrupt for endpoint
 *
 * Not implemented from LM3S ????
 */
#define ENTPOINT_ENABLE_INT(hw_base, eptnum) while(0)

/** Enable interrupt for endpoint
 *
 * Not implemented from LM3S ????
 */
#define ENTPOINT_IS_PING_PONG(hw_base, eptnum) 					\
	((hw_base->USBTXDPKTBUFDIS & (1<<eptnum)))

#define ENTPOINT_FIFO(hw_base, eptnum) &hw_base->USBFIFO[eptnum]

/** Force Stall on endpoint  */
#define FORCE_STALL(hw_base, eptnum) SET_CSR(&hw_base->UDP_CSR[eptnum], UDP_CSR_FORCESTALL);

/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * since UDP does not support high-speed mode.
 */
#define usb_hal_IsHighSpeed() 0

bool usb_hal_get_ep_status(USB_DRV_INFO drv_info, uint8_t eptnum, uint16_t* data);
void usb_end_transfers(Endpoint* endpoint, unsigned int status);
void usb_hal_cancel_hnd(USB_DRV_INFO drv_info, HANDLE hnd);

void usb_start_tx(USB_DRV_INFO drv_info, HANDLE hnd);
void usb_start_rx(USB_DRV_INFO drv_info, HANDLE hnd, uint32_t eptnum, Endpoint *endpoint);

void usb_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum);
void usb_ack_packet(USB_Type* hw_base, Endpoint* endpoint, unsigned int eptnum);
void usb_hal_reset(USB_DRV_INFO drv_info);
void usb_hal_configure(USB_DRV_INFO drv_info);
void usb_hal_stall(USB_Type* hw_base, unsigned int eptnum);
void usb_hal_stall_clear(USB_Type* hw_base, unsigned int eptnum);
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor);

void usb_hal_config_fifo(USB_DRV_INFO drv_info);


#endif /* USB_HAL_H_ */
