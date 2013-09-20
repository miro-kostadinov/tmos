/*
 * gcontainer.h
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#ifndef GCONTAINER_H_
#define GCONTAINER_H_
#include <tmos.h>
#include <gobject.h>

struct GContainer:GObject {

	GObject* focus;
	GObject* children;

	GContainer (): focus(NULL), children(NULL) {};
	GContainer (GId id_t, RECT_T rect_t, GFlags flags_t= GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t), focus(NULL), children(NULL) {};
	~GContainer();

	unsigned int initialize (GMessage msg);
	unsigned int process_idle(GMessage msg);

	void draw (LCD_MODULE* lcd, RECT_T area);
	void draw_this (LCD_MODULE* lcd);

	virtual GObject* addChild (GObject* child);

	void get_focus ();
	bool set_focus (GObject* focused);
	bool set_focus (unsigned char focusId);
	bool set_focus_first();
	bool set_focus_last ();
	bool close (GObject* toClose);
	bool close (unsigned char closeId);
};

#endif /* GCONTAINER_H_ */
