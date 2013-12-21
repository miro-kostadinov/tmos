/*
 * crc32.S
 *
 *  Created on: Dec 19, 2013
 *      Author: miro
 */


#include "port_asm.h"
#include "port_inc.h"

// unsigned int CalculateCRC32Slow(unsigned int crc, const void * buf,
//        unsigned int len);
// NOTE: this function is for reference (it's too slow)
FUNC(   CalculateCRC32Slow     )
                mov             r12, r2
                ldr             r3, = 0xEDB88320
1:
                ldrb            r2,[r1],#1
                eor             r0, r2

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                lsrs            r0, #1
                IT              CS
                eorcs           r0, r3

                subs            r12, #1
                bne             1b

                bx              lr

ENDF(   CalculateCRC32Slow     )

// unsigned int crc32_4_le(const void* buf, unsigned int len)
FUNC(   crc32_4_le     )

                adr             r12, crc32_4_le_table
                mov             r3, #0xFFFFFFFF
                subs            r1, #4
                blo             2f

1: // do 4
                ldr             r2, [r0], #4
                eors            r3, r2

                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4

                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4

                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4

                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4

                subs            r1, #4
                bhs             1b
2:
                adds            r1, #3
                bmi             4f
3: // do 1
                ldrb            r2, [r0], #1
                eors            r3, r2

                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                and             r2, r3, #15
                ldr             r2, [r12, r2, lsl #2]
                eor             r3, r2, r3, lsr #4
                subs            r1, #1
                bhs             3b
4:
                mvns            r0, r3
                bx              lr
__ALIGN_LONG;
crc32_4_le_table:
    .word 0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac
    .word 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c
    .word 0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c
    .word 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c

ENDF(   crc32_4_le     )

.end


