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
#include <gscroll.h>

struct GDialog : GWindow
{
	GScroll* 	vscroll;
	int			vpos;

	GDialog():GWindow(), vscroll(NULL),vpos(0) {};
	GDialog(GId id_t, const RECT_T& rect_t,
			unsigned char displays_t, GFlags flags_t = GO_FLG_DEFAULT)
	:GWindow (id_t, rect_t, displays_t, flags_t), vscroll(NULL), vpos(0)
	{
	};

	virtual ~GDialog()
	{
		if(vscroll)
		{
			delete vscroll;
			vscroll = NULL;
		}
	}
	virtual unsigned int process_key (GMessage& msg);
	virtual unsigned int process_default (GMessage& msg);
	virtual unsigned int process_command(GMessage& msg);

protected:
	virtual void draw_this (LCD_MODULE* lcd);
	virtual unsigned int initialize (GMessage& msg);

};


#endif /* GDIALOG_H_ */
