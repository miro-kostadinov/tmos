/*
 * tmos_libc.S
 *
 *  Created on: Apr 26, 2012
 *      Author: miro
 */




#include "port_asm.h"
#include "port_inc.h"


//void __udiv_qrnnd_c(unsigned int q, unsigned int r, unsigned int n1,
//        unsigned int n0, unsigned int d)
//    unsigned int __d1, __d0, __q1, __q0;
//    unsigned int __r1, __r0, __m;
.macro      udiv_qrnnd      q0, rx, n1, n0, d,    q1, m

        lsrs        \m, \d, #16       //__d1 = __ll_highpart(d);

        udiv        \q1, \n1, \m      // __q1 = (n1) / __d1;
        mls         \rx, \m, \q1, \n1 // __r1 = (n1) % __d1

        lsrs        \m, \n0, #16
        orr         \rx, \m, \rx, lsl #16 // __r1 = __r1 * __ll_B | __ll_highpart(n0)

        uxth        \m, \d             //__d0 = __ll_lowpart(d)
        mul         \m, \q1            // m = (UWtype) __q1 * __d0

        cmp         \rx, \m            // if (__r1 < __m)
        bhs         8f

        subs        \q1, #1            // __q1--
        adds        \rx, \rx, \d       // __r1 += (d);
        bcs         8f                 // if (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */
        cmp         \rx, \m            // if (__r1 < __m)
        bhs         8f
        subs        \q1, #1            // __q1--
        adds        \rx, \rx, \d       // __r1 += (d);
8:
        subs        \rx, \m            // __r1 -= __m

        lsrs        \m, \d, #16       //__d1 = __ll_highpart(d);
        udiv        \q0, \rx, \m      // __q0 = __r1 / __d1;
        mls         \rx, \m, \q0, \rx // __r0 = __r1 % __d1

        uxth        \m, \n0
        orr         \rx, \m, \rx, lsl #16 // __r0 = __r0 * __ll_B | __ll_lowpart(n0)

        uxth        \m, \d             //__d0 = __ll_lowpart(d)
        mul         \m, \q0            // m = (UWtype) __q0 * __d0

        cmp         \rx, \m            // if (__r0 < __m)
        bhs         9f

        subs        \q0, #1            // __q0--
        adds        \rx, \rx, \d       // __r0 += (d);
        bcs         9f                 // if (__r0 >= (d)) /* i.e. we didn't get carry when adding to __r1 */
        cmp         \rx, \m            // if (__r0 < __m)
        bhs         9f
        subs        \q0, #1            // __q0--
        adds        \rx, \rx, \d       // __r0 += (d);
9:
        subs        \rx, \m            // __r0 -= __m

        //  (r) = __r0;
        // (q) = (UWtype) __q1 * __ll_B | __q0;
        adds        \q0, \q0, \q1, lsl #16
.endm


.macro  umul_ppmm   xh, xl, a, b
        umull       \xl, \xh, \a, \b
.endm


//*-----------------------------------------------------------------------------
//*         lonf long functions
//*-----------------------------------------------------------------------------

//unsigned long long udivdi3(unsigned long long u, unsigned long long v);
FUNC(   udivdi3     )

                cmp             r3, #0
                bne             tmos__udivmoddi4
                cmp             r2, #0
                bne             tmos__udivmoddi4
                cmp             r1, #0          // n < 0 ?
                it              eq
                cmpeq           r0, #0
                itt             ne
                movne           r1, #0xffffffff
                movne           r0, r1
                bx              lr

ENDF(   udivdi3     )


//long long divdi3(long long u, long long v);
FUNC(   divdi3     )

                b               __aeabi_ldivmod

ENDF(   divdi3     )


//unsigned long long llabs(long long u);
FUNC(   llabs     )

                cmp             r1, #0
                bge             1f
                negs            r0, r0                      // n < 0
                sbc             r1, r1, r1, lsl #1
1:              bx              lr

ENDF(   llabs     )

