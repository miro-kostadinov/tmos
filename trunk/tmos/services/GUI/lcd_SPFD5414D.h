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

	SPFD5414D(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
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

#ifndef CSX_PIN_INDX
#define CSX_PIN_INDX	2
#endif
#ifndef SCL_PIN_INDX
#define SCL_PIN_INDX	3
#endif
#ifndef SDA_PIN_INDX
#define SDA_PIN_INDX	4
#endif


/**
 * TFT_CHECK class can be used to detect the LCD MODULE installed.
 * It is using GPIOs only so it can be called from AppInit()
 *
 */
struct TFT_CHECK
{
	const PIN_DESC* pins;

	TFT_CHECK(const PIN_DESC* p) : pins(p)
	{
	}
	;

	void delay();
	void tft_write(unsigned int value);
	unsigned int tft_read();

	unsigned int read_id();
};

#endif /* LCD_SPFD5414D_H_ */
