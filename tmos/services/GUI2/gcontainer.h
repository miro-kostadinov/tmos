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

	GContainer (): focus(nullptr), children(nullptr) {};
	GContainer (GId id_t, const RECT_T& rect_t, GFlags flags_t= GO_FLG_DEFAULT)
		:GObject (id_t, rect_t, flags_t), focus(nullptr), children(nullptr) {};
	~GContainer();

	GUI_GET_OBJECT_TYPE(OBJECT_CONTAINER);

	virtual GObject* addChild (GObject* child);
	GObject* get_object(GId xid) override;
	GObject* addChildRef(GObject* child);
	void move(int x, int y) override;

protected:
	unsigned int initialize (GMessage& msg) override;
	unsigned int process_idle(GMessage& msg) override;
	unsigned int process_default(GMessage& msg) override;
	unsigned int process_command(GMessage& msg) override;

	void draw (LCD_MODULE* lcd, RECT_T area) override;
	void draw_this (LCD_MODULE* lcd) override;
	bool get_focus (bool notify_msg = true) override;
	bool is_available() override;

	bool set_flag(GFlags val) override;
	bool clr_flag(GFlags val) override;
	// container specific
	bool focus_on_previous() const;
	bool focus_on_next() const;
	GObject* first_available();
	GObject* last_available();
	bool close (GObject* toClose);
	bool close () override;

private:
	bool set_focus_first();
	bool set_focus_last ();
};

#endif /* GCONTAINER_H_ */
