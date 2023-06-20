/*
 * SPFD5414D.cpp
 *
 *  Created on: 23.01.2011
 *      Author: Miroslav Kostadinow
 */

#include <tmos.h>
#include <lcd_SPFD5414D.h>
#include <fam_cpp.h>
#include <gwindow.h>


#define LCD_DEF_FONT		&FNT7x9
#define LCD_DEF_COL			PIX_WHITE

/*
enum tft_color_lut:unsigned char
{
	lut_black =0,
	lut_blue,
	lut_green,
	lut_cyan,
	lut_red,
	lut_magenta,
	lut_brown,
	lut_lightgray,
	lut_darkgray,
	lut_lightblue,
	lut_lightgreen,
	lut_lightcyan,
	lut_lightred,
	lut_lightmagenta,
	lut_yellow,
	lut_white
};
*/

const unsigned int SPFD5414D::lut_to_tft_color[16] =
{
	PIX_RGB_BLACK,
	PIX_RGB_BLUE,
	PIX_RGB_GREEN,
	PIX_RGB_RED,
	PIX_RGB_LIGHTMAGENTA,
	PIX_RGB_BROWN,
	PIX_RGB_LIGHTBLUE,
	PIX_RGB_LIGHTGRAY,
	PIX_RGB_DARKGRAY,
	PIX_RGB_LIGHTGREEN,
	PIX_RGB_LIGHTCYAN,
	PIX_RGB_LIGHTRED,
	PIX_RGB_CYAN,
	PIX_RGB_MAGENTA,
	PIX_RGB_YELLOW,
	PIX_RGB_WHITE
};

const unsigned short tft_init[] =
{
	SPFD5414D_SLPOUT,
	// ColorModeSet 16bpp
	SPFD5414D_COLMOD, SPFD5414D_DATA(SPFD5414D_COLMOD_16BPP ),
	// MemoryAccess BGR
	SPFD5414D_MADCTR, SPFD5414D_DATA(SPFD5414D_MADCTR_MY | SPFD5414D_MADCTR_MX | SPFD5414D_MADCTR_ML | SPFD5414D_MADCTR_BGR),
	// Display on
	SPFD5414D_NORON,
	SPFD5414D_DISPON,
	// ColumnAddressSet( x0, x0+dx-1 );
	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
	// RowAddressSet( y0, y0+dy-1 );
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
};

const unsigned short tft_init_address[] =
{
	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0),
	SPFD5414D_RAMWR
};

void SPFD5414D::lcd_reset()
{
	lcd_hnd->tsk_write(tft_init, sizeof(tft_init)/2);
}

void SPFD5414D::do_reset()
{
	if( ms_since(reset_timeout) > 500 )
	{
		reset_timeout = CURRENT_TIME;
		lcd_reset();
	}
}

void SPFD5414D::clear_screen()
{
	memclr(disp_buf, sizeof(disp_buf));
}

/*
 * tft_encode_color row by row
void SPFD5414D::tft_encode_color(const int x_pos, bool invert)
{
	if (frame.x0 <= x_pos && x_pos <= frame.x1)
	{
		unsigned char pix = disp_buf[frame.y0][x_pos>>1];
		unsigned char pix_color;
		if(invert)
		{
			if(x_pos & 1)
			{
				pix_color = lut_white - (pix & 0x0F);
				pix = (pix & 0xF0) | pix_color;
			}
			else
			{
				pix_color = (lut_white<<4) - (pix & 0xF0);
				pix = (pix & 0x0F) | pix_color;
			}
		}
		else
		{
			switch(color)
			{
			case PIX_BLACK:
				pix_color = lut_black; break;
#if !GUI_MONOCHROME
			case PIX_BLUE:
				pix_color = lut_blue; break;
			case PIX_GREEN:
				pix_color = lut_green; break;
			case PIX_CYAN:
				pix_color = lut_cyan; break;
			case PIX_RED:
				pix_color = lut_red; break;
			case PIX_MAGENTA:
				pix_color = lut_magenta; break;
			case PIX_BROWN:
				pix_color = lut_brown; break;
			case PIX_LIGHTGRAY:
				pix_color = lut_lightgray; break;
			case PIX_DARKGRAY:
				pix_color = lut_darkgray; break;
			case PIX_LIGHTBLUE:
				pix_color = lut_lightblue; break;
			case PIX_LIGHTGREEN:
				pix_color = lut_lightgreen; break;
			case PIX_LIGHTCYAN:
				pix_color = lut_lightcyan; break;
			case PIX_LIGHTRED:
				pix_color = lut_lightred; break;
			case PIX_LIGHTMAGENTA:
				pix_color = lut_lightmagenta; break;
			case PIX_YELLOW:
				pix_color = lut_yellow; break;
			case PIX_WHITE:
				pix_color = lut_white; break;
#endif
			default:
				pix_color = lut_white;
			}
			if(x_pos & 1)
				pix = (pix & 0xF0) | pix_color;
			else
				pix = (pix & 0x0F) | (pix_color << 4);
		}
		disp_buf[frame.y0][x_pos>>1] = pix; //PIX_WHITE;//_COLOR_ color;
	}
}
*/

