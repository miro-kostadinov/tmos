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
	GText (GId id_t, const RECT_T& rect_t, CSTRING txt_t, const char* caption_t=NULL,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short align_t = SS_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), align(align_t), text_font (font_t),
		 vscroll(NULL), hscroll(NULL), caption(caption_t)
	{;}

	GText () : GObject(),txt (), align(0), text_font (0),
			 vscroll(NULL), hscroll(NULL), caption(NULL)
	{;}

	~GText()
	{
		if(vscroll)
		{
			delete vscroll;
			vscroll = NULL;
		}
		if(hscroll)
		{
			delete hscroll;
			hscroll = NULL;
		}
	}

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


#endif /* GTEXT_H_ */
