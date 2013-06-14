/**************************************************************************//**
 * @file     adc_drv.cpp
 * @ingroup  DRV_SAM3S_ADC
 * @brief    ADC driver implementation
 * @version  V3.00
 * @date     12. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <adc_drv.h>
#include <pmc_drv.h>

/**
 * Turns off the ADC peripheral
 * @param drv_info
 */
static void ADC_off(ADC_INFO drv_info)
{
   	drv_isr_disable(&drv_info->info);
	drv_info->hw_base->ADC_CHDR = 0xFFFF;
    drv_pmc_disable(&drv_info->info);
}

/**
 * Turns off the ADC peripheral
 * @param drv_info
 */
static void ADC_on(ADC_INFO drv_info)
{
    Adc* pADC;

    drv_pmc_enable(&drv_info->info);

    pADC = drv_info->hw_base;
	pADC->ADC_CR = ADC_CR_SWRST;

    /* First Step: Set up by using ADC Mode register */

	pADC->ADC_MR = drv_info->CFG_ADC_MR;

	pADC->ADC_IER = ADC_IER_DRDY;
   	drv_isr_enable(&drv_info->info);
}

/**
 * ADC driver DCR routine
 * @param drv_info
 * @param reason
 * @param param
 */
void ADC_DCR(ADC_INFO drv_info, unsigned int reason, HANDLE param)
{
  	ADC_DRIVER_DATA drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
			ADC_off(drv_info);
			break;

    	case DCR_OPEN:
			if(!drv_data->cnt++)
				ADC_on(drv_info);
			param->res = RES_OK;
			break;

	    case DCR_CLOSE:
			if(!--drv_data->cnt)
				ADC_off(drv_info);
		  	break;

	    case DCR_CANCEL:
            if (drv_data->pending == param)
            {
                drv_data->pending = NULL;
				drv_info->hw_base->ADC_CHDR = 0xFFFF;
		    	svc_HND_SET_STATUS(param, RES_SIG_IDLE);

            } else
            	param->svc_list_cancel(drv_data->waiting);

		  	break;


    }
}

/**
 * ADC Driver routine
 * @param drv_info
 * @param hnd
 */
void ADC_DSR(ADC_INFO drv_info, HANDLE hnd)
{
  	ADC_DRIVER_DATA drv_data = drv_info->drv_data;

	if(hnd->len)
    {
		if(drv_data->pending)
		{
			hnd->list_add(drv_data->waiting);
		} else
		{
		    drv_data->pending = hnd;
		    drv_info->hw_base->ADC_CHER = hnd->mode.as_int;
		    drv_info->hw_base->ADC_CR = ADC_CR_START;
		}
    }else
    	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

/**
 * ADC Driver Interrupt
 * @param drv_info
 */
void ADC_ISR(ADC_INFO drv_info )
{
    unsigned int res;
  	ADC_DRIVER_DATA drv_data = drv_info->drv_data;
    Adc* pADC = drv_info->hw_base;

    if(pADC->ADC_ISR & ADC_ISR_DRDY)
    {
        HANDLE hnd;

        res = pADC->ADC_LCDR;
		if((hnd = drv_data->pending))
		{
            hnd->dst.as_int += res;
            if(!--hnd->len)
            {
                drv_data->pending = NULL;
				pADC->ADC_CHDR = 0xFFFF;
                usr_HND_SET_STATUS(hnd, RES_SIG_OK);
				if(drv_data->pending)
                    return;
				if((hnd = drv_data->waiting))
				{
					drv_data->waiting = hnd->next;
					drv_data->pending = hnd;
					pADC->ADC_CHER = hnd->mode.as_int;

				}
            }
		}
		if(hnd)
			pADC->ADC_CR = ADC_CR_START;
        else
			pADC->ADC_CHDR = 0xFFFF;
    }
}
