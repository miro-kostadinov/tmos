/*
 * gtext.h
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#ifndef GTEXT_H_
#define GTEXT_H_

#include <tmos.h>
#include <gobject.h>
#include <gscroll.h>
#include <fonts.h>
#include <lcd.h>

struct GText: GObject
{
	CSTRING txt;
	unsigned short align;
	const RENDER_MODE* text_font;
	GScroll* vscroll;
	GScroll* hscroll;
	RECT_T scroll_rect;
	const char* caption;
	GText (GId id_t, const RECT_T& rect_t, CSTRING txt_t, const char* caption_t=nullptr,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short align_t = SS_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), align(align_t), text_font (font_t),
		 vscroll(nullptr), hscroll(nullptr), caption(caption_t)
	{;}

	GText (GId id_t, const RECT_T& rect_t, const char* txt_t,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short align_t = SS_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), align(align_t), text_font (font_t),
		 vscroll(nullptr), hscroll(nullptr), caption(nullptr)
	{;}


	GText () : GObject(),txt (), align(0), text_font (0),
			 vscroll(nullptr), hscroll(nullptr), caption(nullptr)
	{;}

	~GText()
	{
		if(vscroll)
		{
			delete vscroll;
			vscroll = nullptr;
		}
		if(hscroll)
		{
			delete hscroll;
			hscroll = nullptr;
		}
	}

	GUI_GET_OBJECT_TYPE(OBJECT_TEXT);

	virtual unsigned int process_default (GMessage& msg);
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_key (GMessage& msg);
	virtual void draw_this (LCD_MODULE* lcd);

	text_metrics_t SetTextAlign(unsigned int new_align );
	void alloc_scrollbars( void );
	virtual void move(int x, int y);
protected:
	void draw_caption(LCD_MODULE* lcd);
};

struct GTitle: GText
{
	GTitle (GId id_t, const RECT_T& rect_t, CSTRING txt_t, const char* caption_t=nullptr,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short align_t = SS_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GText (id_t, rect_t, txt_t, caption_t,
				flags_t, align_t, font_t)
	{;}

	GTitle () :GText()
	{;}

	virtual ~GTitle()
	{;}

	virtual unsigned int process_default (GMessage& msg);
	virtual void draw_this (LCD_MODULE* lcd);
};

#endif /* GTEXT_H_ */
