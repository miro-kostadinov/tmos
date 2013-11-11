/*
 * gwindow.cpp
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#include <gwindow.h>
#include <tmos.h>

void GWindow::draw_this(LCD_MODULE* lcd)
{
//	clear_rect(rect);								//clears the area
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
}

unsigned int GWindow::initialize (GMessage& msg)
{
	GContainer::initialize(msg);												//initializes both the window and its items
	send_message(WM_DRAW, 0, 0L, this);								//initial draw of the window and its items
	return 0;
}

unsigned int GWindow::process_key (GMessage& msg)
{
	if(focus && focus->message(msg))
		return 1;
	switch (msg.param)
	{
	case KEY_ESC:
		notify_message(WM_CLOSE, GO_EXIT, this);														//closes the window
		return 1;
	default:
		return 0;
	}
}

unsigned int GWindow::process_destroy(GMessage& msg)
{
	GObject::close();
	if(parent && parent->focus)
		send_message(WM_DRAW, 0, rect.as_int, parent->focus);
	notify_message(WM_QUIT, msg.param);
	return true;
}

void GWindow::notify_message(WM_MESSAGE code, unsigned int param, GObject* dst)
{
	GMessage msg(code, param, dst);
	TRACELN("%s >>", CURRENT_TASK->name);
	TRACE("%X[%d] ( %s %X ) Notify", dst, (dst)?dst->id:-1, szlist_at(wm_dbg_str, code), param);
	Queue.push(msg);
	if((hnd.res & FLG_BUSY))
		usr_HND_SET_STATUS(&hnd, RES_SIG_OK);										//signals the window thread
}

// client methods

void GWindow::PostMessage(WM_MESSAGE code, unsigned int param, GObject* dst)
{
	GMessage msg(code, param, dst);
	if(hnd.res != RES_CLOSED && hnd.mode1 == GUI_HND_ATTACH)
	{
		if(hnd.res & FLG_SIGNALED)
		{
			tsk_try_signal(hnd.signal);
			hnd.res &=~FLG_SIGNALED;
		}
		hnd.tsk_cancel();
		TRACELN("%s >>", CURRENT_TASK->name);
		TRACE("%X[%d] ( %s %X ) Post", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param);
		hnd.tsk_write(&msg, 0);
		hnd.tsk_start_read(NULL,0);
	}
}

bool GWindow::GetMessage(GMessage& msg)
{
	if(hnd.res != RES_CLOSED)
	{
		if(Queue.empty() && !(hnd.res & FLG_BUSY))
			hnd.tsk_start_read(NULL, 0);
		if( Queue.pop(msg) )
		{
			TRACELN("%s <<", CURRENT_TASK->name);
			TRACE("%X[%d] ( %s %X ) ", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
			if(hnd.res & FLG_SIGNALED)
			{
				tsk_try_signal(hnd.signal);
				hnd.res &=~FLG_SIGNALED;
			}
			if(!(hnd.res & FLG_BUSY))
				hnd.tsk_start_read(NULL, 0);
			return true;
		}
	}
	return false;
}

bool GWindow::DefWinProc(GMessage& msg)
{
	switch(msg.code)
	{
	case WM_QUIT:
		hnd.mode1 = GUI_HND_UNUSED;
		hnd.close();
		return true;
	case WM_CLOSE:
		if(Destroy())
			return true;
		break;
	default:
		break;
	}
	return false;
}


unsigned int GWindow::DoModal()
{
	GMessage msg;

	if(Create())
	{
		while(1)
		{
			if(GetMessage(msg))
			{
				if(DefWinProc(msg))
					return msg.param;
			}
			else
				tsk_sleep(10);
		}
	}
	return 0;
}

bool GWindow::Create()
{
	if(hnd.res == RES_CLOSED)
	{
		if(hnd.tsk_open(GUI_DRV_INDX, this))
			return (hnd.tsk_write(NULL,0)== RES_OK);
	}
	else
	{
		if(hnd.mode1 == GUI_HND_ATTACH)
			return true;
	}
	return false;
}

bool GWindow::Destroy()
{
	GMessage msg;
	if(hnd.res != RES_CLOSED)
	{
		hnd.tsk_cancel();
		do{
			while(Queue.pop(msg))
			{
				TRACELN("%s <<", CURRENT_TASK->name);
				TRACE("%X[%d] ( %s %X ) Destroy ", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
				if(msg.code == WM_QUIT)
				{
					hnd.mode1 = GUI_HND_UNUSED;
					hnd.close();
					return true;
				}
			}
			msg.code = WN_DESTROY;
			msg.dst = this;
			TRACELN("%s >>", CURRENT_TASK->name);
			TRACE("%X[%d] ( %s %X ) Destroy ", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param);
			hnd.tsk_write(&msg, 0);
		}while(hnd.tsk_read(NULL,0)== RES_OK);
	}
	return false;
}
