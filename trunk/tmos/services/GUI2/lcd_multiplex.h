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

	void invalidate (GObject* object, RECT_T area);
	virtual void LPtoDP(POINT_T& size, unsigned char lcd_index=0);
	virtual void DPtoLP(POINT_T& size, unsigned char lcd_index=0);
};


#endif /* LCD_MULTIPLEX_H_ */
