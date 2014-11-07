/*
 * lcd_ST7565S.h
 *
 *  Created on: 04.02.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LCD_ST7565S_H_
#define LCD_ST7565S_H_

#include <lcd.h>

/// V0 voltage regulator internal resistance ratio register value (0-7)
#define ST7565S_RaRb_RATIO(x) (x)
///electric volume register value (0-63)
#define ST7565S_VOLUME(x)  ((x)<< 8)
///start Column Address Set(0..131)
#define ST7565S_START_COLUMN(x)  ((x)<< 16)
#define ST7565S_GET_START_COLUMN(x)  (((x)>> 16)&0xFF)

#define ST7565S_H_FLIP		(0x1000000)	//!< Flip horizontally
#define ST7565S_V_FLIP		(0x2000000)	//!< Flip vertically
#define ST7565S_BIAS2		(0x4000000)	//!< LCD Bias Set (1/7 1/6 ..)


struct ST7565S: public LCD_MODULE
{
	unsigned int reset_timeout;
	unsigned int lcd_mode;
	unsigned char disp_buf[8][132];

	ST7565S(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p, unsigned int m):
		LCD_MODULE(x, y, hnd, p), reset_timeout(0), lcd_mode(m)
	{
	}
	;

	virtual void lcd_command(unsigned int cmd);

	//virtual functions
	void lcd_reset();
	void draw_bitmap(int x0, int y0, const char* src,  int width,  int rows);
	void draw_point( int x,  int y);
	void draw_hline( int x0,  int x1,  int y);
	void draw_bline( int x0,  int x1,  int y);
	void draw_vline( int y0,  int y1,  int x);
	void invert_vline( int y0,  int y1,  int x);
	void invert_hline( int x0,  int x1,  int y);
	virtual void update_screen();
	void clear_screen();
	void redraw_rect (GObject* object, RECT_T area);
	void direct_write (GSplash draw_cb);
	void adjust_for_screen (GObject** object, RECT_T &area);
};

#endif /* LCD_ST7565S_H_ */
