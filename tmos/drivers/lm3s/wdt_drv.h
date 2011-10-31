/**************************************************************************//**
 * @ingroup	 drivers_lm3s
 * @defgroup DRV_LM3S_WDT Watchdog Timer Driver
 * Interface for Watchdog Timer (WDT) driver.
 *
 * The watchdog driver does not support handles. Instead it starts a helper task
 * which feeds the dog at feed_sleep interval. The task is running with zero
 * priority and if it does not receive the control within the wdt_load interval
 * an interrupt will be signaled. Then the ISR will either hang up or reset the
 * system.
 * @note The standard watchdog logic is a bit different. The Watchdog Timer can
 * be configured to generate an interrupt to the controller on its first
 * time-out, and to generate a reset signal on its second time-out.
 * If the interrupt is cleared before the 32-bit counter reaches its second
 * time-out, the 32-bit counter is loaded with the value in the WDTLOAD register,
 * and counting resumes from that value.
 * However this logic does not catch deadlocks in user tasks, so the helper task
 * is supposed to feed dog and if it fails to do so, the ISR will will reset
 * immediately.
 *
 *
 *
 * @file     drivers/lm3s/wdt_drv.h
 * @ingroup	 DRV_LM3S_WDT
 * @brief    WDT driver header
 * @version  V3.00
 * @date     25. October 2011
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/
#ifndef WDT_DRV_H_
#define WDT_DRV_H_

#include <tmos.h>
#include <wdt_lm3s.h>

#ifdef __cplusplus
 extern "C" {
#endif

/** WDT Driver Info structure **/
struct WDT_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;			//!< standard driver info
	WDT_Type*			hw_base;		//!< pointer to the hardware peripheral
	unsigned int		wdt_load;		//!< WDT load value
	unsigned int		feed_sleep;		//!< WDT Feed interval in OS quanta (ms)
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
