/*
 * gclockview.h
 *
 *  Created on: Apr 10, 2023
 *      Author: bratkov
 */

#ifndef GCLOCKVIEW_H_
#define GCLOCKVIEW_H_

#include <stdgui.h>
#include <lcd.h>
#include <gobject.h>
#include <tmos_time.h>

struct GClockView:GObject
{
	POINT_T min, hour, center, displacement;
	int16_t R;

	GClockView(GId id_t, const RECT_T& rect_t,  GFlags flags_t = GO_FLG_SHOW):
		GObject(id_t, rect_t, flags_t), R(0)
	{;}

	void set_clock(const time_t time);

	unsigned int initialize (GMessage& msg) override;
	void draw_this(LCD_MODULE* lcd) override;
	unsigned int process_default (GMessage& msg) override;
};



#endif /* GCLOCKVIEW_H_ */
