/**************************************************************************//**
 * @ingroup	hardware_cm7_cdbg
 * @file	hardware/cortex-m7/cdbg_cm7.h
 * @brief  	Cortex-M7 CDBG
 * @date    18. December 2013
 * @author	Miroslav Kostadinov
 *
 * @defgroup hardware_cm7_cdbg  Core Debug (CDBG)
 * @ingroup	 hardware_cm7
 * Source and definitions for Cortex-M7 Core Debug (CDBG)
 * @{
 *
 ******************************************************************************/

#ifndef CDBG_CM7_H_
#define CDBG_CM7_H_

#include <tmos_types.h>

/***************************************************************************//**
 *  Core Debug (CDBG)
 ******************************************************************************/
#ifndef __ASSEMBLY__
struct CDBG_Type
{
  __IO uint32_t CDBG_DHCSR; //!< (CDBG Offset: 0x00  Debug Halting Control and Status Register    */
  __O  uint32_t CDBG_DCRSR; //!< (CDBG Offset: 0x04  Debug Core Register Selector Register        */
  __IO uint32_t CDBG_DCRDR; //!< (CDBG Offset: 0x08  Debug Core Register Data Register            */
  __IO uint32_t CDBG_DEMCR; //!< (CDBG Offset: 0x0C  Debug Exception and Monitor Control Register */
};
#endif /* __ASSEMBLY__ */

/*******************************************************************************
 * @relates CDBG_Type
 * @{
 ******************************************************************************/

/** @name CDBG_DHCSR: (CDBG Offset: 0x00  Debug Halting Control and Status Register    */
/** @{ */
#define CDBG_DHCSR_DBGKEY		0xFFFF0000 //!< Debug Key
#define CDBG_DHCSR_S_RESET_ST   0x02000000 //!< Indicates that the core has been reset, or is now being reset, since the last time this bit was read
#define CDBG_DHCSR_S_RETIRE_ST  0x01000000 //!< Indicates that an instruction has completed since last read
#define CDBG_DHCSR_S_LOCKUP     0x00080000 //!< Reads as one if the core is running (not halted) and a lockup condition is present
#define CDBG_DHCSR_S_SLEEP      0x00040000 //!< Indicates that the core is sleeping (WFI, WFE, or SLEEP-ON-EXIT)
#define CDBG_DHCSR_S_HALT       0x00020000 //!< The core is in debug state when S_HALT is set.
#define CDBG_DHCSR_S_REGRDY     0x00010000 //!< Register Read/Write on the Debug Core Register Selector register is available. Last transfer is complete.
#define CDBG_DHCSR_C_SNAPSTALL  0x00000020 //!< If the core is stalled on a load/store operation the stall ceases and the instruction is forced to complete.
#define CDBG_DHCSR_C_MASKINTS   0x00000008 //!< Mask interrupts when stepping or running in halted debug.
#define CDBG_DHCSR_C_STEP       0x00000004 //!< Steps the core in halted debug
#define CDBG_DHCSR_C_HALT       0x00000002 //!< Halts the core.
#define CDBG_DHCSR_C_DEBUGEN	0x00000001 //!< Enables debug
/** @} */

/** @name CDBG_DCRSR: (CDBG Offset: 0x04  Debug Core Register Selector Register        */
/** @{ */
#define CDBG_DCRSR_REGWnR       0x00010000 //!< Write /read
#define CDBG_DCRSR_REGSEL       0x0000001F //!< register(s)
/** @} */

/** @name CDBG_DCRDR: (CDBG Offset: 0x08  Debug Core Register Data Register            */
/** @{ */
/** @} */

/** @name CDBG_DEMCR: (CDBG Offset: 0x0C  Debug Exception and Monitor Control Register */
/** @{ */
#define CDBG_DEMCR_TRCENA         0x01000000 //!< This bit must be set to 1 to enable use of the trace and debug blocks
#define CDBG_DEMCR_MON_REQ        0x00080000 //!< This enables the monitor to identify how it wakes up
#define CDBG_DEMCR_MON_STEP       0x00040000 //!< When MON_EN = 1, this steps the core. When MON_EN = 0, this bit is ignored
#define CDBG_DEMCR_MON_PEND       0x00020000 //!< Pend the monitor to activate when priority permits
#define CDBG_DEMCR_MON_EN         0x00010000 //!< Enable the debug monitor
#define CDBG_DEMCR_VC_HARDERR     0x00000400 //!< Debug trap on Hard Fault
#define CDBG_DEMCR_VC_INTERR      0x00000200 //!< Debug Trap on interrupt/exception service errors
#define CDBG_DEMCR_VC_BUSERR      0x00000100 //!< Debug Trap on normal Bus error
#define CDBG_DEMCR_VC_STATERR     0x00000080 //!< Debug trap on Usage Fault state errors
#define CDBG_DEMCR_VC_CHKERR      0x00000040 //!< Debug trap on Usage Fault enabled checking errors
#define CDBG_DEMCR_VC_NOCPERR     0x00000020 //!< Debug trap on Usage Fault access to Coprocessor that is not present or marked as not present in CAR register
#define CDBG_DEMCR_VC_MMERR       0x00000010 //!< Debug trap on Memory Management faults
#define CDBG_DEMCR_VC_CORERESET   0x00000001 //!< Reset Vector Catch. Halt running system if Core reset occurs
/** @} */



/** @} */ // @relates CDBG_Type



#endif /* CDBG_CM7_H_ */

/** @} */
