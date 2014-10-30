/*
 * lcd_ST7565S_RS.h
 *
 *  Created on: 03.09.2013
 *      Author: stanly
 */

#ifndef LCD_ST7565S_RS_H_
#define LCD_ST7565S_RS_H_


#include <lcd_ST7565S.h>


struct ST7565S_RS: public ST7565S
{
	ST7565S_RS(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p, unsigned int m):
		ST7565S(x, y, hnd, p, m)
	{
	}
	;


	//virtual functions
	virtual void lcd_command(unsigned int cmd);
	virtual void update_screen();
};


#endif /* LCD_ST7565S_RS_H_ */
