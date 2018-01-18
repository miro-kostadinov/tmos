/**************************************************************************//**
 * @ingroup	 hardware_stm32f2_gpio
 * @{
 * @file     hardware/stm32/stm32f2/gpio_f2.cpp
 * @brief    STM32F2 GPIO
 * @version  V3.00
 * @date     30. October 2012
 * @author	 Miroslav Kostadinov
 * @}
 */

#include <tmos.h>
#include "fam_cpp.h"

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
		RCC->RCC_AHB1ENR |= RCC_AHB1ENR_GPIOAEN << tmpreg;

		gpio_port = PORT_ADDRESS( tmpreg );
		pin_pattern = PD_PINS_Get(cfg);

		//loop pins
		while(pin_pattern)
		{
			pos = 1+ __CLZ(pin_pattern);
			pin_pattern = (pin_pattern << pos) >> pos;
			pos = (32-pos)*2;

			mode = PD_MODE_Get(cfg);

			/* Change AF mapping */
			if(mode == PD_MODE_Get(PD_MODE_AF))
			{
				do
				{
					tmpreg = locked_get_reg(&gpio_port->GPIO_AFR[pos >> 4]);
					tmpreg &= ~GPIO_AFR_AFRy_Msk(pos>>1);
					tmpreg |= GPIO_AFR_AFRy_Set(pos>>1, PD_MUX_Get(cfg));
				} while (locked_set_reg(&gpio_port->GPIO_AFR[pos >> 4], tmpreg));
			}
			/* set default pin level*/
			if(mode == PD_MODE_Get(PD_MODE_OUTPUT))
			{
				gpio_port->GPIO_BSRR = (1<<(pos>>1)) << ((cfg & PD_ACTIVE_HIGH)?16:0);
			}

			/* Change mode */
			do
			{
				tmpreg = locked_get_reg(&gpio_port->GPIO_MODER);
				tmpreg &= ~(0x3 << pos);
				tmpreg |= mode << pos;
			} while (locked_set_reg(&gpio_port->GPIO_MODER, tmpreg));

			/* Change pullup/pulldowns */
			do
			{
				tmpreg = locked_get_reg(&gpio_port->GPIO_PUPDR);
				tmpreg &= ~(0x3 << pos);
				tmpreg |= PD_PULL_Get(cfg) << pos;
			} while (locked_set_reg(&gpio_port->GPIO_PUPDR, tmpreg));

			if(mode == PD_MODE_Get(PD_MODE_OUTPUT) || mode == PD_MODE_Get(PD_MODE_AF))
			{
				/* Change speed */
				do
				{
					tmpreg = locked_get_reg(&gpio_port->GPIO_OSPEEDR);
					tmpreg &= ~(0x3 << pos);
					tmpreg |= PD_OSPEED_Get(cfg) << pos;
				} while (locked_set_reg(&gpio_port->GPIO_OSPEEDR, tmpreg));

				pos /= 2;

				/* Change output type */
				do
				{
					tmpreg = locked_get_reg(&gpio_port->GPIO_OTYPER);
					tmpreg &= ~(0x1 << pos);
					tmpreg |= PD_OTYPE_Get(cfg) << pos;
				} while (locked_set_reg(&gpio_port->GPIO_OTYPER, tmpreg));
			}
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
	PIO_Cfg((pins & ~(PD_MODE_Msk | PD_ACTIVE_HIGH)) | PD_MODE_OUTPUT);
}


/**
 * Configure pins as output and drive low level
 * @param pins
 */
void PIO_CfgOutput0(PIN_DESC pins)
{
	if(! (pins & PD_OSPEED_Msk))
		pins |= PD_OSPEED_2MHz;
	PIO_Cfg((pins & ~PD_MODE_Msk) | PD_MODE_OUTPUT | PD_ACTIVE_HIGH);
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

	reg = &SYSCFG->SYSCFG_EXTICR[line >>2];
	line &= 3;
	*reg = (*reg & ~SYSCFG_EXTICRy_EXTIx_Msk(line)) | SYSCFG_EXTICRy_EXTIx_Set(line, port & 0xF);
}

