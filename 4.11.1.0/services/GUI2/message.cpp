/*
 * message.cpp
 *
 *  Created on: 10.10.2014
 *      Author: Stanly
 */

#include <tmos.h>
#include <message.h>
#include <gobject.h>
#include <gwindow.h>
#include <gdowait.h>

msgQueue<MAX_MESSAGES> GQueue;
GWindow* Gdesktop;

void processes_all_messages(void)
{
	GMessage msg;
	while (GQueue.pop(msg))														//processes all messages in the queue
	{
#if GUI_DEBUG
		t0 = CURRENT_TIME;
		trace_message();
#endif
		if(msg.code == WM_DELETED || !Gdesktop)
			continue;

		if (msg.dst == NULL )
			msg.dst = Gdesktop->parent;	// lcd message

		if(GWait::dowait_win && msg.code == WN_DESTROY && msg.dst && msg.dst != GWait::dowait_win)
		{
			GWaitOwner** powner = &GWait::dowait_win->owners;
			GWaitOwner* owner;

			while((owner = *powner))
			{
				if(owner->owner == msg.dst)
				{
					*powner =  owner->next;
					delete owner;
					break;
				}
				powner =  &owner->next;
			}
		}

		if(!msg.dst->message(msg) && msg.code == WM_KEY)   						// send message to desktop
			Gdesktop->message(msg);

		if(GWait::dowait_win && msg.dst && msg.dst != GWait::dowait_win && GWait::dowait_cnt)
		{
			GWaitOwner *tmp = GWait::dowait_win->owners;
			if(msg.code == WM_INIT && msg.lparam)
			{
				// msg.lparam set only if window object
				bool hide = true;
				while(tmp)
				{
					if(tmp->owner == msg.dst && msg.dst->is_available())
					{
						hide = false;
						break;
					}
					tmp = tmp->next;
				}
				if(hide)
				{
					GWait::dowait_win->KillTimer(ID_BUSY_CLOCK);
					if( GWait::dowait_win->flags & GO_FLG_SHOW)
					{
						GWait::dowait_win->flags &= ~GO_FLG_SHOW;
						GWindow* win = (GWindow *)(GWait::dowait_win->parent->children);
						while(win)
						{
							if((win->flags & GO_FLG_SHOW) && (win->displays & GWait::dowait_win->displays))
								send_message(WM_DRAW, 0, GWait::dowait_win->rect.as_int, win);
							win = (GWindow *)win->nextObj;
						}
					}
				}
			}
			if(msg.code == WN_DESTROY)
			{
				while(tmp)
				{
					if(tmp->owner == Gdesktop->parent->focus && !(GWait::dowait_win->flags & GO_FLG_SHOW))
					{
						GWait::dowait_win->SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
						break;
					}
					tmp = tmp->next;
				}
			}
		}
#if GUI_DEBUG
		TRACE("\e[4;1;32m %d ms\e[m", ms_since(t0));
#endif
	}
}

void send_message(WM_MESSAGE wm,
		unsigned int param, unsigned long long lparam, GObject* dst)
{
	while(!GQueue.push(GMessage(wm, param, lparam, dst)))
		processes_all_messages();
}

