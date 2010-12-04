/**************************************************************************//**
 * @file     pmc_drv.cpp
 * @ingroup  DRV_AT91_PMC
 * @brief    PMC driver implementation
 * @version  V3.00
 * @date     28. November 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include "pmc_drv.h"
#include <mcu_cpp.h>
#include <eefc.h>

volatile __no_init unsigned int sysdrv_clock_frequency;

void sys_SetMainClock(Pmc* pPMC, unsigned int val)
{
	unsigned int old, temp;

	//enable XT or SC oscillator
	old = pPMC->CKGR_MOR;
	if((old ^ val)& CKGR_MOR_MOSCSEL)
	{
		if(old & CKGR_MOR_MOSCSEL)
		{
			//switch XT -> RC, so first enable RC
			temp = val & (CKGR_MOR_MOSCRCF_Msk | CKGR_MOR_MOSCRCEN | CKGR_MOR_KEY_Msk);
			temp |= (old & ~CKGR_MOR_MOSCRCF_Msk) ;
			old = PMC_SR_MOSCRCS;
		} else
		{
			//switch RC -> XT, so enable XT
			temp = val & ~CKGR_MOR_MOSCSEL;
			temp |= old & (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCRCF_Msk);
			old = PMC_SR_MOSCXTS;
		}
	} else
	{
		if(old & CKGR_MOR_MOSCSEL)
		{
			old = PMC_SR_MOSCXTS;

		} else
		{
			old = PMC_SR_MOSCRCS;
		}
		temp = val;
	}
	pPMC->CKGR_MOR = temp;

	//check the osc is already stable
	while(!(pPMC->PMC_SR & old));

	pPMC->CKGR_MOR = val;
}

/**
 * Set new system clock
 *
 * @param new_clk - clock, encodded as follow:
 *  - bit [1:0] - clock source (MCKR_CSS) for SAM7, SAM3U
 *  - bit [2:0] - clock source (MCKR_CSS) for SAM3S
 *
 * Details:
 *
 * Switching from one clock source and prescaler to another cannot be done in a
 * single step. And when we change only the source or only the prescaler we must
 * ensure that the intermediate combination is allowed.\n
 * Also this function is run from the flash, so we should avoid forbidden
 * frequencies. For example SAM7S64 series have a bug and they can't work in the
 * 3-19MHz range. \n
 *
 * Algorithm:
 * - STEP 1. If we have to change the clock source, we switch first to a known
 * good slow clock.	The slow clock can be embedded oscillator (RC, fast RC or
 * main) and the selection is defined from the application in the PMC_INFO.
 *
 */
void sys_SetNewClock(PMC_INFO drv_info, unsigned int new_clk)
{
	unsigned int	temp;
	Pmc* pPMC = drv_info->hw_base;

// STEP 1: If new source != old source => switch to slow clock

	temp = pPMC->PMC_MCKR;
	if((new_clk ^ temp) & PMC_MCKR_CSS_Msk)
	{
		temp &= ~ PMC_MCKR_CSS_Msk;
		if(new_clk & PMC_MCKR_CSS_Msk)
		{
			//prepare main clock
			sys_SetMainClock(pPMC, drv_info->CFG_CKGR_MOR);

			//switch to main clock
			temp |= PMC_MCKR_CSS_MAIN_CLK;
		} else
		{
			//switch to slow clock
			//JTAG can get lost if current prescaler is too high

		}
		pPMC->PMC_MCKR = temp;


		//wait for  clock switch
		while(!(pPMC->PMC_SR & PMC_SR_MCKRDY));

		// we are now using main/slow source, so we can set the prescaler
		pPMC->PMC_MCKR = new_clk & PMC_MCKR_PRES_Msk;
		//JTAG can get lost now...

		//prescaller change may not be completed
	} else
	{
		// the source is OK, only the prescaller is about to change..
		// If the new clock is lower, we will change the divisor here
		// before we calculate and change the flash wait states.
		if(new_clk > temp)
		{
			pPMC->PMC_MCKR = new_clk;
		}
	}

	//wait until main clock is stable....
	while(!(pPMC->PMC_SR & PMC_SR_MCKRDY));

// STEP 2: We are now running in slow clock or we have reduced the clock
// 		   From now on, we will only increase the clock, but before that
//		   we must prepare the hardware (wait states and osc/pll)

	//calculate the flash wait states <20MHz=0 >30MHz=2
	if((new_clk & PMC_MCKR_CSS_Msk) > PMC_MCKR_CSS_MAIN_CLK)
		temp = EEFC_FMR_FWS(2);
	else
		temp = EEFC_FMR_FWS(0);

	EFC->EEFC_FMR = (EFC->EEFC_FMR & ~EEFC_FMR_FWS_Msk) | temp;



	//check the source
	if((new_clk ^ pPMC->PMC_MCKR ) & PMC_MCKR_CSS_Msk)
	{
		// start pll if required
		if((new_clk & PMC_MCKR_CSS_Msk) == PMC_MCKR_CSS_PLLA_CLK)
		{
			// Start PLL
			pPMC->CKGR_PLLAR = drv_info->CFG_CKGR_PLLAR;
			// Wait PLL
			while(!(pPMC->PMC_SR & PMC_SR_LOCKA));
		}

	}

//STEP 3: set the desired clock and divider if it is not done by now.
	// here the new source is ok...
	// and we know that either the prescaler, either the source is OK (or both)
	pPMC->PMC_MCKR = new_clk;
	//wait until main clock is stable....
	while(!(pPMC->PMC_SR & PMC_SR_MCKRDY));


    //some cleanup...
    temp = sizeof(pPMC->PMC_PCK)/4;
    while(temp--)
    {
    	new_clk |= pPMC->PMC_PCK[temp];
    }

    new_clk &= PMC_MCKR_CSS_Msk;
    //turn off PLL A
    if(new_clk != PMC_MCKR_CSS_PLLA_CLK)
		pPMC->CKGR_PLLAR = drv_info->CFG_CKGR_PLLAR & ~CKGR_PLLAR_MULA_Msk;

    if(new_clk == PMC_MCKR_CSS_SLOW_CLK)
			pPMC->CKGR_MOR = 0;			//turn off oscillator

    new_clk=pPMC->PMC_MCKR;
	temp = drv_info->CFG_FREQUENCIES[new_clk & PMC_MCKR_CSS_Msk];
	temp >>= (new_clk & PMC_MCKR_PRES_Msk) >> 2;
    sysdrv_clock_frequency = temp;

}

extern "C" PMC_DRIVER_INFO pmc_driver;

extern "C" __attribute__ ((weak)) void LowLevelInit( void )
{
	sys_SetNewClock(&pmc_driver, pmc_driver.CFG_BOOT_CLK);
}

/**
 * DCR
 * @param drv_info
 * @param reason
 * @param hnd
 */
void PMC_DCR(PMC_INFO drv_info, unsigned int reason, HANDLE hnd)
{

}

/**
 * DSR
 * @param drv_info
 * @param hnd
 */
void PMC_DSR(PMC_INFO drv_info, HANDLE hnd)
{
	ASSERT(0);
}

/**
 * ISR
 * @param drv_info
 */
void PMC_ISR(PMC_INFO drv_info )
{
	ASSERT(0);
}
