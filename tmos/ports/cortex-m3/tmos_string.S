//==========================================================================
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
//==========================================================================
//
// Author(s):     Miroslav Kostadinov
// Date:          2008-12-02
// Description:   string.h functions
//
//========================================================================
#include "port_asm.h"

//*----------------------------------------------------------------------------
//*		int memcmp( const void *buf1, const void *buf2, size_t count )
//*----------------------------------------------------------------------------
//*The return value indicates the relationship between the buffers.
//*
//* Return Value Relationship of First count Bytes of buf1 and buf2
//*  < 0 buf1 less than buf2
//*  0   buf1 identical to buf2
//*  > 0 buf1 greater than buf2
//*
//*----------------------------------------------------------------------------
FUNC(	memcmp		)
				cbz				r2, 2f
1:
				ldrb			r12, [r0], #1
				ldrb			r3, [r1], #1
				cmp				r12, r3
				bne				3f
				subs			r2, #1
				bne				1b
2:
				movs			r0, #0
  				bx				lr
3:
				subs			r0, r12, r3
  				bx				lr

ENDF(	memcmp		)

//*----------------------------------------------------------------------------
//*		void *memset(const void *dst, int c, unsigned int length)
//*----------------------------------------------------------------------------
//*	return: dst
//*----------------------------------------------------------------------------
FUNC(	memset		)

				cbz				r2, 2f
1:				subs			r2, #1
				strb			r1, [r0, r2]
				bne				1b
2:
				bx				lr
ENDF(	memset		)

//*----------------------------------------------------------------------------
//*		void *memclr(const void *dst, size_t length)
//*----------------------------------------------------------------------------
//*	faster than memset(..., 0, ...)
//*	return: dst
//*----------------------------------------------------------------------------
FUNC(	memclr		)

				lsls			r2, r0, #30
				beq				2f
1://allign r0
				subs			r1, #1
				blo				9f
				strb			r2, [r0], #1
				lsls			r2, r0, #30
				bne				1b //allign
2:
				lsls			r3, r1, #30
				beq				4f
3://align r1
				subs			r1, #1
				strb			r3, [r0, r1]
				lsls			r3, r1, #30
				bne				3b
4:
				lsrs			r1, #3
				IT				CS
				stmiacs			r0!, {r2}
				subs			r1, #1
				blt				9f
5:
				stmia			r0!, {r2, r3}
6:
				subs			r1, #1
				bhs				5b
9:
				bx				lr

ENDF(	memclr		)

//*----------------------------------------------------------------------------
//*		void *memcpy(void *dst, void *src, unsigned int length)
//*----------------------------------------------------------------------------
//*	return: dst
//*----------------------------------------------------------------------------
FUNC(	memcpy		)

				cbz				r2, 2f
1:
				subs			r2, #1
				ldrb			r3, [r1, r2]
				strb			r3, [r0, r2]
				bne				1b
2:
				bx				lr

ENDF(	memcpy		)

//*----------------------------------------------------------------------------
//*		void *memmove(void *dst, void *src, unsigned int length)
//*----------------------------------------------------------------------------
//*	return: dst
//*----------------------------------------------------------------------------
FUNC(	memmove		)

				cbz				r2, 2f
				cmp				r1, r0
				bhi				3f
1:
				subs			r2, #1
				ldrb			r3, [r1, r2]
				strb			r3, [r0, r2]
				bne				1b
2:				bx				lr


3:
				ldrb			r3, [r1], #1
				strb			r3, [r0], #1
				subs			r2, #1
				bne				3b
				bx				lr

ENDF(	memmove		)

//*----------------------------------------------------------------------------
//*		void *strcpy(void *dst, void *src)
//*----------------------------------------------------------------------------
//*	return: dst
//*----------------------------------------------------------------------------
FUNC(	strcpy		)

				cbz				r0, 3f
				cbz				r1, 3f
1:
				ldrb			r3, [r1], #1
  				strb			r3, [r0], #1
				cmp				r3, #0
  				bne				1b
3:
  				bx				lr

ENDF(	strcpy		)

