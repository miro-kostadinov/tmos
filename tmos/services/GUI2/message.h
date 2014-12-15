/*
 * message.h
 *
 *  Created on: 03.09.2013
 *      Author: edguest
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdgui.h>
#include <mqueue.h>

struct GObject;
struct GWindow;

struct GMessage
{
	WM_MESSAGE code;
	unsigned int param;
	unsigned long long lparam;
	GObject* dst;

	GMessage () : code(WM_QUIT), param(0), lparam(0), dst(NULL) {};
	GMessage (WM_MESSAGE code_t, unsigned int param_t, unsigned long long lparam_t, GObject* destination):
		code (code_t), param (param_t), lparam (lparam_t), dst (destination){};
	GMessage (WM_MESSAGE code_t, unsigned int param_t, GObject* destination):
		code (code_t), param (param_t), lparam (0L), dst (destination){};


	GMessage& operator= (GMessage msg)
	{
		code 	= msg.code;
		param 	= msg.param;
		lparam 	= msg.lparam;
		dst		= msg.dst;
		return *this;
	}

};


template< const int size>
struct msgQueue : mqueue<GMessage, size>
{
	msgQueue(): mqueue<GMessage, size>()
		{;}

	bool del_msg_for(GObject* owner)
	{
		unsigned short indx = this->out;
		bool res = false;

		while(this->in != indx)
		{
			if(this->items[indx].dst == owner)
			{
#if GUI_DEBUG
				TRACELN1("\e[4;1;33m");
				TRACE("%X[%d] ( %s 0x%X ) deleted!\e[m",
						this->items[indx].dst, this->items[indx].dst->id,
						szlist_at(wm_dbg_str, this->items[indx].code), this->items[indx].param);
#endif
				this->items[indx].code = WM_DELETED;
				res = true;
			}
			if(++indx == size)
				indx=0;
		}
		return res;
	}

	bool del_msg(WM_MESSAGE code, GObject* owner)
	{
		unsigned short indx = this->out;
		bool res = false;

		while(this->in != indx)
		{
			if(this->items[indx].dst == owner && this->items[indx].code == code)
			{
#if GUI_DEBUG
				TRACELN1("\e[4;1;33m");
				TRACE("%X[%d] ( %s 0x%X ) deleted!\e[m",
						this->items[indx].dst, this->items[indx].dst->id,
						szlist_at(wm_dbg_str, this->items[indx].code), this->items[indx].param);
#endif
				this->items[indx].code = WM_DELETED;
				res = true;
			}
			if(++indx == size)
				indx=0;
		}
		return res;
	}
};

extern msgQueue<MAX_MESSAGES> GQueue;
extern GWindow* Gdesktop;

void processes_all_messages(void);
void send_message(WM_MESSAGE wm, unsigned int param, unsigned long long lparam, GObject* dst);


#endif /* MESSAGE_H_ */
