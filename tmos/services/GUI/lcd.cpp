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

    PIO_CfgOutput1(lcd->pins->bklt_pin);
    while(1)
    {
     	PIO_SetOutput(lcd->pins->bklt_pin);
     	if(tsk_wait_signal(SIG_BACKLIGHT_TASK, backlight_time ))
     		continue;

     	for(int t = 1; t<20; t++)
     	{
     		if(tsk_test_signal(SIG_BACKLIGHT_TASK))
     			break;
         	PIO_SetOutput(lcd->pins->bklt_pin);
           	tsk_sleep(20 - t);
           	PIO_ClrOutput(lcd->pins->bklt_pin);
         	tsk_sleep(t);
     	}
       	tsk_get_signal(SIG_BACKLIGHT_TASK);
    }
}

TASK_DECLARE_STATIC(backlight_task, "BLIT", (void (*)(void))backlight_thread, 1, 30+TRACE_SIZE);


void LCD_MODULE::lcd_init(GUI_CB splash)
{
	// LCD Reset (using lcd_hnd to talk with the MSP driver)

	usr_task_init_static(&backlight_task_desc, true);
	backlight_task.sp->r0.as_voidptr = this;

    PIO_CfgOutput0(pins->rst_pin);
	tsk_sleep(20);
	PIO_SetOutput(pins->rst_pin);
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
	chars_per_row = size_x / font->spacing;
}

void LCD_MODULE::set_xy_all(unsigned int xy, unsigned int all)
{
	pos_y = xy;
	pos_x = xy >> 16;
	allign = all;
}

const char* LCD_MODULE::draw_text(const char *txt)
{
    unsigned int width,len, pos;
	unsigned int c;

//	if( (pos_y + font->hight) < offset_y0)
//		return NULL;

	while(*txt == ' ')
		txt++;

    pos = pos_x + font->distance;
    len = 0;
    width = 0;
    while( pos < size_x )
    {
        if( !(c=txt[len++]) )
        {
            width =pos;
            break;
        }
        if( !IS_ALPHANUM(c))
        {
        	width = pos;
        }
    	pos += font->spacing;
    }

    if(!width && len)
    {
    	width = pos_x + font->distance + (len-1)*font->spacing;
    }


    if(width)
    {
    	len = width -pos_x -font->distance;
    	if(allign != ALL_LEFT)
    	{
            pos = size_x - len;
    		if(allign == ALL_CENTER)
    			pos >>= 1;
    	}
    	else
    		pos = pos_x + font->distance;

    	width = pos + len;

    	while( (pos < width) && (c = *txt))
    	{
    		txt++;
    		if(c>0x1f)
    		{
                c -= 0x20;

				draw_bitmap(pos, pos_y, font->font_data + c * font->char_bytes,
						font->width, font->hight);
    		}
    		pos += font->spacing;
    	}
    }

    pos_y += font->vspacing;
    return txt;
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

	win.mode.as_voidptr = (void*)callback;
    win.next = &win;
    win.prev = &win;
    redraw_screen(&win);
}
