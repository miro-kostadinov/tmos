/**
 * @ingroup	 drv_at91_sam3s
 * @defgroup DRV_SAM3S_ADC Analog-to-Digital Converter (ADC) Driver
 * Interface for Analog-to-Digital Converter (ADC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/sam3s/adc_drv.h
 * @ingroup	 DRV_SAM3S_ADC
 * @brief    ADC driver header
 *
 * @date     12. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef ADC_DRV_H_
#define ADC_DRV_H_

#include <adc.h>

struct ADC_DRIVER_DATA_STRU
{
    unsigned char	cnt;		//!< number of open handles
    HANDLE			waiting;
    HANDLE			pending;
};
/** UART DRIVER DATA pointer type */
typedef ADC_DRIVER_DATA_STRU* ADC_DRIVER_DATA;

struct ADC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Adc*				hw_base;		//!< pointer to the hardware peripheral
	ADC_DRIVER_DATA 	drv_data;		//!< pointer to the driver data
	unsigned int 		CFG_ADC_MR;		//!< ADC MR
	//			ADC_MR_TRANSFER(1) | ADC_MR_TRACKTIM(0) | ADC_MR_SETTLING(3);

	//			(SHTIM << 24)| (STARTUP << 16) | (PRESCAL << 8) | (SLEEP << 5) |
	//        				(LOWRES <<4) | (TRGSEL << 1) | (TRGEN );

};

/** ADC Driver Info */
typedef const ADC_DRIVER_INFO* ADC_INFO;

void ADC_DCR(ADC_INFO drv_info, unsigned int reason, HANDLE param);
void ADC_DSR(ADC_INFO drv_info, HANDLE hnd);
void ADC_ISR(ADC_INFO drv_info );

#endif /* ADC_DRV_H_ */
