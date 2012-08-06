/**
 * @ingroup	 drv_at91
 * @defgroup DRV_AT91_USBD USB Device Port (UDP) Driver
 * Interface for USB Device Port (UDP) driver.
 *
 * AT91 initialization:
 *
 * main.c > CDCDSerialDriver_Initialize(&cdcdSerialDriverDescriptors);
 *
 * 	CDCDSerialDriver_Initialize(pDescriptors):
 * 		USBDDriver_Initialize(pUsbd, pDescriptors, 0);
 * 		CDCDSerial_Initialize(pUsbd, CDCDSerialDriver_CC_INTERFACE);
 * 			 CDCDSerialPort_Initialize()
 * 		USBD_Init();
 *
 *
 * @{
 *
 *
 * @file     drivers/at91/usbd/usbd_drv.h
 * @ingroup	 DRV_AT91_USBD
 * @brief    USBD driver header
 *
 * @date     26. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef USB_DRV_H_
#define USB_DRV_H_

#include <tmos.h>
#include <usbd.h>
#include <series_cpp.h>
#include <USBDDriver.h>

/** USB status ReturnCode */
typedef enum _USBRC {
    USBRC_OK = 0,      /**< Operation was successful */
    USBRC_SUCCESS = 0, /**< Operation was successful */
    /* Bool codes */
    USBRC_FALSE = 0,   /**< As boolean TRUE */
    USBRC_TRUE  = 1,   /**< As boolean FALSE */
    /* Error codes */
    USBRC_BUSY,        /**< EP/Device is already busy */
    USBRC_ABORTED,     /**< Operation aborted due to error or stall */
    USBRC_CANCELED,    /**< Operation canceled by user */
    USBRC_RESET,       /**< Operation aborted due to init/reset/un-configure */
    USBRC_PARTIAL_DONE,/**< Part of operation successfully done */
    USBRC_FINISHED,    /**< All operation successfully done and terminate */

    USBRC_PARAM_ERR,   /**< Failed due to parameter error */
    USBRC_STATE_ERR,   /**< Failed due to state error */
    USBRC_ERROR,       /**< General error */

    USBRC_SW_NOT_SUPPORTED = 0xFD, /**< Failed due to SW not supported */
    USBRC_HW_NOT_SUPPORTED = 0xFE  /**< Failed due to HW not supported */
} USBRC;

#define USB_SETUP_READ_TOUT		200		//!< read timeout for setups
#define USB_SETUP_WRITE_TOUT	200		//!< write timeout for setups

#define DCR_USB_STALL					0x01	//!< doc
#define DCR_USB_SET_ADDRESS				0x02	//!< doc
#define DCR_USB_SET_CONFIGURATION		0x03	//!< doc
#define DCR_USB_CONFIGUREENDPOINT		0x04	//!< doc
#define DCR_USB_HALT					0x05	//!< doc
#define DCR_USB_UNHALT					0x06	//!< doc


/*    Constants: Endpoint states	*/
#define UDP_ENDPOINT_DISABLED       0 //!< Endpoint is disabled.
#define UDP_ENDPOINT_HALTED         1 //!< Endpoint is halted (i.e. STALLs every request).
#define UDP_ENDPOINT_IDLE           2 //!< Endpoint is idle (i.e. ready for transmission).
#define UDP_ENDPOINT_SENDING        4 //!< Endpoint is sending data.
#define UDP_ENDPOINT_RECEIVING      8 //!< Endpoint is receiving data.
#define UDP_ENDPOINT_RECEIVING_OFF 16 //!< Endpoint is receiving data (rxfifo_cnt).
#define UDP_ENDPOINT_BUSY		(UDP_ENDPOINT_SENDING | UDP_ENDPOINT_RECEIVING)//!< doc

/*   Constants: USB device API return values */
#define USBD_STATUS_SUCCESS	  RES_SIG_OK	//!< Indicates the operation was successful.
#define USBD_STATUS_LOCKED    RES_SIG_EOF	//!< Endpoint/device is already busy.
#define USBD_STATUS_ABORTED   RES_SIG_IDLE	//!< Operation has been aborted.
#define USBD_STATUS_RESET     RES_SIG_ERROR //!< Operation has been aborted because the device has been reset.

/*   Constants: USB device states		*/
#define USBD_STATE_SUSPENDED    0 //!< The device is currently suspended.
#define USBD_STATE_ATTACHED     1 //!< USB cable is plugged into the device.
#define USBD_STATE_POWERED      2 //!< Host is providing +5V through the USB cable.
#define USBD_STATE_DEFAULT      3 //!< Device has been reset.
#define USBD_STATE_ADDRESS      4 //!< The device has been given an address on the bus.
#define USBD_STATE_CONFIGURED	5 //!< A valid configuration has been selected.

/** Endpoint data structure */
struct Endpoint
{
	unsigned char state;		//!< endpoint state
	unsigned char bank;			//!< current RAM bank
	unsigned short size;		//!< endpoint size
	unsigned int rxfifo_cnt;	//!< bytes in rxfifo
	HANDLE pending;				//!< pending handles
};

/** USBD Driver Data structure **/
struct USBD_DRIVER_DATA
{
	unsigned char	deviceState;			//!< USBD_STATE_XXX
	unsigned char	previousDeviceState;	//!< previous deviceState
	Endpoint		endpoints[CHIP_USB_NUMENDPOINTS]; //!< Endpoint structures
	USBDDriver		usbdDriver;		//!< USBD driver
};

/** USBD Driver Mode structure **/
struct DRV_USBD_MODE_STRU
{
	unsigned int	baudrate;	//!< baudrate for the mode
};
/** UART Mode */
typedef const DRV_USBD_MODE_STRU * DRV_USBD_MODE;

/** USBD Driver Info structure **/
struct USBD_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;		//!< standard driver info
	Udp *				hw_base;	//!< pointer to the hardware peripheral
	USBD_DRIVER_DATA* 	drv_data;	//!< pointer to the driver data
	const USBDDriverDescriptors * drv_descriptors;	//!< device descriptors
};
/** USART Driver Info */
typedef const USBD_DRIVER_INFO* USBD_INFO;

void USBD_DCR(USBD_INFO drv_info, unsigned int reason, HANDLE param);
void USBD_DSR(USBD_INFO drv_info, HANDLE hnd);
void USBD_ISR(USBD_INFO drv_info );

void USBD_Stall(HANDLE hnd);

/** called to isnstall a minidriver **/
extern "C" void usb_install_minidrv(USBD_INFO drv_info);


#endif /* USB_DRV_H_ */
