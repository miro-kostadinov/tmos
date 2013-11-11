#include <tmos.h>
#include <gobject.h>
#include <lcd.h>
//#include <mqueue.h>
//
//extern mqueue<GMessage, MAX_MESSAGES> GQueue;


void GObject::clear_rect (const RECT_T& area)
{
	int y = area.y0;
    while(y <= area.y1)
        draw_bline(area.x0, area.x1, y++);
}

bool GObject::cut_hline ( int& x0, int& x1, int& y)
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

bool GObject::cut_vline ( int& y0, int& y1, int& x)
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

void GObject::set_xy_all(LCD_MODULE* lcd, unsigned int xy, unsigned int all)
{
	lcd->pos_y = xy + client_rect.y0;
	lcd->pos_x = (xy >> 16) + client_rect.x0;
	lcd->allign = all;
}

int GObject::draw_text (LCD_MODULE* lcd, const char *txt)
{
	if(!txt)
		return 0;

	unsigned int max_chars = (client_rect.x1 - lcd->pos_x - (lcd->font->hdistance << 1)) / lcd->font->hspacing;
	unsigned int len = 0, tmp_len = 0;
	unsigned int c, rows = 0;

	while ((c = txt[len++]) != '\0' && lcd->pos_y <= client_rect.y1)
    {
    	if (len == max_chars)
    	{
    		if (tmp_len)
    			len = tmp_len;
    		draw_text_line (lcd, txt, len);
    		txt += len;
    		len = tmp_len = 0;
    		rows++;
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
    		rows++;
    	}
    }
    if (lcd->pos_y <= client_rect.y1)
    	draw_text_line (lcd, txt, len);
    return (rows * lcd->font->vspacing);
}

