/*
 * dma_f2.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: miro
 */
#include <tmos.h>
#include "fam_cpp.h"

/*
 * CMD		src  dst		P  M
 *  R		 x	  M         M  x		- 1memory to memory 	fill memory
 *  R        x    P			P  x		- periph to 1memory		drain periph
 *  W		 M	  x			x  M		- memory to 1memory     nonsense
 *  W        P    x			P  x		- 1memory to periph  	fill peroph
 * RW		 M	  P			P  M		- memory to periph
 * RW        P    M			P  M		- periph to memory
 * RW        M    M         dM sM		- memory to memory
 *
 *
 *	//mode= periph to memory
 *	hnd.src.as_voidptr = P;
 *
 *	tsk_read(dst, len); or tsk_read_write(dst, P, len);
 *
 *	tsk_read(NULL, len); or tsk_read_write(NULL, P, len);
 *
 *
 *
 */

/* ****************************************************************************
 * 		DMA usage
 *
 * When the handle is open one of the following modes must be selected
 *
 * 1) peripheral to memory
 *
 * 		tsk_write() - no sense
 *
 * 		hnd.src.as_voidptr = PA(peripheral adress); //set once
 * 		hnd.tsk_read(ptr,len);						//then read multiple times
 * 		or
 * 		hnd.tsk_read_write(ptr, PA, len);
 *
 * 		Note: ptr can be NULL (to drain peripherals)
 *
 *
 * 2) memory to peripheral
 *
 * 		tsk_read() - no sense
 *
 * 		hnd.dst.as_voidptr = PA;
 * 		hnd.tsk_write(ptr,len);
 * 		or
 *		hnd.tsk_read_write(PA, ptr, len);
 *
 * 		Note: ptr can be <=0xFF (to fill peripherals)
 *
 * 3) memory to memory
 *
 * 		hnd.src.as_voidptr = src;
 * 		hnd.tsk_read(dst,len);
 * 		or
 * 		hnd.dst.as_voidptr = dst;
 * 		hnd.tsk_write(src,len);
 * 		or
 *		hnd.tsk_read_write(dst, src, len);
 *
 * 		Note: src can be <=0xFF (to fill memory)
 *
 * ****************************************************************************/

void stm32_dma_start(DMA_TypeDef* dmac, uint32_t indx, HANDLE hnd)
{
	DMA_Stream_TypeDef* ch;

	ch = &dmac->DMA_Chx[indx];

	ch->DMA_SxNDTR = hnd->len;

	if( (hnd->src.as_int >> 28) == (BASE_PERIPHERAL >> 28))
	{
		//reading from "peripheral" (peripheral to memory)
		ch->DMA_SxPAR = hnd->src.as_int;
		if(hnd->dst.as_int)
		{
			ch->DMA_SxM0AR = hnd->dst.as_int;
			ch->DMA_SxCR |= DMA_SxCR_MINC;
		}
		else
		{
			ch->DMA_SxM0AR = (uint32_t)&hnd->dst.as_int;
			ch->DMA_SxCR &= ~DMA_SxCR_MINC;
		}
	} else
	{
		//writing to "peripheral" (memory to peripheral or memory to memory)
		ch->DMA_SxPAR = hnd->dst.as_int;
		if(hnd->src.as_int <= 0xFF)
		{
			ch->DMA_SxM0AR = (uint32_t)&hnd->src.as_int;
			ch->DMA_SxCR &= ~DMA_SxCR_MINC;
		} else
		{
			ch->DMA_SxM0AR = hnd->src.as_int;
			ch->DMA_SxCR |= DMA_SxCR_MINC;
		}
	}

	// Enable DMA
	ch->DMA_SxCR |= DMA_SxCR_EN ;
}

void stm32_dma_stop(DMA_TypeDef* dmac, uint32_t indx)
{
	DMA_Stream_TypeDef* ch;

	ch = &dmac->DMA_Chx[indx];
	ch->DMA_SxCR &= ~DMA_SxCR_EN;
}

void stm32_dma_ch_cfg(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode)
{
	DMA_Stream_TypeDef* ch;

	ch = &dmac->DMA_Chx[indx];

	ch->DMA_SxCR = (mode->dma_ch_cr & ~DMA_SxCR_RESERVED_NOINTS);
	ch->DMA_SxFCR = mode->dma_ch_fr;

	// Enable DMA interrupts
//	if(mode->dma_ch_cr & DMA_SxCR_CIRC)
//		ch->DMA_SxCR |= DMA_SxCR_DMEIE | DMA_SxCR_TEIE;
//	else
//		ch->DMA_SxCR |= DMA_SxCR_TCIE | DMA_SxCR_DMEIE | DMA_SxCR_TEIE;

}

void stm32_en_ints(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode)
{
	DMA_Stream_TypeDef* ch;
	uint32_t reg;

	ch = &dmac->DMA_Chx[indx];
	reg = mode->dma_ch_cr;
	if( !(reg & DMA_SxCR_RESERVED_NOINTS))
		ch->DMA_SxCR |= (reg & DMA_SxCR_HTIE) |  DMA_SxCR_DMEIE |
			DMA_SxCR_TEIE | DMA_SxCR_TCIE;

}

void stm32_dis_ints(DMA_TypeDef* dmac, uint32_t indx)
{
	dmac->DMA_Chx[indx].DMA_SxCR &= ~(DMA_SxCR_TCIE | DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_EN);
}


