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

/// USB device states
enum USB_DRIVER_STATE : unsigned char
{
	USB_STATE_DOWN,  	  	//!< The USB is down
	USB_STATE_SUSPENDED,	//!< The device is currently suspended.
	USB_STATE_ATTACHED,		//!< USB cable is plugged into the device.
	USB_STATE_POWERED,		//!< Host is providing +5V through the USB cable.
	USB_STATE_DEFAULT,		//!< Device has been reset.
	USB_STATE_ADDRESS,		//!< The device has been given an address on the bus.
	USB_STATE_CONFIGURED	//!< A valid configuration has been selected.
};

/// USB OTG modes
enum USB_OTG_MODE : unsigned char
{
	USB_MODE_NONE			=0x00,	//!< The mode is not allowed
	USB_MODE_ENABLED		=0x01,	//!< The mode is enabled.
	USB_MODE_STARTING		=0x02,	//!< The mode is starting.
	USB_MODE_ACTIVE			=0x04	//!< The mode is active.
};


#define USB_POWER_REMOTE_WAKE_ACTIVE	0x01  //!< Remote wake up in progress
#define USB_POWER_REMOTE_WAKE_ENABLED	0x02  //!< Remote wake up is enabled
#define USB_POWER_WAKING				0x03

/*    Constants: Endpoint states	*/
#define ENDPOINT_STATE_DISABLED       0x00 //!< Endpoint is disabled.
#define ENDPOINT_STATE_HALTED         0x01 //!< Endpoint is halted (i.e. STALLs every request).
#define ENDPOINT_STATE_IDLE           0x02 //!< Endpoint is idle (i.e. ready for transmission).
#define ENDPOINT_STATE_SENDING        0x04 //!< Endpoint is sending data.
#define ENDPOINT_STATE_RECEIVING      0x08 //!< Endpoint is receiving data.
#define ENDPOINT_STATE_RECEIVING_OFF  0x10 //!< Endpoint is receiving data (rxfifo_cnt).
#define ENDPOINT_STATE_STALL		  0x20 //!< Endpoint is receiving data (rxfifo_cnt).
#define ENDPOINT_STATE_BUSY		(UDP_ENDPOINT_SENDING | UDP_ENDPOINT_RECEIVING)

/*   Constants: USB device API return values */
#define USBD_STATUS_SUCCESS	  RES_SIG_OK	//!< Indicates the operation was successful.
#define USBD_STATUS_LOCKED    RES_SIG_EOF	//!< Endpoint/device is already busy.
#define USBD_STATUS_ABORTED   RES_SIG_IDLE	//!< Operation has been aborted.
#define USBD_STATUS_RESET     RES_SIG_ERROR //!< Operation has been aborted because the device has been reset.

enum USB_EVENT
{
	e_init,						//!< Init after Driver (DCR) RESET
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


#define USB_HANDLE_MODE(hd, cfg, inf, ept) \
	(void*)(((hd)<<24)|((cfg)<<16)|((inf<<8))|(ept))
#define USB_DEFAULT_MODE USB_HANDLE_MODE(1,1,1,1)

struct USB_DRIVER_DATA
{
	USB_DRIVER_STATE usb_state;		//!< Current device state (USBD_STATE_XXX)
	USB_OTG_MODE usb_device_mode;	//!< Device mode
	USB_OTG_MODE usb_host_mode;		//!< Host mode
	unsigned char 	usb_power;		//!< Power status
//	unsigned char	dev_adr;

	unsigned int 	frame_count;
	unsigned int 	wake_count;
	HANDLE			helper;
	HANDLE			waiting;
	HANDLE			pending;		//!< waiting for connections etc
	usb_device		device;
	Endpoint		endpoints[USB_NUMENDPOINTS]; //!< Endpoint structures

	USB_DRIVER_DATA() {};
	virtual ~USB_DRIVER_DATA() {};

	//usb API functions
	void usb_shutdown();
	void usb_configure(const USBDDriverDescriptors *pDescriptors);

	virtual	void usb_event(USB_EVENT event);
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

void USB_DCR(USB_DRV_INFO drv_info, unsigned int reason, HANDLE param);
void USB_DSR(USB_DRV_INFO drv_info, HANDLE hnd);
void USB_B_ISR(USB_DRV_INFO drv_info);


#endif /* USB_DRV_H_ */
