/*
 * gclockview.cpp
 *
 *  Created on: Apr 10, 2023
 *      Author: bratkov
 */

#include <gclockview.h>


void GClockView::set_clock(time_t time)
{
	if(R > 5)
	{
		min = PolarToDP(270 + time.minute*6, (R*3)/4) + center;
		hour = PolarToDP(270 + (time.hour%12)*30 + time.minute/2, R/2 +1) + center;
	}
}

unsigned int GClockView::initialize (GMessage& msg)
{
	GObject::initialize(msg);

	R = client_rect.height();
	if(client_rect.width() < R )
		R = client_rect.width();
	R /= 2;
	if((flags & GO_FLG_BORDER) && R )
		R--;

	if(R > 5)
	{
		displacement = POINT_T(R,R);
		LPtoDP(displacement);
		center = displacement + client_rect.p0;

		set_clock(get_current_time());
		SetTimer(id, 1000);
	}
	send_message(WM_DRAW, 0, 0L, this);
	return 0;
}

void GClockView::draw_this(LCD_MODULE* lcd)
{
	POINT_T p;
	if(R > 5)
	{

		draw_ellipse(center, displacement.x, displacement.y);
		draw_line(center, min);
		draw_line(center, hour);

		for(int i=0; i < 12; i++)
		{
			p = lcd->PolarToDP(i*30, R-2);
			p += center;
			draw_point(p.x, p.y);
		}
	}
}


unsigned int GClockView::process_default (GMessage& msg)
{
	if(msg.code == WM_TIMER)
	{
		if(msg.param == id)
		{
			SetTimer(id, 1000);
			time_t time = get_current_time();
			if(!time.second)
			{
				set_clock(time);
				send_message(WM_DRAW, 0, 0L, this);
			}
		}
	}
	return GObject::process_default(msg);
}

