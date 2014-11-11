/*
 * scb_cm3.h
 *
 *  Created on: Dec 17, 2013
 *      Author: miro
 */

#ifndef SCB_CM3_H_
#define SCB_CM3_H_

#include <tmos_types.h>

/*******************************************************************************
 *  System Control Block (SCB)
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
  __IO uint32_t SCB_CFSR;    //!< (SCB Offset: 0x28)  Configurable Fault Status Register
  __IO uint32_t SCB_HFSR;    //!< (SCB Offset: 0x2C)  Hard Fault Status Register
  __IO uint32_t SCB_DFSR;    //!< (SCB Offset: 0x30)  Debug Fault Status Register
  __IO uint32_t SCB_MMFAR;   //!< (SCB Offset: 0x34)  Mem Manage Address Register
  __IO uint32_t SCB_BFAR;    //!< (SCB Offset: 0x38)  Bus Fault Address Register
  __IO uint32_t SCB_AFSR;    //!< (SCB Offset: 0x3C)  Auxiliary Fault Status Register
  __I  uint32_t SCB_PFR[2];  //!< (SCB Offset: 0x40)  Processor Feature Register
  __I  uint32_t SCB_DFR;     //!< (SCB Offset: 0x48)  Debug Feature Register
  __I  uint32_t SCB_ADR;     //!< (SCB Offset: 0x4C)  Auxiliary Feature Register
  __I  uint32_t SCB_MMFR[4]; //!< (SCB Offset: 0x50)  Memory Model Feature Register
  __I  uint32_t SCB_ISAR[5]; //!< (SCB Offset: 0x60)  ISA Feature Register
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
#define SCB_ICSR_RETTOBASE		   0x00000800 //!< Indicates whether there are preempted active exceptions (0-yes)
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
#define SCB_AIRCR_PRIGROUP		   0x00000700 //!< Interrupt priority grouping field determines the split of group priority from subpriority
#define SCB_AIRCR_PRIGROUP_128	   0x00000000 //!<  7:1 128 Group priorities   2 Subpriorities
#define SCB_AIRCR_PRIGROUP_64	   0x00000100 //!< 	7:2  64 Group priorities   4 Subpriorities
#define SCB_AIRCR_PRIGROUP_32	   0x00000200 //!<	7:3  32 Group priorities   8 Subpriorities
#define SCB_AIRCR_PRIGROUP_16	   0x00000300 //!<	7:4  16 Group priorities  16 Subpriorities
#define SCB_AIRCR_PRIGROUP_8	   0x00000400 //!<	7:5   8 Group priorities  32 Subpriorities
#define SCB_AIRCR_PRIGROUP_4	   0x00000500 //!<	7:6   4 Group priorities  64 Subpriorities
#define SCB_AIRCR_PRIGROUP_2	   0x00000600 //!<	7:7   2 Group priorities 128 Subpriorities
#define SCB_AIRCR_PRIGROUP_1	   0x00000700 //!<	0:8   1 Group priorities 256 Subpriorities
#define SCB_AIRCR_SYSRESETREQ	   0x00000004 //!< System reset request bit (WO)
#define SCB_AIRCR_VECTCLRACTIVE	   0x00000002 //!< Reserved for Debug use
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
#define SCB_CCR_STKALIGN		   0x00000200 //!< Indicates stack alignment on exception entry
#define SCB_CCR_STKALIGN_4		   0x00000000 //!< 	4-byte aligned
#define SCB_CCR_STKALIGN_8		   0x00000200 //!< 	8-byte aligned
#define SCB_CCR_BFHFNMIGN		   0x00000100 //!< Enables handlers with priority -1 or -2 to ignore data BusFaults caused by load and store instructions
#define SCB_CCR_DIV_0_TRP		   0x00000010 //!< Enables faulting or halting when the processor executes an SDIV or UDIV instruction with a divisor of 0
#define SCB_CCR_UNALIGN_TRP		   0x00000008 //!< Enables unaligned access traps
#define SCB_CCR_USERSETMPEND	   0x00000002 //!< Enables unprivileged software access to the NVIC_STIR
#define SCB_CCR_NONBASETHRDENA	   0x00000001 //!< Indicates how the processor enters Thread mode
/** @} */

