/*
 * lcd_ST7565S.cpp
 *
 *  Created on: 04.02.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <lcd_ST7565S.h>
#include <lcd_ST7565S_cmd.h>

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
	lcd_hnd->tsk_read_locked(NULL, 0);

	// A0=0 (cmds)
	PIO_CfgOutput0(pins[A0_PIN_INDX]);

	//command
	lcd_hnd->tsk_write_locked(&cmd, 1);

	//release A0
	PIO_Cfg(pins[A0_PIN_INDX]);

	//unlock
	lcd_hnd->tsk_read(NULL, 0);
}

void ST7565S::lcd_init(GUI_CB splash)

{
	LCD_MODULE::lcd_init(splash);


    //Splash screen..
    if(splash)
    {
    	lcd_single_window(splash);
    }

}

void ST7565S::lcd_reset()
{
	lcd_command(CMD_RESET);

	// LCD bias select
	lcd_command(CMD_BIAS2);

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

void ST7565S::draw_bitmap(unsigned int x0, unsigned int y0,
		const unsigned char* src, unsigned int width, unsigned int rows)
{
	unsigned int offset=0;
	if(y0 < frame_y0)
	{
		offset = frame_y0 - y0;
		if(offset >= rows)
			return;
		y0 += offset;
		rows -= offset;
		offset *= width;
		src += offset /8;
		offset %= 8;
	}
	if(y0 < frame_y1)
	{
		rows += y0;
		if(rows > frame_y1)
			rows = frame_y1;
		rows -= y0;

		width += x0;
		offset = 1<< offset;
		y0 = 1 << (y0&7);
		while(rows--)
		{
			for(unsigned int i=x0; i<width; i++)
			{
				if(src[0] & offset)
					disp_buf[i] |= y0;
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


void ST7565S::draw_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if( (y>=frame_y0) && (y<frame_y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1)
		{
			disp_buf[x0++] |= y;
		}
	}
}

void ST7565S::draw_bline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if( (y>=frame_y0) && (y<frame_y1))
	{
		y = ~(1 << (y&7));
		while(x0 <= x1)
		{
			disp_buf[x0++] &= y;
		}
	}
}

void ST7565S::invert_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if( (y>=frame_y0) && (y<frame_y1))
	{
		y = 1 << (y&7);
		while(x0 <= x1)
		{
			disp_buf[x0++] ^= y;
		}
	}
}

void ST7565S::draw_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if( (y0<frame_y1) && (y1>=frame_y0) )
	{
		if(y0 <frame_y0)
		{
			y0 = 0xFF;
		} else
		{
			y0 = 0xFF << (y0&7);
		}
		if(y1 <frame_y1)
		{
			y0 &= 0xFF >> (frame_y1 - y1 -1);
		}
		disp_buf[x] |= y0;
	}
}

void ST7565S::invert_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if( (y0<frame_y1) && (y1>=frame_y0) )
	{
		if(y0 <frame_y0)
		{
			y0 = 0xFF;
		} else
		{
			y0 = 0xFF << (y0&7);
		}
		if(y1 <frame_y1)
		{
			y0 &= 0xFF >> (frame_y1 - y1 -1);
		}
		disp_buf[x] ^= y0;
	}
}

void ST7565S::update_screen()
{
	unsigned int cmd;

	cmd = CMD_PAGE_ADR(frame_y0 /8) +	(CMD_COLUMN_ADR_LO(4) << 8 )+
			(CMD_COLUMN_ADR_HI(0) << 16) + (CMD_READ_WRITE_START << 24);

	//lock
	lcd_hnd->tsk_read_locked(NULL, 0);

	// A0=0
   	PIO_CfgOutput0(pins[A0_PIN_INDX]);

   	//command
	lcd_hnd->tsk_write_locked(&cmd, 4);

	// A0=1
   	PIO_SetOutput(pins[A0_PIN_INDX]);

   	// data
	lcd_hnd->tsk_write_locked(disp_buf, size_x);

	//release A0
   	PIO_Cfg(pins[A0_PIN_INDX]);

	//unlock
	lcd_hnd->tsk_read(NULL, 0);

}


void ST7565S::clear_screen()
{
	memclr(disp_buf, sizeof(disp_buf));
}


void ST7565S::redraw_screen(WINDOW desktop)
{
    WINDOW win;
    WINDOW top;
	bool update=false;

	if( (unsigned)(CURRENT_TIME-reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
	}

    for(frame_y0=0; frame_y0<size_y; frame_y0+=8)
    {
    	frame_y1 = frame_y0+8;
    	clear_screen();
    	win = desktop;
    	while(win)
    	{
    		update = true;
    		top = (WINDOW)win->next;
			#if GUI_DISPLAYS > 1
			while(top && !(display & top->displays))
				top = (WINDOW) top->next;

			if(display & desktop->displays)
			#endif
			{
				if( (!top) || (win->rect.as_int != top->rect.as_int) )
				{
					set_font(&FNT7x9);
					color = PIX_WHITE;
					if( win->callback((unsigned int)this, WM_DRAW))
					{
						update = false;
					}
				}
			}

    		win = top;
    	}
    	if(update)
    		update_screen();
    }
}
