/*
 * message.h
 *
 *  Created on: 03.09.2013
 *      Author: edguest
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdgui.h>

struct GObject;

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


#endif /* MESSAGE_H_ */
