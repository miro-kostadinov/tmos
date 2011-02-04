/*
 * lcd_SPFD5414D.h
 *
 *  Created on: 30.01.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LCD_SPFD5414D_H_
#define LCD_SPFD5414D_H_

#include <lcd.h>

struct SPFD5414D: public LCD_MODULE
{
	unsigned int reset_timeout;
	unsigned int* disp_buf;
    unsigned int video_buf[256];

	SPFD5414D(unsigned int x, unsigned int y, HANDLE hnd, const LCD_PINS* p) :
		LCD_MODULE(x, y, hnd, p), reset_timeout(0)
	{
	}
	;

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

/**
 * TFT_CHECK class can be used to detect the LCD MODULE installed.
 * It is using GPIOs only so it can be called from AppInit()
 *
 */
struct TFT_CHECK
{
	PIN_DESC SCL;
	PIN_DESC SDA;
	PIN_DESC CSX;
	PIN_DESC RST;

	TFT_CHECK(int clk, int dat, int cs, unsigned rs) :
		SCL(clk), SDA(dat), CSX(cs), RST(rs)
	{
	}
	;

	void delay();
	void tft_write(unsigned int value);
	unsigned int tft_read();

	unsigned int read_id();
};

#endif /* LCD_SPFD5414D_H_ */
