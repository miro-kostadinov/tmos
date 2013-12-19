/*
 * core_map.h
 *
 *  Created on: Dec 18, 2013
 *      Author: miro
 */

#ifndef CORE_MAP_H_
#define CORE_MAP_H_

#include <tmos_types.h>
#include <cdbg_cm3.h>
#include <itm_cm3.h>
#include <mpu_cm3.h>
#include <nvic_cm3.h>
#include <scb_cm3.h>
#include <systick_cm3.h>
#include <tpiu_cm3.h>

/*
 *  Cortex-M3 Memory space
 *
 *  0x00000000-0x1FFFFFFF	Code
 *  0x20000000-0x3FFFFFFF	SRAM
 *  							0x20000000-0x20100000	bit banding
 *  							...
 *  							0x22000000-0x23FFFFFF	bit band alias
 *  							...
 *  0x40000000-0x5FFFFFFF	Peripheral
 *  							0x40000000-0x40100000	bit banding
 *  							...
 *  							0x42000000-0x43FFFFFF	bit band alias
 *  							...
 *  0x60000000-0x9FFFFFFF	External RAM
 *  0xA0000000-0xDFFFFFFF	External device
 *  0xE0000000-0xE003FFFF	Internal Peripheral bus
 *  0xE0040000-0xE00FFFFF	External Peripheral bus
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
#define BASE_ITM        (BASE_INT_PER +0x000000) //!< 0xE0000000 ITM Base Address
#define BASE_DWT        (BASE_INT_PER +0x001000) //!< 0xE0001000 DWT Base Address
#define BASE_FPB        (BASE_INT_PER +0x002000) //!< 0xE0002000 FPB Base Address
#define BASE_SCS        (BASE_INT_PER +0x00E000) //!< 0xE000E000 System control space

/* System control space */
#define BASE_SYST	    (BASE_SCS + 0x0010) //!< 0xE000E010 SysTick Base Address
#define BASE_NVIC       (BASE_SCS + 0x0100) //!< 0xE000E100 NVIC Base Address
#define BASE_SCB        (BASE_SCS + 0x0D00) //!< 0xE000ED00 System Control Block Base Address
#define BASE_MPU      	(BASE_SCS + 0x0D90) //!< 0xE000ED90 Memory Protection Unit
#define BASE_CDBG		(BASE_SCS + 0x0DF0) //!< 0xE000EDF0 Core Debug Base Address

///------------ External Peripheral bus space -------------------------------///
#define BASE_TPIU       (BASE_EXT_PER +0x000000) //!< 0xE0040000 TPIU Base Address
#define BASE_ETM        (BASE_EXT_PER +0x001000) //!< 0xE0041000 ETM Base Address
#define BASE_EXT_PPB    (BASE_EXT_PER +0x002000) //!< 0xE0042000 External PPB Base Address
#define BASE_ROM_TABLE  (BASE_EXT_PER +0x0BF000) //!< 0xE0FF0000 ROM Table




///------------ Core peipherals ---------------------------------------------///
#define ITM		CAST(ITM_Type     , BASE_ITM ) //!< Instrumentation Trace Macrocell (ITM)
#define SYST	CAST(SYST_Type    , BASE_SYST) //!< System timer, SysTick (SYST)
#define NVIC	CAST(NVIC_Type    , BASE_NVIC) //!< Nested Vectored Interrupt Controller (NVIC)
#define SCB		CAST(SCB_Type  	  , BASE_SCB ) //!< System Control Block (SCB)
#define MPU		CAST(MPU_Type  	  , BASE_MPU ) //!< Memory Protection Unit (MPU)
#define CDBG	CAST(CDBG_Type    , BASE_CDBG) //!< CoreDebug
#define TPIU	CAST(TPIU_Type    , BASE_TPIU) //!< Trace Port Interface Unit (TPIU)


///------------ Register definition -----------------------------------------///
/* Nested Vectored Interrupt Controller (NVIC) */
#define REG_NVIC_ISER	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x000) //!< Interrupt Set Enable Register
#define REG_NVIC_ICER	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x080) //!< Interrupt Clear Enable Register
#define REG_NVIC_ISPR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x100) //!< Interrupt Set Pending Register
#define REG_NVIC_ICPR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x180) //!< Interrupt Clear Pending Register
#define REG_NVIC_IABR	REG_ACCESS(__IO uint32_t, BASE_NVIC+0x200) //!< Interrupt Active bit Register
#define REG_NVIC_IPR 	REG_ACCESS(__IO  uint8_t, BASE_NVIC+0x300) //!< Interrupt Priority Register (8Bit wide)
#define REG_NVIC_STIR	REG_ACCESS( __O uint32_t, BASE_NVIC+0xE00) //!< Software Trigger Interrupt Register

/* System Control Block (SCB) */
#define REG_SCB_CPUID   REG_ACCESS(__I  uint32_t, BASE_SCB+0x00) //!< CPU ID Base Register
#define REG_SCB_ICSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x04) //!< Interrupt Control State Register
#define REG_SCB_VTOR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x08) //!< Vector Table Offset Register
#define REG_SCB_AIRCR   REG_ACCESS(__IO uint32_t, BASE_SCB+0x0C) //!< Application Interrupt / Reset Control Register
#define REG_SCB_SCR     REG_ACCESS(__IO uint32_t, BASE_SCB+0x10) //!< System Control Register
#define REG_SCB_CCR     REG_ACCESS(__IO uint32_t, BASE_SCB+0x14) //!< Configuration Control Register
#define REG_SCB_SHPRx	REG_ACCESS(__IO  uint8_t, BASE_SCB+0x18) //!< System Handlers Priority Registers (4-7, 8-11, 12-15)
#define REG_SCB_SHCSR   REG_ACCESS(__IO uint32_t, BASE_SCB+0x24) //!< System Handler Control and State Register
#define REG_SCB_CFSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x28) //!< Configurable Fault Status Register
#define REG_SCB_HFSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x2C) //!< Hard Fault Status Register
#define REG_SCB_DFSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x30) //!< Debug Fault Status Register
#define REG_SCB_MMFAR   REG_ACCESS(__IO uint32_t, BASE_SCB+0x34) //!< Mem Manage Address Register
#define REG_SCB_BFAR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x38) //!< Bus Fault Address Register
#define REG_SCB_AFSR    REG_ACCESS(__IO uint32_t, BASE_SCB+0x3C) //!< Auxiliary Fault Status Register
#define REG_SCB_PFRx    REG_ACCESS(__I  uint32_t, BASE_SCB+0x40) //!< Processor Feature Register
#define REG_SCB_DFR     REG_ACCESS(__I  uint32_t, BASE_SCB+0x48) //!< Debug Feature Register
#define REG_SCB_ADR     REG_ACCESS(__I  uint32_t, BASE_SCB+0x4C) //!< Auxiliary Feature Register
#define REG_SCB_MMFR 	REG_ACCESS(__I  uint32_t, BASE_SCB+0x50) //!< Memory Model Feature Register
#define REG_SCB_ISARx 	REG_ACCESS(__I  uint32_t, BASE_SCB+0x60) //!< ISA Feature Register


#endif /* CORE_MAP_H_ */