#pragma GCC optimize ("Os")

#if PIX_FORMAT == PIX_4BIT_FORMAT
/**
 * function is a placeholder for compatibility. no need for conversion if we're
 * using the 4bit PIX format
 */
static inline __attribute__((always_inline)) tft_color_lut pix_to_4bit(unsigned int color_t)
{
	return tft_color_lut(color_t);
}
#else
/**
 * converts a PIX format color (which may be RGB565 or 4byte PIX, depends on the
 * PIX_FORMAT flag) into 4bit color, for use in the disp_buf. converts only the
 * pre-determined 16 colors, if another color is given it returns the default
 * value of white
 * @param color_t: PIX color or RGB565 color
 * @return: the same 4bit color, or white if unsupported
 */
static tft_color_lut pix_to_4bit(unsigned int color_t)
{
	switch (color_t)
	{
	case PIX_BLACK:
		return lut_black;
# if !GUI_MONOCHROME
	case PIX_BLUE:
		return lut_blue;
	case PIX_GREEN:
		return lut_green;
	case PIX_CYAN:
		return lut_cyan;
	case PIX_RED:
		return lut_red;
	case PIX_MAGENTA:
		return lut_magenta;
	case PIX_BROWN:
		return lut_brown;
	case PIX_LIGHTGRAY:
		return lut_lightgray;
	case PIX_DARKGRAY:
		return lut_darkgray;
	case PIX_LIGHTBLUE:
		return lut_lightblue;
	case PIX_LIGHTGREEN:
		return lut_lightgreen;
	case PIX_LIGHTCYAN:
		return lut_lightcyan;
	case PIX_LIGHTRED:
		return lut_lightred;
	case PIX_LIGHTMAGENTA:
		return lut_lightmagenta;
	case PIX_YELLOW:
		return lut_yellow;
# endif
	default:
		return lut_white;
	}
}
#endif

void SPFD5414D::tft_encode_color(const int x_pos, const int y_pos, unsigned int color_t)
{
	if (frame.x0 <= x_pos && x_pos <= frame.x1)
	{
		unsigned char pix = disp_buf[y_pos][x_pos >> 1];
		unsigned char pix_color;

		pix_color = pix_to_4bit(color_t);

		if (x_pos & 1)
			pix = (pix & 0xF0) | pix_color;
		else
			pix = (pix & 0x0F) | (pix_color << 4);

		disp_buf[y_pos][x_pos >> 1] = pix; //PIX_WHITE;//_COLOR_ color;
	}
}

void SPFD5414D::tft_invert_color(const int x_pos, const int y_pos)
{
	if (frame.x0 <= x_pos && x_pos <= frame.x1)
	{
		unsigned char pix = disp_buf[y_pos][x_pos >> 1];
		unsigned char pix_color;

		if (x_pos & 1)
		{
			pix_color = lut_white - (pix & 0x0F);
			pix = (pix & 0xF0) | pix_color;
		}
		else
		{
			pix_color = (lut_white << 4) - (pix & 0xF0);
			pix = (pix & 0x0F) | pix_color;
		}

		disp_buf[y_pos][x_pos >> 1] = pix;
	}
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
/*
 * draw_bitmap row by row
		while(width--)
		{
			if(src[0] & offset)
				tft_encode_color(x0);
			offset <<= 1;
			if(offset > 255)
			{
				offset = 1;
				src++;
			}
			x0++;
		}
*/
		width += x0;
		while(rows--)
		{
			for( int i=x0; i<width; i++)
			{
				if(src[0] & offset)
				{
					tft_encode_color(i, y0, fg_color);
				}
				offset <<= 1;
				if(offset > 255)
				{
					offset = 1;
					src++;
				}
			}
			y0++;
		}

	}
}

void SPFD5414D::draw_char(int x0, unsigned int ch)
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

			offset = 1<< offset;
			width += x0;
			while(rows--)
			{
				for( int i=x0; i<width; i++)
				{
					if(src[0] & offset)
					{
						tft_encode_color(i, y0, fg_color);
					}
					offset <<= 1;
					if(offset > 255)
					{
						offset = 1;
						src++;
					}
				}
				y0++;
			}

		}
	}
}

void SPFD5414D::draw_point( int x, int y)
{
//	if(frame.y0 == y)
	if(frame.y0 <= y && y < frame.y1 && frame.x0 <= x && x <= frame.x1)
	{
		tft_encode_color(x, y, fg_color);
	}
}

void SPFD5414D::draw_hline( int x0, int x1, int y)
{
//	if(y==frame.y0)
	if( (y>=frame.y0) && (y<frame.y1))
	{
		while(x0 <= x1)
		{
			tft_encode_color(x0++, y, fg_color);
		}
	}
}

void SPFD5414D::draw_bline( int x0, int x1, int y)
{
//	if(y==frame.y0)
	if( (y>=frame.y0) && (y<frame.y1))
	{
		while(x0 <= x1)
		{
			tft_encode_color(x0++, y, bg_color);
		}
	}
}

