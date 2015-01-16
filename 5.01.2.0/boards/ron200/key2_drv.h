#ifndef	KEY_DRV_H
#define	KEY_DRV_H

#include <drivers.h>
#include <tmos_drivers.h>

#define DRV_KEYBUF_SIZE	7

struct KEY2_DRIVER_DATA
{
    HANDLE			waiting;
    unsigned char	key_in, key_out;

    unsigned short	key_buf[DRV_KEYBUF_SIZE+1];
};

struct KEY2_DRIVER_INFO
{
	DRIVER_INFO_Type 	info;
    KEY2_DRIVER_DATA* 	drv_data;
    GPIO_STRU			pins;
    unsigned int 		key_codes[KEY_COUNT];
};


void KEY2_DCR(KEY2_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param);
void KEY2_DSR(KEY2_DRIVER_INFO* drv_info, HANDLE hnd);




#endif
