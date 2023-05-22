/*
 * gtext.cpp
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#include <stdgui.h>

unsigned int GText::initialize (GMessage& msg)
{
	if((flags & GO_FLG_SELECTED) && is_available() && parent)
		get_focus();

	alloc_scrollbars();
	SetTextAlign(align);
	if(msg.param)
		send_message(WM_DRAW, 0, 0L, this);
	return 1;
}

void GText::alloc_scrollbars( void )
{
	if(!(align & SS_WORDWRAP))
		flags &= ~GO_FLG_VSCROLL;

	if(flags & GO_FLG_HSCROLL)
	{
		if(!hscroll)
			hscroll = new GScroll(this);
	}
	else
	{
		if(hscroll)
		{
			delete hscroll;
			hscroll = nullptr;
		}
	}
	if(flags & GO_FLG_VSCROLL)
	{
		if(!vscroll)
			vscroll = new GScroll(this);
	}
	else
	{
		if(vscroll)
		{
			delete vscroll;
			vscroll = nullptr;
		}
	}
}

text_metrics_t GText::SetTextAlign(unsigned int new_align )
{
	text_metrics_t size_of_text;

	if(new_align != align )
		align = new_align;

	client_rect = rect;
//1. if there is a frame, reduce the client rectangle by the dimensions of the frame
	if(flags & GO_FLG_BORDER)
		allocate_border();
//2. if exist caption, then place the caption in the client rectangle and reduce rectangle
	if(caption && *caption)
	{
		if(align & SS_WORDWRAP)
		{
			if(client_rect.height())
				client_rect.y0 += text_font->vspacing + 2*text_font->vdistance;
		}
		else
		{
			if(client_rect.width())
				client_rect.x0 += text_font->hspacing*strlen(caption);
		}
	}

	scroll_rect = client_rect;
//3. the text sizes are calculated
	if(align & SS_WORDWRAP)
	{
		// can contain more than one line
		size_of_text = get_text_metrics(txt.c_str(), client_rect.width(), text_font);
	}
	else
	{
		// all text is placed on one line
		size_of_text = get_text_metrics(txt.c_str(), -1, text_font);
	}
//4.  vertical reduction of the space occupied by the text
	if(size_of_text.height == text_font->vspacing)
		size_of_text.height = text_font->height;	// only one row, resize to char vertical size
	if(size_of_text.height > text_font->vspacing)
		size_of_text.height--;						// multiline text, exclude last v distance

//4. aligning text horizontally
	GUI_ASSERT(client_rect.width() >= text_font->width);
	if(size_of_text.width > scroll_rect.width())
		scroll_rect.x1 = scroll_rect.x0 + size_of_text.width;
	if(hscroll )
	{
		hscroll->ShowScroll(GO_FLG_HSCROLL, true);
		hscroll->SetScrollRange(GO_FLG_HSCROLL, size_of_text.width/text_font->hspacing);
		hscroll->SetScrollPos(GO_FLG_HSCROLL, 0 , false);
	}

//5. aligning text vertically
	GUI_ASSERT(client_rect.height() >= text_font->height);
	if(size_of_text.height <= client_rect.height())
	{
		switch(align & TA_VERTICAL)
		{
		case TA_TOP:
			break;
		case TA_BOTTOM:
			client_rect.y0 = client_rect.y1 - size_of_text.height;
			break;
		case TA_MIDDLE:
			client_rect.y0 += (client_rect.height()+1 - size_of_text.height)>>1;
			break;
		}
		scroll_rect.y0 = client_rect.y0;
		scroll_rect.y1 = scroll_rect.y0 + size_of_text.height;
	}
	else
	{
		if(vscroll)
		{
			vscroll->ShowScroll(GO_FLG_VSCROLL, true);
			// providing space for the frame when selected
			client_rect.Inflate(0, 1);
			scroll_rect = client_rect;
			size_of_text = get_text_metrics(txt.c_str(), client_rect.width(), text_font);
			// calculating range of scrolling
			int range;
			// all text in rows
			range = size_of_text.height/text_font->vspacing;
			// subtracting the visible rows in the client rectangle
			range -= client_rect.height()/text_font->vspacing;
			// the client rectangle may also contain an incomplete row.
			// If there is one, add it to the scroll scope.
			if(client_rect.height()%text_font->vspacing)
				range++;
			vscroll->SetScrollRange(GO_FLG_VSCROLL, range);
			vscroll->SetScrollPos(GO_FLG_VSCROLL, 0 , false);
			// restore client rectangle
			client_rect.Deflate(0, 1);
		}
		scroll_rect.y1 = scroll_rect.y0 + size_of_text.height;
	}
	return size_of_text;
}

unsigned int GText::process_key (GMessage& msg)
{
	int res = 0;
	switch (msg.param & KEY_PRESREP_MASK)
	{
	case KEY_DOWN:
		if(scroll_rect.y1 >= client_rect.y1)
		{
			scroll_rect.Offset(0, -text_font->vspacing);
			res = 1;
		}
		break;

	case KEY_UP:
		if(scroll_rect.y0 < client_rect.y0)
		{
			scroll_rect.Offset(0, +text_font->vspacing);
			res = 1;
		}
		break;
	}
	if(res)
	{
		if(vscroll)
		{
			vscroll->SetScrollPos(GO_FLG_VSCROLL, (client_rect.y0 - scroll_rect.y0)/text_font->vspacing, false);
			invalidate(this, rect); // the rectangle is used to avoid double redrawing
		}
		else
			invalidate(this, client_rect);
	}
	return res;
}

unsigned int GText::process_default (GMessage& msg)
{
	switch(msg.code)
	{
		case WM_KILLFOCUS:
			client_rect.Deflate(0,1);
			break;
		case WM_SETFOCUS:
			if(get_focus(false))
			{
				client_rect.Inflate(0,1);
				return 1;
			}
			break;
		default:
			return GObject::process_default(msg);
	}
	return 0;
}

void GText::move(int x, int y)
{
	GObject::move(x, y);
	scroll_rect.Offset(x, y);
}

void GText::draw_caption(LCD_MODULE* lcd)
{
	if(caption && *caption)
	{
		RECT_T rc(client_rect);
		lcd->pos_x = rect.x0 /*+text_font->hdistance*/ +((flags&GO_FLG_BORDER)?get_border_size().x:0); // BUG: the position must be the same as the one at initialization
		if(align & SS_WORDWRAP)
		{
			lcd->pos_y = rect.y0 +text_font->vdistance +((flags&GO_FLG_BORDER)?get_border_size().y:0);
			client_rect = rect;
			draw_text(lcd, caption);
		}
		else
		{
			lcd->pos_y = client_rect.y0;// +text_font->vdistance;
			client_rect = rect;
			draw_text_line(lcd, caption, strlen(caption));
		}
		client_rect = rc;
	}
}

