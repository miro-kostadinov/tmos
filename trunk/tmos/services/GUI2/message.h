/*
 * message.h
 *
 *  Created on: 03.09.2013
 *      Author: edguest
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#define MAX_MESSAGES 10
#include <tmos.h>

struct GObject;

struct GMessage
{
	unsigned int code;
	unsigned int param;
	unsigned long long lparam;
	GObject* dst;

	GMessage () : code(0), param(0), lparam(0), dst(NULL) {};
	GMessage (unsigned int code_t, unsigned int param_t, unsigned long long lparam_t, GObject* destination):
		code (code_t), param (param_t), lparam (lparam_t), dst (destination){};
//	GMessage (unsigned int code_t, unsigned int param_t, GObject* destination) :
//		code (code_t), param (param_t), lparam (0), dst (destination){};
//	GMessage (unsigned int code_t, unsigned int param_t):
//		code (code_t), param (param_t), lparam (0), dst (NULL){};
//	GMessage (unsigned int code_t, unsigned int param_t, unsigned long long lparam_t):
//		code (code_t), param (param_t), lparam (lparam_t), dst (NULL){};
//	GMessage (unsigned int code_t, unsigned long long lparam_t, GObject* destination) :
//		code (code_t), param (0), lparam (lparam_t), dst (destination){};
};


#endif /* MESSAGE_H_ */
