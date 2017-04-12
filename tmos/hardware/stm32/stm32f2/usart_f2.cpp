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
	RCC_ClocksTypeDef  clocks;

	RCC_GetClocksFreq (&clocks);

	if (periph_id == ID_PERIPH_USART1 || periph_id == ID_PERIPH_USART6)
	{
		return clocks.PCLK2_Frequency;
	}

	return clocks.PCLK1_Frequency;
}


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



