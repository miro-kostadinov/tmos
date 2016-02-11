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
                cbz             r2, _exit
                lsls            r0, #16
                mov             r12, r2
                movs            r2, #0
                movt            r2, #0x8005
1:
                ldrb            r3, [r1], #1
                eor             r0, r0, r3, lsl #24

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                subs            r12, #1
                bne             1b

                lsrs            r0, #16
_exit:
                bx              lr
ENDF(   CalculateCRC16 )

//-----------------------------------------------------------------------------
// unsigned int CalculateCRC16(unsigned int crc, const char * buf,
//        unsigned int len);
FUNC(   CalculateCRC16CCITT     )
                cbz             r2, 2f
                lsls            r0, #16
                mov             r12, r2
                movs            r2, #0
                movt            r2, #0x1021
1:
                ldrb            r3, [r1], #1
                eor             r0, r0, r3, lsl #24

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                lsls            r0, #1
                IT              CS
                eorcs           r0, r2

                subs            r12, #1
                bne             1b

                lsrs            r0, #16
2:
                bx              lr
ENDF(   CalculateCRC16CCITT )

.end
