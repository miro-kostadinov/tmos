/*
 * nvic_cm0.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: miro
 */

#include <tmos.h>

/**
 * @brief  Enable Interrupt in NVIC Interrupt Controller
 *
 * @param  IRQn   The positive number of the external interrupt to enable
 *
 * Enable a device specific interrupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */
EXTERN_C void NVIC_EnableIRQ(uint32_t IRQn)
{
  NVIC->NVIC_ISER = 1 << IRQn; /* enable interrupt */
}

/**
 * @brief  Disable the interrupt line for external interrupt specified
 *
 * @param  IRQn   The positive number of the external interrupt to disable
 *
 * Disable a device specific interrupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */
EXTERN_C void NVIC_DisableIRQ(uint32_t IRQn)
{
  NVIC->NVIC_ICER = 1 << IRQn; /* disable interrupt */
}

/**
 * @brief  Read the interrupt pending bit for a device specific interrupt source
 *
 * @param  IRQn    The number of the device specifc interrupt
 * @return         1 = interrupt pending, 0 = interrupt not pending
 *
 * Read the pending register in NVIC and return 1 if its status is pending,
 * otherwise it returns 0
 */
EXTERN_C uint32_t NVIC_GetPendingIRQ(uint32_t IRQn)
{
  return (NVIC->NVIC_ISPR >> IRQn) & 1;
}

/**
 * @brief  Set the pending bit for an external interrupt
 *
 * @param  IRQn    The number of the interrupt for set pending
 *
 * Set the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
EXTERN_C void NVIC_SetPendingIRQ(uint32_t IRQn)
{
  NVIC->NVIC_ISPR = 1 << IRQn; /* set interrupt pending */
}

/**
 * @brief  Clear the pending bit for an external interrupt
 *
 * @param  IRQn    The number of the interrupt for clear pending
 *
 * Clear the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
EXTERN_C void NVIC_ClearPendingIRQ(uint32_t IRQn)
{
  NVIC->NVIC_ICPR = 1 << IRQn; /* Clear pending interrupt */
}


/**
 * @brief  Set the priority for an interrupt
 *
 * @param  IRQn      The number of the interrupt for set priority
 * @param  priority  The priority to set
 *
 * Set the priority for the specified interrupt. The interrupt
 * number can be positive to specify an external (device specific)
 * interrupt, or negative to specify an internal (core) interrupt.
 *
 * Note: The priority cannot be set for every core interrupt.
 */
EXTERN_C void NVIC_SetPriority(int32_t IRQn, uint32_t priority)
{
	__IO uint32_t* reg;
	uint32_t pos;

	if (IRQn < 0)
	{
		/* set Priority for Cortex-M0 System Interrupts */
		reg = &SCB->SCB_SHPR[( IRQn  + 12) >> 2];
	}
	else
	{
		/* set Priority for device specific Interrupts  */
		reg = &NVIC->NVIC_IPR[IRQn>>2];
	}
	pos = (IRQn & 3) << 3;
	reg[0] = (reg[0] & ~(0xFF << pos)) | (NVIC_PIORITY(priority) << pos);
}

/**
 * @brief  Read the priority for an interrupt
 *
 * @param  IRQn      The number of the interrupt for get priority
 * @return           The priority for the interrupt
 *
 * Read the priority for the specified interrupt. The interrupt
 * number can be positive to specify an external (device specific)
 * interrupt, or negative to specify an internal (core) interrupt.
 *
 * The returned priority value is automatically aligned to the implemented
 * priority bits of the microcontroller.
 *
 * Note: The priority cannot be set for every core interrupt.
 */
EXTERN_C uint32_t NVIC_GetPriority(int32_t IRQn)
{
	__IO uint32_t* reg;
	uint32_t val;

	if (IRQn < 0)
	{
		/* get Priority for Cortex-M0 System Interrupts */
		reg = &SCB->SCB_SHPR[( IRQn +12 ) >> 2];
	}
	else
	{
		/* get Priority for device specific Interrupts  */
		reg = &NVIC->NVIC_IPR[IRQn>>2];
	}
	val = (reg[0] >> ((IRQn & 3) << 3)) & 0xFF;
	return (val >> (8 - __NVIC_PRIO_BITS));
}




