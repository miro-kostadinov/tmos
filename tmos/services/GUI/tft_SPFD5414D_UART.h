/*
 * lcd_SPFD5414D_RS.h
 *
 *  Created on: 20.09.2017
 *      Author: Stanly
 */

#ifndef LCD_SPFD5414D_RS_H_
#define LCD_SPFD5414D_RS_H_

#include <tft_SPFD5414D.h>


struct SPFD5414D_UART: public SPFD5414D
{

	SPFD5414D_UART(unsigned int x, unsigned int y, HANDLE hnd, const PIN_DESC* p) :
		SPFD5414D(x, y, hnd, p)
	{
	}
	;

protected:
	void tft_write_row(unsigned short address_cmd[], unsigned short row) override;
	void tft_reset() override;
};


#endif /* LCD_SPFD5414D_RS_H_ */