/** @defgroup SCB_SHPR[12]:  (SCB Offset: 0x18)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
// The SHPRx registers set the priority level, 0 to 255, of the exception handlers that have
// configurable priority
#define SCB_SHPR_indx_MemManage		0	//!< PRI_4 Priority of system handler 4, MemManage
#define SCB_SHPR_indx_BusFault		1	//!< PRI_5 Priority of system handler 5, BusFault
#define SCB_SHPR_indx_UsageFault	2	//!< PRI_6 Priority of system handler 6, UsageFault
#define SCB_SHPR_indx_SVCall		7	//!< PRI_11 Priority of system handler 11, SVCall
#define SCB_SHPR_indx_PendSV		10	//!< PRI_14 Priority of system handler 14, PendSV
#define SCB_SHPR_indx_SysTick		11	//!< PRI_15 Priority of system handler 15, SysTick exception
/** @} */

/** @defgroup SCB_SHCSR:    (SCB Offset: 0x24)  System Handler Control and State Register */
#define SCB_SHCSR_USGFAULTENA	   0x00040000 //!< UsageFault enable bit
#define SCB_SHCSR_BUSFAULTENA	   0x00020000 //!< BusFault enable bit
#define SCB_SHCSR_MEMFAULTENA	   0x00010000 //!< MemManage enable bit
#define SCB_SHCSR_SVCALLPENDED	   0x00008000 //!< SVCall pending bit
#define SCB_SHCSR_BUSFAULTPENDED   0x00004000 //!< BusFault exception pending bit
#define SCB_SHCSR_MEMFAULTPENDED   0x00002000 //!< MemManage exception pending bit
#define SCB_SHCSR_USGFAULTPENDED   0x00001000 //!< UsageFault exception pending bit
#define SCB_SHCSR_SYSTICKACT	   0x00000800 //!< SysTick exception active bit
#define SCB_SHCSR_PENDSVACT		   0x00000400 //!< PendSV exception active bit
#define SCB_SHCSR_MONITORACT	   0x00000100 //!< Debug monitor active bit
#define SCB_SHCSR_SVCALLACT		   0x00000080 //!< SVCall active bit
#define SCB_SHCSR_USGFAULTACT 	   0x00000008 //!< UsageFault exception active bit
#define SCB_SHCSR_BUSFAULTACT	   0x00000002 //!< BusFault active bit
#define SCB_SHCSR_MEMFAULTACT	   0x00000001 //!< MemManage active bit
/** @} */

/** @defgroup SCB_CFSR:     (SCB Offset: 0x28)  Configurable Fault Status Register */
#define SCB_CFSR_UFSR 			   0xFFFF0000 //!< Usage Fault Status Register
#define SCB_CFSR_UFSR_DIVBYZERO	   0x02000000 //!<	Divide by zero UsageFault
#define SCB_CFSR_UFSR_UNALIGNED    0x01000000 //!<	Unaligned access UsageFault
#define SCB_CFSR_UFSR_NOCP 	       0x00080000 //!<	No coprocessor UsageFault
#define SCB_CFSR_UFSR_INVPC        0x00040000 //!<	Invalid PC load UsageFault, caused by an invalid PC load by EXC_RETURN
#define SCB_CFSR_UFSR_INVSTATE     0x00020000 //!<	Invalid state UsageFault, the PC value stacked for the exception return points to the instruction that attempted the illegal use of the EPSR
#define SCB_CFSR_UFSR_UNDEFINSTR   0x00010000 //!<	Undefined instruction UsageFault:

