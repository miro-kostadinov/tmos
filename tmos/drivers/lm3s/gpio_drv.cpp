/*
 * gpio_drv.c
 *
 *  Created on: 2010-4-22
 *      Author: stanly
 */
#include <tmos.h>
#include <gpio_drv.h>
#include <hardware_cpp.h>
#include <spi_drv.h>

extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];

static void read_handle(GPIO_DRIVER_DATA * drv_data, HANDLE hnd)
{
	PIN_DESC* pin = (PIN_DESC*) hnd->mode.as_voidptr;
	for(int i=0; i < hnd->len && *pin; i++, pin++)
	{
		*(hnd->dst.as_byteptr +i)= PIO_Read(*pin);
	}
}


void dcr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, unsigned int reason, HANDLE hnd)
{
	GPIO_DRIVER_DATA * drv_data = drv_info->drv_data ;
	switch( reason )
	{
		case DCR_ISR:
		{
			unsigned int pattern = ((unsigned int)hnd) << PD_PINS_Pos;
			for(hnd = drv_data->waiting; hnd; hnd = hnd->next)
			{
				PIN_DESC* pin;
				for(pin = (PIN_DESC*)hnd->mode.as_voidptr; *pin; pin++)
				{
					unsigned int port_num;

					port_num = PD_PORT_Get(*pin);
					if((pattern & *pin) && (port_num == drv_info->port_num))
					{
						if(hnd->mode0 == PIOHND_WAITING)
						{
							hnd->mode0 = 0;
							read_handle(drv_data, hnd);
							svc_HND_SET_STATUS(hnd, RES_SIG_OK);
						}
						else
							hnd->mode0=PIOHND_INTPENDING;
					}
				}
			}
		}
		break;

		case DCR_OPEN:
		{
			PIN_DESC * pin = (PIN_DESC *)hnd->mode.as_voidptr;
			hnd->list_add(drv_data->waiting);
			hnd->mode0=0;
			while(*pin)
			{
				PIO_Cfg(*pin);
				if(*pin & PD_INT_ENABLE)
				{
					const GPIO_DRIVER_INFO* info;
					int adr = (int)DRV_TABLE[GPIO_IRQ_INDX[PD_PORT_Get(*pin)]];

					adr &= ~3;
					info = (const GPIO_DRIVER_INFO*)adr;
					if(info->info.isr == (DRV_ISR)isr_GPIO_driver)
					{
						NVIC->NVIC_SetPriority(info->info.drv_index, info->info.isr_priority);
						NVIC->NVIC_EnableIRQ(info->info.drv_index);
						info->hw_base->GPIOIM |= PD_PINS_Get(*pin);
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
//			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		}
		break;

		case DCR_CANCEL:
			if (hnd->res & FLG_BUSY)
			{
				read_handle(drv_data, hnd);
				svc_HND_SET_STATUS(hnd, RES_SIG_CANCEL);
			}
			break;

		case DCR_RESET:
			#ifdef USE_AHB_PERIPHERAL
				SYSCTL->GPIOHBCTL |= (1 << drv_info->port_num);
			#endif
			NVIC->NVIC_DisableIRQ(drv_info->info.drv_index);
			SysCtlPeripheralReset(drv_info->info.peripheral_indx);
			SysCtlPeripheralDisable(drv_info->info.peripheral_indx); // turn off
			break;
		default:
			break;
	}
}

void dsr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, HANDLE hnd)
{
	GPIO_DRIVER_DATA* drv_data = drv_info->drv_data;
	GPIO_Type* port;
	PIN_DESC* pin;
	unsigned int port_num;

	pin = (PIN_DESC*)hnd->mode.as_voidptr;
	if(hnd->cmd & FLAG_WRITE)
    {
		pin = (PIN_DESC*)hnd->mode.as_voidptr;
		for(; *pin && hnd->len; pin++, hnd->len--)
		{

			port_num = PD_PORT_Get(*pin);
			ASSERT(SYSCTL->RCGC2 & (1<<port_num));
			port = PORT_ADDRESS( port_num );
			port->GPIODATA[PD_PINS_Get(*pin)] = *hnd->src.as_byteptr++;
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
				for( ; *pin && hnd->len; pin++, hnd->len--)
				{
					port_num = PD_PORT_Get(*pin);
					port = PORT_ADDRESS( port_num );
					port->GPIOIM |= PD_PINS_Get(*pin);
				}
				hnd->mode0 = PIOHND_WAITING;
				hnd->res = RES_BUSY;
				return;
			}
		}
		hnd->mode0 = 0;
		read_handle(drv_data, hnd);
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}

void isr_GPIO_driver(GPIO_DRIVER_INFO* drv_info )
{
	unsigned int status;

	status = drv_info->hw_base->GPIOMIS;
	drv_info->hw_base->GPIOICR = status;

	usr_drv_icontrol(drv_info->info.drv_index, DCR_ISR, (void *)status);
}
