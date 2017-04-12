/**************************************************************************//**
 * @ingroup	 hardware_stm32f1_gpio
 * @{
 * @file     hardware/stm32/stm32f1/gpio_f1.cpp
 * @brief    STM32F1 GPIO
 * @version  V3.00
 * @date     31. October 2012
 * @author	 Miroslav Kostadinov
 */

#include <tmos.h>
#include <fam_cpp.h>

/**
 * Configure pin(s) according to the pin description
 * @param cfg
 */
void PIO_Cfg(PIN_DESC cfg)
{
	unsigned int pin_pattern, tmpreg, mode, pos;
	GPIO_TypeDef* gpio_port;

	tmpreg = PD_PORT_Get(cfg);
	if (tmpreg <= PD_LAST_PORT)
	{
		RCC->RCC_APB2ENR |= RCC_APB2ENR_IOPAEN << tmpreg;

		gpio_port = PORT_ADDRESS( tmpreg );
		pin_pattern = PD_PINS_Get(cfg);
		mode = PD_MODE_Get(cfg);

		/* calculate CNFx and MODEx bits*/
		switch(mode)
		{
		case 0: //	INPUT
			/* configure pullup/pulldowns*/
			if(cfg & PD_PULL_DOWN)
				gpio_port->GPIO_BRR = pin_pattern;
			if(cfg & PD_PULL_UP)
				gpio_port->GPIO_BSRR = pin_pattern;


			if(cfg & (PD_PULL_UP | PD_PULL_DOWN) )
				mode = GPIO_CRx_MODE_INPUT | GPIO_CRx_CNF_IN_PUPD;
			else
				mode = GPIO_CRx_MODE_INPUT | GPIO_CRx_CNF_IN_GP;
			break;

		case 1: //	OUTPUT
			/* set default pin level*/
			gpio_port->GPIO_BSRR = pin_pattern << ((cfg & PD_ACTIVE_HIGH)?0:16);

			mode = PD_OSPEED_Get(cfg) | (PD_OTYPE_Get(cfg) << 2);
			break;

		case 2: //	AF
			mode = PD_OSPEED_Get(cfg);
			if(mode)
			{
				// when ospeed is set, AD is in output mode
				mode |= GPIO_CRx_CNF_OUT_AF_PP | (PD_OTYPE_Get(cfg) << 2); // PP or OD
			} else
			{
				if(cfg & (PD_PULL_UP | PD_PULL_DOWN) )
					mode |= GPIO_CRx_MODE_INPUT | GPIO_CRx_CNF_IN_PUPD;
				else
					mode |= GPIO_CRx_MODE_INPUT | GPIO_CRx_CNF_IN_GP;
			}
			break;

		default: //	ANALOG
			mode = GPIO_CRx_MODE_INPUT | GPIO_CRx_CNF_IN_AN;
			break;
		}

		/* Configure the eight high port pins */
		if( pin_pattern > 0x80 )
		{
			do	// loop for exclusive access (thread safe)
			{
				tmpreg = locked_get_reg(&gpio_port->GPIO_CRH);

				do // loop pins
				{
					pos = 1+ __CLZ(pin_pattern);
					pin_pattern = (pin_pattern << pos) >> pos;

					pos = (24-pos) << 2;
					tmpreg &= ~(0xF << pos);
					tmpreg |= mode << pos;
				} while(pin_pattern > 0x80);
			} while (locked_set_reg(&gpio_port->GPIO_CRH, tmpreg));
		}

		/* Configure the eight low port pins */
		if( pin_pattern )
		{
			do	// loop for exclusive access (thread safe)
			{
				tmpreg = locked_get_reg(&gpio_port->GPIO_CRL);

				do // loop pins
				{
					pos = 1+ __CLZ(pin_pattern);
					pin_pattern = (pin_pattern << pos) >> pos;

					pos = (32-pos) << 2;
					tmpreg &= ~(0xF << pos);
					tmpreg |= mode << pos;
				} while(pin_pattern);
			} while (locked_set_reg(&gpio_port->GPIO_CRL, tmpreg));
		}
	}
#if ( USE_GPIO_EXPANDER !=	0)
	else
	{
		//for virtual ports just set the default level
		PIO_Deassert(cfg);
	}
#endif
}


/**
 * Configure a list with pin descriptions
 * @param list
 */
void PIO_Cfg_List(const PIN_DESC* list)
{
	while(*list)
	{
		PIO_Cfg(*list);
		list++;
	}
}

/**
 * Configure a list of pins as inputs
 * @param list
 */
void PIO_CfgInput_List(PIN_DESC * list )
{
	while(*list)
	{
		PIO_CfgInput(*list);
		list++;
	}
}


