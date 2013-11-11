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

char inline ST7565S_RS::revert_char (char ch)
{
	unsigned int tmp = __RBIT((unsigned int) ch);
	ch = *(((char*)&tmp)+3);
	return ch;
}

void ST7565S_RS::draw_bitmap( int x0, int y0, const char* src, int width, int rows)
{
	int offset=0;
	if(y0 < frame.y0)
	{
		offset = frame.y0 - y0;
		if(offset >= rows)
			return;
		y0 += offset;
		rows -= offset;
		offset *= width;
		src += offset /8;
		offset %= 8;
	}
	if(y0 < frame.y1)
	{
		rows += y0;
		if(rows > frame.y1)
			rows = frame.y1;
		rows -= y0;

		width += x0;
		offset = 1<< offset;
		y0 = 1 << (y0&7);
		while(rows--)
		{
			for( int i=x0; i<width; i++)
			{
				if((src[0] & offset) && frame.x0 <= i && i <= frame.x1)
					disp_buf[frame.y0>>3][i] |= revert_char(y0);
				offset <<= 1;
				if(offset > 255)
				{
					offset = 1;
					src++;
				}
			}
			y0 <<= 1;
		}
	}
}

void ST7565S_RS::draw_point( int x, int y)
{
	if(frame.y0 <= y && y < frame.y1 && frame.x0 <= x && x <= frame.x1)
	{
		disp_buf[frame.y0>>3][x] |= revert_char(1 << (y&7));
	}
}

void ST7565S_RS::draw_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (frame.y0>>3) > 7)
				{
					TRACELN1("Oooops! draw_hline");
				}
				disp_buf[frame.y0>>3][x0] |= revert_char(y);
			}
			x0++;
		}
	}
}

void ST7565S_RS::draw_bline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = ~(1 << (y&7));
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (frame.y0>>3) > 7)
				{
					TRACELN1("Oooops! draw_bline");
				}
				disp_buf[frame.y0>>3][x0] &= revert_char(y);
			}
			x0++;
		}
	}
}

void ST7565S_RS::invert_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (frame.y0>>3) > 7)
				{
					TRACELN1("Oooops! invert_hline");
				}
				disp_buf[frame.y0>>3][x0] ^= revert_char(y);
			}
			x0++;
		}
	}
}

void ST7565S_RS::draw_vline( int y0, int y1, int x)
{
	if( (y0<=frame.y0) && (y1>=frame.y0) )
	{
		if (frame.x0 <= x && x <= frame.x1)
		{
			if(x > 131 || (frame.y0>>3) > 7)
			{
				TRACELN1("Oooops! draw_vline");
			}
			disp_buf[frame.y0>>3][x] |= revert_char(1 << (frame.y0&7));
		}
	}
}

void ST7565S_RS::invert_vline( int y0, int y1, int x)
{
	if( (y0<=frame.y0) && (y1>=frame.y0) )
	{
		if (frame.x0 <= x && x <= frame.x1)
		{
			if(x > 131 || (frame.y0>>3) > 7)
			{
				TRACELN1("Oooops! invert_vline");
			}
			disp_buf[frame.y0>>3][x] ^= revert_char(1 << (frame.y0&7));
		}
	}
}

void ST7565S_RS::update_screen()
{
	unsigned char cmd[4];

	cmd[0] = CMD_PAGE_ADR(frame.y0 /8);
	cmd[1] = CMD_COLUMN_ADR_LO( (ST7565S_GET_START_COLUMN(lcd_mode) + frame.x0) & 0x0F);
	cmd[2] = CMD_COLUMN_ADR_HI( ((ST7565S_GET_START_COLUMN(lcd_mode) + frame.x0) & 0xF0) >> 4);
	cmd[3] = CMD_READ_WRITE_START;
	*(unsigned int *)((void *)cmd) = __REV(__RBIT(*(unsigned int *)((void*)cmd)));

	// A0=0
//	PIO_Assert(pins[A0_PIN_INDX]);
	PIO_Assert(pins[CS_PIN_INDX]);

   	//command
	lcd_hnd->tsk_write_locked(cmd, 4);

	// A0=1
	PIO_Assert(pins[A0_PIN_INDX]);

   	// data
//	cmd = (1 + frame_x1 - frame_x0)/4;//size_x/4;
//	unsigned int*ptr = (unsigned int*)(void*)(disp_buf[frame_y0/8] + frame_x0);
//	do
//	{
//		*ptr = __REV(__RBIT(*ptr));
//		++ptr;
//	}while(--cmd);
//	unsigned char* cptr;
//	unsigned int tmp;
//	cmd = (1 + frame_x1 - frame_x0)&3;
//	cptr = (unsigned char*)ptr;
//	if (cmd)
//	{
//		tmp =  __REV(__RBIT(*ptr));
//	}
//	while (cmd--)
//	{
//		*cptr = (unsigned char) tmp;
//		tmp = tmp >> 8;
//		cptr++;
//	}
	lcd_hnd->tsk_write_locked(disp_buf[frame.y0>>3] + frame.x0, 1 + frame.x1 - frame.x0);//size_x);

	//release A0
	PIO_Deassert(pins[CS_PIN_INDX]);
	PIO_Deassert(pins[A0_PIN_INDX]);
//   	PIO_Cfg(pins[A0_PIN_INDX]);


}
