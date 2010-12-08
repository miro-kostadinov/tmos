/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for LM3S
 *
 */


#include <tmos.h>
#include <sysctl_lm3s.h>
#include <memmap_inc.h>

/**
 *  Low Level init for LM3S
 */
extern "C" __attribute__ ((weak)) void LowLevelInit( void )
{
	SYSCTL->SysCtlClockSet(SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_OSC
			| SYSCTL_SYSDIV_2);
	system_clock_frequency = SYSCTL->SysCtlClockGet();
}
