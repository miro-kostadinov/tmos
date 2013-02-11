/**
 * @file: nvic_cm3.cpp
 *
 * brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 *
 * license: check README.TXT
 * @date 2010-3-12
 *
 */

#include <tmos.h>
#include "cmsis_cpp.h"

/* ##########################   NVIC functions  #################################### */
/**
 * @brief  Enable Interrupt in NVIC Interrupt Controller
 *
 * @param  IRQn   The positive number of the external interrupt to enable
 *
 * Enable a device specific interupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */
void NVIC_Type::NVIC_EnableIRQ(DRIVER_INDEX IRQn)
{
  this->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* enable interrupt */
}

/**
 * @brief  Disable the interrupt line for external interrupt specified
 *
 * @param  IRQn   The positive number of the external interrupt to disable
 *
 * Disable a device specific interupt in the NVIC interrupt controller.
 * The interrupt number cannot be a negative value.
 */
void NVIC_Type::NVIC_DisableIRQ(DRIVER_INDEX IRQn)
{
  this->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* disable interrupt */
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
uint32_t NVIC_Type::NVIC_GetPendingIRQ(DRIVER_INDEX IRQn)
{
  return((uint32_t) ((this->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0)); /* Return 1 if pending else 0 */
}

/**
 * @brief  Set the pending bit for an external interrupt
 *
 * @param  IRQn    The number of the interrupt for set pending
 *
 * Set the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
void NVIC_Type::NVIC_SetPendingIRQ(DRIVER_INDEX IRQn)
{
  this->ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* set interrupt pending */
}

/**
 * @brief  Clear the pending bit for an external interrupt
 *
 * @param  IRQn    The number of the interrupt for clear pending
 *
 * Clear the pending bit for the specified interrupt.
 * The interrupt number cannot be a negative value.
 */
void NVIC_Type::NVIC_ClearPendingIRQ(DRIVER_INDEX IRQn)
{
  this->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); /* Clear pending interrupt */
}

/**
 * @brief  Read the active bit for an external interrupt
 *
 * @param  IRQn    The number of the interrupt for read active bit
 * @return         1 = interrupt active, 0 = interrupt not active
 *
 * Read the active register in NVIC and returns 1 if its status is active,
 * otherwise it returns 0.
 */
uint32_t NVIC_Type::NVIC_GetActive(DRIVER_INDEX IRQn)
{
  return((uint32_t)((this->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0)); /* Return 1 if active else 0 */
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
void NVIC_Type::NVIC_SetPriority(DRIVER_INDEX IRQn, uint32_t priority)
{
  if(IRQn < 0) {
    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff); } /* set Priority for Cortex-M3 System Interrupts */
  else {
    this->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);    }        /* set Priority for device specific Interrupts  */
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
uint32_t NVIC_Type::NVIC_GetPriority(DRIVER_INDEX IRQn)
{

  if(IRQn < 0) {
    return((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] >> (8 - __NVIC_PRIO_BITS)));  } /* get priority for Cortex-M3 system interrupts */
  else {
    return((uint32_t)(this->IP[(uint32_t)(IRQn)]           >> (8 - __NVIC_PRIO_BITS)));  } /* get priority for device specific interrupts  */
}


/**
 * @brief  Encode the priority for an interrupt
 *
 * @param  PriorityGroup    The used priority group
 * @param  PreemptPriority  The preemptive priority value (starting from 0)
 * @param  SubPriority      The sub priority value (starting from 0)
 * @return                  The encoded priority for the interrupt
 *
 * Encode the priority for an interrupt with the given priority group,
 * preemptive priority value and sub priority value.
 * In case of a conflict between priority grouping and available
 * priority bits (__NVIC_PRIO_BITS) the samllest possible priority group is set.
 *
 * The returned priority value can be used for NVIC_SetPriority(...) function
 */
uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);          /* only values 0..7 are used          */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  return (
           ((PreemptPriority & ((1 << (PreemptPriorityBits)) - 1)) << SubPriorityBits) |
           ((SubPriority     & ((1 << (SubPriorityBits    )) - 1)))
         );
}


/**
 * @brief  Decode the priority of an interrupt
 *
 * @param  Priority           The priority for the interrupt
 * @param  PriorityGroup      The used priority group
 * @param  pPreemptPriority   The preemptive priority value (starting from 0)
 * @param  pSubPriority       The sub priority value (starting from 0)
 *
 * Decode an interrupt priority value with the given priority group to
 * preemptive priority value and sub priority value.
 * In case of a conflict between priority grouping and available
 * priority bits (__NVIC_PRIO_BITS) the samllest possible priority group is set.
 *
 * The priority value can be retrieved with NVIC_GetPriority(...) function
 */
void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* pPreemptPriority, uint32_t* pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);          /* only values 0..7 are used          */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;

  *pPreemptPriority = (Priority >> SubPriorityBits) & ((1 << (PreemptPriorityBits)) - 1);
  *pSubPriority     = (Priority                   ) & ((1 << (SubPriorityBits    )) - 1);
}


