/*
 * gfloating_text.h
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#ifndef GFLOATING_TEXT_H_
#define GFLOATING_TEXT_H_

#include <tmos.h>
#include <gobject.h>

struct GFloating_Text: GObject
{

	CSTRING txt;
	const RENDER_MODE* font;
	RECT_T text_rect;
	int text_width;

	GFloating_Text (GId id_t, const RECT_T& rect_t,
			CSTRING txt_t, GFlags flags_t = GO_FLG_DEFAULT,  const RENDER_MODE* font_t = &FNT5x7)
	:GObject (id_t, rect_t, flags_t),
		txt (txt_t), font (font_t), text_width(0){};

	GUI_GET_OBJECT_TYPE(OBJECT_FTEXT);

	virtual unsigned int process_default(GMessage& msg);
	virtual unsigned int initialize(GMessage& msg);
	virtual void draw_this (LCD_MODULE* lcd);
};


#endif /* GFLOATING_TEXT_H_ */
