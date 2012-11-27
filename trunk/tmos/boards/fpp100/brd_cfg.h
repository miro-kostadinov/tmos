/*
 * tmos_cfg.h
 *
 *  Created on: 13.12.2011
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
#define KEY_DOWN			0x01
#define KEY_UP				0x02
#define KEY_C				0x03

// GUI messages
#define WM_USER				0x01
#define WM_DRAW				0x02
#define WM_KEY				0x03
#define WM_INIT				0x04


#define USE_TMOS_STRING		1
#define USE_CSTRING			1
#define USE_TMOS_STDLIB		1
#define USE_TIME			1
#define USE_TMOS_STDIO		1
#define USE_TMOS_LLONG		1
#define USE_CRC16			0
#define USE_CRC32			1
#define USE_CACHE			0
#define USE_SHA_1			1
#define USE_BASE32			0
#define USE_GUI				1
#define USE_USB				0
#define USE_AES_TMOS		0
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1

#define GUI_DISPLAYS		2
#define GUI_DEFAULT_DISP	2

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEY_COUNT			4

#define KEY_POLLING_TIME	10
#define KEY_DEBOUNCE_TIME 	50 //30 //300
#define KEY_REPEAT_TIME 	300//150 //1000

#endif /* BRD_CFG_H_ */
