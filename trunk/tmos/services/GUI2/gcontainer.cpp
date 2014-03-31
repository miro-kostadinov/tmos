/*
 * gcontainer.cpp
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#include <gcontainer.h>
#include <tmos.h>
#include <lcd.h>


unsigned int GContainer::initialize (GMessage& msg)								//sets the client rectangle and calls the initialize of the children
{
	client_rect = rect;
	if(flags & GO_FLG_BORDER)
		allocate_border();

	for (GObject* tmp = children; tmp; tmp = tmp->nextObj)
	{
		tmp->initialize(msg);
	}
	if((flags & GO_FLG_SELECTED) && is_available() && parent)
		parent->focus = this;
	return 0;
}

unsigned int GContainer::process_idle(GMessage& msg)								//broadcasts the WM_IDLE message
{
	for (GObject* tmp = children; tmp; tmp = tmp->nextObj)
	{
		tmp->process_idle(msg);
	}
	return 0;
}

unsigned int GContainer::process_default(GMessage& msg)
{
	if(parent)
		return parent->process_default(msg);
	return 0;
}

unsigned int GContainer::process_command(GMessage& msg)
{
	if(parent)
		return parent->process_default(msg);
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

GObject* GContainer::get_object(GId xid)
{
	if(id != xid)
	{
		GObject* tmp = children;
		while (tmp)
		{
			if(tmp->id == xid)
				return tmp;
			tmp = tmp->nextObj;
		}
		return GObject::get_object(xid);
	}
	return this;
}

bool GContainer::is_available()
{
	if((flags & (GO_FLG_ENABLED|GO_FLG_SHOW)) == (GO_FLG_ENABLED|GO_FLG_SHOW))
	{
		GObject* tmp = children;
		while (tmp)
		{
			if(tmp->is_available())
				return true;
			tmp = tmp->nextObj;
		}
	}
	return false;
}

bool GContainer::focus_on_previous()
{
	GObject* tmp;
	if (!focus)
	{
		tmp = children;
		while (tmp)
		{
			if(tmp->is_available())
			{
				if(tmp->get_focus())
					return true;
			}
			tmp = tmp->nextObj;
		}
		return false;
	}
	else
	{
		tmp = focus;
		GObject* last = children;
		while(tmp && last)
		{
			if(last->is_available())
			{
				if(last == focus)
				{
					if(tmp == focus)
					{
						while(last)
						{
							if(last->is_available())
								tmp = last;
							last = last->nextObj;
						}
					}
					if(tmp && tmp->get_focus())
						return true;
				}
				tmp = last;

			}
			last = last->nextObj;
		}
	}
	return false;
}

bool GContainer::focus_on_next()
{
	GObject* tmp;
	if (focus)
	{
		tmp = focus->nextObj;
		while(tmp)
		{
			if(tmp->is_available())
			{
				if(tmp->get_focus())
					return true;
			}
			tmp = tmp->nextObj;
		}
	}
	tmp = children;
	while (tmp)
	{
		if(tmp->is_available())
		{
			if(tmp->get_focus())
				return true;
		}
		tmp = tmp->nextObj;
	}
	return false;
}

GObject* GContainer::first_available()
{
	GObject* tmp = children;
	while (tmp)
	{
		if(tmp->is_available())
			break;
		tmp = tmp->nextObj;
	}
	return tmp;
}

GObject* GContainer::last_available()
{
	GObject* tmp = first_available();
	GObject* last = NULL;
	while (tmp)
	{
		if(tmp->is_available())
			last = tmp;
		tmp = tmp->nextObj;
	}
	return last;
}

bool GContainer::close()
{
	if(GObject::close())
	{
		GObject* tmp = children;
		while(tmp)
		{
			tmp->close();
			tmp = tmp->nextObj;
		}
		return true;
	}
	return false;
}

bool GContainer::close (GObject* toClose)
{
	GObject* tmp = children;
	if(tmp)
	{
		if (children == toClose)
		{
			children = children->nextObj;
			if (toClose == focus)
				set_focus_last();
			return true;
		}
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
	}
	return false;
}

bool GContainer::set_flag(GFlags val)
{
	if((flags & val) != val )
	{
		flags |= val;
		if(val & GO_FLG_BORDER)
		{
			allocate_border();
		}
		if(val & ~(GO_FLG_SELECTED|GO_FLG_ENABLED))
			send_message(WM_DRAW, 0, 0L, this);
		return true;
	}
	return false;
}

bool GContainer::clr_flag(GFlags val)
{
	if(flags & val)
	{
		flags &= ~val;
		if(val & GO_FLG_BORDER)
		{
			client_rect = rect;
		}
		if(val & ~(GO_FLG_SELECTED|GO_FLG_ENABLED))
			send_message(WM_DRAW, 0, 0L, this);
		return true;
	}
	return false;
}

bool GContainer::get_focus(bool notify_msg)
{
	if(GObject::get_focus(notify_msg))
	{
		if (focus)
		{
			if(focus->is_available())
			{
				focus->set_flag(GO_FLG_SELECTED);
				return true;
			}
		}
		else
		{
			GObject* tmp = first_available();
			if(tmp && tmp->get_focus())
				return true;
		}
	}
	return false;
}

bool GContainer::set_focus_first ()
{
	focus = first_available();
	if(focus)
		focus->set_flag(GO_FLG_SELECTED);
	return (focus);
}

bool GContainer::set_focus_last ()
{
	focus = last_available();
	if(focus)
		focus->set_flag(GO_FLG_SELECTED);
	return (focus);
}

void GContainer::draw (LCD_MODULE* lcd, RECT_T area)
{
	if (area.normalize(rect))
	{
		lcd->redraw_rect(this, area);//draw_this (lcd);
		GObject* tmp = children;
		while (tmp)
		{
			if(tmp->flags & GO_FLG_SHOW)
				tmp->draw(lcd, area);
			tmp = tmp->nextObj;
		}
	}
}

void GContainer::draw_this(LCD_MODULE* lcd)
{
//	clear_rect(rect);
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

