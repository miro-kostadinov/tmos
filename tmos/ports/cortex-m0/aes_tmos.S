//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2010 Informa Ltd.
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
// Date:          2010-01-13
// Description:   AES ECB decrypt
//
//
//========================================================================
#include "port_asm.h"
#include "aes_tmos.h"

/*
	Notes:

	Block size (ENCRYPTION_BLOCK_LENGTH) is hard coded to 16

	typedef struct
	{
	    int nr;                     //  must be first element
	    unsigned buf[AESCONSZ];     //  key after processing for rounds
	} aes_context;
*/

//-----------------------------------------------------------------------------
//void aes_tmos_de_ecb(const aes_context* ctxt, unsigned char* input,
//		unsigned char* output, unsigned blocks);

FUNC(	aes_tmos_de_ecb		)

#define tmp2	r0
#define	lut		r1
#define tmp 	r2
#define Y0		r3

#define X0		r4
#define X1		r5
#define X2		r6
#define X3		r7


#define DE_ECB_STACK_KEY    0
#define DE_ECB_STACK_KEYC   4
#define DE_ECB_STACK_RNDSC  8
#define DE_ECB_STACK_RNDS   12
#define DE_ECB_STACK_IN     16
#define DE_ECB_STACK_OUT    20
#define DE_ECB_STACK_BLKS   24

    // 0 -
                                                                              //     lut tmp Y0  X0  X1  X2  X3
                                                                              // r0  r1  r2  r3  r4  r5  r6  r7
                                                                              // ctx in  out blk *   *   *   *

	push	{r1-r7}
    ldr     lut, [r0, #AES_CONTEXT_RT] //lut
    mov     r12, r0
    ldr     r4, [r0]                   //rnds
    adds    r4, #5
    lsls    r3, r4, #4
    adds    r0, r3
    adds    r0, #AES_CONTEXT_KEY +32   //key = key[rnd+6+1]  (r0+key + rnd*16 +32)
    mov     r2, r0
    mov     r3, r4
    push    {r0, r2, r3, r4}

    // 0 -rnds

1: // Decript loop
    ldr     tmp, [sp, #DE_ECB_STACK_IN]
	ldmia	tmp!, {X0, X1, X2, X3}		// Copy cipher text block
	str		tmp, [sp, #DE_ECB_STACK_IN]

    ldr     tmp, [sp, #DE_ECB_STACK_RNDS]
    str     tmp, [sp, #DE_ECB_STACK_RNDSC]

    ldr     tmp, [sp, #DE_ECB_STACK_KEY]
    subs    tmp, #16
    str     tmp, [sp, #DE_ECB_STACK_KEYC]

    ldmia   tmp, {r0, r1, r2, r3}
    eors    X0, r0
    eors    X1, r1
    eors    X2, r2
    eors    X3, r3
    mov     lut, r12
    ldr     lut, [lut, #AES_CONTEXT_RT]

2:	//block loop			key, lut, rnd, X0-X3
	// 1. column
	uxtb 	tmp, X0 					// Y0 =  RTO[ ( X0       ) & 0xFF ]
	lsls    tmp, #2
	ldr 	Y0, [lut, tmp]

	lsrs 	tmp, X3, #8 	    		// Y0 ^= RT1[ ( X3 >>  8 ) & 0xFF ]
	uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
	lsrs    tmp2, tmp, #24
	lsls    tmp, #8
	orrs    tmp, tmp2
	eors    Y0, tmp

    lsrs    tmp, X2, #16                // Y0 ^= RT2[ ( X2 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
	rev16   tmp, tmp
	rev     tmp, tmp
	eors 	Y0, Y0, tmp

	lsrs 	tmp, X1, #24 				// Y0 ^= RT3[ ( X1 >>  24 ) & 0xFF ]
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp

    push    {Y0, X0}

	// 2. column
	uxtb 	tmp, X1 					// Y1 =  RT0[ ( X1        ) & 0xFF ]
    lsls    tmp, #2
	ldr 	Y0, [lut, tmp]

	lsrs 	tmp, X0, #8       			// Y1 ^= RT1[ ( X0 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    Y0, tmp

	lsrs 	tmp, X3, #16	 	       	// Y1 ^= RT2[ ( X3 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    Y0, Y0, tmp

	lsrs 	tmp, X2, #24 				// Y1 ^= RT3[ ( X2 >>  24 ) & 0xFF ]
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp

    str     Y0, [sp, #4]


	// 3. column
	uxtb 	tmp, X2 					// Y2 =  RT0[ ( X2        ) & 0xFF ]
    lsls    tmp, #2
	ldr 	Y0, [lut, tmp]

	lsrs 	tmp, X1, #8       			// Y2 ^= RT1[ ( X1 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    Y0, tmp

	lsrs 	tmp, X0, #16     			// Y2 ^= RT2[ ( X0 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    Y0, tmp

	lsrs 	tmp, X3, #24 				// Y2 ^= RT3[ ( X3 >>  24 ) & 0xFF ]
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp


	// 4. column
	uxtb 	tmp, X3 					// X3 =  RT0[ ( X3        ) & 0xFF ]
    lsls    tmp, #2
	ldr 	X3, [lut, tmp]

	lsrs 	tmp, X2, #8       			// X3 ^= RT1[ ( X2 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
	eors 	X3, tmp

	lsrs 	tmp, X1, #16      			// X3 ^= RT2[ ( X1 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
	eors 	X3, tmp

	lsrs 	tmp, X0, #24 				// X3 ^= RT3[ ( X0 >>  24 ) & 0xFF ]
    lsls    tmp, #2
	ldr 	tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
	eors 	X3, tmp

	// add round key
    ldr     tmp, [sp, #DE_ECB_STACK_KEYC+8]
    subs    tmp, #16
    str     tmp, [sp, #DE_ECB_STACK_KEYC+8]
    ldmia   tmp, {tmp2, tmp, X0, X2}
    eors    X3, X2                      //X3 ^= key[3]
    mov     X2, Y0
    eors    X2, X0                      //X2 = Y2 ^ key[2]
    pop     {X0, X1}
    eors    X0, tmp2                    //X0 = Y0 ^ key[0]
    eors    X1, tmp                     //X1 = Y1 ^ key[1]

	// decrement counter
    ldr     tmp, [sp, #DE_ECB_STACK_RNDSC]
    subs    tmp, #1     // do all the rounds,
    str     tmp, [sp, #DE_ECB_STACK_RNDSC]
	bne 	2b		 	// except the last one


	// last round
    mov     lut, r12
    ldr     lut, [lut, #AES_CONTEXT_RSB]

	// 1. column
	uxtb 	tmp, X0 					// Y0 =  FSb[ ( X0       ) & 0xFF ]
	ldrb 	Y0, [lut, tmp]

	lsrs 	tmp, X3, #8      			// Y0 |= FSb[ ( X3 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
	lsls    tmp, #8
	orrs 	Y0, tmp

	lsrs 	tmp, X2, #16 		      	// Y0 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #16
    orrs    Y0, tmp

	lsrs 	tmp, X1, #24
	ldrb 	tmp, [lut, tmp]				// Y0 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
    orrs    Y0, tmp

    push    {Y0, X0}

	// 2. column
	uxtb 	tmp, X1 					// Y1 =  FSb[ ( X1       ) & 0xFF ]
	ldrb 	Y0, [lut, tmp]

	lsrs 	tmp, X0, #8	     	 		// Y1 |= FSb[ ( X0 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #8
	orrs 	Y0, tmp

	lsrs 	tmp, X3, #16      			// Y1 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #16
	orrs	Y0, tmp

	lsrs 	tmp, X2, #24
	ldrb 	tmp, [lut, tmp]				// Y1 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
	orrs 	Y0, tmp

    str     Y0, [sp, #4]

	// 3. column
	uxtb 	tmp, X2 					// Y2 =  FSb[ ( X2       ) & 0xFF ]
	ldrb	Y0, [lut, tmp]

	lsrs 	tmp, X1, #8       			// Y2 |= FSb[ ( X1 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #8
	orrs 	Y0, tmp

	lsrs 	tmp, X0, #16     			// Y2 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #16
	orrs 	Y0, tmp

	lsrs 	tmp, X3, #24
	ldrb 	tmp, [lut, tmp]				// Y2 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
	orrs 	Y0, tmp

	// 4. column
	uxtb 	tmp, X3 					// X3 =  FSb[ ( X3       ) & 0xFF ]
	ldrb 	X3, [lut, tmp]

	lsrs 	tmp, X2, #8       			// X3 |= FSb[ ( X2 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #8
	orrs 	X3, tmp

	lsrs 	tmp, X1, #16 		      	// X3 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
	ldrb 	tmp, [lut, tmp]
    lsls    tmp, #16
	orrs 	X3, tmp

	lsrs 	tmp, X0, #24
	ldrb 	tmp, [lut, tmp]				// X3 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
	orrs 	X3, tmp

	// add round key
    ldr     tmp, [sp, #DE_ECB_STACK_KEYC+8]
    subs    tmp, #16
    ldmia   tmp, {tmp2, tmp, X0, X2}
    eors    X3, X2                      //X3 ^= key[3]
    mov     X2, Y0
    eors    X2, X0                      //X2 = Y2 ^ key[2]
    pop     {X0, X1}
    eors    X0, tmp2                    //X0 = Y0 ^ key[0]
    eors    X1, tmp                     //X1 = Y1 ^ key[1]

    ldr     tmp, [sp, #DE_ECB_STACK_OUT]
    stmia   tmp!, {X0-X3}
    ldr     tmp2, [sp, #DE_ECB_STACK_BLKS]
    subs    tmp2, #1
    bne     3f
    add     sp, #DE_ECB_STACK_BLKS+4
    pop     {r4-r7}
    bx      lr
3:
    str     tmp, [sp, #DE_ECB_STACK_OUT]
    str     tmp2, [sp, #DE_ECB_STACK_BLKS]
    b       1b                          // Decript loop

#undef tmp2
#undef lut
#undef tmp
#undef Y0

#undef X0
#undef X1
#undef X2
#undef X3


ENDF(	aes_tmos_de_ecb	)

//-----------------------------------------------------------------------------
//void aes_tmos_en_key(const aes_context* ctxt, unsigned char* input, unsigned key_size);

// expands aes encryption key
//   key size - 4: 128, 6: 192, 8: 256

FUNC(	aes_tmos_en_key		)
// void acl_aes_key_en(vect key_out, vect key_in, size_t key_size);


#define out		r0      // outputs
#define in      r1      // inputs
#define nk      r2      // nk
#define tmp     r3      // temp
#define cnt     r4      // loop counter
#define sbox    r5      // pointer to sbox
#define st      r6      // substitution
#define acc     r7      // accumulator
#define rnd     r12     // round counter
#define rcon    r1      // pointer to rcon

    push    {r4-r7}
	ldr		sbox, [r0, #AES_CONTEXT_SB]
	str		r2, [r0, #AES_CONTEXT_RNDS]
	adds	r0, #AES_CONTEXT_KEY		//out

	// copy key from in to out
    mov     cnt, nk
1:
	ldmia	in!,  {tmp}
    stmia   out!, {tmp}
    subs    cnt, #1
    bne     1b
    adr     rcon, _rcon_table_

    // number of rounds = 3*nk + 28 (cnt==0)
    lsls    tmp, nk, #1
    adds    tmp, nk
    adds    tmp, #28
    mov     rnd, tmp

    // rnd mod nk == 0
2:
    movs    cnt, #0
    subs    tmp, out, #4
	ldr     st, [tmp]			       	// st = key[i*nk +nk-1]

    lsrs    tmp, st, #8
    uxtb    tmp, tmp
    ldrb    acc, [sbox, tmp]			//acc = FSb[ ( st >>  8 ) & 0xFF ]

    lsrs    tmp, st, #16
    uxtb    tmp, tmp
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, #8
    orrs    acc, tmp            		//acc ^= FSb[ ( st >> 16 ) & 0xFF ] << 8

	lsrs	tmp, st, #24
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, #16
    orrs    acc, tmp            		//acc ^= FSb[ ( st >> 24 ) & 0xFF ] << 16

    uxtb    tmp, st
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, 24
    orrs    acc, tmp            		//acc ^= FSb[ ( st  ) & 0xFF ] << 24

    ldrb    tmp, [rcon]
    adds    rcon, #1
    eors    acc, tmp					//acc ^= RCON[i]

3:	//drain
    lsls    tmp, nk, #2
    subs    tmp, out, tmp
    ldr     tmp, [tmp]
	eors    acc, tmp					//acc ^= key[i*nk]
    stmia 	out!, {acc}					//key[(i+1)*nk] = acc

    mov     tmp, rnd
    subs    tmp, #1
    mov     rnd, tmp
    beq     9f	//done

    adds    cnt, #1
    cmp     cnt, nk
    beq     2b
	cmp     cnt, #4
    bne     3b	//drain
	cmp     nk, #6
    bls     3b  //drain

    // (rnd mod nk == 4) and (nk > 6)
    subs    tmp, out, #4
    ldr     st, [tmp]
    uxtb    tmp, st
    ldrb    acc, [sbox, tmp]            //acc = FSb[ ( st ) & 0xFF ]

    lsrs    tmp, st, #8
    uxtb    tmp, tmp
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, #8
    orrs    acc, tmp                    //acc ^= FSb[ ( st >> 8 ) & 0xFF ] << 8

    lsrs    tmp, st, #16
    uxtb    tmp, tmp
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, #16
    orrs    acc, tmp                    //acc ^= FSb[ ( st >> 16 ) & 0xFF ] << 16

	lsrs    tmp, st, #24
    ldrb    tmp, [sbox, tmp]
    lsls    tmp, #24
    orrs    acc, tmp                    //acc ^= FSb[ ( st >> 24 ) & 0xFF ] << 24
    b       3b  //drain


9: //done
    pop     {r4-r7}
	bx      lr
    nop

_rcon_table_:
	.byte 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36

#undef out
#undef in
#undef nk
#undef tmp
#undef cnt
#undef sbox
#undef st
#undef acc
#undef rnd
#undef rcon
 ENDF(	aes_tmos_en_key	)

//-----------------------------------------------------------------------------
//void aes_tmos_de_key(const aes_context* ctxt, unsigned char* input, unsigned key_size);

FUNC(	aes_tmos_de_key		)

// void acl_aes_key_de(vect key_out, vect key_in, size_t key_size);
//   expands aes decryption key key_in to key_out
//   assuming a little endian processor
// on entry:
//   r0 = pointer to expanded key (output)
//   r1 = pointer to key (input)
//   r2 = key size - 4: 128, 6: 192, 8: 256 (use constants ACL_xxx)

out             .req    r0      // expanded key
acc             .req    r1      // accumulator
nk              .req    r2      // nk
tmp             .req    r3      // temp
st0             .req    r4      // temp
st1             .req    r5      // temp
sbox            .req    r6      // pointer to sbox
Rtab 		    .req    r7      // pointer to inverse table

	push    {r0, r2, r14}
    bl      aes_tmos_en_key
    pop     {r0, r2, tmp}
    mov     lr, tmp

	push    {r4-r7}

	ldr		sbox, [r0, #AES_CONTEXT_SB]
	ldr		Rtab, [r0, #AES_CONTEXT_RT]
	adds	r0, #AES_CONTEXT_KEY		//out
    lsls    nk, #2
    adds 	nk, #20
    adds    out, #16
1:
	ldr     tmp, [out]
    uxtb    st0, tmp
    lsrs    st1, tmp, #8
    uxtb    st1, st1
    ldrb    st0, [sbox, st0]
    ldrb    st1, [sbox, st1]
    lsls    st0, #2
    ldr     acc, [Rtab, st0]
    lsls    st1, #2
    ldr     st1, [Rtab, st1]
    lsrs    st0, st1, #24
    lsls    st1, #8
    orrs    st1, st0
    eors    acc, st1
    lsrs    st0, tmp, #16
    uxtb    st0, st0
    lsrs    st1, tmp, #24
    ldrb    st0, [sbox, st0]
    ldrb    st1, [sbox, st1]
    lsls    st0, #2
    ldr     st0, [Rtab, st0]
    lsls    st1, #2
    ldr     st1, [Rtab, st1]
    rev16   st0, st0
    rev     st0, st0
    eors    acc, st0
    lsrs    st0, st1, #8
    lsls    st1, #24
    orrs    st1, st0
    eors    acc, st1
    stmia   out!,  {acc}
    subs    nk, #1
    bne     1b
    pop     {r4-r7}
    bx      lr


ENDF(	aes_tmos_de_key	)

//-----------------------------------------------------------------------------
//void aes_tmos_en_ecb(const aes_context* ctxt, unsigned char* input,
//      unsigned char* output, unsigned blocks);

FUNC(   aes_tmos_en_ecb     )
#define tmp2    r0
#define lut     r1
#define tmp     r2
#define Y0      r3

#define X0      r4
#define X1      r5
#define X2      r6
#define X3      r7

    push    {r1-r7}
    mov     r12, r0 //context

1: // Encript loop
    ldmia   r1!, {X0, X1, X2, X3}       // Copy cipher text block
    str     r1, [sp]

    mov     Y0, r12
    ldr     tmp, [Y0]                   //rnds
    adds    Y0, #AES_CONTEXT_KEY+16     //key
    adds    tmp, #5
    push    {tmp, Y0}                   // [rnd, key]

    // add round key
    subs    Y0, #16
    ldmia   Y0, {r0, r1, r2, r3}
    eors    X0, r0
    eors    X1, r1
    eors    X2, r2
    eors    X3, r3

    mov     lut, r12
    ldr     lut, [lut, #AES_CONTEXT_FT] //lut

2:  //block loop            key, lut, rnd, X0-X3
    // 1. column
    uxtb    tmp, X0                     // Y0 =  FTO[ ( X0       ) & 0xFF ]
    lsls    tmp, #2
    ldr     Y0, [lut, tmp]

    lsrs    tmp, X1, #8                 // Y0 ^= FT1[ ( X1 >>  8 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    Y0, tmp

    lsrs    tmp, X2, #16               // Y0 ^= FT2[ ( X2 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    Y0, tmp

    lsrs    tmp, X3, #24                // Y0 ^= FT3[ ( X3 >>  24 ) & 0xFF ]
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp

    push    {Y0}

    // 2. column
    uxtb    tmp, X1                     // Y1 =  FT0[ ( X1        ) & 0xFF ]
    lsls    tmp, #2
    ldr     Y0, [lut, tmp]

    lsrs    tmp, X2, #8                 // Y1 ^= FT1[ ( X0 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    Y0, tmp

    lsrs    tmp, X3, #16                // Y1 ^= FT2[ ( X3 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    Y0, tmp

    lsrs    tmp, X0, #24                // Y1 ^= FT3[ ( X2 >>  24 ) & 0xFF ]
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp

    push    {lut, Y0}

    // 3. column
    uxtb    tmp, X2                     // Y2 =  FT0[ ( X2        ) & 0xFF ]
    lsls    tmp, #2
    ldr     Y0, [lut, tmp]

    lsrs    tmp, X3, #8                 // Y2 ^= FT1[ ( X1 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    Y0, tmp

    lsrs    tmp, X0, #16                // Y2 ^= FT2[ ( X0 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    Y0, tmp

    lsrs    tmp, X1, #24                // Y2 ^= FT3[ ( X3 >>  24 ) & 0xFF ]
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    Y0, tmp

    // 4. column
    uxtb    tmp, X3                     // X3 =  FT0[ ( X3        ) & 0xFF ]
    lsls    tmp, #2
    ldr     X3, [lut, tmp]

    lsrs    tmp, X0, #8                 // X3 ^= FT1[ ( X2 >>  8  ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #24
    lsls    tmp, #8
    orrs    tmp, tmp2
    eors    X3, tmp

    lsrs    tmp, X1, #16                // X3 ^= FT2[ ( X1 >>  16 ) & 0xFF ]
    uxtb    tmp, tmp
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    rev16   tmp, tmp
    rev     tmp, tmp
    eors    X3, tmp

    lsrs    tmp, X2, #24                // X3 ^= FT3[ ( X0 >>  24 ) & 0xFF ]
    lsls    tmp, #2
    ldr     tmp, [lut, tmp]
    lsrs    tmp2, tmp, #8
    lsls    tmp, #24
    orrs    tmp, tmp2
    eors    X3, tmp

    // add round key
    ldr     lut, [sp, #4+12]
    ldmia   lut!, {tmp2, tmp, X0, X2}
    str     lut, [sp, #4+12]
    eors    X3, X2                      //X3 ^= key[3]
    mov     X2, Y0
    eors    X2, X0                      //X2 = Y2 ^ key[2]
    pop     {lut, X0, X1}
    eors    X0, tmp2                    //X0 = Y0 ^ key[0]
    eors    X1, tmp                     //X1 = Y1 ^ key[1]


    // decrement counter
    ldr     tmp, [sp, #0+0]
    subs    tmp, #1     // do all the rounds,
    str     tmp, [sp, #0+0]
    bne     2b          // except the last one


    // last round
    mov     lut, r12
    ldr     lut, [lut, #AES_CONTEXT_SB]

    // 1. column
    uxtb    tmp, X0                     // Y0 =  FSb[ ( X0       ) & 0xFF ]
    ldrb    Y0, [lut, tmp]

    lsrs    tmp, X1, #8                 // Y0 |= FSb[ ( X3 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #8
    orrs    Y0, tmp

    lsrs    tmp, X2, #16                // Y0 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #16
    orrs    Y0, tmp

    lsrs    tmp, X3, #24
    ldrb    tmp, [lut, tmp]             // Y0 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
    orrs    Y0, tmp

    push    {Y0}

    // 2. column
    uxtb    tmp, X1                     // Y1 =  FSb[ ( X1       ) & 0xFF ]
    ldrb    Y0, [lut, tmp]

    lsrs    tmp, X2, #8                 // Y1 |= FSb[ ( X0 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #8
    orrs    Y0, tmp

    lsrs    tmp, X3, #16                // Y1 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #16
    orrs    Y0, tmp

    lsrs    tmp, X0, #24
    ldrb    tmp, [lut, tmp]             // Y1 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
    orrs    Y0, tmp

    push    {Y0}

    // 3. column
    uxtb    tmp, X2                     // Y2 =  FSb[ ( X2       ) & 0xFF ]
    ldrb    Y0, [lut, tmp]

    lsrs    tmp, X3, #8                 // Y2 |= FSb[ ( X1 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #8
    orrs    Y0, tmp

    lsrs    tmp, X0, #16                // Y2 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #16
    orrs    Y0, tmp

    lsrs    tmp, X1, #24
    ldrb    tmp, [lut, tmp]             // Y2 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
    orrs    Y0, tmp

    // 4. column
    uxtb    tmp, X3                     // X3 =  FSb[ ( X3       ) & 0xFF ]
    ldrb    X3, [lut, tmp]

    lsrs    tmp, X0, #8                 // X3 |= FSb[ ( X2 >>  8 ) & 0xFF ] <<  8
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #8
    orrs    X3, tmp

    lsrs    tmp, X1, #16                // X3 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 16
    uxtb    tmp, tmp
    ldrb    tmp, [lut, tmp]
    lsls    tmp, #16
    orrs    X3, tmp

    lsrs    tmp, X2, #24
    ldrb    tmp, [lut, tmp]             // X3 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 24
    lsls    tmp, #24
    orrs    X3, tmp

    // add round key
    ldr     tmp, [sp, #4+8]
    ldmia   tmp, {tmp2, tmp, X0, X2}
    eors    X3, X2                      //X3 ^= key[3]
    mov     X2, Y0
    eors    X2, X0                      //X2 = Y2 ^ key[2]
    pop     {X0, X1}
    eors    X0, tmp2                    //X0 = Y0 ^ key[0]
    eors    X1, tmp                     //X1 = Y1 ^ key[1]

    add     sp, #8     // [rnd, key]

    pop     {r1, tmp, Y0}   //r1=input, output, blocks
    stmia   tmp!, {X0-X3}
    subs    Y0, #1
    bne     3f
    pop     {r4-r7}
    bx      lr
3:
    push    {r1, tmp, Y0}
    b       1b                          // Decript loop



ENDF(   aes_tmos_en_ecb     )


.end
