/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_WDT Watchdog Timer Driver
 * Interface for Watchdog Timer (WDT) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/wdt_drv.h
 * @ingroup	 DRV_AT91_WDT
 * @brief    WDT driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef WDT_DRV_H_
#define WDT_DRV_H_


#include <tmos.h>
#include <wdt.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct WDT_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Wdt*				hw_base;		//!< pointer to the hardware peripheral
};
/** WDT Driver Info */
typedef const WDT_DRIVER_INFO* WDT_INFO;

void WDT_DCR(WDT_INFO drv_info, unsigned int reason, HANDLE hnd);
void WDT_DSR(WDT_INFO drv_info, HANDLE hnd);
void WDT_ISR(WDT_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* WDT_DRV_H_ */
