/**
 * @ingroup	 drv_at91_cm3
 * @defgroup DRV_AT91_RTT Real-time Timer Driver
 * Interface for Real-time Timer (RTT) driver.
 * @{
 *
 *
 * @file     drivers/at91/cortex-m3/rtt_drv.h
 * @ingroup	 DRV_AT91_RTT
 * @brief    RTT driver header
 *
 * @date     29. November 2010
 * @author	 Miroslav Kostadinov
 * @}
*/
#ifndef RTT_DRV_H_
#define RTT_DRV_H_


#include <tmos.h>
#include <rtt.h>

#ifdef __cplusplus
 extern "C" {
#endif

struct RTT_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	Rtt*				hw_base;		//!< pointer to the hardware peripheral
};
/** RTT Driver Info */
typedef const RTT_DRIVER_INFO* RTT_INFO;

void RTT_DCR(RTT_INFO drv_info, unsigned int reason, HANDLE hnd);
void RTT_DSR(RTT_INFO drv_info, HANDLE hnd);
void RTT_ISR(RTT_INFO drv_info );


#ifdef __cplusplus
}
#endif


#endif /* RTT_DRV_H_ */
