/*
 * lcd_ST7735R.h
 *
 *  Created on: 14.04.2011
 *      Author: stanly
 */

#ifndef LCD_ST7735R_H_
#define LCD_ST7735R_H_

#include <lcd_SPFD5414D.h>

struct ST7735R : public SPFD5414D
{

    ST7735R(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
    	SPFD5414D(x, y, hnd, p)
	{
	}
	;

	//virtual functions
	virtual void lcd_init(GUI_CB splash);
	virtual void lcd_reset();
};

#endif /* LCD_ST7735R_H_ */
