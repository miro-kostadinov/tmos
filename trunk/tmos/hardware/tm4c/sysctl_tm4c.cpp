/*
 * sysctl_tm4c.cpp
 *
 *  Created on: Aug 23, 2014
 *      Author: miro
 */

#include <tmos.h>
#include <fam_cpp.h>

#if CFG_SERIES	== tm4c129x

//*****************************************************************************
//
// The mapping of system clock frequency to flash memory timing parameters.
//
//*****************************************************************************
const struct
{
    uint32_t ui32Frequency;
    uint32_t ui32MemTiming;
}
g_sXTALtoMEMTIM[] =
{
	{ 16000000, (SYSCTL_MEMTIM0_FBCHT_0_5 | SYSCTL_MEMTIM0_FWS_0 | SYSCTL_MEMTIM0_FBCE |
				 SYSCTL_MEMTIM0_EBCHT_0_5 | SYSCTL_MEMTIM0_EWS_0 | SYSCTL_MEMTIM0_EBCE |
				 SYSCTL_MEMTIM0_MB1) },
    { 40000000, (SYSCTL_MEMTIM0_FBCHT_1_5 | SYSCTL_MEMTIM0_FWS_1 |
    			 SYSCTL_MEMTIM0_EBCHT_1_5 | SYSCTL_MEMTIM0_EWS_1 |
                 SYSCTL_MEMTIM0_MB1) },
    { 60000000, (SYSCTL_MEMTIM0_FBCHT_2_0 | SYSCTL_MEMTIM0_FWS_2 |
                 SYSCTL_MEMTIM0_EBCHT_2_0 | SYSCTL_MEMTIM0_EWS_2 |
                 SYSCTL_MEMTIM0_MB1) },
    { 80000000, (SYSCTL_MEMTIM0_FBCHT_2_5 | SYSCTL_MEMTIM0_FWS_3 |
                 SYSCTL_MEMTIM0_EBCHT_2_5 | SYSCTL_MEMTIM0_EWS_3 |
                 SYSCTL_MEMTIM0_MB1) },
    { 100000000,(SYSCTL_MEMTIM0_FBCHT_3_0 | SYSCTL_MEMTIM0_FWS_4 |
                 SYSCTL_MEMTIM0_EBCHT_3_0 | SYSCTL_MEMTIM0_EWS_4 |
                 SYSCTL_MEMTIM0_MB1) },
    { 120000000,(SYSCTL_MEMTIM0_FBCHT_3_5 | SYSCTL_MEMTIM0_FWS_5 |
                 SYSCTL_MEMTIM0_EBCHT_3_5 | SYSCTL_MEMTIM0_EWS_5 |
                 SYSCTL_MEMTIM0_MB1) },
};

static uint32_t SysCtlMemTimingGet(uint32_t ui32SysClock)
{
    uint32_t ui8Idx;

    //
    // Loop through the flash memory timings.
    //
    for(ui8Idx = 0;
        ui8Idx < (sizeof(g_sXTALtoMEMTIM) / sizeof(g_sXTALtoMEMTIM[0]));
        ui8Idx++)
    {
        //
        // See if the system clock frequency is less than the maximum frequency
        // for this flash memory timing.
        //
        if(ui32SysClock <= g_sXTALtoMEMTIM[ui8Idx].ui32Frequency)
        {
            //
            // This flash memory timing is the best choice for the system clock
            // frequency, so return it now.
            //
            return(g_sXTALtoMEMTIM[ui8Idx].ui32MemTiming);
        }
    }

    //
    // An appropriate flash memory timing could not be found, so the device is
    // being clocked too fast.  Return the default flash memory timing.
    //
    return(0);
}

