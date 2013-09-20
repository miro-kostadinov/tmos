/*
 * gobject.h
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#ifndef GOBJECT_H_
#define GOBJECT_H_
#include <message.h>
#include <tmos.h>
#include <stdgui.h>
struct LCD_MODULE;
struct GContainer;

struct GObject{
	RECT_T rect;
	RECT_T client_rect;
	GContainer* parent;
	GObject* nextObj;
	GId id;
	GFlags flags;

	GObject (): parent(NULL), nextObj(NULL), id(0), flags(0) {};
	GObject (GId id_t, RECT_T rect_t, GFlags flags_t = GO_FLG_DEFAULT)
		:rect(rect_t), parent(NULL), nextObj(NULL), id(id_t), flags(flags_t) {};
	GObject (GId id_t,
			short int x0, short int y0, short int x1, short int y1,
			GFlags flags_t = GO_FLG_DEFAULT)
		:rect(x0, y0, x1, y1), parent(NULL), nextObj(NULL), id(id_t), flags(flags_t){};

	virtual ~GObject(){};

	// Draw methods
	void clear_rect (unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1);
	void draw_border(RECT_T& frame);
	virtual void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows){};
	virtual void draw_hline(unsigned int x0, unsigned int x1, unsigned int y);
	virtual void draw_bline(unsigned int x0, unsigned int x1, unsigned int y);
	virtual void draw_vline(unsigned int y0, unsigned int y1, unsigned int x);
	virtual void invert_vline(unsigned int y0, unsigned int y1, unsigned int x);
	virtual void invert_hline(unsigned int x0, unsigned int x1, unsigned int y);

	bool cut_hline (unsigned int& x0, unsigned int& x1, unsigned int& y);
	bool cut_vline (unsigned int& y0, unsigned int& y1, unsigned int& x);

	void set_xy_all(LCD_MODULE* lcd, unsigned int xy, unsigned int all);
//	void set_xy_on_char (LCD_MODULE* lcd, const char *txt, unsigned int index);
	const char* draw_text(LCD_MODULE* lcd, const char* txt);
	void draw_text_line (LCD_MODULE* lcd, const char* txt, unsigned int len);

	// Redraw methods
	virtual void draw (LCD_MODULE* lcd, RECT_T area);
	virtual void draw_this (LCD_MODULE* lcd){};
	virtual void invalidate (GObject* object, RECT_T area);

	void send_message (GMessage msg);
	unsigned int message(GMessage msg);
	virtual unsigned int initialize (GMessage msg);								//proceeds items with WM_INIT code
	virtual unsigned int process_key (GMessage msg){return 0;};					//proceeds items with WM_KEY code
	virtual unsigned int process_user (GMessage msg){return 0;};				//proceeds items with WM_USER code
	virtual unsigned int process_default (GMessage msg){return 0;};				//proceeds items with unknown code
	virtual unsigned int process_command(GMessage msg){return 0;};				//proceeds items with WM_COMMAND code
	virtual unsigned int process_idle(GMessage msg){return 0;};					//proceeds items with WM_IDLE code
	virtual void get_focus();													//sets the parent focus on this
	bool set_flag(GFlags val);
	bool clr_flag(GFlags val);
	virtual bool close();

};

#endif /* GOBJECT_H_ */
