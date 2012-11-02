/*
 * rcc_f1.cpp
 *
 *  Created on: Nov 1, 2012
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
