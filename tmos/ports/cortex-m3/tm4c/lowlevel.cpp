/**************************************************************************//**
 * @ingroup	 ports_cortex
 * @file
 * @brief    LowLevel init for TM4C
 *
 */


#include <tmos.h>
#include <fam_cpp.h>

WEAK struct TM4C_CLOCK_CFG g_board_clk_cfg =
{
	120000000,													// 120 MHz clock
	25000000,													// 25 MHz xtal
	SYSCTL_RSCLKCFG_PLLSRC_MOSC | SYSCTL_RSCLKCFG_OSCSRC_MOSC |
	SYSCTL_RSCLKCFG_PSYSDIV_Set(3), 							// SYSCTL_RSCLKCFG;
	SYSCTL_PLLFREQ0_MINT_Set(96), 								// SYSCTL_PLLFREQ0;
	SYSCTL_PLLFREQ1_N_Set(5) 									// SYSCTL_PLLFREQ1;
};

/**
 *  Low Level init for LM3S
 */
WEAK_C void LowLevelInit( void )
{
	SysCtlClockSet(&g_board_clk_cfg);
	system_clock_frequency = g_board_clk_cfg.CFG_clk_freq;
}

/**
 * Low level peripheral reset
 * Can be replaced by app
 */
WEAK_C void LowLevelReset( void)
{
	__IO uint32_t* reg;

	for(reg = &SYSCTL->SYSCTL_SRWD; reg <= &SYSCTL->SYSCTL_SREMAC; reg++)
	{
		*reg = -1;	// reset peripherals
		*reg = 0;	// reset peripherals
	}
}

/**
 * Reboots the device
 * Can be replaced by app
 */
WEAK_C void LowLevelReboot( void)
{
    SCB->SCB_AIRCR = SCB_AIRCR_VECTKEY_VAL| SCB_AIRCR_SYSRESETREQ;
    while(1);
}

/**
 * Reset cause
 * @return device specific value for the cause
 */
WEAK_C unsigned int LowLevelResetCause(void)
{
	unsigned int res;

	res = SYSCTL->SYSCTL_RESC;
	SYSCTL->SYSCTL_RESC = ~res;
	return (res);
}
