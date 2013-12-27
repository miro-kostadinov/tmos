/*
 * crc_f2.S
 *
 *  Created on: Dec 21, 2013
 *      Author: miro
 */

#include "port_asm.h"
#include "port_inc.h"
#include "crc_f2.h"
#include "mcu_cpp.h"

// unsigned int stm_crc32_le(const void* buf, unsigned int len)
FUNC(   stm_crc32_le     )
                push            {r4, r5, r6, lr}
                ldr             r2, =BASE_CRC
                movs            r3, #CRC_CR_RESET
                str             r3, [r2, #REG_CRC_CR-BASE_CRC]  //Reset CRC

                subs            r1, #16
                blo             2f

                lsls            r3, r0, #30
                bne             2f
1: // do 16
                ldmia           r0!, {r3-r6}
                rbit            r3, r3
                str             r3, [r2, #REG_CRC_DR-BASE_CRC]
                rbit            r4, r4
                str             r4, [r2, #REG_CRC_DR-BASE_CRC]
                rbit            r5, r5
                str             r5, [r2, #REG_CRC_DR-BASE_CRC]
                rbit            r6, r6
                str             r6, [r2, #REG_CRC_DR-BASE_CRC]
                subs            r1, #16
                bhs             1b
2:
                adds            r1, #12
                bmi             4f
3: // do 4
                ldr             r3, [r0], #4
                rbit            r3, r3
                str             r3, [r2, #REG_CRC_DR-BASE_CRC]
                subs            r1, #4
                bhs             3b
4:
                ldr             r3, [r2, #REG_CRC_DR-BASE_CRC]
                rbit            r3, r3
                adds            r1, #3
                bmi             6f
                adr             r2, crc32_4_le_table
5: // do 1
                ldrb            r4, [r0], #1
                eors            r3, r4
                and             r4, r3, #15
                ldr             r4, [r2, r4, lsl #2]
                eor             r3, r4, r3, lsr #4
                and             r4, r3, #15
                ldr             r4, [r2, r4, lsl #2]
                eor             r3, r4, r3, lsr #4

                subs            r1, #1
                bhs             5b
6:
                mvns            r0, r3
                pop             {r4, r5, r6, pc}
__ALIGN_LONG;
crc32_4_le_table:
    .word 0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac
    .word 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c
    .word 0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c
    .word 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c

ENDF(   stm_crc32_le     )

.end



