/*
 * tmos_cfg.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */


//* TMOS related configurations

#ifndef BRD_CFG_H_
#define BRD_CFG_H_

#define KEY_DOWN_CODE		0x00
#define KEY_REPEAT_CODE		0x80
#define KEY_UP_CODE			0x40
#define KEY_PRESREP_MASK	0x7F

#define KEY_OK				0x00
#define KEY_UP				0x01
#define KEY_DOWN			0x02

// GUI messages
#define WM_USER				0x01
#define WM_DRAW				0x02
#define WM_KEY				0x03
#define WM_INIT				0x04


#define USE_TMOS_STRING		1
#define USE_CSTRING			1
#define USE_TMOS_STDLIB		1
#define USE_TIME			0
#define USE_TMOS_STDIO		1
#define USE_GUI				1


/*----------------------------------------------------------------------------*/
/**
 * \page sam3s_ek_usb "SAM3S-EK - USB device"
 *
 * \section Definitions
 * - \ref BOARD_USB_BMATTRIBUTES
 * - \ref CHIP_USB_UDP
 * - \ref CHIP_USB_PULLUP_INTERNAL
 * - \ref CHIP_USB_NUMENDPOINTS
 * - \ref CHIP_USB_ENDPOINTS_MAXPACKETSIZE
 * - \ref CHIP_USB_ENDPOINTS_BANKS
 */

/** USB attributes configuration descriptor (bus or self powered, remote wakeup) */
#define BOARD_USB_BMATTRIBUTES    USBConfigurationDescriptor_SELFPOWERED_RWAKEUP


#endif /* BRD_CFG_H_ */
