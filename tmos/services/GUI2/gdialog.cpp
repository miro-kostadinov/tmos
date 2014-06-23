/*
 * gdialog.cpp
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#include <gdialog.h>




unsigned int GDialog::process_key (GMessage& msg)
{
	unsigned int res;

	if(GWindow::process_key(msg))
		return 1;

	switch (msg.param)
	{
	case KEY_RIGHT:
	case KEY_DOWN:
		res = focus_on_next();
		break;

	case KEY_LEFT:
	case KEY_UP:
		res =  focus_on_previous();
		break;

	default:
		return 0;
	}
	if(vscroll && focus)
	{
		int y, offset=0;
		y = vpos + focus->rect.y1;
		vscroll->SetScrollPos(GO_FLG_VSCROLL, y/10, true);
		if(focus->rect.y1 > client_rect.y1)
		{
			// scroll down
			offset = client_rect.y1 - focus->rect.y1 -5;
		}
		if(focus->rect.y0 < client_rect.y0 )
		{
			offset = client_rect.y0 -focus->rect.y0 +10;
			// scroll up
		}
		if(offset != 0)
		{
			GObject* tmp = children;

			vpos -= offset;
			while (tmp)
			{
				if(tmp->flags & GO_FLG_SHOW)
				{
					tmp->move(0, offset);
				}
				tmp = tmp->nextObj;
			}

			send_message(WM_DRAW, 0, 0L, this);
		}

	}

	return res;
}

unsigned int GDialog::process_command (GMessage& msg)
{
	switch(msg.param)
	{
	case GO_IDOK:
		notify_message(WM_CLOSE, GO_IDOK, this);								//closes the window
		return 1;
	case GO_IDCANCEL:
		notify_message(WM_CLOSE, GO_IDCANCEL, this);							//closes the window
		return 1;
	default:
		notify_message(WM_COMMAND, msg.param, NULL);
		return 0;
	}
	return 0;
}

unsigned int GDialog::process_default (GMessage& msg)
{
	switch (msg.code)
	{
	case WM_CHANGE:
	{
		if(msg.param)
		{
			GObject* obj = (GObject*)msg.param;
			if (obj->flags & GO_FLG_CHECKED)
				obj->clr_flag (GO_FLG_CHECKED);
			else
				obj->set_flag (GO_FLG_CHECKED);
			notify_message(WM_CHANGE, obj->id, obj);
			return 1;
		}
		break;
	}
	case WM_SETFOCUS:
		return get_focus(false);

	default:
		return 0;
	}
	return 0;
}

void GDialog::draw_this (LCD_MODULE* lcd)
{
	GWindow::draw_this(lcd);
	if(vscroll)
		vscroll->draw_scroll(lcd);
}

unsigned int GDialog::initialize (GMessage& msg)
{
	unsigned int res;

	res = GWindow::initialize(msg);
	if(flags & GO_FLG_VSCROLL)
	{
		GObject* tmp = children;
		int maxy = client_rect.height();

		while (tmp)
		{
			if(tmp->is_available())
			{
				if(maxy < tmp->client_rect.y1)
					maxy = tmp->client_rect.y1;
			}
			tmp = tmp->nextObj;
		}

		if(maxy > client_rect.height())
		{
			if(!vscroll)
				vscroll = new GScroll(this);
			if(vscroll)
				vscroll->SetScrollRange(GO_FLG_VSCROLL, maxy/10 + 1);
		}
	}
	return res;
}
