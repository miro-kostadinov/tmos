/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for AT91
 *
 */


#include <tmos.h>
#include <pmc_drv.h>
#include <platform_cpp.h>
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
WEAK_C void LowLevelInit( void )
{
	sys_SetNewClock(&pmc_driver, pmc_driver.CFG_BOOT_CLK);
}

/**
 * Low level peripheral reset
 * Can be replaced by app
 */
WEAK_C void LowLevelReset( void)
{
	RSTC->RSTC_CR = RSTC_CR_KEY(0xA5) | RSTC_CR_PERRST;
}

/**
 * Reboots the device
 * Can be replaced by app
 */
WEAK_C void LowLevelReboot( void)
{
	RSTC->RSTC_CR = RSTC_CR_KEY(0xA5) | RSTC_CR_PERRST | RSTC_CR_PROCRST;
}

/**
 * Reset cause
 * @return device specific value for the cause
 */
WEAK_C unsigned int LowLevelResetCause(void)
{
	unsigned int res;

	res = (RSTC->RSTC_SR >> 8) & 0x7;
	return res;
}
