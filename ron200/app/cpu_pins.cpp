/*
 * cpu_pins.cpp
 *
 *  Created on: 03.05.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include "cpu_pins.h"


//------------------------------------------------------------------------------
//	Power
//------------------------------------------------------------------------------
const GPIO_STRU PWR_DISABLE = {
		PWR_DIS_PIN,
		PIOMODE_OER,
		PORT_A
};

const GPIO_STRU F_VCCO_EN = {
		F_VCCO_EN_PIN,
		PIOMODE_OER | PIOMODE_SODR,
		PORT_B
};

const GPIO_STRU J_VCC_EN = {
		J_VCC_EN_PIN,
		PIOMODE_OER | PIOMODE_SODR,
		PORT_A
};

void power_fpga_core_on()
{
    PIO_CfgOutput0(&PWR_DISABLE);
}

void power_fpga_core_off()
{
	power_fpga_interface_off();
    PIO_CfgOutput1(&PWR_DISABLE);
}

void power_fpga_interface_on()
{
	power_fpga_core_on();
    PIO_CfgOutput1(&F_VCCO_EN);
}

void power_fpga_interface_off()
{
    PIO_CfgOutput0(&F_VCCO_EN);
}

//------------------------------------------------------------------------------
// FPGA
//------------------------------------------------------------------------------
const GPIO_STRU FPGA_DATANCLK = {
	PIO_PA0 | PIO_PA1 | PIO_PA2 | PIO_PA3 | PIO_PA4 | PIO_PA5 | PIO_PA6 | PIO_PA7 | FPGA_CLK_PIN,
	PIOMODE_OER,
	PORT_A
};

const GPIO_STRU FPGA_DATA = {
	PIO_PA0 | PIO_PA1 | PIO_PA2 | PIO_PA3 | PIO_PA4 | PIO_PA5 | PIO_PA6 | PIO_PA7,
	0,
	PORT_A
};

const GPIO_STRU FPGA_INIT = {
		FPGA_INIT_PIN,
		0,
		PORT_A
};

const GPIO_STRU FPGA_PROG = {
		FPGA_PROG_PIN,
		PIOMODE_OER,
		PORT_A
};

const GPIO_STRU FPGA_RDWR = {
		FPGA_RDWR_PIN,
		PIOMODE_OER,
		PORT_A
};

const GPIO_STRU FPGA_CS = {
		FPGA_CS_PIN,
		PIOMODE_OER,
		PORT_A
};

const GPIO_STRU FPGA_ADRWE = {
		FPGA_WE_PIN,
		PIOMODE_OER,
		PORT_A
};

const GPIO_STRU FPGA_CLK = {
		FPGA_CLK_PIN,
		0,
		PORT_A
};

