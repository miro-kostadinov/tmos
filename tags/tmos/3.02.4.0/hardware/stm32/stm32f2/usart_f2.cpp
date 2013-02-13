/*
 * usart_f2.cpp
 *
 *  Created on: Nov 12, 2012
 *      Author: miro
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

	if (periph_id == ID_PERIPH_USART1)
	{
		return clocks.PCLK2_Frequency;
	}

	return clocks.PCLK1_Frequency;
}





