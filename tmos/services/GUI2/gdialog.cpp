/*
 * gdialog.cpp
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#include <gdialog.h>




unsigned int GDialog::process_key (GMessage& msg)
{
	if(GWindow::process_key(msg))
		return 1;

	switch (msg.param)
	{
	case KEY_RIGHT:
	case KEY_DOWN:
		return focus_on_next();

	case KEY_LEFT:
	case KEY_UP:
		return focus_on_previous();

	default:
		break;
	}
	return 0;
}

unsigned int GDialog::process_command (GMessage& msg)
{
	switch(msg.param)
	{
	case GO_IDOK:
		notify_message(WM_CLOSE, GO_IDOK, this);								//closes the window
		return 1;
	case GO_IDCANCEL:
		notify_message(WM_CLOSE, GO_IDCANCEL, this);							//closes the window
		return 1;
	default:
		notify_message(WM_COMMAND, msg.param, NULL);
		return 0;
	}
	return 0;
}

unsigned int GDialog::process_default (GMessage& msg)
{
	switch (msg.code)
	{
	case WM_CHANGE:
	{
		GObject* obj = (GObject*)msg.param;
		if (obj->flags & GO_FLG_CHECKED)
			obj->clr_flag (GO_FLG_CHECKED);
		else
			obj->set_flag (GO_FLG_CHECKED);
		notify_message(WM_CHANGE, obj->id, obj);
		return 1;
	}
	default:
		return 0;
	}
	return 0;
}

