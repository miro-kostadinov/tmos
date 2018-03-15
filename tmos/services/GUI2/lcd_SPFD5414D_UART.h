/*
 * lcd_SPFD5414D_UART.h
 *
 *  Created on: Mar 6, 2018
 *      Author: miro
 */

#ifndef SERVICES_GUI2_LCD_SPFD5414D_UART_H_
#define SERVICES_GUI2_LCD_SPFD5414D_UART_H_

#include <lcd_SPFD5414D.h>


struct SPFD5414D_UART: public SPFD5414D
{

	SPFD5414D_UART(unsigned int x, unsigned int y,
			unsigned int dx, unsigned int dy,
			HANDLE hnd, const PIN_DESC* p) :
		SPFD5414D(x, y, dx, dy, hnd, p)
	{
	}
	;
	virtual void lcd_reset();

protected:
	virtual void tft_write_row(unsigned short address_cmd[]);
	virtual void tft_init_address_cmd(unsigned short address_cmd[]);
};


#endif /* SERVICES_GUI2_LCD_SPFD5414D_UART_H_ */
