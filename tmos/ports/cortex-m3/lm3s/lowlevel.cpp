/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for LM3S
 *
 */


#include <tmos.h>
#include <sysctl_lm3s.h>
#include <memmap_inc.h>
#include <cmsis_cpp.h>

/**
 *  Low Level init for LM3S
 */
WEAK_C void LowLevelInit( void )
{
	SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC
			| SYSCTL_SYSDIV_2);
	system_clock_frequency = SYSCTL->SysCtlClockGet();
}

/**
 * Low level peripheral reset
 * Can be replaced by app
 */
WEAK_C void LowLevelReset( void)
{
	SYSCTL->SRCR1 = -1;	// reset peripherals
	SYSCTL->SRCR1 = 0;	// reset peripherals
}

/**
 * Reboots the device
 * Can be replaced by app
 */
WEAK_C void LowLevelReboot( void)
{
	SCB->SysCtlReset();
}

/**
 * Reset cause
 * @return device specific value for the cause
 */
WEAK_C unsigned int LowLevelResetCause(void)
{
	unsigned int res;

	res = SYSCTL->SysCtlResetCauseGet();
	SYSCTL->SysCtlResetCauseClear(res);
	return res;
}
