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
#include <fonts.h>

struct GText: GObject
{
	CSTRING txt;
	const RENDER_MODE* text_font;

	GText (GId id_t, RECT_T rect_t, CSTRING txt_t,
			GFlags flags_t = GO_FLG_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), text_font (font_t) {};

	void draw_this (LCD_MODULE* lcd);
};


#endif /* GTEXT_H_ */
