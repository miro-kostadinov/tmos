/*
 * gdialog.cpp
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#include <gdialog.h>



unsigned int GDialog::process_key (GMessage msg)
{
	if(msg.param == KEY_ESC)
	{
		close (GO_IDCANCEL);
		return 1;
	}
	if(focus && focus->message(msg))
		return 1;
	switch (msg.param)
	{
	case KEY_RIGHT:
	case KEY_DOWN:
		if (focus)
		{
			if (focus->nextObj)
				focus->nextObj->get_focus();
		else
			if (children)
				children->get_focus();
		}
		else
		{
			if (children)
			{
				children->get_focus();
			}
			else
				return 0;
		}
		return 1;
	case KEY_LEFT:
	case KEY_UP:
		if (!focus)
		{
			if (children)
				focus = children;
			else
				return 0;
		}
		if (focus == children)
		{
			GObject* tmp = focus;
			while (tmp->nextObj)
				tmp = tmp->nextObj;
			tmp->get_focus();
		}
		else
		{
			GObject* tmp = children;
			while (tmp->nextObj && tmp->nextObj != focus)
				tmp = tmp->nextObj;
			if (tmp->nextObj == focus)
				tmp->get_focus();
		}
		return 1;
	default:
		break;
	}
	return 0;
}

unsigned int GDialog::process_command (GMessage msg)
{
	switch(msg.param)
	{
	case GO_IDOK:
		close(GO_IDOK);
		return 1;
	case GO_IDCANCEL:
		close(GO_IDCANCEL);
		return 1;
	}
	return 0;
}

unsigned int GDialog::process_default (GMessage msg)
{
	switch (msg.code)
	{
	case WM_CHANGE:
		if (((GObject*)msg.param)->flags & GO_FLG_CHECKED)
		{
			((GObject*)msg.param)->clr_flag (GO_FLG_CHECKED);
		}
		else
			((GObject*)msg.param)->set_flag (GO_FLG_CHECKED);
		return 1;
	}
	return 0;
}

