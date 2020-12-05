/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_rcc
 * @{
 * @file     hardware/stm32/stm32f2/rcc_f2.cpp
 * @brief    STM32F2 RCC
 * @version  V3.00
 * @date     12. November 2012
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>


void drv_peripheral_reset(uint32_t periph_id)
{
	__IO uint32_t* reg;
	uint32_t bitmask;

    if(periph_id < ID_NO_PERIPH)
    {
    	bitmask = 1<<(periph_id&0x1f);
    	periph_id >>=5;

		/* RCCPeripheralReset */
		reg = &RCC->RCC_AHB3RSTR;

		// Put the peripheral into the reset state.
		reg[periph_id] |= bitmask;

		// Delay for a little bit.
		for(volatile int ulDelay = 0; ulDelay < 16; ulDelay++)
		{
		}

		// Take the peripheral out of the reset state.
		bitmask = ~bitmask;
		reg[periph_id] &= bitmask;

		// Disable this peripheral clock.
		reg[periph_id + 8] &= bitmask;
   }
}

void RCCPeripheralReset(unsigned int periph_id)
{
	__IO uint32_t* reg;

    if(periph_id < ID_NO_PERIPH)
    {
    	reg = &RCC->RCC_AHB3RSTR;
    	reg += periph_id>>5;

		periph_id = 1<<(periph_id&0x1f);

		// Put the peripheral into the reset state.
		*reg |= periph_id;

		// Delay for a little bit.
		for(volatile int ulDelay = 0; ulDelay < 16; ulDelay++)
		{
		}

		// Take the peripheral out of the reset state.
		*reg &= ~periph_id;
    }
}

void RCCPeripheralEnable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

		reg = &RCC->RCC_AHB3ENR;

		// Enable this peripheral clock.
		reg[index] |= 1<<(periph_id&0x1f);
    }
}

void RCCPeripheralDisable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

		reg = &RCC->RCC_AHB3ENR;

		// Disable this peripheral clock.
		reg[index] &= ~(1<<(periph_id&0x1f));
    }
}

void RCCPeripheralLPEnable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

		reg = &RCC->RCC_AHB3LPENR;

		// Enable this peripheral clock.
		reg[index] |= 1<<(periph_id&0x1f);
    }
}

void RCCPeripheralLPDisable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

		reg = &RCC->RCC_AHB3LPENR;

		// Disable this peripheral clock.
		reg[index] &= ~(1<<(periph_id&0x1f));
    }
}

uint32_t rcc_get_hsi_clk()
{
	return HSI_VALUE >> RCC_CR_HSIDIV_Get(RCC->RCC_CR);
}
/**
  * @brief  Helper function to calculate the PLL frequency before P/Q/R output
  * @param  in_freq PLL Input frequency (based on HSE/(HSI/HSIDIV)/CSI)
  * @param  M      Between 1 and 63
  * @param  N      Between 4 and 512
  * @param  FRACN  Between 0 and 0x1FFF
  * @retval PLL clock frequency (in Hz)  before P/Q/R output
  */
uint32_t RCC_CalcPLLClockFreq(uint32_t in_freq, uint32_t M, uint32_t N, uint32_t FRACN)
{
	uint64_t freq;

	// 	freq = (in_freq / M) * (N + (FRACN / 0x2000));
	freq = in_freq;
	freq *= N * 0x2000 + FRACN;
	freq /= M;
	freq /= 0x2000;
	return freq;
}

uint32_t RCC_GetPLLxClockFreq(uint32_t pll)
{
	uint32_t freq = 0;
	uint32_t m, n, fracn = 0U;

	/* PLL_VCO = (HSE_VALUE, CSI_VALUE or HSI_VALUE/HSIDIV) / PLLM * (PLLN + FRACN)
	 SYSCLK = PLL_VCO / PLLP
	 */

	m = RCC_PLLCLSELR_DIVMx_Get(RCC->RCC_PLLCKSELR, pll);
	if (m != 0U)
	{
		switch (RCC->RCC_PLLCKSELR & RCC_PLLCKSELR_PLLSRC)
		{
		case RCC_PLLCKSELR_PLLSRC_HSI:
			if (RCC->RCC_CR & RCC_CR_HSIRDY)
			{
				freq = rcc_get_hsi_clk();
			}
			break;

		case RCC_PLLCKSELR_PLLSRC_CSI:
			if (RCC->RCC_CR & RCC_CR_CSIRDY)
			{
				freq = CSI_VALUE;
			}
			break;

		case RCC_PLLCKSELR_PLLSRC_HSE:
			if (RCC->RCC_CR & RCC_CR_HSERDY)
			{
				freq = HSE_VALUE;
			}
			break;

		default:
			/* PLL clock disabled */
			break;
		}

		if(freq)
		{
			n = RCC_PLLxDIVR_DIVNx_Get(RCC->RCC_PLLx[pll].RCC_PLLxDIVR);
			if (RCC_PLLCFGR_PLLxFRACEN_Get(RCC->RCC_PLLCFGR, pll))
			{
				fracn = RCC_PLLxFRACR_FRACNx_Get(RCC->RCC_PLLx[pll].RCC_PLLxFRACR);
			}

			freq = RCC_CalcPLLClockFreq(freq, m, n, fracn);
		}
	}
	return freq;
}

