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
	ST7565S_RS(	unsigned int x, unsigned int y,
				unsigned int dx, unsigned int dy,
				HANDLE hnd, const PIN_DESC* p, unsigned int m):
		ST7565S(x, y, dx, dy, hnd, p, m)
	{
	}
	;


	//virtual functions
	virtual void lcd_command(unsigned int cmd);
	void draw_point( int x,  int y);
	void draw_bitmap( int x0, int y0, const char* src, int width, int rows);
	void draw_hline( int x0,  int x1,  int y);
	void draw_bline( int x0,  int x1,  int y);
	void draw_vline( int y0,  int y1,  int x);
	void invert_vline( int y0,  int y1,  int x);
	void invert_hline( int x0,  int x1,  int y);
	virtual void update_screen();
	char inline revert_char (char ch);
};


#endif /* LCD_ST7565S_RS_H_ */
