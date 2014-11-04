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
// Date:          2014-09-13
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

/*
---------- Memory info --------------------
end:
 20008F00 0002 0000  2322
 20008F08 2319 0002  3C3C D456
 20011B6C 0007 2319                0002 0010
 20011B88 0006 0007  0744 2322
 20011BA0 000C 0006                EAD0 0007
 20011BD0 000B 000C                EB30 0007
 20011BFC 000B 000B                EB30 0007
 20011C28 000B 000B                EB30 0007
 20011C54 000C 000B                E908 0007
 20011C84 041E 000C                E8D8 0007
 20012CFC 015E 041E                3FEC A55F
 20013274 0055 015E                DC90 0007
 200133C8 0121 0055                2530 0008
 2001384C 000B 0121                EB30 0007
 20013878 0008 000B                1C58 2001
 20013898 0006 0008  0146 0744
 200138B0 0013 0006                27D0 0008
 200138FC 0009 0013                0426 CE25
 20013920 0121 0009                2530 0008
 20013DA4 0003 0121                0001 0001
 20013DB0 0002 0003  D456 0146
 20013DB8 0006 0002                3080 0008
 20013DD0 0003 0006                0001 0001
 20013DDC 0003 0003                0001 0001
 20013DE8 0003 0003                3068 0008
 20013DF4 0009 0003                0126 0E4D
 20013E18 0006 0009                E538 0007
 20013E30 0006 0006                0001 0004
 20013E48 047E 0006                8A83 B8E5
 20015040 0030 047E                E768 0007
 20015100 047E 0030                B02C 3AFB
 200162F8 012C 047E                2FEE FEC1
 200167A8 009E 012C                F3A0 0007
 20016A20 0007 009E                D6D4 0007
 20016A3C 0029 0007                D760 0007
 20016AE0 00FA 0029                27CA B8DB
 20016EC8 0226 00FA                E6AB 11F2
 20017760 0226 0226                7F57 DF4D
 20017FF8 0000 0226                0000 3C3C

 ---------- check empty holes --------------------
 20011B88 0006 0007 0744 2322
 20013898 0006 0008 0146 0744
 20013DB0 0002 0003 FFFFD456 0146
 20008F08 2319 0002 3C3C FFFFD456
 20017FF8 0000 0226 0000 3C3C
 ---------- Memory info --------------------
 used 25684 bytes in 34 ptrs
 free 35996 bytes in 4 holes
*/


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
				str				r2,[r0]
				adds			r0, #8
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
				lsrs			r3, r1, #3				// if zero (C Clear) then
				bcc				1f						// Main Stack was used, put MSP in R2
				mrs				r2, PSP					// Process Stack was used
				str				r0, [r2, tsk_sp_r0]
				bx				r1
1:
				mrs				r2, MSP
				str				r0, [r2, tsk_sp_r0]
				bx				r1

ENDF(	sys_malloc		)

//------------------------------------------------------------------------------
//void * sys_realloc(void* ptr, unsigned int size)
FUNC(	sys_realloc		)

				push			{lr}
				bl				svc_realloc
				pop				{r1}
				mrs				r2, MSP
				lsrs			r3, r1, #3				// if zero (C Clear) then
				bcc				1f						// Main Stack was used, put MSP in R2
				mrs				r2, PSP					// Process Stack was used
1:
				str				r0, [r2, tsk_sp_r0]
				bx				r1

ENDF(	sys_realloc		)

//------------------------------------------------------------------------------
//void* svc_malloc(unsigned int size);
FUNC(	svc_malloc		)

				ldr				r1, =end				//pool
				b				svc_pool_malloc
ENDF(	svc_malloc		)


#define best		r0
#define bptr		r1
#define bestsize	r12

//------------------------------------------------------------------------------
//void* svc_pool_malloc(unsigned int size, void* pool);
FUNC(	svc_pool_malloc		)					// r0    r1    r2    r3     r12

				cmp				r0, #0
				beq				sma_exit
				adds			r2, r0, #7
				subs			r3, r0, r2		// size	 pool 		 -7
				lsrs			r2, #2			// size  pool  wsize -7
sma_set_best:
				mov				r0, r1
				mov				r12, r3			// bptr	 aptr  wsize asize  bsize
