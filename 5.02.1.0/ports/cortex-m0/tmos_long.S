/*
 * tmos_long.S
 *
 *  r3reated on: Dec 1, 2014
 *      Author: miro
 */


#include "port_asm.h"
#include "port_inc.h"

// int __aeabi_idiv0(int return_value);
// return:
//      the value passed to them as a parameter
//      or fixed value defined by the execution environment (such as 0).
//      Or, raise a signal (often SIGFPE) or throw an exception, and do not return.
FUNC(   __aeabi_idiv0     )
.weak __aeabi_idiv0
                bx              lr
ENDF(   __aeabi_idiv0     )


// typedef struct { int quot r0; int rem r0; } idiv_return;
// unsigned __aeabi_uidiv(unsigned numerator, unsigned denominator);
// __value_in_regs uidiv_return __aeabi_uidivmod(
//              unsigned numerator r0, unsigned denominator r1);
FUNC(   __aeabi_uidiv     )
__aeabi_uidivmod:
                movs            r2, r1
                movs            r1, #0
                adds            r0, r0      // shift first bit out

                .rep            32

                adcs            r1, r1      // get one bit
                subs            r1, r2      // try subtract
                bcs             1f          // check
                add             r1, r2      // undo
1:
                adcs            r0, r0      // shift result in, and next bit out
                .endr

                bx              lr
ENDF(   __aeabi_uidiv     )

// __value_in_regs idiv_return __aeabi_idivmod(int numerator, int denominator);
// int __aeabi_idiv(int numerator, int denominator);
FUNC(   __aeabi_idiv     )
__aeabi_idivmod:
                mov             r12, lr
                cmp             r0, #0
                blt             1f
                cmp             r1, #0
                bge             __aeabi_uidivmod        // ++   ->  ++
                negs            r1, r1
                bl              __aeabi_uidivmod        // +-   ->  -+
                negs            r0, r0
                bx              r12
1:
                negs            r0, r0
                cmp             r1, #0
                bge             2f
                negs            r1, r1
                bl              __aeabi_uidivmod        // --   ->  +-
                negs            r1, r1
                bx              r12
2:
                bl              __aeabi_uidivmod        // -+   ->  --
                negs            r0, r0
                negs            r1, r1
                bx              r12
ENDF(   __aeabi_idiv     )

.end