//------------------------------------------------------------------------------
// signed long long division and remainder, {q, r} = n / d
//
// A pair of (unsigned) long longs is returned in {{r0, r1}, {r2, r3}}, the
//   quotient in {r0, r1}, and the remainder in {r2, r3}
//
// __value_in_regs lldiv_t __aeabi_ldivmod(long long n, long long d)
FUNC(   __aeabi_ldivmod     )

                cbnz            r3, __tmos_ldivmod_helper   // d != 0 ?
                cbnz            r2, __tmos_ldivmod_helper

                cmp             r1, #0          // n < 0 ?
                it              eq
                cmpeq           r0, #0

                itt             lt
                movlt           r1, #0x80000000     // n<0 -> 0x80000000000
                movlt           r0, #0

                itt             gt
                movgt           r1, #0x7fffffff
                movgt           r0, #0xffffffff

//------------------------------------------------------------------------------
//  __aeabi_ldiv0
__aeabi_ldiv0:
                bx              lr

//------------------------------------------------------------------------------
// __value_in_regs lldiv_t __tmos_ldivmod_helper(long long n, long long d)
//FUNC(   __tmos_ldivmod_helper     )
__tmos_ldivmod_helper:

/*
  long long quotient;

  quotient = __divdi3 (a, b);
  *remainder = a - b * quotient;
  return quotient;
*/
//------------------------------------------------------------------------------
// long long __divdi3(long long n, long long d)
                cmp             r1, #0
                blt             1f
                cmp             r3, #0
                blt             2f
                b               tmos__udivmoddi4

1:
                negs            r0, r0                      // n < 0
                sbc             r1, r1, r1, lsl #1
                cmp             r3, #0
                bge             3f
                negs            r2, r2                      // n < 0, d <0
                sbc             r3, r3, r3, lsl #1
                b               tmos__udivmoddi4

2:  // n>=0 , d<0
                negs            r2, r2
                sbc             r3, r3, r3, lsl #1
3:
                push            {lr}
                bl              tmos__udivmoddi4
                negs            r0, r0
                sbc             r1, r1, r1, lsl #1
                negs            r2, r2
                sbc             r3, r3, r3, lsl #1
                pop             {pc}

//ENDF(   __tmos_ldivmod_helper     )

ENDF(   __aeabi_ldivmod     )



