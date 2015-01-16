/*
 * glistbox.cpp
 *
 *  Created on: 22.11.2013
 *      Author: stanly
 */


#include <glistbox.h>
#include <lcd.h>

GListBox::~GListBox()
{
	if(list && list->parent == NULL)
	{
		delete list;
		list = NULL;
	}
}

POINT_T GListBoxMenu::get_border_size(void)
{
	return POINT_T(2,2);
}

void GListBoxMenu::draw_border(RECT_T& frame)
{
	draw_rectangle(frame);
}

bool GListBox::addItem(int num, const char* name)
{
	if(!list)
	{
		if(!(list = new GListBoxMenu))
			return false;
	}
	return list->AppendMenu(0, num, name);
}

bool GListBox::select(int num)
{
	if(list)
	{
		menu_template_t* ptr = list->FindItem(num);
		if(ptr)
		{
			list->item = ptr;
			txt = ptr->item_name;
			return true;
		}
	}
	return false;
}

void GListBox::draw_this (LCD_MODULE* lcd)
{
	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		if(flags & GO_FLG_BORDER)
			draw_border(rect);

		lcd->set_font(text_font);
		lcd->color = PIX_WHITE;
		lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));

		draw_caption(lcd);
		GClientLcd dc(this);
		if(dc.CreateLcd(scroll_rect, lcd))
		{
			lcd->pos_x = dc.client_rect.x0;
			lcd->pos_y = dc.client_rect.y0 +text_font->vdistance;
			remove_amp(txt);
			dc.draw_text(lcd, txt.c_str());
			dc.RelaseLcd();
		}
		if(vscroll)
			vscroll->draw_scroll(lcd);
		if (flags & GO_FLG_SELECTED)
			draw_rectangle(client_rect, true);
	}
}

POINT_T GListBox::get_border_size(void)
{
	return POINT_T(2,2);
}

void GListBox::draw_border(RECT_T& frame)
{
	draw_rectangle(frame);
}

unsigned int GListBox::initialize (GMessage& msg)
{
	GText::initialize(msg);
	RECT_T rc(parent->rect);

	if(list)
	{
		menu_template_t* ptr = list->base;
		int sx =0, len;
		int sy=0;

		if(ptr)
		{
			while(!ptr->item_name.empty() || ptr->item || ptr->parent)
			{
				len = ptr->item_name.length();
				if(len > sx)
					sx = len;
				++sy;
				++ptr;
			}
		}
		sx *= text_font->hspacing;
		sx += 2*get_border_size().x + 3;
		sy *= text_font->vspacing + text_font->vdistance;
		sy += 2*(text_font->vdistance + get_border_size().y);
		if(sy > rc.y1 )
			sx += GO_SCROLL_WIDTH+1;
		if( !sx || !sy )
		{
			delete list;
			list = NULL;
			return 1;
		}
		if(client_rect.height() > 0 && client_rect.width() > 0)
		{
			// align horizontal
			if(sx < client_rect.width())
				rc.x0 = client_rect.x0, rc.x1 = client_rect.x1;
			else
			{
				if(sx < rc.width())
				{
					if(client_rect.x1 - sx > rc.x0)
						rc.x1 = client_rect.x1, rc.x0 = rc.x1 - sx;
					else
					{
						if(client_rect.x0 + sx < rc.x1)
							rc.x0 = rect.x0, rc.x1 = rc.x0 + sx;
						else
						{
							if(rect.x1 - sx > rc.x0)
								rc.x1 = rect.x1, rc.x0 = rc.x1 - sx;
							else
							{
								if(rect.x0 + sx < rc.x1)
									rc.x0 = rect.x0, rc.x1 = rc.x0 + sx;
								else
									rc.x0 = (rc.width() - sx)/2, rc.x1 = rc.x0 + sx;
							}
						}
					}
				}
			}
			// align vertical
			if(sy + client_rect.y1 < rc.y1)
				rc.y0 = client_rect.y1, rc.y1 = rc.y0 + sy;
			else
			{
				if(rc.y0  < client_rect.y0 - sy )
					rc.y1 = client_rect.y0, rc.y0 = rc.y1 - sy;
				else
				{
					if(rc.y1 - client_rect.y1 > rc.y0 - client_rect.y0)
						rc.y0 = client_rect.y1;
					else
						rc.y1 = client_rect.y0;
				}
			}
		}
		else
		{
			if(rc.width() > sx)
				rc.x0 = rc.x1 - sx;
			if(rc.height() > sy)
				rc.y0 = rc.y1 - sy;
		}
		if(!msg.param)
		{
			list->id = id+100;
			list->rect = rc;
			list->title = NULL;//txt.c_str();
			list->flags = GO_FLG_ENABLED|GO_FLG_BORDER;
			if(align & LBS_DROPDOWN)
			{
				send_message(WM_SET_FLAGS, (GO_FLG_SHOW|GO_FLG_SELECTED), 0L, list);
				flags |= GO_FLG_SHOW|GO_FLG_SELECTED;
			}
			list->text_font = text_font;
			parent->addChildRef(list);
		}
		txt = list->item->item_name;
	}
	return 1;
}

unsigned int GListBox::process_key (GMessage& msg)
{
	switch(msg.param)
	{
	case KEY_OK:
		if(!list)
			return 0;
		if(list->flags & GO_FLG_SHOW)
		{
			list->process_key(msg);
			if(txt != list->item->item_name)
			{
				txt = list->item->item_name;
				send_message(WM_DRAW, 0, client_rect.as_int, this);
			}
			list->clr_flag(GO_FLG_SHOW|GO_FLG_SELECTED);
		}
		else
			list->set_flag(GO_FLG_SHOW|GO_FLG_SELECTED);
		return 1;
	case KEY_ESC:
	case KEY_CANCEL:
		if(list && (list->flags & GO_FLG_SHOW))
		{
			list->clr_flag(GO_FLG_SHOW|GO_FLG_SELECTED);
			if(align & LBS_DROPDOWN)
				return 0;
			return 1;
		}
		break;

	default:
		if(list && (list->flags & GO_FLG_SHOW))
			return list->process_key(msg);
		break;
	}
	return 0;
}
