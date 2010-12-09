/**************************************************************************//**
 * @file     pio_drv.cpp
 * @ingroup  DRV_AT91_PIO
 * @brief    PIO driver implementation
 * @version  V3.00
 * @date     09. December 2010
 * @author	 Miroslav Kostadinov
 *
 ******************************************************************************/

#include <tmos.h>
#include <pio_drv.h>
#include <platform_drv.h>

//*----------------------------------------------------------------------------
//*		Helper functions - can be used without driver.
//*----------------------------------------------------------------------------
void GPIO_CfgOutput(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

	pPio->PIO_PER = mask; 		// Set in PIO mode
	pPio->PIO_OER = mask;		// Configure in Output
    pPio->PIO_PUDR = mask;		// disable pullup
}

void GPIO_CfgOutput0(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

    pPio->PIO_PUDR = mask;		// disable pullup
	pPio->PIO_CODR = mask;
	pPio->PIO_PER = mask; 		// Set in PIO mode
	pPio->PIO_OER = mask;		// Configure in Output
}

void GPIO_CfgOutput1(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

	pPio->PIO_SODR = mask;
	pPio->PIO_PER = mask; 		// Set in PIO mode
	pPio->PIO_OER = mask;		// Configure in Output
    pPio->PIO_PUDR = mask;		// disable pullup
}

void GPIO_CfgInput(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

    pPio->PIO_PUDR = mask;		// disable pullup
	pPio->PIO_ODR = mask;		// Disable Output
	pPio->PIO_CODR = mask;
	pPio->PIO_PER = mask; 		// Set in PIO mode
	pPio->PIO_MDDR = mask; 		// Disable Multidrive
}

void GPIO_CfgInputPull(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

	pPio->PIO_ODR = mask;		// Disable Output
	pPio->PIO_CODR = mask;
	pPio->PIO_PER = mask; 		// Set in PIO mode
	pPio->PIO_MDDR = mask; 		// Disable Multidrive
    pPio->PIO_PUER = mask;		// enable pullup
}

void GPIO_CfgPeriph(GPIO pins)
{
	pins->hw_base->PIO_PDR = pins->mask;	// Set in PIO mode
}

void GPIO_CfgPeriphB(GPIO pins)
{
	Pio* pPio = pins->hw_base;
	unsigned int mask = pins->mask;

    pPio->PIO_ABCDSR[1] = mask;
	pPio->PIO_PDR = mask; 		// Set in PIO mode
}

void GPIO_CfgOD(GPIO pins)
{
    pins->hw_base->PIO_MDER = pins->mask; 		// Multidrive
}

void GPIO_SetOutput(GPIO pins)
{
    pins->hw_base->PIO_SODR = pins->mask;
}

void GPIO_ClearOutput(GPIO pins)
{
    pins->hw_base->PIO_CODR = pins->mask;
}

unsigned int GPIO_GetInput(GPIO pins)
{
    return pins->hw_base->PIO_PDSR & pins->mask;
}


//*----------------------------------------------------------------------------
//*			PIO driver
//*----------------------------------------------------------------------------

#define PIOHND_WAITING		0x01
#define PIOHND_INTPENDING	0x02

static void piohnd_read(HANDLE hnd, Pio* pPio)
{
	hnd->mode1 = 0;
	*hnd->dst.as_intptr = pPio->PIO_PDSR & hnd->mode.as_int;
	svc_HND_SET_STATUS(hnd, RES_SIG_OK);
}



