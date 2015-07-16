/*
 * dac_drv.cpp
 *
 *  Created on: Jul 10, 2015
 *      Author: miro
 */

#include <tmos.h>
#include <dac_drv.h>
#if USE_USE_DAC_DMA_DRIVER
#include <dma_drv.h>
#endif

static void ConfigureDacChannel(DAC_TypeDef* dac, const DAC_DRIVER_MODE* mode)
{

	if(mode->dac_ch == 0)
	{
		//chanel 0
		dac->DAC_CR =  (dac->DAC_CR & 0xFFFF0000) | mode->dac_cr;
		if((mode->dac_cr & DAC_CR_TSEL1) == DAC_CR_TSEL1)
		{
			//software triger
			dac->DAC_SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
		} else
		{
			dac->DAC_SWTRIGR &= ~DAC_SWTRIGR_SWTRIG1;
		}
	} else
	{
		//chanel 1
		dac->DAC_CR =  (dac->DAC_CR & 0xFFFF) | (mode->dac_cr << 16);
		if((mode->dac_cr & DAC_CR_TSEL1) == DAC_CR_TSEL1)
		{
			//software triger
			dac->DAC_SWTRIGR |= DAC_SWTRIGR_SWTRIG2;
		} else
		{
			dac->DAC_SWTRIGR &= ~DAC_SWTRIGR_SWTRIG2;
		}
	}
}

#if USE_DAC_DMA_DRIVER
static bool ConfigureDacDma(HANDLE dma_hnd, const DMA_DRIVER_MODE* dma_mode)
{
	if(dma_mode->dma_index < INALID_DRV_INDX)
	{
		if(dma_hnd->res < RES_CLOSED)
		{
			dma_hnd->hcontrol(DCR_CANCEL);
			dma_hnd->close();
		}
		return dma_hnd->drv_open(dma_mode->dma_index, dma_mode);
	}
	return true;
}
#endif

static bool ConfigureDac(DAC_DRIVER_INFO * drv_info)
{
	bool res = true;
	// Enable AND Reset the DAC peripheral
	RCCPeripheralEnable(drv_info->info.peripheral_indx);
	RCCPeripheralReset(drv_info->info.peripheral_indx);
	PIO_Cfg_List(drv_info->dac_pins);
	drv_enable_isr(&drv_info->info);
#if USE_DAC_DMA_DRIVER
	if(!ConfigureDacDma(&drv_info->drv_data->dac_dma_hnd[0], &drv_info->dac_dma_mode[0]))
		res = false;
	if(!ConfigureDacDma(&drv_info->drv_data->dac_dma_hnd[1], &drv_info->dac_dma_mode[1]))
		res = false;
#endif
	return res;
}

