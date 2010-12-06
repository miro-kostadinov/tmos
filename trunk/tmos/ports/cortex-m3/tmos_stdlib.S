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
// Date:          2008-12-07
// Description:   TMOS Memory pool and memory functions
//
//========================================================================

#include "port_asm.h"
#include "port_inc.h"


//  Block structure:
//
//	struct
//	{
//		unsigned short next;		// size in words 2-32767
//		unsigned short prev;		// top 15 bits=size of prev block, lsb=1 when free
//		union
//		{
//			struct
//			{
//				signed short nf;
//				signed short pf;
//			}
//			data[]
//		}
//	}
//
//		31-16  	 		15  14-0
//		prev (free)		0   next
//		   pf       	 nfree



//------------------------------------------------------------------------------
//void svc_pool_init(void* start, void* end);
FUNC(	svc_pool_init		)
//    +----+----+----+----+
// s  | 0  |  2 |  0 |  2 |
//    +----+----+----+----+
//    +----+----+----+----+
//    | 5  | n  |  2 | n  |
//    +----+----+----+----+
//    +----+----+----+----+
// e  | n< |  0 | n  | 0  |
//    +----+----+----+----+

				subs			r1, #8			//e
				movs			r2, #2
				str				r2,[r0, #4]
				str				r2,[r0], #8
				subs			r3, r1, r0		//n
				lsrs			r3, #2
				strh			r3, [r0, #4]
				strh			r3, [r0]
				strh			r2, [r0, #6]
				movs			r2, #5
				strh			r2, [r0, #2]
				lsls			r3, #16
				str				r3, [r1, #4]
				lsls			r3, #1
				str				r3, [r1]
				bx				lr

ENDF(	svc_pool_init		)

//------------------------------------------------------------------------------
//void* sys_malloc(unsigned int size);
FUNC(	sys_malloc		)

				push			{lr}
				bl				svc_malloc
				pop				{r1}
				lsrs			r2, r1, #3
           		ITE         	CC                   	// if zero (C Clear) then
           		mrscc       	r2, MSP              	// Main Stack was used, put MSP in R2
           		mrscs       	r2, PSP              	// Process Stack was used
				str				r0, [r2, tsk_sp_r0]
				bx				r1

ENDF(	sys_malloc		)

//------------------------------------------------------------------------------
//void * sys_realloc(void* ptr, unsigned int size)
FUNC(	sys_realloc		)

				push			{lr}
				bl				svc_realloc
				pop				{r1}
				lsrs			r2, r1, #3
           		ITE         	CC                   	// if zero (C Clear) then
           		mrscc       	r2, MSP              	// Main Stack was used, put MSP in R2
           		mrscs       	r2, PSP              	// Process Stack was used
				str				r0, [r2, tsk_sp_r0]
				bx				r1

ENDF(	sys_realloc		)

//------------------------------------------------------------------------------
//void* svc_malloc(unsigned int size);
FUNC(	svc_malloc		)

				ldr				r1, =end				//pool

ENDF(	svc_malloc		)


#define best		r0
#define bptr		r1
#define asize		r2
#define bsize		r3
#define bestsize	r12

//------------------------------------------------------------------------------
//void* svc_pool_malloc(unsigned int size, void* pool);
FUNC(	svc_pool_malloc		)

				cbz				r0, sma_exit
				adds			asize, r0, #7
				subs			bsize, r0, asize	//  bsize = -7
				lsrs			asize, #2			// r2 = asize
sma_set_best:
				mov				best, bptr
				mov				bestsize, bsize
sma_search_loop:
				ldrsh			bsize, [bptr, #4]
				adds			bptr, bptr, bsize, lsl #2
				ldrh			bsize, [bptr]
				cmp				bsize, bestsize
				bhi				sma_search_loop			//bsize > bestsize
				cmp				bsize, asize
				bhi				sma_set_best			// asize < bsize <= bestsize
				beq				sma_match				// bsize == asize
				cmp				bsize, #0
				bne				sma_search_loop			// 0 < bsize < asize
				//bsize=0
				adds			bsize, bestsize, #7
				beq				sma_fail
				// split existing block (best, bestsize)
//----- malloc split
//    +----+----+----+----+              +-----+----+----+----+
// cf | p1 | bs | pf | nf |           cf |  p1 |bs-s|         |
//    +----+----+----+----+              +-----+----+----+----+
//                                       +-----+----+----+----+ This is the new
//                                     c |bs-s<| s  |   ..    | block at cf+b
//                                       +-----+----+----+----+
//    +----+----+----+----+              +-----+----+----+----+
//  n | bs<|    |   ...   |            n |  s< |    |   ...   |
//    +----+----+----+----+              +-----+----+----+----+
				subs			bsize, bestsize, asize
				cmp				bsize, #1
				beq				7f
				strh			bsize, [best]						// cf-> bs-s
				adds			bptr, asize, bsize, lsl #17			// 	 bs-s<   s
				adds			best, best, bsize, lsl #2
				str				bptr, [best], #4
				lsls			bsize, asize, #1
				subs			bptr, best, #2
				strh			bsize, [bptr, asize, lsl #2]			// n -> s<
sma_exit:		bx				lr


sma_match:		//match (pf, asize=bsize)
//----- malloc match
//    +----+----+----+----+              +----+----+----+-----+
// pf |*?? | ?? |    | pf |           pf |*?? | ?? |    |pf+nf|
//    +----+----+----+----+              +----+----+----+-----+
//             ...                                ...
//    +----+----+----+----+              +----+----+----+-----+  Clear the free
// cf | p1 |  s | pf | nf |           cf | p0 |    |   ..     |  list bit here
//    +----+----+----+----+              +----+----+----+-----+
//             ...                                ...
//    +----+----+----+----+              +----+----+-----+----+
// nf |*?? | ?? | nf  |    |           nf| ?? | ?? |pf+nf|    |
//    +----+----+----+----+              +----+----+-----+----+
				movs			best, bptr
7:
				ldrh			bsize, [best, #2]!
				subs			bsize, #1
				strh			bsize, [best], #2					//cf: p1=>p0
				ldrsh			asize, [best]			//nf
				ldrsh			bsize, [best, #2]		//pf
				adds			asize, bsize
				subs			bptr, best, bsize, lsl #2
				strh			asize, [bptr], #2
				strh			asize, [bptr, asize, lsl #2]
				bx 				lr

sma_fail:
				movs			r0, #0
				bx				lr

ENDF(	svc_pool_malloc		)



//------------------------------------------------------------------------------
//void svc_free(void* ptr);
FUNC(	svc_free		)

				ldr				r1, =end

ENDF(	svc_free		)


//------------------------------------------------------------------------------
//void svc_pool_free(void* ptr, void* pool);
FUNC(	svc_pool_free		)

				cmp				r0, r1
				bls				smf_exit				//BAD ptr!
//check next
				ldrh			r12, [r0, #-4]!			//r12=cs
				ldr				r3, [r0, r12, lsl #2]
				lsls			r3, r3, #16				//r3 = fs<<16
				bcc				smf_check_prev
//    +----+----+----+----+              +------+-----+-----+-----+
// 	p |*?? | ?? | ?? | pf |            p |      |     |     |pf+nf|
//    +----+----+----+----+              +------+-----+-----+-----+
//             ...                                 ...
//    +----+----+----+----+              +------+-----+-----+-----+ This block is
// r0 | cp | cs |   ...   |           r0 |      |cs+fs|    ...    | disconnected
//    +----+----+----+----+              +------+-----+-----+-----+ from free list,
//    +----+----+----+----+                                         assimilated with
// cf |cs1 | fs | pf | nf |                                         the next, and
//    +----+----+----+----+                                         ready for step 2
//    +----+----+----+----+              +------+-----+-----+-----+
// nn | fs<| ?? | ?? | ?? |           nn |cs+fs<|     |    ...    |
//    +----+----+----+----+              +------+-----+-----+-----+
//             ...                                 ...
//    +----+----+----+----+              +------+-----+-----+-----+
//  n |*?? | ?? | nf | ?? |            n |      |     |pf+nf|     |
//    +----+----+----+----+              +------+-----+-----+--- -+

//assimilate next
				adds			r2, r12, r3, lsr #16	//r2 = cs + fs
				strh			r2, [r0]				//r0->next = cs+fs

				adds			r3, r0, #2
				lsls			r2, #1
				strh			r2, [r3, r2, lsl #1]	//nn->prev = cs+fs<

				adds			r12, r0, r12, lsl #2	//r12 = cf
				ldrsh			r2, [r12, #4]!			// r2 = nf
				ldrsh			r3, [r12, #2]			// r3 = pf
				subs			r12, r12, r3, lsl #2	//r12 = p+4
				adds			r3, r2
				strh			r3, [r12], #2			//p->nf = pf+nf
				strh			r3, [r12, r3, lsl #2]	//n->pf = pf+nf

smf_check_prev:
//    +----+----+----+----+              +------+-----+----+----+ This block has
// p  | ??*| cp | ?? | nf |           p  | ??*  |cp+cs|    |    | assimilated the
//    +----+----+----+----+              +------+-----+----+----+ current block
//    +----+----+----+----+
// r0 | cp<| cs |   ...   |
//    +----+----+----+----+
//    +----+----+----+----+              +------+-----+----+----+
//  n | cs<| ?? |   ...   |            n |cp+cs<|     |    |    |
//    +----+----+----+----+              +------+-----+----+----+
				ldrh			r2, [r0, #2]			// r2 = cp<
				subs			r12, r0, r2, lsl #1		//r12 = p
				ldr				r3, [r12]				// r3 = ??* cp
				lsls			r3, r3, #16				// r3 = cp<<16
				bcc				smf_free
//assimilate prev
				ldrh			r1, [r0]				//r1 = cs
				adds			r1, r1, r3, lsr #16		//r1 = cs+cp
				strh			r1, [r12], #2			//p->n = cs+cp
				lsls			r1, #1
				strh			r1, [r12, r1, lsl #1]	//n->pf = cs+cp<
				movs			r0, #0
smf_exit:		bx				lr


3:smf_free:
//    +----+----+----+----+              +----+----+----+----+
//  0 | ?? | ?? |  0 | nf |            0 | ?? | ?? |  0 |  c |
//    +----+----+----+----+              +----+----+----+----+
//             ...                                ...
//    +----+----+----+----+              +----+----+----+----+
// r0 | cp | cs |   ..    |           r0 |cp1 |    |  c |nf-c|
//    +----+----+----+----+              +----+----+----+----+
//             ...                                ...
//    +----+----+----+----+              +----+----+----+----+
// nf |*?? | ?? | nf | ?? |           nf |*?? | ?? |nf-c| ?? |
//    +----+----+----+----+              +----+----+----+----+
				adds			r2, #1
				strh			r2, [r0, #2]		//r0->p = cp1
				subs			r2, r0, r1
				lsrs			r2, r2, #2			// r2 = c
				ldrh			r3, [r1, #4]!		// r3 = nf
				strh			r2, [r1], #2		//0->fn = c
				strh			r2, [r0, #6]		//r0->pf= c
				subs			r2, r3, r2				// r3 = nf-c
				strh			r2, [r0, #4]		//r0->nf=nf-c
				strh			r2, [r1, r3, lsl #2]//nf->pf= nf-c
				movs			r0, #0
				bx				lr
ENDF(	svc_free_pool		)




//------------------------------------------------------------------------------
//void * svc_realloc(void* ptr, unsigned int size)
FUNC(	svc_realloc		)

				cmp				r0, #0
				beq				.L5					//realloc(NULL,size) -> malloc(size)
				cmp				r1, #0
				beq				svc_free			//realloc(ptr, 0)	-> free(ptr)
				mov				r12, r1
				adds			r3, r1, #7
				lsrs			r3, r3, #2			//r3 = newsize_w
				push			{r0, r4, r5}
				ldrh			r2, [r0, #-4]!		//r2=cs
				cmp				r3, r2
				beq				.L9					//size match

//check next	r0, r1=newsize_b, r2=oldsize_w, r3=newsize_w
				add				r4, r0, r2, lsl #2	//r4=cf
				ldr				r5, [r4]
				lsls			r5, r5, #16			//r5=fs<<16
				bcc				smf_check_split

//    +----+----+----+----+              +------+-----+-----+-----+
// p  |*?? | ?? | ?? | pf |           p  |      |     |     |pf+nf|
//    +----+----+----+----+              +------+-----+-----+-----+
//             ...                                 ...
//    +----+----+----+----+              +------+-----+-----+-----+ This block is
// r0 | cp | cs |   ...   |           r0 |      |cs+fs|    ...    | disconnected
//    +----+----+----+----+              +------+-----+-----+-----+ from free list,
//    +----+----+----+----+                                         assimilated with
// cf |cs1 | fs | pf | nf |                                         the next, and
//    +----+----+----+----+                                         ready for step 2
//    +----+----+----+----+              +------+-----+-----+-----+
// nn | fs | ?? | ?? | ?? |           nn |cs+fs<|     |    ...    |
//    +----+----+----+----+              +------+-----+-----+-----+
//             ...                                ...
//    +----+----+----+----+              +------+-----+-----+-----+
// n  |*?? | ?? | nf | ?? |           n  |      |     |pf+nf|     |
//    +----+----+----+----+              +------+-----+-----+--- -+

//assimilate next
				add				r2, r2, r5, lsr #16	//r2=cs+fs
				strh			r2, [r0]			//	r0->next = cs+fs

				add				r5, r0, r2, lsl #2	// r5 = nn
				lsls			r1, r2, #1
				strh			r1, [r5, #2]		//nn->prev = (cs+fs)<<1

				ldrsh			r5, [r4, #4]!
				ldrsh			r1, [r4, #2]
				subs			r4, r4, r1, lsl #2	// r4 = p+4
				adds			r1, r5				// r1 = pf+nf
				strh			r1, [r4], #2		//p->nf = pf+nf
				strh			r1, [r4, r1, lsl #2]//n->pf = pf+nf

smf_check_split:	//	r0, r1=newsize_b, r2=oldsize_w, r3=newsize_w
				subs			r1, r2, r3			//r1=cs-ns
				beq				.L9					//size match
				blo				.L2
				//the current block is bigger
				cmp				r1, #1
				beq				.L9					//can't free
//    +----+----+----+----+              +------+-----+----+----+
//  c |    | cs |   ...   |            c |      | ns  |   ...   |
//    +----+----+----+----+              +------+-----+----+----+
//                                       +------+-----+----+----+ This is the
//                                     s | ns<1 |cs-ns|   ..    | new block at
//                                       +------+-----+----+----+ c+blocks
//    +----+----+----+----+              +------+-----+----+----+
//  n | cs<| ?? |   ...   |            n |cs-ns<|     |   ...   |
//    +----+----+----+----+              +------+-----+----+----+

				strh			r3, [r0]				//c->size = newsize
				adds			r0, r0, r3, lsl #2		//r0=s
				add				r3, r1, r3, lsl #17
				add				r3, #0x10000			// r4  = ns<1 cs-ns
				str				r3, [r0]				//s->size = oldsize -newsize, s->prev= newsize << 1
				lsls			r3, #1
				adds			r1, r0, r1, lsl #2
				strh			r3, [r1, #2]			//n->prev = (oldsize - newsize)<<1





//    +----+----+----+----+              +----+----+----+----+
//  0 | ?? | ?? |  0 | nf |            0 | ?? | ?? |  0 |  c |
//    +----+----+----+----+              +----+----+----+----+
//             ...                                ...
//    +----+----+----+----+              +----+----+----+----+
//  c | cp | cs |   ..    |            c |*cp |    |  c |nf-c|
//    +----+----+----+----+              +----+----+----+----+
//             ...                                ...
//    +----+----+----+----+              +----+----+----+----+
// nf |*?? | ?? | nf | ?? |           nf |*?? | ?? |nf-c| ?? |
//    +----+----+----+----+              +----+----+----+----+
				ldr				r1, =end			//r1=mem0

				subs			r2, r0, r1
				lsrs			r2, r2, #2			//r2=c
				ldrh			r3, [r1, #4]!
				strh			r2, [r1], #2		//mem0->fn = c
				strh			r2, [r0, #6]		//r0->pf= c
				subs			r2, r3, r2
				strh			r2, [r0, #4]		//r0->nf=nf-c
				strh			r2, [r1, r3, lsl #2]//nf->pf= nf-c
.L9:
				pop				{r0, r4, r5}
				bx				lr

.L2:
    // New block is bigger than the old block, so just mm_malloc() a new
    // one, copy the old data to the new block, and free up the old block,
    // but only if the malloc was sucessful!
    			mov				r4, lr
				lsls			r5, r2, #2
    			mov				r0, r12
    			bl				svc_malloc
    			movs			r1, r0
    			ldr				r0, [sp, #0]
    			str				r1, [sp, #0]
    			cbz				r1, .L4
.L3:
				subs			r5, #4
				ldr				r3, [r0, r5]
				str				r3, [r1, r5]
				bhi				.L3

.L4:
				bl				svc_free
				mov				lr, r4
				pop				{r0, r4, r5}
				bx				lr

.L5:
				mov				r0, r1
    			b 				svc_malloc

ENDF(	svc_realloc		)

.end

