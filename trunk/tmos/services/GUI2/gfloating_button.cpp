/*
 * gfloating_button.cpp
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#include <gfloating_button.h>
#include <lcd.h>
#include <gbutton.h>

unsigned int GFloating_Button::initialize (GMessage msg)
{
	GObject::initialize(msg);
	if (type)
		client_rect.x0 += (client_rect.y1 - client_rect.y0);
	addChild (new GFloating_Text (0, client_rect, label));
	if (type)
		client_rect.x0 -= (client_rect.y1 - client_rect.y0);
	children->initialize(msg);
	send_message(GMessage(WM_DRAW, 0, 0L, this));
	return 0;
}

void GFloating_Button::draw_this (LCD_MODULE* lcd)
{
	lcd->color = PIX_WHITE;

	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	if(type)
	{
		set_xy_all(lcd, ((client_rect.y1 - client_rect.y0) >> 1) - (lcd->font->height >> 1), ALL_LEFT);
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

unsigned int GFloating_Button::process_key (GMessage msg)
{
	switch (msg.param)
	{
	case KEY_OK:
		if (type)
			send_message (GMessage (WM_CHANGE, (unsigned int) this, 0L, parent));
		else
			send_message (GMessage (WM_COMMAND, code, 0L, parent));
		return 1;
	}
	return 0;
}
