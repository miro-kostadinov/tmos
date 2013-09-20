/*
 * gfloating_text.h
 *
 *  Created on: 19.09.2013
 *      Author: edguest
 */

#ifndef GFLOATING_TEXT_H_
#define GFLOATING_TEXT_H_

#include <tmos.h>
#include <gcontainer.h>
#include <gtext.h>

struct GFloating_Text: GContainer
{

	CSTRING txt;
	const RENDER_MODE* font;
	unsigned int float_time;
	int offset;

	GFloating_Text (GId id_t, RECT_T rect_t,
			CSTRING txt_t, GFlags flags_t = GO_FLG_DEFAULT,  const RENDER_MODE* font_t = &FNT5x7)
	:GContainer (id_t, rect_t, flags_t),
		txt (txt_t), font (font_t), float_time (CURRENT_TIME), offset (0) {};

	unsigned int process_idle(GMessage msg);
	unsigned int initialize(GMessage msg);
};


#endif /* GFLOATING_TEXT_H_ */
