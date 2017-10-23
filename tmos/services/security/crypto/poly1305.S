/*
 * poly1305.S
 *
 *  Created on: Jun 19, 2017
 *      Author: miro
 */
#include "port_asm.h"

//*----------------------------------------------------------------------------
//*     void poly1305_ctxt_process_block(uint32_t* acc_r, uint8_t* buf)
//*----------------------------------------------------------------------------
FUNC(   poly1305_ctxt_process_block      )
                push            {r0,r4-r9, lr}              // r0   r1  r2  r3  r4  r5  r6  r7  r8  r9  r12 lr

                ldmia           r0!, {r3-r7, r8-r9,r12, lr} //      buf     a0  a1  a2  a3  a4  x0  x1  x2  x3
                ldr             r2, [r1]
                adds            r3, r2          // a0 = acc0 + buf[0]
                ldr             r2, [r1, #4]
                adcs            r4, r2          // a1 = acc1 + buf[1] + c
                ldr             r2, [r1, #8]
                adcs            r5, r2          // a2 = acc2 + buf[2] + c
                ldr             r2, [r1, #12]
                adcs            r6, r2          // a3 = acc3 + buf[4] + c
                adcs            r7, #0          // a4 = acc4 + c
                                                            // r0   r1  r2  r3  r4  r5  r6  r7  r8  r9  r12 lr
//    acc[0] = a[0] * r[0];
                umull           r0, r1, r3, r8              // acc0 c       a0  a1  a2  a3  a4  x0  x1  x2  x3
                movs            r2, #0
//    acc[1] = a[0] * r[1] + a[1] * r[0];
                umlal           r1, r2, r2, r9  // c+a0*r1
                umlal           r1, r2, r4, r8  // +a1*r0
                push            {r0, r1}        // acc0, acc1| acc0 acc1 c  a0  a1  a2  a3  a4  x0  x1  x2  x3
//    acc[2] = a[0] * r[2] + a[1] * r[1] + a[2] * r[0];
                movs            r0, #0
                umlal           r2, r0, r3, r12 // c+a0*r2
                umlal           r2, r0, r4, r9  // +a1*r1
                umlal           r2, r0, r5, r8  // +a2*r0  |    c      acc2 a0  a1  a2  a3  a4  x0  x1  x2  x3

//    acc[3] = a[0]*r[3] + a[1]*r[2] + a[2]*r[1] + a[3]*r[0];
                movs            r1, #0
                umlal           r0, r1, r3, lr  // c+a0*r3
                umlal           r0, r1, r4, r12 // + a1*r2
                umlal           r0, r1, r5, r9  // + a2*r1
                umlal           r0, r1, r6, r8  // + a3*r0 |   acc3 c  acc2     a1  a2  a3  a4  x0  x1  x2  x3

//    acc[4] = a[1]*r[3] + a[2]*r[2] + a[3]*r[1] + a[4]*r[0];
                movs            r3, #0
                umlal           r1, r3, r4, lr  // c+a1*r3
                umlal           r1, r3, r5, r12 // + a2*r2
                umlal           r1, r3, r6, r9  // + a3*r1
                umlal           r1, r3, r7, r8  // + a4*r0 |   ac3 ac4 ac2   c      a2  a3  a4  x0  x1  x2  x3

//    acc[5] = a[2]*r[3] + a[3]*r[2] + a[4]*r[1];
                movs            r4, #0
                umlal           r3, r4, r5, lr  // c+a2*r3
                umlal           r3, r4, r6, r12 // + a3*r2
                umlal           r3, r4, r7, r9  // + a4*r1 |   ac3 ac4 ac2  ac5  c      a3  a4  x0  x1  x2  x3

//    acc[6] = a[3]*r[3] + a[4]*r[2];
                movs            r5, #0
                umlal           r4, r5, r6, lr  // c+a3*r3
                umlal           r4, r5, r7, r12 // + a4*r2 |   ac3 ac4 ac2  ac5 ac6  c  a3  a4  x0  x1  x2  x3

//    acc[7] = a[4]*r[3];
                umlal           r5, r6, r7, lr  // c+a4*r3 |   ac3 ac4 ac2  ac5 ac6 ac7

//    a[0] = acc[4] & 0xFFFFFFFC;
//    a[1] = acc[5] & 0xFFFFFFFF;
//    a[2] = acc[6] & 0xFFFFFFFF;
//    a[3] = acc[7] & 0xFFFFFFFF;
                lsr             r6, r1, #2
                and             r9, r1, #3
                pop             {r7, r8, r12}               // ac3      ac2 a1  a2  a3 a0>2 ac0 ac1 ac4
                                                            // r0   r1  r2  r3  r4  r5  r6  r7  r8  r9  r12 lr
//1    acc[0] += a[0];
//2    acc[0] += (a[0] >> 2) | (a[1] << 30);
//3    acc[1] += a[1];
//4    acc[1] += (a[1] >> 2) | (a[2] << 30);
//5    acc[2] += a[2];
//6    acc[2] += (a[2] >> 2) | (a[3] << 30);
//7    acc[3] += a[3];
//8    acc[3] += (a[3] >> 2);

                adds            r7, r7, r6, lsl #2 //1 acc[0] += a[0]
                adcs            r1, r8, r3         //3 acc[1] += a[1]
                adcs            r2, r4             //5 acc[2] += a[2]
                adcs            r8, r0, r5         //7 acc[3] += a[3]
                adcs            r9, #0
                                                            //      ac1 ac2 a1  a2  a3 a0>2 ac0 ac3 ac4
                                                            // r0   r1  r2  r3  r4  r5  r6  r7  r8  r9  r12 lr
                orr             r0, r6, r3, lsl #30    // (a[0] >> 2) | (a[1] << 30)
                                                            // ac0  ac1 ac2 a1  a2  a3      ac0 ac3 ac4
                lsrs            r3, #2
                orr             r6, r3, r4, lsl #30    // (a[1] >> 2) | (a[2] << 30)
                                                            // ac0  ac1 ac2     a2  a3  ac1 ac0 ac3 ac4
                lsrs            r4, #2
                orr             r4, r4, r5, lsl #30    // (a[2] >> 2) | (a[3] << 30)

                adds            r0, r7                 //2     ac0  ac1 ac2     ac2 a3  ac1     ac3 ac4
                adcs            r1, r6                 //4     ac0  ac1 ac2     ac2 a3          ac3 ac4
                adcs            r2, r4                 //6     ac0  ac1 ac2         a3          ac3 ac4
                adcs            r3, r8, r5, lsr #2     //8     ac0  ac1 ac2 ac3                     ac4
                adcs            r4, r9, #0             //      ac0  ac1 ac2 ac3 ac4

//  a[0] = acc[4] & 0xFFFFFFFC;
//  acc[0] += a[0];
                lsrs            r5, r4, #2
                adds            r0, r0, r5, lsl #2
                adcs            r1, #0
                adcs            r2, #0
                adcs            r3, #0
                adcs            r4, #0

//  acc[0] += a[0] >> 2;
                adds            r0, r5
                adcs            r1, #0
                adcs            r2, #0
                adcs            r3, #0
                adcs            r4, #0

                and             r4, #3
                stmia           r12!, {r0-r4}

                pop             {r4-r9, pc}

ENDF(   poly1305_ctxt_process_block      )


//*----------------------------------------------------------------------------
//*     void poly1305_ctxt_result(uint32_t* acc_r, uint8_t *tag)
//*----------------------------------------------------------------------------
FUNC(   poly1305_ctxt_result      )
                push            {r4-r8, lr}

                                                            // r0   r1  r2  r3  r4  r5  r6  r7  r8  r12 lr
                ldmia           r0!, {r2-r6}                // ctx  out b0  b1  b2  b3, ac4 ac0 ac1 ac2 ac3
                adds            r7, r2, #5          // acc[0] += 5;
                adcs            r8, r3, #0          // acc[1]
                adcs            r12, r4, #0         // acc[2]
                adcs            lr, r5, #0          // acc[3]
                adcs            r6, #0              // acc[4]
                adds            r0, #16             // skip r[]
                cmp             r6, #4
                ITE         HS
                ldmiahs         r0!, {r2-r5}
                ldmialo         r0!, {r7, r8, r12, lr}

                adds            r2, r7
                adcs            r3, r8
                adcs            r4, r12
                adcs            r5, lr
                str             r2, [r1]
                str             r3, [r1, #4]
                str             r4, [r1, #8]
                str             r5, [r1, #12]
                pop             {r4-r8, pc}

ENDF(   poly1305_ctxt_result      )

.end