void SPFD5414D::clear_rect (const RECT_T& area)
{
	RECT_T clr_area(area);
	if(clr_area.normalize(frame))
	{
		int y = clr_area.y0;
		while(y < clr_area.y1)
		{
			int x0 = clr_area.x0, x1 = clr_area.x1;
			if(x0 & 1)
			{
				tft_encode_color(x0++, y, bg_color);
			}

			if(x1 & 1)
				x1++;
			else
			{
				tft_encode_color(x1, y, bg_color);
			}
			if(x1 > x0 && (x1-x0) >= 2)
				memset(&disp_buf[y][x0 >> 1], pix_to_4bit(bg_color) | (pix_to_4bit(bg_color) << 4), (x1 -x0)>>1);
			y++;
		}
	}
}


void SPFD5414D::draw_vline( int y0, int y1, int x)
{
//	if((y0 <= frame.y0) && (y1 >= frame.y0))
//	{
//		tft_encode_color(x);
//	}
	if(frame.x0 <= x && x <= frame.x1)
	{
		if(y0 <= frame.y0)
			y0 = frame.y0;
		while(y0 <= y1 && y0 < frame.y1)
		{
			if (y0>=frame.y0)
			{
				tft_encode_color(x, y0, fg_color);
			}else
				break;
			y0++;
		}
	}
}

void SPFD5414D::invert_vline( int y0, int y1, int x)
{
//	if((y0 <= frame.y0) && (y1 >= frame.y0))
//	{
//		tft_encode_color(x, true);
//	}
	if(frame.x0 <= x && x <= frame.x1)
	{
		if(y0 <= frame.y0)
			y0 = frame.y0;
		while(y0 <= y1 && y0 < frame.y1)
		{
			if (y0>=frame.y0)
			{
				tft_invert_color(x, y0);
			}else
				break;
			y0++;
		}
	}
}

void SPFD5414D::invert_hline( int x0, int x1, int y)
{
//	if(y==frame.y0)
	if( (y>=frame.y0) && (y<frame.y1))
	{
		while(x0 <= x1)
		{
			tft_invert_color(x0++, y);
		}
	}
}

#pragma GCC reset_options


void SPFD5414D::adjust_for_screen (GObject** object, RECT_T& area)				//change the area and object to desktop if the display draws only rows or columns
{
																				//          *** example for display that draws rows ***
																				//area.x0 = 0;
																				//area.x1 = size_x - 1;
																				//*object = children;
}

//
//The TFT modules hardware interface methods

void SPFD5414D::tft_init_address_cmd(unsigned short address_cmd[])
{
	memcpy(address_cmd, tft_init_address, sizeof(tft_init_address));
}

void SPFD5414D::tft_write_row(unsigned short address_cmd[])
{
/*		0					1					2				3					4
  	SPFD5414D_CASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(127),
		5					6					7				8					9
	SPFD5414D_RASET, SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0), SPFD5414D_DATA(0),
	SPFD5414D_RAMWR
 */
	address_cmd[2] = SPFD5414D_DATA(frame.x0); address_cmd[4] = SPFD5414D_DATA(frame.x1);
	address_cmd[7] = SPFD5414D_DATA(frame.y0); address_cmd[9] = SPFD5414D_DATA(frame.y0);
	unsigned int * dst = tft_buf;
	for(int x= 0; x < 128; x+=2)
	{
		*dst++ = lut_to_tft_color[(disp_buf[frame.y0][x] & 0xF0)>>4];
		*dst++ = lut_to_tft_color[disp_buf[frame.y0][x] & 0x0F];
	}
	lcd_hnd->tsk_write(address_cmd, sizeof(tft_init_address)/2);
	lcd_hnd->tsk_write(tft_buf+frame.x0, frame.x1 - frame.x0 +1);
}

void SPFD5414D::redraw_screen (GObject* object, RECT_T area)
{
	if(area.normalize(rect))
	{
		unsigned short cmd_address[sizeof(tft_init_address)/sizeof(unsigned short)];
		frame = area;
#if GUI_DEBUG
		uint32_t t = CURRENT_TIME;
		GUI_TRACELN("LCD%u update {%u,%u %u,%u}", displays, frame.x0, area.y0, frame.x1, area.y1);
#endif
		tft_init_address_cmd(cmd_address);
		for (frame.y0=area.y0; frame.y0 <= area.y1; frame.y0++)
		{
			tft_write_row(cmd_address);
		}
#if GUI_DEBUG
	    GUI_TRACE(" %u ms", ms_since(t));
#endif
	}
}


void SPFD5414D::direct_write(GSplash draw_cb)
{
	unsigned short cmd_address[sizeof(tft_init_address)/sizeof(unsigned short)];

	clear_screen();
	frame.x0 = 0;
	frame.x1 = size_x-1;
	tft_init_address_cmd(cmd_address);
    for(frame.y0=0; frame.y0 < size_y; frame.y0++)
    {
    	frame.y1 = frame.y0+1;
    	draw_cb(this);
    	tft_write_row(cmd_address);
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


