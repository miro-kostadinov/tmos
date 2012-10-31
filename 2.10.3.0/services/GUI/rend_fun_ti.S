// ---------------------------------------------------------
// 		Render Functions Written in assembler for speed
//
//---------------------------------------------------------
#include <port_asm.h>
#include <mcu_inc.h>


// ---------------------------------------------------------
// 		Screen Rendering - EMM driver
//		reder functions rotate the input at 90 degrees cw and mirrow
//---------------------------------------------------------


//				.global			RenderGDIBitmap
//				.global			DrawHLine, InvertHLine, InvertVLine, DrawVLine


				.extern			lcd_buf

//.align 2
//.thumb
//.thumb_func
//.func RenderGDIBitmap
//------------------------------------------------------------------------------
//void RenderGDIBitmap(unsigned char* x0, unsigned int y0, const unsigned char* src, unsigned int width, unsigned int rows)
//RenderGDIBitmap:
FUNC( 	RenderGDIBitmap 	)
												//r0 = x0
												//r1 = y0
												//r2 = src
												//r3 = width
		push			{r4, r5, r6, r7, lr}
		ldr				r6,	[sp, #20]

		movs			r5, #11
		muls			r3, r5
		adds			r0, r3
		negs			r3, r3
		mov				r12, r3					//r12 = -width
		subs			r3, #11

rgdi_feed:
		movs			r4, #8
		ldrb			r5, [r2, #0]
		adds			r2, #1
		adds			r3, #11
		bpl				rgdi_next_row

rgdi_width_loop:
		lsrs			r5, #1
		bcc				rgdi_skip_bit
		ldrb			r7, [r0, r3]
		orrs			r7, r1
		strb			r7, [r0, r3]
rgdi_skip_bit:
		subs			r4, #1
		beq				rgdi_feed
		adds			r3, #11
		bmi				rgdi_width_loop

rgdi_next_row:
		lsls			r1, #1
		cmp				r1, #129
		bcc				rgdi_skip_y
		movs			r1, #1
		adds			r0, #1
rgdi_skip_y:
		subs			r6, #1
		beq				rgdi_exit
		mov				r3, r12					//r3=-width
		b				rgdi_width_loop

rgdi_exit:
		pop				{r4, r5, r6, r7, pc}

ENDF(	RenderGDIBitmap   )

//------------------------------------------------------------------------------
//void DrawHLine(unsigned int x0, unsigned int x1, unsigned int y, char* buf)
//DrawHLine:
FUNC(	DrawHLine	)

		subs			r1, r0
		bmi				dhl_exit
		add				r12, r3, r2, lsr #3
		movs			r3, #11
		muls			r1, r3
		muls			r0, r3
		adds			r0, r12

		and				r3, r2, #7
		movs			r2, #1
		lsls			r2, r3
dhl_loop:
		ldrb			r3, [r0, r1]
		orrs			r3, r2
		strb			r3, [r0, r1]
		subs			r1, #11
		bpl				dhl_loop
dhl_exit:
		bx				lr

ENDF(	DrawHLine	)
//.endfunc

//.thumb_func
//.func InvertVLine
//------------------------------------------------------------------------------
//void DrawHLine(unsigned int x0, unsigned int x1, unsigned int y, char* buf)
//InvertVLine:
FUNC(	InvertVLine	)

		push			{r4, lr}
		movs			r4, #11				//r2 *= 11
		muls			r2, r4
		adds			r2, r3				//r2 += buf
		lsrs			r4, r0, #3			//r4 = y0>>3
		adds			r2, r4

		lsls			r0, #29				//r0 = ff<<(r0&7)
		lsrs			r3, r0, #29			//
		movs			r0, #0xFF
		lsls			r0, r3

		lsrs			r3, r1, #3			//r4 = (y1>>3)-(y0>>3)
		subs			r4, r3, r4
		bmi				ivl_exit
		beq				ivl_last
ivl_loop:
		ldrb			r3, [r2]
		eors			r3, r0
		strb			r3, [r2], #1
		movs			r0, #0xFF
		subs			r4, #1
		bne				ivl_loop
ivl_last:
		movs			r3, #7
		ands			r1, r3
		subs			r3, r1
		movs			r1, #0xFF
		lsrs			r1, r3
		ands			r0, r1
		ldrb			r3, [r2, #0]
		eors			r3, r0
		strb			r3, [r2, #0]
ivl_exit:
		pop				{r4, pc}

ENDF(	InvertVLine	)
//.endfunc


//.thumb_func
//.func InvertHLine
//------------------------------------------------------------------------------
//void InvertHLine(unsigned int x0, unsigned int x1, unsigned int y, char* buf)
//InvertHLine:
FUNC(	InvertHLine	)

		subs			R1, R0
		bmi				ihl_exit
		add				r12, r3, r2, lsr #3
		movs			r3, #11
		muls			r1, r3
		muls			r0, r3
		add				r0, r12
		and				r3, r2, #7
		movs			r2, #1
		lsls			r2, r3
ihl_loop:
		ldrb			r3, [r0, r1]
		eors			r3, r2
		strb			r3, [r0, r1]
		subs			r1, #11
		bpl				ihl_loop
ihl_exit:
		bx				lr

ENDF(	InvertHLine	)
//.endfunc


//.thumb_func
//.func DrawVLine
//------------------------------------------------------------------------------
//void DrawVLine(unsigned int y0, unsigned int y1, unsigned int x, char* buf)
//DrawVLine:
FUNC(	DrawVLine	)

		push			{r4, lr}
		movs			r4, #11				//r2 *= 11
		muls			r2, r4
		adds			r2, r3				//r2 += buf
		lsrs			r4, r0, #3			//r4 = y0>>3
		adds			r2, r4

		lsls			r0, #29				//r0 = ff<<(r0&7)
		lsrs			r3, r0, #29			//
		movs			r0, #0xFF
		lsls			r0, r3

		lsrs			r3, r1, #3			//r4 = (y1>>3)-(y0>>3)
		subs			r4, r3, r4
		bmi				dvl_exit
		beq				dvl_last
dvl_loop:
		ldrb			r3, [r2]
		orrs			r3, r0
		strb			r3, [r2], #1
		movs			r0, #0xFF
		subs			r4, #1
		bne				dvl_loop
dvl_last:
		movs			r3, #7
		ands			r1, r3
		subs			r3, r1
		movs			r1, #0xFF
		lsrs			r1, r3
		ands			r0, r1
		ldrb			r3, [r2, #0]
		orrs			r3, r0
		strb			r3, [r2, #0]
dvl_exit:
		pop				{r4, pc}

ENDF(	DrawVLine	)
//.endfunc



.end

