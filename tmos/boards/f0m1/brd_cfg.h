/*
 * tmos_cfg.h
 *
 *  Created on: 13.12.2011
 *      Author: Miroslav Kostadinov
 */


//* TMOS related configurations

#ifndef BRD_CFG_H_
#define BRD_CFG_H_



#define USE_TMOS_TL_V2		0
#define USE_CSTRING			0

#define USE_TMOS_STRING		1
#define USE_TMOS_STDLIB		0
#define USE_TIME			0
#define USE_TMOS_STDIO		1
#define USE_TMOS_LLONG		0
#define USE_CRC16			1
#define USE_CRC32			0
#define USE_CACHE			0
#define USE_SHA_1			0
#define USE_BASE32			0
#define USE_GUI				0
#define USE_USB				0
#define USE_AES_TMOS		0
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	0

#define USE_UART_DMA_DRIVER 0

#define USE_ADC_DMA_DRIVER 0		//Enable ADC DMA

#define USART_DRV_RX_BUF_SIZE	(48)
#define USE_SPI_DMA_DRIVER 		0		//Disable DMA by default

#endif /* BRD_CFG_H_ */
