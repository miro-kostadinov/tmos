/**************************************************************************//**
 * @ingroup	 hardware_stm32f0_usart
 * @{
 * @file     hardware/stm32/stm32f0/usart_f0.cpp
 * @brief    STM32F0 USART
 * @version  V3.00
 * @date     2. December 2014
 * @author	 Miroslav Kostadinov
 */

#include <tmos.h>
#include <fam_cpp.h>


/**
 * Get peripheral clock for given uart id
 * @param periph_id
 * @param rate
 * @return clock in Hz
 */
unsigned int get_usart_source_clk(unsigned int periph_id, unsigned int rate)
{
	RCC_ClocksTypeDef  clocks;

	RCC_GetClocksFreq (&clocks);

	if (periph_id == ID_PERIPH_USART1 )
	{
	   switch(rate>>30)
	   {
		case RCC_CFGR3_USART1SW_SYSCLK:
			clocks.PCLK1_Frequency = clocks.SYSCLK_Frequency;
		  break;
		case RCC_CFGR3_USART1SW_LSE:
			clocks.PCLK1_Frequency = LSE_VALUE;
		  break;
		case RCC_CFGR3_USART1SW_HSI:
			clocks.PCLK1_Frequency = HSI_VALUE;
		  break;
	   }
	}

	return clocks.PCLK1_Frequency;
}

/**
 * Sets USART baudrate
 * @param usart
 * @param periph_id
 * @param rate
 * @return 1
 */
unsigned int set_usart_baudrate(USART_TypeDef* usart, uint32_t periph_id, uint32_t rate)
{
	uint32_t freq;

	freq = get_usart_source_clk(periph_id, rate);
	rate &= ~(3<<30);
	if (usart->USART_CR1 & USART_CR1_OVER8)
	{
		freq = (freq*2) /rate;
	    rate = (freq & 0xFFF0) | ((freq & 0xF) >> 1);

	} else
	{
		rate =  freq /rate;
	}

	usart->USART_BRR = rate;

	return 1;
}







/** @} */


