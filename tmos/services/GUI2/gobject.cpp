#include <tmos.h>
#include <gobject.h>
#include <lcd.h>
#include <mqueue.h>

extern mqueue<GMessage, MAX_MESSAGES> GQueue;

unsigned int GObject::initialize(GMessage msg)
{
	if(flags & GO_FLG_BORDER)
	{
		client_rect.x0 = rect.x0 +1;
		client_rect.y0 = rect.y0 +1;
		client_rect.x1 = rect.x1 -1;
		client_rect.y1 = rect.y1 -1;
	}
	else
		client_rect = rect;
	if((flags & GO_FLG_SELECTED) && parent)
		parent->focus = this;
	return 1;
}

void GObject::send_message(GMessage msg)
{
	GQueue.push(msg);
}

void GObject::invalidate(GObject* object, RECT_T area)
{
	if (area.normalize(rect))													//cuts the area to the size of the object
		parent->invalidate(this, area);											//calls invalidate to the parent with this as parameter
}

void GObject::draw (LCD_MODULE* lcd, RECT_T area)
{
	if (area.normalize(rect))													//cuts the area to the size of the object
		lcd->redraw_rect(this, area);											//draws itself
}

void GObject::clear_rect (unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    while(y0 <= y1)
    {
        draw_bline(x0, x1, y0++);
    }
}

void GObject::draw_border(RECT_T& frame)
{
	draw_hline(frame.x0, frame.x1, frame.y0);
	draw_hline(frame.x0, frame.x1, frame.y1);
	draw_vline(frame.y0 + 1, frame.y1 - 1, frame.x0);
	draw_vline(frame.y0 + 1, frame.y1 - 1, frame.x1);
}

bool GObject::cut_hline (unsigned int& x0, unsigned int& x1, unsigned int& y)
{
	if ((int)y < rect.y0 || (int)y > rect.y1 || (int)x0 > rect.x1)
		return false;
	if ((int)x0 < rect.x0)
		x0 = rect.x0;
	if (x1 < x0)
		return false;
	if ((int)x1 > rect.x1)
		x1 = rect.x1;
	return true;
}

bool GObject::cut_vline (unsigned int& y0, unsigned int& y1, unsigned int& x)
{
	if ((int)x < rect.x0 || (int)x > rect.x1 || (int)y0 > rect.y1)
		return false;
	if ((int)y0 < rect.y0)
		y0 = rect.y0;
	if (y1 < y0)
		return false;
	if ((int)y1 > rect.y1)
		y1 = rect.y1;
	return true;
}

void GObject::draw_hline (unsigned int x0, unsigned int x1, unsigned int y)
{
	if (cut_hline (x0, x1, y))
		parent->draw_hline(x0, x1, y);
}

void GObject::draw_bline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if (cut_hline (x0, x1, y))
		parent->draw_bline(x0, x1, y);
}

void GObject::draw_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if (cut_vline (y0, y1, x))
		parent->draw_vline(y0, y1, x);
}

void GObject::invert_vline(unsigned int y0, unsigned int y1, unsigned int x)
{
	if (cut_vline (y0, y1, x))
		parent->invert_vline(y0, y1, x);
}

void GObject::invert_hline(unsigned int x0, unsigned int x1, unsigned int y)
{
	if (cut_hline (x0, x1, y))
		parent->invert_hline(x0, x1, y);
}

void GObject::set_xy_all(LCD_MODULE* lcd, unsigned int xy, unsigned int all)
{
	lcd->pos_y = xy + client_rect.y0;
	lcd->pos_x = (xy >> 16) + client_rect.x0;
	lcd->allign = all;
}

//void GObject::set_xy_on_char (LCD_MODULE* lcd, const char *txt, unsigned int index)
//{
//    unsigned int max_chars, len, tmp_len;
//	unsigned int c;
//
//	if(!txt)
//	{
//		lcd->pos_x = client_rect.x0;
//		lcd->pos_y = client_rect.y0;
//		return;
//	}
//	max_chars = (client_rect.x1 - lcd->pos_x - (lcd->font->hdistance << 1)) / lcd->font->hspacing;
//	len = 0;
//    tmp_len = 0;
//    while ((c = txt[len++]) != '\0')
//    {
//    	if (txt[0] == ' ')
//    	{
//    		txt++;
//    		len--;
//    		continue;
//    	}
//    	if (len == max_chars)
//    	{
//    		if (tmp_len)
//    			len = tmp_len;
//    		if ((signed int)(index - len) < 0)
//    			break;
//    		else
//    			index -= len;
//    		lcd->pos_y += lcd->font->vspacing;
//    		txt += len;
//    		len = tmp_len = 0;
//    		continue;
//    	}
//    	if (c == ' ' || c == '-')
//    	{
//    		tmp_len = len;
//    		continue;
//    	}
//    	if (c == '\n' || c == '\r')
//    	{
//    		if ((signed int)(index - len) < 0)
//    			break;
//    		else
//    			index -= len;
//    		lcd->pos_y += lcd->font->vspacing;
//    		txt += len;
//    		len = tmp_len = 0;
//    		continue;
//    	}
//    }
//    lcd->pos_x = client_rect.x0 + lcd->font->hdistance + lcd->font->hspacing * index;
//}

