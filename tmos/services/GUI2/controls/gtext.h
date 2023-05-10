/*
 * gtext.h
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#ifndef GTEXT_H_
#define GTEXT_H_

#include <stdgui.h>
#include <gscroll.h>

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

	unsigned int process_default (GMessage& msg) override;
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_key (GMessage& msg) override;
	void draw_this (LCD_MODULE* lcd) override;

	text_metrics_t SetTextAlign(unsigned int new_align );
	void alloc_scrollbars( void );
	void move(int x, int y) override;
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

	unsigned int process_default (GMessage& msg) override;
	void draw_this (LCD_MODULE* lcd) override;
};

#endif /* GTEXT_H_ */
