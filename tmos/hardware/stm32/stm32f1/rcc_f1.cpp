/**************************************************************************//**
 * @ingroup	 hardware_stm32f1_rcc
 * @{
 * @file     hardware/stm32/stm32f1/rcc_f1.cpp
 * @brief    STM32F1 RCC
 * @version  V3.00
 * @date     1. November 2012
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>



void RCCPeripheralReset(unsigned int periph_id)
{
	__IO uint32_t* reg;

    if(periph_id < ID_NO_PERIPH)
    {
        switch (periph_id>>5)
        {
#ifdef STM32F10X_CL
        case 0:
        	reg = &RCC->RCC_AHBRSTR;
        	break;
#endif /* STM32F10X_CL */

        case 1:
        	reg = &RCC->RCC_APB2RSTR;
        	break;

        case 2:
        	reg = &RCC->RCC_APB1RSTR;
        	break;

        default:
        	reg = NULL;
        	break;
        }

        if(reg)
        {
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
}

void RCCPeripheralEnable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

        if(index < 3)
        {
        	reg = &RCC->RCC_AHBENR;

            // Enable this peripheral clock.
            reg[index] |= 1<<(periph_id&0x1f);
        }
    }
}

void RCCPeripheralDisable(unsigned int periph_id)
{
	__IO uint32_t* reg;
	unsigned int index;

    if(periph_id < ID_NO_PERIPH)
    {
    	index = periph_id>>5;

        if(index < 3)
        {
        	reg = &RCC->RCC_AHBENR;

            // Disable this peripheral clock.
            reg[index] &= ~(1<<(periph_id&0x1f));
        }
    }
}

const uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/**
  * @brief  Returns the frequencies of different on chip clocks.
  * @param  RCC_Clocks: pointer to a RCC_ClocksTypeDef structure which will hold
  *         the clocks frequencies.
  * @note   The result of this function could be not correct when using
  *         fractional value for HSE crystal.
  * @retval None
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
	uint32_t mull, presc;

#ifdef  STM32F10X_CL
	uint32_t prediv1factor, prediv2factor, pll2mull;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
	uint32_t prediv1factor;
#endif

	/* Get SYSCLK source -------------------------------------------------------*/
	switch (RCC->RCC_CFGR & RCC_CFGR_SWS)
	{
	case RCC_CFGR_SWS_HSI: /* HSI used as system clock */
		RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
		break;

	case RCC_CFGR_SWS_HSE: /* HSE used as system clock */
		RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
		break;

	case RCC_CFGR_SWS_PLL: /* PLL used as system clock */

		/* Get PLL clock source and multiplication factor ----------------------*/
		mull = RCC_CFGR_PLLMULL_Get(RCC->RCC_CFGR);

#ifndef STM32F10X_CL
		mull += 2;

		if (!(RCC->RCC_CFGR & RCC_CFGR_PLLSRC))
		{/* HSI oscillator clock divided by 2 selected as PLL clock entry */
			RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * mull;
		}
		else
		{
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
			prediv1factor = (RCC->RCC_CFGR2 & RCC_CFGR2_PREDIV1) + 1;
			/* HSE oscillator clock selected as PREDIV1 clock entry */
			RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * mull;
#else
			/* HSE selected as PLL clock entry */
			if (RCC->RCC_CFGR & RCC_CFGR_PLLXTPRE)
			{/* HSE oscillator clock divided by 2 */
				RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * mull;
			}
			else
			{
				RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * mull;
			}
#endif
		}
#else

		if (mull != RCC_CFGR_PLLMULL_Get(RCC_CFGR_PLLMULL6_5))
		{
			mull = (mull+2)*2;
		}
		else
		{ /* PLL multiplication factor = PLL input clock * 6.5 */
			mull = 13;
		}

		if (!(RCC->RCC_CFGR & RCC_CFGR_PLLSRC))
		{/* HSI oscillator clock divided by 2 selected as PLL clock entry */
			RCC_Clocks->SYSCLK_Frequency = ((HSI_VALUE >> 1) * mull)/2;
		}
		else
		{/* PREDIV1 selected as PLL clock entry */

			/* Get PREDIV1 clock source and division factor */
			prediv1factor = (RCC->RCC_CFGR2 & RCC_CFGR2_PREDIV1) + 1;

			if ( !(RCC->RCC_CFGR2 & RCC_CFGR2_PREDIV1SRC))
			{ /* HSE oscillator clock selected as PREDIV1 clock entry */
				RCC_Clocks->SYSCLK_Frequency = ((HSE_VALUE / prediv1factor) * mull)/2;
			}
			else
			{/* PLL2 clock selected as PREDIV1 clock entry */

				/* Get PREDIV2 division factor and PLL2 multiplication factor */
				prediv2factor = ((RCC->RCC_CFGR2 & RCC_CFGR2_PREDIV2) >> 4) + 1;
				pll2mull = ((RCC->RCC_CFGR2 & RCC_CFGR2_PLL2MUL) >> 8 ) + 2;
				RCC_Clocks->SYSCLK_Frequency = ((((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * mull)/2;
			}
		}
#endif /* STM32F10X_CL */
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

	/* Get ADCCLK prescaler */
	presc = RCC_CFGR_ADCPRE_Get(mull)*2 +2;
	RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}
