
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

/// USB pins defined from the application
extern const unsigned int usb_pins[];

/**
 * Indicates if the device is running in high or full-speed. Always returns 0
 * since UDP does not support high-speed mode.
 */
#define usb_hal_IsHighSpeed() 0

void usb_write_payload(volatile void* dst, HANDLE hnd,	unsigned int size);
unsigned int usb_read_payload(volatile void* src, HANDLE hnd, unsigned int size);

void usb_ept_reset(USB_DRV_INFO drv_info, unsigned int eptnum);
void usb_ack_packet(USB_Type* hw_base, Endpoint* endpoint, unsigned int eptnum);
void usb_hal_reset(USB_DRV_INFO drv_info);
void usb_hal_configure(USB_DRV_INFO drv_info);
void usb_hal_stall(USB_Type* hw_base, unsigned int eptnum, int is_in_dir);
void usb_hal_stall_clear(USB_Type* hw_base, unsigned int eptnum, int is_in_dir);
void usb_hal_ept_config(USB_DRV_INFO drv_info, const USBGenericDescriptor* pDescriptor);
void usb_hal_txpktrdy(USB_Type* hw_base, unsigned int eptnum, int len);

void usb_hal_config_fifo(USB_DRV_INFO drv_info);


#endif /* USB_HAL_H_ */
