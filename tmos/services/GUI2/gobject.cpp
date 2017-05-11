#include <tmos.h>
#include <gobject.h>
#include <lcd.h>

GTimer* GTimer::base_timer = nullptr;

void* GObject::lastAllocated = nullptr;
uint8_t GObject::invalidate_cnt = 0;

GObject::~GObject()
{
}

bool GTimer::TimerProc(void)
{
	if(time_out && CURRENT_TIME >= time_out)
	{
		send_message(WM_TIMER, timer_id, 0L, object);
		time_out = 0;
		return true;
	}
	return false;
}

GTimer::~GTimer()
{
	GTimer *timer = base_timer;
	if(IS_NOT_NULL(this))
	{
		if(this == base_timer)
		{
			base_timer = base_timer->next;
		}
		else
		{
			while(timer->next)
			{
				if(timer->next == this)
				{
					timer->next = this->next;
					break;
				}
				timer = timer->next;
			}
		}
	}
}

void GTimer::RegisterTimer(void)
{
	if(base_timer)
		next = base_timer;
	base_timer = this;
}

bool process_timers(void)
{
	bool res = false;
	GTimer* timer = GTimer::base_timer;
	while(timer)
	{
		if(timer->TimerProc())
			res = true;
		timer = timer->next;
	}
	return res;
}

void GObject::KillObjectTimers(void)
{
	GTimer *timer = GTimer::base_timer;
	while(timer)
	{
		if(timer->object == this)
		{
			delete timer;
			timer = GTimer::base_timer;
		}
		else
			timer = timer->next;
	}
}

GTimer* GObject::FindTimer(GId event)
{
	GTimer *timer = GTimer::base_timer;
	while(timer)
	{
		if(timer->object == this && timer->timer_id == event)
			return timer;
		timer=timer->next;
	}
	return timer;

}

bool GObject::SetTimer(GId event, unsigned int elapse)
{
	GTimer* timer = FindTimer(event);
	if(!timer)
	{
		timer = new GTimer(event,this);
		if(timer)
			timer->RegisterTimer();
	}
	if(timer)
	{
		timer->time_out = CURRENT_TIME + elapse;
		return true;
	}
	return false;
}

void GObject::KillTimer(GId event)
{
	GTimer *del_timer = FindTimer(event);
	if(del_timer)
		delete del_timer;
}

bool GObject::IsActiveTimer(GId event)
{
	GTimer* timer = FindTimer(event);
	if(timer && timer->time_out)
		return true;
	return false;
}

