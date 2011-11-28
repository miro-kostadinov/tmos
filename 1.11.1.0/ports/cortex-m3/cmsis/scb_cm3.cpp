/**
 * @file: scb_cm3.cpp
 *
 * @brief    CMSIS Cortex-M3 Core Peripheral Access Layer Header File
 *
 * license: check README.TXT
 * @date 2010-3-12
 *
 */

#include <tmos.h>
#include "cmsis_cpp.h"

/* ##########################   NVIC functions  #################################### */

/**
 * @brief  Set the Priority Grouping in NVIC Interrupt Controller
 *
 * @param  PriorityGroup is priority grouping field
 *
 * Set the priority grouping field using the required unlock sequence.
 * The parameter priority_grouping is assigned to the field
 * SCB->AIRCR [10:8] PRIGROUP field. Only values from 0..7 are used.
 * In case of a conflict between priority grouping and available
 * priority bits (__NVIC_PRIO_BITS) the smallest possible priority group is set.
 */
void SCB_Type::NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);                         /* only values 0..7 are used          */

  reg_value  =  this->AIRCR;                                                   /* read old register configuration    */
  reg_value &= ~(SCB_AIRCR_VECTKEY_Msk | SCB_AIRCR_PRIGROUP_Msk);             /* clear bits to change               */
  reg_value  =  (reg_value                       |
                (0x5FA << SCB_AIRCR_VECTKEY_Pos) |
                (PriorityGroupTmp << 8));                                     /* Insert write key and priorty group */
  this->AIRCR =  reg_value;
}

/**
 * @brief  Get the Priority Grouping from NVIC Interrupt Controller
 *
 * @return priority grouping field
 *
 * Get the priority grouping from NVIC Interrupt Controller.
 * priority grouping is SCB->AIRCR [10:8] PRIGROUP field.
 */
uint32_t SCB_Type::NVIC_GetPriorityGrouping(void)
{
  return ((this->AIRCR & SCB_AIRCR_PRIGROUP_Msk) >> SCB_AIRCR_PRIGROUP_Pos);   /* read priority grouping field */
}

/**
*
* @brief Resets the device.
*
* @return This function does not return.
*
* This function will perform a software reset of the entire device.  The
* processor and all peripherals will be reset and all device registers will
* return to their default values (with the exception of the reset cause
* register, which will maintain its current value but have the software reset
* bit set as well).
*
*/
void SCB_Type::SysCtlReset(void)
{
    this->AIRCR = (0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk;
    while(1);
}

/**
*
* @brief Puts the processor into deep-sleep mode.
*
* @return None.
*
* This function places the processor into deep-sleep mode; it will not return
* until the processor returns to run mode.  The peripherals that are enabled
* via SysCtlPeripheralDeepSleepEnable() continue to operate and can wake up
* the processor (if automatic clock gating is enabled with
* SysCtlPeripheralClockGating(), otherwise all peripherals continue to
* operate).
*
*/
void SCB_Type::SysCtlDeepSleep(void)
{
	this->SCR |= SCB_SCR_SLEEPDEEP_Msk;     // Enable deep-sleep.
    __WFI();    							// Wait for an interrupt.
    this->SCR &= ~ SCB_SCR_SLEEPDEEP_Msk;	// Disable deep-sleep so that a future sleep will work correctly.
}
