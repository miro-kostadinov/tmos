/**************************************************************************//**
 * @file     rtt_drv.cpp
 * @ingroup  DRV_AT91_RTT
 * @brief    RTT driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "rtt_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void RTT_DCR(RTT_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void RTT_DSR(RTT_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void RTT_ISR(RTT_INFO drv_info )
{
	ASSERT(0);
}
