/**************************************************************************//**
 * @file     supc_drv.cpp
 * @ingroup  DRV_AT91_SUPC
 * @brief    SUPC driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "supc_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void SUPC_DCR(SUPC_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void SUPC_DSR(SUPC_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void SUPC_ISR(SUPC_INFO drv_info )
{
	ASSERT(0);
}
