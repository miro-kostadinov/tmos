/*
 * lcd_ST7565S.cpp
 *
 *  Created on: 04.02.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <lcd_ST7565S.h>

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


//LCD commands A0 = 0
#define CMD_DISPLAY_ON			0xAF		//!< Display On
#define CMD_DISPLAY_OFF			0xAE		//!< Display Off
#define CMD_START_LINE(x)		(0x40 +(x))	//!< Display Start Line Set(0..63)
#define CMD_PAGE_ADR(x)			(0xB0 +(x))	//!< Page Address Set(0..8)
#define CMD_COLUMN_ADR_HI(x)	(0x10 +(x))	//!< Column Address Set(0..131) 4 MSB
#define CMD_COLUMN_ADR_LO(x)	(0x00 +(x))	//!< Column Address Set(0..131) 4 LSB
#define CMD_ADC_NORMAL			0xA0		//!< Segment Driver Direction Select
#define CMD_ADC_REVERSE			0xA1		//!< Segment Driver Direction Select (column)
#define CMD_DISPLAY_NORMAL		0xA6		//!< Display Normal
#define CMD_DISPLAY_REVERSE		0xA7		//!< Display Inverse
#define CMD_DISPLAY_ALL_ON		0xA4		//!< Display All points=ON
#define CMD_DISPLAY_ALL_OFF		0xA5		//!< Display All points=OFF
#define CMD_BIAS1				0xA2		//!< LCD Bias Set (1/9 1/8 ..)
#define CMD_BIAS2				0xA3		//!< LCD Bias Set (1/7 1/6 ..)
#define CMD_RESET				0xE2		//!< Reset
#define CMD_COMMON_NORMAL		0xC0		//!< Common Output mode
#define CMD_COMMON_REVERSE		0xC8		//!< Common Output mode Inverse (lines)

#define POWER_VOLTAGE_FOLLOWER 	1
#define POWER_VOLTAGE_REGULATOR	2
#define POWER_BOOSTER			4
#define POWER_ALL				7
#define CMD_POWER(x)			(0x28 +(x))	//!< Page Address Set(0..8)

#define CMD_V5_REGULATOR(x)		(0x20 +(x))	//!< V5 voltage regulator internal resistor ratio(0..7)

#define CMD_ELECTRONIC_VOLUME	0x81		//!< The Electronic Volume (Double Byte Command) (0..3F)
#define CMD_INDICATOR_ON		0xAD		//!< Static Indicator (Double Byte Command) (0-3) OFF, 1s, 0.5s, on
#define CMD_INDICATOR_OFF		0xAC		//!< Static Indicator OFF

#define CMD_READ_WRITE_START	0xE0		//!< Read/Modify loop start
#define CMD_END					0xEE		//!< Read/Modify loop END

#define CMD_BOOSTER_RATIO		0xF8		//!< The Booster Ratio (Double Byte Command) (0-3)
#define CMD_NOP					0xE3		//!< NOP
#define CMD_TEST				0xFC		//!< Don't use

#define CMD_STATUS_READ						//!< Reads status

//LCD data A0 = 1
#define DATA_WRITE							//!< Write data when A0=1

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
	lcd_command(CMD_ADC_REVERSE);
	// SHL select
	lcd_command(CMD_COMMON_NORMAL);
	// Initial display line
	lcd_command(CMD_START_LINE(0));

	// turn on voltage converter (VC=1, VR=0, VF=0)
	lcd_command(CMD_POWER( 0x4));
	// wait for 50% rising
	tsk_sleep(50);

	// turn on voltage regulator (VC=1, VR=1, VF=0)
	lcd_command(CMD_POWER(0x6));
	// wait >=50ms
	tsk_sleep(50);

	// turn on voltage follower (VC=1, VR=1, VF=1)
	lcd_command(CMD_POWER(0x7));
	// wait
	tsk_sleep(10);

	// set lcd operating voltage (regulator resistor, ref voltage resistor)
	lcd_command(CMD_V5_REGULATOR(0x2));

	lcd_command(CMD_ELECTRONIC_VOLUME);
	lcd_command(0x12);

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

	cmd = CMD_PAGE_ADR(pagemap[frame_y0 /8]) +	(CMD_COLUMN_ADR_LO(1) << 8 )+
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
					win->callback((unsigned int)this, WM_DRAW);
				}
			}

    		win = top;
    	}

    	update_screen();
    }
}
