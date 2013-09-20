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

#define KEY_ON				0x00
#define KEY_AMT				0x01
#define KEY_RA				0x02
#define KEY_RFN				0x03
#define KEY_HASHTAG			0x04
#define KEY_TOTAL			0x05
#define KEY_PLU				0x06
#define KEY_PO				0x07
#define KEY_PRC_PLUS		0x08
#define KEY_PRC_MINUS		0x09
#define KEY_STL				0x0A
#define KEY_CL				0x0B
#define KEY_7				0x0C
#define KEY_4				0x0D
#define KEY_1				0x0E
#define KEY_0				0x0F
#define KEY_VD				0x10
#define KEY_8				0x11
#define KEY_5				0x12
#define KEY_2				0x13
#define KEY_00				0x14
#define KEY_X				0x15
#define KEY_9				0x16
#define KEY_6				0x17
#define KEY_3				0x18
#define KEY_DP				0x19
#define KEY_PF				0x1A
#define KEY_DEP_4			0x1B
#define KEY_DEP_3			0x1C
#define KEY_DEP_2			0x1D
#define KEY_DEP_1			0x1E
#define KEY_CARD			0x20
#define KEY_CHECK			0x21
#define KEY_SHIFT			0x22
#define KEY_DEP_5			0x23

#define KEY_OK				KEY_TOTAL
#define KEY_ENTER			KEY_TOTAL
#define KEY_CANCEL			KEY_CL
#define KEY_UP				KEY_DEP_3
#define KEY_DOWN			KEY_DEP_2
#define KEY_LEFT			KEY_STL
#define KEY_RIGHT			KEY_DEP_5
#define KEY_ESC				KEY_ON

// GUI messages
#define WM_USER				0x01
#define WM_DRAW				0x02
#define WM_KEY				0x03
#define WM_INIT				0x04
#define WM_COMMAND			0x05
#define WM_CHANGE			0x06
#define WM_IDLE				0x07


#define USE_TMOS_TL_V2		1
#define USE_CSTRING			1
#define USE_TTL_STRING 		0	//use old CSTRING


#define USE_TMOS_STRING		1
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
#define USE_USB				1
#define USE_AES_TMOS		0
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1

#define GUI_DISPLAYS		2
#define GUI_DEFAULT_DISP	2

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEY_RD_COUNT		5
#define KEY_DRV_COUNT		7
#define KEY_FIX_COUNT		1

#define USE_ADC_DMA_DRIVER 1		//Enable ADC DMA


#endif /* BRD_CFG_H_ */
