/*
 * gedit.h
 *
 *  Created on: 17.09.2013
 *      Author: edguest
 */

#ifndef GEDIT_H_
#define GEDIT_H_

#include <tmos.h>
#include <gobject.h>
#include <fonts.h>

struct GEdit:GObject
{
	CSTRING txt;
	const RENDER_MODE* text_font;
	unsigned int text_offset;
	unsigned int cursor_blink_time;
	unsigned int pos;
	unsigned int key_input_time;
	char last_key;
	unsigned char times_pressed;
	RECT_T cursor;
	bool shift;

	GEdit (): text_font (&FNT5x7), text_offset (0), cursor_blink_time (CURRENT_TIME), pos (0) , key_input_time (0), last_key (0), times_pressed (0), shift (false) {};
	GEdit (GId id_t, RECT_T rect_t, CSTRING txt_t,	GFlags flags_t = GO_FLG_DEFAULT, const RENDER_MODE* font_t = &FNT5x7)
		:GObject (id_t, rect_t, flags_t), txt (txt_t), text_font (font_t), text_offset (0),
		 cursor_blink_time (CURRENT_TIME), pos (txt.length()), key_input_time (0),
		 last_key (0), times_pressed (0), shift (false) {};

	void draw_this (LCD_MODULE* lcd);
	unsigned int process_key (GMessage msg);
	void process_alpha_key(char pressed_key, const char* key_val);
	unsigned int process_idle (GMessage msg);
	void set_cursor_on_char (const char* text, unsigned int index);

};


#endif /* GEDIT_H_ */
