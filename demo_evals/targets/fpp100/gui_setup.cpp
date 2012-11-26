/*
 * gui_setup.cpp
 *
 *  Created on: Nov 23, 2012
 *      Author: miro
 */

#include <tmos.h>
#include <drivers.h>
#include <lcd.h>
#include <lcd_ST7565S.h>

extern volatile unsigned int backlight_time;

EXTERN_C int detect_displays(GUI_DRIVER_INFO* drv_info)
{
	HANDLE hnd;


	hnd = new CHandle(SPI1_IRQn, &lcd_mode_stru);
	drv_info->lcd[0] = new ST7565S(128, 32, hnd, lcdpins);


    backlight_time = 255*1024;

	return (int)drv_info->lcd[0];
}
