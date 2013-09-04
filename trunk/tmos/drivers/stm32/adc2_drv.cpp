/*
 * adc2_drv.cpp
 *
 *  Created on: Feb 25, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <cmsis_cpp.h>
#include <adc2_drv.h>

bool ADC_START_HND(const ADC_DRIVER_INFO* drv_info, HANDLE hnd)
{
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;
	const ADC_DRIVER_MODE * mode;
	ADC_TypeDef* adc;
	const ADC_CFG_MODE* cfg;

	if(hnd->len >= 2)	// read is always 16 bit
	{
		mode = (const ADC_DRIVER_MODE *)hnd->mode.as_voidptr;
		drv_data->pending[mode->adc_num] = hnd;

		// adc init
		adc = &drv_info->hw_base->ADCx[mode->adc_num];
		cfg = &drv_info->adc_cfg[mode->adc_cfg_indx];
		adc->ADC_SR = 0;
		adc->ADC_SMPR1 = cfg->adc_smpr1;
		adc->ADC_SMPR2 = cfg->adc_smpr2;
		adc->ADC_SQR1 = mode->adc_sqr1;
		adc->ADC_SQR2 = mode->adc_sqr2;
		adc->ADC_SQR3 = mode->adc_sqr3;
		adc->ADC_CR1 = cfg->adc_cr1;
		adc->ADC_CR2 = cfg->adc_cr2;
#if USE_ADC_DMA_DRIVER
		if(mode->adc_num < USE_ADC_DMA_DRIVER)
		{
			if(drv_info->dma_mode[mode->adc_num].dma_index < INALID_DRV_INDX)
			{
				adc->ADC_CR2 |= ADC_CR2_DMA | ADC_CR2_DDS;
				drv_data->dma_hnd[mode->adc_num].drv_read_write(
						hnd->dst.as_voidptr, (void*)&adc->ADC_DR, hnd->len/2);
			}

		}
#endif

		// enable ADC
		adc->ADC_CR2 |= ADC_CR2_ADON;

		// software start conversion
		if(!(cfg->adc_cr2 & ADC_CR2_EXTEN))
			adc->ADC_CR2 |= ADC_CR2_SWSTART;
	} else
	{
		return false;
	}
	return true;
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void ADC_DCR(const ADC_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	const ADC_DRIVER_MODE * mode;
	ADC_DRIVER_DATA* drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
        	// We have 3 peripherals on single IRQ/driver, so we use constants,
        	// instead of drv_info->info.peripheral_indx
			RCCPeripheralReset(ID_PERIPH_ADC1);
			RCCPeripheralReset(ID_PERIPH_ADC2);
			RCCPeripheralReset(ID_PERIPH_ADC3);

			RCCPeripheralDisable(ID_PERIPH_ADC1);
			RCCPeripheralDisable(ID_PERIPH_ADC2);
			RCCPeripheralDisable(ID_PERIPH_ADC3);

#if USE_ADC_DMA_DRIVER
        	drv_data->dma_hnd[0].client.drv_index = drv_info->info.drv_index;
#endif
#if USE_ADC_DMA_DRIVER >1
        	drv_data->dma_hnd[1].client.drv_index = drv_info->info.drv_index;
#endif
#if USE_ADC_DMA_DRIVER >2
        	drv_data->dma_hnd[2].client.drv_index = drv_info->info.drv_index;
#endif
			break;

        case DCR_OPEN:

        	mode = (ADC_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode && mode->adc_cfg_indx < drv_info->adc_cfg_cnt)
        	{
        		// first client for the adc or mode is the same?
        		if(!drv_data->all_cnt || drv_info->adc_cfg[mode->adc_cfg_indx].adc_ccr ==
        				drv_info->hw_base->ADC_CCR)
        		{
        			if(!drv_data->all_cnt)
        			{
            			NVIC->NVIC_SetPriority(drv_info->info.drv_index, drv_info->info.isr_priority);
            			NVIC->NVIC_EnableIRQ(drv_info->info.drv_index);
        			}

        			if(!drv_data->adc_cnt[mode->adc_num]++)
        			{
            			RCCPeripheralEnable(ID_PERIPH_ADC1 + mode->adc_num);

            			// common init (needs at least one ADC enabled, otherwise CCR is not working...
            			drv_info->hw_base->ADC_CCR = drv_info->adc_cfg[mode->adc_cfg_indx].adc_ccr;
#if USE_ADC_DMA_DRIVER
        				// enable DMA controller?
						if(mode->adc_num < USE_ADC_DMA_DRIVER)
						{
		        			if(!drv_data->dma_hnd[mode->adc_num].drv_open(
		        					drv_info->dma_mode[mode->adc_num].dma_index,
		        					&drv_info->dma_mode[mode->adc_num]))
		        				break;

						}
#endif
        			}

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
			signed int adc_num;

			adc_num = hnd - drv_data->dma_hnd;
			if(adc_num >= 0 && adc_num < USE_ADC_DMA_DRIVER)
			{
				hnd = drv_data->pending[adc_num];
				if(hnd)
				{
					//pending is done
					drv_data->pending[adc_num] = NULL;

					//resume waiting
					do
					{
						svc_HND_SET_STATUS(hnd, RES_SIG_OK);

						hnd = drv_data->waiting[adc_num];
						if(!hnd)
							break;
						drv_data->waiting[adc_num] = hnd->next;

					} while(!ADC_START_HND(drv_info, hnd));
				}
			}
			break;
#endif

		case DCR_CLOSE:
        	mode = (const ADC_DRIVER_MODE *)hnd->mode.as_voidptr;
        	if(mode && mode->adc_cfg_indx < drv_info->adc_cfg_cnt)
        	{
    			if(!--drv_data->adc_cnt[mode->adc_num])
    			{
        			RCCPeripheralDisable(ID_PERIPH_ADC1+ mode->adc_num);
#if USE_ADC_DMA_DRIVER
					// enable DMA controller?
					if(mode->adc_num < USE_ADC_DMA_DRIVER)
					{
						drv_data->dma_hnd[mode->adc_num].close();
					}
#endif
    			}
        		if(!drv_data->all_cnt)
        		{
        			NVIC->NVIC_DisableIRQ(drv_info->info.drv_index);
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
	if(mode && (hnd->cmd & FLAG_READ) && (mode->adc_cfg_indx < drv_info->adc_cfg_cnt))
	{
		if(	drv_data->pending[mode->adc_num])
		{
			//adc is busy
			hnd->list_add(drv_data->waiting[mode->adc_num]);
			return;
		}

		// start this adc
		hnd->res  = RES_BUSY;
		if(!ADC_START_HND(drv_info, hnd))
			svc_HND_SET_STATUS(hnd, RES_SIG_OK); //not enough length - done!


	} else
	{
		svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
	}

}


//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------

static void adc_channel_isr(const ADC_DRIVER_INFO* drv_info, uint32_t adc_num, uint32_t status)
{
	HANDLE hnd;
	RES_CODE res;
	ADC_TypeDef* adc;


	adc = &drv_info->hw_base->ADCx[adc_num];
	hnd = drv_info->drv_data->pending[adc_num];
	if(hnd)
	{
		res = 0;

		//process EOC
		if(status & ADC_SR_EOC)
		{
			*hnd->dst.as_shortptr++ = adc->ADC_DR;
			hnd->len -= 2;
			if(hnd->len < 2)
			{
				res = RES_SIG_OK;
			}
		}

		//process JEOC
		if(status & ADC_SR_JEOC)
		{
			uint32_t cnt, jindx=0;

			cnt = ADC_JSQR_JL_Get(adc->ADC_JSQR);
			while(jindx <= cnt)
			{
				*hnd->dst.as_shortptr++ = adc->ADC_JDRx[jindx++];
				hnd->len -= 2;
				if(hnd->len < 2)
				{
					res = RES_SIG_OK;
					break;
				}
			}
		}
		//process watchdog
		if(status & ADC_SR_AWD)
		{
			res |= FLG_SIGNALED | FLG_DATA;
		}

		//process overrun
		if(status & ADC_SR_OVR)
		{
			res |= RES_SIG_ERROR;
		}

		if(res)
		{
			//stop handle
			adc->ADC_CR2 &= ~(ADC_CR2_ADON | ADC_CR2_SWSTART | ADC_CR2_CONT);
			drv_info->drv_data->pending[adc_num] = NULL;

			while( 1 )
			{
				usr_HND_SET_STATUS(hnd, res);

				hnd = drv_info->drv_data->waiting[adc_num];
				if(!hnd)
					break;
				drv_info->drv_data->waiting[adc_num] = hnd->next;

				if(ADC_START_HND(drv_info, hnd))
					break;
				res = RES_SIG_OK;
			}

		}
	}

	// clear status flags
	adc->ADC_SR &= ~status;
}

#define ASC_SR_FLAGS (ADC_SR_OVR | ADC_SR_JEOC | ADC_SR_EOC | ADC_SR_AWD)

void ADC_ISR(const ADC_DRIVER_INFO* drv_info)
{
	uint32_t status;

	status = drv_info->hw_base->ADC_CSR;

//	TRACELN("ADC-%x", status);
	for(uint32_t adc = 0; adc<3; adc++)
	{
		if(status & ASC_SR_FLAGS)
			adc_channel_isr(drv_info, adc, status & ASC_SR_FLAGS);

		status >>= 8;
	}
}