void DAC_DCR(DAC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	DAC_DRIVER_DATA *drv_data = drv_info->drv_data;
	DAC_DRIVER_MODE *dac_mode = (DAC_DRIVER_MODE *)(hnd->mode.as_voidptr);

	switch(reason)
	{
		case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
			RCCPeripheralDisable(drv_info->info.peripheral_indx); // ??? turn off
#if USE_DAC_DMA_DRIVER
        	drv_data->dac_dma_hnd[0].client.drv_index = drv_info->info.drv_index;
        	drv_data->dac_dma_hnd[1].client.drv_index = drv_info->info.drv_index;
#endif
			break;

		case DCR_OPEN:
		{
			if(dac_mode && dac_mode->dac_ch <2 && drv_data->dac_mode[dac_mode->dac_ch] == NULL)
			{
				if(!drv_data->dac_mode[0] && !drv_data->dac_mode[1])
				{
					if(!ConfigureDac(drv_info))
						break;
				}
				ConfigureDacChannel(drv_info->hw_base, dac_mode);

				drv_data->dac_mode[dac_mode->dac_ch] = dac_mode;
				hnd->res = RES_OK;
			}
			break;
		}

		case DCR_CLOSE:
#if USE_DAC_DMA_DRIVER
			drv_data->dac_dma_hnd[dac_mode->dac_ch].close();
#endif
			drv_data->dac_mode[dac_mode->dac_ch] = NULL;
			if(!drv_data->dac_mode[0] && !drv_data->dac_mode[1])
			{
				//Disable ?
				NVIC_DisableIRQ(drv_info->info.drv_index);
				RCCPeripheralDisable(drv_info->info.peripheral_indx);

				PIO_Free_List(drv_info->dac_pins);
			}
			break;

		case DCR_CANCEL:
			// can only cancel dma transfers
#if USE_DAC_DMA_DRIVER
			if(drv_data->dac_dma_hnd[dac_mode->dac_ch].res & FLG_BUSY)
				drv_data->dac_dma_hnd[dac_mode->dac_ch].hcontrol(DCR_CANCEL);
#endif
			break;


		case DCR_SIGNAL:
#if USE_DAC_DMA_DRIVER
			TRACELN1("DAC sig");
			if(hnd == &drv_data->dac_dma_hnd[0])
			{

				hnd = drv_data->pending[0];
				if(hnd)
				{
					if(hnd->len > drv_data->dac_dma_hnd[0].len)
					{
						hnd->src.as_byteptr += hnd->len - drv_data->dac_dma_hnd[0].len;
						hnd->len = drv_data->dac_dma_hnd[0].len;
					}
					drv_data->pending[0] = NULL;
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				}
			} else
			{
				if(hnd == &drv_data->dac_dma_hnd[1])
				{

					hnd = drv_data->pending[1];
					if(hnd)
					{
						if(hnd->len > drv_data->dac_dma_hnd[1].len)
						{
							hnd->src.as_byteptr += hnd->len - drv_data->dac_dma_hnd[1].len;
							hnd->len = drv_data->dac_dma_hnd[1].len;
						}
						drv_data->pending[1] = NULL;
						usr_HND_SET_STATUS(hnd, RES_SIG_OK);
					}
				}
			}
#endif
			break;

	}
}

void DAC_DSR(DAC_DRIVER_INFO* drv_info, HANDLE hnd)
{
	DAC_DRIVER_DATA* drv_data = drv_info->drv_data;
	DAC_TypeDef* dac = drv_info->hw_base;
	DAC_DRIVER_MODE *dac_mode = (DAC_DRIVER_MODE *)(hnd->mode.as_voidptr);
	uint32_t* dac_reg;

	hnd->next = NULL;
	if (hnd->cmd & FLAG_WRITE)
    {
		if(hnd->len >= dac_mode->dac_size)
		{
			hnd->res = RES_BUSY;
			drv_data->pending[dac_mode->dac_ch] = hnd;
			dac_reg = (uint32_t*)(void*)(dac_mode->dac_reg + (char*)dac);
#if USE_DAC_DMA_DRIVER
			if(drv_data->dac_dma_hnd[dac_mode->dac_ch].res < RES_BUSY)
			{
				drv_data->dac_dma_hnd[dac_mode->dac_ch].drv_read_write(dac_reg,
						hnd->src.as_charptr, hnd->len);
				return;
			}
#endif
			if(dac_mode->dac_size == 1)
				*dac_reg = *hnd->src.as_charptr++;
			else
				*dac_reg = *hnd->src.as_shortptr++;
			hnd->len -= dac_mode->dac_size;
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		return;
    }

	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

void DAC_ISR(DAC_DRIVER_INFO* drv_info)
{
	DAC_DRIVER_DATA* drv_data = drv_info->drv_data;
	DAC_TypeDef* dac = drv_info->hw_base;
	uint32_t status;
	HANDLE hnd;

	status = dac->DAC_SR & (DAC_SR_DMAUDR1 | DAC_SR_DMAUDR2);
	TRACELN("DAC isr %x", status);
	if(status)//reset peripheral
	{
		// stop handles
#if USE_DAC_DMA_DRIVER
		hnd = drv_data->pending[0];
		if(hnd)
		{
			drv_data->pending[0] = NULL;
			usr_HND_SET_STATUS(hnd, RES_SIG_ERROR);
		}
		hnd = drv_data->pending[1];
		if(hnd)
		{
			drv_data->pending[1] = NULL;
			usr_HND_SET_STATUS(hnd, RES_SIG_ERROR);
		}
#endif

		// clear status
		dac->DAC_SR = status;

		//reset peripheral
		ConfigureDac(drv_info);

		//reset channels
		if(drv_data->dac_mode[0])
			ConfigureDacChannel(dac, drv_data->dac_mode[0]);
		if(drv_data->dac_mode[1])
			ConfigureDacChannel(dac, drv_data->dac_mode[1]);
	}
}


