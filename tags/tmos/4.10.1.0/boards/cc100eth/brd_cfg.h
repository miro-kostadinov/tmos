/*
 * tmos_cfg.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */


//* TMOS related configurations

#ifndef BRD_CFG_H_
#define BRD_CFG_H_


#define HW_VER_11			1

#define HW_KEY_VER		    2




#define KEY_DOWN_CODE		0x00
#define KEY_REPEAT_CODE		0x80
#define KEY_UP_CODE			0x40
#define KEY_PRESREP_MASK	0x7F

#define KEY_ON				0x00
#define KEY_LEFT			0x00
#define KEY_DOWN			0x02
#define KEY_UP				0x01
#define KEY_NO				0x03
#define KEY_RIGHT			0x03

#if HW_KEY_VER == 1
#define KEY_OK				0x04
#define KEY_C				0x05
#endif

#if HW_KEY_VER == 2
#define KEY_OK				0x05
#define KEY_C				0x04
#endif

#define ENABLE_INDEX		1
#define DISABLE_INDEX		2
#define MENU_INDEX			ENABLE_INDEX

// GUI messages
#define WM_USER				0x01
#define WM_DRAW				0x02
#define WM_KEY				0x03
#define WM_INIT				0x04


#define USE_TMOS_TL_V1		1
#define USE_CSTRING			1

#define USE_TMOS_STRING		1
#define USE_TMOS_STDLIB		1
#define USE_TIME			1
#define USE_TMOS_STDIO		1
#define USE_CRC16			1
#define USE_CRC32			1
#define USE_CACHE			1
#define USE_SHA_1			1
#define USE_GUI				1
#define	USE_GPIO_EXPANDER 	1
#define USE_LWIP_1_4		1

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEY_COUNT			6

#define KEY_POLLING_TIME	10
#define KEY_DEBOUNCE_TIME 	50 //30 //300
#define KEY_REPEAT_TIME 	300//150 //1000

/// USB stack options
#define USE_USB				1
#define USB_ENABLE_DEVICE	1
#define USB_ENABLE_HOST 	0
#define USB_ENABLE_OTG 		0
#define USB_ENABLE_CDC		1
#define USB_ENABLE_PRN		0
#define USB_ENABLE_HID		0

#endif /* BRD_CFG_H_ */
