/*
 * gobject.h
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#ifndef GOBJECT_H_
#define GOBJECT_H_
#include <tmos.h>
#include <stdgui.h>
#include <message.h>
#include <fonts.h>
#include <mqueue.h>

struct LCD_MODULE;
struct GContainer;

extern mqueue<GMessage, MAX_MESSAGES> GQueue;

struct text_metrics_t
{
	union{
		struct{	short int width, height;};
		int as_int;
	};
	text_metrics_t(): as_int(0) {}
	text_metrics_t(int val): as_int(0) {}
};

struct GObject{
	RECT_T rect;
	RECT_T client_rect;
	GContainer* parent;
	GObject* nextObj;
	GId id;
	GFlags flags;

	GObject (): parent(NULL), nextObj(NULL), id(0), flags(0) {};
	GObject (GId id_t, const RECT_T& rect_t, GFlags flags_t = GO_FLG_DEFAULT)
		:rect(rect_t), parent(NULL), nextObj(NULL), id(id_t), flags(flags_t) {};
	GObject (GId id_t,
			short int x0, short int y0, short int x1, short int y1,
			GFlags flags_t = GO_FLG_DEFAULT)
		:rect(x0, y0, x1, y1), parent(NULL), nextObj(NULL), id(id_t), flags(flags_t){};

	virtual ~GObject()
	{
		;
	}

	// Draw methods

	void clear_rect (const RECT_T& area);
	bool cut_hline ( int& x0, int& x1, int& y);
	bool cut_vline ( int& y0, int& y1, int& x);
	void set_xy_all(LCD_MODULE* lcd, unsigned int xy, unsigned int all);
	int  draw_text(LCD_MODULE* lcd, const char* txt);
	void draw_text_line (LCD_MODULE* lcd, const char* txt, unsigned int len);
	void draw_line(int x1, int y1, int x2, int y2);
	virtual bool close();
public:
	// Redraw methods
	virtual bool get_focus();													//sets the parent focus on this
	virtual void draw (LCD_MODULE* lcd, RECT_T area);
	virtual void draw_this (LCD_MODULE* lcd)
	{
		;
	}
	virtual void invalidate (GObject* object, RECT_T area);
	// virtual draw methods ( they are used from the module )
	virtual void allocate_border(void);
	virtual void draw_border(RECT_T& frame);
	virtual void draw_point( int x, int y);
	virtual void draw_hline( int x0, int x1, int y);
	virtual void draw_bline( int x0, int x1, int y);
	virtual void draw_vline( int y0, int y1, int x);
	virtual void invert_vline( int y0, int y1, int x);
	virtual void invert_hline( int x0, int x1, int y);
	virtual void draw_bitmap( int x0, int y0, const char* src, int width, int rows)
	{
		;
	}
	void draw_poligon(RECT_T& frame, bool fill=false);

	// queue message
//	void send_message (GMessage msg);
	void send_message(WM_MESSAGE wm, unsigned int param,
			unsigned long long lparam, GObject* dst) const;
	unsigned int message(GMessage& msg);
	virtual unsigned int initialize (GMessage& msg);							//proceeds items with WM_INIT code
	virtual unsigned int process_key (GMessage& msg)							//proceeds items with WM_KEY code
		{return 0;}
	virtual unsigned int process_default (GMessage& msg)						//proceeds items with unknown code
		{return 0;}
	virtual unsigned int process_command(GMessage& msg)							//proceeds items with WM_COMMAND code
		{return 0;}
	virtual unsigned int process_idle(GMessage& msg)							//proceeds items with WM_IDLE code
		{return 0;}
	virtual unsigned int process_destroy(GMessage& msg); 						//The WM_DESTROY message is sent when a window is being destroyed.

	bool set_flag(GFlags val);
	bool clr_flag(GFlags val);

	virtual GObject* get_object(GId xid);

	virtual bool is_available();

};

// Draw utility
text_metrics_t get_text_metrics(const char* text, short int x_size, const RENDER_MODE* font);

#endif /* GOBJECT_H_ */
