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

#define LCD1_MODE ST7565S_RaRb_RATIO(0) + ST7565S_VOLUME(18) + ST7565S_H_FLIP
#define LCD2_MODE ST7565S_RaRb_RATIO(3) + ST7565S_VOLUME(12) + ST7565S_H_FLIP

EXTERN_C int detect_displays(GUI_DRIVER_INFO* drv_info)
{
	HANDLE hnd;


	//display 1
	hnd = new CHandle(SPI1_IRQn, &lcd1_mode_stru);
	drv_info->lcd[0] = new ST7565S(128, 32, hnd, lcd1pins, LCD1_MODE);

	//display 2
	hnd = new CHandle(SPI1_IRQn, &lcd2_mode_stru);
	drv_info->lcd[1] = new ST7565S(128, 32, hnd, lcd2pins, LCD2_MODE);

	//this is required for multiple displays...
	drv_info->lcd[0]->display = 1;
	drv_info->lcd[1]->display = 2;


    backlight_time = 255*1024;

	return (int)drv_info->lcd[0];
}
