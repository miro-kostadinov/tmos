/*
 * if_jtag.cpp
 *
 *  Created on: 24.04.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include "if_jtag.h"
#include "codes.h"
#include <cpu_pins.h>

#define JTAG_MAX_IDCODE		8
typedef struct
{
 	int ir_len;
 	int idcode[JTAG_MAX_IDCODE];

} JTAG_IDCODES;

const JTAG_IDCODES  Jtag_idcodes[] =
{
	{
		3,
		{ 0x6, -1 }
	},

	{ 	// ARM cores
		4,
		{ 0xE, -1 }
	},

	{
		5,
		{
			0x9, 		// Xilinx Virtex, SPARTAN II
			0x1E, 		// XScale, ARM11
			0x1, 		// MIPS32
			0x2, 		// Blaclfin
			-1
		},
	},

	{
		// TI TMS320DM6446
		6,
		{ 0x4, 0x9, -1 }
	},

	{ 	// XScale, PPC400
		7,
		{ 0x7E, 0x7B, -1 }
	},

	{ 	// MPC5200, MPC8300, MPC8500, Xilinx
		8,
		{ 0x74, 0x1, 0xFE, 0xF3, -1 }
	},

	{ 	// Altera FPGA, MPC5500
		10,
		{ 0x2, 0x6, -1 }
	},

	{ 	// XScale PXA320
		11,
		{ 0x4, -1 }
	},

	{ 	// Chili - currently not supported
		12,
		{ 0xE, -1 }
	},

	{ 	// Intel LXT972A
		16,
		{ 0xFFFE, -1 }
	},


	{ 	// End record - must be last
		-1,
		{ -1 }
	},
};

//const GPIO_STRU IO_BUS = {
//	PIO_PA0 | PIO_PA1 | PIO_PA2 | PIO_PA3 | PIO_PA4 | PIO_PA5 | PIO_PA6 | PIO_PA7 |
//		FPGA_CLK_PIN, FPGA_CS_PIN, FPGA_RDWR_PIN,
//	PIOMODE_OER | PIOMODE_SODR,
//	PORT_A
//};
//
//const GPIO_STRU IO_DATA = {
//	PIO_PA0 | PIO_PA1 | PIO_PA2 | PIO_PA3 | PIO_PA4 | PIO_PA5 | PIO_PA6 | PIO_PA7,
//	0,
//	PORT_A
//};

void if_jtag::init()
{
	io_adr = -1;
	io_output_mode = false;

	PIO_ClrOutput(&FPGA_RDWR);
	pio_open( &io_hnd, &FPGA_DATA );

	PIO_SetOutput(&FPGA_CLK);

	PIO_CfgOutput1(&FPGA_ADRWE);

	idcode_idx = 0;

}

void if_jtag::close()
{
	io_hnd.close();
}

void if_jtag::io_setup_write(unsigned int reg)
{
	// switch to output
	if( !io_output_mode )
	{
		io_output_mode = true;
		PIO_ClrOutput(&FPGA_RDWR);
		PIO_CfgOutput(&FPGA_DATA);
	}

	//change the address
	if( reg != io_adr)
	{
		PIO_SetOutput(&FPGA_ADRWE);
		io_hnd.tsk_write(&reg, 4);

		PIO_ClrOutput(&FPGA_CLK);
		PIO_SetOutput(&FPGA_CLK);

		PIO_ClrOutput(&FPGA_ADRWE);
	}
}

void if_jtag::io_setup_read(unsigned int reg)
{

	//change the address
	if( reg != io_adr)
	{
		// switch to output
		if( !io_output_mode )
		{
			io_output_mode = true;
			PIO_ClrOutput(&FPGA_RDWR);
			PIO_CfgOutput(&FPGA_DATA);
		}

		PIO_SetOutput(&FPGA_ADRWE);
		io_hnd.tsk_write(&reg, 4);

		PIO_ClrOutput(&FPGA_CLK);
		PIO_SetOutput(&FPGA_CLK);

		PIO_ClrOutput(&FPGA_ADRWE);
	}

	// switch to input
	if( io_output_mode )
	{
		io_output_mode = false;
		PIO_CfgInput(&FPGA_DATA);
		PIO_SetOutput(&FPGA_RDWR);
	}
}

void if_jtag::io_do_write(unsigned int val)
{
	io_hnd.tsk_write(&val, 4);
	PIO_ClrOutput(&FPGA_CLK);
	PIO_SetOutput(&FPGA_CLK);
	val >>= 8;
	io_hnd.tsk_write(&val, 4);
	PIO_ClrOutput(&FPGA_CLK);
	PIO_SetOutput(&FPGA_CLK);
	val >>= 8;
	io_hnd.tsk_write(&val, 4);
	PIO_ClrOutput(&FPGA_CLK);
	PIO_SetOutput(&FPGA_CLK);
	val >>= 8;
	io_hnd.tsk_write(&val, 4);
	PIO_ClrOutput(&FPGA_CLK);
	PIO_SetOutput(&FPGA_CLK);
}

int if_jtag::io_do_read()
{
	int val;

	PIO_ClrOutput(&FPGA_CLK);
	val = PIO_Read(&FPGA_DATA);
	PIO_SetOutput(&FPGA_CLK);

	PIO_ClrOutput(&FPGA_CLK);
	val |= PIO_Read(&FPGA_DATA) << 8;
	PIO_SetOutput(&FPGA_CLK);

	PIO_ClrOutput(&FPGA_CLK);
	val |= PIO_Read(&FPGA_DATA) << 16;
	PIO_SetOutput(&FPGA_CLK);

	PIO_ClrOutput(&FPGA_CLK);
	val |= PIO_Read(&FPGA_DATA) << 24;
	PIO_SetOutput(&FPGA_CLK);

	return val;
}


//==============================================================================
//						JTAG_GET_ID()
//						-------------
//	Parameter:
//		result  -> get the result here
//
//	Description:
//		Put the TAP in IDCODE state and get the idcode
//
//	Start state: dosn't matter
//	End   state: RUN_IDLE
//==============================================================================

RES_CODE if_jtag::tap_idcode_get(int idcode_cmd, unsigned int& result)
{
	RES_CODE res;
	unsigned int tap_path_tms;

	tap_path_tms = (curr_tap_state == TAP_STATE_RUN_IDLE) ?
			TAP_PATH_RUNIDLE_SHIFT_IR : TAP_PATH_PAUSE_UPDATE_SHIFT_IR;

	io_write_cmd(tap_path_tms + JFLG_RUNIDLE + JFLG_LSB + JFLG_IR_CMD);
	io_write_data( idcode_cmd);

	io_write_cmd(TAP_PATH_RUNIDLE_SHIFT_DR + JFLG_READ + JFLG_LSB + JFLG_RUNIDLE + 32);
	io_write_data(0);
	curr_tap_state = TAP_STATE_RUN_IDLE;

	res = jtag_read_data(result);

	curr_tap_state = TAP_STATE_RUN_IDLE;

	return res;
}


RES_CODE if_jtag::jtag_idcode_get( int ntap, int ir_cmd, unsigned int& idcode )
{
//	JTAG_CONFIG *config = &Jtag_config;
	const JTAG_IDCODES *pp = &Jtag_idcodes[0];

	if ( ntap == -1 )
	{
		idcode_idx = 0;
		return EX_SUCCESS;
	}

	jtag_select_tap( ntap );

	// removed because of problems with OMAP-L138 - to be tested on other targets
	//===========================================================================
//	jtag_reset();

	// 10ms pause
	//===========
	jtag_wait_ready();

	timer_time_start( 20*1000);
	while( timer_time() );

	while( (pp->ir_len != -1) && pp->ir_len != tap_ir_len[ntap] )
		pp++;

	if ( pp->ir_len == -1 )
	{
		idcode = 0;
		return EINVAL;
	}

	if ( pp->idcode[idcode_idx] == -1 )
		return EX_GENERIC;

	if ( ir_cmd != -1 )
		tap_idcode_get( ir_cmd, idcode );
	else
		tap_idcode_get( pp->idcode[idcode_idx++], idcode );

    return EX_SUCCESS;
}

void if_jtag::jtag_select_tap( int ntap )
{
	if ( ntap != curr_tap )
	{
		TRACELN("jtag_select_tap(ntap = %d)", ntap );

		// wait to complete transmitting
		//=============================
		jtag_wait_ready();

		io_write(JTAG_REG_LENS,  tap_reg_lens[ntap]);
		io_write(JTAG_REG_IR_LENS, tap_reg_ir_lens[ntap]);

		curr_ir_len = tap_ir_len[ntap];
		curr_tap = ntap;
	}


}

void if_jtag::jtag_wait_ready(void)
{
	unsigned int time;
	unsigned int reg;

	jtag_flag_error = 0;
	time = CURRENT_TIME;
	do
	{
		reg = io_read_ctrl();
		if( !(reg & JTAG_REG_CTRL_BUSY))
			return;

		if(Iface_flag_disconnected)
			break;

	} while ((CURRENT_TIME - time) < 1000);

	jtag_flag_error = true;

	#ifdef DEBUG
			ERROR_MSG("FIFO error %d", jtag_flag_error);
	#endif
}

RES_CODE if_jtag::jtag_read_data(unsigned int& result)
{
	unsigned int reg;
	do
	{
		reg = io_read_ctrl();
		if( !(reg & JTAG_REG_CTRL_OUT_EMPTY) )
		{
			result = io_read_data();
			return EX_SUCCESS;
		}
	} while(reg & JTAG_REG_CTRL_TIME);
	result = io_read_data();
	return EX_GENERIC;
}
