/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for AT91
 *
 */


#include <tmos.h>
#include <pmc_drv.h>

/**
 * PMC driver info. The application must declare before the linkage.
 */
extern "C" PMC_DRIVER_INFO pmc_driver;

/**
 * LowLevelInit is the first function called after reset
 *
 * This is the default implementation for AT91/Cortex-M3. It changes the system
 * clock so the boot can pass faster. The application may provide custom
 * implementation for LowLevelInit().
 */
extern "C" __attribute__ ((weak)) void LowLevelInit( void )
{
	sys_SetNewClock(&pmc_driver, pmc_driver.CFG_BOOT_CLK);
}


