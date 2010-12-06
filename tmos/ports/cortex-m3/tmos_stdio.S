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
// Date:          2008-05-01
// Description:   TMOS standard functions
//
//========================================================================
#include "port_asm.h"
#include "port_inc.h"

#define PAD_RIGHT 1
#define PAD_ZERO 2

/*
*		Format specifiers:

//	% [flags] [width] [.precision] [{h | l | ll | I | I32 | I64}]type
//  % [ param-no $] flags width [ . precision ] type conversion
//  % [ param-no $] flags width . * [ param-no $] type conversion

	% [flags] [width | *] [.precision | *] type


	flags	 [-]  [0]

‘-’				Left-justify the result in the field (instead of the normal right-justification).
‘0’				Pad the field with zeros instead of spaces. The zeros are placed after any indication of sign or base.
				This flag is ignored if the ‘-’ flag is also specified, or if a precision is specified.

    width		minimum number of characters to be printed.

    *			the value for width or precision is taken from the argument list

    precision

for d, i, u, x	Specifies the minimum number of digits to be printed. The output is padded with 0 on left. Default is 1.
for c			No effect
for s			Specified the maximum numer of characters to be printed.

	type	[d] [i] [u] [x] [X] [c] [s] [n]

‘%d’, ‘%i’		Print an integer as a signed decimal number. ‘%d’ and ‘%i’ are synonymous
‘%u’			Print an integer as an unsigned decimal number.
‘%x’, ‘%X’		Print an integer as an unsigned hexadecimal number. ‘%x’ uses lower-case letters and ‘%X’ uses upper-case.
‘%c’			Print a single character.
‘%s’			Print a string.
‘%n’			Get the number of characters printed so far.




*
*/

#define PF_ZERO 0x40
#define PF_NEG 	0x02
#define PF_LEFT 0x01

//-----------------------------------------------------------------------------
//int tmos_sprintf( char *buffer, const char *format, ... );
// stack usage 32+4 bytes
									// r0 = *buf
									// r1 = *format
FUNC(	tmos_sprintf		)

		push		{r2, r3}		// push all params in stack
		mov			r12, sp			// r12 = va_list
		sub			sp, #8
		push		{r0, r4, r5, r6, r7}


		ldrb		r3, [r1], #1	// r3 = format charachter
		cbz			r3, _end_loop
		cmp			r3, #'%'
		beq			_check_flags

_copy_format_char:
		strb		r3, [r0], #1
_loop:
		ldrb		r3, [r1], #1	// r3 = format charachter
		cbz			r3, _end_loop
		cmp			r3, #'%'
		bne			_copy_format_char

_check_flags:
		ldrb		r3, [r1], #1
		cmp			r3, #'%'
		beq			_copy_format_char

		//--- process flags [-] [0] [n]
		cmp			r3, #'n'
		beq			_flag_n
		movs		r4, #0			// r4 = width
		movs		r6, #0			// r6 = precision
		movs		r7, #0			// r7 = printflags
		movs		r2, #10

		cmp			r3, #'-'
		beq			_flag_left

_check_flag_0:
		cmp			r3, #'0'
		beq			_flag_0

_check_param_width:
		cmp			r3,  #'*'
		beq			_get_param_width

_check_width:
		cmp			r3, #'9'
		bls			_get_width

_check_precision:
		cmp			r3, #'.'
		beq			_get_precision

		//--- process type [d] [i] [u] [x] [X] [c] [s]
