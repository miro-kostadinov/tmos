/*
 * tmos_cfg.h
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */


//* TMOS related configurations

#ifndef BRD_CFG_H_
#define BRD_CFG_H_

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
#define USE_GUI				0
#define USE_GPIO_EXPANDER 	0
#define USE_NET				1

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1

/// USB stack options
#define USE_USB				1
#define USB_ENABLE_DEVICE	1
#define USB_ENABLE_HOST 	0
#define USB_ENABLE_OTG 		0
#define USB_ENABLE_CDC		1
#define USB_ENABLE_PRN		0
#define USB_ENABLE_HID		0


#endif /* BRD_CFG_H_ */
