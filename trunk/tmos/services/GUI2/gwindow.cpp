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
	GContainer::initialize(msg);									//initializes both the window and its items
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
	close();
	if(parent && parent->focus)
		send_message(WM_DRAW, 0, rect.as_int, parent->focus);
	notify_message(WM_QUIT, msg.param);
	return true;
}

void GWindow::notify_message(WM_MESSAGE code, unsigned int param, GObject* dst)
{
	GMessage msg(code, param, dst);

#if GUI_DEBUG
	TRACELN1("\e[4;1;34m");
	TRACE("%s >>  Notify ", CURRENT_TASK->name);
	TRACE("%X[%d] ( %s %X )\e[m", dst, (dst)?dst->id:-1, szlist_at(wm_dbg_str, code), param);
#endif
	Queue.push(msg);
	if(hnd.res == (FLG_BUSY | FLG_OK))
		usr_HND_SET_STATUS(&hnd, RES_SIG_OK);										//signals the window thread
}

// client methods

void GWindow::PostMessage(WM_MESSAGE code, unsigned int param, GObject* dst)
{
	GMessage msg(code, param, dst);
	if(hnd.res != RES_CLOSED && hnd.mode1 == GUI_HND_ATTACH)
	{
#if GUI_DEBUG
		TRACELN1("\e[4;1;36m");
		TRACE("%s >>  Post, \e[m", CURRENT_TASK->name);
#endif
		if(hnd.res & FLG_SIGNALED)
		{
			tsk_try_signal(hnd.signal);
			hnd.res &=~FLG_SIGNALED;
		}
		hnd.tsk_cancel();
		hnd.tsk_write(&msg, 0);
//		hnd.tsk_start_read(NULL,0);
#if GUI_DEBUG
		TRACELN1("\e[4;1;36m");
		TRACE("%X[%d] ( %s %X )\e[m", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param);
#endif
	}
}

bool GWindow::GetMessage(GMessage& msg, uint32_t tout)
{
	unsigned int sig;

	if(hnd.res < RES_CLOSED)
	{
		if(!(hnd.res & FLG_BUSY))
		{
			if(hnd.res & FLG_SIGNALED)
			{
				tsk_try_signal(hnd.signal);
				hnd.res &=~FLG_SIGNALED;
			}
			if( Queue.pop(msg) )
			{
		#if GUI_DEBUG
				TRACELN1("\e[4;1;36m");
				TRACE("%s <<", CURRENT_TASK->name);
				TRACE("%X[%d] ( %s %X )\e[m ", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
		#endif
				return true;
			}
			hnd.tsk_start_read(NULL, 0);
		}
		if(tout)
		{
			sig = tsk_wait_signal(-1u, tout);
		} else
			sig = tsk_get_signal(-1u);

		if( sig & ~hnd.signal)
		{
			tsk_send_signal(CURRENT_TASK, sig&~hnd.signal);
			sig &= hnd.signal;
		}
		if(sig)
		{
			hnd.res &=~FLG_SIGNALED;
			if( Queue.pop(msg) )
			{
#if GUI_DEBUG
				TRACELN1("\e[4;1;36m");
				TRACE("%s <<", CURRENT_TASK->name);
				TRACE("%X[%d] ( %s %X )\e[m ", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
#endif
				return true;
			}
		}
	} else
		tsk_sleep(10);
	return false;
}

/*
bool GWindow::GetMessage(GMessage& msg)
{
	if(hnd.res != RES_CLOSED)
	{
		if(Queue.empty() && !(hnd.res & FLG_BUSY))
			hnd.tsk_start_read(NULL, 0);
		if( Queue.pop(msg) )
		{
#if GUI_DEBUG
			TRACELN1("\e[4;1;36m");
			TRACE("%s <<", CURRENT_TASK->name);
			TRACE("%X[%d] ( %s %X )\e[m ", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
#endif
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
*/

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
	bool post_destroy= true;

	while(hnd.res < RES_CLOSED)
	{
		if(GetMessage(msg, 10))
		{
			if(msg.code == WM_QUIT)
			{
				hnd.mode1 = GUI_HND_UNUSED;
				hnd.close();
				return true;
			}
		} else
		{
			if(post_destroy)
			{
				PostMessage(WN_DESTROY, 0, this);
				post_destroy = false;
			}
		}
	}
	return false;
}
/*

bool GWindow::Destroy()
{
	GMessage msg;
	if(hnd.res != RES_CLOSED)
	{
		hnd.tsk_cancel();
		do{
			while(Queue.pop(msg))
			{
#if GUI_DEBUG
				TRACELN1("\e[4;1;36m");
				TRACE("%s <<", CURRENT_TASK->name);
				TRACE("%X[%d] ( %s %X ) Destroy\e[m", msg.dst, (msg.dst)?msg.dst->id:-1, szlist_at(wm_dbg_str, msg.code), msg.param);
#endif
				if(msg.code == WM_QUIT)
				{
					hnd.mode1 = GUI_HND_UNUSED;
					hnd.close();
					return true;
				}
			}
			msg.code = WN_DESTROY;
			msg.dst = this;
#if GUI_DEBUG
			TRACELN1("\e[4;1;36m");
			TRACE("%s >>", CURRENT_TASK->name);
			TRACE("%X[%d] ( %s %X ) Destroy\e[m", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param);
#endif
			hnd.tsk_write(&msg, 0);

		}while(hnd.tsk_read(NULL,0)== RES_OK);
	}
	return false;
}
*/