const char* GObject::draw_text (LCD_MODULE* lcd, const char *txt)
{
    unsigned int max_chars, len, tmp_len;
	unsigned int c;

	if(!txt)
		return NULL;
	max_chars = (client_rect.x1 - lcd->pos_x - (lcd->font->hdistance << 1)) / lcd->font->hspacing;
	len = 0;
    tmp_len = 0;
    while ((c = txt[len++]) != '\0' && lcd->pos_y <= client_rect.y1)
    {
    	if (len == max_chars)
    	{
    		if (tmp_len)
    			len = tmp_len;
    		draw_text_line (lcd, txt, len);
    		txt += len;
    		len = tmp_len = 0;
    		continue;
    	}
    	if (c == ' ' || c == '-')
    	{
    		tmp_len = len;
    		continue;
    	}
    	if (c == '\n' || c == '\r')
    	{
    		draw_text_line (lcd, txt, len - 1);
    		txt += len;
    		len = tmp_len = 0;
    	}
    }
    if (lcd->pos_y <= client_rect.y1)
    	draw_text_line (lcd, txt, len);
    return (txt);
}

void GObject::draw_text_line (LCD_MODULE* lcd, const char* txt, unsigned int len)
{
	int pos_x, c;
	if (txt[len-1] == ' ' || txt[len-1] == '\0' )
		len--;
	switch (lcd->allign)
	{
	case ALL_LEFT:
		pos_x = lcd->pos_x + lcd->font->hdistance;
		break;
	case ALL_RIGHT:
		pos_x = client_rect.x1 - lcd->font->hdistance - len * lcd->font->hspacing;
		break;
	case ALL_CENTER:
		pos_x = ((client_rect.x1 + lcd->pos_x) - (len * lcd->font->hspacing)) >> 1;
		break;
	}
	for (unsigned int i = 0; i < len && pos_x < lcd->size_x; i++)
	{
		if((c = txt[i]) > 0x1f)
		{
            c -= 0x20;

			lcd->draw_bitmap(pos_x, lcd->pos_y, lcd->font->font_data + c * lcd->font->char_bytes,
					lcd->font->width, lcd->font->height);
		}
		pos_x += lcd->font->hspacing;
	}
	lcd->pos_y += lcd->font->vspacing;
}

void GObject::get_focus()
{
	if (parent)
	{
		if (parent->focus)
		{
			parent->focus->clr_flag(GO_FLG_SELECTED);
		}
		parent->focus = this;
		set_flag(GO_FLG_SELECTED);
	}
}

bool GObject::set_flag(GFlags val)
{
	if((flags & val) != val )
	{
		flags |= val;
		if(val & GO_FLG_BORDER)
		{
			client_rect.x0 = rect.x0 +1;
			client_rect.y0 = rect.y0 +1;
			client_rect.x1 = rect.x1 -1;
			client_rect.y1 = rect.y1 -1;
		}
		send_message(GMessage(WM_DRAW, 0, 0L, this));
		return true;
	}
	return false;
}

bool GObject::clr_flag(GFlags val)
{
	if(flags & val)
	{
		flags &= ~val;
		if(val & GO_FLG_BORDER)
		{
			client_rect = rect;
		}
		send_message(GMessage(WM_DRAW, 0, 0L, this));
		return true;
	}
	return false;
}

bool GObject::close()
{
	if (parent)
		return parent->close(this);
	return false;
}

