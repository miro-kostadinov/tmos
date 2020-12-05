/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_usart
 * @{
 * @file     hardware/stm32/stm32f2/usart_f2.cpp
 * @brief    STM32F2 USART
 * @version  V3.00
 * @date     12. November 2012
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include <fam_cpp.h>


/**
 * Get peripheral clock for given uart id
 *
 * @param periph_id
 * @return
 */
unsigned int get_usart_source_clk(unsigned int periph_id)
{
	uint32_t freq = 0;

	switch(periph_id)
	{
	case ID_PERIPH_USART1:
	case ID_PERIPH_USART6:
		switch (RCC->RCC_D2CCIP2R & RCC_D2CCIP2R_USART16SEL)
		{
		case RCC_D2CCIP2R_USART16SEL_pclk2:
			freq = RCC_GetPCLKxFreq(RCC_PCLK2_INDX);
			break;

		case RCC_D2CCIP2R_USART16SEL_pll2:
			freq = RCC_GetPLLxClockFreq_Q(RCC_PLL2_INDX);
			break;

		case RCC_D2CCIP2R_USART16SEL_pll3:
			freq = RCC_GetPLLxClockFreq_Q(RCC_PLL3_INDX);
			break;

		case RCC_D2CCIP2R_USART16SEL_hsi:
			freq = rcc_get_hsi_clk();
			break;

		case RCC_D2CCIP2R_USART16SEL_csi:
			freq = CSI_VALUE;
			break;

		case RCC_D2CCIP2R_USART16SEL_lse:
			freq = LSE_VALUE;
			break;

		}
		break;

	case ID_PERIPH_USART2:
	case ID_PERIPH_USART3:
	case ID_PERIPH_UART4:
	case ID_PERIPH_UART5:
	case ID_PERIPH_UART7:
	case ID_PERIPH_UART8:
		switch (RCC->RCC_D2CCIP2R & RCC_D2CCIP2R_USART234578SEL)
		{
		case RCC_D2CCIP2R_USART234578SEL_pclk1:
			freq = RCC_GetPCLKxFreq(RCC_PCLK1_INDX);
			break;

		case RCC_D2CCIP2R_USART234578SEL_pll2:
			freq = RCC_GetPLLxClockFreq_Q(RCC_PLL2_INDX);
			break;

		case RCC_D2CCIP2R_USART234578SEL_pll3:
			freq = RCC_GetPLLxClockFreq_Q(RCC_PLL3_INDX);
			break;

		case RCC_D2CCIP2R_USART234578SEL_hsi:
			freq = rcc_get_hsi_clk();
			break;

		case RCC_D2CCIP2R_USART234578SEL_csi:
			freq = CSI_VALUE;
			break;

		case RCC_D2CCIP2R_USART234578SEL_lse:
			freq = LSE_VALUE;
			break;

		}
		break;
	}

	return freq;
}


/*

unsigned int set_usart_baudrate(USART_TypeDef* usart, uint32_t periph_id, uint32_t rate)
{
	uint32_t integerdivider , fractionaldivider;

	integerdivider = ((25 * get_usart_source_clk(periph_id)) / (2 * rate));
	if( !(usart->USART_CR1 & USART_CR1_OVER8) )
		integerdivider /= 2;

	fractionaldivider = integerdivider;
	integerdivider /= 100;
	fractionaldivider -=  100 * integerdivider;
	if (usart->USART_CR1 & USART_CR1_OVER8)
	{
		fractionaldivider = (((fractionaldivider * 8) + 50) / 100) &  0x07;
	}
	else
	{
		fractionaldivider = (((fractionaldivider * 16) + 50) / 100) &  0x0F;
	}

	usart->USART_BRR = USART_BRR_DIV_Mantissa_Set(integerdivider) | fractionaldivider;

	return 1;
}

*/


