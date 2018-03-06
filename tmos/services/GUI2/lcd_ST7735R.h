/*
 * lcd_ST7735R.h
 *
 *  Created on: 14.04.2011
 *      Author: stanly
 */

#ifndef LCD_ST7735R_H_
#define LCD_ST7735R_H_

#include <lcd_SPFD5414D.h>

#define ST7735R_NOP 			0
#define ST7735R_DISPLAYOFF 		0x28
#define ST7735R_DISPON 			0x29	//!< [0] Display Om
#define ST7735R_SLPOUT 			0x11	//!< [0] Sleep Out
#define ST7735R_NORON			0x13	//!< [0] Normal Display On

#define ST7735R_PWCTR1			0xC0	//!< [2] Power Control 1 (VRH, VCI)

#define ST7735R_SWRESET 		0x01
#define ST7735R_RDDPM 			0x0a
#define ST7735R_RDDMADCTR 		0xb
#define ST7735R_RDDCOLMOD 		0xc
#define ST7735R_RDDCIM 			0xd
#define ST7735R_RDID4 			0xd3
#define ST7735R_CASET 			0x2a	//!< [4] Column Address Set (XS, XE)
#define ST7735R_RASET 			0x2b	//!< [4] Row Address Set (YS, YE)
#define ST7735R_RAMWR 			0x2c	//!< [N] Memory Write
#define ST7735R_RGBSET			0x2d
#define ST7735R_RAMRD 			0x2e
#define ST7735R_RDDSDR 			0x0f
#define ST7735R_RDDID 			0x04
#define ST7735R_RDCOLMD 		0x0c
#define ST7735R_RDDST 			0x09
#define ST7735R_COLMOD 			0x3a	//!< [1] Interface Pixel Format (color mode)
#define ST7735R_MADCTR 			0x36	//!< [1] Memory Data Access Control

#define ST7735R_FRMCTR1			0xB1
#define ST7735R_FRMCTR2			0xB2
#define ST7735R_FRMCTR3			0xB3

#define ST7735R_INVCTR			0xB4
#define ST7735R_PWCTR1			0xC0
#define ST7735R_PWCTR2			0xC1
#define ST7735R_PWCTR3			0xC2
#define ST7735R_PWCTR4			0xC3
#define ST7735R_PWCTR5			0xC4
#define ST7735R_VMCTR1			0xC5
#define ST7735R_GAMCTRP1		0xE0
#define ST7735R_GAMCTRN1		0xE1
#define ST7735R_DISSET5			0xB6
#define ST7735R_EXTCTRL			0xF0

#define ST7735R_DATA(data)		(0x0100 |(data))
#define ST7735R_CMD(cmd)		(cmd)

#define ST7735R_COLMOD_12BPP	0x3
#define ST7735R_COLMOD_16BPP	0x5
#define ST7735R_COLMOD_18BPP	0x6

#define ST7735R_MADCTR_MY		0x80	//!< Row address order
#define ST7735R_MADCTR_MX		0x40	//!< Column address order
#define ST7735R_MADCTR_MV		0x20	//!< Row/Column exchange
#define ST7735R_MADCTR_ML		0x10	//!< Vertical refresh order
#define ST7735R_MADCTR_BGR		0x08	//!< RGB-BGR order (1=BGR)

struct ST7735R : public SPFD5414D
{

    ST7735R(unsigned int x, unsigned int y,
    		unsigned int dx, unsigned int dy,
    		HANDLE hnd, const PIN_DESC* p) :
    	SPFD5414D(x, y, dx, dy, hnd, p)
	{
	}
	;

	//virtual functions
	virtual void lcd_reset();
	virtual void redraw_screen(GObject* object, RECT_T area);

};

#endif /* LCD_ST7735R_H_ */
