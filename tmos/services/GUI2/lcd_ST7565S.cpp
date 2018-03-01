/*
 * lcd_ST7565S.cpp
 *
 *  Created on: 04.02.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <lcd_ST7565S.h>
#include <lcd_ST7565S_cmd.h>
#include <gwindow.h>

/* ST7565S info
 *
 * Internal RAM 65x132 = 8580 bits
 * 65 lines
 * 132 columns
 * 1 line = 8 pages (8 bits each) + 1
 *
 * Column address is auto incremented up to 131(0x83)
 *
 */



#define A0_PIN_INDX	2


void ST7565S::lcd_command(unsigned int cmd)
{
	//lock
	lcd_hnd->tsk_read_locked(nullptr, 0);

	// A0=0 (cmds)
	PIO_CfgOutput0(pins[A0_PIN_INDX]);

	//command
	lcd_hnd->tsk_write_locked(&cmd, 1);

	//release A0
	PIO_Cfg(pins[A0_PIN_INDX]);

	//unlock
	lcd_hnd->tsk_read(nullptr, 0);
}

void ST7565S::lcd_reset()
{
	lcd_command(CMD_RESET);

	// LCD bias select
	if(lcd_mode & ST7565S_BIAS2)
		lcd_command(CMD_BIAS2);
	else
		lcd_command(CMD_BIAS1);
	// ADC select
	if(lcd_mode & ST7565S_H_FLIP)
		lcd_command(CMD_ADC_REVERSE);
	else
		lcd_command(CMD_ADC_NORMAL);

	// SHL select
	if(lcd_mode & ST7565S_V_FLIP)
		lcd_command(CMD_COMMON_REVERSE);
	else
		lcd_command(CMD_COMMON_NORMAL);

	// Initial display line
	lcd_command(CMD_START_LINE(0));

	// turn on voltage converter (VC=1, VR=0, VF=0)
//	lcd_command(CMD_POWER( 0x4));
	// wait for 50% rising
//	tsk_sleep(50);

	// turn on voltage regulator (VC=1, VR=1, VF=0)
//	lcd_command(CMD_POWER(0x6));
	// wait >=50ms
//	tsk_sleep(50);

	// turn on voltage follower (VC=1, VR=1, VF=1)
	lcd_command(CMD_POWER(0x7));
	// wait
//	tsk_sleep(10);

	// set lcd operating voltage (regulator resistor, ref voltage resistor)
	lcd_command(CMD_V5_REGULATOR(lcd_mode & 0x7)); //ST7565S_RaRb_RATIO

	lcd_command(CMD_ELECTRONIC_VOLUME);
	lcd_command((lcd_mode >>8) & 0x3F);		//ST7565S_VOLUME

	lcd_command(CMD_DISPLAY_ON);
	lcd_command(CMD_DISPLAY_ALL_ON);

}

void ST7565S::draw_bitmap( int x0, int y0, const char* src, int width, int rows)
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
					disp_buf[frame.y0>>3][i] |= y0;
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

void ST7565S::draw_point( int x, int y)
{
	if(color == PIX_WHITE)
	{
		if(frame.y0 <= y && y < frame.y1 && frame.x0 <= x && x <= frame.x1)
		{
			disp_buf[frame.y0>>3][x] |= (1 << (y&7));
		}
	}
	else
	{
		if(frame.y0 <= y && y < frame.y1 && frame.x0 <= x && x <= frame.x1)
		{
			disp_buf[frame.y0>>3][x] |= (1 << (y&7));
		}
	}
}

void ST7565S::draw_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1 && frame.x0 <= x0 && x0 <= frame.x1)
		{
			disp_buf[frame.y0>>3][x0++] |= y;
		}
	}
}

void ST7565S::draw_bline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = ~(1 << (y&7));
		while(x0 <= x1 && frame.x0 <= x0 && x0 <= frame.x1)
		{
			disp_buf[frame.y0>>3][x0++] &= y;
		}
	}
}

void ST7565S::invert_hline( int x0, int x1, int y)
{
	if( (y>=frame.y0) && (y<frame.y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1 && frame.x0 <= x0 && x0 <= frame.x1)
		{
			disp_buf[frame.y0>>3][x0++] ^= y;
		}
	}
}

void ST7565S::draw_vline( int y0, int y1, int x)
{
	if( (y0<=frame.y0) && (y1>=frame.y0) && frame.x0 <= x && x <= frame.x1)
	{
		disp_buf[frame.y0/8][x] |= 1 << (frame.y0&7);
	}
}

void ST7565S::invert_vline( int y0, int y1, int x)
{
	if( (y0<=frame.y0) && (y1>=frame.y0) && frame.x0 <= x && x <= frame.x1)
	{
		disp_buf[frame.y0>>3][x] ^= 1 << (frame.y0&7);
	}
}

