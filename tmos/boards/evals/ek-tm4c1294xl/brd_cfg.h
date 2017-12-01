/*
 * tmos_cfg.h
 *
 *  Created on: 13.12.2011
 *      Author: Miroslav Kostadinov
 */


//* TMOS related configurations

#ifndef BRD_CFG_H_
#define BRD_CFG_H_


// GUI messages
#define WM_USER				0x01
#define WM_DRAW				0x02
#define WM_KEY				0x03
#define WM_INIT				0x04


#define USE_TMOS_TL_V2		1
#define USE_CSTRING			1

#define USE_TMOS_STRING		1
#define USE_TMOS_STDLIB		1
#define USE_TIME			0
#define USE_TMOS_STDIO		1
#define USE_TMOS_LLONG		0
#define USE_CRC16			0
#define USE_CRC32			0
#define USE_CACHE			0
#define USE_SHA_1			0
#define USE_BASE32			0
#define USE_GUI				0
#define USE_USB				0
#define USE_AES_TMOS		0
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1

#define USE_NET				0

#endif /* BRD_CFG_H_ */
