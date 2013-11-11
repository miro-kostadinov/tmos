/*
 * gtext.cpp
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#include <gtext.h>
#include <tmos.h>
#include <lcd.h>

unsigned int GText::initialize (GMessage& msg)
{
	if((flags & GO_FLG_SELECTED) && parent)
		parent->focus = this;

	alloc_scrollbars();
	SetTextAlign(align);
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
			hscroll = NULL;
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
			vscroll = NULL;
		}
	}
}

text_metrics_t GText::SetTextAlign(unsigned int new_align )
{
	text_metrics_t txt_size;

	client_rect = rect;
	if(flags & GO_FLG_BORDER)
		allocate_border();
	client_rect.x0 += text_font->hdistance;
	client_rect.y0 += text_font->vdistance;
	scroll_rect = client_rect;

	if(new_align != align )
		align = new_align;


	if(align & SS_WORDWRAP)
	{
		txt_size = get_text_metrics(txt.c_str(), client_rect.width(), text_font);
		if(vscroll)
		{
			vscroll->SetScrollRange(GO_FLG_VSCROLL, txt_size.height/text_font->vspacing);
			vscroll->SetScrollPos(GO_FLG_VSCROLL, 0 , false);
		}
	}
	else
	{
		txt_size = get_text_metrics(txt.c_str(), -1, text_font);
		if(hscroll)
		{
			hscroll->SetScrollRange(GO_FLG_HSCROLL, txt_size.width/text_font->hspacing);
			hscroll->SetScrollPos(GO_FLG_HSCROLL, 0 , false);
		}
	}

	// align text horizontally
	if(txt_size.width > scroll_rect.width())
		scroll_rect.x1 = scroll_rect.x0 + txt_size.width;
	if(hscroll && (flags & GO_FLG_HSCROLL))
	{
		flags &= ~GO_FLG_HSCROLL;
		hscroll->ShowScroll(GO_FLG_HSCROLL, true);
	}

	// align text vertically
	if(txt_size.height <= client_rect.height())
	{
		switch(align & TA_VERTICAL)
		{
		case TA_TOP:
			break;
		case TA_BOTTOM:
			client_rect.y0 = client_rect.y1 - txt_size.height;
			break;
		case TA_MIDDLE:
			client_rect.y0 += (client_rect.height() - txt_size.height)>>1;
			break;
		}
		scroll_rect.y0 = client_rect.y0;
		scroll_rect.y1 = scroll_rect.y0 + txt_size.height;
	}
	else
	{
		scroll_rect.y1 = scroll_rect.y0 + txt_size.height;
		if(vscroll && (flags & GO_FLG_VSCROLL))
		{
			flags &= ~GO_FLG_VSCROLL;
			vscroll->ShowScroll(GO_FLG_VSCROLL, true);
		}
	}
	send_message(WM_DRAW, 0, rect.as_int, this);
	return txt_size;
}

unsigned int GText::process_key (GMessage& msg)
{
	switch (msg.param & KEY_PRESREP_MASK)
	{
	case KEY_DOWN:
		if(scroll_rect.y1 > client_rect.y1)
		{
			scroll_rect.y0 --;
			scroll_rect.y1 --;
			if(vscroll)
				vscroll->SetScrollPos(GO_FLG_VSCROLL, client_rect.y0 - scroll_rect.y0, true);
			invalidate(this, client_rect);
		}
		return 1;
	case KEY_UP:
		if(scroll_rect.y0 < client_rect.y0)
		{
			scroll_rect.y0 ++;
			scroll_rect.y1 ++;
			if(vscroll)
				vscroll->SetScrollPos(GO_FLG_VSCROLL, client_rect.y0 - scroll_rect.y0, true);
			invalidate(this, client_rect);
		}
		return 1;
	}
	return 0;
}

void GText::draw_this (LCD_MODULE* lcd)
{
	if(flags & GO_FLG_BORDER)
		draw_border(rect);

	lcd->set_font(text_font);
	lcd->color = PIX_WHITE;
	lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));

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
		draw_poligon(client_rect, true);
}
