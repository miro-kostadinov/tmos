/*
 * dma_f2.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: miro
 */
#include <tmos.h>
#include "fam_cpp.h"


void stm32_dma_start(DMA_TypeDef* dmac, uint32_t indx, HANDLE hnd)
{
	DMA_Stream_TypeDef* ch;

	ch = &dmac->DMA_Chx[indx];

	ch->DMA_SxNDTR = hnd->len;

	if( (hnd->src.as_int >> 28) == (PERIPH_BASE >> 28))
	{
		//reading from "peripheral"
		ch->DMA_SxPAR = hnd->src.as_int;
		ch->DMA_SxM0AR = hnd->dst.as_int;
	} else
	{
		//writing to "peripheral"
		ch->DMA_SxPAR = hnd->dst.as_int;
		ch->DMA_SxM0AR = hnd->src.as_int;
	}

	// Enable DMA
	ch->DMA_SxCR |= DMA_SxCR_EN ;
}

void stm32_dma_ch_cfg(DMA_TypeDef* dmac, uint32_t indx, DMA_DRIVER_MODE* mode)
{
	DMA_Stream_TypeDef* ch;

	ch = &dmac->DMA_Chx[indx];

	ch->DMA_SxCR = mode->dma_ch_cr;
	ch->DMA_SxFCR = mode->dma_ch_fr;

	// Enable DMA interrupts
	ch->DMA_SxCR |= DMA_SxCR_TCIE | DMA_SxCR_DMEIE | DMA_SxCR_TEIE;

}

void stm32_dis_ints(DMA_TypeDef* dmac, uint32_t indx)
{
	dmac->DMA_Chx[indx].DMA_SxCR &= ~(DMA_SxCR_TCIE | DMA_SxCR_DMEIE | DMA_SxCR_TEIE | DMA_SxCR_EN);
}


