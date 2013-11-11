/*
 * gmenu.cpp
 *
 *  Created on: 03.10.2013
 *      Author: stanly
 */

#include <tmos.h>
#include <lcd.h>
#include <gmenu.h>


MENUTEMPLATE* GMenu::GetItem(int parent_id, int item_id)
{
	MENUTEMPLATE* tmp = base;
	if(tmp)
	{
		while(tmp->item_name)
		{
			if(tmp->parent == parent_id && tmp->item == item_id )
				return tmp;
			tmp++;
		}
	}
	return NULL;
}

MENUTEMPLATE* GMenu::FindItem(int item_id)
{
	MENUTEMPLATE* tmp = base;
	if(tmp)
	{
		while(tmp->item_name)
		{
			if(tmp->item == item_id)
				return tmp;
			tmp++;
		}
	}
	return NULL;
}

MENUTEMPLATE* GMenu::GetMenu(int parent_id, MENUTEMPLATE* start)
{
	MENUTEMPLATE* tmp = start;
	if(!start)
		tmp = base;
	if(tmp)
	{
		while(tmp->item_name)
		{
			if(tmp->parent == parent_id)
				return tmp;
			tmp++;
		}
	}
	return NULL;
}

int GMenu::GetMenuSize(int menu_id)
{
	MENUTEMPLATE* tmp = base;
	int res = 0;
	if(tmp)
	{
		while(tmp->item_name)
		{
			if(tmp->parent == menu_id)
				++res;
			++tmp;
		}
	}
	return res;
}

bool GMenu::add_item(int parent_id, int item_id, const char* name)
{
	base = (MENUTEMPLATE *)tsk_realloc(base, (size +2)*sizeof(MENUTEMPLATE));
	if(!base)
		return false;
	base[size].parent = parent_id;
	base[size].item = item_id;
	base[size].item_name = name;

	memclr(&base[++size], sizeof(MENUTEMPLATE));
	return true;
}

int GMenu::get_item_pos(MENUTEMPLATE* ptr)
{
	int res=0;
	MENUTEMPLATE*  start = GetMenu(ptr->parent);
	while(start)
	{
		if(ptr->item == start->item)
			break;
		++res;
		start = GetMenu(ptr->parent, start +1);
	}
	return res;
}

bool GMenu::AppendMenu( int parent_id, int item_id, const char* item_name)
{
	if(!parent_id)
	{
		if(!base)
		{
			if(!add_item(parent_id, item_id, item_name))
				return false;
			item = menu = base;
		}
		else
		{
			if(GetItem(0, item_id))
				return false;
			if(!add_item(parent_id, item_id, item_name))
				return false;
		}
		return true;
	}
	else
	{
		if(!menu ||GetItem(parent_id, item_id))
			return false;
		if(add_item(parent_id, item_id, item_name))
			return true;
	}
	return false;
}

bool GMenu::LoadMenu(const MENUTEMPLATE* pat)
{
	if(pat)
	{
		while(pat->item_name && pat->item)
		{
			if( ! AppendMenu(pat->parent, pat->item, pat->item_name) )
				return false;
			pat++;
		}
	}
	return true;
}

unsigned int remove_amp(CSTRING& str)
{
	unsigned int pos;
	if(str.find(pos, '&'))
	{
		str.erase(pos, 1);
		++pos;
	}
	else
		pos = 0;
	return pos;
}

unsigned int GMenu::initialize (GMessage& msg)
{
	GObject::initialize(msg);
	if(title)
		client_rect.y0 += text_font->vspacing + 2*text_font->vdistance;
	client_rect.x0 += text_font->hdistance;
	client_rect.y0 += text_font->vdistance;
	item = menu = base;
	size = GetMenuSize(0);
	set_scroll();
	return 1;
}

