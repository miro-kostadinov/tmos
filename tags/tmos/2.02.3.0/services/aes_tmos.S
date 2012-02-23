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

#define key		r0
#define	lut		r1
#define tmp 	r2
#define Y0		r3

#define X0		r4
#define X1		r5
#define X2		r6
#define X3		r7

#define Y1		r8
#define Y2 		r9
#define tmp2	r12
#define rnd		r14

	.save 	{r0-r9, lr}
	push	{r0-r9, lr}

1: // Decript loop
	ldmia	r1!, {X0, X1, X2, X3}		// Copy cipher text block
	str		r1, [sp, #4]

	ldr		lut, [r0, #AES_CONTEXT_RT] //lut
	ldr		rnd, [r0], #AES_CONTEXT_KEY//rnds, key
	adds	rnd, #5
	adds	tmp, rnd, #2
	adds	key, key, tmp, lsl #4			//key = key[rnd+6+1]

	ldmdb	key!, {tmp, Y0, Y1, Y2}
	eors	X0, tmp
	eors	X1, Y0
	eors	X2, Y1
	eors	X3, Y2

2:	//block loop			key, lut, rnd, X0-X3
	// 1. column
	uxtb 	tmp, X0 					// Y0 =  RTO[ ( X0       ) & 0xFF ]
	ldr 	Y0, [lut, tmp, lsl #2]

	uxtb 	tmp, X3, ror #8 			// Y0 ^= RT1[ ( X3 >>  8 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y0, Y0, tmp, ror #24

	uxtb 	tmp, X2, ROR #16 			// Y0 ^= RT2[ ( X2 >>  16 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y0, Y0, tmp, ror #16

	lsrs 	tmp, X1, #24 				// Y0 ^= RT3[ ( X1 >>  24 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y0, Y0, tmp, ror #8

	// 2. column
	uxtb 	tmp, X1 					// Y1 =  RT0[ ( X1        ) & 0xFF ]
	ldr 	Y1, [lut, tmp, lsl #2]

	uxtb 	tmp, X0, ror #8 			// Y1 ^= RT1[ ( X0 >>  8  ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y1, Y1, tmp, ror #24

	uxtb 	tmp, X3, ror #16	 		// Y1 ^= RT2[ ( X3 >>  16 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y1, Y1, tmp, ror #16

	lsrs 	tmp, X2, #24 				// Y1 ^= RT3[ ( X2 >>  24 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y1, Y1, tmp, ror #8

	// 3. column
	uxtb 	tmp, X2 					// Y2 =  RT0[ ( X2        ) & 0xFF ]
	ldr 	Y2, [lut, tmp, lsl #2]

	uxtb 	tmp, X1, ror #8 			// Y2 ^= RT1[ ( X1 >>  8  ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y2, Y2, tmp, ror #24

	uxtb 	tmp, X0, ror #16 			// Y2 ^= RT2[ ( X0 >>  16 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y2, Y2, tmp, ror #16

	lsrs 	tmp, X3, #24 				// Y2 ^= RT3[ ( X3 >>  24 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	Y2, Y2, tmp, ror #8

	// 4. column
	uxtb 	tmp, X3 					// X3 =  RT0[ ( X3        ) & 0xFF ]
	ldr 	X3, [lut, tmp, lsl #2]

	uxtb 	tmp, X2, ror #8 			// X3 ^= RT1[ ( X2 >>  8  ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	X3, X3, tmp, ror #24

	uxtb 	tmp, X1, ror #16 			// X3 ^= RT2[ ( X1 >>  16 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	X3, X3, tmp, ror #16

	lsrs 	tmp, X0, #24 				// X3 ^= RT3[ ( X0 >>  24 ) & 0xFF ]
	ldr 	tmp, [lut, tmp, lsl #2]
	eors 	X3, X3, tmp, ror #8

	// add round key
	ldmdb 	key!, { tmp, tmp2 }
	eors 	X2, Y2, tmp
	eors 	X3, tmp2
	ldmdb 	key!, { tmp, Y2 }
	eors 	X0, Y0, tmp
	eors 	X1, Y1, Y2

	// decrement counter
	subs 	rnd, #1 	// do all the rounds,
	bne 	2b		 	// except the last one


	// last round
	ldr 	tmp, [sp]
	ldr		lut, [tmp, #AES_CONTEXT_RSB]

	// 1. column
	uxtb 	tmp, X0 					// Y0 =  FSb[ ( X0       ) & 0xFF ]
	ldrb 	Y0, [lut, tmp]

	uxtb 	tmp, X3, ror #8 			// Y0 |= FSb[ ( X3 >>  8 ) & 0xFF ] <<  8
	ldrb 	tmp, [lut, tmp]
	orr 	Y0, Y0, tmp, lsl #8

	uxtb 	tmp, X2, ror #16 			// Y0 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 16
	ldrb 	tmp, [lut, tmp]
	orr 	Y0, Y0, tmp, lsl #16

	lsrs 	tmp, X1, #24
	ldrb 	tmp, [lut, tmp]				// Y0 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 24
	orrs 	Y0, Y0, tmp, lsl #24

	// 2. column
	uxtb 	tmp, X1 					// Y1 =  FSb[ ( X1       ) & 0xFF ]
	ldrb 	Y1, [lut, tmp]

	uxtb 	tmp, X0, ror #8		 		// Y1 |= FSb[ ( X0 >>  8 ) & 0xFF ] <<  8
	ldrb 	tmp, [lut, tmp]
	orrs 	Y1, Y1, tmp, lsl #8

	uxtb 	tmp, X3, ror #16 			// Y1 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 16
	ldrb 	tmp, [lut, tmp]
	orrs	Y1, Y1, tmp, lsl #16

	lsrs 	tmp, X2, #24
	ldrb 	tmp, [lut, tmp]				// Y1 |= FSb[ ( X2 >> 16 ) & 0xFF ] << 24
	orrs 	Y1, Y1, tmp, lsl #24

	// 3. column
	uxtb 	tmp, X2 					// Y2 =  FSb[ ( X2       ) & 0xFF ]
	ldrb	Y2, [lut, tmp]

	uxtb 	tmp, X1, ror #8 			// Y2 |= FSb[ ( X1 >>  8 ) & 0xFF ] <<  8
	ldrb 	tmp, [lut, tmp]
	orrs 	Y2, Y2, tmp, lsl #8

	uxtb 	tmp, X0, ror #16 			// Y2 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 16
	ldrb 	tmp, [lut, tmp]
	orrs 	Y2, Y2, tmp, lsl #16

	lsrs 	tmp, X3, #24
	ldrb 	tmp, [lut, tmp]				// Y2 |= FSb[ ( X3 >> 16 ) & 0xFF ] << 24
	orrs 	Y2, Y2, tmp, lsl #24

	// 4. column
	uxtb 	tmp, X3 					// X3 =  FSb[ ( X3       ) & 0xFF ]
	ldrb 	X3, [lut, tmp]

	uxtb 	tmp, X2, ror #8 			// X3 |= FSb[ ( X2 >>  8 ) & 0xFF ] <<  8
	ldrb 	tmp, [lut, tmp]
	orrs 	X3, X3, tmp, lsl #8

	uxtb 	tmp, X1, ror #16 			// X3 |= FSb[ ( X1 >> 16 ) & 0xFF ] << 16
	ldrb 	tmp, [lut, tmp]
	orrs 	X3, X3, tmp, lsl #16

	lsrs 	tmp, X0, #24
	ldrb 	tmp, [lut, tmp]				// X3 |= FSb[ ( X0 >> 16 ) & 0xFF ] << 24
	orrs 	X3, X3, tmp, lsl #24

	// add round key
	ldmdb 	key!, { tmp, tmp2 }
	eors 	X2, Y2, tmp
	eors 	X3, tmp2
	ldmdb 	key!, { tmp, Y2 }
	eors 	X0, Y0, tmp
	eors 	X1, Y1, Y2


	ldmia	sp, {r0-r3}					//r0=ctxt, r1=input, r2=output, r3=blocks
	stmia	r2!, {X0-X3}
	subs	r3, #1
	IT		EQ
	popeq	{r0-r9, pc}
	str		r2, [sp, #8]
	str		r3, [sp, #12]
	b 		1b							// Decript loop

#undef key
#undef lut
#undef tmp
#undef Y0

#undef X0
#undef X1
#undef X2
#undef X3

#undef Y1
#undef Y2
#undef tmp2
#undef rnd

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
#define rnd     r8      // round counter
#define rcon    r12     // pointer to rcon

    push    {r4-r8}
	ldr		sbox, [r0, #AES_CONTEXT_SB]
	str		r2, [r0, #AES_CONTEXT_RNDS]
	adr		rcon, _rcon_table_
	adds	r0, #AES_CONTEXT_KEY		//out

	// copy key from in to out
    mov     cnt, nk
1:
	ldmia	in!,  {tmp}
    stmia   out!, {tmp}
    subs    cnt, #1
    bne     1b

    // number of rounds = 3*nk + 28 (cnt==0)
    adds	rnd, nk, nk, lsl #1
    adds    rnd, #28

    // rnd mod nk == 0
2:
    movs    cnt, #0
	ldr     st, [out, #-4]				// st = key[i*nk +nk-1]

    uxtb    tmp, st, ror #8
    ldrb    acc, [sbox, tmp]			//acc = FSb[ ( st >>  8 ) & 0xFF ]

    uxtb    tmp, st, ror #16
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #8		//acc ^= FSb[ ( st >> 16 ) & 0xFF ] << 8

	lsrs	tmp, st, #24
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #16		//acc ^= FSb[ ( st >> 24 ) & 0xFF ] << 16

    uxtb    tmp, st
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #24		//acc ^= FSb[ ( st  ) & 0xFF ] << 24

    ldrb    tmp, [rcon], #1
    eor     acc, tmp					//acc ^= RCON[i]

3:	//drain
	sub		tmp, out, nk, lsl #2
    ldr     tmp, [tmp]
	eors    acc, tmp					//acc ^= key[i*nk]
    stmia 	out!, {acc}					//key[(i+1)*nk] = acc

    subs    rnd, #1
    beq     9f	//done

    adds    cnt, #1
    cmp     cnt, nk
    beq     2b
	cmp     cnt, #4
    bne     3b	//drain
	cmp     nk, #6
    bls     3b  //drain

    // (rnd mod nk == 4) and (nk > 6)
    ldr     st, [out, #-4]
    uxtb    tmp, st
    ldrb    acc, [sbox, tmp]

    uxtb    tmp, st, ror #8
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #8

    uxtb    tmp, st, ror #16
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #16

	lsrs    tmp, st, #24
    ldrb    tmp, [sbox, tmp]
    orrs    acc, acc, tmp, lsl #24
    b       3b  //drain


9: //done
    pop     {r4-r8}
	bx      lr

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

//void aes_tmos_en_key(const aes_context* ctxt, unsigned char* input, unsigned key_size);
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
    pop     {r0, r2, r14}

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
    uxtb    st1, tmp, ror #8
    ldrb    st0, [sbox, st0]
    ldrb    st1, [sbox, st1]
    ldr     acc, [Rtab, st0, lsl #2]
    ldr     st1, [Rtab, st1, lsl #2]
    eors    acc, acc, st1, ror #24
    uxtb    st0, tmp, ror #16
    lsrs    st1, tmp, #24
    ldrb    st0, [sbox, st0]
    ldrb    st1, [sbox, st1]
    ldr     st0, [Rtab, st0, lsl #2]
    ldr     st1, [Rtab, st1, lsl #2]
    eors    acc, acc, st0, ror #16
    eors    acc, acc, st1, ror #8
    stmia   out!,  {acc}
    subs    nk, #1
    bne     1b
    pop     {r4-r7}
    bx      lr


ENDF(	aes_tmos_de_key	)




.end
