/*
 * gfloating_button.h
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#ifndef GFLOATING_BUTTON_H_
#define GFLOATING_BUTTON_H_

#include <tmos.h>
#include <gbutton.h>
#include <gfloating_text.h>
#include <gcontainer.h>

struct GFloating_Button: GContainer
{
	CSTRING label;
	unsigned int code;
	unsigned char type;

	GFloating_Button (GId id_t, const RECT_T& rect_t, unsigned int msg_code,
			const char* label_t, GFlags flags_t = GO_FLG_DEFAULT, unsigned char type_t = GB_TYPE_NONE )
		:GContainer (id_t, rect_t, flags_t), label (label_t), code (msg_code), type(type_t){};

	GUI_GET_OBJECT_TYPE(OBJECT_FBUTTON);

//	virtual void allocate_border(void);
	virtual void draw_border(RECT_T& frame);
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_key (GMessage& msg);
	virtual void draw_this (LCD_MODULE* lcd);
};


#endif /* GFLOATING_BUTTON_H_ */
