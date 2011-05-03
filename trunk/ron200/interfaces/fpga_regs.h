/*
 * fpga_regs.h
 *
 *  Created on: 24.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef FPGA_REGS_H_
#define FPGA_REGS_H_

#define	JTAG_REG_DATA					0x00				// address of data register for LITTLE endian
#define	JTAG_REG_DATA_BE				0x01				// address of data register for BIG endian
#define	JTAG_REG_CMD					0x00				// address of command register
#define	JTAG_REG_CTRL					0x80				// address of control/status register
#define	JTAG_REG_LENS					0x81				// address of lengths register
#define	JTAG_REG_CLK					0x82				// address of control register for JTAG clocks
#define	JTAG_REG_IR_LENS				0x83				// address of IR lengths register


#define	JTAG_REG_CTRL_RST				(1 << 0)
#define	JTAG_REG_CTRL_PLLEN				(1 << 1)
#define	JTAG_REG_CTRL_PLL_LOCKED		(1 << 2)
#define	JTAG_REG_CTRL_RUNCLK			(1 << 3)			// 1- clock enable; 0 - clock disabled
#define	JTAG_REG_CTRL_USE_RTCK			(1 << 4)
#define	JTAG_REG_CTRL_IN_REMPTY			(1 << 5)			// read-only; rdempty - if asserted, indicates that the fifo is empty
#define	JTAG_REG_CTRL_IN_WEMPTY			(1 << 6)			// read-only; wrempty - indicates that the fifo is empty; 1 - OUT_FIFO is empty, 0 - OUT_FIFO is NOT empty
#define	JTAG_REG_CTRL_OUT_EMPTY			(1 << 7)			// read-only; If asserted, indicates that the fifo is empty; 1 - IN_FIFO is empty, 0 - IN_FIFO in NOT empty
#define	JTAG_REG_CTRL_BUSY				(1 << 8)			// 1=fpga is doing something
#define	JTAG_REG_CTRL_TIME				(1 << 9)			// 1=time timer is still running


#define	JTAG_REG_CLK_RD_CLK_DELAY		(1 << 8)			// 8-bit, only 1 bit can be set
#define	JTAG_REG_CLK_RD_CLK_DELAY_MASK	(0xFFFF00FF)
#define	JTAG_REG_CLK_CLKDIV				(1 << 16)			// 16-bit JTAG clock devider

#define	JTAG_REG_LENS_FOOT_LEN			(1 << 0)			// 8-bit: number of bits send after data
#define	JTAG_REG_LENS_HEAD_LEN			(1 << 8)			// 8-bit: number of bits send before data
#define	JTAG_REG_LENS_SKIP_LEN			(1 << 16)			// 8-bit: number of skiped bits before begin of receiving

#define	JTAG_REG_CMD_LEN				(1 << 0) 			// 6-bits: number of data bits to send
#define	JTAG_REG_CMD_LSB				(1 << 8)			// flag LSB: if 1 first send bit0, bit1, bit2, ...
#define	JTAG_REG_CMD_READBACK			(1 << 12)			// if 1 read the same number of bits
#define	JTAG_REG_CMD_SKIP_BACK			(1 << 13)			// if 1 by reading skip JTAG_REG_LENS_SKIP_LEN bits
#define JTAG_REG_CMD_HEAD				(1 << 16)			// flag send HEAD
#define JTAG_REG_CMD_FOOT				(1 << 17)			// flag send FOOT
#define JTAG_REG_CMD_IR					(1 << 18)			// flag send IR command
#define JTAG_REG_CMD_TCKSTOP			(1 << 19)			// flag stop TCK
#define JTAG_REG_CMD_RUNIDLE			(1 << 20)			// if 1 go in RUN_IDLE mode, else PAUSE
#define JTAG_REG_CMD_TMSSTART			(21)				// 11-bit TMS sequence
#define JTAG_REG_CMD_TMS				(1 << JTAG_REG_CMD_TMSSTART)			// 11-bit TMS sequence

#define JTAG_REG_LEDCTRL_DIV			(1 << 0)			// 7-bits divider for led blink
#define JTAG_REG_LEDCTRL_AND			(1 << 7)			// AND bit
#define JTAG_REG_LEDCTRL_OR				(1 << 8)			// OR bit
#define	JTAG_REG_LEDCTRL_KEEP_TDI		(1 << 9)			// if 1 keep the TDI until the next Shift-xR

#define JTAG_REG_CLKCTRL				JTAG_REG_CTRL
#define	JTAG_REG_CLKCTRL_RUNCLK			JTAG_REG_CTRL_RUNCLK

#define	JTAG_REG_LENS_SCANPRED_LEN		JTAG_REG_LENS_FOOT_LEN
#define	JTAG_REG_LENS_SCANSUCC_LEN		JTAG_REG_LENS_HEAD_LEN

#define JTAG_REG_IR_LENS_FOOT_LEN		(1 << 0)
#define JTAG_REG_IR_LENS_HEAD_LEN		(1 << 8)



#endif /* FPGA_REGS_H_ */
