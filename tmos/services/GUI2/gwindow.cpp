/*
 * gwindow.cpp
 *
 *  Created on: 21.08.2013
 *      Author: edguest
 */

#include <gwindow.h>
#include <tmos.h>

void GWindow::draw_this(LCD_MODULE* lcd)
{
	clear_rect(rect.x0, rect.y0, rect.x1, rect.y1);								//clears the area
	if(flags & GO_FLG_BORDER)
		draw_border(rect);
}

unsigned int GWindow::initialize (GMessage msg)
{
	GContainer::initialize(msg);												//initializes both the window and its items
	send_message(GMessage(WM_DRAW, 0, 0L, this));								//initial draw of the window and its items
	return 0;
}

unsigned int GWindow::process_key (GMessage msg)
{
	switch (msg.param)
	{
	case KEY_ESC:
		return close(0);														//closes the window
	default:
		return RES_OK;
	}
}

unsigned int GWindow::DoModal()
{
	if(hnd.tsk_open(GUI_DRV_INDX, this))
	{
		hnd.tsk_read(NULL,0);
		return hnd.dst.as_int;
	}
	return 0;
}

bool GWindow::close (unsigned int reason)
{
	if (parent && parent->close(this))											//detaches the window from the Z list
		send_message(GMessage(WM_DRAW, 0, rect.as_int, parent));				//send a message to redraw the area covered by the window
	hnd.dst.as_int = reason;
	usr_HND_SET_STATUS(&hnd, FLG_SIGNALED);										//signals the window thread
	return false;
}
