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
	text_metrics_t txt_size;

	client_rect = rect;

	if(flags & GO_FLG_BORDER)
		allocate_border();
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

	if(new_align != align )
		align = new_align;


	if(align & SS_WORDWRAP)
	{
		txt_size = get_text_metrics(txt.c_str(), client_rect.width(), text_font);
		if(vscroll)
		{
			if(client_rect.height() && txt_size.height > client_rect.height())
			{
				txt_size = get_text_metrics(txt.c_str(), client_rect.width()- GO_SCROLL_WIDTH, text_font);
				client_rect.Inflate(0, 1);
				vscroll->SetScrollRange(GO_FLG_VSCROLL, txt_size.height/text_font->vspacing);// /text_font->vspacing);
				vscroll->SetScrollPos(GO_FLG_VSCROLL, 0 , false);
				client_rect.Deflate(0, 1);
			}
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
	// check number of rows
	if(txt_size.height == text_font->vspacing)
		txt_size.height = text_font->height;	// only one row, resize to char vertical size
	if(txt_size.height > text_font->vspacing)
		txt_size.height--;						// exclude last v distance
	// align text horizontally
	if(scroll_rect.width())
	{
		if(txt_size.width > scroll_rect.width())
			scroll_rect.x1 = scroll_rect.x0 + txt_size.width;
		if(hscroll )
			hscroll->ShowScroll(GO_FLG_HSCROLL, true);
	}

	// align text vertically
	if(scroll_rect.height())
	{
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
				client_rect.y0 += (client_rect.height()+1 - txt_size.height)>>1;
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
	}
//	send_message(WM_DRAW, 0, rect.as_int, this);
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
			return 1;
		}
		break;

	case KEY_UP:
		if(scroll_rect.y0 < client_rect.y0)
		{
			scroll_rect.y0 ++;
			scroll_rect.y1 ++;
			if(vscroll)
				vscroll->SetScrollPos(GO_FLG_VSCROLL, client_rect.y0 - scroll_rect.y0, true);
			invalidate(this, client_rect);
			return 1;
		}
		break;
	}
	return 0;
}

unsigned int GText::process_default (GMessage& msg)
{
	switch(msg.code)
	{
		case WM_KILLFOCUS:
			client_rect.Deflate(0,1);
			if(scroll_rect.y1 >= client_rect.y1 && scroll_rect.y0 <= client_rect.y0 )
			{
				scroll_rect.Offset(0, -1);
				if(vscroll)
					vscroll->SetScrollPos(GO_FLG_VSCROLL, client_rect.y0 - scroll_rect.y0, true);
			}
			break;
		case WM_SETFOCUS:
			if(get_focus(false))
			{
				if(scroll_rect.y0 <= client_rect.y0 && scroll_rect.y1 >= client_rect.y1 )
					scroll_rect.Offset(0, 1);
				client_rect.Inflate(0,1);
				if(vscroll)
					vscroll->SetScrollPos(GO_FLG_VSCROLL, client_rect.y0 - scroll_rect.y0, true);
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
		lcd->pos_x = rect.x0 +text_font->hdistance +((flags&GO_FLG_BORDER)?get_border_size().x:0);
		if(align & SS_WORDWRAP)
		{
//			lcd->pos_x = rect.x0 +text_font->hdistance;
			lcd->pos_y = rect.y0 +text_font->vdistance +((flags&GO_FLG_BORDER)?get_border_size().y:0);
//			lcd->pos_y = client_rect.y0 -(text_font->vspacing +2*text_font->vdistance);
			client_rect = rect;
			draw_text(lcd, caption);
		}
		else
		{
//			lcd->pos_x = rect.x0 +text_font->hdistance +((flags&GO_FLG_BORDER)?get_border_size().x:0);
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
			draw_border(rect);

		lcd->set_font(text_font);
		lcd->allign = (align & (TA_HORIZONTAL|TA_VERTICAL));

		draw_caption(lcd);
		GClientLcd dc(this);
		if(dc.CreateLcd(scroll_rect, lcd))
		{
			lcd->pos_x = dc.client_rect.x0;
			lcd->pos_y = dc.client_rect.y0;// +text_font->vdistance;
			dc.draw_text(lcd, txt.c_str());
			dc.RelaseLcd();
		}
		if(vscroll)
			vscroll->draw_scroll(lcd);
		if (flags & GO_FLG_SELECTED)
			draw_poligon(rect);//client_rect);
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
