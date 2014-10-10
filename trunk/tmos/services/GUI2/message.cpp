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

		if(!msg.dst->message(msg) && msg.code == WM_KEY)   						// send message to desktop
			Gdesktop->message(msg);
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

