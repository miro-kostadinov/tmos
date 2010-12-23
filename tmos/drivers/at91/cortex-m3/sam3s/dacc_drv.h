/**
 * @ingroup	 drv_at91_sam3s
 * @defgroup DRV_SAM3S_DACC Digital-to-Analog Converter Controller (DACC) Driver
 * Interface for Digital-to-Analog Converter Controller (DACC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/sam3s/dacc_drv.h
 * @ingroup	 DRV_SAM3S_DACC
 * @brief    DACC driver header
 *
 * @date     22. December 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef DAC_DRV_H_
#define DAC_DRV_H_

#include <dacc.h>

struct DRV_DACC_MODE_STRU
{
    unsigned int	DACC_MR;	//!< DACC_MR register value
    unsigned int 	DACC_CHER;	//!< dacc channel
};
/** DACC Mode */
typedef const DRV_DACC_MODE_STRU * DRV_DACC_MODE;

struct DACC_DRIVER_DATA_STRU
{
    unsigned char	cnt;		//!< number of open handles
    HANDLE			pending;
};
/** DACC DRIVER DATA pointer type */
typedef DACC_DRIVER_DATA_STRU* DACC_DRIVER_DATA;

struct DACC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Dacc*				hw_base;		//!< pointer to the hardware peripheral
	DACC_DRIVER_DATA 	drv_data;		//!< pointer to the driver data
};

/** DACC Driver Info */
typedef const DACC_DRIVER_INFO* DACC_INFO;

void DACC_DCR(DACC_INFO drv_info, unsigned int reason, HANDLE param);
void DACC_DSR(DACC_INFO drv_info, HANDLE hnd);
void DACC_ISR(DACC_INFO drv_info );

#endif /* DAC_DRV_H_ */
