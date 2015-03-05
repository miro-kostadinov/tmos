//==============================================================================
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
//==============================================================================
//
// Author(s):     Miroslav Kostadinov
// Date:          2008-05-01
// Description:   TMOS standard functions
//
//==============================================================================
#include "port_asm.h"
#include "swi_tab.h"

#include <brd_cfg.h>

.macro 		m_udiv10 		num_rem, res, tmp

		lsrs		\tmp, \num_rem, #2		// tmp = num/4
        subs		\res, \num_rem, \tmp	// res = 3/4 num
		lsrs		\tmp, \res, #4
        adds		\res, \tmp				// res += res>>4
		lsrs		\tmp, \res, #8
        adds	 	\res, \tmp				// res += res>>8
		lsrs		\tmp, \res, #16
        adds		\res, \tmp				// res += res>>16
        lsrs		\res, #3

		lsls		\tmp, \res, #2			// tmp = res*4
		adds		\tmp, \res				// tmp = res*5
		lsls		\tmp, #1				// tmp = res*10

		subs		\num_rem, \tmp
		cmp			\num_rem, #10
		blo			1f
		subs		\num_rem, #10
		adds		\res, #1
1:
.endm

#ifndef USE_TMOS_LLONG
#define USE_TMOS_LLONG   0
#endif


/*******************************************************************************
    Format specifiers:

        % [parameter] [flags] [width | *] [.precision | *] [length] type

    parameter:
        n$      n is the number of parameter to use. If not specified the next
                parameter from the arguments is used.
                NOTE: This implementation is not exactly the same as the POSIX
                or GNU extensions. The differences are:
                - The standard requires if a placeholder is used once, ALL the
                rest of the placeholders MUST also specify a parameter.
                Here n$ actually seeks the argument stream. For example:
                    printf("%2$*d", dummy, num, width);
                is legal and will print 'num'.
                - The standard requires that there may be no gaps in the numbers
                of arguments specified using '$'.
                Here gaps are allowed as in the above example
                - Special care must be taken for 64-bit arguments. The compiler
                aligns them so a dummy argument must counted if the 64-bit
                argument is passed at even position. The 64-bit arguments are
                also counted twice, so the following is correct and will produce
                the same output (1 2 3):
                    tmos_sprintf(buf, "%lu %lu %u", 1LL, 2LL, 3);       // aligned
                    tmos_sprintf(buf, "%lu %u %lu ", 1LL, 2, 3LL);      // unaligned
                    tmos_sprintf(buf, "%1$lu %3$lu %5$u", 1LL, 2LL, 3); // aligned
                    tmos_sprintf(buf, "%1$lu %3$u %5$lu ",1LL, 2, 3LL); // unaligned


    flags: [-] [0]
        '-'     Left-justify the result in the field  (instead  of the normal
                right-justification).
        '0'     Pad the field with zeros instead of spaces. The zeros are placed
                after any indication of sign or base.
                This flag is ignored if the '-' flag is also specified, or if a
                precision is specified.
    width:      minimum number of characters to be printed.
    *           the value for width or precision is taken from the argument list
                n$ can be used to seek the argument list.

    length:
        'l'     Modifies the size of the argument to 64-bit

    precision:

for d, i, u, x  Specifies the minimum number of digits to be printed. The output
                is padded with 0 on left. Default is 1.
for c           No effect
for s           Specified the maximum numer of characters to be printed.

    type: [d] [i] [u] [x] [X] [c] [s] [n]

    '%d', '%i'  Print an integer as a signed decimal number. '%d' and '%i' are
                synonymous
    '%u'        Print an integer as an unsigned decimal number.
    '%x', '%X'  Print an integer as an unsigned hexadecimal number. '%x' uses
                lower-case letters and '%X' uses upper-case.
    '%c'        Print a single character.
    '%s'        Print a string.
    '%n'        Get the number of characters printed so far.


*******************************************************************************/

#define PF_ZERO 0x40
#define PF_NEG  0x02
#define PF_LEFT 0x01
#define PF_LONG 0x04

//-----------------------------------------------------------------------------
//int tmos_sprintf( char *buffer, const char *format, ... );
// stack usage 32+12 bytes
                                    // r0 = *buf
                                    // r1 = *format
FUNC(   tmos_sprintf        )

        push        {r2, r3}        // push all params in stack
        mov         r2, sp          // r2 = va_list
        push        {lr}
        bl          tmos_vsprintf
        pop         {r1}
        add         sp, #8
        bx          r1
ENDF(   tmos_sprintf        )

//-----------------------------------------------------------------------------
// int tmos_vsprintf( char *buffer, const char *format, va_list arg );
// stack usage 32 bytes

