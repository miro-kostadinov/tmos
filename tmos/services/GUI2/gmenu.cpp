/*
 * gmenu.cpp
 *
 *  Created on: 03.10.2013
 *      Author: stanly
 */

#include <tmos.h>
#include <lcd.h>
#include <gmenu.h>


menu_template_t* GMenu::GetItem(int parent_id, int item_id)
{
	menu_template_t* tmp = base;
	if(tmp)
	{
		while(!tmp->item_name.empty())
		{
			if(tmp->parent == parent_id && tmp->item == item_id )
				return tmp;
			tmp++;
		}
	}
	return NULL;
}

menu_template_t* GMenu::FindItem(int item_id)
{
	menu_template_t* tmp = base;
	if(tmp)
	{
		while(!tmp->item_name.empty())
		{
			if(tmp->item == item_id)
				return tmp;
			tmp++;
		}
	}
	return NULL;
}

menu_template_t* GMenu::GetMenu(int parent_id, menu_template_t* start)
{
	menu_template_t* tmp = start;
	if(!start)
		tmp = base;
	if(tmp)
	{
		while(!tmp->item_name.empty())
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
	menu_template_t* tmp = base;
	int res = 0;
	if(tmp)
	{
		while(!tmp->item_name.empty())
		{
			if(tmp->parent == menu_id)
				++res;
			++tmp;
		}
	}
	return res;
}

void GMenu::adjust_item_names()
{
	menu_template_t* tmp = menu;
	while(tmp)
	{
		if(tmp && client_rect.width() && !tmp->item_name.empty())
		{
			CSTRING name = tmp->item_name;
			unsigned int pos = remove_amp(name);
			char* ptr = strchr(name.c_str(), '\t');
			if(ptr)
			{
				CSTRING l_str, r_str;
				int max_chars = (client_rect.width() - (text_font->hdistance << 1)) / text_font->hspacing;
				int l_size = ptr - name.c_str()+1;
				l_str.assign(name.c_str(), l_size);

				while(*ptr && IS_SPACE(*ptr))
					ptr++;
				r_str = ptr;
				int r_size = r_str.length();

				if(l_size + r_size >= max_chars)
				{
					if(r_size < max_chars)
					{
						l_size = max_chars - r_size;
						l_str.erase(l_size, -1u);
					}
					else
					{
						r_size = max_chars;
						l_size =0;
						l_str.clear();
					}
				}
				max_chars -= l_size + r_size;
				while(max_chars)
				{
					l_str +=' ';
					max_chars--;
				}
				l_str += r_str;
				if(pos)
					l_str.insert('&', pos-1);
				tmp->item_name = l_str;
			}
		}
		tmp = GetMenu(tmp->parent, tmp+1);
	}
}

bool GMenu::add_item(int parent_id, int item_id, const CSTRING& name)
{
	menu_template_t * new_base;

	new_base = (menu_template_t *)tsk_malloc_clear((size +2)*sizeof(menu_template_t));
	if(!new_base)
		return false;
	if(base)
	{
		for(int i=0; i<size; i++)
		{
			new_base[i].parent = base[i].parent;
			new_base[i].item = base[i].item;
			new_base[i].item_name = base[i].item_name;
			base[i].item_name.free();
		}
		tsk_free(base);
	}
	base = new_base;
	base[size].parent = parent_id;
	base[size].item = item_id;
	base[size].item_name = name;

	size++;
	item = menu = base;
	return true;
}

int GMenu::get_item_pos(menu_template_t* ptr)
{
	int res=0;
	menu_template_t*  start = GetMenu(ptr->parent);
	while(start)
	{
		if(ptr->item == start->item)
			break;
		++res;
		start = GetMenu(ptr->parent, start +1);
	}
	return res;
}

bool GMenu::SetReplaceItem(int item_id, const CSTRING& item_name)
{
	menu_template_t* ptr;
	ptr = GetItem(0,item_id);
	if(ptr)
	{
		ptr->item_name = item_name;
		return true;
	}
	return	AppendMenu(0, item_id, item_name);
}
bool GMenu::RemoveItem(int item_id)
{
	menu_template_t * new_base;
	menu_template_t* ptr;

	ptr = FindItem(item_id);
	if(!ptr || !base)
		return true;
	if(GetMenu(ptr->item))
		RemoveItem(ptr->item);
//	int pos = ptr - base;

	new_base = (menu_template_t *)tsk_malloc_clear((size +2)*sizeof(menu_template_t));
	if(!new_base)
		return false;

	if(item == ptr)
	{
		menu_template_t* tmp;
		// remove selected
		tmp = GetMenu(item->parent, ptr+1);
		if(tmp)
		{
			item = tmp; // select next
			menu = GetMenu(item->parent);
		}
		else
		{
			if(menu && item && (item != menu))
			{
				menu_template_t* last;
				tmp =menu;
				do
				{
					last = tmp;
					tmp = GetMenu(item->parent, tmp +1);
				} while(tmp && (tmp != item));
				if(tmp)
					item =last;
				else
					item = menu = base;
			}
			else
				item = menu = base;
		}
	}

	for(int i=0, j=0; !IsEmpty(base + i); i++)
	{
		if(base +i == ptr)
		{
			base[i].item_name.free();
			continue;
		}
//		if(i==pos)
//		{
//			if(menu == base+i || item == base+i)
//				item=menu=new_base;
//			base[i].item_name.free();
//			continue;
//		}
		new_base[j].parent = base[i].parent;
		new_base[j].item = base[i].item;
		new_base[j].item_name = base[i].item_name;
		base[i].item_name.free();

		if(item == base+i)
			item = new_base +j;
		j++;
	}
	tsk_free(base);
	base = new_base;
	size = GetMenuSize(item->parent);
	menu = GetMenu(item->parent);
	set_scroll();
	return true;
}

bool GMenu::Select(int item_id)
{
	menu_template_t* ptr = FindItem(item_id);
	if(ptr)
	{
		item = ptr;
		menu = GetMenu(item->parent);
		size = GetMenuSize(item->parent);
		return true;
	}
	return false;
}

bool GMenu::AppendMenu( int parent_id, int item_id, const CSTRING& item_name)
{
	if(!parent_id)
	{
		if(!base)
		{
			if(!add_item(parent_id, item_id, item_name))
				return false;
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
	menu_template_t* p= (menu_template_t*)pat;
	if(p)
	{
		while(!p->item_name.empty() && p->item)
		{
			if( ! AppendMenu(p->parent, p->item, p->item_name) )
				return false;
			p++;
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
	size = GetMenuSize(0);
	flags &= ~GO_FLG_VSCROLL; // forced allocation of the scroll bar
	set_scroll();
	if(msg.param)
		send_message(WM_DRAW, 0, 0L, this);
	return 1;
}

void GMenu::draw_this (LCD_MODULE* lcd)
{
	int menu_height;
	int sy, ey;
	int rows;
	int row_height;
	CSTRING str;
	menu_template_t* tmp;

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
		if( !(rows & 1) && rows > size )
			rows--;

		sy = get_item_pos(item);
		if(sy >= rows/2)
		{
			sy -= rows/2;
			if(sy + rows >= size && size >= rows )
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
				if(str.length())
					draw_text_line(lcd, str.c_str(), str.length());
				else
					lcd->pos_y += text_font->vspacing;
				if(tmp==item && (flags & GO_FLG_SELECTED))
				{
					for (int i = lcd->pos_y - row_height; i < lcd->pos_y; i++)
						invert_hline (client_rect.x0, client_rect.x1, i);
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
		menu_template_t* next_menu = GetMenu(item->item);
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
			set_scroll();
			send_message(WM_DRAW, 0, 0L, this);
		}
		return true;
	}
	return false;
}

unsigned int GMenu::process_key (GMessage& msg)
{
	menu_template_t* tmp;
	switch (msg.param)
	{
	case KEY_RIGHT:
		tmp = GetMenu(item->item);
		if( tmp )
		{
			menu = tmp;
			item = tmp;
			size = GetMenuSize(item->parent);
			if(set_scroll())
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
			if(set_scroll())
				send_message(WM_DRAW, 0, 0L, this);
		}
		return 1;

	case KEY_UP:
		if(menu && item && (item != menu))
		{
			menu_template_t* last;

			tmp = menu;
			do
			{
				last = tmp;
				tmp = GetMenu(item->parent, tmp +1);
			} while(tmp && (tmp != item));
			if(tmp)
			{
				item =last;
				if(scroll)
					scroll->SetScrollPos(GO_FLG_VSCROLL, get_item_pos(last));
				send_message(WM_DRAW, client_rect.as_int, 0L, this);
				return 1;
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
					hot_pos = strchr(tmp->item_name.c_str(), '&');
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

	if( !(rows & 1) && rows > size )
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
	adjust_item_names();
	return res;
}
