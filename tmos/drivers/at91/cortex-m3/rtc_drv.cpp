/**************************************************************************//**
 * @file     rtc_drv.cpp
 * @ingroup  DRV_AT91_RTC
 * @brief    RTC driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "rtc_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void RTC_DCR(RTC_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void RTC_DSR(RTC_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void RTC_ISR(RTC_INFO drv_info )
{
	ASSERT(0);
}
