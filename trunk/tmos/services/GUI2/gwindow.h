/*
 * gwindow.h
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#ifndef GWINDOW_H_
#define GWINDOW_H_
#include <tmos.h>
#include <gcontainer.h>

struct GWindow: GContainer
{
#if GUI_DISPLAYS > 1
	unsigned char displays;
#endif
	CHandle hnd;

	GWindow (): displays(0xFF) {};
	GWindow (GId id_t, RECT_T rect_t,
			unsigned char displays_t, GFlags flags_t = GO_FLG_DEFAULT)
	:GContainer (id_t, rect_t, flags_t), displays (displays_t) {};

	void draw_this(LCD_MODULE* lcd);
	unsigned int initialize (GMessage msg);
	unsigned int process_key (GMessage msg);
	unsigned int DoModal();
	bool close (unsigned int reason);
};


#endif /* GWINDOW_H_ */
