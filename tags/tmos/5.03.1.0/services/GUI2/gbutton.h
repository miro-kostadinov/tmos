/*
 * gbutton.h
 *
 *  Created on: 30.08.2013
 *      Author: edguest
 */

#ifndef GBUTTON_H_
#define GBUTTON_H_

#include <gobject.h>

#define GB_TYPE_NONE		0
#define GB_TYPE_RADIO		1
#define GB_TYPE_CHECK		2

/*
#define GICON_CIRCLE			0
#define GICON_CHECKED_CIRCLE	1
#define GICON_SQUARE			2
#define GICON_CHECKED_SQUARE	3
#define GICON_SQUARE_SQUARE		4
#define GICON_UP				5
#define GICON_DOWN				6
#define GICON_LEFT				7
#define GICON_RIGHT				8

*/
struct GButton: GObject
{
	CSTRING label;
	unsigned int code;
	unsigned char type;

	GButton (GId id_t, const RECT_T& rect_t, unsigned int msg_code,
			const char* label_t, GFlags flags_t = GO_FLG_DEFAULT|GO_FLG_BORDER, unsigned char type_t = GB_TYPE_NONE )
		:GObject (id_t, rect_t, flags_t), label (label_t), code (msg_code), type(type_t){};

//	virtual void allocate_border(void);
	virtual void draw_border(RECT_T& frame);

	virtual void draw_this (LCD_MODULE* lcd);
	virtual unsigned int process_key (GMessage& msg);
};


#endif /* GBUTTON_H_ */