void GText::draw_this (LCD_MODULE* lcd)
{
	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		lcd->set_color(PIX_WHITE);
		if(flags & GO_FLG_BORDER)
			lcd->draw_border(rect);

		lcd->set_font(text_font);
		lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));

		draw_caption(lcd);
		GClientLcd dc(this);
		if(dc.CreateLcd(scroll_rect, lcd))
		{
			lcd->pos_x = dc.client_rect.x0;
			lcd->pos_y = dc.client_rect.y0;
			dc.draw_text(lcd, txt.c_str());
			dc.RelaseLcd();
		}
		if(vscroll)
			vscroll->draw_scroll(lcd);
		if (flags & GO_FLG_SELECTED)
			lcd->draw_poligon(rect);
	}
}

unsigned int GTitle::process_default (GMessage& msg)
{
	return GObject::process_default(msg);
}

void GTitle::draw_this (LCD_MODULE* lcd)
{
	if(client_rect.height() > 0 && client_rect.width() > 0)
	{
		lcd->set_font(text_font);
#if !GUI_MONOCHROME
		lcd->set_color(PIX_WHITE);
#endif
		lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));
		GClientLcd dc(this);
		if(dc.CreateLcd(scroll_rect, lcd))
		{
			lcd->pos_x = dc.client_rect.x0;
			lcd->pos_y = dc.client_rect.y0;// +text_font->vdistance;
			dc.draw_text(lcd, txt.c_str());
			dc.RelaseLcd();
		}

		RECT_T rc(client_rect);
		client_rect = parent->rect;
		client_rect.y1 = client_rect.y0 + rc.height();
		draw_border(client_rect);
		draw_poligon(client_rect, true);
		draw_hline(client_rect.x0, client_rect.x1, client_rect.y1);
		client_rect = rc;
	}
}