//*----------------------------------------------------------------------------
//*		char* strncpy(char* dst, const char* src, unsigned int len)
//*----------------------------------------------------------------------------
//*	return: If a NUL character is written to the destination, the function shall return
//* 		the address of the first such NUL character. Otherwise, it shall return &dst[len]
//*----------------------------------------------------------------------------
FUNC(	strncpy		)

				cbz				r0, 3f
				cbz				r1, 3f
				cbz				r2, 3f
1:
				ldrb			r3, [r1], #1
  				strb			r3, [r0], #1
  				subs			r2, #1
				cbz				r3, 2f
  				bne				1b
				bx				lr

2: //If the length of src is less than len, the remaining of the length is cleaned up.
  				subs			r2, #1
  				IT				HS
				strbhs			r3, [r0, r2]
				bhi				2b
  				subs			r0, #1
3:				bx				lr

ENDF(	strncpy		)

//*----------------------------------------------------------------------------
//*		unsigned int strlen(const char* dst)
//*----------------------------------------------------------------------------
FUNC(	strlen		)

				cbz				r0, 2f
				adds			r2, r0, #1
1:
				ldrb			r3, [r0], #1
				cmp				r3, #0
  				bne				1b
  				subs			r0, r2
2: 				bx				lr

ENDF(	strlen		)

//*----------------------------------------------------------------------------
//*		char* strchr(const char* dst, unsigned int i)
//*----------------------------------------------------------------------------
FUNC(	strchr		)

				cbz				r0, 3f
1:
				ldrb			r3, [r0], #1
				cbz				r3, 2f
  				cmp				r3, r1
 				bne				1b
  				subs			r3, r0, #1
2:
  				movs			r0, r3
3:				bx				lr

ENDF(	strchr		)

//*----------------------------------------------------------------------------
//*		char* strrchr(const char* dst, unsigned int i)
//*----------------------------------------------------------------------------
FUNC(	strrchr		)

				cbz				r0, 3f
				movs			r2, #0
1:
				ldrb			r3, [r0], #1
  				cmp				r3, r1
  				IT				EQ
  				subeq			r2, r0, #1
				cmp				r3, #0
 				bne				1b
 				movs			r0, r2
3: 				bx				lr

ENDF(	strrchr		)

//*----------------------------------------------------------------------------
//*		char * strpbrk ( const char * str1, const char * str2 );
//*----------------------------------------------------------------------------
FUNC(	strpbrk		)

				cbz				r0, 3f
				cbz				r1, 3f
1:
  				ldrb			r2, [r0], #1
				cbz				r2, 3f
				mov				r12, r1
2:
				ldrb			r3, [r12], #1
				cmp				r3, #0
				beq				1b
				cmp				r2, r3
				bne				2b
  				subs			r0, #1
				bx				lr

3:
  				movs			r0, #0
				bx				lr

ENDF(	strpbrk		)

//*----------------------------------------------------------------------------
//*     unsigned int strcspn(const char * str1, const char * str2);
//*----------------------------------------------------------------------------
FUNC(   strcspn     )

                cbz             r0, 4f
                push            {r0}
1:
                ldrb            r2, [r0]
                cbz             r2, 3f
                mov             r12, r1
2:
                ldrb            r3, [r12], #1
                cmp             r2, r3
                beq             3f
                cmp             r3, #0
                bne             2b
                adds            r0, #1
                b               1b

3:
                pop             {r1}
                subs            r0, r1
4:
                bx              lr

ENDF(   strcspn     )

#if USE_FIXED_CPAGE
//*----------------------------------------------------------------------------
//*		int islower(unsigned int c)
//*----------------------------------------------------------------------------
FUNC(	islower		)

				uxtb			r2, r0
				adr				r1, _char_2_type_
				ldrb			r0, [r2, r1]
				lsls			r0, #31
				bx				lr

ENDF(	islower		)

//*----------------------------------------------------------------------------
//*		int isupper(unsigned int c)
//*----------------------------------------------------------------------------
FUNC(	isupper		)

				uxtb			r2, r0
				adr				r1, _char_2_type_
				ldrb			r0, [r2, r1]
				lsrs			r0, #7
				bx				lr

ENDF(	isupper		)
#endif //USE_FIXED_CPAGE

