/*
 * lcd_ST7565S_RS.cpp
 *
 *  Created on: 03.09.2013
 *      Author: stanly
 */


#include <tmos.h>
#include <lcd_ST7565S_RS.h>
#include <lcd_ST7565S_cmd.h>

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

#pragma GCC optimize ("Os")
__attribute__((always_inline)) char ST7565S_RS::revert_char (char ch)
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
//		y0 = 1 << (y0&7);
		while(rows--)
		{
			for( int i=x0; i<width; i++)
			{
				if((src[0] & offset) && frame.x0 <= i && i <= frame.x1)
				{
//					disp_buf[frame.y0>>3][i] |= revert_char(y0);
					disp_buf[y0>>3][i] |= revert_char( 1<<(y0&7) );
				}
				offset <<= 1;
				if(offset > 255)
				{
					offset = 1;
					src++;
				}
			}
//			y0 <<= 1;
			y0++;
		}
	}
}

void ST7565S_RS::draw_char(int x0, unsigned int ch)
{
	if(ch > 0x1f && font)
	{
		ch -= 0x20;
		int y0 = pos_y;
		const char* src = font->font_data + ch * font->char_bytes;
		int width = font->width;
		int rows = font->height;
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
	//		y0 = 1 << (y0&7);
			while(rows--)
			{
				for( int i=x0; i<width; i++)
				{
					if((src[0] & offset) && frame.x0 <= i && i <= frame.x1)
					{
	//					disp_buf[frame.y0>>3][i] |= revert_char(y0);
						disp_buf[y0>>3][i] |= revert_char( 1<<(y0&7) );
					}
					offset <<= 1;
					if(offset > 255)
					{
						offset = 1;
						src++;
					}
				}
	//			y0 <<= 1;
				y0++;
			}
		}
	}
}

void ST7565S_RS::draw_point( int x, int y)
{
	if(frame.y0 <= y && y < frame.y1 && frame.x0 <= x && x <= frame.x1)
	{
		GUI_ASSERT(x <= 131 && (y>>3) <= 7);
		if(color == PIX_WHITE)
			disp_buf[y>>3][x] |= revert_char(1 << (y&7));
		else
			disp_buf[y>>3][x] &= ~revert_char(1 << (y&7));
	}
}

void ST7565S_RS::draw_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		char val =revert_char(1 << (y&7));
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (y>>3) > 7)
				{
					TRACELN1("Oooops! draw_hline");
				}
				if(color == PIX_WHITE)
					disp_buf[y>>3][x0] |= val;//revert_char(1 << (y&7));
				else
					disp_buf[y>>3][x0] &= ~val;//revert_char(1 << (y&7));
			}
			x0++;
		}
	}
}

void ST7565S_RS::draw_bline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		char val = revert_char(1 << (y&7));
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (y>>3) > 7)
				{
					TRACELN1("Oooops! draw_bline");
				}
				disp_buf[y>>3][x0] &= ~val;//revert_char(1 << (y&7));
			}
			x0++;
		}
	}
}

void ST7565S_RS::invert_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		char val = revert_char(1 << (y&7));
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
			{
				if(x0 > 131 || (y>>3) > 7)
				{
					TRACELN1("Oooops! invert_hline");
				}
				disp_buf[y>>3][x0] ^= val;//revert_char(1 << (y&7));
			}
			x0++;
		}
	}
}

void ST7565S_RS::draw_vline( int y0, int y1, int x)
{
/*
	// only valid for drawing line by line
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
*/
	if(y0 <= frame.y0)
		y0 = frame.y0;
	while(y0 <= y1 && y0 < frame.y1)
	{
		if (frame.x0 <= x && x <= frame.x1 && y0>=frame.y0)
		{
			if(x > 131 || (y0>>3) > 7)
			{
				TRACELN1("Oooops! draw_vline");
			}
			disp_buf[y0>>3][x] |= revert_char(1 << (y0&7));
		}else
			break;
		y0++;
	}
}

void ST7565S_RS::invert_vline( int y0, int y1, int x)
{
/*
	// only valid for drawing line by line
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
*/
	if(y0 <= frame.y0)
		y0 = frame.y0;
	while(y0 <= y1 && y0 < frame.y1)
	{
		if (frame.x0 <= x && x <= frame.x1 && y0>=frame.y0)
		{
			if(x > 131 || (y0>>3) > 7)
			{
				TRACELN1("Oooops! invert_vline");
			}
			disp_buf[y0>>3][x] ^= revert_char(1 << (y0&7));
		}else
			break;
		y0++;
	}
}

#pragma GCC reset_options

void ST7565S_RS::update_screen()
{
	unsigned char cmd[4];
	unsigned int* cmd_ptr;

	cmd[0] = CMD_PAGE_ADR(frame.y0 /8);
	cmd[1] = CMD_COLUMN_ADR_LO( (ST7565S_GET_START_COLUMN(lcd_mode) + frame.x0) & 0x0F);
	cmd[2] = CMD_COLUMN_ADR_HI( ((ST7565S_GET_START_COLUMN(lcd_mode) + frame.x0) & 0xF0) >> 4);
	cmd[3] = CMD_READ_WRITE_START;
	cmd_ptr = (unsigned int *)((void *)cmd);
	*cmd_ptr = __REV(__RBIT(*cmd_ptr));

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
