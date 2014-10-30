/*
 * exti_stm32.cpp
 *
 *  Created on: Feb 19, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <fam_cpp.h>

void exti_enable_irq(EXTI_TypeDef* exti, uint32_t lines, pio_def type)
{
    /* Clear Rising Falling edge configuration */
	exti->EXTI_RTSR &= ~lines;
	exti->EXTI_FTSR &= ~lines;

    /* Select the trigger for the selected external interrupts */
	if(type & PD_INT_FE) // falling edge
		exti->EXTI_FTSR |= lines;
	if(type & PD_INT_RE) // rising edge
		exti->EXTI_RTSR |= lines;

	exti->EXTI_IMR |= lines;

}

void exti_enable_event(EXTI_TypeDef* exti, uint32_t lines, pio_def type)
{
    /* Clear Rising Falling edge configuration */
	exti->EXTI_RTSR &= ~lines;
	exti->EXTI_FTSR &= ~lines;

    /* Select the trigger for the selected external interrupts */
	if(type & PD_INT_FE) // falling edge
		exti->EXTI_FTSR |= lines;
	if(type & PD_INT_RE) // rising edge
		exti->EXTI_RTSR |= lines;

	exti->EXTI_EMR |= lines;
}

void exti_disable_irq(EXTI_TypeDef* exti, uint32_t lines)
{
	exti->EXTI_IMR &= ~lines;
}

void exti_disable_event(EXTI_TypeDef* exti, uint32_t lines)
{
	exti->EXTI_EMR &= ~lines;
}