//*----------------------------------------------------------------------------
//*		int strcmp(const char* dst, const char* src)
//*----------------------------------------------------------------------------
//*	return: A value greater than zero indicates that the first character that
//* 		does not match has a greater value in str1 than in str2
//*----------------------------------------------------------------------------
FUNC(	strcmp		)
				cbz				r0, 4f
				cbz				r1, 3f
1:
				ldrb			r2, [r0], #1
				ldrb			r3, [r1], #1
				cbz				r2, 2f
				cmp				r2, r3
				beq				1b
2:
				subs			r0, r2, r3
  				bx				lr
3:
                ldrb            r0, [r0]
4:
                cbz             r1, 5f
                ldrb            r1, [r1]
5:
				subs			r0, r1
  				bx				lr
ENDF(	strcmp		)

//*----------------------------------------------------------------------------
//*		int strncmp(const char* dst, const char* src, unsigned int len)
//*----------------------------------------------------------------------------
//*	return:
//*----------------------------------------------------------------------------
FUNC(	strncmp		)

				cbz				r2, 3f
1:
				ldrb			r12, [r0], #1
				ldrb			r3, [r1], #1
				subs			r2, #1
				beq				2f
				cbz				r3, 2f
				cmp				r12, r3
				beq				1b
2:
				subs			r0, r12, r3
  				bx				lr

3:
				movs			r0, #0
  				bx				lr

ENDF(	strncmp		)

//*----------------------------------------------------------------------------
//*     const char * strstr ( const char * str1, const char * str2 );
//*     char * strstr (       char * str1, const char * str2 );
//*
//*     char* strstr(const char*, const char*);
//*----------------------------------------------------------------------------
//* return:
//*----------------------------------------------------------------------------
FUNC(  strstr      )

                push            {r4,r5}
1:
                mov             r3, r1
                ldrb            r5, [r3], #1        //first = *str2++
                cbz             r5, 9f              //found
2:
                ldrb            r4, [r0], #1        //r4 = *str1++
                cbz             r4, 7f              //fail
                cmp             r4, r5
                bne             2b
                //first char matches
                mov             r2, r0
3:
                ldrb            r5, [r3], #1        //r5 = *str2++
                cbz             r5, 8f              //found (-1)
                ldrb            r4, [r2], #1        //r4 = *tmp++
                cbz             r4, 7f              //fail
                cmp             r4, r5
                beq             3b
                // dosn't match
                b               1b
7:
                movs            r0, #1
8:
                subs            r0, #1
9:
                pop             {r4, r5}
                bx              lr

ENDF(   strstr     )

#if USE_FIXED_CPAGE
//*----------------------------------------------------------------------------
//*		int *strcasecmp(void *dst, void *src)
//*----------------------------------------------------------------------------
//*	return:
//*----------------------------------------------------------------------------
#if USE_FIXED_CPAGE == 1250
FUNC_SECT(  strcasecmp, .text.casecmp   )

                push            {r4, r5}
                adr             r2, _char_2_type_
1:
                ldrb            r3, [r0], #1
                ldrb            r4, [r2, r3]
                lsrs            r4, #7              //isupper
                beq             3f
                lsrs            r4, r3, #6
                cmp             r4, #2
                bne             2f
                cmp             r3, #0xA5        //A
                it              eq
                moveq           r3, #0xB9 - 16   //a
                cmp             r3, #0xBC        //L
                it              eq
                moveq           r3, 0xBE -16     //l
                subs            r3, #16
2:
                adds            r3, #32
3:

                ldrb            r5, [r1], #1
                cbz             r3, 6f
                ldrb            r4, [r2, r5]
                lsrs            r4, #7              //isupper
                beq             5f
                lsrs            r4, r5, #6
                cmp             r4, #2
                bne             4f
                cmp             r5, #0xA5        //A
                it              eq
                moveq           r5, #0xB9 - 16   //a
                cmp             r5, #0xBC        //L
                it              eq
                moveq           r5, 0xBE -16     //l
                subs            r5, #16
4:
                adds            r5, #32
5:

                cmp             r3, r5
                beq             1b
6:
                subs            r0, r3, r5
                pop             {r4, r5}
                bx              lr

ENDF(   strcasecmp      )
#endif // USE_FIXED_CPAGE == 1250

