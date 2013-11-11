/*
 * gfloating_button.cpp
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#include <gfloating_button.h>
#include <lcd.h>
#include <gbutton.h>

void GFloating_Button::allocate_border(void)
{
	client_rect.x0 = rect.x0 +2;
	client_rect.y0 = rect.y0 +2;
	client_rect.x1 = rect.x1 -3;
	client_rect.y1 = rect.y1 -3;
}

void GFloating_Button::draw_border(RECT_T& frame)
{
	draw_hline(frame.x0, frame.x1-1, frame.y0);
	draw_vline(frame.y0 + 1, frame.y1 - 1, frame.x0);
	draw_hline(frame.x0, frame.x1-1, frame.y1-1);
	draw_vline(frame.y0 + 1, frame.y1 - 2, frame.x1-1);

	draw_hline(frame.x0+1, frame.x1, frame.y1);
	draw_vline(frame.y0+1, frame.y1, frame.x1);
}

unsigned int GFloating_Button::initialize (GMessage& msg)
{
	GObject::initialize(msg);

	if (type)
		client_rect.x0 += (client_rect.y1 - client_rect.y0);
	addChild (new GFloating_Text (0, client_rect, label));
	if (type)
		client_rect.x0 -= (client_rect.y1 - client_rect.y0);
	children->initialize(msg);
	send_message(WM_DRAW, 0, 0L, this);
	return 0;
}

void GFloating_Button::draw_this (LCD_MODULE* lcd)
{
	lcd->color = PIX_WHITE;

	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	if(type)
	{
		set_xy_all(lcd, ((client_rect.y1 - client_rect.y0) >> 1) - (lcd->font->height >> 1), TA_LEFT);
		lcd->pos_x = client_rect.x0 + lcd->font->hdistance;
		if (type == GB_TYPE_RADIO)
		{
			if (flags & GO_FLG_CHECKED)
				lcd->draw_icon(GICON_CHECKED_CIRCLE);
			else
				lcd->draw_icon(GICON_CIRCLE);
		}
		else
		{
			if (flags & GO_FLG_CHECKED)
				lcd->draw_icon(GICON_CHECKED_SQUARE);
			else
				lcd->draw_icon(GICON_SQUARE);
		}
	}
}

unsigned int GFloating_Button::process_key (GMessage& msg)
{
	switch (msg.param)
	{
	case KEY_OK:
		if (type)
			send_message (WM_CHANGE, (unsigned int) this, 0L, parent);
		else
			send_message (WM_COMMAND, code, 0L, parent);
		return 1;
	}
	return 0;
}
