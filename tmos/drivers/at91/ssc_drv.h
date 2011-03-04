/**
 * @ingroup	 drv_at91
 * @defgroup DRV_AT91_SSC Syncrhonous Serial Controller  Driver
 * Interface for Syncrhonous Serial Controller (SSC) driver.
 * @{
 * SPI mode
 *
 * HANDLE update:
 *
 * @file     drivers/at91/ssc_drv.h
 * @ingroup	 DRV_AT91_SSC
 * @brief    SSC driver header
 *
 * @date     06. February 2011
 * @author	 Miroslav Kostadinov
 * @}
*/

#ifndef SSC_DRV_H_
#define SSC_DRV_H_

#include <tmos.h>
#include <ssc.h>
#include <pio_drv.h>

struct SSC_MODE_STRU
{
	GPIO_STRU 		cs_pin;
	unsigned int 	baudrate;
};

struct SSC_DRIVER_DATA_STRU
{
    HANDLE			pending;	//!< Receive queue
    HANDLE			waiting;	//!< Receive queue
    unsigned char	cnt;		//!< number of open handles
};
/** SSC DRIVER DATA pointer type */
typedef SSC_DRIVER_DATA_STRU* SSC_DRIVER_DATA;

struct SSC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Ssc *				hw_base;		//!< pointer to the hardware peripheral
	SSC_DRIVER_DATA 	drv_data;		//!< pointer to the driver data
	GPIO_STRU 			ssc_pins;
};
/** SSC Driver Info */
typedef const SSC_DRIVER_INFO* SSC_INFO;

void SSC_DCR(SSC_INFO drv_info, unsigned int reason, HANDLE param);
void SSC_DSR(SSC_INFO drv_info, HANDLE hnd);
void SSC_ISR(SSC_INFO drv_info );

#endif /* SSC_DRV_H_ */
