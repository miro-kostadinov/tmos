/**************************************************************************//**
 * @file     eefc_drv.cpp
 * @ingroup  DRV_AT91_EEFC
 * @brief    EEFC driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "eefc_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void EEFC_DCR(EEFC_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void EEFC_DSR(EEFC_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void EEFC_ISR(EEFC_INFO drv_info )
{
	ASSERT(0);
}
