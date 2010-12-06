/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_EEFC Enhanced Embedded Flash Controller Driver
 * Interface for Enhanced Embedded Flash Controller (EEFC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/eefc_drv.h
 * @ingroup	 DRV_AT91_EEFC
 * @brief    EEFC driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef EEFC_DRV_H_
#define EEFC_DRV_H_


#include <tmos.h>
#include <eefc.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct EEFC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Efc*				hw_base;		//!< pointer to the hardware peripheral
};
/** EEFC Driver Info */
typedef const EEFC_DRIVER_INFO* EEFC_INFO;

void EEFC_DCR(EEFC_INFO drv_info, unsigned int reason, HANDLE hnd);
void EEFC_DSR(EEFC_INFO drv_info, HANDLE hnd);
void EEFC_ISR(EEFC_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* EEFC_DRV_H_ */
