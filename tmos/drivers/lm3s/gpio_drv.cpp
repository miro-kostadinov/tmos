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

/* TODO: This driver needs to be rewritten from the scratch!
 * I'll do some dirty patches now just to compile it...
 *
 */
extern 	 char* const DRV_TABLE[INALID_DRV_INDX+1];

const DRIVER_INDEX GPIO_IRQ_INDX[] = {
	GPIOPortA_IRQn,
	GPIOPortB_IRQn,
	GPIOPortC_IRQn,
	GPIOPortD_IRQn,
	GPIOPortE_IRQn,
	GPIOPortF_IRQn,
	GPIOPortG_IRQn,
	GPIOPortH_IRQn,
	GPIOPortJ_IRQn
};

GPIO_Type * PIO_Base(PIN_DESC Pin)
{
	if(Pin.port_num == PORT_V)
		return (GPIO_Type *)NULL;
#ifndef USE_AHB_PERIPHERAL
    if(SYSCTL->GPIOHSCTL & (1 << Pin.port_num))
        return (GPIO_Type *)g_pulGPIOBaseAddrs[(Pin.port_num << 1) + 1];
    else
        return (GPIO_Type *)g_pulGPIOBaseAddrs[Pin.port_num << 1];
#else
    return (GPIO_Type *)(0x40050000 + ((Pin.port_num + 8)<<12));
#endif
}

void PIO_Cfg(PIN_DESC Pin)
{
	unsigned int shift_num=0;
	GPIO_Type *Port_Base = PIO_Base(Pin);
	if(Port_Base)
	{
		SYSCTL->RCGC2 |= (1<<Pin.port_num); // peripheral clock enable
		SYSCTL->SysCtlPeripheralDeepSleepEnable(SYSCTL_PERIPH_GPIOA + Pin.port_num);
		SYSCTL->SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA + Pin.port_num);
		if(Pin.state & PIN_UNLOCK)
		{
			Port_Base->LOCK = GPIO_LOCK_KEY_DD;
			Port_Base->CR = Pin.pin_pattern;
			Port_Base->AFSEL = 0;
			Port_Base->CR = 0;
			Port_Base->LOCK = 0;
		}
		Port_Base->GPIOPadConfigSet(Pin.pin_pattern,Pin.pad_strength,Pin.pad_type);
		Port_Base->GPIODirModeSet(Pin.pin_pattern,Pin.pin_dir);
		Port_Base->GPIOIntTypeSet(Pin.pin_pattern,Pin.pin_int);
		if(Pin.pin_mux)
		{
			while(!(Pin.pin_pattern&1))
			{
				shift_num +=4;
				Pin.pin_pattern>>=1;
			}
			Port_Base->PCTL = ( (Port_Base->PCTL &~(0xf << shift_num)) | (Pin.pin_mux << shift_num) );
		}
	}
}


void PIO_Cfg_List(PIN_DESC * list )
{
	while(list->as_int)
	{
		PIO_Cfg(*list);
		list++;
	}
}

/** this is the SPI mode to reach extended pins
 * should be declared somewhere (usually in the application)
 */
extern const SPI_DRIVER_MODE sreg_mode_stru;

/** bad implementation! */
extern GPIO_DRIVER_DATA gpio_data;

void PIO_Write(PIN_DESC Pin, unsigned char val )
{
	GPIO_Type *Port_Base = PIO_Base(Pin);
	if(Port_Base)
	{
	    ASSERT(SYSCTL->RCGC2&(1<<Pin.port_num));
		Port_Base->DATA_Bits[Pin.pin_pattern] = val;
	}
	else
	{
		CHandle sreg;
		sreg.tsk_safe_open(SSI0_IRQn, &sreg_mode_stru);
		gpio_data.sreg_val = (gpio_data.sreg_val & (~Pin.pin_pattern))|(val & Pin.pin_pattern);
		sreg.tsk_write(&gpio_data.sreg_val, 1 );
	}
}

unsigned char PIO_Read(PIN_DESC Pin)
{
	GPIO_Type *Port_Base = PIO_Base(Pin);
	if(Port_Base)
	{
	    ASSERT(SYSCTL->RCGC2&(1<<Pin.port_num));
		return Port_Base->DATA_Bits[Pin.pin_pattern];
	}
	else
		return (Pin.pin_pattern & gpio_data.sreg_val);
}

void PIO_CfgOutput1(PIN_DESC Pin)
{
	Pin.pin_dir = DIR_OUT;
	PIO_Cfg(Pin);
	PIO_Write(Pin,Pin.pin_pattern);
}

void PIO_CfgOutput0(PIN_DESC Pin)
{
	Pin.pin_dir = DIR_OUT;
	PIO_Cfg(Pin);
	PIO_Write(Pin,0);
}

void PIO_CfgInput(PIN_DESC Pin)
{
	Pin.pin_dir = DIR_IN;
	PIO_Cfg(Pin);
}


void PIO_SetOutput(PIN_DESC Pin)
{
	PIO_Write(Pin,Pin.pin_pattern);
}

void PIO_ClrOutput(PIN_DESC Pin)
{
	PIO_Write(Pin,0);
}