//------------------------------------------------------------------------------
// long long __gnu_ldivmod_helper (long long a, long long b,long long *remainder)
//
FUNC(   __gnu_ldivmod_helper     )
                push            {lr}
                bl              __tmos_ldivmod_helper
                mov             r12, r3
                ldr             r3, [sp, #4]           //*reminder
                cbz             r3, 1f
                stmia           r3, {r2, r12}
1:
                pop             {pc}

ENDF(   __gnu_ldivmod_helper     )


//------------------------------------------------------------------------------
// __value_in_regs lldiv_t tmos__udivmoddi4(long long n, long long d)           r0 r1 r2 r3 r4 r5 r6 r7  r8 r9 r12
FUNC(   tmos__udivmoddi4     )
                push            {r4-r9}         //                              n0 n1 d0 d1 __ __ __ __  __ __ __
                cmp             r3, #0          //  if (d1 == 0)
                bne             4f
                clz             r4, r2          // count_leading_zeros(bm, d0); n0 n1 d0 d1 bm __ __ __  __ __ __
                cmp             r2, r1          //      if (d0 > n1)
                bls             2f
                movs            r7, #0
                cbz             r4, 1f              // if (bm != 0)
                /* Normalize, i.e. make the most significant bit of the
                 denominator set.  */
                lsls            r2, r4          // d0 = d0 << bm;               n0 n1 d0 d1 bm __ __ __  __ __ __
                rsb             r5, r4, #32     //  (W_TYPE_SIZE - bm)          __ __ __ __ __ xx __ __  __ __ __
                lsrs            r5, r0, r5      //  (n0 >> (W_TYPE_SIZE - bm))
                lsls            r1, r4          // n1 = (n1 << bm) |
                orrs            r1, r5          // n1 = (n1 << bm) | (n0 >> (W_TYPE_SIZE - bm));
                lsls            r0, r4          // n0 = n0 << bm                n0 n1 d0 d1 __ __ __ __  __ __ __

1:              //udiv_qrnnd   (q0, n0,   n1, n0, d0);                          n0 n1 d0 __ bm __ __ q1  __ __ __
                udiv_qrnnd      r6, r5,   r1, r0, r2,   r3, r8  //              n0 n1 d0 __ bm n0 q0 q1  __ __ __

                mov             r0, r6          //q0                            __ __ __ __ bm n0 q0 q1  __ __ __
                movs            r1, r7          //q1
                lsrs            r2, r5, r4      /* Remainder in n0 >> bm.  */
                movs            r3, #0
                pop             {r4-r9}
                bx              lr

2:                                             //                               r0 r1 r2 r3 r4 r5 r6 r7  r8 r9 r12
                cmp             r4, #0           //if (bm == 0)                 n0 n1 d0 d1 bm __ __ __  __ __ __
                beq             3f
                /* Normalize.  */
                rsb             r5, r4, #32     //  b = W_TYPE_SIZE - bm;       n0 n1 d0 d1 bm _b __ __  __ __ __
                lsls            r2, r4          // d0 = d0 << bm;
                lsrs            r6, r1, r5      // n2 = n1 >> b;                n0 n1 d0 d1 bm _b n2 __  __ __ __

                lsrs            r5, r0, r5      //  (n0 >> b)                   n0 n1 d0 d1 bm __ n2 __  __ __ __
                lsls            r1, r4          //  (n1 << bm)
                orr             r1, r5          // n1 = (n1 << bm) | (n0 >> b);
                lsls            r0, r4          // n0 = n0 << bm

                //udiv_qrnnd    (q1, n1,  n2, n1, d0);
                udiv_qrnnd      r7, r5,   r6, r1, r2,   r8, r9 //               n0 n1 d0 d1 bm n1 n2 q1  __ __ __

                //udiv_qrnnd    (q0, n0,  n1, n0, d0);
                mov             r1, r5          //                              n0 n1 d0 d1 bm __ __ q1  __ __ __
                b               1b

3:                                              //                              n0 n1 d0 d1 bm __ __ __  __ __ __
                /* From (n1 >= d0) /\ (the most significant bit of d0 is set),
                 conclude (the most significant bit of n1 is set) /\ (the
                 leading quotient digit q1 = 1).

                 This special case is necessary, not an optimization.
                 (Shifts counts of W_TYPE_SIZE are undefined.)  */

                subs            r1, r2          //n1 -= d0;
                movs            r7, #1
                b               1b


4:
                cmp             r3, r1          // if (d1 > n1)
                bls             6f
5:                                              //                              n0 n1 d0 d1 bm __ __ __  __ __ __
                mov             r2, r0          //r0
                mov             r3, r1          //r1
                movs            r0, #0          //q0    q0 = 0;
                movs            r1, #0          //q1    q1 = 0;
                pop             {r4-r9}
                bx              lr              //                              r0 r1 r2 r3 r4 r5 r6 r7  r8 r9 r12

6:
                clz             r4, r3          // count_leading_zeros(bm, d1); n0 n1 d0 d1 bm __ __ __  __ __ __
                cbnz            r4, 1f          // if (bm == 0)
                /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
                 conclude (the most significant bit of n1 is set) /\ (the
                 quotient digit q0 = 0 or 1).

                 This special case is necessary, not an optimization.  */

                /* The condition on the next line takes advantage of that
                 n1 >= d1 (true due to program flow).  */
                cmp             r1, r3      //if (n1 > d1 || n0 >= d0)
                bhi             7f
                cmp             r0, r2
                blo             5b
7:
                subs            r2, r0, r2  //sub_ddmmss(n1, n0, n1,n0, d1,d0);
                sbc             r3, r1, r3
                movs            r0, #1          //q0    q0 = 1;
                movs            r1, #0          //q1    q1 = 0;
                pop             {r4-r9}
                bx              lr


1:
                /* Normalize.  */               //                              r0 r1 r2 r3 r4 r5 r6 r7  r8 r9 r12
                rsb             r5, r4, #32     //  b = W_TYPE_SIZE - bm;       n0 n1 d0 d1 bm _b __ __  __ __ __

                lsls            r3, r4          //  (d1 << bm)
                lsrs            r7, r2, r5      //  (d0 >> b)                   n0 n1 d0 d1 bm _b __ xx  __ __ __
                orrs            r3, r7          // d1 = (d1 << bm) | (d0 >> b)

                lsls            r2, r4          // d0 = d0 << bm
                lsrs            r7, r1, r5      // n2 = n1 >> b                 n0 n1 d0 d1 bm _b __ n2  __ __ __
                lsls            r1, r4          //  (n1 << bm)
                lsrs            r6, r0, r5      //  (n0 >> b)
                orrs            r1, r6          // n1 = (n1 << bm) | (n0 >> b);
                lsls            r0, r4          // d0 = d0 << bm

                //udiv_qrnnd    (q0, n1, n2, n1, d1);
                udiv_qrnnd      r6, r8,  r7, r1, r3,   r9, r12      //          n0 n1 d0 d1 bm _b q0 n2  n1 __ __
                                                                    //          n0 __ d0 d1 bm _b q0 __  n1 __ __
                //umul_ppmm    (m1, m0, q0, d0);
                umul_ppmm       r1, r7, r6, r2                      //          n0 m1 d0 d1 bm _b q0 m0  n1 __ __

                //if (m1 > n1 || (m1 == n1 && m0 > n0))
                cmp             r1, r8          // m1 > n1 ?
                bhi             2f
                bne             3f
                cmp             r7, r0          // m0 > n0 ?
                bls             3f
2:
                subs            r6, #1          // q0--
                //sub_ddmmss(m1, m0, m1, m0, d1, d0);
                subs            r7, r2
                sbcs            r1, r3
3:                                                                  //          n0 m1 __ __ bm _b q0 m0  n1 __ __

                // sub_ddmmss(n1, n0, n1, n0, m1, m0);
                subs            r0, r7
                sbcs            r1, r8, r1                          //          n0 n1 __ __ bm _b q0 __  __ __ __
                // rr.s.low = (n1 << b) | (n0 >> bm);
                lsls            r2, r1, r5
                lsrs            r7, r0, r4
                orrs            r2, r7

                // rr.s.high = n1 >> bm;
                lsrs            r3, r1, r4
                mov             r0, r6
                movs            r1, #0
                pop             {r4-r9}
                bx              lr

ENDF(   tmos__udivmoddi4     )


//*-----------------------------------------------------------------------------
//*         Workaround __value_in_regs functions
//*-----------------------------------------------------------------------------

//int64_t tmos_ldivmod(int64_t u, int64_t v, int64_t* rem);
FUNC(   tmos_ldivmod     )
                push            {lr}
                bl              __aeabi_ldivmod
                ldr             r12, [sp, #4]
                cmp             r12, #0
                beq             1f
                strd            r2, r3, [r12]
1:
                pop             {pc}

ENDF(   tmos_ldivmod     )


//uint64_t tmos_uldivmod(uint64_t u, uint64_t v, uint64_t* res);
FUNC(   tmos_uldivmod     )
                push            {lr}
                bl              udivdi3
                ldr             r12, [sp, #4]
                cmp             r12, #0
                beq             1f
                strd            r2, r3, [r12]
1:
                pop             {pc}

ENDF(   tmos_uldivmod     )


//*-----------------------------------------------------------------------------
//*         64-bit shifts
//*-----------------------------------------------------------------------------
// long long __aeabi_llsr(long long num, int shift)
FUNC(   __aeabi_llsr     )
                rsb             r12,r2,#32
                subs            r3,r2,#32
                iteee           pl
                lsrpl           r0,r1,r3
                lsrmi           r0,r2
                lslmi           r3,r1,r12
                orrmi           r0,r0,r3
                lsrs            r1,r2
                bx              lr
ENDF(   __aeabi_llsr     )

//long long __aeabi_llsl(long long, int)
FUNC(   __aeabi_llsl     )
                rsb             r12,r2,#32
                subs            r3,r2,#32
                iteee           pl
                lslpl           r1,r0,r3
                lslmi           r1,r2
                lsrmi           r3,r0,r12
                orrmi           r1,r3
                lsls            r0, r2
                bx              lr
ENDF(   __aeabi_llsl     )

//long long __aeabi_lasr(long long, int)
FUNC(   __aeabi_lasr     )

                rsb             r12,r2,#32
                subs            r3,r2,#32
                iteee           pl
                asrpl           r0,r1,r3
                lsrmi           r0,r2
                lslmi           r3,r1,r12
                orrmi           r0,r0,r3
                asrs            r1, r2
                bx              lr
ENDF(   __aeabi_lasr     )


.end
