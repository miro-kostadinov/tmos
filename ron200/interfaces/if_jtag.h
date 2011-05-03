/*
 * if_jtag.h
 *
 *  Created on: 22.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef IF_JTAG_H_
#define IF_JTAG_H_

#include "interface.h"
#include "fpga_regs.h"

#define	JTAG_MAX_TAPS				16
#define JTAG_MAX_TAP_IR_LEN			32
#define JTAG_MAX_SUM_TAP_IR_LEN		255
#define	JTAG_MAX_CMDS				(1 << 8)

// The PATHs for the TAP state machine.
// The shift starts with the MSB until the last '1' which is not send
//====================================================================
#define	TAP_PATH_RUNIDLE_SHIFT_DR			(0x480 << JTAG_REG_CMD_TMSSTART)		// Run-Idle -> Select-DR -> Capture-DR -> Shift-DR
#define	TAP_PATH_RUNIDLE_SHIFT_IR			(0x640 << JTAG_REG_CMD_TMSSTART)		// Run-Idle -> Select-DR -> Select-IR ->Capture-IR -> Shift-IR
#define	TAP_PATH_PAUSE_UPDATE_SHIFT_DR		(0x720 << JTAG_REG_CMD_TMSSTART)		// Pause -> Exit2-DR -> Update-DR -> Select-DR -> Capture-DR -> Shift-DR
#define	TAP_PATH_PAUSE_UPDATE_SHIFT_IR		(0x790 << JTAG_REG_CMD_TMSSTART)		// Pause -> Exit2-IR -> Update-IR -> Select-DR -> Select-IR -> Capture-IR -> Shift-IR
#define	TAP_PATH_PAUSE_SHIFT				(0x500 << JTAG_REG_CMD_TMSSTART)		// Pause -> Exit2-xR -> Shift-xR
#define	TAP_PATH_PAUSE_RUN_UPDATE_SHIFT_DR	(0x690 << JTAG_REG_CMD_TMSSTART)		// Pause -> Exit2-DR -> Update-DR -> Run_Idle -> Select-DR -> Capture-DR -> Shift-DR
#define	TAP_PATH_PAUSE_RUN_UPDATE_SHIFT_IR	(0x6C8 << JTAG_REG_CMD_TMSSTART)		// Pause -> Exit2-IR -> Update-IR -> Run_Idle -> Select-DR -> Select-IR -> Capture-IR -> Shift-IR
#define TAP_PATH_RESET						(0x7FF << JTAG_REG_CMD_TMSSTART)

#define	JFLG_RUNIDLE						(JTAG_REG_CMD_RUNIDLE)
#define JFLG_LSB							(JTAG_REG_CMD_LSB)
#define	JFLG_READ							(JTAG_REG_CMD_READBACK | JTAG_REG_CMD_SKIP_BACK | JTAG_REG_CMD_HEAD)
#define	JFLG_FOOT							(JTAG_REG_CMD_FOOT)
#define	JFLG_HEAD							(JTAG_REG_CMD_HEAD)
#define	JFLG_STOP							(JTAG_REG_CMD_TCKSTOP)

#define  JFLG_IR_CMD						(JTAG_REG_CMD_IR | JTAG_REG_CMD_HEAD | JTAG_REG_CMD_FOOT | curr_ir_len)


// the state of the TAP port
//==========================
enum TAP_STATE
{
    TAP_STATE_RESET,
    TAP_STATE_RUN_IDLE,
    TAP_STATE_SELECT_DR,
    TAP_STATE_SELECT_IR,
    TAP_STATE_CAPTURE_DR,
    TAP_STATE_CAPTURE_IR,
    TAP_STATE_SHIFT_DR,
    TAP_STATE_SHIFT_IR,
    TAP_STATE_EXIT1_DR,
    TAP_STATE_EXIT1_IR,
    TAP_STATE_PAUSE_DR,
    TAP_STATE_PAUSE_IR,
    TAP_STATE_EXIT2_DR,
    TAP_STATE_EXIT2_IR,
    TAP_STATE_UPDATE_DR,
    TAP_STATE_UPDATE_IR

} ;

class if_jtag: interface
{
	int				curr_tap;				// current TAP
	TAP_STATE		curr_tap_state;
	int				curr_ir_len;
	int 			idcode_idx;
	volatile int	jtag_flag_error;

	int				tap_ir_len[JTAG_MAX_TAPS];			// the length of each IR in JTAG chain

	// some variables for the TAP state machine
	//=========================================
	uint32_t		tap_reg_lens[JTAG_MAX_TAPS];				// value for fpga register lens
	uint32_t		tap_reg_ir_lens[JTAG_MAX_TAPS];				// value for fpga IR register lens

	unsigned int 	io_adr;
	bool	io_output_mode;
	CHandle			io_hnd;


	void close();

	void io_setup_write(unsigned int reg);
	void io_setup_read(unsigned int reg);
	void io_do_write(unsigned int val);
	int io_do_read();

	void io_write(unsigned int reg, int val)
	{
		io_setup_write(reg);
		io_do_write(val);
	}

	int io_read(unsigned int reg)
	{
		io_setup_read(reg);
		return io_do_read();
	}

	void io_write_data(int val)
	{
		io_write(JTAG_REG_DATA, val);
	};

	void io_write_cmd(int val)
	{
		io_write(JTAG_REG_CMD, val);
	};

	int  io_read_data()
	{
		return io_read(JTAG_REG_DATA);
	};

	int io_read_ctrl()
	{
		return io_read(JTAG_REG_CTRL);
	};

	void timer_time_start(unsigned int count)
	{

	}
	int timer_time()
	{
		return io_read_ctrl() & JTAG_REG_CTRL_TIME;
	}

	RES_CODE jtag_read_data(unsigned int& result);
	RES_CODE tap_idcode_get(int idcode_cmd, unsigned int& result);
	void jtag_wait_ready(void);

public:
	void init();
	RES_CODE jtag_idcode_get( int ntap, int ir_cmd, unsigned int& idcode );
	void jtag_select_tap( int ntap );

};


#endif /* IF_JTAG_H_ */
