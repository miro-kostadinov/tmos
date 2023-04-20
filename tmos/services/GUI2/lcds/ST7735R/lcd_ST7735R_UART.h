/*
 * lcd_ST7735R_UART.h
 *
 *  Created on: Mar 6, 2018
 *      Author: miro
 */

#ifndef SERVICES_GUI2_LCD_ST7735R_UART_H_
#define SERVICES_GUI2_LCD_ST7735R_UART_H_


#include <lcd_ST7735R.h>
#include <lcd_SPFD5414D_UART.h>

struct ST7735R_UART : public SPFD5414D_UART
{
	static const unsigned short ST7735R_lsb_init[];
	static const unsigned short ST7735R_lsb_init_size;

	ST7735R_UART(unsigned int x, unsigned int y,
			unsigned int dx, unsigned int dy,
			HANDLE hnd, const PIN_DESC* p) :
		SPFD5414D_UART(x, y, dx, dy, hnd, p)
	{
	}
	;

	void lcd_reset() override;
};


#endif /* SERVICES_GUI2_LCD_ST7735R_UART_H_ */
