/*
 * lcd_multiplex.h
 *
 *  Created on: 28.08.2013
 *      Author: edguest
 */

#ifndef LCD_MULTIPLEX_H_
#define LCD_MULTIPLEX_H_

#include <stdgui.h>
#include <lcd.h>

struct LCD_MULT: GContainer
{
	LCD_MODULE* lcd[GUI_DISPLAYS];

	GUI_GET_OBJECT_TYPE(OBJECT_DISPLAY_MULTIPLEXER);

	void invalidate (GObject* object, RECT_T area) override;
	void LPtoDP(POINT_T& size, const unsigned char lcd_index=0) const override;
	void DPtoLP(POINT_T& size, const unsigned char lcd_index=0) const override;
	POINT_T PolarToDP(const int deg, const int r, const unsigned char lcd_index=0) const override;
protected:
	void redraw_screen(GObject* object, RECT_T area) override;
	inline virtual unsigned int is_lcd() const override __attribute__((optimize("Os"), always_inline))
	{
		return 1;
	}

};


#endif /* LCD_MULTIPLEX_H_ */
