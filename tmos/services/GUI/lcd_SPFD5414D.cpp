/*
 * SPFD5414D.cpp
 *
 *  Created on: 23.01.2011
 *      Author: Miroslav Kostadinow
 */

#include <tmos.h>
#include <lcd_SPFD5414D.h>


#define TFT_NOP 			0
#define TFT_DISPLAYOFF 		0x28
#define TFT_DISPON 			0x29	//!< [0] Display Om
#define TFT_SLPOUT 			0x11	//!< [0] Sleep Out
#define TFT_NORON			0x13	//!< [0] Normal Display On

#define TFT_PWCTR1			0xC0	//!< [2] Power Control 1 (VRH, VCI)

#define TFT_SWRESET 		0x01
#define TFT_RDDPM 			0x0a
#define TFT_RDDMADCTR 		0xb
#define TFT_RDDCOLMOD 		0xc
#define TFT_RDDCIM 			0xd
#define TFT_RDID4 			0xd3
#define TFT_CASET 			0x2a	//!< [4] Column Address Set (XS, XE)
#define TFT_RASET 			0x2b	//!< [4] Row Address Set (YS, YE)
#define TFT_RAMWR 			0x2c	//!< [N] Memory Write
#define TFT_RAMRD 			0x2e
#define TFT_RDDSDR 			0x0f
#define TFT_RDDID 			0x04
#define TFT_RDCOLMD 		0x0c
#define TFT_RDDST 			0x09
#define TFT_COLMOD 			0x3a	//!< [1] Interface Pixel Format (color mode)
#define TFT_MADCTR 			0x36	//!< [1] Memory Data Access Control
#define TFT_RGBSET 			0x2d

#define TFT_DATA(data)		(0x0100 |(data))
#define TFT_CMD(cmd)		cmd

#define TFT_COLMOD_12BPP	0x3
#define TFT_COLMOD_16BPP	0x5
#define TFT_COLMOD_18BPP	0x6

#define TFT_MADCTR_MY		0x80	//!< Row address order
#define TFT_MADCTR_MX		0x40	//!< Column address order
#define TFT_MADCTR_MV		0x20	//!< Row/Column exchange
#define TFT_MADCTR_ML		0x10	//!< Vertical refresh order
#define TFT_MADCTR_BGR		0x08	//!< RGB-BGR order (1=BGR)


const unsigned short tft_init[] =
{
	TFT_SLPOUT,

	// ColorModeSet 16bpp
	TFT_COLMOD, TFT_DATA(TFT_COLMOD_16BPP ),

	//_MemoryAccess BGR
	TFT_MADCTR, TFT_DATA(TFT_MADCTR_MY | TFT_MADCTR_MX | TFT_MADCTR_ML | TFT_MADCTR_BGR),

	//  Display on
	TFT_NORON,
	TFT_DISPON,


//	  TFT_ColumnAddressSet( x0, x0+dx-1 );
	TFT_CASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),

//	  TFT_RowAddressSet( y0, y0+dy-1 );
	TFT_RASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),


};


const unsigned short tft_init_3[] =
{
	TFT_CASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),
	TFT_RASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),
};



// InitLcd is called to initialize the hardware
void SPFD5414D::lcd_init(GUI_CB splash)
{
	LCD_MODULE::lcd_init(splash);

    //Splash screen..
    if(splash)
    {
    	lcd_single_window(splash);
    	tsk_sleep(3000);
    }
}

void SPFD5414D::lcd_reset()
{
	lcd_hnd->tsk_write(tft_init, sizeof(tft_init)/2);
}

void SPFD5414D::draw_bitmap(unsigned int x0, unsigned int y0,
		const unsigned char* src, unsigned int width, unsigned int rows)
{
	unsigned int offset=0;
//	RenderGDIBitmap(disp_buf + (y0>>3) + (x0 * 11), 1<<(y0&7), src, width, rows);
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

		offset = 1<< offset;
		while(width--)
		{
			if(src[0] & offset)
				disp_buf[x0] = color;
			offset <<= 1;
			if(offset > 255)
			{
				offset = 1;
				src++;
			}
			x0++;
		}
	}
}


void SPFD5414D::draw_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if(y==frame_y0)
	{
		while(x0 <= x1)
		{
			disp_buf[x0++] = color;
		}
	}
}

void SPFD5414D::draw_bline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if(y==frame_y0)
	{
		while(x0 <= x1)
		{
			disp_buf[x0++] = PIX_BLACK;
		}
	}
}

