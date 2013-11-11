/*
 * gdialog.h
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#ifndef GDIALOG_H_
#define GDIALOG_H_

#include <tmos.h>
#include <gwindow.h>

struct GDialog : GWindow
{
	GDialog(GId id_t, const RECT_T& rect_t,
			unsigned char displays_t, GFlags flags_t = GO_FLG_DEFAULT)
	:GWindow (id_t, rect_t, displays_t, flags_t) {};

	virtual unsigned int process_key (GMessage& msg);
	virtual unsigned int process_default (GMessage& msg);
	virtual unsigned int process_command(GMessage& msg);

};


#endif /* GDIALOG_H_ */