/*-----------------------------------------------------------------------------
/				Line by line rectangle draw
/

void GObject::draw_rect (LCD_MODULE* lcd, RECT_T rect_t)
{
	GObject *first = this;
	while (first->parent)
	{
		if (first->parent->parent)
		{
			first = first->parent;
		}
		else
		{
			break;
		}
	}
	if (!rect_t.normalize(first->rect))
		return;
	if (!rect_t.normalize(0, 0, lcd->size_x, lcd->size_y))
		return;
	while (rect_t.y0 <= rect_t.y1)
	{
		short int from = rect_t.x0;
		short int to;
		while (from < rect_t.x1)
		{
			GObject* next = first;
			to =  rect_t.x1;
			while (next)
			{
			#if GUI_DISPLAYS > 1
				if (first->displays & next->displays & lcd->display)
			#endif
				{
					if (from >= next->rect.x0 && from <= next->rect.x1)
						from = next->rect.x1 + 1;
					if (from < next->rect.x0 && to >= next->rect.x0)
						to = next->rect.x0 - 1;
				}
				next = next->nextObj;
			}
			if (from > to)
				break;
			draw_object (lcd, from, rect_t.y0, to, rect_t.y0);
			from = to + 1;
		}
		rect_t.y0++;
	}
}
*/

/*-----------------------------------------------------------------------------
/				Recursive rectangle draw
/


void GObject::draw_rect (LCD_MODULE* lcd, RECT_T rect_t, GObject* first)
{
	while (first->parent)
	{
		if (first->parent->parent)
		{
			first = first->parent;
		}
		else
		{
			break;
		}
	}
	rect_t.normalize(first->rect);
	rect_t.normalize(0, 0, lcd->size_x, lcd->size_y);
	if (!first->nextObj)
	{
		draw_rect (lcd, rect_t.x0, rect_t.y0, rect_t.x1, rect_t.y1);
		return;
	}
	first = first->nextObj;
	if (rect_t.x0 >= first->rect.x0 && rect_t.x1 <= first->rect.x1 && rect_t.y0 >= first->rect.y0 && rect_t.y1 <= first->rect.y1)
	{
		return;
	}
	if (rect_t.x1 < first->rect.x0 || rect_t.x0 < first->rect.x1)
	{
		draw_rect (lcd, rect_t.x0, rect_t.y0, rect_t.x1, rect_t.y1, first);
		return;
	}
	if (rect_t.y0 < first->rect.y0)
	{
		if (rect_t.y1 < first->rect.y0)
		{
			draw_rect (lcd, rect_t.x0, rect_t.y0, rect_t.x1, rect_t.y1, first);
		}
		else
		{
			draw_rect (lcd, rect_t.x0, rect_t.y0, rect_t.x1, first->rect.y0, first);
			if (rect_t.y1 > first->rect.y1)
			{
				draw_rect (lcd, rect_t.x0, first->rect.y1, rect_t.x1, rect_t.y1, first);
				if (rect_t.x0 < first->rect.x0)
				{
					draw_rect (lcd, rect_t.x0, first->rect.y0, first->rect.x0, first->rect.y1, first);
				}
				if (rect_t.x1 > first->rect.x1)
				{
					draw_rect (lcd, first->rect.x1, first->rect.y0, rect_t.x1, first->rect.y1, first);
				}
			}
			else
			{
				if (rect_t.x0 < first->rect.x0)
				{
					draw_rect (lcd, rect_t.x0, first->rect.y0, first->rect.x1, rect_t.y1, first);
				}
				if (rect_t.x1 > first->rect.x1)
				{
					draw_rect (lcd, first->rect.x1, first->rect.y0, rect_t.x1, rect_t.y1, first);
				}
			}
		}
	}
	else
	{
		if (rect_t.y0 > first->rect.y1)
		{
			draw_rect (lcd, rect_t.x0, rect_t.y0, rect_t.x1, rect_t.y1, first);
		}
		else
		{
			draw_rect (lcd, rect_t.x0, first->rect.y1, rect_t.x1, rect_t.y1, first);
			if (rect_t.x0 < first->rect.x0)
			{
				draw_rect (lcd, rect_t.x0, rect_t.y0, first->rect.x0, first->rect.y1, first);
			}
			if (rect_t.x1 > first->rect.x1)
			{
				draw_rect (lcd, first->rect.x1, rect_t.y0, rect_t.x1, first->rect.y1, first);
			}
		}
	}


}

-----------------------------------------------------------------------------*/

unsigned int GObject::message (GMessage msg)
{
	switch (msg.code)
	{
	case WM_INIT:
		return initialize(msg);
	case WM_KEY:
		return process_key (msg);
	case WM_USER:
		return process_user (msg);
	case WM_COMMAND:
		return process_command(msg);
	case WM_DRAW:
		if(msg.lparam)
			msg.dst->invalidate(msg.dst, RECT_T(msg.lparam));
		else
			msg.dst->invalidate(msg.dst, msg.dst->rect);
		return RES_OK;
	case WM_IDLE:
		return process_idle(msg);
	default:
		return process_default (msg);
	}
}

