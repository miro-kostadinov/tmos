/*
 * scb_cm0.h
 *
 *  Created on: Sep 9, 2014
 *      Author: miro
 */

#ifndef SCB_CM0_H_
#define SCB_CM0_H_

#include <tmos_types.h>

/*******************************************************************************
 *  System Control Block (SCB) 0xE000ED00
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct SCB_Type
{
  __I  uint32_t SCB_CPUID;   //!< (SCB Offset: 0x00)  CPU ID Base Register
  __IO uint32_t SCB_ICSR;    //!< (SCB Offset: 0x04)  Interrupt Control State Register
  __IO uint32_t SCB_VTOR;    //!< (SCB Offset: 0x08)  Vector Table Offset Register
  __IO uint32_t SCB_AIRCR;   //!< (SCB Offset: 0x0C)  Application Interrupt / Reset Control Register
  __IO uint32_t SCB_SCR;     //!< (SCB Offset: 0x10)  System Control Register
  __IO uint32_t SCB_CCR;     //!< (SCB Offset: 0x14)  Configuration Control Register
  __IO uint8_t  SCB_SHPR[12];//!< (SCB Offset: 0x18)  System Handlers Priority Registers (4-7, 8-11, 12-15)
  __IO uint32_t SCB_SHCSR;   //!< (SCB Offset: 0x24)  System Handler Control and State Register
  __IO uint32_t reserved[2]; //!< (SCB Offset: 0x28)
  __IO uint32_t SCB_DFSR;    //!< (SCB Offset: 0x30)  Debug Fault Status Register
};
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @defgroup SCB_regs_define
 * @{
 ******************************************************************************/

/** @defgroup SCB_CPUID:    (SCB Offset: 0x00)  CPU ID Base Register          */
#define SCB_CPUID_IMPLEMENTER	   0xFF000000 //!< Implementer code
#define SCB_CPUID_IMPLEMENTER_ARM  0x41000000 //!<	ARM
#define SCB_CPUID_VARIANT		   0x00F00000 //!< Variant number, the r value in the rnpn product revision identifier
#define SCB_CPUID_PARTNO		   0x0000FFF0 //!< Part number of the processor
#define SCB_CPUID_PARTNO_M3		   0x0000C230 //!< 	Cortex-M3
#define SCB_CPUID_PARTNO_M4		   0x0000C240 //!< 	Cortex-M4
#define SCB_CPUID_REVISION		   0x0000000F //!< Revision number, the p value in the rnpn product revision identifier
/** @} */

/** @defgroup SCB_ICSR:     (SCB Offset: 0x04)  Interrupt Control State Register */
#define SCB_ICSR_NMIPENDSET		   0x80000000 //!< NMI set-pending bit
#define SCB_ICSR_PENDSVSET		   0x10000000 //!< PendSV set-pending bit
#define SCB_ICSR_PENDSVCLR		   0x08000000 //!< PendSV clear-pending bit (WO1)
#define SCB_ICSR_PENDSTSET		   0x04000000 //!< SysTick exception set-pending bit
#define SCB_ICSR_PENDSTCLR		   0x02000000 //!< SysTick exception clear-pending bit
#define SCB_ICSR_ISRPREEMPT		   0x00800000 //!< This bit is reserved for Debug use and reads-as-zero when the processor is not in Debug
#define SCB_ICSR_ISRPENDING		   0x00400000 //!< Interrupt pending flag, excluding NMI and Faults
#define SCB_ICSR_VECTPENDING	   0x003FF000 //!< Indicates the exception number of the highest priority pending enabled exception
#define SCB_ICSR_VECTACTIVE		   0x000001FF //!< Contains the active exception number (0 = Thread mode)
/** @} */

/** @defgroup SCB_VTOR:     (SCB Offset: 0x08)  Vector Table Offset Register  */
#define SCB_VTOR_TBLOFF			   0xFFFFFF80 //!< he offset of the vector table base address
/** @} */

/** @defgroup SCB_AIRCR:    (SCB Offset: 0x0C)  Application Interrupt / Reset Control Register */
#define SCB_AIRCR_VECTKEY		   0xFFFF0000 //!< Register key
#define SCB_AIRCR_VECTKEY_STAT	   0xFA050000 //!< 	Reads as 0xFA05
#define SCB_AIRCR_VECTKEY_VAL	   0x05FA0000 //!< 	write 0x5FA to VECTKEY
#define SCB_AIRCR_ENDIANESS		   0x00008000 //!< Data endianness bit is implementation defined (0 = Little-endian)
#define SCB_AIRCR_SYSRESETREQ	   0x00000004 //!< System reset request bit (WO)
#define SCB_AIRCR_VECTCLRACTIVE	   0x00000002 //!< Clears all active state information for fixed and configurable exceptions
#define SCB_AIRCR_VECTRESET		   0x00000001 //!< Reserved for Debug use
/** @} */

/** @defgroup SCB_SCR:      (SCB Offset: 0x10)  System Control Register       */
#define SCB_SCR_SEVONPEND		   0x00000010 //!< Send Event on Pending bit
#define SCB_SCR_SEVONPEND_en	   0x00000000 //!< 	only enabled interrupts or events can wakeup the processor
#define SCB_SCR_SEVONPEND_all	   0x00000010 //!< 	enabled events and all interrupts, including disabled interrupts, can wakeup the processor
#define SCB_SCR_SLEEPDEEP		   0x00000004 //!< Uses sleep(0) or deep sleep(1) as its low power mode
#define SCB_SCR_SLEEPONEXIT		   0x00000002 //!< Indicates sleep-on-exit when returning from Handler mode to Thread mode
/** @} */

/** @defgroup SCB_CCR:      (SCB Offset: 0x14)  Configuration Control Register */
#define SCB_CCR_STKALIGN		   0x00000200 //!< Indicates stack alignment on exception entry Read-As-One
#define SCB_CCR_STKALIGN_8		   0x00000200 //!< 	8-byte aligned (always)
#define SCB_CCR_UNALIGN_TRP		   0x00000008 //!< Enables unaligned access traps Read-As-One
/** @} */

/** @defgroup SCB_SHPR[12]:  (SCB Offset: 0x18)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
// The SHPRx registers set the priority level, 0 to 255, of the exception handlers that have
// configurable priority
#define SCB_SHPR_indx_SVCall		7	//!< PRI_11 Priority of system handler 11, SVCall
#define SCB_SHPR_indx_PendSV		10	//!< PRI_14 Priority of system handler 14, PendSV
#define SCB_SHPR_indx_SysTick		11	//!< PRI_15 Priority of system handler 15, SysTick exception
/** @} */

/** @defgroup SCB_SHCSR:    (SCB Offset: 0x24)  System Handler Control and State Register */
#define SCB_SHCSR_SVCALLPENDED	   0x00008000 //!< SVCall pending bit
/** @} */

/** @defgroup SCB_DFSR:     (SCB Offset: 0x30)  Debug Fault Status Register   */
#define SCB_DFSR_EXTERNA		   0x00000010 //!<
#define SCB_DFSR_VCATCH		       0x00000008 //!<
#define SCB_DFSR_DWTTRAP	  	   0x00000004 //!<
#define SCB_DFSR_BKPT		  	   0x00000002 //!<
#define SCB_DFSR_HALTED		  	   0x00000001 //!<
/** @} */



/** @} */ // @defgroup SCB_regs_define

#endif /* SCB_CM0_H_ */
