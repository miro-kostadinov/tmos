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

#define SS_RIGHT		TA_RIGHT
#define SS_CENTER    	TA_CENTER
#define SS_LEFT    		TA_LEFT
#define SS_TOP    		TA_TOP
#define SS_MIDDLE    	TA_MIDDLE
#define SS_BOTTOM  		TA_BOTTOM
#define SS_WORDWRAP 	0x0100

#define SS_DEFAULT		(SS_WORDWRAP|SS_CENTER|SS_MIDDLE)
struct GText: GObject
{
	CSTRING txt;
	unsigned short align;
	const RENDER_MODE* text_font;
	GScroll* vscroll;
	GScroll* hscroll;
	RECT_T scroll_rect;

	GText (GId id_t, const RECT_T& rect_t, CSTRING txt_t,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short align_t = SS_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), align(align_t), text_font (font_t),
		 vscroll(NULL), hscroll(NULL) {};
	~GText()
	{
		if(vscroll)
			delete vscroll;
		if(hscroll)
			delete hscroll;
	}

	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_key (GMessage& msg);
	virtual void draw_this (LCD_MODULE* lcd);

	text_metrics_t SetTextAlign(unsigned int new_align );
	void alloc_scrollbars( void );

};


#endif /* GTEXT_H_ */
