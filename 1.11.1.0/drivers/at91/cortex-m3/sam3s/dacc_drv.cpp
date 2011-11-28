/**************************************************************************//**
 * @file     dacc_drv.cpp
 * @ingroup  DRV_SAM3S_DACC
 * @brief    DACC driver implementation
 * @version  V3.00
 * @date     12. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <dacc_drv.h>
#include <pmc_drv.h>

/** Turn off the DACC peripheral
 *
 * @param drv_info
 */
static void DACC_off(DACC_INFO drv_info)
{
   	drv_isr_disable(&drv_info->info);
	drv_info->hw_base->DACC_CR = DACC_CR_SWRST;
    drv_pmc_disable(&drv_info->info);
}

/** Turn on the DACC peripheral
 *
 * @param drv_info
 * @param pMode
 */
static void DACC_on(DACC_INFO drv_info, DRV_DACC_MODE pMode)
{
    Dacc* pDacc;

    pDacc = drv_info->hw_base;

    drv_pmc_enable(&drv_info->info);

	pDacc->DACC_CR = DACC_CR_SWRST;
	pDacc->DACC_WPMR = DACC_WPMR_WPKEY(0x444143);


    /* Set DACC Mode register */
	pDacc->DACC_MR = pMode->DACC_MR;

//    //* Enable PDC
//	pDacc->DACC_PTCR = DACC_PTCR_TXTEN;
//
//	pDacc->DACC_IER = DACC_IER_ENDTX;
   	drv_isr_enable(&drv_info->info);
}

/** Start transmit hnd
 *
 * @param pDacc
 * @param hnd
 */
static void START_TX_HND(Dacc*	pDacc, HANDLE hnd)
{
	pDacc->DACC_TPR = hnd->src.as_int;
	pDacc->DACC_TCR = hnd->len;
	pDacc->DACC_PTCR = DACC_PTCR_TXTEN ;
    pDacc->DACC_IER = DACC_IER_TXBUFE;
    hnd->res = RES_BUSY;
}

/** DACC Driver DCR routine
 *
 * @param drv_info
 * @param reason
 * @param param
 */
void DACC_DCR(DACC_INFO drv_info, unsigned int reason, HANDLE param)
{
  	DACC_DRIVER_DATA drv_data = drv_info->drv_data;
  	Dacc* pDacc = drv_info->hw_base;

	switch(reason)
    {
        case DCR_RESET:
			DACC_off(drv_info);
			break;

    	case DCR_OPEN:
    	{
			DRV_DACC_MODE pMode = (DRV_DACC_MODE)(param->mode.as_voidptr);

			if(drv_data->cnt)
			{
				if(pMode->DACC_MR != pDacc->DACC_MR)
					return;
			} else
			{
				DACC_on(drv_info, pMode);
			}
			pDacc->DACC_CHER = pMode->DACC_CHER;
			drv_data->cnt++;
			param->res = RES_OK;
			break;
    	}

	    case DCR_CLOSE:
			if(!--drv_data->cnt)
				DACC_off(drv_info);
		  	break;

	    case DCR_CANCEL:
            if (drv_data->pending == param)
            {
            	pDacc->DACC_PTCR = DACC_PTCR_TXTDIS;
            	drv_data->pending = param->next;
            	if(drv_data->pending)
            		START_TX_HND(pDacc, drv_data->pending);
		    	svc_HND_SET_STATUS(param, RES_SIG_IDLE);

            } else
            	param->svc_list_cancel(drv_data->pending);
		  	break;
    }
}

/** DACC Driver DSR routine
 *
 * @param drv_info
 * @param hnd
 */
void DACC_DSR(DACC_INFO drv_info, HANDLE hnd)
{
  	DACC_DRIVER_DATA drv_data = drv_info->drv_data;

	if(hnd->len)
    {
		if(drv_data->pending)
		{
			hnd->list_add(drv_data->pending);
		} else
		{
		    hnd->next = NULL;
		    drv_data->pending = hnd;
          	START_TX_HND(drv_info->hw_base, hnd);
		}
    }else
    	svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

/** DACC Driver Interrupt
 *
 * @param drv_info
 */
void DACC_ISR(DACC_INFO drv_info )
{
  	DACC_DRIVER_DATA drv_data = drv_info->drv_data;
    Dacc* pDacc = drv_info->hw_base;

    if(pDacc->DACC_ISR & DACC_ISR_TXBUFE)
    {
        HANDLE hnd;

		pDacc->DACC_IDR = DACC_IDR_TXBUFE;
		if((hnd = drv_data->pending))
		{
      		hnd->len = 0;
  			hnd->src.as_int = pDacc->DACC_TPR;
      		drv_data->pending = hnd->next;
	      	if( drv_data->pending )
	          	START_TX_HND(pDacc, drv_data->pending);
			usr_HND_SET_STATUS(hnd, RES_SIG_OK);

		}
    }
}
