/*
 * lcd_multiplex.cpp
 *
 *  Created on: 28.08.2013
 *      Author: edguest
 */

#include <tmos.h>
#include <lcd_multiplex.h>
#include <gwindow.h>

void LCD_MULT::invalidate (GObject* object, RECT_T area)
{
	ENTER_FUNCTION(area, this)
	for (int i = 0; i < GUI_DISPLAYS; i++)
	{
		if (lcd[i]->displays & object->displays)
		{
			parent = lcd[i];
			rect = lcd[i]->rect;
			if(object == this)
				lcd[i]->invalidate(lcd[i], area);
			else
				lcd[i]->invalidate(object, area);
		}
		parent = nullptr;
	}
	LEAVE_FUNCTION(area, this)
}

void LCD_MULT::redraw_screen(GObject* object, RECT_T area)
{
	for (int i = 0; i < GUI_DISPLAYS; i++)
	{
		if (lcd[i]->displays & object->displays)
		{
			parent = lcd[i];
			rect = lcd[i]->rect;
			lcd[i]->redraw_screen(object, area);
		}
		parent = nullptr;
	}
}

void LCD_MULT::LPtoDP(POINT_T& size, unsigned char lcd_index)
{
	if(lcd_index < GUI_DISPLAYS)
		lcd[lcd_index]->LPtoDP(size, lcd_index);
}

void LCD_MULT::DPtoLP(POINT_T& size, unsigned char lcd_index)
{
	if(lcd_index < GUI_DISPLAYS)
		lcd[lcd_index]->DPtoLP(size, lcd_index);
}