_check_type:
		ldr			r5, [r12], #4		//r5=argument
		cmp			r3, #'c'
		beq			_print_c
		cmp			r3, #'s'
		beq			_print_s

		str			r6, [sp, #20]
		add			r6, sp, #20
		cmp			r3, #'d'
		IT			NE
		cmpne		r3, #'i'
		beq			_print_d
		cmp			r3, #'u'
		beq			_print_u
		cmp			r3, #'x'
		IT			NE
		cmpne		r3, #'X'
		beq			_print_hex
_unk_format:
		movs		r3, #'?'
		strb		r3, [r0], #1
		movs		r3, #0
_end_loop:
		strb		r3, [r0]
		pop			{r1, r4, r5, r6, r7}
		add			sp, #8+8
		subs		r0, r0, r1
		bx			lr




_flag_n:
		ldr			r2, [r12], #4
		ldr			r3, [sp]
		subs		r3, r0
		str			r3, [r2]
		b			_loop

_flag_left:
		movs		r7, #PF_LEFT
		ldrb		r3, [r1], #1
		b			_check_flag_0

_flag_0:
		adds		r7, #PF_ZERO
		ldrb		r3, [r1], #1
		b			_check_param_width

_get_param_width:
		ldr			r4, [r12], #4
		ldrb		r3, [r1], #1
		b			_check_precision

_get_width:
		subs		r5, r3, #'0'
		blo			_check_precision
		mla			r4, r4, r2, r5
		ldrb		r3, [r1], #1
		b			_check_width

_get_precision:
		ldrb		r3, [r1], #1
		cmp			r3, #'*'
		beq			_get_param_precision

_get_precision_val:
		cmp			r3, #'9'
		bhi			_check_type
		subs		r3, #'0'
		blo			_unk_format
		mla			r6, r6, r2, r3
		ldrb		r3, [r1], #1
		b			_get_precision_val

_get_param_precision:
		ldr			r6, [r12], #4
		ldrb		r3, [r1], #1
		b			_check_type

_print_c:
		cbz			r5, _prints_left_pad
		uxtb		r3, r5
		add			r5, sp, #20
		str			r3, [r5]

_print_s:
		cbz			r5, _prints_left_pad
		mov			r2, r5
_print_get_len:
		ldrb		r3, [r5], #1
		cmp			r3, #0
		bne			_print_get_len
		subs		r5, r2				//r5 = L
		subs		r5, #1
		beq			_prints_left_pad
		cbz			r6, _prints_pd		//r6 = P
		cmp			r5, r6
		IT			HI
		movhi		r5, r6
		cmp			r4, r6
		IT			HI
		movhi		r4, r6
_prints_pd:
		subs		r4, r5
		bls			_prints_copy
		lsrs		r7, #1
		bcs			_prints_copy
		movs		r3, #' '
_prints_right_pad:
		strb		r3, [r0], #1
		subs		r4, #1
		bne			_prints_right_pad
_prints_copy:
		ldrb		r3, [r2], #1
		strb		r3, [r0], #1
		subs		r5, #1
		bne			_prints_copy
_prints_left_pad:
		subs		r4, #1
		blt			_loop
		movs		r3, #' '
		strb		r3, [r0], #1
		b			_prints_left_pad


_print_hex:
		subs		 r3, #'x'-'a'+10
_print_hex_loop:
		and			r2, r5, #0xF
		cmp			r2, #9
		ITE			HI
		addhi		r2, r2, r3
		addls		r2, #'0'
		strb		r2, [r6, #1]!
		lsrs		r5, r5, #4
		bne			_print_hex_loop
		b			_print_numeric



_print_d:
		cmp			r5, #0
		ITT			MI
		addmi		r7, #PF_NEG
		rsbmi		r5, r5, #0
_print_u:
		udiv		r3, r5, r2
		mls			r5, r2, r3, r5
		adds		r5, #'0'
		strb		r5, [r6, #1]!
		cbz			r3, _print_numeric
		udiv		r5, r3, r2
		mls			r3, r2, r5, r3
		adds		r3, #'0'
		strb		r3, [r6, #1]!
		cmp			r5, #0
		bne			_print_u
_print_numeric:
		add			r5, sp, #20		//r4 = W
		ldrb		r2, [r5]		//r2 = P
		subs		r5, r6, r5		//r5 = L
		cmp			r2, r5
		ITE			HI
		movhi		r3, r2
		movls		r3, r5			//r3 = max(P,L)
		subs		r4, r3
		bls			_print_left_align
		lsrs		r3, r7, #1		//c=FL_LEFT
		bcs			_print_left_align
		//right with W>0
		lsrs		r3, r7, #2		//c=FL_NEG, r3=zero>>2
		bls 		_print_right_pad
		//PF_ZERO && PF_NEG
//_print_right_minus:
		subs		r7, #PF_NEG
		movs		r3, #'-'
		subs		r2, #1
		strb		r3, [r0], #1
		subs		r4, #1
		bls			_print_precision
		lsrs		r3, r7, #2

_print_right_pad:
		adds		r3, #' '
_print_right_pad_loop:
		strb		r3, [r0], #1
		subs		r4, #1
		bne			_print_right_pad_loop
_print_left_align:
		lsrs		r3, r7, #2		//c=FL_NEG
		bcs			_print_left_minus
_print_precision:
		subs		r2, r5
		ble			_print_digits
		movs		r3, #'0'
_print_precision_loop:
		strb		r3, [r0], #1
		subs		r2, #1
		bne			_print_precision_loop
_print_digits:
		ldrb		r3, [r6], #-1
		strb		r3, [r0], #1
		subs		r5, #1
		bne			_print_digits
		movs		r3, #' '
_print_left_pad:
		subs		r4, #1
		blt			_loop
		strb		r3, [r0], #1
		b			_print_left_pad

_print_left_minus:
		movs		r3, #'-'
		subs		r2, #1
		strb		r3, [r0], #1
		subs		r4, #1
		b			_print_precision








ENDF(	tmos_sprintf		)


#if defined(realloc_swi) && defined(malloc_swi)
//-----------------------------------------------------------------------------
FUNC(	cstring_realloc		)

		push		{r1}

		cbz			r0, 8f
		subs		r1, r0, #4
		ldr			r0, [sp, #4]
		subs		r1, r0
		strh		r1, [r0, #2]
		adds		r1, r1

		svc			realloc_swi
		str			r0, [sp, #4]
		cbz			r0, 9f
		subs		r0, #4
		ldrh		r1, [r0]		//size
		add			r8, r0, r1, lsl #2
		ldrh		r1, [r0, #6]	//length
		adds		r1, #8
		adds		r0, r1
		pop			{r1}
		bx			lr

8:
		movs		r0, #16
		svc			malloc_swi
		str			r0, [sp, #4]
		cbz			r0, 9f
		movs		r1, #1
		str			r1, [r0], #4
		add			r8, r0, #12		//end ptr
		pop			{r1}
		bx			lr

9:		//fail
		pop			{r0, r1, r4, r5, r6, r7, r8, lr}
		add			sp, #8+8
		movs		r0, #0
		bx			lr


ENDF(	cstring_realloc		)
//-----------------------------------------------------------------------------
//void * cstring_sprintf( char *buffer, const char *format, ... );
// stack usage 32+8+20+ bytes
									// r0 = *buf
									// r1 = *format
FUNC(	cstring_sprintf		)

		push		{r2, r3}		// push all params in stack
		mov			r12, sp			// r12 = va_list
		sub			sp, #8
		push		{r0, r4, r5, r6, r7, r8, lr}

		cmp			r0, #0
		beq			1f
		subs		r0, #4
		ldrh		r5, [r0]		//r5 = dyn size
		add			r8, r0, r5, lsl #2	//r8 =end
		ldrh		r4, [r0, #6]	//length
		adds		r4, #8
		adds		r0, r4
		cmp			r0, r8
1:
		IT			hs
		blhs		cstring_realloc


		ldrb		r3, [r1], #1	// r3 = format charachter
		cbz			r3, _cs_end_loop
		cmp			r3, #'%'
		beq			_cs_check_flags

_cs_copy_format_char:
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc

_cs_loop:
		ldrb		r3, [r1], #1	// r3 = format charachter
		cbz			r3, _cs_end_loop
		cmp			r3, #'%'
		bne			_cs_copy_format_char

_cs_check_flags:
		ldrb		r3, [r1], #1
		cmp			r3, #'%'
		beq			_cs_copy_format_char

		//--- process flags [-] [0] [n]
		cmp			r3, #'n'
		beq			_cs_flag_n
		movs		r4, #0			// r4 = width
		movs		r6, #0			// r6 = precision
		movs		r7, #0			// r7 = printflags
		movs		r2, #10

		cmp			r3, #'-'
		beq			_cs_flag_left

_cs_check_flag_0:
		cmp			r3, #'0'
		beq			_cs_flag_0

_cs_check_param_width:
		cmp			r3,  #'*'
		beq			_cs_get_param_width

_cs_check_width:
		cmp			r3, #'9'
		bls			_cs_get_width

_cs_check_precision:
		cmp			r3, #'.'
		beq			_cs_get_precision

		//--- process type [d] [i] [u] [x] [X] [c] [s]
_cs_check_type:
		ldr			r5, [r12], #4		//r5=argument
		cmp			r3, #'c'
		beq			_cs_print_c
		cmp			r3, #'s'
		beq			_cs_print_s

		str			r6, [sp, #28]
		add			r6, sp, #28
		cmp			r3, #'d'
		IT			NE
		cmpne		r3, #'i'
		beq			_cs_print_d
		cmp			r3, #'u'
		beq			_cs_print_u
		cmp			r3, #'x'
		IT			NE
		cmpne		r3, #'X'
		beq			_cs_print_hex
_cs_unk_format:
		movs		r3, #'?'
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		movs		r3, #0
_cs_end_loop:
		strb		r3, [r0]
		subs		r1, r0, #4
		pop			{r0, r4, r5, r6, r7, r8, lr}
		subs		r1, r0
		strh		r1, [r0, #2]
		add			sp, #8+8
		bx			lr




_cs_flag_n:
		ldr			r2, [r12], #4
		ldr			r3, [sp]
		subs		r3, r0
		subs		r3, #4
		str			r3, [r2]
		b			_cs_loop

_cs_flag_left:
		movs		r7, #PF_LEFT
		ldrb		r3, [r1], #1
		b			_cs_check_flag_0

_cs_flag_0:
		adds		r7, #PF_ZERO
		ldrb		r3, [r1], #1
		b			_cs_check_param_width

_cs_get_param_width:
		ldr			r4, [r12], #4
		ldrb		r3, [r1], #1
		b			_cs_check_precision

_cs_get_width:
		subs		r5, r3, #'0'
		blo			_cs_check_precision
		mla			r4, r4, r2, r5
		ldrb		r3, [r1], #1
		b			_cs_check_width

_cs_get_precision:
		ldrb		r3, [r1], #1
		cmp			r3, #'*'
		beq			_cs_get_param_precision

_cs_get_precision_val:
		cmp			r3, #'9'
		bhi			_cs_check_type
		subs		r3, #'0'
		blo			_cs_unk_format
		mla			r6, r6, r2, r3
		ldrb		r3, [r1], #1
		b			_cs_get_precision_val

_cs_get_param_precision:
		ldr			r6, [r12], #4
		ldrb		r3, [r1], #1
		b			_cs_check_type

_cs_print_c:
		cbz			r5, _cs_prints_left_pad
		uxtb		r3, r5
		add			r5, sp, #28
		str			r3, [r5]

_cs_print_s:
		cbz			r5, _cs_prints_left_pad
		mov			r2, r5
_cs_print_get_len:
		ldrb		r3, [r5], #1
		cmp			r3, #0
		bne			_cs_print_get_len
		subs		r5, r2				//r5 = L
		subs		r5, #1
		beq			_cs_prints_left_pad
		cbz			r6, _cs_prints_pd		//r6 = P
		cmp			r5, r6
		IT			HI
		movhi		r5, r6
		cmp			r4, r6
		IT			HI
		movhi		r4, r6
_cs_prints_pd:
		subs		r4, r5
		bls			_cs_prints_copy
		lsrs		r7, #1
		bcs			_cs_prints_copy
		movs		r3, #' '
_cs_prints_right_pad:
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r4, #1
		bne			_cs_prints_right_pad
_cs_prints_copy:
		ldrb		r3, [r2], #1
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r5, #1
		bne			_cs_prints_copy
_cs_prints_left_pad:
		subs		r4, #1
		blt			_cs_loop
		movs		r3, #' '
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		b			_cs_prints_left_pad


_cs_print_hex:
		subs		 r3, #'x'-'a'+10
_cs_print_hex_loop:
		and			r2, r5, #0xF
		cmp			r2, #9
		ITE			HI
		addhi		r2, r2, r3
		addls		r2, #'0'
		strb		r2, [r6, #1]!
		lsrs		r5, r5, #4
		bne			_cs_print_hex_loop
		b			_cs_print_numeric



_cs_print_d:
		cmp			r5, #0
		ITT			MI
		addmi		r7, #PF_NEG
		rsbmi		r5, r5, #0
_cs_print_u:
		udiv		r3, r5, r2
		mls			r5, r2, r3, r5
		adds		r5, #'0'
		strb		r5, [r6, #1]!
		cbz			r3, _cs_print_numeric
		udiv		r5, r3, r2
		mls			r3, r2, r5, r3
		adds		r3, #'0'
		strb		r3, [r6, #1]!
		cmp			r5, #0
		bne			_cs_print_u
_cs_print_numeric:
		add			r5, sp, #28		//r4 = W
		ldrb		r2, [r5]		//r2 = P
		subs		r5, r6, r5		//r5 = L
		cmp			r2, r5
		ITE			HI
		movhi		r3, r2
		movls		r3, r5			//r3 = max(P,L)
		subs		r4, r3
		bls			_cs_print_left_align
		lsrs		r3, r7, #1		//c=FL_LEFT
		bcs			_cs_print_left_align
		//right with W>0
		lsrs		r3, r7, #2		//c=FL_NEG, r3=zero>>2
		bls 		_cs_print_right_pad
		//PF_ZERO && PF_NEG
//_print_right_minus:
		subs		r7, #PF_NEG
		movs		r3, #'-'
		subs		r2, #1
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r4, #1
		bls			_cs_print_precision
		lsrs		r3, r7, #2

_cs_print_right_pad:
		adds		r3, #' '
_cs_print_right_pad_loop:
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r4, #1
		bne			_cs_print_right_pad_loop
_cs_print_left_align:
		lsrs		r3, r7, #2		//c=FL_NEG
		bcs			_cs_print_left_minus
_cs_print_precision:
		subs		r2, r5
		ble			_cs_print_digits
		movs		r3, #'0'
_cs_print_precision_loop:
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r2, #1
		bne			_cs_print_precision_loop
_cs_print_digits:
		ldrb		r3, [r6], #-1
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r5, #1
		bne			_cs_print_digits
		movs		r3, #' '
_cs_print_left_pad:
		subs		r4, #1
		blt			_cs_loop
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		b			_cs_print_left_pad

_cs_print_left_minus:
		movs		r3, #'-'
		subs		r2, #1
		strb		r3, [r0], #1
		cmp			r0, r8
		IT			hs
		blhs		cstring_realloc
		subs		r4, #1
		b			_cs_print_precision



ENDF(	cstring_sprintf		)
#endif

//*----------------------------------------------------------------------------
//*			sscanf function
//*  stack usage
//* 	28 bytes
//*	 return:
//*  	number of successfuly scaned format fields
//*	 format:
//*		' ' - any number of spaces
//* 	'%' - [*] [width] [{h | l | ll | I64 | L}]type
//* 			* - field is scanned but not stored
//*				type:
//*					n - store the number of input characters processed to *int
//*
//*----------------------------------------------------------------------------


// int tmos_sscanf(consy char *buf, const char *format, ...)
										// r0 = *buf
										// r1 = *format
FUNC(	tmos_sscanf		)

		push		{r0, r2, r3}		// push all params in stack
		mov			r2, sp				// ++r2 = va_list
		push		{r4, r5, r6, r7}


_sloop:
		ldrb		r3, [r1], #1		// r3 = format charachter
		cmp			r3, #'%'
		beq			_check_sformat
		cmp			r3, #' '
		bhi			_check_schar
_check_spaces:
		cbz			r3, _end_sscanf
		ldrb		r3, [r0]
		cmp			r3, #' '
		bhi			_sloop
		adds		r0, #1
		b			_check_spaces

_check_schar:
		ldrb		r12, [r0], #1
		cmp			r3, r12
		beq			_sloop
_end_sscanf:
_bad_sscanf:
		pop			{r4, r5, r6, r7}
		mov			r3, sp
		subs		r2, r3
		lsrs		r0, r2, #2
		add			sp, #3*4
		bx			lr




_scanf_n:
		ldr			r4, [r2, #4]!
		cbz			r4, _scanf_n_d
		ldr			r3, [sp, #4*4]
		subs		r3, r0
		str			r3, [r4]
_scanf_n_d:
		b			_sloop

_check_sformat:
		ldrb		r3, [r1], #1
		cmp			r3, #'%'
		beq			_check_schar
		movs		r4, #0				//r4 = width
		subs		r7, r3, #'*'
		IT			EQ
		ldrbeq		r3, [r1], #1
		cmp			r3, #'n'
		beq			_scanf_n
		movs		r5, #10
_check_swidth:
		cmp			r3, #'9'
		bls			_get_swidth
_check_stype:

		ldr			r12, [r2, #4]!
		cmp			r3, #'c'
		beq			_sscanf_c
		cmp			r3, #'['
		beq			_sscanf_set

_skip_white:
		ldrb		r6, [r0]
		cbz			r6, _failed_sscanf
		cmp			r6, #' '
		IT			LS
		addls		r0, #1
		bls			_skip_white

		subs		r5, r3, #'u'
		beq			_sscanf_u
		cmp			r3, #'d'
		beq			_sscanf_d
		cmp			r3, #'i'
		beq			_sscanf_i
		subs		r5, r3, #'x'
		beq			_sscanf_x
		cmp			r3, #'s'
		bne			_failed_sscanf

_sscanf_s:
		cmp			r12, #0
		IT			NE
		strbne		r6, [r12], #1
		ldrb		r6, [r0, #1]!
		subs		r4, #1
		IT			NE
		cmpne		r6, #' '
		bhi			_sscanf_s
		movs		r6, #0
		cmp			r12, #0
		IT			NE
		strbne		r6, [r12]
		b			_sloop

_get_swidth:
		subs		r6, r3, #'0'
		blo			_check_stype
		mla			r4, r4, r5, r6
		ldrb		r3, [r1], #1
		b			_check_swidth

_failed_sscanf:
		cbz			r7, _b_bad_sscanf
		subs		r2, #4
_b_bad_sscanf:
		b			_bad_sscanf

_sscanf_c:
		ldrb		r6, [r0], #1
		cmp			r12, #0
		IT			NE
		strbne		r6, [r12], #1
		cmp			r6, #0
		beq			_failed_sscanf
		subs		r4, r4, #1
		bhi			_sscanf_c
		b			_sloop


_sscanf_d:
		cmp			r6, #'-'
		ITT			EQ
		moveq		r5, #'-'
		ldrbeq		r6, [r0, #1]!
_sscanf_u:
		subs		r6, #'0'			// x >= 0
		blo			_failed_sscanf
		cmp			r6, #9				// 9 >= 0
		bhi			_failed_sscanf
_sscanf_u_loop:
		ldrb		r3, [r0, #1]!
_sscanf_u_loop1:
		subs		r4, #1
		beq			_sscanf_u_done
		subs		r3, #'0'			// x >= 0
		ITTT		HS
		rsbshs		r7, r3, #9			// 9 >= 0
		addhs		r6, r6, r6, lsl #2
		addhs		r6, r3, r6, lsl #1
		bhs			_sscanf_u_loop

_sscanf_u_done:
		cmp			r5, #'-'		//if negative
		IT			EQ
		rsbeq		r6, r6, #0
		cmp			r12, #0			//if ptr != null
		IT			NE
		strne		r6, [r12]
		b			_sloop

_sscanf_i:
		cmp			r6, #'-'		//check the sign
		ITT			EQ
		moveq		r5, #'-'
		ldrbeq		r6, [r0, #1]!
		cmp			r6, #'0'
		bne			_sscanf_u		//first digit not '0' goto %u
		movs		r6, #0			//first digit is '0'
		ldrb		r3, [r0, #1]!
		cmp			r3, #'x'
		IT			NE
		cmpne		r3, #'X'
		bne			_sscanf_u_loop1		//second digit != 'x'
		ldrb		r6, [r0, #1]!
_sscanf_x:
		cmp			r6, #'a'
		IT			HS
		subhs		r6, #'a'-'A'		// makeupper
		subs		r6, #'0'			//  x >= 0
		IT			HS
		rsbshs		r3, r6, #15+7		// 22 >= x
		blo			_failed_sscanf
		cmp			r6, #9
		ITT			HI
		subhi		r6, #7
		rsbshi		r3, r6, #10
		bhi			_failed_sscanf		//  x > '9' && x < 'A'

_sscanf_X_loop:
		ldrb		r3, [r0, #1]!
		subs		r4, #1
		beq			_sscanf_u_done
		cmp			r3, #'a'
		IT			HS
		subhs		r3, #'a'-'A'
		subs		r3, r3, #'0'		//  x >= 0
		blo			_sscanf_u_done
		cmp			r3, #15+7
		bhi			_sscanf_u_done

		cmp			r3, #9
		IT			HI
		subhi		r3, #7
		add			r6, r3, r6, lsl #4
		IT			HI
		rsbshi		r3, r3, #10
		bls			_sscanf_X_loop
		lsrs		r6, #4
		b			_sscanf_u_done



_sscanf_set:
		cmp			r7, #0
		IT			NE
		movne		r7, #1
		mov			r3, r1
_sscanf_set_loop:
		mov			r1, r3
		ldrb		r6, [r0], #1
_sscanf_set_next:
		ldrb		r5, [r1], #1
_sscanf_set_next1:
		cmp			r5, #0
		beq			_failed_sscanf
		cmp			r5, #']'
		IT			NE
		cmpne		r6, #0
		beq			_sscanf_set_done
		cmp			r5, #'^'
		beq			_sscanf_set_not
		cmp			r6, r5
		blo			_sscanf_set_next
		beq			_sscanf_set_match

_sscanf_set_set:
		ldrb		r5, [r1], #1
		cmp			r5, #'-'
		bne			_sscanf_set_next1
		ldrb		r5, [r1], #1
		cmp			r6, r5
		bhi			_sscanf_set_next

_sscanf_set_match:
		cmp			r12, #0
		ITT			NE
		addne		r7, r7, #1
		strbne		r6, [r12], #1
		subs		r4, r4, #1
		bne			_sscanf_set_loop
_sscanf_set_done:
		movs		r6, #0
		cmp			r12, #0
		IT			NE
		strbne		r6, [r12]
		cmp			r7, #1
_sscanf_set_cleanup:
		bls			_failed_sscanf
		cmp			r5, #']'
		beq			_sloop
		ldrb		r5, [r1], #1
		cmp			r5, #0
		b			_sscanf_set_cleanup

_sscanf_set_not:
		ldrb		r5, [r1], #1
		cmp			r5, #0
		beq			_failed_sscanf
		cmp			r6, r5
		blo			_sscanf_set_match
		beq			_sscanf_set_not_skip
		ldrb		r5, [r1], #1
		cmp			r5, #0
		beq			_failed_sscanf
		cmp			r5, #'-'
		bne			_sscanf_set_match
		ldrb		r5, [r1], #1
		cmp			r5, #0
		beq			_failed_sscanf
		cmp			r6, r5
		bhi			_sscanf_set_match
		b			_sscanf_set_next

_sscanf_set_not_skip:
		ldrb		r5, [r1], #1
		cmp			r5, #'-'
		bne			_sscanf_set_next1
		ldrb		r5, [r1], #1
		b			_sscanf_set_next1

ENDF(	tmos_sscanf		)


.end

