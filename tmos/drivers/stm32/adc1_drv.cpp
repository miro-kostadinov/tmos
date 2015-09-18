/*
 * adc1_drv.cpp
 *
 *  Created on: Sep 12, 2015
 *      Author: miro
 */

#include <tmos.h>
#include <adc_f0.h>
#include <adc1_drv.h>


static RES_CODE ADC_START_HND(const ADC_DRIVER_INFO* drv_info, HANDLE hnd)
{
	RES_CODE res;
	uint32_t reg, tout = 255;
	ADC_TypeDef* adc = drv_info->hw_base;
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;
	const ADC_DRIVER_MODE* mode = (const ADC_DRIVER_MODE *)hnd->mode.as_voidptr;

	if(hnd->len)
	{
		// start this adc?
		reg = adc->ADC_CR;
		while(reg & ADC_CR_BUSY)
		{
			if(!tout--)
			{
				TRACELN1("ADC TOUT!");
				return RES_SIG_ERROR;
			}
			TRACELN("ADC_CR=%08x", reg);
			reg = adc->ADC_CR;
		}
		if(reg & ADC_CR_ADSTART)
		{
			// ADC is working...
			if(mode != drv_data->adc_mode)
			{
				hnd->list_add(drv_data->waiting);
				return RES_IDLE;
			}
		} else
		{
			if((reg & ADC_CR_ADEN) && (mode != drv_data->adc_mode))
			{
				//stop
				adc->ADC_CR = ADC_CR_ADDIS;
				tout = 255;
				reg = adc->ADC_CR;
				while(reg & ADC_CR_BUSY)
				{
					if(!tout--)
					{
						TRACELN1("ADC TOUT!");
						return RES_SIG_ERROR;
					}
					TRACELN("ADC_CR=%08x", reg);
					reg = adc->ADC_CR;
				}
			}
			if(!(reg & ADC_CR_ADEN))
			{
				// Enable ADC
				adc->ADC_CR = ADC_CR_ADEN;
				adc->ADC_IER = ADC_IER_ADRDYIE;
				hnd->next = drv_data->pending;
				drv_data->pending = hnd;
       			drv_data->adc_mode = mode;
				return RES_IDLE;
			}

			//Start ADC
			adc->ADC_CFGR1 = mode->adc_cfgr1;
			switch( mode->adc_cfgr1 & (ADC_CFGR1_ALIGN | ADC_CFGR1_RES))
			{
			case ADC_CFGR1_RES_8:
			case ADC_CFGR1_RES_6:
			case ADC_CFGR1_ALIGN | ADC_CFGR1_RES_6:
				drv_data->sample_size =0;
				break;
			default:
				drv_data->sample_size =1;
				break;
			}
			adc->ADC_SMPR = mode->adc_smpr;
			adc->ADC_CHSELR = mode->adc_chselr;
			adc->ADC_CCR = mode->adc_ccr;
			adc->ADC_TR = mode->adc_tr;
			if(mode->adc_tr)
				adc->ADC_IER |= ADC_IER_AWDIE;
			else
				adc->ADC_IER &= ~ADC_IER_AWDIE;

#if USE_ADC_DMA_DRIVER
			if((mode->adc_cfgr1 & ADC_CFGR1_DMAEN) && drv_data->dma_hnd.res < RES_BUSY)
			{
				adc->ADC_IER &= ~(ADC_IER_EOCIE | ADC_IER_EOSEQIE);
				adc->ADC_CFGR1 |= ADC_CFGR1_DMAEN;
			}
			else
#endif
			{
				adc->ADC_CR |= ADC_CR_ADSTART;
			}

		}
		// make it pending
		hnd->next = drv_data->pending;
		drv_data->pending = hnd;
#if USE_ADC_DMA_DRIVER
		if((mode->adc_cfgr1 & ADC_CFGR1_DMAEN) && drv_data->dma_hnd.res < RES_BUSY)
		{
			drv_data->dma_hnd.drv_read_write(
					hnd->dst.as_voidptr, (void*)&adc->ADC_DR, hnd->len>>drv_data->sample_size);
		}
		else
#endif
		{
			adc->ADC_IER |= ADC_IER_EOCIE | ADC_IER_EOSEQIE;
		}
		res = RES_IDLE;
	} else
		res = RES_SIG_OK; //not enough length - done!

	return res;
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void ADC_DCR(const ADC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	const ADC_DRIVER_MODE* mode;
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
			RCCPeripheralReset(ID_PERIPH_ADC);
			RCCPeripheralDisable(ID_PERIPH_ADC);
			break;

        case DCR_OPEN:

        	mode = (ADC_DRIVER_MODE*)hnd->mode.as_voidptr;
        	if(mode)
        	{
        		// first client for the adc or mode is the same?
        		if(!drv_data->cnt++)
        		{
        			rcc_cfg_adc_clock(drv_info->adc_clk);

           			RCCPeripheralEnable(drv_info->info.peripheral_indx);


           			// calibrate
           			if(!drv_data->adc_mode)
           			{
           				if(!drv_info->hw_base->ADC_CR)
           					drv_info->hw_base->ADC_CR = ADC_CR_ADCAL;
           			}

#if USE_ADC_DMA_DRIVER
        				// enable DMA controller?
					if(!drv_data->dma_hnd.drv_open(drv_info->dma_mode.dma_index,
								&drv_info->dma_mode))
					{
						drv_data->cnt--;
		        		break;
					}

#endif


       				drv_enable_isr(&drv_info->info);
        			PIO_Cfg_List(mode->adc_pins);
        		}

				hnd->res = RES_OK;
        	}
        	break;

		case DCR_CANCEL:
			break;

#if USE_ADC_DMA_DRIVER
		case DCR_SIGNAL:
			//signal dma complete

			hnd = drv_data->pending;
			if(hnd)
			{
				RES_CODE res;

				//pending is done
				drv_data->pending = hnd->next;

				//resume waiting
				res = hnd->res & (FLG_DATA | FLG_ERROR | ADC_ISR_EOSEQ);
				svc_HND_SET_STATUS(hnd, res | RES_SIG_OK);

				hnd = drv_data->pending;
				if(hnd)
				{
					drv_data->pending = hnd->next;
				} else
				{
					hnd = drv_data->waiting;
					if(hnd)
					{
						drv_data->waiting = hnd->next;
					}
				}
				if(hnd)
				{
					res = ADC_START_HND(drv_info, hnd);
					if(res & FLG_SIGNALED)
						svc_HND_SET_STATUS(hnd, res);
				}
			}
			break;
#endif

		case DCR_CLOSE:
        	mode = (const ADC_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode)
        	{
    			if(!--drv_data->cnt)
    			{
    				//stop

        			NVIC_DisableIRQ(drv_info->info.drv_index);
        			RCCPeripheralDisable(drv_info->info.peripheral_indx);
#if USE_ADC_DMA_DRIVER
					// close DMA handle
					drv_data->dma_hnd.close();
#endif
    			}
        	}
        	break;
   }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void ADC_DSR(const ADC_DRIVER_INFO* drv_info, HANDLE hnd)
{
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;
	const ADC_DRIVER_MODE * mode;

	mode = (const ADC_DRIVER_MODE *)hnd->mode.as_voidptr;
	if(mode && (hnd->cmd & FLAG_READ) )
	{
		if(	drv_data->pending)
		{
			//adc is busy
			if(mode != drv_data->adc_mode)
				hnd->list_add(drv_data->waiting);
			else
				hnd->list_add(drv_data->pending);
		} else
		{
			RES_CODE res;

			res = ADC_START_HND(drv_info, hnd);
			if(res & FLG_SIGNALED)
				svc_HND_SET_STATUS(hnd, res);
			else
				hnd->res  = RES_BUSY;
		}
	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	}

}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void ADC_ISR(const ADC_DRIVER_INFO* drv_info)
{
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;
	uint32_t status;
	HANDLE hnd;
	RES_CODE res;

	status = drv_info->hw_base->ADC_ISR;

//	TRACELN("ADC-%x", status);
	if(status)
	{
		hnd = drv_data->pending;
		if(status & ADC_ISR_ADRDY)
		{
			drv_info->hw_base->ADC_ISR = ADC_ISR_ADRDY;
			if(hnd)
			{
				drv_data->pending = hnd->next;

				res = ADC_START_HND(drv_info, hnd);
				if(res & FLG_SIGNALED)
					usr_HND_SET_STATUS(hnd, res);
			}
		} else
		{
			drv_info->hw_base->ADC_ISR = status;
			if(hnd)
			{
				res = hnd->res & (FLG_DATA | FLG_ERROR | ADC_ISR_EOSEQ);
				if(status & ADC_ISR_AWD)
					res |= FLG_DATA;

				if(status & ADC_ISR_OVR)
					res |= FLG_ERROR;

				if(status & ADC_ISR_EOSEQ)
					res |= FLG_EOF;

				if(status & ADC_ISR_EOC)
					res |= FLG_OK;

				if(res & FLG_OK)
				{
					if(drv_data->sample_size)
					{
						// 16-bit
						if(hnd->len > 1)
						{
							*hnd->dst.as_shortptr++ = drv_info->hw_base->ADC_DR;
							hnd->len -= 2;
						}

					} else
					{
						if(hnd->len)
						{
							*hnd->dst.as_byteptr++ = drv_info->hw_base->ADC_DR;
							hnd->len -= 1;
						}
					}
					if(hnd->len <= drv_data->sample_size)
					{
						//done
						drv_data->pending = hnd->next;
						usr_HND_SET_STATUS(hnd, res | FLG_SIGNALED);

						hnd = drv_data->pending;
						if(hnd)
						{
							drv_data->pending = hnd->next;
						} else
						{
							hnd = drv_data->waiting;
							if(hnd)
							{
								drv_data->waiting = hnd->next;
							}
						}
						if(hnd)
						{
							res = ADC_START_HND(drv_info, hnd);
							if(res & FLG_SIGNALED)
								usr_HND_SET_STATUS(hnd, res);
						}
						return;
					}
				}

				hnd->res |= res;

			}
		}
	}
}


