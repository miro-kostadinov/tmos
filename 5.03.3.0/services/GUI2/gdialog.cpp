/*
 * gdialog.cpp
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#include <gdialog.h>

unsigned int ms_since(unsigned int time);

int GDialog::move_objects(int offset, int& selected_indx)
{
	GObject* tmp = children;
	int num_of_items=0;
	selected_indx = 0;
	while (tmp)
	{
		if(tmp->is_available())
			num_of_items++;
		if(tmp == focus)
			selected_indx = num_of_items;
		tmp->move(0, offset);
		tmp = tmp->nextObj;
	}
	return num_of_items;
}

void GDialog::move(int x, int y)
{
	if(vscroll && focus)
	{
		int items=0, offset=0;
		y = (client_rect.height() - focus->rect.height())/2;
		if(y < 0)
			y = 0;
		if(focus->rect.y1 > client_rect.y1)
		{
			// scroll down
			GObject* tmp = focus;
			while(tmp)
			{
				if(!tmp->nextObj)
				{
					if(tmp->rect.y1 - focus->rect.y0 <= client_rect.height())
						offset = client_rect.y1 - tmp->rect.y1;
					else
						offset = client_rect.y1 -y - focus->rect.y1;
					break;
				}
				else
				{
					if(tmp->nextObj->is_available() && focus->rect.y1 < tmp->nextObj->rect.y1)
					{
						offset = client_rect.y1 -y - focus->rect.y1;
						break;
					}
				}
				tmp = tmp->nextObj;
			}
		}
		if(focus->rect.y0 < client_rect.y0 )
		{
			// scroll up
			offset =0;
			GObject* tmp = children;
			while(tmp)
			{
				if(tmp->is_available() && tmp == focus)
				{
					if(focus->rect.y1 - children->rect.y0 <= client_rect.height())
						offset = client_rect.y0 - children->rect.y0;
					else
						offset = client_rect.y0 +y - focus->rect.y0;
					break;
				}
				tmp = tmp->nextObj;
			}
		}
		if(offset != 0)
		{
			unsigned int  delay = 5, t0;
			int precision =(offset>0)?3:-3;
			if((offset > 0 && offset - precision < 0) || (offset < 0 && offset-precision > 0))
				precision = 0;
			offset -= precision;
			int  offset_step = offset /5, offset_rem = offset%5;
			while(offset)
			{
				t0 = CURRENT_TIME;
				items = move_objects(offset_step+offset_rem, y);
				offset -= offset_step + offset_rem;
				delay +=20;
				offset_rem = 0;
				invalidate(this, rect);
				while(ms_since(t0) < delay)
					tsk_sleep(1);
			}
			offset_rem = ((precision>0)?1:-1);
			while(precision)
			{
				t0 = CURRENT_TIME;
				items = move_objects(offset_rem, y);
				precision -= offset_rem;
				invalidate(this, rect);
				while(ms_since(t0) < delay)
					tsk_sleep(1);
			}
			if(client_rect.y0 == children->rect.y0)
				y = 0;
			vscroll->SetScrollRange(GO_FLG_VSCROLL, items);
			vscroll->SetScrollPos(GO_FLG_VSCROLL, y, true); // y/10
			send_message(WM_DRAW, 0, 0L, this);
		}
	}
}

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

	move(0,0);
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
	if(vscroll)
		vscroll->draw_scroll(lcd);
	GWindow::draw_this(lcd);
}

unsigned int GDialog::initialize (GMessage& msg)
{
	unsigned int res;

	res = GWindow::initialize(msg);
	if(flags & GO_FLG_VSCROLL)
	{
		GObject* tmp = children;
		int maxy = client_rect.height();
		int items=0;
		while (tmp)
		{
			if(tmp->is_available())
				items++;
			if(maxy < tmp->client_rect.y1)
			{
				maxy = tmp->client_rect.y1; // rect
			}
			tmp = tmp->nextObj;
		}

		if(maxy > client_rect.height())
		{
			if(!vscroll)
				vscroll = new GScroll(this);
			if(vscroll)
			{
				if(flags & GO_FLG_BORDER)
					client_rect.x1 += get_border_size().x;
				vscroll->SetScrollRange(GO_FLG_VSCROLL, items);
				vscroll->SetScrollPos(GO_FLG_VSCROLL, 0, false);
				flags &= ~GO_FLG_VSCROLL;
				vscroll->ShowScroll(GO_FLG_VSCROLL, true);
			}
		}
	}
	return res;
}
