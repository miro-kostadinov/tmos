//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2008 Informa Ltd.
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
// Date:          2008-Apr-03
// Description:   SWI table definition
//
//
//========================================================================

#define usr_task_suspend_swi		1
#define SWI_1_FUNCTION				.word		svc_task_suspend		+1

#define usr_send_signal_swi			2
#define SWI_2_FUNCTION              .word		svc_send_signal			+1

#define  usr_task_schedule_swi		3
#define SWI_3_FUNCTION				.word		svc_task_schedule		+1

#define tsk_sleep_swi				4
#define SWI_4_FUNCTION				.word		sys_sleep				+1

#define tsk_sleep_rel_swi			5
#define SWI_5_FUNCTION				.word		sys_sleep_rel			+1

#define tsk_sleep_until_swi			6
#define SWI_6_FUNCTION				.word		sys_sleep_until			+1

#define tsk_try_signal_swi			7
#define SWI_7_FUNCTION              .word		sys_test_signal			+1

#define tsk_get_signal_swi			8
#define SWI_8_FUNCTION              .word		sys_get_signal			+1

#define tsk_wait_signal_swi			9
#define SWI_9_FUNCTION              .word		sys_wait_signal			+1

#define tsk_resume_wait_signal_swi	10
#define SWI_10_FUNCTION             .word		sys_resume_wait_signal	+1

#define usr_drv_hcontrol_swi		11
#define SWI_11_FUNCTION             .word		svc_drv_hcontrol		+1

#define usr_drv_icontrol_swi		12
#define SWI_12_FUNCTION             .word		svc_drv_icontrol		+1

#define tsk_start_handle_swi		13
#define SWI_13_FUNCTION             .word		svc_drv_service			+1

#define usr_HND_SET_STATUS_swi		14
#define SWI_14_FUNCTION             .word		svc_HND_SET_STATUS		+1

#define tsk_start_and_wait_swi			15
#define SWI_15_FUNCTION             .word		sys_drv_service_wait	+1

#define malloc_swi					16
#define SWI_16_FUNCTION             .word		sys_malloc	 			+1

#define free_swi					17
#define SWI_17_FUNCTION             .word		svc_free 				+1

#define realloc_swi					18
#define SWI_18_FUNCTION             .word		sys_realloc				+1


#define SWI_19_FUNCTION
#define SWI_20_FUNCTION
#define SWI_21_FUNCTION
#define SWI_22_FUNCTION
#define SWI_23_FUNCTION
#define SWI_24_FUNCTION
#define SWI_25_FUNCTION
#define SWI_26_FUNCTION
#define SWI_27_FUNCTION
#define SWI_28_FUNCTION
#define SWI_29_FUNCTION
#define SWI_29_FUNCTION

#define CUSTOM_SWI_FUNCTIONS



