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

enum key_mode:unsigned char {KT_BG_CAPS=1, KT_BG, KT_EN_CAPS, KT_EN, KT_DIGIT};

#define ES_RIGHT		TA_RIGHT
#define ES_CENTER    	TA_CENTER
#define ES_LEFT    		TA_LEFT
#define ES_MULTILINE	0x0100
#define ES_PASSWORD		0x0200
#define ES_WANTRETURN   0x0400
#define ES_HIDE_CURSOR	0x0800
#define ES_AUTO_SCROLL	0x1000

#define ES_DEFAULT		(ES_CENTER|ES_MULTILINE|ES_AUTO_SCROLL)

struct GEdit:GText
{
	text_metrics_t text_size;
	unsigned int cursor_blink_time;
	unsigned int pos;
	unsigned int key_input_time;
	char last_key;
	unsigned char times_pressed;
	RECT_T cursor;
	key_mode shift;
	GMenu* edit_menu;

	GEdit (	GId id_t, const RECT_T& rect_t, CSTRING txt_t,
			GFlags flags_t = GO_FLG_DEFAULT, unsigned short edit_flags_t = ES_DEFAULT,
			const RENDER_MODE* font_t = &FNT5x7)
		:GText (id_t, rect_t, txt_t, flags_t, edit_flags_t, font_t),
		 text_size (0), cursor_blink_time (CURRENT_TIME),
		 pos (txt.length()), key_input_time (0),
		 last_key (0), times_pressed (0), shift (KT_BG_CAPS), edit_menu(NULL)
	{};
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
	virtual unsigned int process_idle (GMessage& msg);

	virtual void draw_this (LCD_MODULE* lcd);
protected:
	void process_alpha_key(char pressed_key, const char* key_val);
	bool set_cursor_on_char (const char* text, unsigned int index);
	bool set_cursor_y_char (const char* text, unsigned int index);
	bool set_cursor_x_char (const char* text, unsigned int index);
	void text_change();
	void pos_change(int val);
	void show_cursor();
};


#endif /* GEDIT_H_ */
