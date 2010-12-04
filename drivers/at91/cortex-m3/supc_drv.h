/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_SUPC Supply Controller Driver
 * Interface for Supply Controller (SUPC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/supc_drv.h
 * @ingroup	 DRV_AT91_SUPC
 * @brief    SUPC header
 *
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/

#ifndef SUPC_DRV_H_
#define SUPC_DRV_H_

#include <tmos.h>
#include <supc.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct SUPC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Supc *				hw_base;		//!< pointer to the hardware peripheral
};
/** SUPC Driver Info */
typedef const SUPC_DRIVER_INFO* SUPC_INFO;

void SUPC_DCR(SUPC_INFO drv_info, unsigned int reason, HANDLE hnd);
void SUPC_DSR(SUPC_INFO drv_info, HANDLE hnd);
void SUPC_ISR(SUPC_INFO drv_info );


#ifdef __cplusplus
}
#endif

#endif /* SUPC_DRV_H_ */
