/**************************************************************************//**
 * @file     wdt_drv.cpp
 * @ingroup  DRV_AT91_WDT
 * @brief    WDT driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "wdt_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void WDT_DCR(WDT_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void WDT_DSR(WDT_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void WDT_ISR(WDT_INFO drv_info )
{
	ASSERT(0);
}
