/**************************************************************************//**
 * @file     core_cmInstr.h
 * @brief    CMSIS Cortex-M Core Instruction Access Header File
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

#ifndef __CORE_CMINSTR_H__
#define __CORE_CMINSTR_H__

/* #########################  Core Instruction Access  ######################### */

#if (defined (__GNUC__)) /*------------------ GNU Compiler ---------------------*/
/* GNU gcc specific functions */

 /**
 * @brief  No Operation
 *
 * No Operation does nothing. This instruction can be used for code alignment
 * purposes.
 */
static inline void __NOP(void)                      { asm volatile ("nop"); }

 /**
 * @brief  Wait For Interrupt
 *
 * Wait For Interrupt is a hint instruction that suspends execution until
 * one of a number of events occurs.
 */
static inline void __WFI(void)                      { asm volatile ("wfi"); }

 /**
 * @brief  Wait For Event
 *
 * Wait For Event is a hint instruction that permits the processor to enter
 * a low-power state until one of a number of events occurs.
 */
static inline void __WFE(void)                      { asm volatile ("wfe"); }

 /**
 * @brief  Send Event
 *
 * Send Event is a hint instruction. It causes an event to be signaled
 * to the CPU.
 */
static inline void __SEV(void)                      { asm volatile ("sev"); }

 /**
 * @brief  Instruction Synchronization Barrier
 *
 * Instruction Synchronization Barrier flushes the pipeline in the processor,
 * so that all instructions following the ISB are fetched from cache or
 * memory, after the instruction has been completed
 */
static inline void __ISB(void)                      { asm volatile ("isb"); }

 /**
 * @brief  Data Synchronization Barrier
 *
 * The DSB instruction operation acts as a special kind of Data Memory Barrier.
 * The DSB operation completes when all explicit memory accesses before this
 * instruction complete.
 */
static inline void __DSB(void)                      { asm volatile ("dsb"); }

 /**
 * @brief  Data Memory Barrier
 *
 * DMB ensures the apparent order of the explicit memory operations before
 * and after the instruction, without ensuring their completion.
 */
static inline void __DMB(void)                      { asm volatile ("dmb"); }

/**
 * @brief  Reverse byte order (32 bit)
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in integer value
 */
static inline uint32_t __REV(uint32_t value)
{
  uint32_t result;

  asm volatile ("rev %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}

/**
 * @brief  Reverse byte order (16 bit)
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in unsigned short value
 */
static inline uint32_t __REV16(uint16_t value)
{
  uint32_t result;

  asm volatile ("rev16 %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}

/**
 * @brief  Reverse byte order in signed short value with sign extension to integer
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse byte order in signed short value with sign extension to integer
 */
static inline int32_t __REVSH(int16_t value)
{
  uint32_t result=0;

  asm volatile ("revsh %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}


#if       (__CORTEX_M >= 0x03)

/**
 * @brief  Reverse bit order of value
 *
 * @param  value  value to reverse
 * @return        reversed value
 *
 * Reverse bit order of value
 */
static inline uint32_t __RBIT(uint32_t value)
{
  uint32_t result=0;

   asm volatile ("rbit %0, %1" : "=r" (result) : "r" (value) );
   return(result);
}

/**
 * @brief  LDR Exclusive (8 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 8 bit value
 */
static inline uint8_t __LDREXB(uint8_t *addr)
{
    uint8_t result=0;

   asm volatile ("ldrexb %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}

/**
 * @brief  LDR Exclusive (16 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 16 bit values
 */
static inline uint16_t __LDREXH(uint16_t *addr)
{
    uint16_t result=0;

   asm volatile ("ldrexh %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}

/**
 * @brief  LDR Exclusive (32 bit)
 *
 * @param  *addr  address pointer
 * @return        value of (*address)
 *
 * Exclusive LDR command for 32 bit values
 */
static inline uint32_t __LDREXW(uint32_t *addr)
{
    uint32_t result=0;

   asm volatile ("ldrex %0, [%1]" : "=r" (result) : "r" (addr) );
   return(result);
}

/**
 * @brief  STR Exclusive (8 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 8 bit values
 */
static inline uint32_t __STREXB(uint8_t value, uint8_t *addr)
{
   uint32_t result=0;

   asm volatile ("strexb %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}

/**
 * @brief  STR Exclusive (16 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 16 bit values
 */
static inline uint32_t __STREXH(uint16_t value, uint16_t *addr)
{
   uint32_t result=0;

   asm volatile ("strexh %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}

/**
 * @brief  STR Exclusive (32 bit)
 *
 * @param  value  value to store
 * @param  *addr  address pointer
 * @return        successful / failed
 *
 * Exclusive STR command for 32 bit values
 */
static inline uint32_t __STREXW(uint32_t value, uint32_t *addr)
{
   uint32_t result=0;

   asm volatile ("strex %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (value) );
   return(result);
}

 /**
 * @brief  Remove the exclusive lock created by ldrex
 *
 * Removes the exclusive lock which is created by ldrex.
 */
static inline void __CLREX(void)                    { asm volatile ("clrex"); }

#endif /* (__CORTEX_M >= 0x03) */



#endif

#endif // __CORE_CMINSTR_H__