#if USE_TMOS_LLONG
#define VSPRINTF_BUF_SIZE 24
#else
#define VSPRINTF_BUF_SIZE 12
#endif

FUNC(   tmos_vsprintf        )               // r0  r1  r2  r3  r4  r5  r6  r7  r12     0
        push        {r0, r4, r5, r6, r7}	 // dst fmt arg ___ ___ ___ ___ ___ ___     [dst0 xxxxxx buf12-24
        sub         sp, #VSPRINTF_BUF_SIZE
        mov         r12, r2                  // dst fmt ___ ___ ___ ___ ___ ___ ar0
_loop:
        ldrb        r3, [r1]	             // dst fmt arg  c  ___ ___ ___ ___ ar0
        adds		r1, #1
        cmp         r3, #0
        beq         _end_loop
        cmp         r3, #'%'
        beq         _check_flags

_copy_format_char:                           // dst fmt arg  c  ___ ___ ___ ___ ar0
        strb        r3, [r0]
        adds		r0, #1
        ldrb        r3, [r1]
        adds		r1, #1
        cmp			r3, #0
        beq         _end_loop
        cmp         r3, #'%'
        bne         _copy_format_char

_check_flags:                                // dst fmt arg '%' ___ ___ ___ ___ ar0
        ldrb        r3, [r1]
        adds		r1, #1
        cmp         r3, #'%'
        beq         _copy_format_char



        //--- process flags [-] [0] [n]      // dst fmt arg  c  ___ ___ ___ ___ ar0
        cmp         r3, #'-'
        beq			_get_flag_left
        movs	    r7, #0                   // dst fmt arg  c  ___ ___ ___ flg ar0

_check_flag_zero:
        cmp         r3, #'0'
        beq			_get_flag_zero

_check_width:
        movs        r4, #0                   // dst fmt arg  c  wdt ___ ___ flg ar0
        movs        r6, #0                   // dst fmt arg  c  wdt ___ pre flg ar0
        cmp         r3, #'9'
        bhi			_check_type

_get_width:									 // dst fmt arg  c  wdt ___ pre flg ar0
        //--- (<=9) process [*] [*n$]
        movs        r5, r3
        subs        r5, #'0'
        blo         _check_param_width

_get_width_loop:
		lsls		r3, r4, #3
		lsls		r4, #1
		adds		r4, r3
		adds		r4, r5
        ldrb        r3, [r1]	             // dst fmt arg  c  wdt ___ pre flg ar0 arg*
        adds		r1, #1
        cmp         r3, #'9'
        bhi         _check_type
        movs        r5, r3
        subs        r5, #'0'
        bhs         _get_width_loop
        cmp         r3, #'$'
        bne         _check_precision
        cmp         r7, #0
        bne         _check_precision
        subs        r4, #1
        blo         _unk_format
        lsls		r2, r4, #2
        add         r2, r12
        b           _check_flags

_check_param_width: 	// (<0)
        cmp         r3,  #'*'
        bne			_check_precision
        ldrb        r3, [r1]
        adds		r1, #1
        cmp			r3, #'9'
        bhi			_get_param_width
        movs        r5, r3
        subs        r5, #'0'
        blo         _get_param_width
_get_param_width_loop:
		lsls		r3, r4, #3
		lsls		r4, #1
		adds		r4, r3
		adds		r4, r5
        ldrb        r3, [r1]                 // dst fmt arg  c  wdt ___ pre flg ar0
        adds		r1, #1
        cmp         r3, #'9'
        bhi         _unk_format
        movs        r5, r3
        subs        r5, #'0'
        bhs         _get_param_width_loop
        cmp         r3, #'$'
        bne         _unk_format
        subs        r4, #1
        blo         _unk_format
        lsls		r5, r4, #2
        add         r5, r12
        ldr         r4, [r5]
        ldrb        r3, [r1]
        adds		r1, #1
        b           _check_precision

_get_flag_left:
        movs		r7, #PF_LEFT
        ldrb		r3, [r1]
        adds		r1, #1
        b 			_check_flag_zero

_get_flag_zero:
        adds		r7, #PF_ZERO
        ldrb		r3, [r1]				 // dst fmt arg  c   0  ___ pre flg
        adds		r1, #1
        b			_check_width

_get_param_precision:                        // dst fmt arg  c  wdt ___ pre flg ar0
        ldmia       r2!, {r6}

        //--- process type [d] [i] [u] [x] [X] [c] [s]
_check_type:
        ldmia       r2!, {r5}        	     // dst fmt arg  c  wdt arg pre flg ar0
        cmp         r3, #'c'
        beq         _print_c
        cmp         r3, #'s'
        beq         _print_s
        cmp         r3, #'n'
        beq         _flag_n
