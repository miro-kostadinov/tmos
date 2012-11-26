/*
 * lcd_ST7565S.h
 *
 *  Created on: 04.02.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LCD_ST7565S_H_
#define LCD_ST7565S_H_

#include <lcd.h>

#define ST7565S_LINES 32
#define ST7565S_COLUMNS 1	32

struct ST7565S: public LCD_MODULE
{
	unsigned int reset_timeout;
	unsigned char disp_buf[132];

	ST7565S(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
		LCD_MODULE(x, y, hnd, p), reset_timeout(0)
	{
	}
	;

	void lcd_command(unsigned int cmd);

	//virtual functions
	void lcd_init(GUI_CB splash);
	void lcd_reset();
	void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows);
	void draw_hline(unsigned int x0, unsigned int x1, unsigned int y);
	void draw_bline(unsigned int x0, unsigned int x1, unsigned int y);
	void draw_vline(unsigned int y0, unsigned int y1, unsigned int x);
	void invert_vline(unsigned int y0, unsigned int y1, unsigned int x);
	void invert_hline(unsigned int x0, unsigned int x1, unsigned int y);
	void update_screen();
	void clear_screen();
	void redraw_screen(WINDOW desktop);
};

#endif /* LCD_ST7565S_H_ */
