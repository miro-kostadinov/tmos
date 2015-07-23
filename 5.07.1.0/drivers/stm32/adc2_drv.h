/*
 * adc2_drv.h
 *
 * ADC driver for F2(F4?) family
 *  Created on: Feb 25, 2013
 *      Author: miro
 */

#ifndef ADC2_DRV_H_
#define ADC2_DRV_H_

#include <port_cpp.h>
#include <hardware_cpp.h>

#ifndef USE_ADC_DMA_DRIVER
#define USE_ADC_DMA_DRIVER 0		//Disable DMA by default
#endif


/** ADC Driver mode structure **/
struct ADC_CFG_MODE
{
	uint32_t adc_ccr;				//!< ADC_CCR register value (ADCPRE and MULTI are important)

	uint32_t adc_cr1;				//!< ADC_CR1 register value
	uint32_t adc_cr2;				//!< ADC_CR2 register value (without ADON)

	uint32_t adc_smpr1;				//!< ADC_SMPR1 register value
	uint32_t adc_smpr2;				//!< ADC_SMPR2 register value
};

/** ADC Driver mode structure **/
struct ADC_DRIVER_MODE
{
	uint32_t adc_sqr1;				//!< ADC_SQR1 register value
	uint32_t adc_sqr2;				//!< ADC_SQR2 register value
	uint32_t adc_sqr3;				//!< ADC_SQR3 register value
	const PIN_DESC* adc_pins;		//!< List of adc pin configurations
	uint8_t	 adc_num;				//!< ADC 1, 2 or 3
	uint8_t	 adc_cfg_indx;			//!< driver configuration

};

/** ADC Driver data structure **/
struct ADC_DRIVER_DATA
{
	union
	{
		struct
		{
			uint8_t	adc_cnt[3];		//!< number of clients for ADC x
		};
		uint32_t	all_cnt;		//!< number of clients for all ADCs
	};
	HANDLE	pending[3];				//!< pinding clients for each ADC
	HANDLE	waiting[3];				//!< waiting clients for each ADC
#if USE_ADC_DMA_DRIVER
	CHandle	dma_hnd[USE_ADC_DMA_DRIVER];
#endif
};


/** ADC Driver Info structure **/
struct ADC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	ADCS_TypeDef*		hw_base;		//!< ADC peripheral
	ADC_DRIVER_DATA* 	drv_data;		//!< driver data
	uint8_t				adc_cfg_cnt;
	const ADC_CFG_MODE*	adc_cfg;
#if USE_ADC_DMA_DRIVER
	DMA_DRIVER_MODE		dma_mode[USE_ADC_DMA_DRIVER];
#endif
};

void ADC_DCR(const ADC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd);
void ADC_DSR(const ADC_DRIVER_INFO* drv_info, HANDLE hnd);
void ADC_ISR(const ADC_DRIVER_INFO* drv_info);



#endif /* ADC2_DRV_H_ */
