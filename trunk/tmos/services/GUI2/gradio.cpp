/*
 * gradio.cpp
 *
 *  Created on: 13.09.2013
 *      Author: edguest
 */

#include <gradio.h>

GObject* GRadio::addChild (GObject* child)
{
	GContainer::addChild(child);
	if(child->flags & GO_FLG_CHECKED)											//checks if the newly added child is configured as CHECKED
	{
		if(checked)
			child->flags &= ~GO_FLG_CHECKED;									//if there is already a checked item, exits
		else
			checked = child;													//else sets the new item as checked
	}
	return child;
}

unsigned int GRadio::process_key (GMessage msg)
{
	if(focus && focus->message(msg))
		return 1;
	switch (msg.param)
	{
	case KEY_RIGHT:
	case KEY_DOWN:
		if (focus)
		{
			if (focus->nextObj)
				focus->nextObj->get_focus();									//sets the focus on the next child
			else
			{
				focus->clr_flag(GO_FLG_SELECTED);								//if there is not next child, clears the SELECTED flag from the current focus
				return 0;
			}
		}
		else
		{
			if (children)
			{
				children->get_focus();											//if there isn't focus, sets the focus on the first child
			}
			else
				return 0;
		}
		return 1;
	case KEY_LEFT:
	case KEY_UP:
			if (children)
			{
				if (focus == children)
				{
					focus->clr_flag(GO_FLG_SELECTED);							//if the first child is focused, clears the SELECTED flag from it
					return 0;
				}
				GObject* tmp = children;
				while (tmp->nextObj && tmp->nextObj != focus)
					tmp = tmp->nextObj;											//sets the focus on the child before the previous focus
				tmp->get_focus();
				return 1;
			}
			break;
	default:
		break;
	}
	return 0;
}

unsigned int  GRadio::process_default (GMessage msg)
{
	if (msg.code == WM_CHANGE)
	{
		if (checked != (GObject*)msg.param)										//if the new checked item is same as the previous, exits the function with 1
		{
			if (checked)
				checked->clr_flag(GO_FLG_CHECKED);								//if there is already a checked item, clears its CHECKED flag
			checked = (GObject*)msg.param;
			checked->set_flag(GO_FLG_CHECKED);									//sets CHECKED flag on the new checked item
		}
		return 1;
	}
	return 0;
}
