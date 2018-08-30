/*
 * lcd.cpp
 *
 *  Created on: 26.01.2011
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <lcd.h>

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

TASK_DECLARE_STATIC(backlight_task, "BLIT", (void (*)(void))backlight_thread, 10, 50+TRACE_SIZE);


void LCD_MODULE::lcd_init(GUI_CB splash)
{
#if GUI_DISPLAYS > 1
	if(display == 1)
#endif
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
}

void LCD_MODULE::backlight_signal(void)
{
	usr_send_signal(&backlight_task, SIG_BACKLIGHT_TASK);
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
	    	if(allign != ALL_LEFT)
	    	{
	            pos = size_x - len;
	    		if(allign == ALL_CENTER)
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
	return NULL;
}

const char* LCD_MODULE::draw_text (const char* txt)
{
	if(!txt)
		return NULL;
	while(*txt == ' ' || *txt =='\r' || *txt == '\n')
		txt++;
	return draw_text_no_space (txt);
}

const char* LCD_MODULE::draw_text_no_space (const char *txt)
{
    unsigned int width,len, pos;
	unsigned int c;

//	if( (pos_y + font->hight) < offset_y0)
//		return (NULL);
	if(!txt)
		return NULL;

//	while(*txt == ' ' || *txt =='\r' || *txt == '\n')
//		txt++;

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
    	if(allign != ALL_LEFT)
    	{
            pos = size_x - len;
    		if(allign == ALL_CENTER)
    			pos >>= 1;
    	}
    	else
    		pos = pos_x + font->hdistance;

    	width = pos + len;

    	while( (pos < width) && (c = *txt))
    	{
    		txt++;
    		if(c>0x1f)
    		{
                c -= 0x20;

				draw_bitmap(pos, pos_y, font->font_data + c * font->char_bytes,
						font->width, font->height);
    		}
    		pos += font->hspacing;
    	}
    }

    pos_y += font->vspacing;
    return (txt);
}
const char* LCD_MODULE::draw_row(const char *txt)
{
    unsigned int width,len, pos;
	unsigned int c;

//	if( (pos_y + font->hight) < offset_y0)
//		return (NULL);
	if(!txt)
		return NULL;

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
    	if(allign != ALL_LEFT)
    	{
            pos = size_x - len;
    		if(allign == ALL_CENTER)
    			pos >>= 1;
    	}
    	else
    		pos = pos_x + font->hdistance;

    	width = pos + len;

    	while( (pos < width) && (c = *txt))
    	{
    		txt++;
    		if(c>0x1f)
    		{
                c -= 0x20;

				draw_bitmap(pos, pos_y, font->font_data + c * font->char_bytes,
						font->width, font->height);
    		}
    		pos += font->hspacing;
    	}
    }

    pos_y += font->vspacing;
    return (txt);
}


void LCD_MODULE::clear_rect(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{

    while(y0 <= y1)
    {
        draw_bline(x0, x1, y0++);
    }
}

void LCD_MODULE::lcd_single_window(GUI_CB callback)
{
	CWindow win;
	win.rect.x0 = 0;
	win.rect.y0 = 0;
	win.rect.x1 = size_x;
	win.rect.y1 = size_y;
	win.mode.as_voidptr = (void*)callback;
    win.next = NULL;
#if GUI_DISPLAYS > 1
    win.displays = display;
#endif
    redraw_screen(&win);
}
