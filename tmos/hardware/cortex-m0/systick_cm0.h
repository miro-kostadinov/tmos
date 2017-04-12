/**************************************************************************//**
 * @ingroup	hardware_cm0_systick
 * @file	hardware/cortex-m0/systick_cm0.h
 * @brief  	Cortex-M0 SYST
 * @date    9. September 2014
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_cm0_systick  System timer, SysTick (SYST)
 * @ingroup	 hardware_cm0
 * Source and definitions for Cortex-M0 System timer, SysTick (SYST)
 * @{
 *
 ******************************************************************************/

#ifndef SYSTICK_CM0_H_
#define SYSTICK_CM0_H_


#include <tmos_types.h>

/***************************************************************************//**
 *  System timer, SysTick (SYST) 0xE000E010
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct SYST_Type
{
  __IO uint32_t SYST_CSR;   //!< (SYST Offset: 0x00) SysTick Control and Status Register
  __IO uint32_t SYST_RVR;   //!< (SYST Offset: 0x04  SysTick Reload Value Register
  __IO uint32_t SYST_CVR;   //!< (SYST Offset: 0x08  SysTick Current Value Register
  __I  uint32_t SYST_CALIB; //!< (SYST Offset: 0x0C  SysTick Calibration Register
};
#endif /* __ASSEMBLY__ */

/***************************************************************************//**
 * @relates SYST_Type
 * @{
 ******************************************************************************/

/** @name SYST_CSR:   (SYST Offset: 0x00) SysTick Control and Status Register */
/** @{ */
#define SYST_CSR_COUNTFLAG		   0x00010000 //!< Returns 1 if timer counted to 0 since last time this was read
#define SYST_CSR_CLKSOURCE		   0x00000004 //!< clock source: 0=external clock, 1=processor clock
#define SYST_CSR_TICKINT		   0x00000002 //!< Enables SysTick exception request
#define SYST_CSR_ENABLE			   0x00000001 //!< Enables the counter
/** @} */

/** @name SYST_RVR:   (SYST Offset: 0x04  SysTick Reload Value Register       */
/** @{ */
#define SYST_RVR_RELOAD			   0x00FFFFFF //!< Value to load into the SYST_CVR register when the counter is enabled and when it reaches 0
/** @} */

/** @name SYST_CVR:   (SYST Offset: 0x08  SysTick Current Value Register      */
/** @{ */
#define SYST_CVR_CURRENT		   0x00FFFFFF //!< Reads return the current value of the SysTick counter
/** @} */

/** @name SYST_CALIB: (SYST Offset: 0x0C  SysTick Calibration Register        */
/** @{ */
#define SYST_CALIB_NOREF		   0x80000000 //!< Indicates whether the device provides a reference clock to the processor
#define SYST_CALIB_SKEW			   0x40000000 //!< Indicates whether the TENMS value is exact
#define SYST_CALIB_TENMS		   0x00FFFFFF //!< Reload value for 10ms (100Hz) timing, subject to system clock skew errors
/** @} */

/** @} */ // @relates SYST_Type


#endif /* SYSTICK_CM0_H_ */

/** @} */
