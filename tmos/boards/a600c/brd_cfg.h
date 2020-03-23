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
#define USE_BASE32			1
#define USE_GUI				0
#define USE_GUI2			1
#define USE_AES_TMOS		1
#define STATIC_AES_TABLES	1
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

#define USE_ADC_DMA_DRIVER 1		//Enable ADC DMA

/// USB stack options
#define USE_USB				1
#define USB_ENABLE_DEVICE	1
#define USB_ENABLE_HOST 	1
#define USB_ENABLE_OTG 		1
#define USB_ENABLE_CDC		1
#define USB_ENABLE_PRN		1
#define USB_ENABLE_HID		1
#define USB_ENABLE_MSC		1
#define USB_ENABLE_VID		1

/// Ethernet
#define USE_NET				1
#define USE_LWIP_1_4_1		1
#define ETH_PHY_TLK110		1
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		USART DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define USART_DRV_RX_BUF_SIZE	(1024)

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Wi-Fi ESP8266 DRIVER
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define USE_WIFI 			1
#define USE_WIFI_ESP8266	3
#define WIFI_FLOW_CONTROL   0	// use AT+CIPRECVMODE

#define USE_SECURITY		1
#define USE_SECURITY_TLS	1
#define USE_SECURITY_CRYPTO	1
#define USE_SECURITY_TLS_DRV 1

#endif /* BRD_CFG_H_ */
