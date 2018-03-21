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

const unsigned int SPFD5414D::lut_to_tft_color[16] =
{
	PIX_BLACK,
	PIX_BLUE,
	PIX_GREEN,
	PIX_CYAN,
	PIX_RED,
	PIX_MAGENTA,
	PIX_BROWN,
	PIX_LIGHTGRAY,
	PIX_DARKGRAY,
	PIX_LIGHTBLUE,
	PIX_LIGHTGREEN,
	PIX_LIGHTCYAN,
	PIX_LIGHTRED,
	PIX_LIGHTMAGENTA,
	PIX_YELLOW,
	PIX_WHITE
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

void SPFD5414D::clear_screen()
{
	memclr(disp_buf, sizeof(disp_buf));
}

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
				tft_encode_color(x0);
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
	if(frame.y0 == y)
	{
		tft_encode_color(x);
	}
}

void SPFD5414D::draw_hline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		while(x0 <= x1)
		{
			tft_encode_color(x0++);
		}
	}
}

void SPFD5414D::draw_bline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		unsigned int bkp = color;
		color = PIX_BLACK;
		while(x0 <= x1)
		{
			tft_encode_color(x0++);
		}
		color = bkp;
	}
}

void SPFD5414D::draw_vline( int y0, int y1, int x)
{
	if((y0 <= frame.y0) && (y1 >= frame.y0))
	{
		tft_encode_color(x);
	}
}

void SPFD5414D::invert_vline( int y0, int y1, int x)
{
	if((y0 <= frame.y0) && (y1 >= frame.y0))
	{
		tft_encode_color(x, true);
	}
}

void SPFD5414D::invert_hline( int x0, int x1, int y)
{
	if(y==frame.y0)
	{
		while(x0 <= x1)
		{
			tft_encode_color(x0++, true);
		}
	}
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

void SPFD5414D::redraw_rect (GObject* object, RECT_T area)						//redraws an area of the object on the LCD by calling its draw (LCD_MODULE, RECT_T) function
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
		if(owner)
		{
			// check that is window
			if(owner != this)
			{
				// It is a common object, try to find its owner window
				while(owner)
				{
					if(owner->parent && owner->parent == this)
						break;
					owner = owner->parent;
				}
			}
			else
			{
				// It is a window
				owner = object;
			}
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
#if GUI_DISPLAYS > 1
					GUI_TRACELN("LCD%u draw {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
#else
					GUI_TRACELN("LCD draw {%u,%u %u,%u}", frame.x0, area.y0, frame.x1, area.y1);
#endif
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
	{
#if GUI_DISPLAYS > 1
		GUI_TRACELN("LCD%u draw {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
#else
		GUI_TRACELN("LCD draw {%u,%u %u,%u}", frame.x0, area.y0, frame.x1, area.y1);
#endif
	}
    GUI_TRACE(" %d ms\e[m", ms_since(t0));
#endif
}

void SPFD5414D::redraw_screen (GObject* object, RECT_T area)
{
	if(area.normalize(rect))
	{
		unsigned short cmd_address[sizeof(tft_init_address)/sizeof(unsigned short)];
		frame = area;
#if GUI_DEBUG
		uint32_t t = CURRENT_TIME;
#if GUI_DISPLAYS > 1
		GUI_TRACELN("LCD%u update {%u,%u %u,%u}", display, frame.x0, area.y0, frame.x1, area.y1);
#else
		GUI_TRACELN("LCD update {%u,%u %u,%u}", frame.x0, area.y0, frame.x1, area.y1);
#endif
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


