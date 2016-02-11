/*
 * crc16.S
 *
 *  Created on: Mar 7, 2012
 *      Author: miro
 */

#include "port_asm.h"


//-----------------------------------------------------------------------------
// unsigned int CalculateCRC16(unsigned int crc, const char * buf,
//        unsigned int len);
FUNC(   CalculateCRC16     )
                lsls            r0, #16
                cmp             r2, #0
                beq             4f
                adds            r2, r1
                mov             r12, r2
                ldr             r2, =0x80050000
1:
                ldrb            r3, [r1]
                adds            r1, #1
                lsls            r3, #24
                eors            r0, r3
                movs            r3, #8
2:
                lsls            r0, #1
                bcc             3f
                eors            r0, r2
3:
                subs            r3, #1
                bne             2b
                cmp             r1, r12
                bne             1b
4:
                lsrs            r0, #16
                bx              lr
ENDF(   CalculateCRC16 )

//-----------------------------------------------------------------------------
// unsigned int CalculateCRC16(unsigned int crc, const char * buf,
//        unsigned int len);
FUNC(   CalculateCRC16CCITT     )
                lsls            r0, #16
                cmp             r2, #0
                beq             4f
                adds            r2, r1
                mov             r12, r2
                ldr             r2, =0x10210000
1:
                ldrb            r3, [r1]
                adds            r1, #1
                lsls            r3, #24
                eors            r0, r3
                movs            r3, #8
2:
                lsls            r0, #1
                bcc             3f
                eors            r0, r2
3:
                subs            r3, #1
                bne             2b
                cmp             r1, r12
                bne             1b
4:
                lsrs            r0, #16
                bx              lr
ENDF(   CalculateCRC16CCITT )

.end