bool GObject::StopTimer(GId event)
{
	GTimer* timer = FindTimer(event);
	if(timer)
	{
		if(timer->time_out)
		{
			timer->time_out =0;
			return true;
		}
	}
	return false;
}

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
    	if (c == ' ' || c == '-')
    		tmp_len = len;

    	if (len == max_chars)
    	{
    		if (tmp_len && txt[len] && !strchr(" -\n\r", txt[len]))
    			len = tmp_len;
    		draw_text_line (lcd, txt, len);
    		txt += len;
    		len = tmp_len = 0;
    		rows++;
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

//	if (len && (txt[len-1] == ' ' || txt[len-1] == '\0') )
	if (len && (strchr(" \r\n",txt[len-1]) || txt[len-1] == '\0'))
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

void GObject::redraw_screen(GObject* object, RECT_T area)
{
	// implemented by the LCD module
	if(area)
	{
		parent->redraw_screen(this, area);
	}
}

void GObject::invalidate(GObject* object, RECT_T area)
{
#if GUI_DEBUG
	uint32_t t;
	if(!invalidate_cnt++)
		t = CURRENT_TIME;
	GUI_TRACELN(">> Invalidate %u", invalidate_cnt);
#else
	invalidate_cnt++;
#endif
	if (area)
	{
		if(flags & GO_FLG_SHOW)
			area.normalize(rect);													//cuts the area to the size of the object
		if(area)
			parent->invalidate(this, area);											//calls invalidate to the parent with this as parameter
	}
	invalidate_cnt--;
#if GUI_DEBUG
	GUI_TRACELN("<< Invalidate %u", invalidate_cnt);
	if(!invalidate_cnt && area)
	{
		GUI_TRACELN("Invalidate {%u,%u %u,%u} %u ms", area.x0, area.y0, area.x1, area.y1, ms_since(t));
		parent->redraw_screen(this, area);
	}
#else
	if(!invalidate_cnt && area)
	{
		parent->redraw_screen(this, area);
	}
#endif
}


void GObject::draw_poligon(const RECT_T& frame, bool fill)
{
	if(frame.height() < 4 || frame.width() < 4)
	{
		draw_rectangle(frame, fill);
		return;
	}
	draw_hline(frame.x0 +2, frame.x1 -2, frame.y0); // top line
	draw_hline(frame.x0 +2, frame.x1 -2, frame.y1); // bottom line
	draw_vline(frame.y0 +2, frame.y1 -2, frame.x0); // left
	draw_vline(frame.y0 +2, frame.y1 -2, frame.x1); // right

	draw_point(frame.x0+1, frame.y0+1);
	draw_point(frame.x1-1, frame.y0+1);
	draw_point(frame.x0+1, frame.y1-1);
	draw_point(frame.x1-1, frame.y1-1);
	if(fill)
	{
		invert_hline(frame.x0 +2, frame.x1 -2, frame.y0+1); // top line
		for(int i=frame.y0 +2; i <= frame.y1 -2; i++)
			invert_hline(frame.x0+1, frame.x1-1, i);
		invert_hline(frame.x0 +2, frame.x1 -2, frame.y1-1);
	}
}

void GObject::draw_rectangle(const RECT_T& frame, bool fill)
{
	if(frame.height() && frame.width() )
	{
		draw_hline(frame.x0, frame.x1, frame.y0); // top line
		draw_hline(frame.x0, frame.x1, frame.y1); // bottom line
		draw_vline(frame.y0 +1, frame.y1 -1, frame.x0); // left
		draw_vline(frame.y0 +1, frame.y1 -1, frame.x1); // right
		if(fill)
		{
			for(int i=frame.y0 +1; i <= frame.y1 -1; i++)
				invert_hline(frame.x0+1, frame.x1-1, i);
		}
		return;
	}
	if(frame.height())
	{
		draw_vline(frame.y0, frame.y1, frame.x0);
		return;
	}
	if(frame.width())
	{
		draw_hline(frame.x0, frame.x1, frame.y0);
		return;
	}
	draw_point(frame.x0, frame.y0);
}

void GObject::allocate_border(void)
{
	if(rect.height() && rect.width())
	{
		POINT_T pt = get_border_size();
		client_rect = rect;
		client_rect.Inflate(pt.x, pt.y);
	}
}

POINT_T GObject::get_border_size(void)
{
	return POINT_T(GO_OBJ_FRAME_WIDTH, GO_OBJ_FRAME_HEIGHT);
}

void GObject::LPtoDP(POINT_T& size, unsigned char lcd_index)
{
	if(parent)
		parent->LPtoDP(size, lcd_index);
}

void GObject::DPtoLP(POINT_T& size, unsigned char lcd_index)
{
	if(parent)
		parent->DPtoLP(size, lcd_index);
}

void GObject::draw_border(RECT_T& frame)
{
	if(frame.width() >= 4 && frame.height() >= 4 )
		draw_poligon(frame);
	else
		draw_rectangle(frame);
}

void GObject::draw_point( int x, int y)
{
	if ((int)y >= rect.y0 && (int)y <= rect.y1 &&
		(int)x >= rect.x0 && (int)x <= rect.x1)
		parent->draw_point(x,y);
}

void  GObject::draw_line(int x0, int y0, int x1, int y1)
{

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err, e2;
	int dx, dy;

	dx = (sx == 1)?(x1 - x0):(x0 - x1);
	dy = (sy == 1)?(y1 - y0):(y0 - y1);

	err = (dx > dy ? dx : -dy) / 2;

	while(1)
	{
		draw_point(x0, y0);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void GObject::draw_circle(int x0, int y0, int radius, int sectors)
{
	int x = radius, y = 0;
	int radiusError = 1 - x;

	while (x >= y)
	{
		if( sectors & (1<<0) )
			draw_point( x + x0,  y + y0);
		if( sectors & (1<<1) )
			draw_point( y + x0,  x + y0);
		if( sectors & (1<<2) )
			draw_point(-y + x0,  x + y0);
		if( sectors & (1<<3) )
			draw_point(-x + x0,  y + y0);
		if( sectors & (1<<4) )
			draw_point(-x + x0, -y + y0);
		if( sectors & (1<<5) )
			draw_point(-y + x0, -x + y0);
		if( sectors & (1<<6) )
			draw_point( y + x0, -x + y0);
		if( sectors & (1<<7) )
			draw_point( x + x0, -y + y0);
		y++;
		if (radiusError < 0)
		{
			radiusError += 2 * y + 1;
		}
		else
		{
			x--;
			radiusError += 2 * (y - x + 1);
		}
	}
}

void GObject::draw_ellipse (int x0, int y0, int width, int height, int sectors)
{
    int a2 = width * width;
    int b2 = height * height;
    int fa2 = 4 * a2, fb2 = 4 * b2;
    int x, y, sigma;

    /* first half */
    for (x = 0, y = height, sigma = 2*b2+a2*(1-2*height); b2*x <= a2*y; x++)
    {
		if( sectors & (1<<1) )
			draw_point (x0 + x, y0 + y);
		if( sectors & (1<<2) )
			draw_point (x0 - x, y0 + y);
		if( sectors & (1<<6) )
			draw_point (x0 + x, y0 - y);
		if( sectors & (1<<5) )
			draw_point (x0 - x, y0 - y);
        if (sigma >= 0)
        {
            sigma += fa2 * (1 - y);
            y--;
        }
        sigma += b2 * ((4 * x) + 6);
    }

    /* second half */
    for (x = width, y = 0, sigma = 2*a2+b2*(1-2*width); a2*y <= b2*x; y++)
    {
		if( sectors & (1<<0) )
			draw_point (x0 + x, y0 + y);
		if( sectors & (1<<3) )
			draw_point (x0 - x, y0 + y);
		if( sectors & (1<<7) )
			draw_point (x0 + x, y0 - y);
		if( sectors & (1<<4) )
			draw_point (x0 - x, y0 - y);
        if (sigma >= 0)
        {
            sigma += fb2 * (1 - x);
            x--;
        }
        sigma += a2 * ((4 * y) + 6);
    }
}

void GObject::fill_circle(int x0, int y0, int radius)
{
	int x = radius, y = 0;
	int radiusError = 1 - x;

	while (x >= y)
	{
		draw_hline(-x + x0, x + x0, y +y0);
		draw_hline(-x + x0, x + x0, -y +y0);
		draw_hline(-y + x0, y + x0, x +y0);
		draw_hline(-y + x0, y + x0, -x +y0);
		y++;
		if (radiusError < 0)
		{
			radiusError += 2 * y + 1;
		}
		else
		{
			x--;
			radiusError += 2 * (y - x + 1);
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

int GObject::overlapped(GObject* obj, RECT_T& frame)
{
	int res = 0;
	if (obj && !(obj->flags & GO_FLG_TRANSPARENT) && (obj->flags & GO_FLG_SHOW))
	{
		if(frame.y1 <= rect.y0 || rect.y1 < frame.y0 )
			return 0x000;
		if(frame.x1 < rect.x0 || rect.x1 < frame.x0 )
			return 0x000;

		if(frame.y1 <= obj->rect.y0 || obj->rect.y1 < frame.y0 )
			return 0x000;
		if(frame.x1 < obj->rect.x0 || obj->rect.x1 < frame.x0 )
			return 0x000;

		if(obj->rect.y1 < rect.y0 || rect.y1 < obj->rect.y0)
			return 0x000;
		if(obj->rect.x1 < rect.x0 || rect.x1 < obj->rect.x0)
			return 0x000;

		RECT_T backup(rect.as_int);
		if(frame.x0 > rect.x0)
			rect.x0 = frame.x0;
		if(frame.x1 < rect.x1)
			rect.x1 = frame.x1;

		if (obj->rect.x0 > rect.x0)
		{
			//     |----------------|   		object
			//                      |-----|     object
			//    |----------------| 	 		this
			if (obj->rect.x0 > rect.x1)
			{
				// does not overlap
				res = 0x111;
			}
			else
			{
				//     |--                   		object
				//    |---                  		this

				//           |-----|				object
				//    |-------------------|			this
				//    |......|-----|......| 		x0 - x0  x1 << object x0

				//                |----------|		object
				//    |-------------------|			this
				//    |...........|-------| 		x0 - x0  x1 << object x0
				frame.x0 = rect.x0;
				frame.x1 = obj->rect.x0;

				if (obj->rect.x1 < rect.x1)
				{
					res = 0x010; // partially redraw left and right
				}
				else
				{
					res = 0x100; // partially redraw on left side
				}
			}
		}
		else
		{
			if (obj->rect.x1 < rect.x0)
			{
				//  |-----|                  		object
				//         |---------|       		this
				// does not overlap
				res = 0x111;
			}
			else
			{
				//  |-----                   		object
				//    |---                   		object
				//    |---                  		this
				if (obj->rect.x1 < rect.x1)
				{
					//    |-----|						object
					//    |-------------------|			this
					//    |-----|.............| 		x0 >> object x1  x1 - x1

					//  |----------|					object
					//    |-------------------|			this
					//    |--------|..........| 		x0 >> object x1  x1 - x1
					frame.x0 = obj->rect.x1;
					frame.x1 = rect.x1;
					res = 0x001; // partially redraw on right side
				}
				else
				{
					//  |-----------------------|		object
					//    |-------------------|  		object
					//    |-------------------|			this
					//    |-------------------| 		It no need to be redrawn
					res = 0x000;
				}
			}
		}
		rect = backup;
	}
	return res;
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

	if((flags & GO_FLG_SELECTED) && is_available() && parent)
		get_focus();
	return 1;
}

unsigned int GObject::process_destroy(GMessage& msg)
{
	return close();
}

unsigned int GObject::process_default (GMessage& msg)
{
	switch(msg.code)
	{
		case WM_SETFOCUS:
			return get_focus(false);
		default:
			if(parent)
				return parent->process_default(msg);
			break;
	}
	return 0;
}

bool GObject::get_focus(bool notify_msg)
{
	if(flags & GO_FLG_ENABLED)
	{
		if (parent)
		{
			if(parent->focus != this)
			{
				if (parent->focus)
				{
					parent->focus->clr_flag(GO_FLG_SELECTED);
					send_message(WM_KILLFOCUS, parent->focus->id, 0L, parent->focus);
				}
				parent->focus = this;
				if(notify_msg)
					send_message(WM_SETFOCUS, id, 0L, this);
				parent->move(0,0);
			}
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
	KillObjectTimers();
	GQueue.del_msg_for(this);
	if (parent)
		return parent->close(this);
	return false;
}

bool GObject::remove()
{
	KillObjectTimers();
	GQueue.del_msg_for(this);
	if (parent && parent->close(this))
	{
		GObject** pchild = &parent->children;
		GObject* child;

		while((child = *pchild))
		{
			if(child == this)
			{
				*pchild =  child->nextObj;
				return true;
			}
			pchild =  &child->nextObj;
		}
	}
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
	return nullptr;
}

bool GObject::is_available()
{
	if ((flags & (GO_FLG_ENABLED|GO_FLG_SHOW)) == (GO_FLG_ENABLED|GO_FLG_SHOW))
		return true;
	return false;
}

void GObject::move(int x, int y)
{
	rect.Offset(x, y);
	client_rect.Offset(x, y);
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

	if(!text || !text[0])
	{
    	size.height = font->vspacing;
		return size;
	}
	if(x_size == -1)
		x_size = 0x7FFF;
	int max_chars = (x_size - (font->hdistance << 1)) / font->hspacing;
	int len = 0, tmp_len = 0;
	unsigned int c;

	while ((c = text[len++]) != '\0')
    {
    	if (c == ' ' || c == '-')
    		tmp_len = len;

    	if (len == max_chars)
    	{
    		if (tmp_len && text[len] && !strchr(" -\n\r", text[len]))
    			len = tmp_len;
    		size.height++;
    		if(size.width < len)
    			size.width = len;
    		text += len;
    		len = tmp_len = 0;
    		continue;
    	}
    	if (c == '\n' || c == '\r')
    	{
    		size.height++;
    		if(size.width < (len -1))
    			size.width = len -1;
    		text += len;
    		len = tmp_len = 0;
    	}
    }
    if (len > 1)
    {
		size.height++;
		if(size.width < (len -1))
			size.width = len -1;
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
