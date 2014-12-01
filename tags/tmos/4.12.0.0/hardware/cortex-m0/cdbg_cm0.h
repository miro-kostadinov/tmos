/*
 * cdbg_cm0.h
 *
 *  Created on: Sep 9, 2014
 *      Author: miro
 */

#ifndef CDBG_CM0_H_
#define CDBG_CM0_H_

#include <tmos_types.h>

/*******************************************************************************
 *  Core Debug (CDBG)		0xE000EDF0
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
 * @defgroup CDBG_regs_define
 * @{
 ******************************************************************************/

/** @defgroup CDBG_DHCSR: (CDBG Offset: 0x00  Debug Halting Control and Status Register    */
#define CDBG_DHCSR_DBGKEY		0xFFFF0000 //!< Debug Key
#define CDBG_DHCSR_S_RESET_ST   0x02000000 //!< Indicates that the core has been reset, or is now being reset, since the last time this bit was read
#define CDBG_DHCSR_S_RETIRE_ST  0x01000000 //!< Indicates that an instruction has completed since last read
#define CDBG_DHCSR_S_LOCKUP     0x00080000 //!< Reads as one if the core is running (not halted) and a lockup condition is present
#define CDBG_DHCSR_S_SLEEP      0x00040000 //!< Indicates that the core is sleeping (WFI, WFE, or SLEEP-ON-EXIT)
#define CDBG_DHCSR_S_HALT       0x00020000 //!< The core is in debug state when S_HALT is set.
#define CDBG_DHCSR_S_REGRDY     0x00010000 //!< Register Read/Write on the Debug Core Register Selector register is available. Last transfer is complete.
#define CDBG_DHCSR_C_MASKINTS   0x00000008 //!< Mask interrupts when stepping or running in halted debug.
#define CDBG_DHCSR_C_STEP       0x00000004 //!< Steps the core in halted debug
#define CDBG_DHCSR_C_HALT       0x00000002 //!< Halts the core.
#define CDBG_DHCSR_C_DEBUGEN	0x00000001 //!< Enables debug
/** @} */

/** @defgroup CDBG_DCRSR: (CDBG Offset: 0x04  Debug Core Register Selector Register        */
#define CDBG_DCRSR_REGWnR       0x00010000 //!< Write /read
#define CDBG_DCRSR_REGSEL       0x0000001F //!< register(s)
/** @} */

/** @defgroup CDBG_DCRDR: (CDBG Offset: 0x08  Debug Core Register Data Register            */
/** @} */

/** @defgroup CDBG_DEMCR: (CDBG Offset: 0x0C  Debug Exception and Monitor Control Register */
#define CDBG_DEMCR_DWTENA         0x01000000 //!< Global enable for all features configured and controlled by the DWT unit
#define CDBG_DEMCR_VC_HARDERR     0x00000400 //!< Debug trap on Hard Fault
#define CDBG_DEMCR_VC_CORERESET   0x00000001 //!< Reset Vector Catch. Halt running system if Core reset occurs
/** @} */



/** @} */ // @defgroup CDBG_regs_define



#endif /* CDBG_CM0_H_ */
