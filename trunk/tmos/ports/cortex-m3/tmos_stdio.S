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

FUNC(   tmos_vsprintf        )               // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12     0
        sub         sp, #VSPRINTF_BUF_SIZE
        push        {r0, r4, r5, r6, r7, r8} // dst fmt arg ___ ___ ___ ___ ___ ___ ___     [dst0 xxxxxx buf12-24
        mov			r8, r2
        mov         r12, r2                  // dst fmt ___ ___ ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        cmp         r3, #0
        beq         _end_loop
        cmp         r3, #'%'
        beq         _check_flags

_copy_format_char:                           // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        strb        r3, [r0], #1
_loop:                                       // dst fmt ___ ___ ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1
        cmp			r3, #0
        beq         _end_loop
        cmp         r3, #'%'
        bne         _copy_format_char

_check_flags:                                // dst fmt ___ '%' ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1
        cmp         r3, #'%'
        beq         _copy_format_char

        //--- process flags [-] [0] [n]      // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        cmp         r3, #'n'
        beq         _flag_n
        movs        r4, #0                   // dst fmt ___  c   0  ___ ___ ___ ar0 arg*
        movs        r6, #0                   // dst fmt ___  c   0  ___ pre ___ ar0 arg*
        movs        r2, #10                  // dst fmt 10   c   0  ___ pre ___ ar0 arg*

        cmp         r3, #'-'
        ITTE        EQ
        moveq       r7, #PF_LEFT
        ldrbeq      r3, [r1], #1             // dst fmt 10   c   0  ___ pre flg ar0 arg*
        movne       r7, #0                   // dst fmt 10   c   0  ___ pre flg ar0 arg*
        cmp         r3, #'0'
        ITT         EQ
        addeq       r7, #PF_ZERO
        ldrbeq      r3, [r1], #1             // dst fmt 10   c   0  ___ pre flg ar0 arg*

        //--- process [*] [*n$]
        cmp         r3,  #'*'
        beq         _get_param_width

_check_width:                                // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bls         _get_width

_check_precision:                            // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'.'
        beq         _get_precision
                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
        //--- process type [d] [i] [u] [x] [X] [c] [s]
_check_type:
        ldr         r5, [r12], #4            // dst fmt 10   c  wdt arg pre flg ar0 arg*
        cmp         r3, #'c'
        beq         _print_c
        cmp         r3, #'s'
        beq         _print_s

#if USE_TMOS_LLONG
        cmp         r3, #'l'
        bne			_skip_long
        eor			r3, r12, r8
        lsrs		r3, #3
        IT			cc
        ldrcc       r5, [r12], #4 // for stack alignment
        adds        r7, #PF_LONG
        ldrb	    r3, [r1], #1