#if USE_FIXED_CPAGE == 1251
FUNC_SECT(  strcasecmp, .text.casecmp   )

                push            {r4, r5}
                adr             r2, _char_2_type_
1:
                ldrb            r3, [r0], #1
                ldrb            r4, [r2, r3]
                lsrs            r4, #7              //isupper
                add             r3, r3, r4, lsl #5  //tolower
                ldrb            r5, [r1], #1
                ldrb            r4, [r2, r5]
                lsrs            r4, #7              //isupper
                add             r5, r5, r4, lsl #5  //tolower
                cbz             r3, 2f
                cmp             r3, r5
                beq             1b
2:
                subs            r0, r3, r5
                pop             {r4, r5}
                bx              lr

ENDF(   strcasecmp      )
#endif // USE_FIXED_CPAGE == 1251

//*----------------------------------------------------------------------------
//*		int *strncasecmp(void *dst, void *src, unsigned int len)
//*----------------------------------------------------------------------------
//*	return:
//*----------------------------------------------------------------------------
#if USE_FIXED_CPAGE == 1250
FUNC_SECT(  strncasecmp,    .text.casecmp   )

                cbz             r2, 9f
                adr             r3, _char_2_type_
                push            {r4, r5, r6}
1:
                ldrb            r6, [r0], #1
                ldrb            r4, [r3, r6]
                lsrs            r4, r4, #7          //isupper
                beq             3f
                lsrs            r4, r6, #6
                cmp             r4, #2
                bne             2f
                cmp             r6, #0xA5        //A
                it              eq
                moveq           r6, #0xB9 - 16   //a
                cmp             r6, #0xBC        //L
                it              eq
                moveq           r6, 0xBE -16     //l
                subs            r6, #16
2:
                adds            r6, #32
3:

                ldrb            r5, [r1], #1
                cbz             r6, 6f
                ldrb            r4, [r3, r5]
                lsrs            r4, r4, #7          //isupper
                beq             5f
                lsrs            r4, r5, #6
                cmp             r4, #2
                bne             4f
                cmp             r5, #0xA5        //A
                it              eq
                moveq           r5, #0xB9 - 16   //a
                cmp             r5, #0xBC        //L
                it              eq
                moveq           r5, 0xBE -16     //l
                subs            r5, #16
4:
                adds            r5, #32
5:

                subs            r2, #1
                beq             6f
                cmp             r6, r5
                beq             1b
6:
                subs            r0, r6, r5
                pop             {r4, r5, r6}
                bx              lr
9:
                movs            r0, #0
                bx              lr

ENDF(   strncasecmp     )
#endif // USE_FIXED_CPAGE == 1250

#if USE_FIXED_CPAGE == 1251
FUNC_SECT(  strncasecmp,    .text.casecmp   )

                cbz             r2, 3f
                adr             r3, _char_2_type_
                push            {r4, r5, r6}
1:
                ldrb            r6, [r0], #1
                ldrb            r4, [r3, r6]
                lsrs            r4, r4, #7          //isupper
                add             r6, r6, r4, lsl #5  //tolower
                ldrb            r5, [r1], #1
                cbz             r6, 2f
                ldrb            r4, [r3, r5]
                lsrs            r4, r4, #7          //isupper
                add             r5, r5, r4, lsl #5  //tolower
                subs            r2, #1
                beq             2f
                cmp             r6, r5
                beq             1b
2:
                subs            r0, r6, r5
                pop             {r4, r5, r6}
                bx              lr
3:
                movs            r0, #0
                bx              lr

ENDF(   strncasecmp     )
#endif // USE_FIXED_CPAGE == 1251

//*----------------------------------------------------------------------------
//*     int toupper(int c)
//*----------------------------------------------------------------------------
//* return:
//*----------------------------------------------------------------------------
#if USE_FIXED_CPAGE == 1250
FUNC_SECT(  toupper, .text.casecmp      )

                adr             r2, _char_2_type_
                ldrb            r1, [r0, r2]
                lsls            r1, r1, #31             //islower
                beq             1f
                lsrs            r1, r0, #6
                cmp             r1, #2
                bne             4f
                cmp             r0, #0xB9    //a
                beq             2f
                cmp             r0, #0xBE    //l
                beq             3f
                subs            r0, #16
