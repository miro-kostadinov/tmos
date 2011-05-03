/*
 * fpga.cpp
 *
 *  Created on: 18.04.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include "fpga.h"
#include <pwmc.h>
#include <pmc.h>
#include <matrix.h>
#include "cpu_pins.h"

#include <if_jtag.h>



static uint8_t swap_bits( uint8_t bb )
{
	unsigned int res;
	asm volatile (
		"	rbit			%0, %1				\n\t"

	  : "=r"(res)
	  : "r" (bb)
	  :
	  );

	return res >> 24;
}

int fpga_open( HANDLE fpga_pio_hnd )
{
	uint32_t pio, ii;

	// Toggle PROG_B Low to reset FPGA
	//================================
	PIO_CfgInput(&FPGA_INIT);
	PIO_CfgOutput1(&FPGA_RDWR);
	PIO_CfgOutput1(&FPGA_CS);

	PIO_CfgOutput0(&FPGA_PROG);
	for (ii = 1000;  ii && pio; ii-- )
		pio = PIO_Read(&FPGA_INIT);

	if ( ii == 0 )
		return 1;

	// FPGA ready for configuration when INIT_B returns High
	//======================================================
	PIO_SetOutput(&FPGA_PROG);
	for (ii = 1000;  ii && !pio; ii-- )
		pio = PIO_Read(&FPGA_INIT);

	if ( ii == 0 )
		return 1;

	// Drive RDWR_B Low coincident with driving CSI_B Low.
	PIO_ClrOutput(&FPGA_RDWR);
	PIO_ClrOutput(&FPGA_CS);

	return 0;
}

extern volatile unsigned int g_fcco_pwm;

static void start_fpga_clk(void)
{
	Pwm* pwmc = PWM;


	// Turn on 12MHz on PWM channel 1
 	PMC_EnablePeripheral(ID_PWM);
    pwmc->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_MCK; //24MHz
    pwmc->PWM_CH_NUM[1].PWM_CPRD = 4;
    pwmc->PWM_CH_NUM[1].PWM_CDTY = 2;
    pwmc->PWM_ENA = PWM_ENA_CHID1;

    // enable pwm channel 1 on this pin
	PIO_CfgPeriphB(&FPGA_INIT);


	// Apply 3.3V on DACC (reference for F_VCCO )
	g_fcco_pwm = 2500;


	//Enable F_VCCO
//	CHandle pio_hnd;
//	pio_open( &pio_hnd, &F_VCCO_EN );

	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO12; // Enable PB12 (erase) pin

	power_fpga_interface_on();
}

if_jtag jtag;

int fpga_close( HANDLE fpga_pio_hnd )
{
	uint32_t ii, pio;

	// check for DONE??? - not available
	//==================================

	// eight CCLK cycles after DONE goes High
	for (ii = 0; ii < 160; ii++)
	{
		// toggle clock
		pio = 0;
		fpga_pio_hnd->tsk_write( &pio, sizeof(pio) );
		pio = FPGA_CLK_PIN;
		fpga_pio_hnd->tsk_write( &pio, sizeof(pio) );
	}

	pio = PIO_Read(&FPGA_INIT);

	PIO_SetOutput(&FPGA_CS);
//	PIO_SetOutput(&FPGA_RDWR);

	// Reset FPGA
	PIO_CfgOutput1(&FPGA_CS);
	PIO_ClrOutput(&FPGA_CS);
	PIO_SetOutput(&FPGA_CS);

	start_fpga_clk();

	int res;
	res = jtag.jtag_idcode_get(-1,-2, ii);
	TRACELN("ID CODE %x = %x", res, ii);

	// check for CRC error
	//====================
	return pio ? 0: 1;
}

void fpga_write( HANDLE fpga_pio_hnd, uint8_t *data, uint32_t len )
{
	uint32_t ii, pio;

	for (ii = 0; ii < len; ii++)
	{
		pio = swap_bits( data[ii] );

		// clock low
		fpga_pio_hnd->tsk_write( &pio, sizeof(pio) );

		// write data with clock high
		pio |= FPGA_CLK_PIN;
		fpga_pio_hnd->tsk_write( &pio, sizeof(pio) );
	}
}

