/*
 * tim_drv.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: miro
 */

#include <tmos.h>
#include <tim_drv.h>

static void ConfigureTimer(TIM_DRV_INF drv_info, const TIMER_CONTROL_MODE *mode)
{
	TIM_TypeDef* hw_base = drv_info->hw_tmr_base;

	hw_base->TIM_CR1 = mode->cr1;
	hw_base->TIM_CR2 = mode->cr2;
	hw_base->TIM_ARR = mode->arr_or_ccr;
	hw_base->TIM_PSC = mode->psc;
	hw_base->TIM_RCR = mode->rcr;
	hw_base->TIM_SMCR = mode->smcr;
	hw_base->TIM_EGR = TIM_EGR_UG;
}

static void ConfigureChannel(TIM_DRV_INF drv_info, const TIMER_CHANNEL_MODE *mode)
{
	TIM_TypeDef* hw_base = drv_info->hw_ch_base;
	unsigned int indx;

	indx = mode->ch_indx;
	switch(indx)
	{
	case 1:
		hw_base->TIM_CCMR1 = (hw_base->TIM_CCMR1 & ~0xFF) | mode->ccmr;
		break;
	case 2:
		hw_base->TIM_CCMR1 = (hw_base->TIM_CCMR1 & ~0xFF00) | (mode->ccmr << 8);
		break;
	case 3:
		hw_base->TIM_CCMR2 = (hw_base->TIM_CCMR2 & ~0xFF) | mode->ccmr;
		break;
	case 4:
		hw_base->TIM_CCMR2 = (hw_base->TIM_CCMR2 & ~0xFF00) | (mode->ccmr << 8);
		break;
	}

	indx --;
	hw_base->TIM_CCRx[indx] = mode->arr_or_ccr;
	indx <<= 2;
	hw_base->TIM_CCER = (hw_base->TIM_CCER & ~(0xF << indx)) | (mode->ccer << indx);

	hw_base->TIM_DIER |= mode->dier;

	PIO_Cfg(mode->ch_pin);
}

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
//
// read/write size=32 bit
void TIM_DCR(TIM_DRV_INF drv_info, unsigned int reason, HANDLE hnd)
{
	TIMER_DRIVER_DATA *drv_data;

	switch(reason)
	{
		case DCR_RESET:
			RCCPeripheralReset(drv_info->info.peripheral_indx);
			RCCPeripheralDisable(drv_info->info.peripheral_indx); // ??? turn off
			DebugFreezeTimer(drv_info->info.peripheral_indx);
			break;

		case DCR_OPEN:
		{
			TIMER_DRIVER_MODE *timer_mode = (TIMER_DRIVER_MODE *)(hnd->mode.as_voidptr);
			if(timer_mode)
			{
				drv_data = &drv_info->drv_data_tbl[timer_mode->tim_indx];

				if(drv_data->cnt)
				{
					if(timer_mode->ch_indx == 0 && timer_mode->ch_indx > 4)
					{
						break; // control already open
					}
					ConfigureChannel(drv_info, (TIMER_CHANNEL_MODE*)timer_mode);
				}
				else
				{
					if(timer_mode->ch_indx != 0 )
					{
						break; // control must be open first
					}
					// Enable AND Reset the Timer peripheral
					RCCPeripheralEnable(drv_info->info.peripheral_indx);
					RCCPeripheralReset(drv_info->info.peripheral_indx);
					ConfigureTimer(drv_info, (TIMER_CONTROL_MODE*)timer_mode);
				}
				drv_enable_isr(&drv_info->info);
				drv_data->cnt++;
				hnd->mode0 = timer_mode->ch_indx;
				hnd->mode1 = timer_mode->tim_indx;
				hnd->res = RES_OK;
			}
			break;
		}

		case DCR_CLOSE:
		{
			drv_data = &drv_info->drv_data_tbl[hnd->mode1];
			if(drv_data->cnt)
				drv_data->cnt--;
			if(!drv_data->cnt)
			{
				NVIC_DisableIRQ(drv_info->info.drv_index);
				RCCPeripheralReset(drv_info->info.peripheral_indx);
				RCCPeripheralDisable(drv_info->info.peripheral_indx);
			}
			break;
		}

		case DCR_CANCEL:
		{
			drv_data = &drv_info->drv_data_tbl[hnd->mode1];
			if(hnd == drv_data->pending[hnd->mode0])
			{
				svc_HND_SET_STATUS(hnd, RES_SIG_IDLE);
				drv_data->pending[hnd->mode0] = NULL;
			}
			break;
		}

	}
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void TIM_DSR(TIM_DRV_INF drv_info, HANDLE hnd)
{
	if ( (hnd->cmd & FLAG_WRITE) && (hnd->len >3) )
	{

		if(hnd->mode0)
		{
			//write CCRx
			drv_info->hw_ch_base->TIM_CCRx[hnd->mode0-1] = *hnd->src.as_intptr++;
		} else
		{
			drv_info->hw_tmr_base->TIM_ARR = *hnd->src.as_intptr++;
		}

		hnd->len -=4;
	}

	if (hnd->cmd & FLAG_COMMAND)
	{
		//enable timer
		if(hnd->mode0)
			drv_info->hw_ch_base->TIM_CR1 |= TIM_CR1_CEN;
		else
			drv_info->hw_tmr_base->TIM_CR1 |= TIM_CR1_CEN;
	}

	if (hnd->cmd & FLAG_LOCK)
	{
		uint32_t mask;

		//wait for interrupt
		drv_info->drv_data_tbl[hnd->mode1].pending[hnd->mode0] = hnd;
		hnd->res = RES_BUSY;
		mask = ((TIMER_DRIVER_MODE*)hnd->mode.as_voidptr)->dier;
		if(hnd->mode0)
		{

			drv_info->hw_ch_base->TIM_SR = ~mask;
			drv_info->hw_ch_base->TIM_DIER |= mask;
		}
		else
		{
			drv_info->hw_tmr_base->TIM_SR = ~mask;
			drv_info->hw_tmr_base->TIM_DIER |= mask;
		}
	}else
	{
		if ( (hnd->cmd & FLAG_READ) && (hnd->len >3) )
		{
			if(hnd->mode0)
			{
				//Read CCRx
				*hnd->dst.as_intptr++ = drv_info->hw_ch_base->TIM_CCRx[hnd->mode0-1];
			} else
			{
				*hnd->dst.as_intptr++ = drv_info->hw_tmr_base->TIM_CNT;
			}

			hnd->len -=4;
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void TIM_ISR(TIM_DRV_INF drv_info)
{
	TIM_TypeDef* timer;
	TIMER_DRIVER_DATA* drv_data;
	HANDLE hnd;
	unsigned int status;
	unsigned int mask;


	//Process Channel ISRs
	timer = drv_info->hw_ch_base;
	drv_data = &drv_info->drv_data_tbl[drv_info->drv_ch_idx];
	status = timer->TIM_SR;
	for(int channel=1; status && (channel<=4); ++channel)
	{
		mask = ((TIM_SR_CC1IF | TIM_SR_CC1OF)>>1) << channel;
		mask &= status;
		if(mask)
		{
			hnd = drv_data->pending[channel];
			if(hnd)
			{
				//Callback ?
				if ( (hnd->client.drv_index & 3) && (hnd->client.drv_index >= INALID_DRV_INDX) )
				{
					RES_CODE res;

					res = ((TIMER_DRV_CBF)hnd->client.callback)(hnd, RES_IDLE);
					if( res & FLG_SIGNALED)
					{
						hnd->res = res;
						drv_data->pending[channel] = NULL;
						hnd = NULL;
					}
				}

				if(hnd)
				{
					//write CCRx ?
					if ( (hnd->cmd & FLAG_WRITE) && (hnd->len >3) )
					{
						timer->TIM_CCRx[channel-1] = *hnd->src.as_intptr++;

						hnd->len -=4;
					}

					//Read CCRx ?
					if ( (hnd->cmd & FLAG_READ) && (hnd->len >3) )
					{
						*hnd->dst.as_intptr++ = timer->TIM_CCRx[channel-1];

						hnd->len -=4;
					}

					// signal?
					if((mask>>8) || (hnd->len < 4))
					{
						//signal error=overflow, ok=capture
						usr_HND_SET_STATUS(hnd, (mask>>8)?RES_SIG_ERROR:RES_SIG_OK);
						drv_data->pending[channel] = NULL;
					}
				}


				//clear interrupt
				timer->TIM_SR = ~mask;
			}else
			{
				// disable interrupt
				timer->TIM_DIER &= ~mask;
			}
			status &= ~mask;
		}
	}

	//Process Control/Timer ISRs
	timer = drv_info->hw_tmr_base;
	drv_data = &drv_info->drv_data_tbl[drv_info->drv_tmr_idx];
	status = timer->TIM_SR & drv_info->isr_tmr_mask;
	if( status )
	{
		hnd = drv_data->pending[0];
		if(hnd)
		{
			//Callback ?
			if ( (hnd->client.drv_index & 3) && (hnd->client.drv_index >= INALID_DRV_INDX) )
			{
				RES_CODE res;

				res = ((TIMER_DRV_CBF)hnd->client.callback)(hnd, RES_IDLE);
				if( res & FLG_SIGNALED)
				{
					hnd->res = res;
					drv_data->pending[0] = NULL;
					hnd = NULL;
					timer->TIM_CR1 &= ~TIM_CR1_CEN;
					timer->TIM_CNT = 0;
				}
			}

			if(hnd)
			{
				//write CCRx ?
				if ( (hnd->cmd & FLAG_WRITE) && (hnd->len >3) )
				{
					timer->TIM_ARR = *hnd->src.as_intptr++;
					hnd->len -=4;
				}

				//Read CCRx ?
				if ( (hnd->cmd & FLAG_READ) && (hnd->len >3) )
				{
					*hnd->dst.as_intptr++ = timer->TIM_CNT;
					hnd->len -=4;
				}

				// signal?
				if(hnd->len < 4)
				{
					//signal error=overflow, ok=capture
					usr_HND_SET_STATUS(hnd, RES_SIG_OK);
					drv_data->pending[0] = NULL;
				}
			}




			//clear interrupt
			timer->TIM_SR = ~status;
		}else
		{
			// disable interrupt
			timer->TIM_DIER &= ~status;
		}
	}

}


