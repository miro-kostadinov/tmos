/*
 * lcd.cpp
 *
 *  Created on: 26.01.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <gwindow.h>
#include <lcds/lcd.h>

volatile unsigned int backlight_time;

WEAK void backlight_thread(LCD_MODULE *lcd)
{
    ALLOCATE_SIGNAL(SIG_BACKLIGHT_TASK);

    PIO_CfgOutput1(lcd->pins[BKLT_PIN_INDX]);
    while(1)
    {
     	PIO_SetOutput(lcd->pins[BKLT_PIN_INDX]);
     	if(tsk_wait_signal(SIG_BACKLIGHT_TASK, backlight_time ))
     		continue;
     	if(backlight_time)
     	{
			// reduce the current consumption to 5%
			int t = 20;
			while(!tsk_test_signal(SIG_BACKLIGHT_TASK))
			{
				PIO_SetOutput(lcd->pins[BKLT_PIN_INDX]);
				tsk_sleep(t);
				PIO_ClrOutput(lcd->pins[BKLT_PIN_INDX]);
				tsk_sleep(20 - t--);
				if(!t)
					t=1;
			}
     	}
       	tsk_get_signal(SIG_BACKLIGHT_TASK);
    }
}

TASK_DECLARE_STATIC(backlight_task, "BLIT", (void (*)(void))backlight_thread, 10, 140+TRACE_SIZE);


void LCD_MODULE::lcd_init(GSplash splash)
{
	if(displays == 1)
	{
		usr_task_init_static(&backlight_task_desc, false);
		backlight_task.sp->r0.as_voidptr = this;
		usr_task_schedule(backlight_task_desc.tsk);
	}

	// LCD Reset
	PIO_CfgOutput0(pins[RST_PIN_INDX]);
	tsk_sleep(20);
	PIO_SetOutput(pins[RST_PIN_INDX]);
	tsk_sleep(20);

    //LCD Handle initialization (LCD attached to SPI)
    lcd_hnd->tsk_safe_open(lcd_hnd->drv_index, lcd_hnd->mode.as_voidptr);
    lcd_reset();
    direct_write(splash);
}

void LCD_MODULE::backlight_signal(void)
{
	usr_send_signal(&backlight_task, SIG_BACKLIGHT_TASK);
}

void LCD_MODULE::invalidate (GObject* object, RECT_T area)						//goes through the window list and adjusts the area vertically
{
	INVALIDATE_ENTER_FUNCTION(area, this)
	if(!area.normalize(rect))
	{
		INVALIDATE_LEAVE_FUNCTION(area, this)
		return;
	}

	GObject* tmp = object->nextObj;
	if(object == this)
	{
		for (tmp = children; tmp; tmp = tmp->nextObj)
			tmp->invalidate(tmp, area); // tmp->rect
		INVALIDATE_LEAVE_FUNCTION(area, this)
		return;
	}
	while (tmp)
	{
		if (
				(tmp->displays & this->displays) &&
				(tmp->flags & GO_FLG_SHOW) && !(tmp->flags & GO_FLG_TRANSPARENT) )
		{
			if (area.x0 >= tmp->rect.x0 && area.x1 <= tmp->rect.x1)
			{																	//the entire area is covered horizontally
				if (area.y0 < tmp->rect.y0)
				{
					if (area.y1 <= tmp->rect.y1 && area.y1 >= tmp->rect.y0)
						area.y1 = tmp->rect.y0 - 1;								//the whole bottom part of the area is covered
				}
				else
				{
					if (area.y1 <= tmp->rect.y1)
					{
						INVALIDATE_LEAVE_FUNCTION(area, this)
						return;													//the whole area is covered
					}
					if (area.y1 > tmp->rect.y1 && area.y0 <= tmp->rect.y1)
						area.y0 = tmp->rect.y1 + 1;								//the whole upper part of the area is covered
				}
			}
		}
		tmp = tmp->nextObj;
	}
	adjust_for_screen(&object, area);											//change the area and object to desktop if the display draws only rows or columns
	for (tmp = object; tmp; tmp = tmp->nextObj)
	{
		if (tmp->displays & this->displays)
		{
			if (tmp->flags & GO_FLG_SHOW)
			{
				tmp->draw(this, area);											//calls the  draw function (for the area) for every window after object in the Z-order
			}
		}
	}
	INVALIDATE_LEAVE_FUNCTION(area, this)
}

void LCD_MODULE::redraw_rect (GObject* object, RECT_T area)						//redraws an area of the object on the LCD by calling its draw_this(LCD_MODULE) function
{
	do_reset();
	//1. Try to get the bottom window that contain a redrawing object
	GObject* initial;
	if(object)
	{
#if GUI_DEBUG
		unsigned int t0 = CURRENT_TIME;
#endif
		GUI_TRACE("\e[1;31m" "LCD%u redraw %s[%d] ", display,
			szlist_at(obj_type_str,object->get_object_type()), (object)?object->id:-1);
		GUI_TRACE("area[%d,%d,%d,%d] obj{%d,%d,%d,%d}",
			area.x0, area.y0, area.x1, area.y1,
			object->rect.x0, object->rect.y0, object->rect.x1, object->rect.y1
			);
		gui_trace_sleep();

		initial = children;
		GObject* owner = object->parent;

		// check that is window
		if(owner)
		{
			if(!owner->is_lcd())
			{	// It is a common object, try to find its owner window
				while(owner)
				{
					if(owner->parent && owner->parent->is_lcd())
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
		GUI_ASSERT_MSG((owner && initial), "GUI Z-order exception!");
	// 2. preparation of drawing frames if overlaps occur
		GObject* tmp;
		int res=0; // unknown
		// frame adjustment to the drawing area
		frame = area;
		if(!initial->nextObj)
		{   // the object is the topmost,  draw the whole area
			frame.y1++; // for row by row drawing compatibility
			if(!(object->flags & GO_FLG_TRANSPARENT))
				clear_rect(frame);
			object->draw_this(this);
		}else
		{	// the object may be overlapped by another
			bool row_by_row = false;
			for(;frame.y0 <= area.y1 && !row_by_row; frame.y0++)
			{
				frame.y1 = frame.y0+1;
				tmp = initial;
				while((tmp=tmp->nextObj))
				{
					if(tmp->displays & this->displays)
						res |= object->overlapped(tmp, frame);
					if(res & OVER_FLG_FULL_OVERLAP)
					{
						row_by_row = true;
						break;
					}
				}
				if((res & OVER_FLG_DRAWING) && (res & OVER_FLG_PARTIAL))
				{
					row_by_row = true;
				}
			}
			if(!row_by_row)
			{ // the object is not overlap, draw whole area
				frame = area;
				frame.y1++; // for row by row drawing compatibility
				if(!(object->flags & GO_FLG_TRANSPARENT))
					clear_rect(frame);
				object->draw_this(this);
			}
			else
			{
				// the object is overlap, draw row by row
				for(frame.y0=area.y0; frame.y0 <= area.y1; frame.y0++)
				{
					frame.y1 = frame.y0+1;
					frame.x0 = area.x0;
					frame.x1 = area.x1;

					res = OVER_FLG_DRAWING;
					while(1)
					{
						tmp = initial->nextObj;
						while(tmp)
						{
							if(tmp->displays & this->displays)
								res |= object->overlapped(tmp, frame);
							tmp = tmp->nextObj;
						}
						if(res & OVER_FLG_FULL_OVERLAP)
							break;
						if(res & OVER_FLG_DRAWING)
						{
							if(!(object->flags & GO_FLG_TRANSPARENT))
								clear_rect(frame);
							object->draw_this(this);
							if(res & OVER_FLG_PARTIAL)
							{
								GUI_TRACELN("partial draw {%u,%u %u,%u}", frame.x0, area.y0, frame.x1, area.y1);
								frame.x0 = frame.x1+1;
								frame.x1 = area.x1;
								res = OVER_FLG_DRAWING;
								if(frame.x0 <= frame.x1)
									continue;
							}
						}
						break;
					}
				}
			}
		}
#if GUI_DEBUG
		GUI_TRACE(" %d ms\e[m", ms_since(t0));
		gui_trace_sleep();
#endif
	}
//	uint32_t t = CURRENT_TIME;
//	for (frame.y0=(area.y0 - (area.y0&7)); frame.y0 <= area.y1; frame.y0 += 8)
//	{
//		update_screen();
//	}
//	TRACE(" %u ms", ms_since(t));
}

void LCD_MODULE::set_font(const RENDER_MODE* afont)
{
	font = afont;
	chars_per_row = size_x / font->hspacing;
}

void LCD_MODULE::set_xy_all(unsigned int xy, unsigned int all)
{
	pos_y = xy;
	pos_x = xy >> 16;
	allign = all;
}
const char* LCD_MODULE::get_next_txt_row(const char *txt) const
{
    unsigned int width,len, pos;
	unsigned int c;

	if(txt && txt[0])
	{
		while(*txt == ' ' || *txt =='\r' || *txt == '\n')
			txt++;

	    pos = pos_x + font->hdistance;
	    len = 0;
	    width = 0;
	    while( pos < size_x )
	    {
	    	c=txt[len++];
	        if( !IS_ALPHANUM(c))
	        {
	        	width = pos;
	        	if(c == 0 || c =='\r' || c == '\n')
	        		break;
	        }
	    	pos += font->hspacing;
	    }

	    if(!width && len)
	    {
	    	width = pos_x + font->hdistance + (len-1)*font->hspacing;
	    }


	    if(width)
	    {
	    	len = width -pos_x -font->hdistance;
	    	if((allign & TA_HORIZONTAL) != TA_LEFT)
	    	{
	            pos = size_x - len;
	    		if((allign & TA_HORIZONTAL) == TA_CENTER)
	    			pos >>= 1;
	    	}
	    	else
	    		pos = pos_x + font->hdistance;

	    	width = pos + len;

	    	while( (pos < width) && *txt)
	    	{
	    		txt++;
	    		pos += font->hspacing;
	    	}
	    }
		if(txt[0])
		    return (txt);
	}
	return nullptr;
}

const char* LCD_MODULE::draw_text (const char* txt)
{
	if(!txt)
		return nullptr;
	while(*txt == ' ' || *txt =='\r' || *txt == '\n')
		txt++;
	return draw_text_no_space (txt);
}

const char* LCD_MODULE::draw_text_no_space (const char *txt)
{
    unsigned int width,len, pos;
	unsigned int c;

	if(!txt)
		return nullptr;

    pos = pos_x + font->hdistance;
    len = 0;
    width = 0;
    while( pos < size_x )
    {
    	c=txt[len++];
        if( !IS_ALPHANUM(c))
        {
        	width = pos;
        	if(c == 0 || c =='\r' || c == '\n')
        		break;
        }
    	pos += font->hspacing;
    }

    if(!width && len)
    {
    	width = pos_x + font->hdistance + (len-1)*font->hspacing;
    }


    if(width)
    {
    	len = width -pos_x -font->hdistance;
    	if((allign & TA_HORIZONTAL) != TA_LEFT)
    	{
            pos = size_x - len;
    		if((allign & TA_HORIZONTAL) == TA_CENTER)
    			pos >>= 1;
    	}
    	else
    		pos = pos_x + font->hdistance;

    	width = pos + len;

    	while( (pos < width) && (c = *txt))
    	{
    		txt++;
    		draw_char(pos, c);
/*
    		if(c>0x1f)
    		{
                c -= 0x20;

				draw_bitmap(pos, pos_y, font->font_data + c * font->char_bytes,
						font->width, font->height);
    		}
*/
    		pos += font->hspacing;
    	}
    }

    pos_y += font->vspacing;
    return (txt);
}