/**
 * Free resources used for the pins
 * This function should be called when the pins will no longer used. The function
 * will disable the gpio interrupts etc.
 * @note no usage counter is used, so the first call will free the resources!
 * @param cfg
 */
void PIO_Free(PIN_DESC cfg)
{
//	unsigned int port_num, pin_pattern;
//	GPIO_TypeDef* gpio_port;
//
//
//	port_num = PD_PORT_Get(cfg);
//	if(port_num <= PD_LAST_PORT)
//	{
//		gpio_port = PORT_ADDRESS( port_num );
//		pin_pattern = PD_PINS_Get(cfg);
//		gpio_port->GPIOIM &= ~pin_pattern;
//		if(!gpio_port->GPIOIM)
//		{
//			NVIC->NVIC_DisableIRQ(GPIO_IRQ_INDX[port_num]);
//		}
//	}
}


/**
 * Free resources for a list of pin descriptions
 * @param list
 */
void PIO_Free_List(const PIN_DESC* list)
{
	while (*list)
	{
		PIO_Free(*list);
		list++;
	}
}


/**
 * Write value to pin(s)
 * @param pins
 * @param val
 */
void PIO_Write(PIN_DESC pins, unsigned int val)
{
	unsigned int port_num;

	port_num = PD_PORT_Get(pins);
	if (port_num <= PD_LAST_PORT)
	{
		GPIO_TypeDef* gpio_port = PORT_ADDRESS( port_num );
//		ASSERT(SYSCTL->RCGC2&(1<<port_num));
		val &= 0xFFFF;
		gpio_port->GPIO_BSRR = (pins & val) | ((pins & ~val)<<16);
	}
#if ( USE_GPIO_EXPANDER !=	0)
	else
	{
		VPIO_Write(pins, val);
	}
#endif
}


/**
 * Read the current state for pio(s).
 * @param pins
 * @return pin set
 */
pio_set PIO_Read(PIN_DESC pins)
{
	unsigned int port_num;

	port_num = PD_PORT_Get(pins);
	if(port_num <= PD_LAST_PORT)
	{
		GPIO_TypeDef* gpio_port = PORT_ADDRESS( port_num );
//	    ASSERT(SYSCTL->RCGC2&(1<<port_num));
		return (gpio_port->GPIO_IDR & pins & 0xFFFF);
	}
#if ( USE_GPIO_EXPANDER !=	0)
	return VPIO_Read(pins);
#else
	return 0;
#endif
}


/**
 * Drive output pins to high level
 * @param pins
 */
void PIO_SetOutput(PIN_DESC pins)
{
	PIO_Write(pins, PD_PINS_Get(pins));
}


/**
 * Drive output pins to low level
 * @param pins
 */
void PIO_ClrOutput(PIN_DESC pins)
{
	PIO_Write(pins, 0);
}


/**
 * Configure pins as output and drive high level
 * @param pins
 */
void PIO_CfgOutput1(PIN_DESC pins)
{
	if(! (pins & PD_OSPEED_Msk))
		pins |= PD_OSPEED_2MHz;
	PIO_Cfg((pins & ~PD_MODE_Msk) | PD_MODE_OUTPUT | PD_ACTIVE_HIGH);
}


/**
 * Configure pins as output and drive low level
 * @param pins
 */
void PIO_CfgOutput0(PIN_DESC pins)
{
	if(! (pins & PD_OSPEED_Msk))
		pins |= PD_OSPEED_2MHz;
	PIO_Cfg((pins & ~(PD_MODE_Msk | PD_ACTIVE_HIGH)) | PD_MODE_OUTPUT);
}

/**
 * Configure pins as output and drive active level
 * @param pins
 */
void PIO_Assert(PIN_DESC pins)
{
	PIO_Write(pins, (pins & PD_ACTIVE_HIGH) ? PD_PINS_Get(pins):0);
}

/**
 * Configure pins as output and drive inactive level
 * @param pins
 */
void PIO_Deassert(PIN_DESC pins)
{
	PIO_Write(pins, (pins & PD_ACTIVE_HIGH) ? 0:PD_PINS_Get(pins));
}


/**
 * Configure pins as input
 * @param pins
 */
void PIO_CfgInput(PIN_DESC pins)
{
	PIO_Cfg((pins & ~PD_MODE_Msk ) | PD_MODE_INPUT);
}

void exti_set_line_source(unsigned int line, unsigned int port)
{
	__IO uint32_t * reg;

	reg = &AFIO->AFIO_EXTICR[line >>2];
	line &= 3;
	*reg = (*reg & ~AFIO_EXTICRy_EXTIx_Msk(line)) | AFIO_EXTICRy_EXTIx_Set(line, port & 0xF);
}


/** @} */





