/*
 * SPFD5414D.cpp
 *
 *  Created on: 23.01.2011
 *      Author: Miroslav Kostadinow
 */

#include <tmos.h>
#include <lcd_SPFD5414D.h>
#include <fam_cpp.h>


#define LCD_DEF_FONT		&FNT7x9
#define LCD_DEF_COL			PIX_WHITE


const unsigned short tft_init[] =
{
	SPFD5414D_SLPOUT,

	// ColorModeSet 16bpp
	SPFD5414D_COLMOD, SPFD5414D_DATA(SPFD5414D_COLMOD_16BPP ),

	//_MemoryAccess BGR
	SPFD5414D_MADCTR, SPFD5414D_DATA(SPFD5414D_MADCTR_MY | SPFD5414D_MADCTR_MX | SPFD5414D_MADCTR_ML | SPFD5414D_MADCTR_BGR),

	//  Display on
	SPFD5414D_NORON,
	SPFD5414D_DISPON,


//	  SPFD5414D_ColumnAddressSet( x0, x0+dx-1 );
	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),

//	  SPFD5414D_RowAddressSet( y0, y0+dy-1 );
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),


};


const unsigned short tft_init_address[] =
{
//	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0),
	SPFD5414D_RAMWR
};




void SPFD5414D::lcd_reset()
{
	lcd_hnd->tsk_write(tft_init, sizeof(tft_init)/2);
}

void SPFD5414D::redraw_screen (GObject* object, RECT_T area)
{
	redraw_rect(object, area);
}

void SPFD5414D::draw_bitmap( int x0, int y0, const char* src, int width, int rows)
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

		offset = 1<< offset;
		while(width--)
		{
			if(src[0] & offset)
				if (frame.x0 <= x0 && x0 <= frame.x1)
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

void SPFD5414D::draw_point( int x, int y)
{
	if(frame.y0 == y && frame.x0 <= x && x <= frame.x1)
	{
		disp_buf[x] |= color;
	}
}

void SPFD5414D::draw_hline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
				disp_buf[x0] = color;
			x0++;
		}
	}
}

void SPFD5414D::draw_bline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
				disp_buf[x0] = PIX_BLACK;
			x0++;
		}
	}
}

void SPFD5414D::draw_vline( int y0, int y1, int x)
{
	if((y0 <= frame.y0) && (y1 >= frame.y0))
	{
		if (frame.x0 <= x && x <= frame.x1)
			disp_buf[x] = color;
	}
}

void SPFD5414D::invert_vline( int y0, int y1, int x)
{
	if((y0 <= frame.y0) && (y1 >= frame.y0))
	{
		if (frame.x0 <= x && x <= frame.x1)
			disp_buf[x] ^= (PIX_WHITE ^ PIX);
	}
}

void SPFD5414D::invert_hline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		while(x0 <= x1)
		{
			if (frame.x0 <= x0 && x0 <= frame.x1)
				disp_buf[x0] ^= (PIX_WHITE ^ PIX);
			x0++;
		}
	}
}

void SPFD5414D::clear_screen()
{
	for(int i=0; i<128; i++)
		disp_buf[i] = PIX_BLACK;
}

void SPFD5414D::adjust_for_screen (GObject** object, RECT_T& area)				//change the area and object to desktop if the display draws only rows or columns
{
																				//          *** example for display that draws rows ***
																				//area.x0 = 0;
																				//area.x1 = size_x - 1;
																				//*object = children;
}

//
//The TFT modules hardware interface methods
void SPFD5414D::tft_write_row(unsigned short row_address_cmd[], unsigned short row)
{
	if(row == -1) // initialize row_address buffer
	{
		memcpy(row_address_cmd, tft_init_address, sizeof(tft_init_address));
	}
	else
	{
		row_address_cmd[2] = row_address_cmd[4] = row;
		lcd_hnd->tsk_write(row_address_cmd, sizeof(tft_init_address)/2);
		lcd_hnd->tsk_write(disp_buf, 256);
		if(disp_buf == video_buf)
			disp_buf += 128;
		else
			disp_buf = video_buf;
	}
}


void SPFD5414D::redraw_rect (GObject* object, RECT_T area)						//redraws an area of the object on the LCD by calling its draw (LCD_MODULE, RECT_T) function
{
	unsigned short cmd_address[sizeof(tft_init_address)/sizeof(unsigned short)];
	unsigned short row=0;

	tft_write_row(cmd_address, -1);

	if( (unsigned)(CURRENT_TIME-reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
	}
	if(object && !(object->flags & GO_FLG_TRANSPARENT))
		clear_rect(area);
	frame.x0 = area.x0;
	frame.x1 = area.x1;
	disp_buf = video_buf;
    for(frame.y0=area.y0; frame.y0 <= area.y1; frame.y0++)
    {
    	frame.y1 = frame.y0+1;
		if (object)
			object->draw_this(this);

		tft_write_row(cmd_address, row++);
    }
}

void SPFD5414D::direct_write(GSplash draw_cb)
{
	unsigned short cmd_address[sizeof(tft_init_address)/sizeof(unsigned short)];
	unsigned short row=0;

	tft_write_row(cmd_address, -1);

	frame.x0 = 0;
	frame.x1 = size_x-1;
	disp_buf = video_buf;
    for(frame.y0=0; frame.y0 <= size_y-1; frame.y0++)
    {
    	frame.y1 = frame.y0+1;
    	clear_screen();
   		draw_cb(this);

		tft_write_row(cmd_address, row++);
    }
}


void TFT_CHECK::delay(unsigned int time)
{
	if(time)
	{
		tsk_sleep(time);
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
	PIO_SetOutput(pins[SCL_PIN_INDX]);
	delay();
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
	return (res);
}

unsigned int TFT_CHECK::read_id()
{

    PIO_CfgOutput0(pins[RST_PIN_INDX]);
	delay(5);
	PIO_SetOutput(pins[RST_PIN_INDX]);
	delay(150);

	PIO_CfgOutput0(pins[CSX_PIN_INDX]);
	delay(5);
	tft_write(SPFD5414D_SWRESET);
	delay(5);
	PIO_SetOutput(pins[CSX_PIN_INDX]);
	delay(150);

    PIO_CfgOutput0(pins[SCL_PIN_INDX]);

	PIO_CfgOutput0(pins[CSX_PIN_INDX]);
	tft_write(SPFD5414D_SLPOUT);
	PIO_SetOutput(pins[CSX_PIN_INDX]);
	delay(150);
	return id();
}

unsigned int TFT_CHECK::id()
{
	unsigned int res =0;

	PIO_CfgOutput0(pins[SCL_PIN_INDX]);
	PIO_CfgOutput0(pins[CSX_PIN_INDX]);
	tft_write(SPFD5414D_RDDID);
	res = tft_read();	//2A
	res <<= 8;
	res |= tft_read(); //2A40
	res <<= 8;
	res |= tft_read(); //2A4033
	PIO_SetOutput(pins[CSX_PIN_INDX]);
	return res;
}


