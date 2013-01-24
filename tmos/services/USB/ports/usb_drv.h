/**
 * @ingroup	 services
 * @defgroup SVC_USB USB Driver
 * Interface for USB  driver.
 *
 *
 *
 * @{
 *
 *
 * @file     services/USB/usb_drv.h
 * @ingroup	 SVC_USB
 * @brief    USB driver header
 *
 * @date     23. May 2011
 * @author	 Miroslav Kostadinov
 * @}
*/

#ifndef USB_DRV_H_
#define USB_DRV_H_

#include <tmos.h>
#include <usb_opt.h>
#include <usb_descriptors.h>
#include <usb_device.h>

#define DCR_USB_STALL					0x01

/// USB states
#define	USBST_ALL_DOWN			0x00	//!< The USB is down (no host, no device)

#define	USBST_DEVICE_MODE		0x40	//!< The USB is in device mode
#define USBST_DEVICE_POWERED	0x41	//!< Host is providing +5V through the USB cable
#define USBST_DEVICE_SUSPENDED	0x42	//!< The device is currently suspended
#define USBST_DEVICE_DEFAULT	0x43	//!< Device has been reset
#define	USBST_DEVICE_ADDRESS	0x44	//!< The device has been given an address on the bus
#define	USBST_DEVICE_CONFIGURED	0x45	//!< A valid configuration has been selected

#define	USBST_HOST_MODE			0x80	//!< The USB is in host mode


/*    Constants: Endpoint states	*/
#define ENDPOINT_STATE_DISABLED       0x00 //!< Endpoint is disabled.
#define ENDPOINT_STATE_HALTED         0x01 //!< Endpoint is halted (i.e. STALLs every request).
#define ENDPOINT_STATE_IDLE           0x02 //!< Endpoint is idle (i.e. ready for transmission).
#define ENDPOINT_STATE_SENDING        0x04 //!< Endpoint is sending data.
#define ENDPOINT_STATE_RECEIVING      0x08 //!< Endpoint is receiving data.
#define ENDPOINT_STATE_RECEIVING_OFF  0x10 //!< Endpoint is receiving data (rxfifo_cnt).
#define ENDPOINT_STATE_STALL		  0x20 //!< Endpoint is receiving data (rxfifo_cnt).

/*   Constants: USB device API return values */
#define USBD_STATUS_ABORTED   RES_SIG_IDLE	//!< Operation has been aborted (stalled)
#define USBD_STATUS_RESET     RES_SIG_ERROR //!< Operation has been aborted because the device has been reset

enum USB_EVENT
{
	e_powered,
	e_reset,					//!< USB bus reset signaled
	e_susppend,
	e_resume,
	e_disconnect,
	e_wakeup
};

#ifndef USB_NUMENDPOINTS
#define USB_NUMENDPOINTS 16
#endif

#if USB_NUMENDPOINTS > 16
#error "Maximum 16 endpoints supported from the stack"
#endif


#define USB_DRV_MODE(tx_ep, rx_ep) ((const void*)(((tx_ep)<< 8) | (rx_ep)))

struct USB_DRIVER_DATA
{
	uint8_t		 	usb_state;			//!< Current usb state (USBST_XXX)
	uint8_t		 	usb_previous_state;	//!< Previous usb state (USDST_XXX)
//	unsigned char	dev_adr;

	unsigned int 	frame_count;
	unsigned int 	wake_count;
	HANDLE			helper;
	HANDLE			waiting;
	HANDLE			pending;			//!< waiting for connections etc
	usb_device		device;
	Endpoint		endpoints[USB_NUMENDPOINTS]; //!< Endpoint structures
};

struct USB_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	USB_CONTROLLER *	hw_base;	//!< pointer to the hardware peripheral
	USB_DRIVER_DATA* 	drv_data;	//!< pointer to the driver data
	const USBDDriverDescriptors * dev_descriptors;
	const usb_config_t*		cfg;		//!< configuration port-specific parameters
};

typedef const USB_DRIVER_INFO* USB_DRV_INFO;

void usbdrv_thread(USB_DRV_INFO drv_info);

void USB_DCR(USB_DRV_INFO drv_info, unsigned int reason, HANDLE param);
void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd);
void USB_B_ISR(USB_DRV_INFO drv_info);


#endif /* USB_DRV_H_ */
