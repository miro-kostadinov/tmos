/*
 * gcontainer.cpp
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#include <gcontainer.h>
#include <tmos.h>
#include <lcd.h>


unsigned int GContainer::initialize (GMessage msg)								//sets the client rectangle and calls the initialize of the children
{
	if(flags & GO_FLG_BORDER)
	{
		client_rect.x0 = rect.x0 +1;
		client_rect.y0 = rect.y0 +1;
		client_rect.x1 = rect.x1 -1;
		client_rect.y1 = rect.y1 -1;
	}
	for (GObject* tmp = children; tmp; tmp = tmp->nextObj)
	{
		tmp->initialize(msg);
	}
	if((flags & GO_FLG_SELECTED) && parent)
		parent->focus = this;
	return 0;
}

unsigned int GContainer::process_idle(GMessage msg)								//broadcasts the WM_IDLE message
{
	for (GObject* tmp = children; tmp; tmp = tmp->nextObj)
	{
		tmp->process_idle(msg);
	}
	return 0;
}

GObject* GContainer::addChild (GObject* child)
{
	GObject* tmp = children;
	if (tmp)
	{
		while (tmp->nextObj)
			tmp = tmp->nextObj;
		tmp->nextObj = child;
	}
	else
		children = child;
	child->parent = this;
	return child;
}

bool GContainer::close (GObject* toClose)
{
	if (children == toClose)
	{
		if (children->nextObj)
			children = children->nextObj;
		if (toClose == focus)
			set_focus_last();
		return true;
	}
	GObject* tmp = children;
	while (tmp->nextObj)
	{
		if (toClose == tmp->nextObj)
		{
			tmp->nextObj = toClose->nextObj;
			if (toClose == focus)
				set_focus_last();
			return true;
		}
		tmp = tmp->nextObj;
	}
	return false;
}

bool GContainer::close(unsigned char closeID)
{
	GObject* tmp = children;
	while (tmp)
	{
		if (tmp->id == closeID)
			return close(tmp);
		tmp = tmp->nextObj;
	}
	return false;
}

void GContainer::get_focus()
{
	GObject::get_focus();
	if (focus)
		focus->set_flag(GO_FLG_SELECTED);
}

bool GContainer::set_focus_first ()
{
	if (children)
	{
		focus = children;
		return true;
	}
	return false;
}

bool GContainer::set_focus_last ()
{
	focus = children;
	while (focus->nextObj)
		focus = focus->nextObj;
	if (!focus)
		return false;
	return true;
}

bool GContainer::set_focus(GObject* focused)
{
	GObject* tmp = children;
	while (tmp)
	{
		if (tmp == focused)
		{
			focus = tmp;
			return true;
		}
		tmp = tmp->nextObj;
	}
	return false;
}

bool GContainer::set_focus(unsigned char id_t)
{
	GObject* tmp = children;
	while (tmp)
	{
		if (tmp->id == id_t)
		{
			focus = tmp;
			return true;
		}
		tmp = tmp->nextObj;
	}
	return false;
}

void GContainer::draw (LCD_MODULE* lcd, RECT_T area)
{
	if (area.normalize(rect))
	{
		lcd->redraw_rect(this, area);//draw_this (lcd);
		GObject* tmp = children;
		while (tmp)
		{
			tmp->draw(lcd, area);
			tmp = tmp->nextObj;
		}
	}
}

void GContainer::draw_this(LCD_MODULE* lcd)
{
//	clear_rect(rect.x0, rect.y0, rect.x1, rect.y1);
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
}

GContainer::~GContainer ()
{
	GObject* tmp;
	while (children)
	{
		tmp = children;
		children = children->nextObj;
		delete tmp;
	}
}

