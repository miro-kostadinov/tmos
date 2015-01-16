/*
 * core_map.h
 *
 *  Created on: Sep 08, 2014
 *      Author: miro
 */

#ifndef CORE_MAP_H_
#define CORE_MAP_H_

#include <tmos_types.h>
#include <cdbg_cm0.h>
#include <mpu_cm0.h>
#include <nvic_cm0.h>
#include <scb_cm0.h>
#include <systick_cm0.h>

/*
 *  Cortex-M0 Memory space
 *
 *  0x00000000-0x1FFFFFFF	Code
 *  0x20000000-0x3FFFFFFF	SRAM
 *  0x40000000-0x5FFFFFFF	Peripheral
 *  0x60000000-0x9FFFFFFF	External RAM
 *  0xA0000000-0xDFFFFFFF	External device
 *  0xE0000000-0xE00FFFFF	Private peripheral bus
 *	0xE0100000-0xFFFFFFFF	Vendor Specific
 *
 *
 */

///------------ Address spaces ----------------------------------------------///
#define BASE_CODE		0x00000000 //!< Code space
#define BASE_SRAM		0x20000000 //!< SRAM space
#define BASE_PERIPHERAL	0x40000000 //!< Peripheral space
#define BASE_EXT_RAM	0x60000000 //!< External RAM space
#define BASE_EXT_DEVICE	0xA0000000 //!< External device space
#define BASE_INT_PER	0xE0000000 //!< Internal Peripheral bus space
#define BASE_EXT_PER	0xE0040000 //!< External Peripheral bus space
#define BASE_VENDOR		0xE0100000 //!< Vendor Specific space

///------------ Peripheral space --------------------------------------------///

///------------ Internal Peripheral bus space -------------------------------///
#define BASE_DWT        (BASE_INT_PER +0x001000) //!< 0xE0001000 DWT Base Address
#define BASE_FPB        (BASE_INT_PER +0x002000) //!< 0xE0002000 Breakpoint Unit Base Address
#define BASE_SCS        (BASE_INT_PER +0x00E000) //!< 0xE000E000 System control space

/* System control space */
#define BASE_AUXID	    (BASE_SCS + 0x0000) //!< 0xE000E000 Includes the Auxiliary Control register
#define BASE_SYST	    (BASE_SCS + 0x0010) //!< 0xE000E010 SysTick Base Address
#define BASE_NVIC       (BASE_SCS + 0x0100) //!< 0xE000E100 NVIC Base Address
#define BASE_SCB        (BASE_SCS + 0x0D00) //!< 0xE000ED00 System Control Block Base Address
#define BASE_MPU      	(BASE_SCS + 0x0D90) //!< 0xE000ED90 Memory Protection Unit
#define BASE_CDBG		(BASE_SCS + 0x0DF0) //!< 0xE000EDF0 Core Debug Base Address



///------------ Core peipherals ---------------------------------------------///
#define SYST	CAST(SYST_Type    , BASE_SYST) //!< 0xE000E010 System timer, SysTick (SYST)
#define NVIC	CAST(NVIC_Type    , BASE_NVIC) //!< 0xE000E100 Nested Vectored Interrupt Controller (NVIC)
#define SCB		CAST(SCB_Type  	  , BASE_SCB ) //!< 0xE000ED00 System Control Block (SCB)
#define MPU		CAST(MPU_Type  	  , BASE_MPU ) //!< 0xE000ED90 Memory Protection Unit (MPU)
#define CDBG	CAST(CDBG_Type    , BASE_CDBG) //!< 0xE000EDF0 CoreDebug


///------------ Register definition -----------------------------------------///
/* Nested Vectored Interrupt Controller (NVIC) */
#define REG_NVIC_ISER	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x000) //!< Interrupt Set Enable Register
#define REG_NVIC_ICER	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x080) //!< Interrupt Clear Enable Register
#define REG_NVIC_ISPR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x100) //!< Interrupt Set Pending Register
#define REG_NVIC_ICPR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x180) //!< Interrupt Clear Pending Register
#define REG_NVIC_IABR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x200) //!< Interrupt Active bit Register
#define REG_NVIC_IPR 	REG_ACCESS(__IO  uint8_t, BASE_NVIC+0x300) //!< Interrupt Priority Register (8Bit wide)
#define REG_NVIC_STIR	REG_ACCESS( __O uint32_t, BASE_NVIC+0xE00) //!< Software Trigger Interrupt Register

#define REG_SCB_ACTLR   REG_ACCESS(__IO uint32_t, BASE_AUXID+0x08) //!< 0xE000E008 The Auxiliary Control Register
/* System Control Block (SCB) */
#define REG_SCB_CPUID   REG_ACCESS(__I  uint32_t, BASE_SCB+0x00) //!< 0xE000ED00 CPU ID Base Register
#define REG_SCB_ICSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x04) //!< 0xE000ED04 Interrupt Control State Register
#define REG_SCB_VTOR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x08) //!< 0xE000ED08 Vector Table Offset Register
#define REG_SCB_AIRCR   REG_ACCESS(__IO uint32_t, BASE_SCB+0x0C) //!< 0xE000ED0C Application Interrupt / Reset Control Register
#define REG_SCB_SCR     REG_ACCESS(__IO uint32_t, BASE_SCB+0x10) //!< 0xE000ED10 Optional System Control Register
#define REG_SCB_CCR     REG_ACCESS(__IO uint32_t, BASE_SCB+0x14) //!< 0xE000ED14 Configuration Control Register
#define REG_SCB_SHPRx	REG_ACCESS(__IO  uint8_t, BASE_SCB+0x18) //!< System Handlers Priority Registers (4-7, 8-11, 12-15)
#define REG_SCB_SHPR2	REG_ACCESS(__IO uint32_t, BASE_SCB+0x1C) //!< 0xE000ED1C System Handler Priority Register 2
#define REG_SCB_SHPR3	REG_ACCESS(__IO uint32_t, BASE_SCB+0x20) //!< 0xE000ED20 System Handler Priority Register 3
#define REG_SCB_SHCSR   REG_ACCESS(__IO uint32_t, BASE_SCB+0x24) //!< 0xE000ED24 System Handler Control and State Register
#define REG_SCB_DFSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x30) //!< 0xE000ED30 Debug Fault Status Register


#endif /* CORE_MAP_H_ */
