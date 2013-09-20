/*
 * gtext.cpp
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#include <gtext.h>
#include <tmos.h>
#include <lcd.h>

void GText::draw_this (LCD_MODULE* lcd)
{
	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	lcd->set_font(text_font);
	lcd->color = PIX_WHITE;
	lcd->pos_x = client_rect.x0 + (signed)lcd->font->hdistance;
	lcd->pos_y = client_rect.y0 + (signed)lcd->font->vdistance;
	lcd->allign = ALL_LEFT;
	draw_text(lcd, txt.c_str());

	if (flags & GO_FLG_SELECTED)
	{
		for (int i = client_rect.y0; i <= client_rect.y1; i++)
			invert_hline (client_rect.x0, client_rect.x1, i);
	}
}