void LCD_MODULE::draw_icon (unsigned char icon)
{
	draw_bitmap(pos_x, pos_y, (font->icon_data + (icon*font->char_bytes)), font->width, font->height);
}

const char* LCD_MODULE::draw_row(const char *txt)
{
    unsigned int width,len, pos;
	unsigned int c;

//	if( (pos_y + font->hight) < offset_y0)
//		return (nullptr);
	if(!txt)
		return nullptr;

	while(*txt == ' ')
		txt++;

    pos = pos_x + font->hdistance;
    len = 0;
    width = 0;
    while( pos < size_x )
    {
        width =pos;
        if( !(c=txt[len++]) )
        {
            break;
        }
    	pos += font->hspacing;
    }

    if(!width && len)
    {
    	width = pos_x + font->hdistance + (len-1)*font->hspacing;
    }


    if(width)
    {
    	len = width -pos_x -font->hdistance;
    	if((allign & TA_HORIZONTAL) != TA_LEFT)
    	{
            pos = size_x - len;
    		if((allign & TA_HORIZONTAL) == TA_CENTER)
    			pos >>= 1;
    	}
    	else
    		pos = pos_x + font->hdistance;

    	width = pos + len;

    	while( (pos < width) && (c = *txt))
    	{
    		txt++;
    		draw_char(pos, c);
/*
    		if(c>0x1f)
    		{
                c -= 0x20;

				draw_bitmap(pos, pos_y, font->font_data + c * font->char_bytes,
						font->width, font->height);
    		}
    		pos += font->hspacing;
*/
    	}
    }

    pos_y += font->vspacing;
    return (txt);
}


