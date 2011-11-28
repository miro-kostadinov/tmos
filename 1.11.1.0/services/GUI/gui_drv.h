#ifndef	GUI_DRV_H
#define	GUI_DRV_H

#include <lcd.h>

#define SIG_GUI_TASK		2


struct GUI_DRIVER_DATA
{
    HANDLE			waiting;
    HANDLE			helper;
};

struct GUI_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;
	GUI_DRIVER_DATA * 	drv_data;
	LCD_MODULE*			lcd;
};

void GUI_DCR(GUI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param);
void GUI_DSR(GUI_DRIVER_INFO* drv_info, HANDLE hnd);


#endif
