/*
 * lcd_ST7735R.cpp
 *
 *  Created on: 14.04.2011
 *      Author: stanly
 */
#include <tmos.h>
#include <lcd_ST7735R.h>

const unsigned short ST7735R_init[] =
{
	ST7735R_SLPOUT,

	//------------------------------------ST7735R Frame Rate-----------------------------------------//
	ST7735R_FRMCTR1,	ST7735R_DATA(0x01),	ST7735R_DATA(0x2C),	ST7735R_DATA(0x2D),
	ST7735R_FRMCTR2,	ST7735R_DATA(0x01),	ST7735R_DATA(0x2C),	ST7735R_DATA(0x2D),
	ST7735R_FRMCTR3,	ST7735R_DATA(0x01),	ST7735R_DATA(0x2C),	ST7735R_DATA(0x2D),	ST7735R_DATA(0x01),	ST7735R_DATA(0x2C),	ST7735R_DATA(0x2D),
	//------------------------------------End ST7735R Frame Rate-----------------------------------------//
	ST7735R_INVCTR, 	ST7735R_DATA(0x04), //Column inversion
	//------------------------------------ST7735R Power Sequence-----------------------------------------//
	ST7735R_DISSET5, 	ST7735R_DATA(0xB4),	ST7735R_DATA(0xF0), //Extend gate non-overlap time
	ST7735R_PWCTR1,	ST7735R_DATA(0xA2),	ST7735R_DATA(0x02),	ST7735R_DATA(0x84),
	ST7735R_PWCTR2,	ST7735R_DATA(0xC5),
	ST7735R_PWCTR3,	ST7735R_DATA(0x0A),	ST7735R_DATA(0x00),
	ST7735R_PWCTR4,	ST7735R_DATA(0x8A),	ST7735R_DATA(0x2A),
	ST7735R_PWCTR5,	ST7735R_DATA(0x8A),	ST7735R_DATA(0xEE),
	//---------------------------------End ST7735R Power Sequence-------------------------------------//
	ST7735R_VMCTR1, 	ST7735R_DATA(0x0E), //VCOM
	//_MemoryAccess BGR
	ST7735R_MADCTR, ST7735R_DATA(/*ST7735R_MADCTR_MY | ST7735R_MADCTR_MX | ST7735R_MADCTR_ML |*/ ST7735R_MADCTR_BGR),
	//------------------------------------ST7735R Gamma Sequence-----------------------------------------//
	ST7735R_GAMCTRP1,
	ST7735R_DATA(0x0f),
	ST7735R_DATA(0x1a),
	ST7735R_DATA(0x0f),
	ST7735R_DATA(0x18),
	ST7735R_DATA(0x2f),
	ST7735R_DATA(0x28),
	ST7735R_DATA(0x20),
	ST7735R_DATA(0x22),
	ST7735R_DATA(0x1f),
	ST7735R_DATA(0x1b),
	ST7735R_DATA(0x23),
	ST7735R_DATA(0x37),
	ST7735R_DATA(0x00),
	ST7735R_DATA(0x07),
	ST7735R_DATA(0x02),
	ST7735R_DATA(0x10),
	ST7735R_GAMCTRN1,
	ST7735R_DATA(0x0f),
	ST7735R_DATA(0x1b),
	ST7735R_DATA(0x0f),
	ST7735R_DATA(0x17),
	ST7735R_DATA(0x33),
	ST7735R_DATA(0x2c),
	ST7735R_DATA(0x29),
	ST7735R_DATA(0x2e),
	ST7735R_DATA(0x30),
	ST7735R_DATA(0x30),
	ST7735R_DATA(0x39),
	ST7735R_DATA(0x3f),
	ST7735R_DATA(0x00),
	ST7735R_DATA(0x07),
	ST7735R_DATA(0x03),
	ST7735R_DATA(0x10),
	//------------------------------------End ST7735R Gamma Sequence-----------------------------------------//
	ST7735R_EXTCTRL, //Enable test command
	ST7735R_DATA(0x01),
	0xF6, //Disable ram power save mode
	ST7735R_DATA(0x00),

	// ColorModeSet 16bpp
	ST7735R_COLMOD, ST7735R_DATA(ST7735R_COLMOD_16BPP ),


	//  Display on
	ST7735R_NORON,
	ST7735R_DISPON,


//	  TFT_ColumnAddressSet( x0, x0+dx-1 );
	ST7735R_CASET, ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(127),

//	  TFT_RowAddressSet( y0, y0+dy-1 );
	ST7735R_RASET, ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(127),


};


const unsigned short ST7735R_init_3[] =
{
	ST7735R_CASET, ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(127),
	ST7735R_RASET, ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(0), ST7735R_DATA(127),
};


void ST7735R::lcd_reset()
{
	tsk_sleep(100);
	lcd_hnd->tsk_write(ST7735R_init, 1);
	tsk_sleep(120);
	lcd_hnd->tsk_write(ST7735R_init+1, sizeof(ST7735R_init)/2 -1);
}

void ST7735R::redraw_screen (GObject* object, RECT_T area)
{
	redraw_rect(object, area);
}

