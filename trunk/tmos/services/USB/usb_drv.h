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
#if USB_ENABLE_DEVICE
#include <usb_device.h>
#endif

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
#define ENDPOINT_STATE_NYET			  0x40 //!< Endpoint is not ready
#define ENDPOINT_STATE_ERR			  0x80 //!< Endpoint has error

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

#if USB_ENABLE_OTG
#define USB_OTG_FLG_DEV			0x0001	//!< Device mode requested. Set on USB_CMD_DEVICE_CONFIG. Cleared
#define USB_OTG_FLG_DEV_CON		0x0002	//!< Device detected. Set on USBIDVISC, BRST
#define USB_OTG_FLG_DEV_OK		0x0004	//!< Device mode entered. Set on BRST
#define USB_OTG_FLG_HOST		0x0008	//!< Host mode requested. Set on USB_CMD_OTG. Cleared on USBIDVISC, vbus droop, DISCON, SUSPEND, BRST
#define USB_OTG_FLG_HOST_CON	0x0010	//!< Host connected. Set on USB_CMD_OTG. Cleared on !FLG_HOST
#define USB_OTG_FLG_HOST_PWR	0x0020	//!< Host power enabled. Set on USBIDVISC. Cleared on !FLG_HOST
#define USB_OTG_FLG_HOST_RST	0x0040	//!< Set from interrupt to tell helper that it can reset the bus
#define USB_OTG_FLG_HOST_OK		0x0080	//!< Host mode entered. Set on HBRST. Cleared on !FLG_HOST
#define USB_OTG_FLG_SUSPEND		0x0100
#define USB_OTG_FLG_WKUP		0x0200
#define USB_OTG_FLG_ALL			0x03FF

#define OTG_H_SIG_RST			0x01
#define OTG_H_SIG_RESUME		0x02
#define OTG_H_SIG_CON			0x04
#endif
//TODO: clear DEV, DEV_CONECT


///** USB state counter
// * This a 16 bit counter which is updated when the driver state changes.
// */
#define USB_STATE_CNT_INVALID	1		//!< when OTG state is in progress the LSB bit is set

#ifndef USB_NUMENDPOINTS
#define USB_NUMENDPOINTS 16
#endif

#if USB_NUMENDPOINTS > 16
#error "Maximum 16 endpoints supported from the stack"
#endif


#define USB_DRV_MODE(tx_ep, rx_ep) ((const void*)(((tx_ep)<< 8) | (rx_ep) | 0x80000000))

#if USB_ENABLE_HOST
struct usb_hub_port_t
{
	uint8_t dev_adress;    //!< current address
	uint8_t dev_interface; //!< current interface
	uint8_t hub_num;		//!< hub number to which this device is attached
	uint8_t hub_port;		//!< hub port to which this device is attached
};
#endif

struct USB_DRIVER_DATA
{
	HANDLE			helper;
	HANDLE			waiting;			//!< waiting for the helper thread
	HANDLE			pending;			//!< waiting for connections etc
	Task* 			helper_task;
	Endpoint		endpoints[USB_NUMENDPOINTS]; //!< Endpoint structures
#if USB_ENABLE_OTG
	uint16_t		drv_state_cnt;		//!< session counter
	uint16_t		otg_flags;
#endif

#if USB_ENABLE_DEVICE
	uint8_t		 	usb_state;			//!< Current usb state (USBST_XXX)
	uint8_t		 	usb_previous_state;	//!< Previous usb state (USDST_XXX)
	usb_device		device;
#endif

#if USB_ENABLE_HOST
	uint32_t		otg_h_sig;
	usb_hub_port_t 	usb_hub[MAX_HUB_PORTS + 1]; //!< More than 1 device can be supported if hub present
#endif

};

struct USB_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	USB_CONTROLLER *	hw_base;	//!< pointer to the hardware peripheral
	USB_DRIVER_DATA* 	drv_data;	//!< pointer to the driver data
	const USBDDriverDescriptors * dev_descriptors;
	const usb_config_t*	cfg;		//!< configuration port-specific parameters
	const char*			usb_name;
};

#if TRACE_USB_LEVEL >= TRACE_LEVEL_DEBUG
#define TRACE_USB_NAME(drv)	TRACELN1_USB(""); TRACE1_USB(drv->usb_name)
#else
#define TRACE_USB_NAME(drv)
#endif

typedef const USB_DRIVER_INFO* USB_DRV_INFO;

#define USB_DRIVER_SIG	1

void usbdrv_thread(USB_DRV_INFO drv_info);

void USB_DCR(USB_DRV_INFO drv_info, unsigned int reason, HANDLE param);
void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd);
void USB_ISR(USB_DRV_INFO drv_info);


#endif /* USB_DRV_H_ */