sma_search_loop:
				ldrh			r3, [r1, #4]
				sxth			r3, r3
				lsls			r3, #2
				adds			r1, r3
				ldrh			r3, [r1]
				cmp				r3, r12
				bhi				sma_search_loop			//bsize > bestsize
				cmp				r3, r2
				bhi				sma_set_best			// asize < bsize <= bestsize
				beq				sma_match				// bsize == asize
				cmp				r3, #0
				bne				sma_search_loop			// 0 < bsize < asize
				//bsize=0						   r0    r1    r2    r3     r12
				mov				r1, r12			// bptr        wsize		bsize
				adds			r3, r1, #7
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
				subs			r3, r1, r2
				cmp				r3, #1
				beq				7f				// bptr        wsize  remain bsize
				strh			r3, [r0]					// cf-> bs-s
				lsls			r1, r3, #17
				adds			r1, r2					// bs-s<   s
				lsls			r3, #2
				adds			r0, r3
				str				r1, [r0]
				adds			r0, #4
#ifdef USE_MEMORY_TRACKING
#if USE_TASK_REGISTERS
                mov             r1, REG_MTASK
#else
                ldr             r1, =main_task
#endif
				ldr				r3, [r1, #tsk_aloc_mem]
				adds			r3, r2
				str				r3, [r1, #tsk_aloc_mem]
				ldr				r3, [r1, #tsk_aloc_ptrs]
				adds			r3, #1
				str				r3, [r1, #tsk_aloc_ptrs]
#endif
				lsls			r3, r2, #1
				lsls			r2, #2
				subs			r1, r0, #2
				strh			r3, [r1, r2]			// n -> s<
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
												// r0    r1    r2    r3     r12
												//		 ptr   wsize
				movs			r0, r1
7:
#ifdef USE_MEMORY_TRACKING
                //r0 = best [r1=bptr r2=asize r3=bsize]
#if USE_TASK_REGISTERS
                mov             r1, REG_MTASK
#else
                ldr             r1, =main_task
#endif
				ldr				r3, [r1, #tsk_aloc_mem]
				ldrh			r2, [r0]
				adds			r3, r2
				str				r3, [r1, #tsk_aloc_mem]
				ldr				r3, [r1, #tsk_aloc_ptrs]
				adds			r3, #1
				str				r3, [r1, #tsk_aloc_ptrs]
#endif
				ldrh			r3, [r0, #2]
				subs			r3, #1
				strh			r3, [r0, #2]	//cf: p1=>p0
				adds			r0, #4
				ldr				r3, [r0]
				sxth			r1, r3				//nf
				asrs			r3, #16				//pf
				adds			r1, r3
				lsls			r3, #2
				subs			r3, r0, r3
				strh			r1, [r3]
				adds			r3, #2
				lsls			r2, r1, #2
				strh			r1, [r3, r2]
				bx 				lr

sma_fail:
				movs			r0, #0
				bx				lr

ENDF(	svc_pool_malloc		)



//------------------------------------------------------------------------------
//void svc_free(void* ptr);
FUNC(	svc_free		)

				ldr				r1, =end
				b 				svc_pool_free

ENDF(	svc_free		)


//------------------------------------------------------------------------------
//void svc_pool_free(void* ptr, void* pool);
FUNC(	svc_pool_free		)						// r0    r1    r2    r3		r12
//------------------------------------------------------------------------------
													// ptr  pool
				cmp				r0, r1
				bls				smf_exit  //BAD ptr!
//check next
				subs			r0, #4
				mov				r1,	r12
				ldrh			r1, [r0]			// ptr  size				pool
#ifdef USE_MEMORY_TRACKING
#if USE_TASK_REGISTERS
                mov             r2, REG_MTASK
#else
                ldr             r2, =main_task
#endif
				ldr				r3, [r2, #tsk_aloc_mem]
				subs			r3, r1
				str				r3, [r2, #tsk_aloc_mem]
				ldr				r3, [r2, #tsk_aloc_ptrs]
				subs			r3, #1
				str				r3, [r2, #tsk_aloc_ptrs]
#endif
				lsls			r2, r1, #2
				ldr				r3, [r0, r2]
				lsls			r3, #16				// ptr 	wsize   size fs<<16 pool
				bcc				smf_check_prev
//    +----+----+----+----+              +------+-----+-----+-----+
// 	p | ?? |*?? | pf | ?? |            p |      |     |pf+nf|	  |
//    +----+----+----+----+              +------+-----+-----+-----+
//        ..          .                            ...
//    +----+----+----+----+              +------+-----+-----+-----+ This block is
// r0 | cs | cp ..   |   .|           r0 | cs+fs|     |    ...    | disconnected
//    +----+----+----+----+              +------+-----+-----+-----+ from free list,
//    +----+----+----+----+                                         assimilated with
// cf | fs |cs1 | nf | pf |                                         the next, and
//    +----+----+----+----+                                         ready for step 2
//    +----+----+----+----+              +------+-----+-----+-----+
// nn | ?? | fs<| ?? | ?? |           nn |		|cs+fs<|    ...    |
//    +----+----+----+----+              +------+-----+-----+-----+
//        ..          .                            ...
//    +----+----+----+----+              +------+-----+-----+-----+
//  n | ?? |*?? | ?? | nf |            n |      |     |		|pf+nf|
//    +----+----+----+----+              +------+-----+-----+--- -+

//assimilate next
				lsrs			r3, #16
				adds			r3, r1	//r3 = cs+fs
				strh			r3, [r0]			// ptr  wsize size  cs+fs	pool

				lsls			r3, #1
				lsls			r1, r3, #1
				adds			r1, r0
				strh			r3, [r1, #2]	//nn->prev = cs+fs<

				adds			r2, r0			//r2=cf
				ldr				r1, [r2, #4]
				sxth			r3, r1			//r3=cf->nf
				asrs			r1, #16			//r1=cf->pf
				adds			r3, r1			//r3=pf+nf
				lsls			r1, #2
				subs			r2, r1			//r2=p
				strh			r3, [r2, #4]	//p->nf = pf+nf
				lsls			r1, r3, #2
				adds			r2, r1
				strh			r3, [r2, #6]	//n->pf = pf+nf

smf_check_prev:										// ptr 	                    pool
//    +----+----+----+----+              +------+-----+----+----+ This block has
// p  | cp | ??*| ?? | nf |           p  | ??*  |cp+cs|    |    | assimilated the
//    +----+----+----+----+              +------+-----+----+----+ current block
//    +----+----+----+----+
// r0 | cs | cp<|   ...   |
//    +----+----+----+----+
//    +----+----+----+----+              +------+-----+----+----+
//  n | ?? | cs<|   ...   |            n |cp+cs<|     |    |    |
//    +----+----+----+----+              +------+-----+----+----+
				ldrh			r2, [r0, #2]	// r2=cp<
				lsls			r1, r2, #1
				subs			r1, r0, r1		// r1=p
				ldr				r3, [r1]
				lsls			r3, #16 		// r3=cp<<16
				bcc				smf_free
//assimilate prev									// ptr 	p      cp<          pool
				ldrh			r2, [r0]		//r2 = cs
				lsrs			r3, #16
				adds			r2, r3			//r2 = cp+cs
				strh			r2, [r1]		//p->n = cs+cp
				lsls			r2, #1
				lsls			r3, r2, #1
				adds			r1, r3
				strh			r2, [r1, #2]	//n->pf = cs+cp<
				movs			r0, #0
smf_exit:		bx				lr


3:smf_free:											// r0    r1    r2    r3		r12
													// ptr         wsz<			pool
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
				mov				r1, r12
				subs			r2, r0, r1
				asrs			r2, r2, #2			// r2 = c
				ldrh			r3, [r1, #4]		// r3 = nf
				strh			r2, [r1, #4] 		//0->fn = c
				strh			r2, [r0, #6]		//r0->pf= c
				subs			r2, r3, r2			// r2 = nf-c
				strh			r2, [r0, #4]		//r0->nf=nf-c
				lsls			r3, #2
				adds			r1, r3
				strh			r2, [r1, #6]		//nf->pf= nf-c
				movs			r0, #0
				bx				lr
ENDF(	svc_pool_free		)




//------------------------------------------------------------------------------
//void * svc_realloc(void* ptr, unsigned int size)
FUNC(	svc_realloc		)					// r0    r1    r2    r3	   r4	 r5
//------------------------------------------------------------------------------

				cmp				r1, #0		// ptr   nsz
				beq				svc_free		// realloc(ptr, 0)	-> free(ptr)
				cmp				r0, #0
				beq				j_svc_malloc	// realloc(NULL, nsz)	-> malloc(nsz)

				adds			r1, #7
				lsrs			r1, #2			// ptr 	 nszw
				subs			r3, r0, #4
				ldrh			r2, [r3]	// ptr4	nszw   cs    ptr
				cmp				r1, r2
				beq				smr_exit		//size match
//check next
#ifdef USE_MEMORY_TRACKING
				push			{r0, r4, r5, r7}
#if USE_TASK_REGISTERS
				mov 			r7, REG_MTASK
#else
                ldr             r7, =main_task
#endif

#else
				push			{r0, r4, r5}
#endif
				lsls			r4, r2, #2
				ldr				r5, [r3, r4]
				lsls			r5, #16				//r5=fs<<16
				bcc				smr_check_split

//    +----+----+----+----+              +------+-----+-----+-----+
// p  |*?? | ?? | ?? | pf |           p  |      |     |     |pf+nf|
//    +----+----+----+----+              +------+-----+-----+-----+
//             ...                                 ...
//    +----+----+----+----+              +------+-----+-----+-----+ This block is
// r3 | cp | cs |   ...   |           r3 |      |cs+fs|    ...    | disconnected
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
//assimilate next							// r0    r1    r2    r3	   r4	 r5
				lsrs			r5, #16		// ptr4 nszw   cs	 ptr   csb   fs
				adds			r2, r5		//            cs+fs
				strh			r2, [r3]			//	r3->next = cs+fs

				lsls			r2, #1
				adds			r3, r2
				adds			r3, r2
				strh			r2, [r3, #2]		//nn->prev = (cs+fs)<<1
#ifdef USE_MEMORY_TRACKING
                ldr             r3, [r7, #tsk_aloc_mem]
                adds            r3, r5
                str             r3, [r7, #tsk_aloc_mem]
#endif
											// r0    r1    r2    r3	   r4	 r5
											// ptr4 nszw   os< 	       csb
				ldr				r5, [r0, r4]
				sxth			r3, r5			//r3=cf->nf
				asrs			r5, #16			//r5=cf->pf
				adds			r3, r5			//r3=pf+nf
				lsls			r5, #2
				subs			r4, r5			//r4=p
				strh			r3, [r0, r4]	//p->nf = pf+nf
				lsls			r5, r3, #2
				adds			r4, r5
				adds			r4, r0
				strh			r3, [r4, #2]	//n->pf = pf+nf
				lsrs			r2, #1

smr_check_split:	//	r0, , r1=newsize_w r2=oldsize_w
				subs			r3, r2, r1			//r3=cs-ns
				beq				smr_exit_pop		//size match
				blo				smr_realloc
				//the current block is bigger
				cmp				r3, #1
				beq				smr_exit_pop		//can't free
//    +----+----+----+----+              +------+-----+----+----+
//  c |    | cs |   ...   |            c |      | ns  |   ...   |
//    +----+----+----+----+              +------+-----+----+----+
//                                       +------+-----+----+----+ This is the
//                                     s | ns<1 |cs-ns|   ..    | new block at
//                                       +------+-----+----+----+ c+blocks
//    +----+----+----+----+              +------+-----+----+----+
//  n | cs<| ?? |   ...   |            n |cs-ns<|     |   ...   |
//    +----+----+----+----+              +------+-----+----+----+

#ifdef USE_MEMORY_TRACKING
				ldr				r4, [r7, #tsk_aloc_mem]
				subs			r4, r3
				str				r4, [r5, #tsk_aloc_mem]
#endif
				lsls			r4, r3, #2
				subs			r0, #4
				strh			r3, [r0]				//c->size = newsize
				adcs			r3, r3
				lsls			r3, #16
				adds			r3, r1					// r3  = ns<1 cs-ns
				str				r3, [r0, r4]			//s->size = oldsize -newsize, s->prev= newsize << 1
				adds			r0, r4					//r0=s
				lsls			r3, #1
				lsls			r4, r3, #1
				adds			r4, r0
				strh			r3, [r4, #2]			//n->prev = (oldsize - newsize)<<1

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
				ldrh			r3, [r1, #4]
				strh			r2, [r1, #4]		//mem0->fn = c
				strh			r2, [r0, #6]		//r0->pf= c
				subs			r2, r3, r2
				strh			r2, [r0, #4]		//r0->nf=nf-c
				lsls			r3, #2
				adds			r3, r1
				strh			r2, [r3, #6]		//nf->pf= nf-c


smr_exit_pop:
#ifdef USE_MEMORY_TRACKING
				pop				{r0, r4, r5, r7}
#else
				pop				{r0, r4, r5}
#endif
smr_exit:
				bx				lr

j_svc_malloc:
				mov				r0, r1
				ldr				r1, =end				//pool
				b				svc_pool_malloc




smr_realloc:
    // New block is bigger than the old block, so just mm_malloc() a new
    // one, copy the old data to the new block, and free up the old block,
    // but only if the malloc was sucessful!
    			mov				r4, lr
				lsls			r5, r2, #2
				lsls			r0, r1, #2
				subs			r0, #1
    			bl				svc_malloc
    			movs			r1, r0
    			ldr				r0, [sp, #0]
    			str				r1, [sp, #0]
    			beq				.L4
.L3:
				subs			r5, #4
				ldr				r3, [r0, r5]
				str				r3, [r1, r5]
				bhi				.L3

.L4:
				bl				svc_free
				mov				lr, r4
				b				smr_exit_pop


ENDF(	svc_realloc		)

.end

