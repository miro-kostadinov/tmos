/*
 * lcd_ST7565S_RS.cpp
 *
 *  Created on: 03.09.2013
 *      Author: stanly
 */


#include <tmos.h>
#include <lcd_ST7565S_RS.h>
#include <lcd_ST7565S_cmd.h>
#include <cmsis_cpp.h>

#define A0_PIN_INDX	2
#define CS_PIN_INDX	3

void ST7565S_RS::lcd_command(unsigned int cmd)
{

	cmd = __RBIT(cmd);

	// A0=0 (cmds)
//	PIO_Deassert(pins[A0_PIN_INDX]);
	PIO_Assert(pins[CS_PIN_INDX]);

	//command
	lcd_hnd->tsk_write_locked(((char*)&cmd)+3, 1);

	//release A0
	PIO_Deassert(pins[CS_PIN_INDX]);
//	PIO_Deassert(pins[A0_PIN_INDX]);

}

void ST7565S_RS::update_screen()
{
	unsigned int cmd;

	cmd = CMD_PAGE_ADR(frame_y0 /8) +	(CMD_COLUMN_ADR_LO(4) << 8 )+
			(CMD_COLUMN_ADR_HI(0) << 16) + (CMD_READ_WRITE_START << 24);
	cmd = __REV(__RBIT(cmd));

	// A0=0
//	PIO_Assert(pins[A0_PIN_INDX]);
	PIO_Assert(pins[CS_PIN_INDX]);

   	//command
	lcd_hnd->tsk_write_locked(&cmd, 4);

	// A0=1
	PIO_Assert(pins[A0_PIN_INDX]);

   	// data
	cmd = size_x/4;
	unsigned int*ptr = (unsigned int*)(void*)disp_buf;
	do
	{
		*ptr = __REV(__RBIT(*ptr));
		++ptr;
	}while(--cmd);
	lcd_hnd->tsk_write_locked(disp_buf, size_x);

	//release A0
	PIO_Deassert(pins[CS_PIN_INDX]);
	PIO_Deassert(pins[A0_PIN_INDX]);
//   	PIO_Cfg(pins[A0_PIN_INDX]);


}
