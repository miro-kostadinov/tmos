/*
 * gscroll.cpp
 *
 *  Created on: 07.10.2013
 *      Author: stanly
 */

#include <stdgui.h>

bool GScroll::ShowScroll(GFlags sb, bool visible)
{
	bool res = false;
	if(sb&GO_FLG_HSCROLL)
	{
		if(visible)
		{
			if(!(object->flags&GO_FLG_HSCROLL))
			{
				object->flags |= GO_FLG_HSCROLL;
				res = true;
			}
		}
		else
		{
			if(object->flags&GO_FLG_HSCROLL)
			{
				object->flags &= ~GO_FLG_HSCROLL;
				res = true;
			}
		}
	}
	if(sb&GO_FLG_VSCROLL)
	{
		if(visible)
		{
			if(!(object->flags&GO_FLG_VSCROLL))
			{
				object->client_rect.x1 -= GO_SCROLL_WIDTH;//+1;
				object->flags |= GO_FLG_VSCROLL;
				res = true;
				//res = (res || object->set_flag(GO_FLG_VSCROLL));
			}
		}
		else
		{
			if(object->flags&GO_FLG_VSCROLL)
			{
				object->flags &= ~GO_FLG_VSCROLL;
				object->client_rect.x1 += GO_SCROLL_WIDTH;//+1;
				res = true;
				//res = ( res || object->clr_flag(GO_FLG_VSCROLL));
			}
		}
	}
	return res;
}

void GScroll::draw_scroll(LCD_MODULE* lcd)
{
	if(object->flags & (GO_FLG_VSCROLL|GO_FLG_HSCROLL))
	{
		RECT_T rect(object->client_rect);

		if(object->flags & GO_FLG_HSCROLL)
		{
			if(object->flags & GO_FLG_BORDER)
				rect.y1 = object->rect.y1;
			rect.y0 = rect.y1 - (GO_SCROLL_WIDTH -1);
			if(ratio)
			{
				lcd->draw_border(rect);
				rect.x0 +=pos;
				rect.x1 = rect.x0 + page;
				for(int i= rect.y0 +1; i < rect.y1; i++)
					lcd->draw_hline(rect.x0, rect.x1, i);
			}
			else
				lcd->clear_rect(rect);
		}
		else
		{

			rect.x0 = rect.x1 +1;
			rect.x1 = rect.x0 + (GO_SCROLL_WIDTH -1);
			if(ratio)
			{
				lcd->draw_border(rect);
				rect.y0 +=pos;
				rect.y1 = rect.y0 + page;

				for(int i= rect.x0 +1; i < rect.x1; i++)
					lcd->draw_vline(rect.y0, rect.y1, i);
			}
			else
				lcd->clear_rect(rect);
		}
	}
}

void GScroll::SetScrollPos(GFlags sb, unsigned short p, bool redraw)
{
	RECT_T rect(object->client_rect);
	unsigned int view_size=0;
	if(sb & GO_FLG_HSCROLL)
	{
		rect.y0 = rect.y1 +1;
		rect.y1 = rect.y0 + (GO_SCROLL_WIDTH -1);
		view_size = rect.width();
	}
	if(sb & GO_FLG_VSCROLL)
	{
		rect.x0 = rect.x1 +1;
		rect.x1 = rect.x0 + (GO_SCROLL_WIDTH -1);
		view_size = rect.height();
	}

	if(sb & (GO_FLG_HSCROLL|GO_FLG_VSCROLL))
	{
		view_size -=2;  //size of bottom/right cover
		//pos = 2 + ((p * ratio)>>10) + ((((p*ratio)&0x3FF) >= 512)?1:0);
		p = p * ratio;
		pos = 2 + (p+500)/1000;
		if( pos + page  > view_size )
			pos = view_size - page;
	}

	if(redraw && (object->flags &(GO_FLG_HSCROLL|GO_FLG_VSCROLL)))
		send_message(WM_DRAW, 0, rect.as_int, object);
}

void GScroll::SetScrollRange(GFlags sb, unsigned int p)
{
	unsigned int view_size=0;
	ratio = 0;

	if(p)
	{
		if(sb & GO_FLG_HSCROLL)
			view_size = object->client_rect.width();
		if(sb & GO_FLG_VSCROLL)
			view_size = object->client_rect.height();
		if(view_size > 4 + GO_SCROLL_WIDTH)
		{
			view_size -=4;
			if(sb &(GO_FLG_HSCROLL|GO_FLG_VSCROLL))
			{

				if(--p)
				{
					if(p == 1)
						p++;
					ratio = (view_size * 1000)/p;
					page = ratio/1000 + (((ratio%1000) >= 500)?1:0);
					if(!page)
						page = 3;
					ratio = ((view_size - page)*1000)/p;
				}
				else
				{
					ratio = (view_size * 1000)/2;
					page = ratio/1000 + (((ratio%1000) >= 500)?1:0);
					if(!page)
						page = 3;
				}
			}
		}
	}
}
