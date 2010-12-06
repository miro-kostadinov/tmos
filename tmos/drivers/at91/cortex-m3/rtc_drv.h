/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_RTC Real-time Clock Driver
 * Interface for Real-time Clock (RTC) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/rtc_drv.h
 * @ingroup	 DRV_AT91_RTC
 * @brief    RTC driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef RTC_DRV_H_
#define RTC_DRV_H_


#include <tmos.h>
#include <rtc.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct RTC_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Rtc*				hw_base;		//!< pointer to the hardware peripheral
};
/** RTC Driver Info */
typedef const RTC_DRIVER_INFO* RTC_INFO;

void RTC_DCR(RTC_INFO drv_info, unsigned int reason, HANDLE hnd);
void RTC_DSR(RTC_INFO drv_info, HANDLE hnd);
void RTC_ISR(RTC_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* RTC_DRV_H_ */