void SysCtlClockSet(const TM4C_CLOCK_CFG* cfg)
{
	uint32_t reg;


	do
	{
		//clear failures
		SYSCTL->SYSCTL_MISC = SYSCTL_MISC_MOFMIS;

		if( (cfg->CFG_RSCLKCFG & SYSCTL_RSCLKCFG_OSCSRC) == SYSCTL_RSCLKCFG_OSCSRC_MOSC)
		{
			// Clear MOSC power down, high oscillator range setting, and no crystal
			// present setting.
			reg = SYSCTL->SYSCTL_MOSCCTL & ~(SYSCTL_MOSCCTL_OSCRNG | SYSCTL_MOSCCTL_PWRDN
							| SYSCTL_MOSCCTL_NOXTAL);

			// Increase the drive strength for MOSC of 10 MHz and above.
			if (cfg->CFG_xtal_freq >= 10000000)
			{
				reg |= SYSCTL_MOSCCTL_OSCRNG;
			}

			if( (reg ^ SYSCTL->SYSCTL_MOSCCTL) &
					(SYSCTL_MOSCCTL_OSCRNG| SYSCTL_MOSCCTL_PWRDN | SYSCTL_MOSCCTL_NOXTAL))
			{
				//clear status
				SYSCTL->SYSCTL_MISC = SYSCTL_MISC_MOSCPUPMIS;

				// apply new settings
				SYSCTL->SYSCTL_MOSCCTL = reg;

				//wait
				for(int retry=0; retry < 1000; retry++)
				{
					if(SYSCTL->SYSCTL_RIS & (SYSCTL_RIS_MOFRIS | SYSCTL_RIS_MOSCPUPRIS))
						break;
				}
			}

		}
	} while(SYSCTL->SYSCTL_RIS & SYSCTL_RIS_MOFRIS);

	if(cfg->CFG_RSCLKCFG & SYSCTL_RSCLKCFG_USEPLL)
	{
		// Use PLL

		// Set the memory timings for the maximum external frequency since
		// this could be a switch to PIOSC or possibly to MOSC which can be
		// up to 25MHz.
		SYSCTL->SYSCTL_MEMTIM0 = SysCtlMemTimingGet(25000000);

		// Clear the old PLL divider and source in case it was set.
		reg = SYSCTL->SYSCTL_RSCLKCFG & ~(SYSCTL_RSCLKCFG_PSYSDIV | SYSCTL_RSCLKCFG_OSCSRC
						| SYSCTL_RSCLKCFG_PLLSRC | SYSCTL_RSCLKCFG_USEPLL);

		// Update the memory timings to match running from PIOSC.
		SYSCTL->SYSCTL_RSCLKCFG = reg | SYSCTL_RSCLKCFG_MEMTIMU;

		// If there were no changes to the PLL do not force the PLL to lock by
		// writing the PLL settings.
		if ( SYSCTL->SYSCTL_PLLFREQ1 != cfg->CFG_PLLFREQ1
				|| SYSCTL->SYSCTL_PLLFREQ0	!= cfg->CFG_PLLFREQ0)
		{
			// Set the oscillator source.
			SYSCTL->SYSCTL_RSCLKCFG |= cfg->CFG_RSCLKCFG & (SYSCTL_RSCLKCFG_OSCSRC |
					SYSCTL_RSCLKCFG_PLLSRC | SYSCTL_RSCLKCFG_PSYSDIV | SYSCTL_RSCLKCFG_OSYSDIV);

			//
			// Set the M, N and Q values provided from the table and preserve
			// the power state of the main PLL.
			//
			SYSCTL->SYSCTL_PLLFREQ1 = cfg->CFG_PLLFREQ1;
			reg = SYSCTL->SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_PLLPWR;
			SYSCTL->SYSCTL_PLLFREQ0 = reg | (cfg->CFG_PLLFREQ0 & ~SYSCTL_PLLFREQ0_PLLPWR);

			reg = 1;
		} else
			reg = 0;

		// Set the Flash and EEPROM timing values.
		SYSCTL->SYSCTL_MEMTIM0 = SysCtlMemTimingGet(cfg->CFG_clk_freq);

		// Check if the PLL is already powered up.
		if (!(SYSCTL->SYSCTL_PLLFREQ0 & SYSCTL_PLLFREQ0_PLLPWR))
		{
			// Power up the PLL.
			SYSCTL->SYSCTL_PLLFREQ0 |= SYSCTL_PLLFREQ0_PLLPWR;
		} else
		{
			if(reg)
			{
				// Trigger the PLL to lock to the new frequency.
				SYSCTL->SYSCTL_RSCLKCFG |= SYSCTL_RSCLKCFG_NEWFREQ;
			}
		}

		// Wait until the PLL has locked.
		for (int tout = 32768; tout > 0; tout--)
		{
			if ((SYSCTL->SYSCTL_PLLSTAT & SYSCTL_PLLSTAT_LOCK))
			{
				// Set the new clock configuration.
				SYSCTL->SYSCTL_RSCLKCFG |= SYSCTL_RSCLKCFG_MEMTIMU | SYSCTL_RSCLKCFG_USEPLL;
				break;
			}
		}
	} else
	{
		// Set the Flash and EEPROM timing values for PIOSC.
		SYSCTL->SYSCTL_MEMTIM0 = SysCtlMemTimingGet(16000000);

		// Make sure that the PLL is powered down since it is not being used.
		SYSCTL->SYSCTL_PLLFREQ0 &= ~SYSCTL_PLLFREQ0_PLLPWR;

		//
		// Clear the old PLL divider and source in case it was set.
		//
		reg = SYSCTL->SYSCTL_RSCLKCFG;
		reg &= ~(SYSCTL_RSCLKCFG_OSYSDIV | SYSCTL_RSCLKCFG_OSCSRC | SYSCTL_RSCLKCFG_USEPLL);

		// Update the memory timings.
		reg |= SYSCTL_RSCLKCFG_MEMTIMU;

		// Set the new clock configuration.
		SYSCTL->SYSCTL_RSCLKCFG = reg;

		// Set the memory timing values for the new system clock.
		SYSCTL->SYSCTL_MEMTIM0 = SysCtlMemTimingGet(cfg->CFG_clk_freq);

		// Set the new system clock values.
		SYSCTL->SYSCTL_RSCLKCFG = cfg->CFG_RSCLKCFG | SYSCTL_RSCLKCFG_MEMTIMU;
	}

}
#endif




