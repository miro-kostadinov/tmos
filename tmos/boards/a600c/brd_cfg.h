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
#define KEY_REPEAT_CODE		0x8000
#define KEY_UP_CODE			0x4000
#define KEY_ASCII_CODE		0x0100
#define KEY_SCAN_CODE		0x0200
#define KEY_PRESREP_MASK	0x7FFF
#define KEY_STATE_MASK		(KEY_REPEAT_CODE|KEY_UP_CODE)

/*
#define KEY_ON				0x00         //KEY_ON
#define KEY_D_PLUS			0x01         //KEY_AMT
#define KEY_HASHTAG			0x02         //KEY_RA
#define KEY_STAR			0x03         //KEY_RFN
//#define KEY_SHIFT			0x04         //KEY_HASHTAG
#define KEY_TOTAL			0x05         //KEY_TOTAL
#define KEY_PLU				0x06         //KEY_PLU
#define KEY_RFN				0x07         //KEY_PO
#define KEY_PRC_PLUS		0x08         //KEY_PRC_PLUS
#define KEY_PRC_MINUS		0x09         //KEY_PRC_MINUS
#define KEY_STL				0x0A         //KEY_STL
#define KEY_CL				0x0B         //KEY_CL
//#define KEY_7				0x0C         //KEY_7
//#define KEY_4				0x0D         //KEY_4
//#define KEY_1				0x0E         //KEY_1
//#define KEY_X				0x0F         //KEY_0
#define KEY_VD				0x10         //KEY_VD
//#define KEY_8				0x11         //KEY_8
//#define KEY_5				0x12         //KEY_5
//#define KEY_2				0x13         //KEY_2
//#define KEY_0				0x14         //KEY_00
#define KEY_AMT				0x15         //KEY_X
//#define KEY_9				0x16         //KEY_9
//#define KEY_6				0x17         //KEY_6
//#define KEY_3				0x18         //KEY_3
//#define KEY_DP				0x19         //KEY_DP
#define KEY_PF				0x1A         //KEY_PF
#define KEY_DEP_4			0x1B         //KEY_DEP_4
#define KEY_DEP_3			0x1C         //KEY_DEP_3
#define KEY_DEP_2			0x1D         //KEY_DEP_2
#define KEY_DEP_1			0x1E         //KEY_DEP_1
#define KEY_NC				0x1F
#define KEY_PY2				0x20         //KEY_CARD
#define KEY_PY1				0x21         //KEY_CHECK
#define KEY_DEP_6			0x22         //KEY_SHIFT
#define KEY_DEP_5			0x23         //KEY_DEP_5
*/
/*
#define KEY_OK				KEY_TOTAL
#define KEY_ENTER			KEY_TOTAL
#define KEY_CANCEL			KEY_CL
#define KEY_C				KEY_CL
#define KEY_UP				KEY_DEP_3
#define KEY_DOWN			KEY_DEP_2
#define KEY_LEFT			KEY_STL
#define KEY_RIGHT			KEY_DEP_5
#define KEY_ESC				KEY_ON
*/

#define USE_TMOS_TL_V2		1
#define USE_CSTRING			1


#define USE_TMOS_STRING		1
#define USE_TMOS_STDLIB		1
#define USE_TIME			1
#define USE_TMOS_STDIO		1
#define USE_TMOS_LLONG		1
#define USE_CRC16			0
#define USE_CRC32			1
#define USE_CACHE			1
#define USE_SHA_1			1
#define USE_BASE32			0
#define USE_GUI				0
#define USE_GUI2			1
#define USE_AES_TMOS		0
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1
#define USE_TASK_REGISTERS	1

#define GUI_DISPLAYS		2
#define GUI_DEFAULT_DISP	2

#define URL_CUSTOM_ROUTING	1

#define USE_CPU_SLEEP_MODE	1

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 KEY DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define KEY_RD_COUNT		5
#define KEY_DRV_COUNT		7
#define KEY_FIX_COUNT		1
#define KEY_DRV_STACK_SIZE (70)

#define USE_ADC_DMA_DRIVER 1		//Enable ADC DMA

/// USB stack options
#define USE_USB				1
#define USB_ENABLE_DEVICE	1
#define USB_ENABLE_HOST 	1
#define USB_ENABLE_OTG 		1
#define USB_ENABLE_CDC		1
#define USB_ENABLE_PRN		0
#define USB_ENABLE_HID		1

#endif /* BRD_CFG_H_ */
