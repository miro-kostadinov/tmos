/*
 * dma_drv.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <dma_drv.h>

extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];

uint32_t dma_drv_get_ndtr(DRIVER_INDEX drv_index)
{
	unsigned int ndtr = 0;

	if(drv_index < INALID_DRV_INDX)
	{
		DMA_DRIVER_INFO* drv_info;

		drv_info = (DMA_DRIVER_INFO*)(void*)(DRV_TABLE[drv_index]-1);
		ndtr = stm32_dma_ndtr(drv_info->hw_base, drv_info->ch_indx);
	}

	return ndtr;
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void DMA_DCR(DMA_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	DMA_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
        	if(drv_data->dma_state !=  DMA_ST_RESET)
        	{
        		// there can be one DMA Driver for each channel/stream
        		// but only the 1st channel will reset the entier DMA peripheral
        		drv_data->dma_state =  DMA_ST_RESET;
    			RCCPeripheralReset(drv_info->info.peripheral_indx);
            	RCCPeripheralDisable(drv_info->info.peripheral_indx);
        	}
        	break;

        case DCR_OPEN:

        	DMA_DRIVER_MODE * mode;
        	mode = (DMA_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode)
        	{
        		// first for this DMA?
        		if(!drv_data->cnt++)
        		{
        			// enable DMA controller
        			RCCPeripheralEnable(drv_info->info.peripheral_indx);
        		}

        		// first for this channel?
        		if(!drv_info->ch_data->cnt++)
        		{
        			// enable channel IRQ
    				drv_enable_isr(&drv_info->info);
        		}

				hnd->res = RES_OK;
        	}
        	break;

		case DCR_CANCEL:
			if(hnd->res == RES_BUSY)
			{
				DMA_CHANNEL_DATA* ch_data;

				ch_data = drv_info->ch_data;
				if(hnd == ch_data->pending)
				{
					ch_data->pending = NULL;
					ch_data->last_mode = NULL;
					stm32_dma_stop(drv_info->hw_base, drv_info->ch_indx);
					svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
				} else
					hnd->svc_list_cancel(ch_data->waiting);
			}

			break;

		case DCR_CLOSE:
			if(!--drv_info->ch_data->cnt)
    			NVIC_DisableIRQ(drv_info->info.drv_index);

			if(!--drv_info->drv_data->cnt)
				RCCPeripheralDisable(drv_info->info.peripheral_indx);
        	break;
   }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void DMA_DSR(DMA_DRIVER_INFO* drv_info, HANDLE hnd)
{
	DMA_DRIVER_MODE* mode;
	DMA_CHANNEL_DATA* ch_data;

	if(hnd->len)
	{
		ch_data = drv_info->ch_data;
		if(ch_data->pending)
		{
	    	//the DMA channel is busy with other client
	    	hnd->list_add(ch_data->waiting);
		} else
		{
			if(stm32_dma_is_en(drv_info->hw_base, drv_info->ch_indx))
			{
				// do nothing if already enabled
			} else
			{
				mode = (DMA_DRIVER_MODE *)hnd->mode.as_voidptr;

				if(mode != ch_data->last_mode)
				{
					ch_data->last_mode = mode;
					// configure the channel
					stm32_dma_ch_cfg(drv_info->hw_base, drv_info->ch_indx, mode);
				}

				// start transfer...
				stm32_dma_start(drv_info->hw_base, drv_info->ch_indx, hnd);
			}
			hnd->res  = RES_BUSY;
			ch_data->pending = hnd;

		}
	} else
	{
		// zero length... done!
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}

}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
//*
//* Note DMA driver must have higher priority than any other driver using DMA
//*
void DMA_ISR(DMA_DRIVER_INFO* drv_info)
{
	DMA_CHANNEL_DATA* ch_data;
	uint32_t status;
	HANDLE hnd;

	status = stm32_get_ints(drv_info->hw_base, drv_info->ch_indx);
	ch_data = drv_info->ch_data;
	hnd = ch_data->pending;
	if(hnd)
	{
		ch_data->pending = NULL;
		if(status & STM32_DMA_ERRORS)
		{
			hnd->error = status;
			usr_HND_SET_STATUS(hnd, RES_SIG_ERROR);
		} else
		{
			if(status & STM32_DMA_COMPLETE)
			{
				hnd->len = 0;
			} else
			{
				hnd->len = stm32_dma_ndtr(drv_info->hw_base, drv_info->ch_indx);
			}
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);
			if(status & STM32_DMA_HALF)
			{
				return;	// leave it working...
			}
		}

		// start waiting...
		if( ch_data->pending == NULL && (hnd = ch_data->waiting))
		{
			DMA_DRIVER_MODE* mode;

			ch_data->waiting = hnd->next;

			mode = (DMA_DRIVER_MODE *)hnd->mode.as_voidptr;
			if(mode != ch_data->last_mode)
			{
				ch_data->last_mode = mode;
				// configure the channel
				stm32_dma_ch_cfg(drv_info->hw_base, drv_info->ch_indx, mode);
			}

			// start transfer...
			stm32_dma_start(drv_info->hw_base, drv_info->ch_indx, hnd);
			ch_data->pending = hnd;
		}
	} else
	{
		stm32_dis_ints(drv_info->hw_base, drv_info->ch_indx);
	}
}