#if USE_TMOS_LLONG
        cmp         r3, #'l'
        bne			_check_long_done
        adds        r7, #PF_LONG
		mov			r3, r12
        eors		r3, r2
        lsrs		r3, #3
        bcs			1f
        ldmia       r2!, {r5}	  // for stack alignment
1:
        ldrb	    r3, [r1]
        adds		r1, #1
_check_long_done:
#endif
        push		{r4, r6}
        add         r6, sp, #8            	 // dst fmt arg  c  wdt arg buf flg ar0    wdt pre flg
        cmp         r3, #'d'
        beq         _print_d
        cmp		    r3, #'i'
        beq         _print_d
        cmp         r3, #'u'
        beq         _print_u
        b			_check_hex

_unk_format:
        movs        r3, #'?'
        strb        r3, [r0]
        adds		r0, #1
        movs        r3, #0
_end_loop:
        strb        r3, [r0]
        add         sp, #VSPRINTF_BUF_SIZE
        pop         {r1, r4, r5, r6, r7}
        subs        r0, r1
        bx          lr



_get_param_width:
        ldmia       r2!, {r4}
_check_precision:                            // dst fmt arg  c  wdt ___ pre flg ar0
        cmp         r3, #'.'
        bne			_check_type
//get_precision:  							 // r0  r1  r2  r3  r4  r5  r6  r7  r12
        ldrb        r3, [r1]
        adds		r1, #1
        cmp         r3, #'*'
        bne         _get_precision_val

        ldrb        r3, [r1]
        adds		r1, #1
        cmp			r3, #'9'
        bhi			_get_param_precision
        movs        r5, r3
        subs        r5, #'0'
        blo			_get_param_precision
_get_param_precision_loop:
		lsls		r3, r6, #3
		lsls		r6, #1
		adds		r6, r3
		adds		r6, r5
        ldrb        r3, [r1]
        adds		r1, #1
        cmp         r3, #'9'
        bhi         _unk_format
        movs        r5, r3
        subs        r5, #'0'
        bhs         _get_param_precision_loop
        cmp         r3, #'$'
        bne         _unk_format
        subs        r6, #1
        blo         _unk_format
        lsls		r5, r6, #2
        add         r5, r12
        ldr         r6, [r5]
        ldrb        r3, [r1]
        adds		r1, #1
        b           _check_type


_get_precision_val:
        cmp         r3, #'9'
        bhi         _check_type
        subs        r3, #'0'
        blo         _unk_format
		lsls		r5, r6, #3
		lsls		r6, #1
		adds		r6, r3
		adds		r6, r5
        ldrb        r3, [r1]
        adds		r1, #1
        b           _get_precision_val