void PIO_Free(PIN_DESC Pin)
{
	unsigned int port_num = Pin.port_num;

	if(port_num != PORT_V)
	{
		GPIO_Type* port = PIO_Base(Pin);

		unsigned char pattern = Pin.pin_pattern;
		port->GPIOPinIntDisable(pattern);
		port->AFSEL &=~pattern;
		port->DEN &=~pattern;
		port->PDR &=~pattern;
		port->PUR &=~pattern;
		port->PCTL &=~pattern;

		if(!port->IM)
		{
//			#ifdef USE_AHB_PERIPHERAL
//				SYSCTL->GPIOHBCTL &= ~(1 << Pin.port_num);
//			#endif

			NVIC->NVIC_DisableIRQ(GPIO_IRQ_INDX[port_num]);
//			port_num += SYSCTL_PERIPH_GPIOA;	//convert to SYSCTL_PERIPH_GPIOx
//			SysCtlPeripheralReset(port_num);
//			SysCtlPeripheralDisable(port_num);
		}
	}
}

void PIO_Free_List(PIN_DESC * list )
{
	while(list->as_int)
	{
		PIO_Free(*list);
		list++;
	}
}

static void read_handle(GPIO_DRIVER_DATA * drv_data, HANDLE hnd)
{
	PIN_DESC * pin = (PIN_DESC *) hnd->mode.as_voidptr;
	for(int i=0; i < hnd->len && pin->as_int; i++, pin++)
	{

		*(hnd->dst.as_byteptr +i)= PIO_Base(*pin)->DATA_Bits[pin->pin_pattern];
	}
}


void dcr_GPIO_driver(GPIO_DRIVER_INFO * drv_info, unsigned int reason, HANDLE hnd)
{
	GPIO_DRIVER_DATA * drv_data = drv_info->drv_data ;
	switch( reason )
	{
		case DCR_ISR:
		{
			PIN_DESC * pin;
			unsigned int interrupt_pattern = (unsigned int)hnd;
			for(hnd = drv_data->waiting; hnd; hnd = hnd->next)
			{
				for(pin = (PIN_DESC *)hnd->mode.as_voidptr; pin->as_int; pin++)
				{
					if( (pin->port_num != drv_info->port_num ) || !(pin->pin_pattern & interrupt_pattern) )
						continue;
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
		break;

		case DCR_OPEN:
		{
			PIN_DESC * pin = (PIN_DESC *)hnd->mode.as_voidptr;
			hnd->list_add(drv_data->waiting);
			hnd->mode0=0;
			while(pin->as_int)
			{
				PIO_Cfg(*pin);
				if(pin->state & PIN_INT_ENABLE)
				{
					const GPIO_DRIVER_INFO* info;
					int adr = (int)DRV_TABLE[GPIO_IRQ_INDX[pin->port_num]];

					adr &= ~3;
					info = (const GPIO_DRIVER_INFO*)adr;
					if(info->info.isr == (DRV_ISR)isr_GPIO_driver)
					{
						NVIC->NVIC_SetPriority(info->info.drv_index, info->info.isr_priority);
						NVIC->NVIC_EnableIRQ(info->info.drv_index);
						info->hw_base->GPIOPinIntEnable(pin->pin_pattern);
					}
				}
				pin++;
			}
			hnd->res = RES_OK;
		}
		break;

		case DCR_CLOSE:
		{
			PIN_DESC * pin = (PIN_DESC *)hnd->mode.as_voidptr;
			while(pin->as_int)
			{
				PIO_Free(*pin);
				pin++;
			}
			hnd->list_remove(drv_data->waiting);
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		}
		break;

		case DCR_CANCEL:
			read_handle(drv_data, hnd);
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
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
	GPIO_DRIVER_DATA * drv_data = drv_info->drv_data;
	GPIO_Type * port;
	PIN_DESC * pin;

	if(hnd->cmd & FLAG_WRITE)
    {
		for(pin = (PIN_DESC * )hnd->mode.as_voidptr; pin->as_int && hnd->len; pin++, hnd->len--, hnd->src.as_byteptr++)
		{
			if(!(SYSCTL->RCGC2 & (1<<pin->port_num)))
				TRACELN("oops");
			ASSERT(SYSCTL->RCGC2 & (1<<pin->port_num));
			PIO_Base(*pin)->DATA_Bits[pin->pin_pattern] = *hnd->src.as_byteptr;
		}
		svc_HND_SET_STATUS(hnd, RES_OK);
    }

	if(hnd->cmd & FLAG_READ)
	{
		if(hnd->cmd & FLAG_LOCK)
		{
			if(!(hnd->mode0 == PIOHND_INTPENDING))
			{
				int i=0;
				for( pin = (PIN_DESC * )hnd->mode.as_voidptr; pin->as_int && i<hnd->len; pin++, i++)
				{
					port = PIO_Base(*pin);
					port->GPIOIntTypeSet(pin->pin_pattern,GPIO_BOTH_EDGES);
					port->GPIOPinIntEnable(pin->pin_pattern);
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
	unsigned int status = drv_info->hw_base->GPIOPinIntStatus(true);
	drv_info->hw_base->GPIOPinIntClear(status);

	usr_drv_icontrol(drv_info->info.drv_index, DCR_ISR, (void *)status);
}
