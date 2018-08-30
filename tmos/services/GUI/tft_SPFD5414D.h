/*
 * lcd_SPFD5414D.h
 *
 *  Created on: 30.01.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LCD_SPFD5414D_H_
#define LCD_SPFD5414D_H_

#include <lcd.h>

#define SPFD5414D_NOP 				0x00
#define SPFD5414D_DISPLAYOFF 		0x28
#define SPFD5414D_DISPON 			0x29	//!< [0] Display Om
#define SPFD5414D_SLPOUT 			0x11	//!< [0] Sleep Out
#define SPFD5414D_NORON				0x13	//!< [0] Normal Display On

#define SPFD5414D_PWCTR1			0xC0	//!< [2] Power Control 1 (VRH, VCI)

#define SPFD5414D_SWRESET 			0x01
#define SPFD5414D_RDDPM 			0x0a
#define SPFD5414D_RDDMADCTR 		0x0b
#define SPFD5414D_RDDCOLMOD 		0x0c
#define SPFD5414D_RDDCIM 			0x0d
#define SPFD5414D_RDID4 			0xd3
#define SPFD5414D_CASET 			0x2a	//!< [4] Column Address Set (XS, XE)
#define SPFD5414D_RASET 			0x2b	//!< [4] Row Address Set (YS, YE)
#define SPFD5414D_RAMWR 			0x2c	//!< [N] Memory Write
#define SPFD5414D_RAMRD 			0x2e
#define SPFD5414D_RDDSDR 			0x0f
#define SPFD5414D_RDDID 			0x04
#define SPFD5414D_RDCOLMD 			0x0c
#define SPFD5414D_RDDST 			0x09
#define SPFD5414D_COLMOD 			0x3a	//!< [1] Interface Pixel Format (color mode)
#define SPFD5414D_MADCTR 			0x36	//!< [1] Memory Data Access Control
#define SPFD5414D_RGBSET 			0x2d

#define SPFD5414D_DATA(data)		(0x0100 |(data))
#define SPFD5414D_CMD(cmd)			(cmd)

#define SPFD5414D_COLMOD_12BPP		0x3
#define SPFD5414D_COLMOD_16BPP		0x5
#define SPFD5414D_COLMOD_18BPP		0x6

#define SPFD5414D_MADCTR_MY			0x80	//!< Row address order
#define SPFD5414D_MADCTR_MX			0x40	//!< Column address order
#define SPFD5414D_MADCTR_MV			0x20	//!< Row/Column exchange
#define SPFD5414D_MADCTR_ML			0x10	//!< Vertical refresh order
#define SPFD5414D_MADCTR_BGR		0x08	//!< RGB-BGR order (1=BGR)

// base TFT module
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
	void lcd_init(GUI_CB splash) override;
	void lcd_reset() override; // call tft_reset()
	void draw_bitmap(unsigned int x0, unsigned int y0,
			const unsigned char* src, unsigned int width, unsigned int rows) override;
	void draw_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_bline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void draw_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_vline(unsigned int y0, unsigned int y1, unsigned int x) override;
	void invert_hline(unsigned int x0, unsigned int x1, unsigned int y) override;
	void update_screen() override // not used into TFT modules
		{;};
	void clear_screen() override;
	void redraw_screen(WINDOW desktop) override;
	//The TFT modules hardware interface methods
protected:
	virtual void tft_reset();
	virtual void tft_write_row(unsigned short row_address_cmd[], unsigned short row);
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

	static void delay(unsigned int time = 0);
	void tft_write(unsigned int value);
	unsigned int tft_read();

	unsigned int read_id();
	unsigned int id();
};

#endif /* LCD_SPFD5414D_H_ */