void PIO_DCR(PIO_INFO drv_info, unsigned int reason, HANDLE param)
{
	PIO_DRIVER_DATA drv_data;
	Pio* pPio;

    pPio = drv_info->hw_base;
    drv_data = drv_info->drv_data;

	switch(reason)
    {

	    case DCR_ISR:
            {
                HANDLE hnd;

                hnd = drv_data->waiting;
                while(hnd)
                {
                    if((unsigned int)param & hnd->mode.as_int )
                    {
                        if( hnd->mode1 & PIOHND_WAITING )
                        {
                            piohnd_read(hnd, pPio);
                        } else
                        {
                            hnd->mode1 = PIOHND_INTPENDING;
                        }
                    }
                    hnd = hnd->next;
                }
            }
		  	break;

    	case DCR_OPEN:
            {
                GPIO pins = (GPIO)param->mode.as_voidptr;
            	param->list_add(drv_data->waiting);
            	param->res = RES_OK;
                reason = param->mode0;

                if(reason & PIOMODE_PER)
                {
                    if(reason & PIOMODE_BSR)
                        pPio->PIO_ABCDSR[1] = pins->mask;
                    pPio->PIO_PER = pins->mask;
                }

                if(reason & PIOMODE_SODR)
                    pPio->PIO_SODR = pins->mask;

                if(reason & PIOMODE_OER)
                    pPio->PIO_OER = pins->mask;

                if(reason & PIOMODE_IFER)
                    pPio->PIO_IFER = pins->mask;


                if(reason & PIOMODE_IER)
                    pPio->PIO_IER = pins->mask;

                if(reason & PIOMODE_MDER)
                    pPio->PIO_MDER = pins->mask;
                else
                    pPio->PIO_MDDR = pins->mask;

                if(reason & PIOMODE_PPUER)
                    pPio->PIO_PUER = pins->mask;
                else
                    pPio->PIO_PUDR = pins->mask;


                drv_pmc_enable(&drv_info->info);
               	drv_isr_enable(&drv_info->info);

            }
		  	break;

	    case DCR_CLOSE:
            {
            	param->list_remove(drv_data->waiting);
                if((!drv_data->waiting) && (!drv_data->cnt))
                {
                   	drv_isr_disable(&drv_info->info);
                    drv_pmc_disable(&drv_info->info);
                };
            }
		  	break;

	    case DCR_CANCEL:
			((HANDLE)param)->mode1 = 0;
			*((HANDLE)param)->dst.as_intptr = pPio->PIO_PDSR & ((HANDLE)param)->mode.as_int;
//            piohnd_read((HANDLE)param, pPio);
		  	break;


	    case DCR_PARAMS:
            drv_data->cnt += (unsigned int)param;

			if((!drv_data->waiting) && (!drv_data->cnt))
			{
                drv_pmc_disable(&drv_info->info);
			} else
			{
                drv_pmc_enable(&drv_info->info);
			}
			break;

	    case DCR_RESET:
               pPio->PIO_PUDR = -1u;
		  	break;
	}
}

void PIO_DSR(PIO_INFO drv_info, HANDLE hnd)
{
	Pio* pPio = drv_info->hw_base;

    if(hnd->len == 4)
    {
        if(hnd->cmd & FLAG_WRITE)
        {
            pPio->PIO_OWER = hnd->mode.as_int;
            pPio->PIO_OWDR = ~hnd->mode.as_int;
            pPio->PIO_ODSR = * hnd->src.as_intptr;
    		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
    		return;
        }

		if(hnd->cmd & FLAG_READ)
		{
			if(hnd->cmd & FLAG_LOCK)
				if(!(hnd->mode1 & PIOHND_INTPENDING))
				{
					hnd->mode1 = PIOHND_WAITING;
					pPio->PIO_IER = hnd->mode.as_int;
					hnd->res = RES_BUSY;
					return;
				}
			piohnd_read(hnd, pPio);
			return;
		}


    }

    svc_HND_SET_STATUS(hnd, RES_SIG_ERROR);
}

void PIO_ISR(PIO_INFO drv_info )
{
  	unsigned int pin_ints;
	Pio* pPio = drv_info->hw_base;

    pin_ints = pPio->PIO_ISR;
    pin_ints &= pPio->PIO_IMR;

    pPio->PIO_IDR = pin_ints;
    isr_contol(&drv_info->info, DCR_ISR, (void*)pin_ints);
}

