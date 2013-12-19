/*
 * core_instr.h
 *
 *  Created on: Dec 18, 2013
 *      Author: miro
 */

#ifndef CORE_INSTR_H_
#define CORE_INSTR_H_

#include <tmos_types.h>
#include <hardware_inc.h>

/**
 * @brief  No Operation
 *
 * No Operation does nothing. This instruction can be used for code alignment
 * purposes.
 */
static inline void __NOP(void)
{
	asm volatile ("nop");
}

 /**
 * @brief  Wait For Interrupt
 *
 * Wait For Interrupt is a hint instruction that suspends execution until
 * one of a number of events occurs.
 */
static inline void __WFI(void)
{
	asm volatile ("wfi");
}

 /**
 * @brief  Wait For Event
 *
 * Wait For Event is a hint instruction that permits the processor to enter
 * a low-power state until one of a number of events occurs.
 */
static inline void __WFE(void)
{
	asm volatile ("wfe");
}

 /**
 * @brief  Send Event
 *
 * Send Event is a hint instruction. It causes an event to be signaled
 * to the CPU.
 */
static inline void __SEV(void)
{
	asm volatile ("sev");
}

 /**
 * @brief  Instruction Synchronization Barrier
 *
 * Instruction Synchronization Barrier flushes the pipeline in the processor,
 * so that all instructions following the ISB are fetched from cache or
 * memory, after the instruction has been completed
 */
static inline void __ISB(void)
{
	asm volatile ("isb");
}

 /**
 * @brief  Data Synchronization Barrier
 *
 * The DSB instruction operation acts as a special kind of Data Memory Barrier.
 * The DSB operation completes when all explicit memory accesses before this
 * instruction complete.
 */
static inline void __DSB(void)
{
	asm volatile ("dsb");
}

 /**
 * @brief  Data Memory Barrier
 *
 * DMB ensures the apparent order of the explicit memory operations before
 * and after the instruction, without ensuring their completion.
 */
static inline void __DMB(void)
{
	asm volatile ("dmb");
}

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
static inline void __CLREX(void)
{
	asm volatile ("clrex");
}

#endif /* (__CORTEX_M >= 0x03) */

/**
* @brief  Enable IRQ Interrupts
*
* Enables IRQ interrupts by clearing the I-bit in the CPSR.
* Can only be executed in Privileged modes.
*/
static inline void __enable_irq(void)
{
	asm volatile ("cpsie i");
}

/**
* @brief  Disable IRQ Interrupts
*
* Disables IRQ interrupts by setting the I-bit in the CPSR.
* Can only be executed in Privileged modes.
*/
static inline void __disable_irq(void)
{
	asm volatile ("cpsid i");
}

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
	uint32_t result = 0;

	asm volatile ("MRS %0, ipsr" : "=r" (result) );
	return (result);
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
	uint32_t result = 0;

	asm volatile ("MRS %0, apsr" : "=r" (result) );
	return (result);
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
	uint32_t result = 0;

	asm volatile ("MRS %0, xpsr" : "=r" (result) );
	return (result);
}

/**
 * @brief  Return the Process Stack Pointer
 *
 * @return ProcessStackPointer
 *
 * Return the actual process stack pointer
 */
static inline uint32_t __get_PSP(void)
{
	register uint32_t result asm ("r0") = 0;

	asm volatile ("MRS %0, psp\n"
			"BX  lr     \n" : "=r" (result) );
	return (result);
}

/**
 * @brief  Set the Process Stack Pointer
 *
 * @param  topOfProcStack  Process Stack Pointer
 *
 * Assign the value ProcessStackPointer to the MSP
 * (process stack pointer) Cortex processor register
 */
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
static inline uint32_t __get_MSP(void)
{
	register uint32_t result asm ("r0") = 0;

	asm volatile ("MRS %0, msp\n"
			"BX  lr     \n" : "=r" (result) );
	return (result);
}

/**
 * @brief  Set the Main Stack Pointer
 *
 * @param  topOfMainStack  Main Stack Pointer
 *
 * Assign the value mainStackPointer to the MSP
 * (main stack pointer) Cortex processor register
 */
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
	uint32_t result = 0;

	asm volatile ("MRS %0, primask" : "=r" (result) );
	return (result);
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

#if (__CORTEX_M >= 0x03)

/**
 * @brief  Enable FIQ Interrupts
 *
 * Enables FIQ interrupts by clearing the F-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __enable_fault_irq(void)
{
	asm volatile ("cpsie f");
}

/**
 * @brief  Disable FIQ Interrupts
 *
 * Disables FIQ interrupts by setting the F-bit in the CPSR.
 * Can only be executed in Privileged modes.
 */
static inline void __disable_fault_irq(void)
{
	asm volatile ("cpsid f");
}

/**
 * @brief  Return the Base Priority value
 *
 * @return BasePriority
 *
 * Return the content of the base priority register
 */
static inline uint32_t __get_BASEPRI(void)
{
	uint32_t result = 0;

	asm volatile ("MRS %0, basepri_max" : "=r" (result) );
	return (result);
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
	uint32_t result = 0;

	asm volatile ("MRS %0, faultmask" : "=r" (result) );
	return (result);
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

#if (__CORTEX_M == 0x04)

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


#endif /* CORE_INSTR_H_ */