void LCD_MODULE::clear_rect (const RECT_T& area)
{
	int y = area.y0;
    while(y <= area.y1)
        draw_bline(area.x0, area.x1, y++);
}

void LCD_MODULE::LPtoDP(POINT_T& size, const unsigned char lcd_index) const
{
	if(dot_pitch_x > dot_pitch_y)
	{
		size.x *= dot_pitch_y;
		size.x = (size.x/dot_pitch_x) + (((size.x%dot_pitch_x) >= (dot_pitch_x/2))?1:0);
	}
	if(dot_pitch_x < dot_pitch_y)
	{
		size.y *= dot_pitch_x;
		size.y = (size.y/dot_pitch_y) + (((size.y%dot_pitch_y) >= (dot_pitch_y/2))?1:0);
	}
}

void LCD_MODULE::DPtoLP(POINT_T& size, const unsigned char lcd_index) const
{
	if(dot_pitch_x > dot_pitch_y)
	{
		size.x *= dot_pitch_x;
		size.x = (size.x/dot_pitch_y) + (((size.x%dot_pitch_y) >= (dot_pitch_y/2))?1:0);
	}
	if(dot_pitch_x < dot_pitch_y)
	{
		size.y *= dot_pitch_y;
		size.y = (size.y/dot_pitch_x) + (((size.y%dot_pitch_x) >= (dot_pitch_x/2))?1:0);
	}
}

POINT_T LCD_MODULE::PolarToDP(const int deg, const int r, const unsigned char lcd_index) const
{
	POINT_T p;
	int32_t rx, ry;

	rx = ry = r * 1000;
	if(dot_pitch_x > dot_pitch_y)
	{
		rx = (rx * dot_pitch_y)/dot_pitch_x;
	}
	if(dot_pitch_x < dot_pitch_y)
	{
		ry = (ry * dot_pitch_x)/dot_pitch_y;
	}
	p.x = (int64_t(cos_x10000(deg)) * rx)/10000000;
	p.y = (int64_t(sin_x10000(deg)) * ry)/10000000;
	return p;
}

bool GClientLcd::CreateLcd(RECT_T& area, LCD_MODULE* lcd)
{
	if(client_rect.normalize(lcd->frame))
	{
		parent = lcd;
		rect=lcd->frame;
		lcd->frame = client_rect;
		client_rect = area; // replace the region of a drawing with this scrolling
		return true;
	}
	return false;
}

bool GClientLcd::RelaseLcd( )
{
	if(parent)
	{
		((LCD_MODULE*)parent)->frame = rect;
		return true;
	}
	return false;
}