_skip_long:
#endif

        str         r6, [sp, #24]
        add         r6, sp, #24              // dst fmt 10   c  wdt arg buf flg ar0 arg*   pre
        cmp         r3, #'d'
        IT          NE
        cmpne       r3, #'i'
        beq         _print_d
        cmp         r3, #'u'
        beq         _print_u
        cmp         r3, #'p'
        IT          NE
        cmpne       r3, #'x'
        IT          NE
        cmpne       r3, #'X'
        beq         _print_hex
_unk_format:
        movs        r3, #'?'
        strb        r3, [r0], #1
        movs        r3, #0
_end_loop:
        strb        r3, [r0]
        pop         {r1, r4, r5, r6, r7, r8}
        add         sp, #VSPRINTF_BUF_SIZE
        subs        r0, r0, r1
        bx          lr


_flag_n:
        ldr         r2, [r12], #4
        ldr         r3, [sp]
        subs        r3, r0
        str         r3, [r2]
        b           _loop

_get_param_width:                            // dst fmt 10  '*' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        rsbs        r5, r3, #'9'
        ITE         HS
        subshs      r5, r3, #'0'
        ldrlo       r4, [r12], #4
        blo         _check_precision
_get_param_width_loop:
        mla         r4, r4, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _unk_format
        subs        r5, r3, #'0'
        bhs         _get_param_width_loop
        cmp         r3, #'$'
        bne         _unk_format
        subs        r4, #1
        blo         _unk_format
        ldr         r4, [r8, r4, lsl #2]
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        b           _check_precision
                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_get_width:                                  // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        subs        r5, r3, #'0'
        blo         _check_precision
_get_width_loop:
        mla         r4, r4, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _check_precision
        subs        r5, r3, #'0'
        bhs         _get_width_loop
        cmp         r3, #'$'
        bne         _check_precision
        cmp         r7, #0
        bne         _check_precision
        subs        r4, #1
        blo         _unk_format
        add         r12, r8, r4, lsl #2
        b           _check_flags

_get_precision:                              // dst fmt 10  '.' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1
        cmp         r3, #'*'
        beq         _get_param_precision     // dst fmt 10   c  wdt ___ pre flg ar0 arg*

_get_precision_val:
        cmp         r3, #'9'
        bhi         _check_type
        subs        r3, #'0'
        blo         _unk_format
        mla         r6, r6, r2, r3
        ldrb        r3, [r1], #1
        b           _get_precision_val

_get_param_precision:                        // dst fmt 10  '*' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        rsbs        r5, r3, #'9'
        ITE         HS
        subshs      r5, r3, #'0'
        ldrlo       r6, [r12], #4
        blo         _check_type
_get_param_precision_loop:
        mla         r6, r6, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _unk_format
        subs        r5, r3, #'0'
        bhs         _get_param_precision_loop
        cmp         r3, #'$'
        bne         _unk_format
        subs        r6, #1
        blo         _unk_format
        ldr         r6, [r8, r6, lsl #2]
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        b           _check_type


_print_c:                                    // dst fmt 10  'c' wdt arg pre flg ar0 arg*
        cbz         r5, _prints_left_pad
        uxtb        r3, r5
        add         r5, sp, #24
        str         r3, [r5]

_print_s:                                    // dst fmt 10  's' wdt arg pre flg ar0 arg*
        cbz         r5, _prints_left_pad
        mov         r2, r5                   // dst fmt str ___ wdt arg pre flg ar0 arg*
_print_get_len:
        ldrb        r3, [r5], #1
        cmp         r3, #0
        bne         _print_get_len
        subs        r5, r2                   // dst fmt str ___ wdt len pre flg ar0 arg*
        subs        r5, #1
        beq         _prints_left_pad
        cbz         r6, _prints_pd        // precision?
        cmp         r5, r6                // if(len > pre)
        IT          HI
        movhi       r5, r6                //  len = pre
        cmp         r4, r6                // if (wdt > pre)
        IT          HI
        movhi       r4, r6                //  wdt = pre
_prints_pd:                                  // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
        subs        r4, r5                   // dst fmt str ___ w-l len ___ flg ar0 arg*
        bls         _prints_copy
        lsrs        r7, #1
        bcs         _prints_copy
        movs        r3, #' '
_prints_right_pad:                           // dst fmt str ' ' w-l len ___ ___ ar0 arg*
        strb        r3, [r0], #1
        subs        r4, #1
        bne         _prints_right_pad
_prints_copy:                                // dst fmt str ' ' 0   len ___ ___ ar0 arg*
        ldrb        r3, [r2], #1
        strb        r3, [r0], #1
        subs        r5, #1
        bne         _prints_copy
_prints_left_pad:                            // dst fmt ___ ___ w-l ___ ___ ___ ar0 arg*
        subs        r4, #1
        blt         _loop
        movs        r3, #' '
        strb        r3, [r0], #1
        b           _prints_left_pad

                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_print_hex:                                  // dst fmt ___ 'x' wdt arg buf flg ar0 arg*   pre
        subs        r3, #'x'-'a'+10
#if USE_TMOS_LLONG
        push        {r0}
        movs        r0, #8
#endif
_print_hex_loop:
        and         r2, r5, #0xF
        cmp         r2, #9
        ITE         HI
        addhi       r2, r2, r3
        addls       r2, #'0'
        strb        r2, [r6, #1]!
#if USE_TMOS_LLONG
        subs        r0, #1
#endif
        lsrs        r5, r5, #4
        bne         _print_hex_loop

#if USE_TMOS_LLONG
        lsrs        r5, r7, #3
        ITT         cc
        popcc       {r0}
        bcc         _print_numeric
        subs        r7, #PF_LONG
        ldr         r5, [r12], #4            // dig fmt ___ 'x' wdt arg buf flg ar0 arg*  pre
        cmp         r5, #0
        ITT          EQ
        popeq       {r0}
        beq         _print_numeric
        movs        r2, #'0'
1:
        subs        r0, #1
        ITT         HS
        strbhs      r2, [r6, #1]!
        bhs         1b
        movs        r0, #8
        b           _print_hex_loop
#else
        b           _print_numeric           // dst fmt ___ ___ wdt  0  buf flg ar0 arg*   pre
#endif


#if USE_TMOS_LLONG
_print_d:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        lsrs        r3, r7, #3
        bcc         _print_d32
        ldr         r3, [r12], #4
        cmp         r3, #0
        bge         _print_u64
        negs        r5, r5                      // n < 0
        sbc         r3, r3, r3, lsl #1
        adds        r7, #PF_NEG

_print_u64:                                  // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        subs        r7, #PF_LONG
        push        {r0, r1}                 // r0  r1  r2  r3  r4  r5  r6  r7
        lsr         r1, r5, #16              // n0  n1  10  n2_ wdt ___ buf flg
        uxth        r0, r5

        cbz         r3, _print_u64_done
1:
        udiv        r5, r3, r2
        mls         r3, r5, r2, r3           // n0  n1  10  t1  wdt m2  buf flg ___ ___
        add         r3, r1, r3, lsl  #16
        udiv        r1, r3, r2
        mls         r3, r1, r2, r3           // n0  n1  10  t1  wdt m2  buf flg ___ ___
        add         r3, r0, r3, lsl #16
        udiv        r0, r3, r2
        mls         r3, r0, r2, r3
        adds        r3, #'0'
        strb        r3, [r6, #1]!
        movs        r3, r5
        bne         1b
_print_u64_done:
        adds        r5, r0, r1, lsl #16
        pop         {r0, r1}
        b           _print_u32

_print_d32:
        cmp         r5, #0
        bge          _print_u32
        adds        r7, #PF_NEG
        rsbs        r5, r5, #0
        b           _print_u32

_print_u:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        lsrs        r3, r7, #3
        ITT         CS
        ldrcs       r3, [r12], #4
        bcs         _print_u64

_print_u32:

#else
_print_d:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        cmp         r5, #0
        ITT         MI
        addmi       r7, #PF_NEG
        rsbmi       r5, r5, #0
_print_u:                                    // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
#endif

        udiv        r3, r5, r2
        mls         r5, r2, r3, r5
        adds        r5, #'0'
        strb        r5, [r6, #1]!
        cbz         r3, _print_numeric
        udiv        r5, r3, r2
        mls         r3, r2, r5, r3
        adds        r3, #'0'
        strb        r3, [r6, #1]!
        cmp         r5, #0
        bne         _print_u                 // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_print_numeric:                              // dst fmt ___ ___ wdt ___ buf flg ar0 arg*   pre
        add         r5, sp, #24
        ldrb        r2, [r5]                 // dst fmt pre ___ wdt ___ buf flg ar0 arg*
        subs        r5, r6, r5               // dst fmt pre ___ wdt len buf flg ar0 arg*
        cmp         r2, r5
        ITE         HI
        movhi       r3, r2
        movls       r3, r5            //r3 = max(P,L)
        subs        r4, r3                   // dst fmt pre max w-m len buf flg ar0 arg*
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
        subs        r2, #1
        strb        r3, [r0], #1
        subs        r4, #1
        bls         _print_precision
        lsrs        r3, r7, #2

_print_right_pad:
        adds        r3, #' '
_print_right_pad_loop:
        strb        r3, [r0], #1
        subs        r4, #1
        bne         _print_right_pad_loop
_print_left_align:
        lsrs        r3, r7, #2        //c=FL_NEG
        bcs         _print_left_minus
_print_precision:                            // dst fmt pre ___ wdt len buf flg ar0 arg*
        subs        r2, r5
        ble         _print_digits
        movs        r3, #'0'
_print_precision_loop:                       // dst fmt pre ___ wdt len buf ___ ar0 arg*
        strb        r3, [r0], #1
        subs        r2, #1
        bne         _print_precision_loop
_print_digits:                               // dst fmt ___ ___ w-m len buf ___ ar0 arg*
        ldrb        r3, [r6], #-1
        strb        r3, [r0], #1
        subs        r5, #1
        bne         _print_digits
        movs        r3, #' '
_print_left_pad:                             // dst fmt ___ ___ w-m ___ ___ ___ ar0 arg*
        subs        r4, #1
        blt         _loop
        strb        r3, [r0], #1
        b           _print_left_pad

_print_left_minus:                           // dst fmt pre ___ wdt len buf flg ar0 arg*
        movs        r3, #'-'
        subs        r2, #1
        strb        r3, [r0], #1
        subs        r4, #1
        b           _print_precision

ENDF(   tmos_vsprintf        )

FUNC(   tmos_dbg_char       )
        push        {r0, r1, r2, lr}
        mov         r0, r3
        bl          usr_trace_char
        pop         {r0, r1, r2, r3}
        adds        r0, #1
        bx          r3
ENDF(   tmos_dbg_char       )

// int tmos_dbgprintf( uint32_t zero, const char *format, va_list arg );
// stack usage 32 bytes
FUNC(   tmos_dbgprintf       )               // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12     0
        push        {lr}
        sub         sp, #VSPRINTF_BUF_SIZE
        push        {r0, r4, r5, r6, r7, r8} // dst fmt arg ___ ___ ___ ___ ___ ___ ___     [dst0 xxxxxx buf12-24
        mov         r8, r2
        mov         r12, r2                  // dst fmt ___ ___ ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        cmp         r3, #0
        beq         _dbg_end_loop
        cmp         r3, #'%'
        beq         _dbg_check_flags

_dbg_copy_format_char:                       // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        bl          tmos_dbg_char
_dbg_loop:                                   // dst fmt ___ ___ ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1
        cmp         r3, #0
        beq         _dbg_end_loop
        cmp         r3, #'%'
        bne         _dbg_copy_format_char

_dbg_check_flags:                            // dst fmt ___ '%' ___ ___ ___ ___ ar0 arg*
        ldrb        r3, [r1], #1
        cmp         r3, #'%'
        beq         _dbg_copy_format_char

        //--- process flags [-] [0] [n]      // dst fmt ___  c  ___ ___ ___ ___ ar0 arg*
        cmp         r3, #'n'
        beq         _dbg_flag_n
        movs        r4, #0                   // dst fmt ___  c   0  ___ ___ ___ ar0 arg*
        movs        r6, #0                   // dst fmt ___  c   0  ___ pre ___ ar0 arg*
        movs        r2, #10                  // dst fmt 10   c   0  ___ pre ___ ar0 arg*

        cmp         r3, #'-'
        ITTE        EQ
        moveq       r7, #PF_LEFT
        ldrbeq      r3, [r1], #1             // dst fmt 10   c   0  ___ pre flg ar0 arg*
        movne       r7, #0                   // dst fmt 10   c   0  ___ pre flg ar0 arg*
        cmp         r3, #'0'
        ITT         EQ
        addeq       r7, #PF_ZERO
        ldrbeq      r3, [r1], #1             // dst fmt 10   c   0  ___ pre flg ar0 arg*

        //--- process [*] [*n$]
        cmp         r3,  #'*'
        beq         _dbg_get_param_width

_dbg_check_width:                            // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bls         _dbg_get_width

_dbg_check_precision:                        // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'.'
        beq         _dbg_get_precision
                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
        //--- process type [d] [i] [u] [x] [X] [c] [s]
_dbg_check_type:
        ldr         r5, [r12], #4            // dst fmt 10   c  wdt arg pre flg ar0 arg*
        cmp         r3, #'c'
        beq         _dbg_print_c
        cmp         r3, #'s'
        beq         _dbg_print_s

#if USE_TMOS_LLONG
        cmp         r3, #'l'
        bne         _dbg_skip_long
        eor         r3, r12, r8
        lsrs        r3, #3
        IT          cc
        ldrcc       r5, [r12], #4 // for stack alignment
        adds        r7, #PF_LONG
        ldrb        r3, [r1], #1
_dbg_skip_long:
#endif

        str         r6, [sp, #24]
        add         r6, sp, #24              // dst fmt 10   c  wdt arg buf flg ar0 arg*   pre
        cmp         r3, #'d'
        IT          NE
        cmpne       r3, #'i'
        beq         _dbg_print_d
        cmp         r3, #'u'
        beq         _dbg_print_u
        cmp         r3, #'p'
        IT          NE
        cmpne       r3, #'x'
        IT          NE
        cmpne       r3, #'X'
        beq         _dbg_print_hex
_dbg_unk_format:
        movs        r3, #'?'
        bl          tmos_dbg_char
        movs        r3, #0
_dbg_end_loop:
        bl          tmos_dbg_char
        pop         {r1, r4, r5, r6, r7, r8}
        add         sp, #VSPRINTF_BUF_SIZE
        subs        r0, r0, r1
        pop         {pc}


_dbg_flag_n:
        ldr         r2, [r12], #4
        ldr         r3, [sp]
        subs        r3, r0
        str         r3, [r2]
        b           _dbg_loop

_dbg_get_param_width:                        // dst fmt 10  '*' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        rsbs        r5, r3, #'9'
        ITE         HS
        subshs      r5, r3, #'0'
        ldrlo       r4, [r12], #4
        blo         _dbg_check_precision
_dbg_get_param_width_loop:
        mla         r4, r4, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _dbg_unk_format
        subs        r5, r3, #'0'
        bhs         _dbg_get_param_width_loop
        cmp         r3, #'$'
        bne         _dbg_unk_format
        subs        r4, #1
        blo         _dbg_unk_format
        ldr         r4, [r8, r4, lsl #2]
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        b           _dbg_check_precision
                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_dbg_get_width:                              // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        subs        r5, r3, #'0'
        blo         _dbg_check_precision
_dbg_get_width_loop:
        mla         r4, r4, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _dbg_check_precision
        subs        r5, r3, #'0'
        bhs         _dbg_get_width_loop
        cmp         r3, #'$'
        bne         _dbg_check_precision
        cmp         r7, #0
        bne         _dbg_check_precision
        subs        r4, #1
        blo         _dbg_unk_format
        add         r12, r8, r4, lsl #2
        b           _dbg_check_flags

_dbg_get_precision:                          // dst fmt 10  '.' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1
        cmp         r3, #'*'
        beq         _dbg_get_param_precision // dst fmt 10   c  wdt ___ pre flg ar0 arg*

_dbg_get_precision_val:
        cmp         r3, #'9'
        bhi         _dbg_check_type
        subs        r3, #'0'
        blo         _dbg_unk_format
        mla         r6, r6, r2, r3
        ldrb        r3, [r1], #1
        b           _dbg_get_precision_val

_dbg_get_param_precision:                    // dst fmt 10  '*' wdt ___ pre flg ar0 arg*
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        rsbs        r5, r3, #'9'
        ITE         HS
        subshs      r5, r3, #'0'
        ldrlo       r6, [r12], #4
        blo         _dbg_check_type
_dbg_get_param_precision_loop:
        mla         r6, r6, r2, r5
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        cmp         r3, #'9'
        bhi         _dbg_unk_format
        subs        r5, r3, #'0'
        bhs         _dbg_get_param_precision_loop
        cmp         r3, #'$'
        bne         _dbg_unk_format
        subs        r6, #1
        blo         _dbg_unk_format
        ldr         r6, [r8, r6, lsl #2]
        ldrb        r3, [r1], #1             // dst fmt 10   c  wdt ___ pre flg ar0 arg*
        b           _dbg_check_type


_dbg_print_c:                                // dst fmt 10  'c' wdt arg pre flg ar0 arg*
        cbz         r5, _dbg_prints_left_pad
        uxtb        r3, r5
        add         r5, sp, #24
        str         r3, [r5]

_dbg_print_s:                                // dst fmt 10  's' wdt arg pre flg ar0 arg*
        cbz         r5, _dbg_prints_left_pad
        mov         r2, r5                   // dst fmt str ___ wdt arg pre flg ar0 arg*
_dbg_print_get_len:
        ldrb        r3, [r5], #1
        cmp         r3, #0
        bne         _dbg_print_get_len
        subs        r5, r2                   // dst fmt str ___ wdt len pre flg ar0 arg*
        subs        r5, #1
        beq         _dbg_prints_left_pad
        cbz         r6, _dbg_prints_pd    // precision?
        cmp         r5, r6                // if(len > pre)
        IT          HI
        movhi       r5, r6                //  len = pre
        cmp         r4, r6                // if (wdt > pre)
        IT          HI
        movhi       r4, r6                //  wdt = pre
_dbg_prints_pd:                              // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
        subs        r4, r5                   // dst fmt str ___ w-l len ___ flg ar0 arg*
        bls         _dbg_prints_copy
        lsrs        r7, #1
        bcs         _dbg_prints_copy
        movs        r3, #' '
_dbg_prints_right_pad:                       // dst fmt str ' ' w-l len ___ ___ ar0 arg*
        bl          tmos_dbg_char
        subs        r4, #1
        bne         _dbg_prints_right_pad
_dbg_prints_copy:                            // dst fmt str ' ' 0   len ___ ___ ar0 arg*
        ldrb        r3, [r2], #1
        bl          tmos_dbg_char
        subs        r5, #1
        bne         _dbg_prints_copy
_dbg_prints_left_pad:                        // dst fmt ___ ___ w-l ___ ___ ___ ar0 arg*
        subs        r4, #1
        blt         _dbg_loop
        movs        r3, #' '
        bl          tmos_dbg_char
        b           _dbg_prints_left_pad

                                             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_dbg_print_hex:                              // dst fmt ___ 'x' wdt arg buf flg ar0 arg*   pre
        subs        r3, #'x'-'a'+10
#if USE_TMOS_LLONG
        push        {r0}
        movs        r0, #8
#endif
_dbg_print_hex_loop:
        and         r2, r5, #0xF
        cmp         r2, #9
        ITE         HI
        addhi       r2, r2, r3
        addls       r2, #'0'
        strb        r2, [r6, #1]!
#if USE_TMOS_LLONG
        subs        r0, #1
#endif
        lsrs        r5, r5, #4
        bne         _dbg_print_hex_loop

#if USE_TMOS_LLONG
        lsrs        r5, r7, #3
        ITT         cc
        popcc       {r0}
        bcc         _dbg_print_numeric
        subs        r7, #PF_LONG
        ldr         r5, [r12], #4            // dig fmt ___ 'x' wdt arg buf flg ar0 arg*  pre
        cmp         r5, #0
        ITT          EQ
        popeq       {r0}
        beq         _dbg_print_numeric
        movs        r2, #'0'
1:
        subs        r0, #1
        ITT         HS
        strbhs      r2, [r6, #1]!
        bhs         1b
        movs        r0, #8
        b           _dbg_print_hex_loop
#else
        b           _dbg_print_numeric       // dst fmt ___ ___ wdt  0  buf flg ar0 arg*   pre
#endif


#if USE_TMOS_LLONG
_dbg_print_d:                                // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        lsrs        r3, r7, #3
        bcc         _dbg_print_d32
        ldr         r3, [r12], #4
        cmp         r3, #0
        bge         _dbg_print_u64
        negs        r5, r5                      // n < 0
        sbc         r3, r3, r3, lsl #1
        adds        r7, #PF_NEG

_dbg_print_u64:                              // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        subs        r7, #PF_LONG
        push        {r0, r1}                 // r0  r1  r2  r3  r4  r5  r6  r7
        lsr         r1, r5, #16              // n0  n1  10  n2_ wdt ___ buf flg
        uxth        r0, r5

        cbz         r3, _dbg_print_u64_done
1:
        udiv        r5, r3, r2
        mls         r3, r5, r2, r3           // n0  n1  10  t1  wdt m2  buf flg ___ ___
        add         r3, r1, r3, lsl  #16
        udiv        r1, r3, r2
        mls         r3, r1, r2, r3           // n0  n1  10  t1  wdt m2  buf flg ___ ___
        add         r3, r0, r3, lsl #16
        udiv        r0, r3, r2
        mls         r3, r0, r2, r3
        adds        r3, #'0'
        strb        r3, [r6, #1]!
        movs        r3, r5
        bne         1b
_dbg_print_u64_done:
        adds        r5, r0, r1, lsl #16
        pop         {r0, r1}
        b           _dbg_print_u32

_dbg_print_d32:
        cmp         r5, #0
        bge          _dbg_print_u32
        adds        r7, #PF_NEG
        rsbs        r5, r5, #0
        b           _dbg_print_u32

_dbg_print_u:                                // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        lsrs        r3, r7, #3
        ITT         CS
        ldrcs       r3, [r12], #4
        bcs         _dbg_print_u64

_dbg_print_u32:

#else
_dbg_print_d:                                // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
        cmp         r5, #0
        ITT         MI
        addmi       r7, #PF_NEG
        rsbmi       r5, r5, #0
_dbg_print_u:                                // dst fmt 10  ___ wdt arg buf flg ar0 arg*   pre
#endif

        udiv        r3, r5, r2
        mls         r5, r2, r3, r5
        adds        r5, #'0'
        strb        r5, [r6, #1]!
        cbz         r3, _dbg_print_numeric
        udiv        r5, r3, r2
        mls         r3, r2, r5, r3
        adds        r3, #'0'
        strb        r3, [r6, #1]!
        cmp         r5, #0
        bne         _dbg_print_u             // r0  r1  r2  r3  r4  r5  r6  r7  r8  r12
_dbg_print_numeric:                          // dst fmt ___ ___ wdt ___ buf flg ar0 arg*   pre
        add         r5, sp, #24
        ldrb        r2, [r5]                 // dst fmt pre ___ wdt ___ buf flg ar0 arg*
        subs        r5, r6, r5               // dst fmt pre ___ wdt len buf flg ar0 arg*
        cmp         r2, r5
        ITE         HI
        movhi       r3, r2
        movls       r3, r5            //r3 = max(P,L)
        subs        r4, r3                   // dst fmt pre max w-m len buf flg ar0 arg*
        bls         _dbg_print_left_align
        lsrs        r3, r7, #1        //c=FL_LEFT
        bcs         _dbg_print_left_align
        //right with W>0
        lsrs        r3, r7, #2        //c=FL_NEG, r3=zero>>2
        bls         _dbg_print_right_pad
        //PF_ZERO && PF_NEG
//_print_right_minus:
        subs        r7, #PF_NEG
        movs        r3, #'-'
        subs        r2, #1
        bl          tmos_dbg_char
        subs        r4, #1
        bls         _dbg_print_precision
        lsrs        r3, r7, #2

_dbg_print_right_pad:
        adds        r3, #' '
_dbg_print_right_pad_loop:
        bl          tmos_dbg_char
        subs        r4, #1
        bne         _dbg_print_right_pad_loop
_dbg_print_left_align:
        lsrs        r3, r7, #2        //c=FL_NEG
        bcs         _dbg_print_left_minus
_dbg_print_precision:                        // dst fmt pre ___ wdt len buf flg ar0 arg*
        subs        r2, r5
        ble         _dbg_print_digits
        movs        r3, #'0'
_dbg_print_precision_loop:                   // dst fmt pre ___ wdt len buf ___ ar0 arg*
        bl          tmos_dbg_char
        subs        r2, #1
        bne         _dbg_print_precision_loop
_dbg_print_digits:                           // dst fmt ___ ___ w-m len buf ___ ar0 arg*
        ldrb        r3, [r6], #-1
        bl          tmos_dbg_char
        subs        r5, #1
        bne         _dbg_print_digits
        movs        r3, #' '
_dbg_print_left_pad:                         // dst fmt ___ ___ w-m ___ ___ ___ ar0 arg*
        subs        r4, #1
        blt         _dbg_loop
        bl          tmos_dbg_char
        b           _dbg_print_left_pad

_dbg_print_left_minus:                       // dst fmt pre ___ wdt len buf flg ar0 arg*
        movs        r3, #'-'
        subs        r2, #1
        bl          tmos_dbg_char
        subs        r4, #1
        b           _dbg_print_precision

ENDF(   tmos_dbgprintf        )

//-----------------------------------------------------------------------------
// int tmos_vprintf_len( const char *format, va_list arg );
// stack usage 32 bytes
FUNC(   tmos_vprintf_len        )   // r0  r1  r2  r3  r4  r5  r6  r7  r12
                                    // fmt arg

        push        {r4, r5, r6, r7, r8}
        mov         r2, r0          // fmt arg fmt
        and			r8, r1, #4
        movs        r0, #0          // res arg fmt

_vpl_loop:
        ldrb        r3, [r2], #1    // res arg fmt c
        adds        r0, #1
        cbz         r3, _vpl__end_loop
        cmp         r3, #'%'
        bne         _vpl_loop

        ldrb        r3, [r2], #1    // r3 = format charachter
        cmp         r3, #'%'
        beq         _vpl_loop
        subs        r0, #1

        //--- process flags [-] [0] [n]
        cmp         r3, #'n'
        beq         _vpl_loop
        movs        r4, #0          // res arg fmt c wdt
        movs        r6, #0          // res arg fmt c wdt ___ pre
        movs        r7, #10         // res arg fmt c wdt ___ pre 10

        cmp         r3, #'-'
        IT          EQ
        ldrbeq      r3, [r2], #1

        cmp         r3, #'0'
        IT          EQ
        ldrbeq      r3, [r2], #1

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
        ldr         r5, [r1], #4    // res arg fmt c wdt ___ pre 10
        cmp         r3, #'c'
        beq         _vpl_print_c
        cmp         r3, #'s'
        beq         _vpl_print_s
        movs        r5, #11
#if USE_TMOS_LLONG
        cmp         r3, #'l'
        bne			_vpl_skip_long
        eor			r3, r1, r8
        lsrs		r3, #3
        IT			cc
        addcc       r1, #4 // for stack alignment
        add       	r1, #4
        add       	r5, #10
		add			r2, #1
_vpl_skip_long:
#endif
        cmp         r5, r6
        bhs         _vpl_add_len
        mov         r5, r6                // r5 = max(12, precision)
        b           _vpl_add_len

_vpl_unk_format:
        adds        r0, #1
_vpl__end_loop:
        pop         {r4, r5, r6, r7, r8}
        bx          lr


_vpl_get_param_width:
        ldr         r4, [r1], #4
        ldrb        r3, [r2], #1
        b           _vpl_check_precision

_vpl_get_width:
        subs        r5, r3, #'0'
        blo         _vpl_check_precision
        mla         r4, r4, r7, r5
        ldrb        r3, [r2], #1
        b           _vpl_check_width

_vpl_get_precision:
        ldrb        r3, [r2], #1
        cmp         r3, #'*'
        beq         _vpl_get_param_precision

_vpl_get_precision_val:
        cmp         r3, #'9'
        bhi         _vpl_check_type
        subs        r3, #'0'
        blo         _vpl_unk_format
        mla         r6, r6, r7, r3
        ldrb        r3, [r2], #1
        b           _vpl_get_precision_val

_vpl_get_param_precision:
        ldr         r6, [r1], #4
        ldrb        r3, [r2], #1
        b           _vpl_check_type

_vpl_print_c:
        cbz         r5, _vpl_add_width
        movs        r5, #1
        b           _vpl_add_len

_vpl_print_s:
        cbz         r5, _vpl_add_width    // "%s",NULL
        mov         r7, r5
_vpl_print_get_len:
        ldrb        r3, [r5], #1
        cmp         r3, #0
        bne         _vpl_print_get_len
        subs        r5, r7                //r5 = strlen(arg)
        subs        r5, #1

        cbz         r6, _vpl_add_len      // Precision==0
        cmp         r5, r6
        IT          HI
        movhi       r5, r6                //r5 = len = min(len, precsion)
_vpl_add_len:
        cmp         r5, r4
        IT          HI
        movhi       r4, r5                //r4 = max(width, len)
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
FUNC(    tmos_sscanf        )

        cbz         r0, _sexit
        push        {r0, r2, r3}        // push all params in stack
        mov         r2, sp              // ++r2 = va_list
#if USE_TMOS_LLONG
        push        {r4, r5, r6, r7, r8, r9}
#else
        push        {r4, r5, r6, r7}
#endif


_sloop:
        ldrb        r3, [r1], #1        // r3 = format charachter
        cmp         r3, #'%'
        beq         _check_sformat
        cmp         r3, #' '
        bhi         _check_schar
_check_spaces:
        cbz         r3, _end_sscanf
        ldrb        r3, [r0]
        cmp         r3, #' '
        bhi         _sloop
        adds        r0, #1
        b           _check_spaces

_check_schar:
        ldrb        r12, [r0], #1
        cmp         r3, r12
        beq         _sloop
_end_sscanf:
_bad_sscanf:
#if USE_TMOS_LLONG
        pop         {r4, r5, r6, r7, r8, r9}
#else
        pop         {r4, r5, r6, r7}
#endif
        mov         r3, sp
        subs        r2, r3
        lsrs        r0, r2, #2
        add         sp, #3*4
_sexit:
        bx          lr




_scanf_n:
        cbz         r7, _scanf_n_d
#if USE_TMOS_LLONG
        ldr         r3, [sp, #6*4]
#else
        ldr         r3, [sp, #4*4]
#endif
        subs        r3, r0, r3
        str         r3, [r7]
_scanf_n_d:
        b           _sloop

_check_sformat:							// r0  r1  r2  r3  r4  r5  r6  r7  r12
        ldrb        r3, [r1], #1		// src fmt arg f        		   10
        cmp         r3, #'%'
        beq         _check_schar
        mov         r12, #10
        movs        r4, #0              // src fmt arg f   wdt
        subs        r7, r3, #'*'        // src fmt arg f   wdt         prm
        ITE         EQ
        ldrbeq      r3, [r1], #1
        ldrne       r7, [r2, #4]!
        cmp         r3, #'n'
        beq         _scanf_n
_check_swidth:
        cmp         r3, #'9'
        bls         _get_swidth
_check_stype:

        cmp         r3, #'c'
        beq         _sscanf_c
        cmp         r3, #'['
        beq         _sscanf_set

_skip_white:
        ldrb        r6, [r0]			// src fmt arg f   wdt       c  prm  10
        cbz         r6, _failed_sscanf
        cmp         r6, #' '
        IT          LS
        addls       r0, #1
        bls         _skip_white

        movs        r5, #0				// src fmt arg f   wdt flg c  prm  10
#if USE_TMOS_LLONG						// r0  r1  r2  r3  r4  r5  r6  r7  r12
        movs        r9, #0
        cmp         r3, #'l'
        ITT         EQ
        addeq       r5, #PF_LONG
        ldrbeq      r3, [r1], #1
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
        cmp         r7, #0
        IT          NE
        strbne      r6, [r7], #1
        ldrb        r6, [r0, #1]!
        subs        r4, #1
        IT          NE
        cmpne       r6, #' '
        bhi         _sscanf_s
        movs        r6, #0
        cmp         r7, #0
        beq         _sloop
        strb        r6, [r7]
        b           _sloop

_get_swidth:
        subs        r6, r3, #'0'
        blo         _check_stype
        mla         r4, r4, r12, r6
        ldrb        r3, [r1], #1
        b           _check_swidth

_failed_sscanf:
        cbz         r7, _b_bad_sscanf
        subs        r2, #4
_b_bad_sscanf:
        b           _bad_sscanf

_sscanf_c:								// src fmt arg f   wdt 10      prm  10
        ldrb        r6, [r0], #1
        cmp         r7, #0
        IT          NE
        strbne      r6, [r7], #1
        cmp         r6, #0
        beq         _failed_sscanf
        subs        r4, r4, #1
        bhi         _sscanf_c
        b           _sloop


_sscanf_d:                              // r0  r1  r2  r3  r4  r5  r6  r7  r12
        cmp         r6, #'+'            // src fmt arg ___ wdt flg  c  par
        IT          EQ
        ldrbeq      r6, [r0, #1]!
        cmp         r6, #'-'            // src fmt arg ___ wdt flg  c  par
        ITT         EQ
        addeq       r5, #PF_NEG
        ldrbeq      r6, [r0, #1]!
_sscanf_u:
        subs        r6, #'0'            // x >= 0
        blo         _failed_sscanf
        cmp         r6, #9              // 9 >= 0
        bhi         _failed_sscanf
_sscanf_u_loop:                         // src fmt arg ___ wdt flg res par
        ldrb        r3, [r0, #1]!
_sscanf_u_loop1:                        // src fmt arg  c  wdt flg res par
        subs        r4, #1
        beq         _sscanf_u_done
        subs        r3, #'0'            // x >= 0
#if USE_TMOS_LLONG
        blo         _sscanf_u_done
        cmp         r3, #9
        bhi         _sscanf_u_done         // r9:r6 -> r8:r6
        umull       r6, r8, r6, r12        // r8:r6 = r6*10
        mla         r8, r9, r12, r8        // r8 += r9 *10
        adds        r6, r3
        adc         r9, r8, #0             // r9:r6 = r8:r6 + r3
        b           _sscanf_u_loop
#else
        ITTT        HS
        rsbshs      r12, r3, #9         // 9 >= 0
        addhs       r6, r6, r6, lsl #2
        addhs       r6, r3, r6, lsl #1
        bhs         _sscanf_u_loop
#endif

_sscanf_u_done:
        cmp         r7, #0          //if ptr != null
        beq         _sloop
        lsrs        r5, #2        //if negative
#if USE_TMOS_LLONG
        bcc         1f
        negs        r6, r6
        sbc         r9, r9, r9, lsl #1
1:
        cbz         r5, 2f
        str         r9, [r7, #4]
2:
        str         r6, [r7]
        b           _sloop
#else
        IT          CS
        negcs       r6, r6
        str         r6, [r7]
        b           _sloop
#endif


_sscanf_i:
        cmp         r6, #'-'        //check the sign
        ITT         EQ
        addeq       r5, #PF_NEG
        ldrbeq      r6, [r0, #1]!
        cmp         r6, #'0'
        bne         _sscanf_u       //first digit not '0' goto %u
        movs        r6, #0          //first digit is '0'
        ldrb        r3, [r0, #1]!
        cmp         r3, #'x'
        IT          NE
        cmpne       r3, #'X'
        bne         _sscanf_u_loop1     //second digit != 'x'
        ldrb        r6, [r0, #1]!
_sscanf_x:
        cmp         r6, #'a'
        IT          HS
        subhs       r6, #'a'-'A'        // makeupper
        subs        r6, #'0'            //  x >= 0
        IT          HS
        rsbshs      r3, r6, #15+7        // 22 >= x
        blo         _failed_sscanf
        cmp         r6, #9
        ITT         HI
        subhi       r6, #7
        rsbshi      r3, r6, #10
        bhi         _failed_sscanf      //  x > '9' && x < 'A'

_sscanf_X_loop:
        ldrb        r3, [r0, #1]!
        subs        r4, #1
        beq         _sscanf_u_done
        cmp         r3, #'a'
        IT          HS
        subhs       r3, #'a'-'A'
        subs        r3, r3, #'0'        //  x >= '0'
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
        lsls        r9, #4
        add         r9, r9, r6, lsr #28
#endif
        add         r6, r3, r6, lsl #4
        b           _sscanf_X_loop



_sscanf_set:
        movs        r12, #1
        mov         r3, r1
_sscanf_set_loop:
        mov         r1, r3
        ldrb        r6, [r0]
        cbz         r6, _sscanf_set_done
_sscanf_set_next:
        ldrb        r5, [r1], #1
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
        ldrb        r5, [r1], #1
        cmp         r5, #'-'
        bne         _sscanf_set_next1
        ldrb        r5, [r1], #1
        cmp         r6, r5
        bhi         _sscanf_set_next

_sscanf_set_match:
        adds        r0, #1
        movs        r12, #2
        cmp         r7, #0
        IT          NE
        strbne      r6, [r7], #1
        subs        r4, r4, #1
        bne         _sscanf_set_loop
_sscanf_set_done:
        movs        r6, #0
        cmp         r7, #0
        IT          NE
        strbne      r6, [r7]
        cmp         r12, #1
_sscanf_set_cleanup:
        bls         _failed_sscanf
        cmp         r5, #']'
        beq         _sloop
        ldrb        r5, [r1], #1
        cmp         r5, #0
        b           _sscanf_set_cleanup

_sscanf_set_not:
        ldrb        r5, [r1], #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r6, r5
        blo         _sscanf_set_match
        beq         _sscanf_set_not_skip
        ldrb        r5, [r1], #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r5, #'-'
        bne         _sscanf_set_match
        ldrb        r5, [r1], #1
        cmp         r5, #0
        beq         _failed_sscanf
        cmp         r6, r5
        bhi         _sscanf_set_match
        b           _sscanf_set_next

_sscanf_set_not_skip:
        ldrb        r5, [r1], #1
        cmp         r5, #'-'
        bne         _sscanf_set_next1
        ldrb        r5, [r1], #1
        b           _sscanf_set_next1

ENDF(    tmos_sscanf        )


.end