1:
                bx              lr

2:
                movs            r0, #0xA5    //A
                bx              lr

3:
                movs            r0, #0xBC    //L
                bx              lr

4:
                subs            r0, #32
                bx              lr

ENDF(   toupper     )
#endif // USE_FIXED_CPAGE == 1250

#if USE_FIXED_CPAGE == 1251
FUNC_SECT(  toupper, .text.casecmp      )

                adr             r2, _char_2_type_
                ldrb            r1, [r0, r2]
                lsls            r1, r1, #31             //islower
                sub             r0, r0, r1, lsr #31-5   //tolower
                bx              lr

ENDF(   toupper     )
#endif // USE_FIXED_CPAGE == 1251

//*----------------------------------------------------------------------------
//*		int tolower(int c)
//*----------------------------------------------------------------------------
//*	return:
//*----------------------------------------------------------------------------
#if USE_FIXED_CPAGE == 1250
FUNC_SECT(  tolower, .text.casecmp      )

                adr             r2, _char_2_type_
                ldrb            r1, [r0, r2]
                lsrs            r1, r1, #7              //isupper
                beq             1f
                lsrs            r1, r0, #6
                cmp             r1, #2
                bne             4f
                cmp             r0, #0xA5    //A
                beq             2f
                cmp             r0, #0xBC    //L
                beq             3f
                adds            r0, #16
1:
                bx              lr

2:
                movs            r0, #0xB9    //a
                bx              lr

3:
                movs            r0, #0xBE    //l
                bx              lr

4:
                adds            r0, #32
                bx              lr

ENDF(   tolower     )
#endif // USE_FIXED_CPAGE == 1250

#if USE_FIXED_CPAGE == 1251
FUNC_SECT(  tolower, .text.casecmp      )

                adr             r2, _char_2_type_
                ldrb            r1, [r0, r2]
                lsrs            r1, r1, #7              //isupper
                add             r0, r0, r1, lsl #5      //toupper
                bx              lr

ENDF(   tolower     )
#endif // USE_FIXED_CPAGE == 1251



//upper
#define	_U			0x80
//lower
#define	_L			0x01
//digit
#define	_N			0x44
//space
#define	_S			0x08
//punct
#define _P			0x10
//control
#define _C			0x20
//hex
#define _X			0x40
#define	_B			0x02

				.global G_char_2_type_
#if USE_FIXED_CPAGE == 1250
G_char_2_type_:
_char_2_type_:
    .byte   _C, _C, _C, _C, _C, _C, _C, _C                                  //00
    .byte   _C, _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C|_S,  _C, _C              //08
    .byte   _C, _C, _C, _C, _C, _C, _C, _C                                  //10
    .byte   _C, _C, _C, _C, _C, _C, _C, _C                                  //18
    .byte   _S|_B,  _P, _P, _P, _P, _P, _P, _P                              //20  !"#$%&'
    .byte   _P, _P, _P, _P, _P, _P, _P, _P                                  //28 ()*+,-./
    .byte   _N, _N, _N, _N, _N, _N, _N, _N                                  //30 01234567
    .byte   _N, _N, _P, _P, _P, _P, _P, _P                                  //38 89:;<=>?
    .byte   _P, _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U          //40 @ABCDEFG
    .byte   _U, _U, _U, _U, _U, _U, _U, _U                                  //48 HIJKLMNO
    .byte   _U, _U, _U, _U, _U, _U, _U, _U                                  //50 PQRSTUVW
    .byte   _U, _U, _U, _P, _P, _P, _P, _P                                  //58 XYZ[\]^_
    .byte   _P, _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L          //60 'abcdefg
    .byte   _L, _L, _L, _L, _L, _L, _L, _L                                  //68 hijklmno
    .byte   _L, _L, _L, _L, _L, _L, _L, _L                                  //70 pqrstuvw
    .byte   _L, _L, _L, _P, _P, _P, _P, _C                                  //78 xyz{|}~
    .byte    0,  0, _P,  0, _P,  _P,  0,  0                                 //80 €�‚�„…†‡ 128
    .byte    0, _P, _U, _P, _U,  _U, _U, _U                                 //88 �‰Š‹ŚŤŽŹ 136
    .byte    0, _P, _P, _P, _P, _P, _P, _P                                  //90 �‘’“”•–— 144
    .byte    0,  0, _L, _P, _L, _L, _L, _L                                  //98 *™š›śťžź 152
    .byte   _C, _P, _P, _U,  0, _U, _P, _P                                  //A0  ˇ˘Ł¤Ą¦§ 160
    .byte   _P,  0, _U, _P,  0,  0,  0, _U                                  //A8 ¨©Ş«¬ ­®Ż 168
    .byte   _P, _P,  0, _L, _P,  0,  0,  0                                  //B0 °±˛ł´µ¶· 176
    .byte    0, _L, _L, _P, _U, _P, _L, _L                                  //B8 ¸ąş»Ľ˝ľż 184
    .byte   _U, _U, _U, _U, _U, _U, _U, _U                                  //C0 ŔÁÂĂÄĹĆÇ 192
    .byte   _U, _U, _U, _U, _U, _U, _U, _U                                  //C8 ČŇĘËĚÍÎĎ 200
    .byte   _U, _U, _U, _U, _U, _U, _U,  0                                  //D0 ĐŃŇÓÔŐÖ× 208
    .byte   _U, _U, _U, _U, _U, _U, _U,  0                                  //D8 ŘŮÚŰÜÝŢß 216
    .byte   _L, _L, _L, _L, _L, _L, _L, _L                                  //E0 ŕáâăäĺćç 224
    .byte   _L, _L, _L, _L, _L, _L, _L, _L                                  //E8 čéęëěíîď 232
    .byte   _L, _L, _L, _L, _L, _L, _L, _P                                  //F0 đńňóôőö÷ 240
    .byte   _L, _L, _L, _L, _L, _L, _L,  0                                  //F8 řůúűüý6˙ 248
