/*
 * exti_drv.cpp
 *
 *  Created on: Nov 1, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <cmsis_cpp.h>
#include <exti_drv.h>

extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];

static void read_handle(HANDLE hnd)
{
	PIN_DESC* pin = (PIN_DESC*) hnd->mode.as_voidptr;

	while( *pin && hnd->len > 1)
	{
		// read on virtual ports is not supported through handles
		if (PD_PORT_Get(*pin) <= PD_LAST_PORT)
			*hnd->dst.as_shortptr = PIO_Read(*pin);
		else
			*hnd->dst.as_shortptr = 0;
		pin ++;
		hnd->dst.as_shortptr++;
		hnd->len -=2;
	}
}


void EXTI_DCR(EXTI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE hnd)
{
	EXTI_DRIVER_DATA * drv_data = drv_info->drv_data ;
	const PIN_DESC* pin;
	unsigned int lines;

	switch( reason )
	{
		case DCR_ISR:
		{
			lines = (unsigned int)hnd; //lines with edge

			// loop waiting handles
			for(hnd = drv_data->waiting; hnd; hnd = hnd->next)
			{
				// loop pin description list in the handle
				for(pin = (PIN_DESC*)hnd->mode.as_voidptr; *pin; pin++)
				{
					if(lines & *pin)
					{
						if(hnd->mode0 == PIOHND_WAITING)
						{
							hnd->mode0 = PIOHND_IDLE;
							read_handle(hnd);
							svc_HND_SET_STATUS(hnd, RES_SIG_OK);
						}
						else
							hnd->mode0 = PIOHND_INTPENDING;

						break; //done with this handle
					}
				}
			}
		}
		break;

		case DCR_OPEN:
		{
			//Enable AFIO/SYSCFG...
			RCCPeripheralEnable(drv_info->info.peripheral_indx);

			pin = (PIN_DESC *)hnd->mode.as_voidptr;
			hnd->list_add(drv_data->waiting);
			hnd->mode0 = PIOHND_IDLE;
			while(*pin)
			{
				unsigned int pin_pattern, pos;

				PIO_Cfg(*pin);
				if(PD_MODE_Get(*pin) == PD_MODE_INPUT)
				{
					if(PD_INT_Get(*pin))
					{
						pin_pattern = PD_PINS_Get(*pin);
						while(pin_pattern)
						{
							pos = 31 - __CLZ(pin_pattern);
							lines = 1 << pos;
							pin_pattern ^= lines;

							if(pos < 5)
								pos = EXTI0_IRQn + pos;
							else
							{
								if(pos <10)
									pos = EXTI9_5_IRQn;
								else
									pos = EXTI15_10_IRQn;
							}

							// Handles can be open with any EXTI driver, so check
							// here if the actual driver is installed

							int adr = (int)DRV_TABLE[pos];
							adr &= ~3;

							const EXTI_DRIVER_INFO* info;
							info = (const EXTI_DRIVER_INFO*)adr;
							if(info->info.isr == (DRV_ISR)EXTI_ISR)
							{
								NVIC->NVIC_SetPriority(info->info.drv_index, info->info.isr_priority);
								NVIC->NVIC_EnableIRQ(info->info.drv_index);


							    /* Clear Rising Falling edge configuration */
								info->hw_base->EXTI_RTSR &= ~lines;
								info->hw_base->EXTI_FTSR &= ~lines;

								exti_set_line_source(31 - __CLZ(lines), PD_PORT_Get(*pin));

							    /* Select the trigger for the selected external interrupts */
								if(*pin & PD_INT_FE) // falling edge
									info->hw_base->EXTI_FTSR |= lines;
								if(*pin & PD_INT_RE) // rising edge
									info->hw_base->EXTI_RTSR |= lines;

							    /* Enable line interrupt */
								info->hw_base->EXTI_IMR |= lines;
							}
						}
					}
				}
				pin++;
			}
			hnd->res = RES_OK;
		}
		break;

		case DCR_CLOSE:
		{
			PIO_Cfg_List((PIN_DESC*)hnd->mode.as_voidptr);
			hnd->list_remove(drv_data->waiting);
		}
		break;

		case DCR_CANCEL:
			if (hnd->res & FLG_BUSY)
			{
				read_handle(hnd);
				svc_HND_SET_STATUS(hnd, RES_SIG_CANCEL);
			}
			break;

		case DCR_RESET:
			NVIC->NVIC_DisableIRQ(drv_info->info.drv_index);
			RCCPeripheralReset(drv_info->info.peripheral_indx);
			RCCPeripheralDisable(drv_info->info.peripheral_indx); // turn off
			break;
		default:
			break;
	}
}

void EXTI_DSR(EXTI_DRIVER_INFO* drv_info, HANDLE hnd)
{
	PIN_DESC* pin;

	pin = (PIN_DESC*)hnd->mode.as_voidptr;
	if(hnd->cmd & FLAG_WRITE)
    {
		pin = (PIN_DESC*)hnd->mode.as_voidptr;
		while( *pin && hnd->len > 1)
		{
			// write on virtual ports is not supported through handles
			if (PD_PORT_Get(*pin) <= PD_LAST_PORT)
				PIO_Write(*pin, *hnd->src.as_shortptr);
			pin ++;
			hnd->src.as_shortptr++;
			hnd->len -=2;
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		return;
    }

	if(hnd->cmd & FLAG_READ)
	{
		if(hnd->cmd & FLAG_LOCK)
		{
			if(!(hnd->mode0 == PIOHND_INTPENDING))
			{
				hnd->mode0 = PIOHND_WAITING;
				hnd->res = RES_BUSY;
				return;
			}
		}
		hnd->mode0 = PIOHND_IDLE;
		read_handle(hnd);
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}

void EXTI_ISR(EXTI_DRIVER_INFO* drv_info )
{
	unsigned int status;

	status = drv_info->hw_base->EXTI_IMR & drv_info->int_mask;
	status &= drv_info->hw_base->EXTI_PR;

	if(status)
	{
		drv_info->hw_base->EXTI_PR = status;
		usr_drv_icontrol(drv_info->info.drv_index, DCR_ISR, (void *)status);
	}

}



