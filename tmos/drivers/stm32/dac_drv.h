/*
 * dac_drv.h
 *
 *  Created on: Jul 10, 2015
 *      Author: miro
 */

#ifndef DRIVERS_STM32_DAC_DRV_H_
#define DRIVERS_STM32_DAC_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>
#include <mcu_cpp.h>

#ifndef USE_DAC_DMA_DRIVER
#define USE_DAC_DMA_DRIVER 1		//Enable DMA by default
#endif

#ifndef USE_DAC_INTERRUPRT
#define USE_DAC_INTERRUPRT 	1		//Enable ISR by default
#endif

/** DAC Driver mode structure **/
struct DAC_DRIVER_MODE
{
	uint16_t dac_cr;		//!< DAC_CR register value
	uint16_t dac_ch;		//!< DAC channel (0/1)
	uint16_t dac_reg;		//!< DAC registers offset
	uint16_t dac_size;		//!< Transfer size 1=8bit, 2=16-bit
};

/** DAC Driver data structure **/
struct DAC_DRIVER_DATA
{
	DAC_DRIVER_MODE* dac_mode[2];		//!< number of open handles
	HANDLE 			 pending[2];		//!< pending channels
#if USE_DAC_DMA_DRIVER
	CHandle			 dac_dma_hnd[2];
#endif
};

/** DAC Driver Info structure **/
struct DAC_DRIVER_INFO
{
	DRIVER_INFO_Type   info;			//!< Standard driver info
	DAC_TypeDef* 	   hw_base;			//!< DAC Hardware registers
	const PIN_DESC*	   dac_pins; 		//!< dac pins
	DAC_DRIVER_DATA*   drv_data;		//!< driver data
#if USE_DAC_DMA_DRIVER
	DMA_DRIVER_MODE	   dac_dma_mode[2];
#endif
};

void DAC_DCR(DAC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void DAC_DSR(DAC_DRIVER_INFO* drv_info, HANDLE hnd);
#if USE_DAC_INTERRUPRT
void DAC_ISR(DAC_DRIVER_INFO* drv_info);
#endif


#endif /* DRIVERS_STM32_DAC_DRV_H_ */