_flag_n:
        ldr         r3, [sp, #VSPRINTF_BUF_SIZE]
        subs        r3, r0
        str         r3, [r5]
        b           _loop



_print_c:                                    // dst fmt arg 'c' wdt arg pre flg ar0
        cmp         r5, #0
        beq			_prints_left_pad
        uxtb        r3, r5
        add         r5, sp, #4
        str         r3, [r5]

_print_s:                                    // dst fmt arg 's' wdt arg pre flg ar0
        cmp         r5, #0
        beq			_prints_left_pad
		adds		r3, r6, r4
		bne			_print_s_get_len
		subs		r5, r0
1:
        ldrb        r3, [r0, r5]
        cmp			r3, #0
        beq			_j_loop
        strb        r3, [r0]
        adds		r0, #1
        b			1b

_print_s_get_len:
2:		// get length/precision
        ldrb        r3, [r5]
        cmp			r3, #0
		beq			_prints_left_pad
		str			r7, [sp]
		movs		r7, #1
3:
        ldrb        r3, [r5, r7]
        cmp			r3, #0
		beq			4f
        adds		r7, #1
		cmp			r7, r6
        bne			3b
4:
        subs        r4, r7                   // dst fmt arg ___ w-l str ___ len ar0
        bls         _prints_copy
		ldr			r3, [sp]
        lsrs        r3, #1
        bcs         _prints_copy
        movs        r3, #' '
_prints_right_pad:                           // dst fmt str ' ' w-l len ___ ___ ar0 arg*
        strb        r3, [r0]
        adds		r0, #1
        subs        r4, #1
        bne         _prints_right_pad
_prints_copy:                                // dst fmt str ' ' 0   len ___ ___ ar0 arg*
		subs		r5, r0
1:
        ldrb        r3, [r5, r0]
        strb        r3, [r0]
        adds		r0, #1
        subs        r7, #1
        bne         1b
_prints_left_pad:                            // dst fmt ___ ___ w-l ___ ___ ___ ar0 arg*
        subs        r4, #1
        bge         2f
_j_loop:
        b 	        _loop
2:
        movs        r3, #' '
        strb        r3, [r0]
        adds		r0, #1
        b           _prints_left_pad




#if USE_TMOS_LLONG
_print_u:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        lsrs        r3, r7, #3
        bcc			_print_u32
        ldmia		r2!, {r3}
        b          _print_u64

_print_d:                                    // dst fmt arg ___ wdt?arg buf?flg?ar0 	wdt pre
        lsrs        r3, r7, #3
        bcc         _print_d32
        ldmia		r2!, {r3}
        cmp         r3, #0
        bge         _print_u64
        lsls		r1, r3, #1
        negs        r5, r5                      // n < 0
        sbcs        r3, r1
        adds        r7, #PF_NEG

_print_u64:                                  // dst fmt arg  hi ___ low buf flg ar0 	wdt pre
        subs        r7, #PF_LONG
        cmp         r3, #0
        beq			_print_u32
        push        {r0, r1}                 // r0  r1  r2  r3  r4  r5  r6  r7
        lsrs        r1, r5, #16              // n0  n1  arg hi  ___ ___ buf flg
        uxth        r5, r5

_print_top_u64:
		m_udiv10 	r3, r0, r4	//num_rem, res, tmp
		lsls		r3, #16
		adds		r3, r1
		m_udiv10 	r3, r1, r4	//num_rem, res, tmp
		lsls		r3, #16
		adds		r3, r5
		m_udiv10 	r3, r5, r4	//num_rem, res, tmp
        adds        r3, #'0'
        adds		r6, #1
        strb        r3, [r6]
        movs        r3, r0
        bne         _print_top_u64
        lsls		r1, #16
        adds        r5, r0
        pop         {r0, r1}
        b           _print_u32

_print_d32:
        cmp         r5, #0
        bge          _print_u32
        adds        r7, #PF_NEG
        rsbs        r5, r5, #0

_print_u32:

#else
_print_d:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        cmp         r5, #0
        bge          _print_u
        adds        r7, #PF_NEG
        rsbs        r5, r5, #0
_print_u:                                    // dst fmt arg ___ ___ arg buf flg ar0    pre
#endif
		m_udiv10 	r5, r3, r4	//num_rem, res, tmp
        adds        r5, #'0'
        adds		r6, #1
        strb        r5, [r6]
        movs		r5, r3
        bne			_print_u
_print_numeric:                              // dst fmt arg ___ ___ ___ buf flg ar0    wdt pre
		add			r3, sp, #8
        subs        r5, r6, r3
		pop			{r4, r6}				 // dst fmt arg ___ wdt len pre flg ar0
		movs		r3, r6
        cmp         r3, r5
        bhi			1f
        movs		r3, r5
1:
        subs        r4, r3                   // dst fmt arg max w-m len pre flg ar0
        bls         _print_left_align
        lsrs        r3, r7, #1        //c=FL_LEFT
        bcs         _print_left_align
        //right with W>0
        lsrs        r3, r7, #2        //c=FL_NEG, r3=zero>>2
        bls         _print_right_pad
        //PF_ZERO && PF_NEG
//_print_right_minus:
        subs        r7, #PF_NEG
        movs        r3, #'-'
        strb        r3, [r0]
        adds		r0, #1
        subs        r6, #1
        subs        r4, #1
        bls         _print_precision
        lsrs        r3, r7, #2

_print_right_pad:
        adds        r3, #' '
1:
        strb        r3, [r0]
        adds		r0, #1
        subs        r4, #1
        bne         1b
_print_left_align:
        lsrs        r3, r7, #2        //c=FL_NEG
        bcc         _print_precision
//_print_left_minus:                         // dst fmt arg ___ wdt len pre ___ ar0 *
        movs        r3, #'-'
        subs        r6, #1
        strb        r3, [r0]
        adds		r0, #1
        subs        r4, #1
_print_precision:                            // dst fmt arg ___ wdt len pre __g ar0
        subs        r6, r5
        ble         _print_digits
        movs        r3, #'0'
        adds		r0, r6
_print_precision_loop:                       // dst fmt arg ___ wdt len pre ___ ar0 arg*
        subs        r6, #1
        strb        r3, [r0, r6]
        bne         _print_precision_loop
_print_digits:                               // dst fmt arg ___ wdt len ___ ___ ar0
        add         r6, sp, #0
1:
        ldrb        r3, [r6, r5]
        strb        r3, [r0]
        adds		r0, #1
        subs        r5, #1
        bne         1b
		b		    _prints_left_pad  	     // dst fmt arg ___ w-l ___ ___ ___ ar0

_check_hex:
        cmp         r3, #'p'
        beq         _print_ptr
        cmp         r3, #'x'
        beq         _print_hex
        cmp         r3, #'X'
        beq         _print_hex
        add         sp, #8
		b 			_unk_format


_print_ptr:
		movs		r3, #'X'
                                             // r0  r1  r2  r3  r4  r5  r6  r7  r12
_print_hex:                                  // dst fmt arg 'x' ___ arg buf flg ar0    wdt pre
        subs        r3, #'x'-'a'-'0'+10
_print_hex_loop:
		lsls		r4, r5, #28
		lsrs		r4, #28
        cmp         r4, #9
        bls			1f
        adds		r4, r3
1:
		adds		r4, #'0'
		adds		r6, #1
		strb		r4, [r6]
        lsrs        r5, #4
        bne         _print_hex_loop

#if USE_TMOS_LLONG
        lsrs        r5, r7, #3
        bcc         _print_numeric
        subs        r7, #PF_LONG
        ldmia       r2!, {r5} 				 // dst fmt arg 'x' ___ arg buf flg ar0    wdt pre
        cmp         r5, #0
        beq         _print_numeric
1:
        add			r4, sp, #8+8
		cmp			r6, r4
        bhs			_print_hex_loop
        movs        r4, #'0'
		adds		r6, #1
		strb		r4, [r6]
		b			1b
#else
        b           _print_numeric           // dst fmt ___ ___ wdt  0  buf flg ar0 arg*   pre
#endif



ENDF(   tmos_vsprintf        )








//-----------------------------------------------------------------------------
// int tmos_vprintf_len( const char *format, va_list arg );
// stack usage 32 bytes
FUNC(   tmos_vprintf_len        )   // r0  r1  r2  r3  r4  r5  r6  r7  r12
                                    // fmt arg

        push        {r4, r5, r6, r7}
        mov         r2, r0          // fmt arg fmt
        movs		r7, #4
        ands		r7, r1
        movs        r0, #0          // res arg fmt

_vpl_loop:
        ldrb        r3, [r2]	    // res arg fmt c
        adds		r2, #1
        adds        r0, #1
        cmp         r3, #0
        beq			_vpl__end_loop
        cmp         r3, #'%'
        bne         _vpl_loop

        ldrb        r3, [r2]	    // r3 = format charachter
        adds		r2, #1
        cmp         r3, #'%'
        beq         _vpl_loop
        subs        r0, #1

        //--- process flags [-] [0] [n]
        cmp         r3, #'n'
        beq         _vpl_loop
        movs        r4, #0          // res arg fmt c wdt
        movs        r6, #0          // res arg fmt c wdt ___ pre

        cmp         r3, #'-'
        bne			2f
1:
        ldrb        r3, [r2]
        adds		r2, #1
2:
        cmp         r3, #'0'
        beq			1b

        cmp         r3,  #'*'
        beq         _vpl_get_param_width

_vpl_check_width:
        cmp         r3, #'9'
        bls         _vpl_get_width

_vpl_check_precision:
        cmp         r3, #'.'
        beq         _vpl_get_precision

        //--- process type [d] [i] [u] [x] [X] [c] [s]
_vpl_check_type:
		ldmia		r1!, {r5}	    // res arg fmt c   wdt ___ pre
        cmp         r3, #'c'		// r0  r1  r2  r3  r4  r5  r6  r7  r12
        beq         _vpl_print_c
        cmp         r3, #'s'
        beq         _vpl_print_s
        movs        r5, #11
#if USE_TMOS_LLONG
        cmp         r3, #'l'
        bne			_vpl_skip_long
        movs		r3, r7
        eors		r3, r1
        lsrs		r3, #3
        bcs			1f
        adds	    r1, #4 // for stack alignment
1:
        adds       	r1, #4
        adds       	r5, #10
		adds		r2, #1
_vpl_skip_long:
#endif
        cmp         r5, r6
        bhs         _vpl_add_len
        mov         r5, r6                // r5 = max(12, precision)
        b           _vpl_add_len

_vpl_unk_format:
        adds        r0, #1
_vpl__end_loop:
        pop         {r4, r5, r6, r7}
        bx          lr


_vpl_get_param_width:
		ldmia		r1!, {r4}
        ldrb        r3, [r2]
        adds		r2, #1
        b           _vpl_check_precision

_vpl_get_width:
        movs        r5, r3
        subs        r5, #'0'
        blo         _vpl_check_precision

		lsls		r3, r4, #3
		lsls		r4, #1
		adds		r4, r3
		adds		r4, r5
        ldrb        r3, [r2]
        adds		r2, #1
        b           _vpl_check_width

_vpl_get_precision:
        ldrb        r3, [r2]
        adds		r2, #1
        cmp         r3, #'*'
        beq         _vpl_get_param_precision

_vpl_get_precision_val:
        cmp         r3, #'9'
        bhi         _vpl_check_type
        subs        r3, #'0'
        blo         _vpl_unk_format
        lsls		r5, r6, #3
        lsls		r6, #1
        adds		r6, r5
        adds		r6, r3
        ldrb        r3, [r2]
        adds		r2, #1
        b           _vpl_get_precision_val

_vpl_get_param_precision:
		ldmia		r1!, {r6}
        ldrb        r3, [r2]
        adds		r2, #1
        b           _vpl_check_type

_vpl_print_c:
        movs        r5, #1
        b           _vpl_add_len

_vpl_print_s:
		cmp			r5, #0
		beq			_vpl_add_width    // "%s",NULL
        mov         r12, r5
_vpl_print_get_len:
        ldrb        r3, [r5]
        cmp         r3, #0
		beq			1f
        adds		r5, #1
        subs		r6, #1
        bne         _vpl_print_get_len
1:
        mov			r3, r12
        subs		r5, r3		    //r5 = len = min(len, precsion)
_vpl_add_len:
        cmp         r4, r5
        bhi			_vpl_add_width
        movs	    r4, r5                //r4 = max(width, len)
_vpl_add_width:
        add         r0, r4
        b           _vpl_loop

ENDF(   tmos_vprintf_len        )


//*----------------------------------------------------------------------------
//*         sscanf function
//*  stack usage
//*     28 bytes
//*  return:
//*     number of successfuly scaned format fields
//*  format:
//*     ' ' - any number of spaces
//*     '%' - [*] [width] type
//*             * - field is scanned but not stored
//*  type:
//*     n - Store the number of input characters processed to *int
//*     d - Matches an optionally signed decimal integer;
//*     i - Matches an optionally signed integer
//*     u - Matches an unsigned decimal integer
//*     x - Matches an unsigned hexadecimal integer
//*     s - Matches a sequence of non-white-space characters. The input string
//*         stops at space or at the maximum field width, whichever occurs first
//*     c - Matches a sequence of characters whose length is specified by the
//*         maximum field width (default 1).
//*     [ - Matches a non-empty sequence of characters. The sequence is defined
//*         by the codes between the brackets [].  Codes can include:
//*             x   - single char (other than ']', '^', '-')
//*             x-y - range
//*             ^x  - any char but x
//*             ^x-y - any char not in this range
//*
//*----------------------------------------------------------------------------


// int tmos_sscanf(consy char *buf, const char *format, ...)
                                        // r0 = *buf
                                        // r1 = *format
FUNC(    tmos_sscanf        )				 // r0  r1  r2  r3  r4  r5  r6  r7  r12
                                       		 // src fmt arg arg
        cmp			r0, #0
        beq			_sexit
        push        {r0, r2, r3}        // push all params in stack
        add         r2, sp, #4          // ++r2 = va_list
        push        {r4, r5, r6, r7}


_sloop:
        ldrb        r3, [r1]			  	 // src fmt arg fc
        adds		r1, #1
        cmp         r3, #'%'
        beq         _check_sformat
        cmp         r3, #' '
        bls         _check_spaces

_check_schar:
        ldrb        r4, [r0]
        adds		r0, #1
        cmp         r3, r4
        beq         _sloop
_end_sscanf:
_bad_sscanf:
        pop         {r4, r5, r6, r7}
        add         r3, sp, #4
        subs        r2, r3
        lsrs        r0, r2, #2
        add         sp, #3*4
_sexit:
        bx          lr


_check_spaces:
        cmp         r3, #0
        beq			_end_sscanf
        ldrb        r3, [r0]
        cmp         r3, #' '
        bhi         _sloop
        adds        r0, #1
        b           _check_spaces



_scanf_n:
		cmp			r7, #0
		beq			_sloop
        ldr         r3, [sp, #4*4]
        subs        r3, r0, r3
        str         r3, [r7]
        b           _sloop

_check_sformat:								 // r0  r1  r2  r3  r4  r5  r6  r7  r12
        ldrb        r3, [r1]				 // src fmt arg fc
        adds		r1, #1
        cmp         r3, #'%'
        beq         _check_schar
        movs        r4, #0             		 // src fmt arg fc  wdt
        cmp			r3, #'*'
        beq			_get_sparam
        ldmia		r2!, {r7}				 // src fmt arg fc  wdt         prm
_check_sn:
        cmp         r3, #'n'
        beq         _scanf_n
_check_swidth:
        cmp         r3, #'9'
        bhi         _check_stype
        movs		r6, r3
        subs		r6, #'0'
        blo         _check_stype
        lsls		r5, r4, #3
        lsls		r4, #1
        adds		r4, r5
        adds		r4, r6
        ldrb        r3, [r1]
        adds		r1, #1
        b           _check_swidth

_check_stype:
        cmp         r3, #'c'
        beq         _sscanf_c
        cmp         r3, #'['
        beq         _j_sscanf_set

_skip_white:
        ldrb        r6, [r0]			// src fmt arg f   wdt       c  prm  10
        cmp         r6, #0
        beq			_failed_sscanf
        adds		r0, #1
        cmp         r6, #' '
        bls         _skip_white
        subs        r0, #1

        movs        r5, #0				// src fmt arg f   wdt flg c  prm  10
#if USE_TMOS_LLONG						// r0  r1  r2  r3  r4  r5  r6  r7  r12
        cmp         r3, #'l'
        bne			1f
        adds		r5, #PF_LONG
        ldrb        r3, [r1]
        adds		r1, #1
#endif
        cmp         r3, #'u'
        beq         _sscanf_u
        cmp         r3, #'d'
        beq         _sscanf_d
        cmp         r3, #'i'
        beq         _sscanf_i
        cmp         r3, #'x'
        beq         _sscanf_x
        cmp         r3, #'s'
        bne         _failed_sscanf

_sscanf_s:								// src fmt arg f   wdt flg c  prm  10
		adds		r0, #1
        cmp         r7, #0
        beq			1f
        strb	    r6, [r7]
        adds		r7, #1
1:
        subs        r4, #1
        beq			2f
        ldrb        r6, [r0]
        cmp         r6, #' '
        bhi         _sscanf_s
        movs        r4, #0
2:
        cmp         r7, #0
        beq         _sloop
        strb        r4, [r7]
        b           _sloop

_j_sscanf_set:
        b  			_sscanf_set


_get_sparam:
        ldrb		r3, [r1]
        adds		r1, #1
        movs		r7, #0
		b        	_check_sn


_sscanf_c:								// src fmt arg f   wdt 10      prm  10
        ldrb        r6, [r0]
        adds		r0, #1
        cmp         r7, #0
        beq			1f
        strb	    r6, [r7]
        adds		r7, #1
1:
        cmp         r6, #0
        beq         _failed_sscanf
        subs        r4, #1
        bhi         _sscanf_c
        b           _sloop



_sscanf_d:                              	 // r0  r1  r2  r3  r4  r5  r6  r7  r12
        cmp         r6, #'+'            	 // src fmt arg ___ wdt flg  c  par
        bne			1f
        adds		r0, #1
        ldrb        r6, [r0]
1:
        cmp         r6, #'-'            	 // src fmt arg ___ wdt flg  c  par
        bne			_sscanf_u
        adds        r5, #PF_NEG
        adds		r0, #1
        ldrb        r6, [r0]
_sscanf_u:
        subs        r6, #'0'            // x >= 0
        blo         _failed_sscanf
        cmp         r6, #9              // 9 >= 0
        bhi         _failed_sscanf
_sscanf_u_loop1:                        	 // src fmt arg ___ wdt flg res par
		push		{r1, r2}
		movs		r1, #0
_sscanf_u_loop:                         	 // src hi  ___ _c_ wdt flg res par
        adds		r0, #1
        subs        r4, #1
        beq         _sscanf_u_done
        ldrb        r3, [r0]
        subs        r3, #'0'            // x >= 0
        blo         _sscanf_u_done
        cmp         r3, #9
        bhi         _sscanf_u_done
#if USE_TMOS_LLONG

        lsls		r2, r1, #2		//  r1		  		r6
        adds		r1, r2			//  r1*5			r6

        lsrs		r2, r6, #30
        adds		r1, r2			// 	r1*5+[r6*4]		r6

        lsls		r2, r6, #2
        adds		r6, r2			// 	r1*5+[r6*4]		r6*5
        adcs		r1, r1			//  r1*10+[r6*5]	r6*5

        movs		r2, #0
        adds		r6, r6			//  r1*10+[r6*5]	r6*10
        adcs		r1, r2			//  r1*10+[r6*10]	r6*10

        adds		r6, r3
        adcs		r1, r2

        b           _sscanf_u_loop
#else
        lsls		r6, #1			// 2x
        adds		r3, r6			// 2x + c
        lsls		r6, #2			// 8x
        adds    	r6, r3			// 10x + c
        b           _sscanf_u_loop
#endif

_sscanf_u_done:								 // src hi  ___ ___ ___ flg lo  par
		mov			r4, r1
		pop			{r1, r2}				 // src fmt arg ___ hi  flg lo  par
        cmp         r7, #0          //if ptr != null
        beq         _j_sloop
        lsrs        r5, #2        //if negative
#if USE_TMOS_LLONG
        bcc         1f
        lsls		r3, r4, #1
        negs        r6, r6                      // n < 0
        sbcs        r4, r3
1:
        cmp         r5, #0
        beq			2f			  //if !PF_LONG
        str         r4, [r7, #4]
2:
#else
        bcc         3f
        negs        r6, r6
3:
#endif
        str         r6, [r7]
_j_sloop:
        b           _sloop

_failed_sscanf:
        cmp         r7, #0
        beq			1f
        subs        r2, #4
1:
        b           _bad_sscanf


_sscanf_i:
        cmp         r6, #'-'        //check the sign
        bne			1f
        adds        r5, #PF_NEG
        adds		r0, #1
        ldrb        r6, [r0]
1:
        cmp         r6, #'0'
        bne         _sscanf_u       //first digit not '0' goto %u
        movs        r6, #0          //first digit is '0'
        ldrb        r3, [r0, #1]
        cmp         r3, #'x'
        beq			2f
        cmp         r3, #'X'
        bne         _sscanf_u_loop1     //second digit != 'x'
2:
		adds		r0, #2
        ldrb        r6, [r0]
_sscanf_x:
        cmp         r6, #'a'
        blo			3f
        subs        r6, #'a'-'A'        // makeupper
3:
        subs        r6, #'0'            //  x >= 0
        blo         _failed_sscanf
        cmp			r6, #15+7        // 22 >= x
        bhi         _failed_sscanf
        cmp         r6, #9
        bls			4f
        subs	    r6, #7
        cmp	 		r6, #9
        bls         _failed_sscanf      //  x > '9' && x < 'A'
4:
		push		{r1, r2}
		movs		r1, #0
_sscanf_X_loop:
		adds		r0, #1
        ldrb        r3, [r0]
        subs        r4, #1
        beq         _sscanf_u_done
        cmp         r3, #'a'
        blo			5f
        subs        r3, #'a'-'A'
5:
        subs        r3, #'0'            //  x >= '0'
        blo         _sscanf_u_done
        cmp         r3, #15+7
        bhi         _sscanf_u_done      // '0' <= x <= 'F'

        cmp         r3, #9              // 0 .. 22
        bls         1f                  // 0 .. 9 -> 10 ..22
        subs        r3, #7              // 3..15
        cmp         r3, #9
        bls         _sscanf_u_done
1:
#if USE_TMOS_LLONG
        lsls        r1, #4
        lsrs		r2, r6, #28
        adds        r1, r2
#endif
		lsls		r6, #4
        adds        r6, r3
        b           _sscanf_X_loop

_sscanf_set:
        movs        r3, #1
        mov         r12, r1
_sscanf_set_loop:
        mov         r1, r12
        ldrb        r6, [r0]
        cmp         r6, #0
        beq			_sscanf_set_done
_sscanf_set_next:
        ldrb        r5, [r1]
        adds		r1, #1
_sscanf_set_next1:
        cmp         r5, #0
        beq         _failed_sscanf      // missing ']'
        cmp         r5, #']'
        beq         _sscanf_set_done    // ] or source end
        cmp         r5, #'^'
        beq         _sscanf_set_not
        cmp         r6, r5
        blo         _sscanf_set_next
        beq         _sscanf_set_match

_sscanf_set_set:
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #'-'
        bne         _sscanf_set_next1
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r6, r5
        bhi         _sscanf_set_next

_sscanf_set_match:
        adds        r0, #1
        movs        r3, #2
        cmp         r7, #0
        beq			1f
        strb	    r6, [r7]
        adds		r7, #1
1:
        subs        r4, r4, #1
        bne         _sscanf_set_loop
_sscanf_set_done:
        cmp         r7, #0
        beq			1f
        movs        r6, #0
        strb        r6, [r7]
1:
        cmp         r3, #1
_sscanf_set_cleanup:
        bls         _failed_sscanf
        cmp         r5, #']'
        beq         _j_sloop
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #0
        b           _sscanf_set_cleanup

_sscanf_set_not:
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r6, r5
        blo         _sscanf_set_match
        beq         _sscanf_set_not_skip
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r5, #'-'
        bne         _sscanf_set_match
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r6, r5
        bhi         _sscanf_set_match
        b           _sscanf_set_next

_sscanf_set_not_skip:
        ldrb        r5, [r1]
        adds		r1, #1
        cmp         r5, #'-'
        bne         _sscanf_set_next1
        ldrb        r5, [r1]
        adds		r1, #1
        b           _sscanf_set_next1




ENDF(    tmos_sscanf        )


.end

