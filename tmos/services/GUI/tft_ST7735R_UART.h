/*
 * lcd_ST7735R_UART.h
 *
 *  Created on: 25.09.2017
 *      Author: Stanly
 */

#ifndef LCD_ST7735R_UART_H_
#define LCD_ST7735R_UART_H_

#include <tft_ST7735R.h>
#include <tft_SPFD5414D_UART.h>

struct ST7735R_UART : public SPFD5414D_UART
{
	static const unsigned short ST7735R_lsb_init[];
	static const unsigned short ST7735R_lsb_init_size;

	ST7735R_UART(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
		SPFD5414D_UART(x, y, hnd, p)
	{
	}
	;

protected:
	virtual void tft_reset();
};


#endif /* LCD_ST7735R_UART_H_ */
