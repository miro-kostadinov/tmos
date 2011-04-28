//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2010 Informa Ltd.
//
// TMOS is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later version.
//
// TMOS is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
//==========================================================================
//
// Author(s):     Miroslav Kostadinov
// Date:          2008-05-01
// Description:   CPU exceptions processing
//
//==========================================================================

#include "port_asm.h"
#include "port_inc.h"
#include "cmsis_inc.h"


.section .ExceptionVectors,"ax"
				.extern SWI_TABLE
//------------------------------------------------------------------------------
//  Exception Vectors
//  This section can be placed in Flash or RAM, depending on the linker script
//  For bootstrap application it is placed at flash (0x000000)
//------------------------------------------------------------------------------
ENTRY(	VECTOR_TABLE	)

					.extern			systick_driver

	.word			__stack_svc_end				// 00 The system stack pointer
	.word			sys_reset_handler			// 04 -3
	.word			sys_nmi_handler				// 08 -2
	.word			sys_hard_fault_handler		// 0C -1
    .word			sys_mpu_handler				// 10 The MPU fault handler
    .word			sys_bus_fault_handler		// 14 The bus fault handler
    .word			sys_usage_fault_handler		// 18 The usage fault handler
    .word			SWI_TABLE					// 1C Reserved	use it to avoid optimization of SWI_TABLE
    .word			DRV_TABLE					// 20 Reserved
    .word			0							// 24 Reserved
    .word			0							// 28 Reserved
    .word			sys_svc_handler				// 2C SVCall handler
    .word			sys_debug_handler			// 30 Debug monitor handler
    .word			0							// 34 Reserved
    .word			sys_pendsv_handler			// 38 The PendSV handler
	.word			systick_driver	+1

	// DRV_TABLE is placed here

END(	VECTOR_TABLE	)


#define NVIC_CFG_CTRL_STKALIGN  0x00000200

//*----------------------------------------------------------------------------
//*			RESET handler
//*----------------------------------------------------------------------------
FUNC(	sys_reset_handler	)
	// Low Level init
				bl			LowLevelInit

	// memory intit
				/* Copy .relocate section  */
		        ldr     	r1, =_efixed
		        ldr     	r0, =_srelocate
		        ldr     	r2, =_erelocate
		        subs		r2, r0
		        bl			memcpy


				/* Clear .zero section (Zero init) */
                ldr     	r1, =_szero
                ldr     	r2, =_ezero
                movs		r3, #0
2:              stmia   	r1!, {r3}
         		cmp     	r1, r2
                blo     	2b


				/* Fill .tasks section with 0xDEADBEAF (helps to catch stack underrun) */
                ldr     	r1, =_stasks
                ldr     	r2, =_etasks
                ldr			r3, = 0xDEADBEAF
3:              stmia  		r1!, {r3}
         		cmp     	r1, r2
                blo     	3b

	// Core intit

			    /* Set the vector table pointer to the beginning of SRAM. */
			    ldr     	r0, = NVIC_VTABLE
			    ldr     	r1, = VECTOR_TABLE
			    //adr			r1, VECTOR_TABLE
			    str     	r1, [r0]

			    /* Disable STKALIGN */
			    ldr			r0, =NVIC_CFG_CTRL
			    ldr			r1, [r0]
			    bic			r1, #NVIC_CFG_CTRL_STKALIGN
			    str			r1, [r0]

			    /* Switch to process stack */
				movs		r1, #2
				msr			CONTROL, r1

			    /* Switch to main task */
				ldr			r0, =main_task_desc
		        ldmia   	r0!, {r4-r6}			//r4=main_task, r5=main_task_stack, r6=main()

				mov			REG_MTASK, r4
                mov			REG_CTASK, r4
                adds		r5, #0x20
                mov			sp, r5


	// Low level init
                svc			0


      			/* Call main()	*/
      			blx			r6

      			// main should not return, but if it does we have a dead loop here
9:
				wfe
		        b			9b


p_main_task:
				.word 		main_task  //Pointer for PEEDI thread suppport
ENDF(	sys_reset_handler	)

//*----------------------------------------------------------------------------
//*			call Class constructors
//*----------------------------------------------------------------------------
FUNC(	sys_call_static_ctors	)
				push		{r4, r5, lr}
				ldr			r4, =_ctors_start_
				ldr			r5, =_ctors_end_
1:
				cmp			r4,r5

				IT			EQ
				popeq		{r4, r5, pc}

				ldmdb		r5!, {r2}
				blx			r2
				b			1b

ENDF(	sys_call_static_ctors	)

//*----------------------------------------------------------------------------
//*			NMI handler
//*----------------------------------------------------------------------------
FUNC(	sys_nmi_handler		)

				b 			sys_nmi_handler

ENDF(	sys_nmi_handler		)


//*----------------------------------------------------------------------------
//*			Hard Fault handler
//*----------------------------------------------------------------------------
				.extern HardFaultIsr

FUNC(	sys_hard_fault_handler		)

				b			HardFaultIsr

ENDF(	sys_hard_fault_handler		)


//*----------------------------------------------------------------------------
//*			MPU handler
//*----------------------------------------------------------------------------
FUNC(	sys_mpu_handler		)

				b			HardFaultIsr

ENDF(	sys_mpu_handler		)


//*----------------------------------------------------------------------------
//*			BUS Fault handler
//*----------------------------------------------------------------------------
FUNC(	sys_bus_fault_handler	)

				b 			HardFaultIsr

ENDF(	sys_bus_fault_handler	)


//*----------------------------------------------------------------------------
//*			Usage Fault handler
//*----------------------------------------------------------------------------
FUNC(	sys_usage_fault_handler		)

				b 			sys_usage_fault_handler

ENDF(	sys_usage_fault_handler		)


//*----------------------------------------------------------------------------
//*			SVC handler
//*----------------------------------------------------------------------------
FUNC_SECT(	sys_svc_handler, .SWIHandler	)

				tst			lr, #0x04				// Test EXC_RETURN number in LR bit 2
           		ITE         EQ                   	// if zero (equal) then
           		mrseq       r0, MSP              	// Main Stack was used, put MSP in R0
           		mrsne       r0, PSP              	// Process Stack was used

				ldr 		r3,[r0, #tsk_sp_pc] 	// Get Task PC
				ldrB 		r1,[r3, #-2] 			// Get parameter at stacked PC – 2

				adr			r2, mos_switab
				ldr			r12, [r2, r1, lsl #2]
				ldmia		r0, {r0-r3}
				bx			r12

mos_switab:

	// SWI_TABLE is placed here


ENDF(	sys_svc_handler		)


//*----------------------------------------------------------------------------
//*			Debug handler
//*----------------------------------------------------------------------------
FUNC(	sys_debug_handler	)

				b 			sys_debug_handler

ENDF(	sys_debug_handler	)


//*----------------------------------------------------------------------------
//*			PendSV handler
//*----------------------------------------------------------------------------
FUNC(	sys_pendsv_handler	)
				mrs 		r3, PSP 						// Recover PSP into R12
				stmia 		REG_CTASK, {r3-r9}				// Push non-stack registers
				ldr			REG_CTASK, [REG_MTASK, #tsk_renext]
				ldmia		REG_CTASK, {r3-r9}				// Restore non-stacked registers
				msr 		PSP, r3 						// Set PSP to R12
				bx 			lr 								// Return back to Thread

ENDF(	sys_pendsv_handler	)




.end
