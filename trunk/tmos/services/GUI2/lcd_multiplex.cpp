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
	for (int i = 0; i < GUI_DISPLAYS; i++)
	{
		if (lcd[i]->display & ((GWindow*)object)->displays)
		{
			parent = lcd[i];
			rect = lcd[i]->rect;
			lcd[i]->invalidate(object, area);
		}
	}
}