void SPFD5414D::draw_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if((y0 <= frame_y0) && (y1 >= frame_y0))
	{
		disp_buf[x] = color;
	}
}

void SPFD5414D::invert_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if((y0 <= frame_y0) && (y1 >= frame_y0))
	{
		disp_buf[x] ^= (PIX_WHITE ^ PIX);
	}
}

void SPFD5414D::invert_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if(y==frame_y0)
	{
		while(x0 <= x1)
		{
			disp_buf[x0++] ^= (PIX_WHITE ^ PIX);
		}
	}
}

void SPFD5414D::update_screen()
{
	lcd_hnd->tsk_write(disp_buf, 256);
	if(disp_buf == video_buf)
		disp_buf += 128;
	else
		disp_buf = video_buf;
}

void SPFD5414D::clear_screen()
{
	for(int i=0; i<128; i++)
		disp_buf[i] = PIX_BLACK;
}

extern unsigned int cpu_usage;

void SPFD5414D::redraw_screen(WINDOW desktop)
{
    WINDOW win;
    WINDOW top;
	unsigned int tft_cmd = TFT_RAMWR;

	if( (unsigned)(CURRENT_TIME-reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
	}
	lcd_hnd->tsk_write(&tft_cmd, 1);
	disp_buf = video_buf;
    for(frame_y0=0; frame_y0<size_y-1; frame_y0++)
    {
    	frame_y1 = frame_y0+1;
    	clear_screen();
    	win = desktop;
    	do
    	{
    		top = win->prev;
    		if( (top == desktop) || (win->rect.as_int != top->rect.as_int) )
    		{
    			set_font(&FNT7x9);
    			color = PIX_WHITE;
    			win->callback((unsigned int)this, WM_DRAW);
    		}

    		win = top;
    	} while(win != desktop);

    	update_screen();
    }
	color = PIX_LIGHTGRAY;
	draw_hline(0, (cpu_usage*127)/100, 127);
	update_screen();

}


void TFT_CHECK::delay()
{
	for(int i=0; i<200; i++)
	{
		asm volatile ("nop");
	}
}

void TFT_CHECK::tft_write( unsigned int value)
{
	//SET SDA to Port
	PIO_CfgOutput0(pins[SDA_PIN_INDX]);

	//9 data bits
	for (int i = 0; i < 9; i++)
	{
		PIO_ClrOutput(pins[SCL_PIN_INDX]);
		delay();

		//    TFT_SDA = nByte & Mask8[i] ? 1 : 0;//51 msec
		if(value & 0x100)
		{
			PIO_SetOutput(pins[SDA_PIN_INDX]);
		} else
		{
			PIO_ClrOutput(pins[SDA_PIN_INDX]);
		}
		value <<= 1;
		delay();

		PIO_SetOutput(pins[SCL_PIN_INDX]);
		delay();
	}
	PIO_ClrOutput(pins[SCL_PIN_INDX]);
	delay();
}

unsigned int TFT_CHECK::tft_read( )
{
	unsigned int res = 0;

	PIO_CfgInput(pins[SDA_PIN_INDX]);

	//8 data bits
	for (int i = 0; i < 8; i++)
	{
		PIO_ClrOutput(pins[SCL_PIN_INDX]);
		delay();


		res <<= 1;
		if(PIO_Read(pins[SDA_PIN_INDX]))
			res ++;

		PIO_SetOutput(pins[SCL_PIN_INDX]);
		delay();
	}
	PIO_ClrOutput(pins[SCL_PIN_INDX]);
	delay();
	return res;
}

unsigned int TFT_CHECK::read_id()
{
	unsigned int res =0;

    PIO_CfgOutput0(pins[RST_PIN_INDX]);
	delay();
	PIO_SetOutput(pins[RST_PIN_INDX]);
	for(res=0; res < 1000; res++)
		delay();
    PIO_CfgOutput0(pins[SCL_PIN_INDX]);
	PIO_CfgOutput0(pins[CSX_PIN_INDX]);

	tft_write(TFT_SLPOUT);
	tft_write(TFT_RDDID);
	res = tft_read();	//2A
	res <<= 8;
	res |= tft_read(); //2A40
	res <<= 8;
	res |= tft_read(); //2A4033
	PIO_SetOutput(pins[CSX_PIN_INDX]);
	return res;
}



