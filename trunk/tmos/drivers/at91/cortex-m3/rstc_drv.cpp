/**************************************************************************//**
 * @file     rstc_drv.cpp
 * @ingroup  DRV_AT91_RSTC
 * @brief    RSTC driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "rstc_drv.h"

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void RSTC_DCR(RSTC_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void RSTC_DSR(RSTC_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void RSTC_ISR(RSTC_INFO drv_info )
{
	ASSERT(0);
}