void GObject::draw_text_line (LCD_MODULE* lcd, const char* txt, unsigned int len)
{
	int pos_x, c;

	if(!txt )
		return;

	if (len && (txt[len-1] == ' ' || txt[len-1] == '\0') )
		len--;

	if(!len)
		return;

	if((int)len > (pos_x =strlen(txt)))
		len = pos_x;

	switch ((lcd->allign & TA_HORIZONTAL))
	{
	case TA_LEFT:
		pos_x = lcd->pos_x; //+ lcd->font->hdistance;
		break;
	case TA_RIGHT:
		pos_x = client_rect.x1 /*- lcd->font->hdistance*/ - ((int)len * lcd->font->hspacing);
		break;
	case TA_CENTER:
		pos_x = ((client_rect.x1 + lcd->pos_x) - ((int)len * lcd->font->hspacing)) >> 1;
		break;
	}
	pos_x += lcd->font->hdistance;
	for (unsigned int i = 0; i < len && pos_x < client_rect.x1; i++)
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

void GObject::draw (LCD_MODULE* lcd, RECT_T area)
{
	if ((flags & GO_FLG_SHOW) && area.normalize(rect))							//cuts the area to the size of the object
	{
		lcd->redraw_rect(this, area);											//draws itself
	}
}

void GObject::invalidate(GObject* object, RECT_T area)
{
	if (area)
	{
		if(flags & GO_FLG_SHOW)
			area.normalize(rect);													//cuts the area to the size of the object
		if(area)
			parent->invalidate(this, area);											//calls invalidate to the parent with this as parameter
	}
}


void GObject::allocate_border(void)
{
	client_rect.x0 = rect.x0 +3;
	client_rect.y0 = rect.y0 +3;
	client_rect.x1 = rect.x1 -3;
	client_rect.y1 = rect.y1 -3;
}


void GObject::draw_poligon(RECT_T& frame, bool fill)
{
	if(fill)
	{
		invert_hline(frame.x0 +2, frame.x1 -2, frame.y0); // top line
		invert_hline(frame.x0 +1, frame.x1 -1, frame.y0+1);
		for(int i=frame.y0 +2; i <= frame.y1 -2; i++)
			invert_hline(frame.x0, frame.x1, i);
		invert_hline(frame.x0+1, frame.x1 -1, frame.y1-1);
		invert_hline(frame.x0 +2, frame.x1 -2, frame.y1); // bottom line

	}else
	{
		draw_hline(frame.x0 +2, frame.x1 -2, frame.y0); // top line
		draw_hline(frame.x0 +2, frame.x1 -2, frame.y1); // bottom line
		draw_vline(frame.y0 +2, frame.y1 -2, frame.x0); // left
		draw_vline(frame.y0 +2, frame.y1 -2, frame.x1); // right

		draw_point(frame.x0+1, frame.y0+1);
		draw_point(frame.x1-1, frame.y0+1);
		draw_point(frame.x0+1, frame.y1-1);
		draw_point(frame.x1-1, frame.y1-1);
	}
}

void GObject::draw_border(RECT_T& frame)
{
	if(frame.width() >= 4 && frame.height() >= 4 )
		draw_poligon(frame);
	else
	{
		draw_hline(frame.x0, frame.x1, frame.y0); // top line
		draw_hline(frame.x0, frame.x1, frame.y1); // bottom line
		draw_vline(frame.y0, frame.y1, frame.x0); // left
		draw_vline(frame.y0, frame.y1, frame.x1); // right
	}
}

void GObject::draw_point( int x, int y)
{
	if ((int)y >= rect.y0 && (int)y <= rect.y1 &&
		(int)x >= rect.x0 && (int)x <= rect.x1)
		parent->draw_point(x,y);
}

void  GObject::draw_line(int x1, int y1, int x2, int y2)
{
	int x, y;
	int xs, xe, ys, ye;
	int a, b;

	if(x1 > x2)
		xs = x2, xe = x1;
	else
		xs = x1, xe = x2;
	if(y1 > y2)
		ys = y2, ye = y1;
	else
		ys = y1, ye = y2;
	if (ys >= rect.y0 && ye <= rect.y1 &&
		xs >= rect.x0 && xe <= rect.x1)
	{
		if(xs == xe)
		{
			draw_vline(ys, ye, xs);
			return;
		}
		if(ys == ye)
		{
			draw_hline(xs, xe, ys);
			return;
		}


		if((ye - ys) < (xe-xs))
		{
			a = ((y1-y2)*10000)/(x1-x2);
			b = y1*10000 - a*x1;
			while(xs <= xe )
			{
				y = (a*xs + b)/10000;
				parent->draw_point(xs++, y);
			}
		}
		else
		{
			a = ((x1-x2)*10000)/(y1-y2);
			b = x1*10000 - a*y1;
			while(ys <= ye )
			{
				x = (a*ys + b)/10000;
				parent->draw_point(x, ys);
			}
		}
	}
}

void GObject::draw_hline ( int x0, int x1, int y)
{
	if (cut_hline (x0, x1, y))
		parent->draw_hline(x0, x1, y);
}

void GObject::draw_bline( int x0, int x1, int y)
{
	if (cut_hline (x0, x1, y))
		parent->draw_bline(x0, x1, y);
}

void GObject::draw_vline( int y0, int y1, int x)
{
	if (cut_vline (y0, y1, x))
		parent->draw_vline(y0, y1, x);
}

void GObject::invert_vline( int y0, int y1, int x)
{
	if (cut_vline (y0, y1, x))
		parent->invert_vline(y0, y1, x);
}

void GObject::invert_hline( int x0, int x1, int y)
{
	if (cut_hline (x0, x1, y))
		parent->invert_hline(x0, x1, y);
}

void GObject::send_message(WM_MESSAGE wm,
		unsigned int param, unsigned long long lparam, GObject* dst) const
{
	GQueue.push(GMessage(wm, param, lparam, dst));
}

unsigned int GObject::message (GMessage& msg)
{
	switch (msg.code)
	{
	case WM_INIT:
		return initialize(msg);
	case WM_KEY:
		return process_key (msg);
	case WM_COMMAND:
		return process_command(msg);
	case WM_DRAW:
		if(msg.lparam)
			msg.dst->invalidate(msg.dst, (RECT_T&)msg.lparam);
		else
			msg.dst->invalidate(msg.dst, msg.dst->rect);
		return RES_OK;
	case WN_DESTROY:
		return process_destroy(msg);
	case WM_SET_FLAGS:
		return set_flag(msg.param);
	case WM_CLR_FLAGS:
		return clr_flag(msg.param);
	case WM_IDLE:
		return process_idle(msg);
	default:
		return process_default (msg);
	}
}


unsigned int GObject::initialize(GMessage& msg)
{
	client_rect = rect;
	if(flags & GO_FLG_BORDER)
		allocate_border();

	if((flags & GO_FLG_SELECTED) && parent)
		parent->focus = this;
	return 1;
}

unsigned int GObject::process_destroy(GMessage& msg)
{
	return close();
}

bool GObject::get_focus()
{
	if(flags & GO_FLG_ENABLED)
	{
		if (parent)
		{
			if (parent->focus)
			{
				parent->focus->clr_flag(GO_FLG_SELECTED);
			}
			parent->focus = this;
			set_flag(GO_FLG_SELECTED);
			return true;
		}
	}
	return false;
}

bool GObject::set_flag(GFlags val)
{
	if((flags & val) != val )
	{
		flags |= val;
		if(val & GO_FLG_BORDER)
		{
			allocate_border();
		}
		send_message(WM_DRAW, 0, 0L, this);
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
		send_message(WM_DRAW, 0, 0L, this);
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

GObject* GObject::get_object(GId xid)
{
	if(id == xid)
		return this;
	else
	{
		if(parent)
			return parent->get_object(xid);
	}
	return NULL;
}

bool GObject::is_available()
{
	if ((flags & (GO_FLG_ENABLED|GO_FLG_SHOW)) == (GO_FLG_ENABLED|GO_FLG_SHOW))
		return true;
	return false;
}

//GObject* GObject::next_available()
//{
//	GObject* tmp = nextObj;
//	while(tmp)
//	{
//		if(tmp->is_available())
//			return tmp;
//		tmp = tmp->nextObj;
//	}
//	if(parent)
//		return parent->nextObj;
//	return tmp;
//}

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


text_metrics_t get_text_metrics(const char* text, short int x_size, const RENDER_MODE* font )
{
	text_metrics_t size;
	size.as_int=0;
	const char* ptr = text;

	if(!text)
		return size;
	if(x_size == -1)
		x_size = 0x7FFF;
	int max_chars = (x_size - (font->hdistance << 1)) / font->hspacing;
	int len = 0, tmp_len = 0;
	unsigned int c;

	while ((c = text[len++]) != '\0')
    {
    	if (len == max_chars)
    	{
    		if (tmp_len)
    			len = tmp_len;
    		size.height++;
   			size.width += len;
    		text += len;
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
    		size.height++;
   			size.width += len-1;
    		text += len;
    		len = tmp_len = 0;
    	}
    }
    if (len)
    {
		size.height++;
		size.width += len -1;
    }
    if(	x_size == 0x7FFF )
    {
    	size.width = strlen(ptr) * font->hspacing;
    	size.height = font->vspacing;
    }
    else
    {
    	size.width *= font->hspacing;
    	size.height *= font->vspacing;
    }
    return (size);

}
