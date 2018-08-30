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

#define ST7565S_H_FLIP		(0x1000000)	//!< Flip horizontally
#define ST7565S_V_FLIP		(0x2000000)	//!< Flip vertically
#define ST7565S_BIAS2		(0x4000000)	//!< LCD Bias Set (1/7 1/6 ..)


struct ST7565S: public LCD_MODULE
{
	unsigned int reset_timeout;
	unsigned int lcd_mode;
	unsigned char disp_buf[132];

	ST7565S(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p, unsigned int m):
		LCD_MODULE(x, y, hnd, p), reset_timeout(0), lcd_mode(m)
	{
	}
	;

	virtual void lcd_command(unsigned int cmd);

	//virtual functions
	void lcd_init(GUI_CB splash) override;
	void lcd_reset() override;
	void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows) override;
	void draw_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_bline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	virtual void update_screen() override;
	void clear_screen() override;
	void redraw_screen(WINDOW desktop) override;
};

#endif /* LCD_ST7565S_H_ */