void ST7565S::redraw_screen (GObject* object, RECT_T area)
{
	if(area.normalize(rect))
	{
		frame = area;
#if GUI_DEBUG
		uint32_t t = CURRENT_TIME;
		GUI_TRACELN("LCD%u update {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
#endif
		for (frame.y0=(area.y0 - (area.y0&7)); frame.y0 <= area.y1; frame.y0 += 8)
		{
			update_screen();
		}
#if GUI_DEBUG
	    GUI_TRACE(" %u ms", ms_since(t));
#endif
	}
}

void ST7565S::update_screen()
{
	unsigned int cmd;

	cmd = CMD_PAGE_ADR(frame.y0 /8) +	(CMD_COLUMN_ADR_LO(((lcd_mode>>16)&0xF) + (frame.x0 & 0x0F)) << 8 )+
			(CMD_COLUMN_ADR_HI(((lcd_mode>>20)&0xF) + (frame.x0 & 0xF0)) << 16) + (CMD_READ_WRITE_START << 24);

	//lock
	lcd_hnd->tsk_read_locked(nullptr, 0);

	// A0=0
   	PIO_CfgOutput0(pins[A0_PIN_INDX]);

   	//command
	lcd_hnd->tsk_write_locked(&cmd, 4);

	// A0=1
   	PIO_SetOutput(pins[A0_PIN_INDX]);

   	// data
	lcd_hnd->tsk_write_locked(disp_buf[frame.y0/8], 1 + frame.x1 - frame.x0);//size_x);

	//release A0
   	PIO_Cfg(pins[A0_PIN_INDX]);

	//unlock
	lcd_hnd->tsk_read(nullptr, 0);

}


void ST7565S::clear_screen()
{
	memclr(disp_buf, sizeof(disp_buf));
}

void ST7565S::adjust_for_screen (GObject** object, RECT_T& area)				//change the area and object to desktop if the display draws only rows or columns
{
	//area.x0 = 0;																//          *** example for display that draws rows ***
	//area.x1 = size_x - 1;														//area.x0 = 0;
	//*object = children;															//area.x1 = size_x - 1;
																				//*object = children;
}


void ST7565S::redraw_rect (GObject* object, RECT_T area)						//redraws an area of the object on the LCD by calling its draw (LCD_MODULE, RECT_T) function
{
#if GUI_DEBUG
	unsigned int t0 = CURRENT_TIME;
	GUI_TRACELN1("\e[4;1;31m");
#endif
	if( (unsigned)(CURRENT_TIME-reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
		GUI_TRACE("RST %u ms\r\n", ms_since(t0));
	}
	GUI_TRACE("%s[%d]", szlist_at(obj_type_str,object->get_object_type()), (object)?object->id:-1);
	//1. Try to get the bottom window that contain a redrawing object
	GObject* initial;
	if(object)
	{
		initial = children;
		GObject* owner = object->parent;
		// check that is window
		if(owner && owner->parent != this)
		{	// It is a common object, try to find its owner window
			while(owner)
			{
				if(owner->parent && owner->parent->parent == this)
					break;
				owner = owner->parent;
			}
		}
		else
		{
			// It is a window
			owner = object;
		}

		if(owner)
		{
			while(initial)
			{
				if(initial == owner)
					break;
				initial = initial->nextObj;
			}
		}
		if(!owner || !initial )
		{
			TRACE_ERROR("GUI Z-order exception!");
		}
	// 2. draw
		frame.x0 = area.x0;
		frame.x1 = area.x1;
	    for(frame.y0=area.y0; frame.y0 <= area.y1; frame.y0++)
	    {
	    	frame.y1 = frame.y0+1;
    		GObject* tmp = initial->nextObj;
    		int res =0;
    		while(1)
    		{
				while(tmp)
				{
#if GUI_DISPLAYS > 1
					if(((GWindow*)tmp)->displays & 	display	)
#endif
						res |= object->overlapped(tmp, frame);
					tmp = tmp->nextObj;
				}

				if(!(object->flags & GO_FLG_TRANSPARENT))
					clear_rect(frame);
				object->draw_this(this);

				if(res)
				{
					GUI_TRACELN("LCD%u draw {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
					frame.x0 = frame.x1+1;
					frame.x1 = area.x1;
					tmp = initial->nextObj;
					res =0;
					if(frame.x0 <= frame.x1)
					{
						continue;
					}
					else
					{
						frame.x0 = area.x0;
						frame.x1 = area.x1;
					}
				}
				break;
    		}
	    }
	}
//	uint32_t t = CURRENT_TIME;
//    for (frame.y0=(area.y0 - (area.y0&7)); frame.y0 <= area.y1; frame.y0 += 8)
//    {
//    	update_screen();
//    }
//    TRACE(" %u ms", ms_since(t));
#if GUI_DEBUG
	if(frame.x0 <= frame.x1)
		GUI_TRACELN("LCD%u draw {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
    GUI_TRACE(" %d ms\e[m", ms_since(t0));
#endif
}

void ST7565S::direct_write(GSplash draw_cb)
{
	clear_screen();
	frame.x0 = 0;
	frame.x1 = size_x-1;
    for(frame.y0=0; frame.y0 < size_y; frame.y0++)
    {
    	frame.y1 = frame.y0+1;
    	draw_cb(this);
    }
    for (frame.y0=0; frame.y0 < size_y; frame.y0 += 8)
    {
    	update_screen();
    }
}

