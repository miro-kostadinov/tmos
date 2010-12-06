/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_RSTC Reset Controller Driver
 * Interface for Reset Controller (RSTC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/rstc_drv.h
 * @ingroup	 DRV_AT91_RSTC
 * @brief    RSTC driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef RSTC_DRV_H_
#define RSTC_DRV_H_


#include <tmos.h>
#include <rstc.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct RSTC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Rstc*				hw_base;		//!< pointer to the hardware peripheral
};
/** EEFC Driver Info */
typedef const RSTC_DRIVER_INFO* RSTC_INFO;

void RSTC_DCR(RSTC_INFO drv_info, unsigned int reason, HANDLE hnd);
void RSTC_DSR(RSTC_INFO drv_info, HANDLE hnd);
void RSTC_ISR(RSTC_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* RSTC_DRV_H_ */
