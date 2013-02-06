/*
 * rcc_f2.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: miro
 */


#include <tmos.h>
#include <fam_cpp.h>
#include <cmsis_cpp.h>



void RCCPeripheralReset(unsigned int periph_id)
{
	__IO uint32_t* reg;

    if(periph_id < ID_NO_PERIPH)
    {
    	reg = &RCC->RCC_AHB1RSTR;
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

		reg = &RCC->RCC_AHB1ENR;

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

		reg = &RCC->RCC_AHB1ENR;

		// Disable this peripheral clock.
		reg[index] &= ~(1<<(periph_id&0x1f));
    }
}

const uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @brief  Returns the frequencies of different on chip clocks; SYSCLK, HCLK,
  *         PCLK1 and PCLK2.
  *
  * @note   The system frequency computed by this function is not the real
  *         frequency in the chip. It is calculated based on the predefined
  *         constant and the selected clock source:
  * @note     If SYSCLK source is HSI, function returns values based on HSI_VALUE(*)
  * @note     If SYSCLK source is HSE, function returns values based on HSE_VALUE(**)
  * @note     If SYSCLK source is PLL, function returns values based on HSE_VALUE(**)
  *           or HSI_VALUE(*) multiplied/divided by the PLL factors.
  * @note     (*) HSI_VALUE is a constant defined in stm32f2xx.h file (default value
  *               16 MHz) but the real value may vary depending on the variations
  *               in voltage and temperature.
  * @note     (**) HSE_VALUE is a constant defined in stm32f2xx.h file (default value
  *                25 MHz), user has to ensure that HSE_VALUE is same as the real
  *                frequency of the crystal used. Otherwise, this function may
  *                have wrong result.
  *
  * @note   The result of this function could be not correct when using fractional
  *         value for HSE crystal.
  *
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *          the clocks frequencies.
  *
  * @note   This function can be used by the user application to compute the
  *         baudrate for the communication peripherals or configure other parameters.
  * @note   Each time SYSCLK, HCLK, PCLK1 and/or PCLK2 clock changes, this function
  *         must be called to update the structure's field. Otherwise, any
  *         configuration based on this function will be incorrect.
  *
  * @retval None
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
	uint32_t mull, presc;

	/* Get SYSCLK source -------------------------------------------------------*/
	switch (RCC->RCC_CFGR & RCC_CFGR_SWS)
	{
	case RCC_CFGR_SWS_HSI: /* HSI used as system clock source */
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;

	case RCC_CFGR_SWS_HSE: /* HSE used as system clock  source */
		RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
		break;

	case RCC_CFGR_SWS_PLL: /* PLL used as system clock  source */

		/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
		 SYSCLK = PLL_VCO / PLLP
		 */
		presc = RCC->RCC_PLLCFGR;

		if (presc & RCC_PLLCFGR_PLLSRC) //pllsource
		{
			/* HSE used as PLL clock source */
			mull = HSE_VALUE;
		} else
		{
			/* HSI used as PLL clock source */
			mull = HSI_VALUE;
		}
		mull /= presc & RCC_PLLCFGR_PLLM;
		mull *= RCC_PLLCFGR_PLLN_Get(presc);

		presc = (RCC_PLLCFGR_PLLP_Get(presc)+1) * 2;
		RCC_Clocks->SYSCLK_Frequency = mull / presc;
		break;

	default:
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	}


	mull = RCC->RCC_CFGR;

	/* Get HCLK prescaler */
	presc = APBAHBPrescTable[RCC_CFGR_HPRE_Get(mull)];
	RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

	/* Get PCLK1 prescaler */
	presc = APBAHBPrescTable[RCC_CFGR_PPRE1_Get(mull)];
	RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

	/* Get PCLK2 prescaler */
	presc = APBAHBPrescTable[RCC_CFGR_PPRE2_Get(mull)];
	RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
}



