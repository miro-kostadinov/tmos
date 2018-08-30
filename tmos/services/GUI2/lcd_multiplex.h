/*
 * lcd_multiplex.h
 *
 *  Created on: 28.08.2013
 *      Author: edguest
 */

#ifndef LCD_MULTIPLEX_H_
#define LCD_MULTIPLEX_H_

#include <tmos.h>
#include <lcd.h>
#include <gcontainer.h>

struct LCD_MULT: GContainer
{
	LCD_MODULE* lcd[GUI_DISPLAYS];

	GUI_GET_OBJECT_TYPE(OBJECT_DISPLAY_MULTIPLEXER);

	void invalidate (GObject* object, RECT_T area) override;
	void LPtoDP(POINT_T& size, unsigned char lcd_index=0) override;
	void DPtoLP(POINT_T& size, unsigned char lcd_index=0) override;
protected:
	void redraw_screen(GObject* object, RECT_T area) override;
};


#endif /* LCD_MULTIPLEX_H_ */
