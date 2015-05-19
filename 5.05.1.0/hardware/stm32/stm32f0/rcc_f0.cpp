/*
 * rcc_f0.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: miro
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
    	if(periph_id)
    	{
    		reg = &RCC->RCC_CIR; // [1]=RCC_APB2RSTR, [2]=RCC_APB1RSTR

    	} else
    	{
   			reg = &RCC->RCC_AHBRSTR;
    	}

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
    	if(periph_id >> 5)
    	{
    		reg = &RCC->RCC_CIR;
    		reg += periph_id >> 5; // [1]=RCC_APB2RSTR, [2]=RCC_APB1RSTR

    	} else
    	{
   			reg = &RCC->RCC_AHBRSTR;
    	}

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

		reg = &RCC->RCC_AHBENR;

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

		reg = &RCC->RCC_AHBENR;

		// Disable this peripheral clock.
		reg[index] &= ~(1<<(periph_id&0x1f));
    }
}

void RCCPeripheralLPEnable(unsigned int periph_id)
{
}

void RCCPeripheralLPDisable(unsigned int periph_id)
{
}

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
		presc = RCC->RCC_CFGR;
		mull = RCC_CFGR_PLLMUL_Get(presc);
		if(mull >=15)
			mull = 14;
		mull += 2;

		if (presc & RCC_CFGR_PLLSRC) //pllsource
		{
			/* HSE/PREDIV selected as PLL input clock */
			mull *= HSE_VALUE;
			presc = (RCC->RCC_CFGR2 & RCC_CFGR2_PREDIV) + 1;
		} else
		{
			/* HSI/2 selected as PLL input clock */
			mull *= HSI_VALUE;
			presc = 2;
		}

		RCC_Clocks->SYSCLK_Frequency = mull / presc;
		break;

	default:
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;
	}


	mull = RCC->RCC_CFGR;

	/* Get HCLK prescaler */
	presc = RCC_CFGR_HPRE_Get(mull);
	switch(presc)
	{
	case 8 ... 11:
		presc -= 7;
		break;
	case 12 ... 15:
		presc -= 6;
		break;
	default:
		presc = 0;
		break;
	}
	RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

	presc = RCC_CFGR_PPRE_Get(mull);
	if(presc >= 4)
		presc -= 3;
	else
		presc = 0;
	/* Get PCLK1 prescaler */
	RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

	/* Get PCLK2 prescaler */
	if(presc)
		presc --;
	RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
}



