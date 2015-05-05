/*
 * gfloating_text.cpp
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#include <gfloating_text.h>
#include <lcd.h>

unsigned int GFloating_Text::initialize(GMessage& msg)
{
/*
	client_rect = rect;
	addChild (new GText (0, rect, txt, GO_FLG_DEFAULT, font));
	children->client_rect = rect;
	children->client_rect.y0 = rect.y0 + (rect.height() >> 1) - ((font->vspacing + font->vdistance) >> 1);
	children->client_rect.y1 = children->client_rect.y0 + font->vdistance + font->vspacing;
	children->client_rect.x1 = children->rect.x1 = rect.x0 + text_width;
	return 1;
*/

	GObject::initialize(msg);
	text_rect = client_rect;
	text_width = (font->hdistance<<2) +(txt.length()+1)*font->hspacing;
	client_rect.y0 = text_rect.y0 = client_rect.y0 + (rect.height() >> 1) - ((font->vspacing + font->vdistance) >> 1);
	client_rect.y1 = text_rect.y1 = text_rect.y0 + font->vdistance + font->vspacing;
	if(text_width > client_rect.width() )
		text_rect.x1 = text_rect.x0 + text_width;
	if(msg.param)
		send_message(WM_DRAW, 0, client_rect.as_int, this);
	return 1;
}

void GFloating_Text::draw_this (LCD_MODULE* lcd)
{
	lcd->set_font(font);
	lcd->color = PIX_WHITE;
	lcd->pos_x = text_rect.x0 + (signed)lcd->font->hdistance;
	lcd->pos_y = text_rect.y0 + (signed)lcd->font->vdistance;
	lcd->allign = TA_LEFT;

	RECT_T tmp(client_rect);
	client_rect = text_rect;
	draw_text_line(lcd, txt.c_str(), txt.length());
	client_rect = tmp;
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
}

unsigned int GFloating_Text::process_idle(GMessage& msg)
{
	if (parent->flags & GO_FLG_SELECTED)
	{
		if (text_rect.width() > client_rect.width())
			if ((CURRENT_TIME - float_time) > 200)
			{
				invalidate (this, client_rect);
				if (text_rect.x1 <= client_rect.x0)
				{
					text_rect.x0 =  client_rect.x1;
					text_rect.x1 =  client_rect.x1 + text_width;
				}
				text_rect.x0 -= 3;
				text_rect.x1 -= 3;
				float_time = CURRENT_TIME;
			}
	}
	else
	{
		if (text_rect.x0 != client_rect.x0)
		{
			text_rect.x0 = client_rect.x0;
			text_rect.x1 = client_rect.x0 + text_width;
			invalidate (this, rect);
		}
	}
	return 0;
}

