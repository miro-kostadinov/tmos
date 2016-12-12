/*
 * gwindow.h
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#ifndef GWINDOW_H_
#define GWINDOW_H_
#include <tmos.h>
#include <mqueue.h>
#include <gcontainer.h>
#include <gui_drv.h>

struct GWindow: GContainer
{
#if GUI_DISPLAYS > 1
	unsigned char displays;
#endif
protected:
	CHandle hnd;
	mqueue<GMessage, MAX_MESSAGES> Queue;
public:
	GWindow (): displays(0xFF)
	{
		hnd.mode1 = GUI_HND_UNUSED;
	};
	GWindow (GId id_t, const RECT_T& rect_t,
			unsigned char displays_t, GFlags flags_t = GO_FLG_DEFAULT)
	:GContainer (id_t, rect_t, flags_t), displays (displays_t)
	{
		hnd.mode1 = GUI_HND_UNUSED;
	};
	virtual ~GWindow()
	{
		Destroy();
	}

	GUI_GET_OBJECT_TYPE(OBJECT_WINDOW);

protected:
	friend void gui_thread(GUI_DRIVER_INFO* drv_info);

	virtual void draw_this(LCD_MODULE* lcd);
	virtual unsigned int initialize (GMessage& msg);
	virtual unsigned int process_key (GMessage& msg);
	virtual unsigned int process_destroy(GMessage& msg);


public:
	// client methods
	virtual bool DefWinProc(GMessage& msg);
	unsigned int DoModal();
	void PostMessage(WM_MESSAGE code, unsigned int param, GObject* dst = nullptr);
	bool GetMessage(GMessage& msg, uint32_t tout = 0);
	bool Create();
	bool Destroy();
	void notify_message(WM_MESSAGE code, unsigned int param, GObject* dst = nullptr);
	CHandle* get_handle() {return &hnd; }
};


#endif /* GWINDOW_H_ */