#define SCB_CFSR_BFSR 			   0x0000FF00 //!< Bus Fault Status Register
#define SCB_CFSR_BFSR_BFARVALID    0x00008000 //!< 	BusFault Address Register (SCB_BFAR) valid flag
#define SCB_CFSR_BFSR_LSPERR       0x00002000 //!< 	bus fault occurred during floating-point lazy state preservation
#define SCB_CFSR_BFSR_STKERR       0x00001000 //!< 	BusFault on stacking for exception entry
#define SCB_CFSR_BFSR_UNSTKERR     0x00000800 //!< 	BusFault on unstacking for a return from exception
#define SCB_CFSR_BFSR_IMPRECISERR  0x00000400 //!< 	Imprecise data bus error
#define SCB_CFSR_BFSR_PRECISERR    0x00000200 //!< 	Precise data bus error
#define SCB_CFSR_BFSR_IBUSERR      0x00000100 //!< 	Instruction bus error

#define SCB_CFSR_MMFSR 			   0x000000FF //!< Memory Management Fault Status Register
#define SCB_CFSR_MMFSR_MMARVALID   0x00000080 //!< 	MemManage Fault Address Register (SCB_MMFAR) valid flag
#define SCB_CFSR_MMFSR_MLSPERR     0x00000020 //!< 	MemManage fault occurred during floating-point lazy state preservation (Cortex-M4F)
#define SCB_CFSR_MMFSR_MSTKERR     0x00000010 //!< 	MemManage fault on stacking for exception entry
#define SCB_CFSR_MMFSR_MUNSTKERR   0x00000008 //!< 	MemManage fault on unstacking for a return from exception
#define SCB_CFSR_MMFSR_DACCVIOL    0x00000002 //!< 	Data access violation flag
#define SCB_CFSR_MMFSR_IACCVIOL    0x00000001 //!< 	Instruction access violation flag
/** @} */

/** @defgroup SCB_HFSR:     (SCB Offset: 0x2C)  Hard Fault Status Register    */
#define SCB_HFSR_DEBUGEVT		   0x80000000 //!< Reserved for Debug use
#define SCB_HFSR_FORCED			   0x40000000 //!< 	Indicates a forced hard fault, generated by escalation of a fault with configurable priority that cannot be handles, either because of priority or because it is disabled
#define SCB_HFSR_VECTTBL		   0x00000002 //!<	Indicates a BusFault on a vector table read during exception processing
/** @} */

/** @defgroup SCB_DFSR:     (SCB Offset: 0x30)  Debug Fault Status Register   */
#define SCB_DFSR_EXTERNA		   0x00000010 //!<
#define SCB_DFSR_VCATCH		       0x00000008 //!<
#define SCB_DFSR_DWTTRAP	  	   0x00000004 //!<
#define SCB_DFSR_BKPT		  	   0x00000002 //!<
#define SCB_DFSR_HALTED		  	   0x00000001 //!<
/** @} */

/** @defgroup SCB_MMFAR:    (SCB Offset: 0x34)  Mem Manage Address Register   */
/** @} */

/** @defgroup SCB_BFAR:     (SCB Offset: 0x38)  Bus Fault Address Register    */
/** @} */

/** @defgroup SCB_AFSR:     (SCB Offset: 0x3C)  Auxiliary Fault Status Register */
/** @} */

/** @defgroup SCB_PFR[2]:   (SCB Offset: 0x40)  Processor Feature Register    */
/** @} */

/** @defgroup SCB_DFR:      (SCB Offset: 0x48)  Debug Feature Register        */
/** @} */

/** @defgroup SCB_ADR:      (SCB Offset: 0x4C)  Auxiliary Feature Register    */
/** @} */

/** @defgroup SCB_MMFR[4]:  (SCB Offset: 0x50)  Memory Model Feature Register */
/** @} */

/** @defgroup SCB_ISAR[5]:  (SCB Offset: 0x60)  ISA Feature Register          */
/** @} */



/** @} */ // @defgroup SCB_regs_define

#endif /* SCB_CM3_H_ */
