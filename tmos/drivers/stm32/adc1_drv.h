/*
 * adc1_drv.h
 *
 *  Created on: Sep 11, 2015
 *      Author: miro
 */

#ifndef DRIVERS_STM32_ADC1_DRV_H_
#define DRIVERS_STM32_ADC1_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>

#ifndef USE_ADC_DMA_DRIVER
#define USE_ADC_DMA_DRIVER 0		//Disable DMA by default
#endif

/** ADC Driver mode structure **/
struct ADC_DRIVER_MODE
{
	uint32_t adc_cfgr1;        //!< ADC Configuration register 1 value
	uint32_t adc_smpr;         //!< ADC Sampling time register value
	uint32_t adc_tr;           //!< ADC watchdog threshold register value
	uint32_t adc_chselr;       //!< ADC channel selection register value
	uint32_t adc_ccr;          //!< ADC common configuration register value
	const PIN_DESC* adc_pins;  //!< List of adc pin configurations

};

/** ADC Driver data structure **/
struct ADC_DRIVER_DATA
{
	HANDLE	pending;					//!< pending clients
	HANDLE	waiting;					//!< waiting clients
	const ADC_DRIVER_MODE* adc_mode;	//!< current ADC mode
#if USE_ADC_DMA_DRIVER
	CHandle	dma_hnd;
#endif
	uint16_t cnt;						//!< number of clients
	uint16_t sample_size;				//!< 0=8 or 1=16 bit sampling
};

/** ADC Driver Info structure **/
struct ADC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	ADC_TypeDef*		hw_base;		//!< ADC peripheral
	ADC_DRIVER_DATA* 	drv_data;		//!< driver data
	uint32_t			adc_clk;		//!< ADC clock configuration
#if USE_ADC_DMA_DRIVER
	DMA_DRIVER_MODE		dma_mode;
#endif
};

void ADC_DCR(const ADC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void ADC_DSR(const ADC_DRIVER_INFO* drv_info, HANDLE hnd);
void ADC_ISR(const ADC_DRIVER_INFO* drv_info);


#endif /* DRIVERS_STM32_ADC1_DRV_H_ */
