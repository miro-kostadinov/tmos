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
// Date:          2015-09-08
// Description:   CPU exceptions processing
//
//==========================================================================

#include "port_asm.h"
#include "core_map.h"


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
    .word			0							// 10 MPU (not used)
    .word			0							// 14 bus (not used)
    .word			0							// 18 usage fault (not used)
    .word			SWI_TABLE					// 1C Reserved	use it to avoid optimization of SWI_TABLE
    .word			DRV_TABLE					// 20 Reserved
    .word			0							// 24 Reserved
    .word			0							// 28 Reserved
    .word			sys_svc_handler				// 2C SVCall handler
    .word			0							// 30 Debug monitor (not used)
    .word			0							// 34 Reserved
    .word			sys_pendsv_handler			// 38 The PendSV handler
	.word			systick_driver	+1

	// DRV_TABLE is placed here

END(	VECTOR_TABLE	)


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
			    ldr     	r0, = BASE_SCB
			    ldr     	r1, = VECTOR_TABLE
			    str     	r1, [r0, # REG_SCB_VTOR - BASE_SCB]

			    /* Switch to process stack */
				movs		r1, #2
				msr			CONTROL, r1

			    /* Switch to main task */
				ldr			r0, =main_task_desc
		        ldmia   	r0!, {r4-r6}			//r4=main_task, r5=main_task_stack, r6=main()

#if USE_TASK_REGISTERS
				mov			REG_MTASK, r4
                mov			REG_CTASK, r4
#else
                str         r4, [r4, #tsk_int_task]
#endif
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
				beq			2f

				ldm			r5!, {r2}
				blx			r2
				b			1b
2:
				pop			{r4, r5, pc}

ENDF(	sys_call_static_ctors	)

				.extern FaultHandler

//*----------------------------------------------------------------------------
//*			NMI handler
//*----------------------------------------------------------------------------
FUNC(	sys_nmi_handler		)

				b 			FaultHandler

ENDF(	sys_nmi_handler		)


//*----------------------------------------------------------------------------
//*			Hard Fault handler
//*----------------------------------------------------------------------------

FUNC(	sys_hard_fault_handler		)

				b			FaultHandler

ENDF(	sys_hard_fault_handler		)



//*----------------------------------------------------------------------------
//*			SVC handler
//*----------------------------------------------------------------------------
FUNC_SECT(	sys_svc_handler, .SWIHandler	)

				mov			r1, lr					// Test EXC_RETURN number in LR bit 2
				lsrs		r1, #3					// if zero (equal) then
				mrs			r0,	MSP					// Main Stack was used, put MSP in R0
				bcc			1f
				mrs			r0,	PSP
1:

				ldr 		r3,[r0, #tsk_sp_pc] 	// Get Task PC
				subs		r3, #2
				ldrB 		r1,[r3] 				// Get parameter at stacked PC minus 2
				lsls		r1, #2
				adr			r2, mos_switab
				ldr			r3, [r2, r1]
				mov			r12, r3
				ldmia		r0, {r0-r3}
				bx			r12
mos_switab:

	// SWI_TABLE is placed here


ENDF(	sys_svc_handler		)


//*----------------------------------------------------------------------------
//*			PendSV handler
//*----------------------------------------------------------------------------
FUNC(	sys_pendsv_handler	)
#if USE_TASK_REGISTERS
				mrs 		r3, PSP 						// Recover PSP into R12
				mov			r2,	REG_CTASK
				stmia		r2!, {r3-r7}					// Push non-stack sp, r4-r7 registers
				mov			r3, r8
				mov			r4, r9
				stmia		r2!, {r3-r4}					// Push non-stack r8,r9 registers
#if TASK_HISTORY_LOG
				mov         r4, REG_CTASK
#endif
				mov			r0, REG_MTASK
				ldr			r2, [r0, #tsk_renext]
				mov			REG_CTASK, r2
#if TASK_HISTORY_LOG
				str         r4, [r2, #tsk_int_task]
#endif
				ldmia		r2!, {r3-r7}					// Restore non-stacked registers
				ldmia		r2!, {r0-r1}
				msr 		PSP, r3 						// Set PSP to R12
				mov			r8, r0
				mov			r9, r1
				bx 			lr 								// Return back to Thread


#else


				mrs 		r3, PSP 						// Recover PSP into R12
				ldr			r1, =main_task
				ldr			r2, [r1, #tsk_int_task]			// r2=old task
				stmia 		r2, {r3-r11}					// Push non-stack registers
				ldr			r0, [r1, #tsk_renext]			// r0=new task
				ldmia		r0, {r3-r11}					// Restore non-stacked registers
#if TASK_HISTORY_LOG
				str			r2, [r0, #tsk_int_task]
#endif
				str			r0, [r1, #tsk_int_task]
				msr 		PSP, r3 						// Set PSP to R12
				bx 			lr 								// Return back to Thread
#endif
ENDF(	sys_pendsv_handler	)

//*----------------------------------------------------------------------------
//*         EABI Unwind
//*----------------------------------------------------------------------------
.text
.global __aeabi_unwind_cpp_pr0
.hidden __aeabi_unwind_cpp_pr0
.type __aeabi_unwind_cpp_pr0, %function

.global __aeabi_unwind_cpp_pr1
.hidden __aeabi_unwind_cpp_pr1
.type __aeabi_unwind_cpp_pr1, %function

.global __aeabi_unwind_cpp_pr2
.hidden __aeabi_unwind_cpp_pr2
.type __aeabi_unwind_cpp_pr2, %function

__aeabi_unwind_cpp_pr0:
__aeabi_unwind_cpp_pr1:
__aeabi_unwind_cpp_pr2:
		 		bx 			lr

.size __aeabi_unwind_cpp_pr0,.-__aeabi_unwind_cpp_pr0
.size __aeabi_unwind_cpp_pr1,.-__aeabi_unwind_cpp_pr1
.size __aeabi_unwind_cpp_pr2,.-__aeabi_unwind_cpp_pr2


.end
