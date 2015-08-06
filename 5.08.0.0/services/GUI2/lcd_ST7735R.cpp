/*
 * lcd_ST7735R.cpp
 *
 *  Created on: 14.04.2011
 *      Author: stanly
 */
#include <tmos.h>
#include <lcd_ST7735R.h>

#define TFT_NOP 			0
#define TFT_DISPLAYOFF 		0x28
#define TFT_DISPON 			0x29	//!< [0] Display Om
#define TFT_SLPOUT 			0x11	//!< [0] Sleep Out
#define TFT_NORON			0x13	//!< [0] Normal Display On

#define TFT_PWCTR1			0xC0	//!< [2] Power Control 1 (VRH, VCI)

#define TFT_SWRESET 		0x01
#define TFT_RDDPM 			0x0a
#define TFT_RDDMADCTR 		0xb
#define TFT_RDDCOLMOD 		0xc
#define TFT_RDDCIM 			0xd
#define TFT_RDID4 			0xd3
#define TFT_CASET 			0x2a	//!< [4] Column Address Set (XS, XE)
#define TFT_RASET 			0x2b	//!< [4] Row Address Set (YS, YE)
#define TFT_RAMWR 			0x2c	//!< [N] Memory Write
#define TFT_RAMRD 			0x2e
#define TFT_RDDSDR 			0x0f
#define TFT_RDDID 			0x04
#define TFT_RDCOLMD 		0x0c
#define TFT_RDDST 			0x09
#define TFT_COLMOD 			0x3a	//!< [1] Interface Pixel Format (color mode)
#define TFT_MADCTR 			0x36	//!< [1] Memory Data Access Control
#define TFT_RGBSET 			0x2d

#define TFT_DATA(data)		(0x0100 |(data))
#define TFT_CMD(cmd)		cmd

#define TFT_COLMOD_12BPP	0x3
#define TFT_COLMOD_16BPP	0x5
#define TFT_COLMOD_18BPP	0x6

#define TFT_MADCTR_MY		0x80	//!< Row address order
#define TFT_MADCTR_MX		0x40	//!< Column address order
#define TFT_MADCTR_MV		0x20	//!< Row/Column exchange
#define TFT_MADCTR_ML		0x10	//!< Vertical refresh order
#define TFT_MADCTR_BGR		0x08	//!< RGB-BGR order (1=BGR)


const unsigned short ST7735R_init[] =
{
	TFT_SLPOUT,

	//------------------------------------ST7735R Frame Rate-----------------------------------------//
	0xB1,	TFT_DATA(0x01),	TFT_DATA(0x2C),	TFT_DATA(0x2D),
	0xB2,	TFT_DATA(0x01),	TFT_DATA(0x2C),	TFT_DATA(0x2D),
	0xB3,	TFT_DATA(0x01),	TFT_DATA(0x2C),	TFT_DATA(0x2D),	TFT_DATA(0x01),	TFT_DATA(0x2C),	TFT_DATA(0x2D),
	//------------------------------------End ST7735R Frame Rate-----------------------------------------//
	0xB4, 	TFT_DATA(0x04), //Column inversion
	//------------------------------------ST7735R Power Sequence-----------------------------------------//
	0xB6, 	TFT_DATA(0xB4),	TFT_DATA(0xF0), //Extend gate non-overlap time
	0xC0,	TFT_DATA(0xA2),	TFT_DATA(0x02),	TFT_DATA(0x84),
	0xC1,	TFT_DATA(0xC5),
	0xC2,	TFT_DATA(0x0A),	TFT_DATA(0x00),
	0xC3,	TFT_DATA(0x8A),	TFT_DATA(0x2A),
	0xC4,	TFT_DATA(0x8A),	TFT_DATA(0xEE),
	//---------------------------------End ST7735R Power Sequence-------------------------------------//
	0xC5, 	TFT_DATA(0x0E), //VCOM
	//_MemoryAccess BGR
	TFT_MADCTR, TFT_DATA(/*TFT_MADCTR_MY | TFT_MADCTR_MX | TFT_MADCTR_ML |*/ TFT_MADCTR_BGR),
	//------------------------------------ST7735R Gamma Sequence-----------------------------------------//
	0xe0,
	TFT_DATA(0x0f),
	TFT_DATA(0x1a),
	TFT_DATA(0x0f),
	TFT_DATA(0x18),
	TFT_DATA(0x2f),
	TFT_DATA(0x28),
	TFT_DATA(0x20),
	TFT_DATA(0x22),
	TFT_DATA(0x1f),
	TFT_DATA(0x1b),
	TFT_DATA(0x23),
	TFT_DATA(0x37),
	TFT_DATA(0x00),
	TFT_DATA(0x07),
	TFT_DATA(0x02),
	TFT_DATA(0x10),
	0xe1,
	TFT_DATA(0x0f),
	TFT_DATA(0x1b),
	TFT_DATA(0x0f),
	TFT_DATA(0x17),
	TFT_DATA(0x33),
	TFT_DATA(0x2c),
	TFT_DATA(0x29),
	TFT_DATA(0x2e),
	TFT_DATA(0x30),
	TFT_DATA(0x30),
	TFT_DATA(0x39),
	TFT_DATA(0x3f),
	TFT_DATA(0x00),
	TFT_DATA(0x07),
	TFT_DATA(0x03),
	TFT_DATA(0x10),
	//------------------------------------End ST7735R Gamma Sequence-----------------------------------------//
	0xF0, //Enable test command
	TFT_DATA(0x01),
	0xF6, //Disable ram power save mode
	TFT_DATA(0x00),

	// ColorModeSet 16bpp
	TFT_COLMOD, TFT_DATA(TFT_COLMOD_16BPP ),


	//  Display on
	TFT_NORON,
	TFT_DISPON,


//	  TFT_ColumnAddressSet( x0, x0+dx-1 );
	TFT_CASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),

//	  TFT_RowAddressSet( y0, y0+dy-1 );
	TFT_RASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),


};


const unsigned short ST7735R_init_3[] =
{
	TFT_CASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),
	TFT_RASET, TFT_DATA(0), TFT_DATA(0), TFT_DATA(0), TFT_DATA(127),
};


void ST7735R::lcd_reset()
{
	tsk_sleep(100);
	lcd_hnd->tsk_write(ST7735R_init, 1);
	tsk_sleep(120);
	lcd_hnd->tsk_write(ST7735R_init+1, sizeof(ST7735R_init)/2 -1);
}

