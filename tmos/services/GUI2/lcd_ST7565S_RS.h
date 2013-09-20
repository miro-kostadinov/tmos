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
	void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows);
	void draw_hline(unsigned int x0, unsigned int x1, unsigned int y);
	void draw_bline(unsigned int x0, unsigned int x1, unsigned int y);
	void draw_vline(unsigned int y0, unsigned int y1, unsigned int x);
	void invert_vline(unsigned int y0, unsigned int y1, unsigned int x);
	void invert_hline(unsigned int x0, unsigned int x1, unsigned int y);
	virtual void update_screen();
	char inline revert_char (char ch);
};


#endif /* LCD_ST7565S_RS_H_ */