void GMenu::draw_this (LCD_MODULE* lcd)
{
	int menu_height;
	int sy, ey;
	int rows;
	int row_height;
	CSTRING str;
	MENUTEMPLATE* tmp;

	lcd->color = PIX_WHITE;

	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	if(item && menu)
	{

		if(scroll)
			scroll->draw_scroll(lcd);

		lcd->color = PIX_WHITE;
		lcd->allign = TA_CENTER;
		lcd->set_font(text_font);

		// draw title
		if(!item->parent)
			str = title;
		else
		{
			tmp = FindItem(menu->parent);
			if(tmp)
			{
				str = tmp->item_name;
				remove_amp(str);
			}
		}
		menu_height = client_rect.height() - (2*text_font->vdistance);
		if(!str.empty())
		{
			lcd->pos_x = client_rect.x0 - text_font->hspacing;
			lcd->pos_y = client_rect.y0 - text_font->vspacing;
			RECT_T rc(client_rect);
			client_rect = rect;
			draw_text(lcd, str.c_str());
			draw_hline(	rect.x0 +rect.width()/8,
						rect.x1 -rect.width()/8,
						lcd->pos_y);
			client_rect = rc;
		}

		// draw items

		row_height = text_font->vspacing + text_font->vdistance;
		rows = menu_height / row_height;
		if( !(rows & 1) && rows > 2 )
			rows--;

		sy = get_item_pos(item);
		if(sy >= rows/2)
		{
			sy -= rows/2;
			if(sy + rows >= size)
			{
				sy = size - rows;
			}
		}
		else
			sy = 0;
		ey = sy + rows;


		tmp = menu;
		lcd->allign = TA_LEFT;
		lcd->pos_x = client_rect.x0;
		lcd->pos_y = client_rect.y0 + (2*text_font->vdistance) + (menu_height - (rows*row_height))/2;

		while(tmp)
		{
			int y = get_item_pos(tmp);
			if( y >= sy && y < ey )
			{
				str = tmp->item_name;
				rows = remove_amp(str);
				draw_text(lcd, str.c_str());
				if(tmp==item)
				{
					for (int i = lcd->pos_y - row_height; i < lcd->pos_y; i++)
						invert_hline (client_rect.x0, client_rect.x1-1, i);
				}
				if(rows)
				{
					rows = lcd->pos_x + (rows-1)*text_font->hspacing;
					draw_hline(rows, rows + text_font->hspacing, lcd->pos_y- text_font->vdistance);
				}
				lcd->pos_y += text_font->vdistance;
			}
			tmp = GetMenu(tmp->parent, tmp+1);
		}
	}
}

bool GMenu::process_selected()
{
	if(item)
	{
		MENUTEMPLATE* next_menu = GetMenu(item->item);
		if( !next_menu )
		{
			send_message(WM_DRAW, client_rect.as_int, 0L, this);
			send_message (WM_COMMAND, item->item, 0L, parent);
		}
		else
		{
			menu = next_menu;
			item = next_menu;
			size = GetMenuSize(menu->parent);
			if(!set_scroll())
				send_message(WM_DRAW, 0, 0L, this);
		}
		return true;
	}
	return false;
}

unsigned int GMenu::process_key (GMessage& msg)
{
	MENUTEMPLATE* tmp;
	switch (msg.param)
	{
	case KEY_RIGHT:
		tmp = GetMenu(item->item);
		if( tmp )
		{
			menu = tmp;
			item = tmp;
			size = GetMenuSize(item->parent);
			if(!set_scroll())
				send_message(WM_DRAW, 0, 0L, this);
		}
		return 1;

	case KEY_LEFT:
		tmp = FindItem(item->parent);
		if( tmp )
		{
			item = tmp;
			menu = GetMenu(item->parent);
			size = GetMenuSize(item->parent);
			if(!set_scroll())
				send_message(WM_DRAW, 0, 0L, this);
		}
		return 1;

	case KEY_UP:
		if(menu && item && item != menu)
		{
			tmp = menu;
			MENUTEMPLATE* last;
			while(tmp)
			{

				if(tmp == item)
				{
					item =last;
					if(scroll)
						scroll->SetScrollPos(GO_FLG_VSCROLL, get_item_pos(last));
					send_message(WM_DRAW, client_rect.as_int, 0L, this);
					return 1;
				}
				last = tmp;
				tmp = GetMenu(item->parent, tmp +1);
			}
		}
		return 1;

	case KEY_DOWN:
		tmp = GetMenu(item->parent, item +1);
		if( tmp )
		{
			item = tmp;
			if(scroll)
				scroll->SetScrollPos(GO_FLG_VSCROLL, get_item_pos(item));
			send_message(WM_DRAW, client_rect.as_int, 0L, this);
		}
		return 1;

	case KEY_OK:
		return process_selected();

	default:
		{
			char  ch = TranslateKey(msg.param);
			char* hot_pos;
			if(ch)
			{
				tmp = menu;
				while(tmp)
				{
					hot_pos = strchr(tmp->item_name, '&');
					if(hot_pos && hot_pos[1] == ch)
					{
						item = tmp;
						return process_selected();
					}
					tmp = GetMenu(menu->parent, tmp +1);
				}
			}
		}
		break;
	}
	return 0;
}

bool GMenu::set_scroll(void)
{
	bool res = false;
	int rows =	(client_rect.height() -
				((title)?(text_font->vspacing+2*text_font->vdistance):0)) /
				(text_font->vspacing + text_font->vdistance);

	if( !(rows & 1) && rows > 2 )
		rows--;

	if(rows < size && !scroll)
	{
		scroll = new GScroll(this);
	}
	if(scroll)
	{
		scroll->SetScrollRange(GO_FLG_VSCROLL, size);
		if(rows < size)
		{
			int tmp = get_item_pos(item);
			scroll->SetScrollPos(GO_FLG_VSCROLL, tmp);
			res = scroll->ShowScroll(GO_FLG_VSCROLL, true);
		}
		else
			res = scroll->ShowScroll(GO_FLG_VSCROLL, false);
	}
	return res;
}