#else
#if USE_FIXED_CPAGE == 1251
.align 2
G_char_2_type_:
_char_2_type_:
	.byte	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C									//00
	.byte	_C,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C|_S,	_C,	_C				//08
	.byte	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C									//10
	.byte	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C									//18
	.byte	_S|_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P								//20  !"#$%&'
	.byte	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P									//28 ()*+,-./
	.byte	_N,	_N,	_N,	_N,	_N,	_N,	_N,	_N									//30 01234567
	.byte	_N,	_N,	_P,	_P,	_P,	_P,	_P,	_P									//38 89:;<=>?
	.byte	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U			//40 @ABCDEFG
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//48 HIJKLMNO
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//50 PQRSTUVW
	.byte	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P									//58 XYZ[\]^_
	.byte	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L			//60 'abcdefg
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//68 hijklmno
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//70 pqrstuvw
	.byte	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C									//78 xyz{|}~
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//80 ЂЃ‚ѓ„…†‡ 128
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//88 €‰Љ‹ЊЌЋЏ 136
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//90 ђ‘’“”•–— 144
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//98 *™љ›њќћџ 152
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//A0  ЎўЈ¤Ґ¦§ 160
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//A8 Ё©Є«¬­®Ї 168
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//B0 °±Ііґµ¶· 176
	.byte	0,	0,	0,	0,	0,	0,	0,	0									//B8 ё№є»јЅѕї 184
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//C0 АБВГДЕЖЗ 192
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//C8 ИТКЛМНОП 200
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//D0 РСТУФХЦЧ 208
	.byte	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U									//D8 ШЩЪЫЬЭЮЯ 216
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//E0 абвгдежз 224
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//E8 ийклмноп 232
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//F0 рстуфхцч 240
	.byte	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L									//F8 шщъыьэ6я 248

#endif // USE_FIXED_CPAGE == 1251
#endif // USE_FIXED_CPAGE == 1250

#endif //USE_FIXED_CPAGE


//*----------------------------------------------------------------------------
//*		unsigned int XNUM_TO_DIGIT(unsigned int c)
//*----------------------------------------------------------------------------
//*	return:
//*----------------------------------------------------------------------------
FUNC(	XNUM_TO_DIGIT		)

				cmp				r0, #9
				ITE				HI
				addhi			r0, #'A'-10
				addls			r0, #'0'
				bx				lr

ENDF(	XNUM_TO_DIGIT		)


.end
