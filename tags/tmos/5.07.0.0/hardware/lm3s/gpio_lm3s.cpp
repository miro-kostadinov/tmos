/**************************************************************************//**
 * @ingroup	 lm3s_gpio
 * @{
 * @file     gpio_lm3s.cpp
 * @brief    GPIO class functions
 * @version  V3.00
 * @date     12. March 2010
 * @author	 Miroslav Kostadinov
 *
 *
 ******************************************************************************/

#include <tmos.h>
#include "fam_cpp.h"


/**
 * Configure pin(s) according to the pin description
 * @param cfg
 */
void PIO_Cfg(PIN_DESC cfg)
{
	unsigned int port_num, pin_pattern;
	GPIO_Type* port_base;

	port_num = PD_PORT_Get(cfg);
	if(port_num <= PD_LAST_PORT)
	{
		port_base = PORT_ADDRESS(port_num );
		port_num = 1 << port_num;
		pin_pattern = PD_PINS_Get(cfg);

		// peripheral clock enable
		SYSCTL->RCGC2 |= port_num;
		SYSCTL->DCGC2 |= port_num; // SysCtlPeripheralDeepSleepEnable
		SYSCTL->SCGC2 |= port_num; // SysCtlPeripheralSleepEnable

		// Unlock the commit register
		if(cfg & PD_LOCK)
		{
			port_base->GPIOLOCK = GPIO_LOCK_KEY_DD;
			port_base->GPIOCR |= pin_pattern;
		}

		// Set default output level. This has no effect if the pin is not yet
		// configured as output
		port_base->GPIODATA[pin_pattern] = (cfg & PD_ACTIVE_HIGH)?0:pin_pattern;

	    // Set the output drive strength.
		(&port_base->GPIODR2R)[PD_STRENGTH_Get(cfg)] |= pin_pattern;

		// Set slew rate
		port_base->GPIOSLR = ((cfg & PD_SLEW_RATE)?
				(port_base->GPIOSLR | pin_pattern) :
				(port_base->GPIOSLR & ~pin_pattern));

	    // Set the pin type.
		port_base->GPIOODR = ((cfg & PD_MODE_OD) ?
				(port_base->GPIOODR | pin_pattern) :
				(port_base->GPIOODR & ~pin_pattern));
		port_base->GPIOPUR = ((cfg & PD_PULL_UP) ?
				(port_base->GPIOPUR | pin_pattern) :
				(port_base->GPIOPUR & ~pin_pattern));
		port_base->GPIOPDR = ((cfg & PD_PULL_DOWN) ?
				(port_base->GPIOPDR | pin_pattern) :
				(port_base->GPIOPDR & ~pin_pattern));
		port_base->GPIODEN = ((cfg & (PD_PUSH_PULL | PD_MODE_OD)) ?
				(port_base->GPIODEN | pin_pattern) :
				(port_base->GPIODEN & ~pin_pattern));

	    //
	    // Set the analog mode select register.  This register only appears in
	    // DustDevil-class (and later) devices, but is a harmless write on
	    // Sandstorm- and Fury-class devices.
	    //
		port_base->GPIOAMSEL = ((PD_MODE_Get(cfg) == PD_MODE_ANALOG) ?
				(port_base->GPIOAMSEL | pin_pattern) :
				(port_base->GPIOAMSEL & ~pin_pattern));

		//	Select input/output
		port_base->GPIODIR = ((cfg & PD_OUTPUT) ?
	            (port_base->GPIODIR | pin_pattern) :
	            (port_base->GPIODIR & ~pin_pattern));

		// If GPIODEN is set and a pin then becomes an output a feedback circuit
		// will sense the level of the pin before it becomes an output and keep
		// the pin at that level after it becomes an output.
		// We do not want to keep the current level but to set the default from
		// the configuration
		port_base->GPIODATA[pin_pattern] = (cfg & PD_ACTIVE_HIGH)?0:pin_pattern;


		//	Select GPIO/peripheral
		port_base->GPIOAFSEL = ((cfg & PD_AFSEL) ?
	            (port_base->GPIOAFSEL | pin_pattern) :
	            (port_base->GPIOAFSEL & ~pin_pattern));

		//	Select mux
		asm volatile ("clz %0, %1" : "=r" (port_num) : "r" (pin_pattern) );
		port_num = (31- port_num)*4;
		port_base->GPIOPCTL = ( (port_base->GPIOPCTL &~(0xf << port_num)) |
				(PD_MUX_Get(cfg) << port_num) );

		//	Select interrupt type
		port_base->GPIOIBE = ((cfg & PD_INT_BE) ?
				(port_base->GPIOIBE | pin_pattern) :
				(port_base->GPIOIBE & ~pin_pattern));
		port_base->GPIOIS = ((cfg & PD_INT_SENSE) ?
				(port_base->GPIOIS | pin_pattern) :
				(port_base->GPIOIS & ~pin_pattern));
		port_base->GPIOIEV = ((cfg & PD_INT_EVENT) ?
				(port_base->GPIOIEV | pin_pattern) :
				(port_base->GPIOIEV & ~pin_pattern));

		// Lock the commit register
		if(cfg & PD_LOCK)
		{
			port_base->GPIOLOCK = GPIO_LOCK_KEY_DD;
			port_base->GPIOCR &= ~pin_pattern;
		}
	} else
	{
		//for virtual ports just set the default level
		PIO_Deassert(cfg);
	}
}


/**
 * Configure a list with pin descriptions
 * @param list
 */
void PIO_Cfg_List(const PIN_DESC* list )
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
 * port number to driver index table
 */
extern "C" const DRIVER_INDEX GPIO_IRQ_INDX[] = {
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


/**
 * Free resources used for the pins
 * This function should be called when the pins will no longer used. The function
 * will disable the gpio interrupts etc.
 * @note no usage counter is used, so the first call will free the resources!
 * @param cfg
 */
void PIO_Free(PIN_DESC cfg)
{
	unsigned int port_num, pin_pattern;
	GPIO_Type* port_base;


	port_num = PD_PORT_Get(cfg);
	if(port_num <= PD_LAST_PORT)
	{
		port_base = PORT_ADDRESS( port_num );
		pin_pattern = PD_PINS_Get(cfg);
		port_base->GPIOIM &= ~pin_pattern;
		if(!port_base->GPIOIM)
		{
			NVIC_DisableIRQ(GPIO_IRQ_INDX[port_num]);
		}
	}
}


/**
 * Free resources for a list of pin descriptions
 * @param list
 */
void PIO_Free_List(PIN_DESC* list)
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
		GPIO_Type* port_base = PORT_ADDRESS( port_num );
		ASSERT(SYSCTL->RCGC2&(1<<port_num));
		port_base->GPIODATA[PD_PINS_Get(pins)] = val;
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
		GPIO_Type* port_base = PORT_ADDRESS( port_num );
	    ASSERT(SYSCTL->RCGC2&(1<<port_num));
		return (port_base->GPIODATA[PD_PINS_Get(pins)]);
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
	PIO_Cfg((pins & ~PD_AFSEL) | PD_OUTPUT);
	PIO_Write(pins, PD_PINS_Get(pins));
}


/**
 * Configure pins as output and drive low level
 * @param pins
 */
void PIO_CfgOutput0(PIN_DESC pins)
{
	PIO_Cfg((pins & ~PD_AFSEL) | PD_OUTPUT);
	PIO_Write(pins, 0);
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
	PIO_Cfg(pins & ~(PD_AFSEL | PD_OUTPUT));
}


/** @} ingroup lm3s_gpio */
