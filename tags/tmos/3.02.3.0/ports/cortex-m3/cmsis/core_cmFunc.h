/**************************************************************************//**
 * @file     core_cmFunc.h
 * @brief    CMSIS Cortex-M Core Function Access Header File
 * @version  V1.40
 * @date     16. February 2010
 *
 * @note
 * Copyright (C) 2009-2010 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M
 * processor based microcontrollers.  This file can be freely distributed
 * within development tools that are supporting such ARM based processors.
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/

#ifndef __CORE_CMFUNC_H__
#define __CORE_CMFUNC_H__

#include <cmsis_cpp.h>

/* ###########################  Core Function Access  ########################### */

#if (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
/* GNU gcc specific functions */
 /**
 * @brief  Enable IRQ Interrupts
 *
 * Enables IRQ interrupts by clearing the I-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __enable_irq(void)               { asm volatile ("cpsie i"); }

 /**
 * @brief  Disable IRQ Interrupts
 *
 * Disables IRQ interrupts by setting the I-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __disable_irq(void)              { asm volatile ("cpsid i"); }

/**
 * @brief  Return the Control Register value
*
*  @return Control value
 *
 * Return the content of the control register
 */
static inline uint32_t __get_CONTROL(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, control" : "=r" (result) );
  return(result);
}

/**
 * @brief  Set the Control Register value
 *
 * @param  control  Control value
 *
 * Set the control register
 */
static inline void __set_CONTROL(uint32_t control)
{
  asm volatile ("MSR control, %0" : : "r" (control) );
}

/**
 * @brief  Get IPSR Register value
 *
 * @return uint32_t IPSR value
 *
 * return the content of the IPSR register
 */
static inline uint32_t __get_IPSR(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, ipsr" : "=r" (result) );
  return(result);
}

/**
 * @brief  Get APSR Register value
 *
 * @return uint32_t APSR value
 *
 * return the content of the APSR register
 */
static inline uint32_t __get_APSR(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, apsr" : "=r" (result) );
  return(result);
}

/**
 * @brief  Get xPSR Register value
 *
 * @return uint32_t xPSR value
 *
 * return the content of the xPSR register
 */
static inline uint32_t __get_xPSR(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, xpsr" : "=r" (result) );
  return(result);
}

/**
 * @brief  Return the Process Stack Pointer
 *
 * @return ProcessStackPointer
 *
 * Return the actual process stack pointer
 */
//static inline uint32_t __get_PSP(void) __attribute__( ( naked ) );
static inline uint32_t __get_PSP(void)
{
  register uint32_t result asm ("r0") = 0;

  asm volatile ("MRS %0, psp\n"
                  "BX  lr     \n"  : "=r" (result) );
  return(result);
}

/**
 * @brief  Set the Process Stack Pointer
 *
 * @param  topOfProcStack  Process Stack Pointer
 *
 * Assign the value ProcessStackPointer to the MSP
 * (process stack pointer) Cortex processor register
 */
//static inline void __set_PSP(uint32_t topOfProcStack) __attribute__( ( naked ) );
static inline void __set_PSP(uint32_t topOfProcStack)
{
  asm volatile ("MSR psp, %0\n"
                  "BX  lr     \n" : : "r" (topOfProcStack) );
}

/**
 * @brief  Return the Main Stack Pointer
 *
 * @return Main Stack Pointer
 *
 * Return the current value of the MSP (main stack pointer)
 * Cortex processor register
 */
//static inline uint32_t __get_MSP(void) __attribute__( ( naked ) );
static inline uint32_t __get_MSP(void)
{
  register uint32_t result asm ("r0") = 0;

  asm volatile ("MRS %0, msp\n"
                  "BX  lr     \n"  : "=r" (result) );
  return(result);
}

/**
 * @brief  Set the Main Stack Pointer
 *
 * @param  topOfMainStack  Main Stack Pointer
 *
 * Assign the value mainStackPointer to the MSP
 * (main stack pointer) Cortex processor register
 */
//static inline void __set_MSP(uint32_t topOfMainStack) __attribute__( ( naked ) );
static inline void __set_MSP(uint32_t topOfMainStack)
{
  asm volatile ("MSR msp, %0\n"
                  "BX  lr     \n" : : "r" (topOfMainStack) );
}

/**
 * @brief  Return the Priority Mask value
 *
 * @return PriMask
 *
 * Return state of the priority mask bit from the priority mask register
 */
static inline uint32_t __get_PRIMASK(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}

/**
 * @brief  Set the Priority Mask value
 *
 * @param  priMask  PriMask
 *
 * Set the priority mask bit in the priority mask register
 */
static inline void __set_PRIMASK(uint32_t priMask)
{
  asm volatile ("MSR primask, %0" : : "r" (priMask) );
}


#if       (__CORTEX_M >= 0x03)

 /**
 * @brief  Enable FIQ Interrupts
 *
 * Enables FIQ interrupts by clearing the F-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __enable_fault_irq(void)         { asm volatile ("cpsie f"); }

 /**
 * @brief  Disable FIQ Interrupts
 *
 * Disables FIQ interrupts by setting the F-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __disable_fault_irq(void)        { asm volatile ("cpsid f"); }

/**
 * @brief  Return the Base Priority value
 *
 * @return BasePriority
 *
 * Return the content of the base priority register
 */
static inline uint32_t __get_BASEPRI(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, basepri_max" : "=r" (result) );
  return(result);
}

/**
 * @brief  Set the Base Priority value
 *
 * @param  basePri  BasePriority
 *
 * Set the base priority register
 */
static inline void __set_BASEPRI(uint32_t value)
{
  asm volatile ("MSR basepri, %0" : : "r" (value) );
}

/**
 * @brief  Return the Fault Mask value
 *
 * @return FaultMask
 *
 * Return the content of the fault mask register
 */
static inline uint32_t __get_FAULTMASK(void)
{
  uint32_t result=0;

  asm volatile ("MRS %0, faultmask" : "=r" (result) );
  return(result);
}

#endif /* (__CORTEX_M >= 0x03) */

/**
 * @brief  Set the Fault Mask value
 *
 * @param  faultMask  faultMask value
 *
 * Set the fault mask register
 */
static inline void __set_FAULTMASK(uint32_t faultMask)
{
  asm volatile ("MSR faultmask, %0" : : "r" (faultMask) );
}


#if       (__CORTEX_M == 0x04)

/**
 * @brief  Return the FPSCR value
 *
 * @return FloatingPointStatusControlRegister
 *
 * Return the content of the FPSCR register
 */
static inline uint32_t __get_FPSCR(void)
{
#if (__FPU_PRESENT == 1)
  uint32_t result=0;

  asm volatile ("MRS %0, fpscr" : "=r" (result) );
  return(result);
#else
   return(0);
#endif
}

/**
 * @brief  Set the FPSCR value
 *
 * @param  fpscr  FloatingPointStatusControlRegister
 *
 * Set the FPSCR register
 */
static inline void __set_FPSCR(uint32_t fpscr)
{
#if (__FPU_PRESENT == 1)
  asm volatile ("MSR control, %0" : : "r" (fpscr) );
#endif
}

#endif /* (__CORTEX_M == 0x04) */


#elif (defined (__TASKING__)) /*--------------- TASKING Compiler -----------------*/
/* TASKING carm specific functions */

/*
 * The CMSIS functions have been implemented as intrinsics in the compiler.
 * Please use "carm -?i" to get an up to date list of all instrinsics,
 * Including the CMSIS ones.
 */

#endif

#endif // __CORE_CMFUNC_H__
