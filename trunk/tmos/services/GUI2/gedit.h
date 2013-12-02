/*
 * gedit.h
 *
 *  Created on: 17.09.2013
 *      Author: edguest
 */

#ifndef GEDIT_H_
#define GEDIT_H_

#include <tmos.h>
#include <gtext.h>
#include <fonts.h>
#include <gscroll.h>
#include <gmenu.h>

#define EDIT_INPUT_TIME		777
#define EDIT_BLINK_TIME		500

#define EDIT_TIMER_INPUT	1
#define EDIT_TIMER_BLINK	2

enum key_mode:unsigned char {KT_BG_CAPS=1, KT_BG, KT_EN_CAPS, KT_EN, KT_DIGIT};

struct GEdit:GText
{
	text_metrics_t text_size;
	bool	cursor_on;
	unsigned int pos;
	char last_key;
	unsigned char times_pressed;
	RECT_T cursor;
	unsigned int max_len;
	key_mode shift;
	GMenu* edit_menu;

	GEdit (	GId id_t, const RECT_T& rect_t, CSTRING txt_t, const char* caption_t = NULL,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short edit_flags_t = ES_DEFAULT,
			const RENDER_MODE* font_t = &FNT5x7)
		:GText (id_t, rect_t, txt_t, caption_t, flags_t, edit_flags_t, font_t),
		 text_size (0), cursor_on(false), pos (txt.length()), last_key (0),
		 times_pressed (0), max_len(-1u), shift (KT_BG_CAPS), edit_menu(NULL)
	{;}
	virtual ~GEdit()
	{
		if(edit_menu)
		{
			delete edit_menu;
			edit_menu = NULL;
		}
	}
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_key (GMessage& msg);
	virtual unsigned int process_default(GMessage& msg);

	virtual void draw_this (LCD_MODULE* lcd);
protected:
	void process_alpha_key(char pressed_key, const char* key_val);
	bool set_cursor_on_char (void);
	bool set_cursor_y_char (void);
	bool set_cursor_x_char (void);
	void text_change();
	void pos_change(int val, bool modified_text=true);
	void show_cursor();
};


#endif /* GEDIT_H_ */