uint32_t RCC_GetPLLxClockFreq_P(rcc_pll_indx_t pll)
{
	  uint32_t freq;

	  freq = RCC_GetPLLxClockFreq(pll);
	  if(freq && RCC_PLLCFGR_DIVPxEN_Get(RCC->RCC_PLLCFGR, pll))
		  freq /= RCC_PLLxDIVR_DIVPx_Get(RCC->RCC_PLLx[pll].RCC_PLLxDIVR);

	  return freq;
}

uint32_t RCC_GetPLLxClockFreq_Q(rcc_pll_indx_t pll)
{
	  uint32_t freq;

	  freq = RCC_GetPLLxClockFreq(pll);
	  if(freq && RCC_PLLCFGR_DIVQxEN_Get(RCC->RCC_PLLCFGR, pll))
		  freq /= RCC_PLLxDIVR_DIVQx_Get(RCC->RCC_PLLx[pll].RCC_PLLxDIVR);

	  return freq;
}

uint32_t RCC_GetPLLxClockFreq_R(rcc_pll_indx_t pll)
{
	  uint32_t freq;

	  freq = RCC_GetPLLxClockFreq(pll);
	  if(freq && RCC_PLLCFGR_DIVRxEN_Get(RCC->RCC_PLLCFGR, pll))
		  freq /= RCC_PLLxDIVR_DIVRx_Get(RCC->RCC_PLLx[pll].RCC_PLLxDIVR);

	  return freq;
}

uint32_t RCC_GetSystemClockFreq()
{
  uint32_t frequency;

  /* Get SYSCLK source -------------------------------------------------------*/
	switch (RCC->RCC_CFGR & RCC_CFGR_SWS)
	{
	/* No check on Ready: Won't be selected by hardware if not */
	case RCC_CFGR_SWS_HSI:
		frequency = rcc_get_hsi_clk();
		break;

	case RCC_CFGR_SWS_CSI:
		frequency = CSI_VALUE;
		break;

	case RCC_CFGR_SWS_HSE:
		frequency = HSE_VALUE;
		break;

	case RCC_CFGR_SWS_PLL1:
		frequency = RCC_GetPLLxClockFreq_P(RCC_PLL1_INDX);
		break;

	default:
		/* Nothing to do */
		frequency = 0;
		break;
	}

  return frequency;
}


uint32_t RCCEx_GetD1SysClockFreq(void)
{
	uint32_t freq;
	uint32_t prescaller;

	freq = RCC_GetSystemClockFreq();
	prescaller = RCC_D1CFGR_D1CPRE_Get(RCC->RCC_D1CFGR);
	if (prescaller > 7)
	{
		if (prescaller > 11)
			prescaller -= 6;
		else
			prescaller -= 7;
		freq >>= prescaller;
	}
	return freq;
}

uint32_t RCC_GetHCLKFreq()
{
	uint32_t freq;
	uint32_t prescaller;

	freq = RCCEx_GetD1SysClockFreq();
	prescaller = RCC_D1CFGR_HPRE_Get(RCC->RCC_D1CFGR);
	if(prescaller > 7)
	{
		if(prescaller > 11)
			prescaller -= 6;
		else
			prescaller -= 7;
		freq >>= prescaller;
	}
  return freq;
}

/**
 *  GET PCLK1-4 (APB1-4) clock frequency (in Hz)
 *
 * @param pclk
 * @return PCLK(pclk) frequency
 */
uint32_t RCC_GetPCLKxFreq(rcc_pclk_indx_t pclk)
{
	uint32_t prescaller;
	uint32_t freq;

	switch (pclk)
	{
	case RCC_PCLK1_INDX:
		prescaller = RCC_D2CFGR_D2PPRE1_Get(RCC->RCC_D2CFGR);
		break;

	case RCC_PCLK2_INDX:
		prescaller = RCC_D2CFGR_D2PPRE2_Get(RCC->RCC_D2CFGR);
		break;

	case RCC_PCLK3_INDX:
		prescaller = RCC_D1CFGR_D1PPRE_Get(RCC->RCC_D1CFGR);
		break;

	case RCC_PCLK4_INDX:
		prescaller = RCC_D3CFGR_D3PPRE_Get(RCC->RCC_D3CFGR);
		break;

	default:
		return 0;
	}

	freq = RCC_GetHCLKFreq();
	if (prescaller > 3)
		freq >>= (prescaller - 3);

	return freq;
}


