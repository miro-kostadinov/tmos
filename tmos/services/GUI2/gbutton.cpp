/*
 * gbutton.cpp
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#include <gbutton.h>
#include <tmos.h>
#include <lcd.h>

void GButton::draw_this (LCD_MODULE* lcd)
{
	lcd->color = PIX_WHITE;

	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	lcd->set_font(&FNT5x7);
	lcd->color = PIX_WHITE;
	set_xy_all(lcd, ((client_rect.y1 - client_rect.y0) >> 1) - (lcd->font->height >> 1), ALL_CENTER);
	if(type)
	{
		lcd->allign = ALL_LEFT;
		lcd->pos_x = client_rect.x0 + lcd->font->vdistance;
		if (type == GB_TYPE_RADIO)												//radio button draw
		{
			if (flags & GO_FLG_CHECKED)
				lcd->draw_icon(GICON_CHECKED_CIRCLE);
			else
				lcd->draw_icon(GICON_CIRCLE);
		}
		else																	//check box draw
		{
			if (flags & GO_FLG_CHECKED)
				lcd->draw_icon(GICON_CHECKED_SQUARE);
			else
				lcd->draw_icon(GICON_SQUARE);
		}
		lcd->pos_x = (client_rect.y1 - client_rect.y0) + client_rect.x0;
	}
	draw_text(lcd, label.c_str());
	if (flags & GO_FLG_SELECTED)
	{
		for (int i = client_rect.y0; i <= client_rect.y1; i++)
			invert_hline (client_rect.x0, client_rect.x1, i);
	}
}


unsigned int GButton::process_key (GMessage msg)
{
	switch (msg.param)
	{
	case KEY_OK:
		if (type)
			send_message (GMessage (WM_CHANGE, (unsigned int) this, 0L, parent));	//sends a change message if the button is radio or check box
		else
			send_message (GMessage (WM_COMMAND, code, 0L, parent));				//sends a command based on the button code
		return 1;
	}
	return 0;
}
