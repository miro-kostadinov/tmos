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

	friend GObject;

	GObject* focus;
	GObject* children;

	GContainer (): focus(NULL), children(NULL) {};
	GContainer (GId id_t, const RECT_T& rect_t, GFlags flags_t= GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t), focus(NULL), children(NULL) {};
	~GContainer();

	virtual GObject* addChild (GObject* child);
	virtual GObject* get_object(GId xid);

protected:
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_idle(GMessage& msg);
	virtual unsigned int process_default(GMessage& msg);
	virtual unsigned int process_command(GMessage& msg);

	virtual void draw (LCD_MODULE* lcd, RECT_T area);
	virtual void draw_this (LCD_MODULE* lcd);
	virtual bool get_focus (bool notify_msg = true);
	virtual bool is_available();

	// container specific
	bool focus_on_previous();
	bool focus_on_next();
	GObject* first_available();
	GObject* last_available();
	bool close (GObject* toClose);
	bool close ();

private:
	bool set_focus_first();
	bool set_focus_last ();
};

#endif /* GCONTAINER_H_ */
