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
#define USE_TMOS_TL_V2		0
#define USE_CSTRING			0
#define USE_TTL_STRING 		0	//use old CSTRING


#define USE_TMOS_STRING		1
#define USE_TMOS_STDLIB		1
#define USE_TIME			0
#define USE_TMOS_STDIO		1
#define USE_CRC32			1
#define USE_CACHE			0
#define USE_SHA_1			0
#define USE_BASE32			0
#define USE_GUI				0
#define USE_GUI2			0
#define USE_USB				1
#define USE_AES_TMOS		1
#define	USE_GPIO_EXPANDER 	0

#define USE_EXCEPTION_RECORD 1
#define USE_MEMORY_TRACKING	1



#define USE_ADC_DMA_DRIVER 0		//Enable ADC DMA


#endif /* BRD_CFG_H_ */
