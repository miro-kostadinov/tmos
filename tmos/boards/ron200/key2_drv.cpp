//////////////////////////////////////////////////////////////////////////
//
//			Keyboard Driver
//
//////////////////////////////////////////////////////////////////////////

#include "key2_drv.h"
#include <tmos.h>

static void key_post(unsigned int key)
{
	TRACELN("[%2X]", key);
    usr_drv_icontrol(KEY_DRV_INDX, DCR_PARAMS, (void *)key);
}

unsigned int volatile btns_pressed;

void key_drv_thread(KEY2_DRIVER_INFO* drv_info)
{
    unsigned int i, mask;
	unsigned char button_ticks[KEY_COUNT];
	unsigned int new_keys;
	CHandle keyrd_hnd;

	pio_open(&keyrd_hnd, &drv_info->pins);

	for(i=0; i<KEY_COUNT; i++)
        button_ticks[i] = 0;
    btns_pressed = 0;

    for(;;)
    {

        // scan keyboard every 5ms while some key is pressed
        do
        {
            tsk_sleep(KEY_POLLING_TIME-4);
            keyrd_hnd.tsk_read(&new_keys, 4);

			mask = 1;
			for(i=0; i<KEY_COUNT; i++)
			{
				if (new_keys & drv_info->key_codes[i])
				{
					button_ticks[i] = 0;
					if (mask & btns_pressed)
					{
						btns_pressed ^= mask;
						key_post(KEY_UP_CODE+i);
					}
				} else
				{//button pressed
					button_ticks[i]++;
					if(mask & btns_pressed)
					{ //button hold?
						if (button_ticks[i] > (KEY_REPEAT_TIME/KEY_POLLING_TIME))
						{
							button_ticks[i] = 0;
							key_post(KEY_REPEAT_CODE+i);
						}
					} else
					if (button_ticks[i] > (KEY_DEBOUNCE_TIME/KEY_POLLING_TIME))
					{
						button_ticks[i] = 0;
						btns_pressed |= mask;
						key_post(KEY_DOWN_CODE+i);
					}

				}

				mask <<=1;
			}
        } while(new_keys ^ drv_info->pins.mask);

		//wait for any key...
        keyrd_hnd.tsk_read_locked(&new_keys, 4);

    }
}
TASK_DECLARE_STATIC(keyboard_task, "KEYT", (void (*)(void))key_drv_thread, 5, 50 + TRACE_SIZE);

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void KEY2_DCR(KEY2_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param)
{
	KEY2_DRIVER_DATA* drv_data;

	drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
            drv_data->key_in = 0;
            drv_data->key_out = 0;
           	usr_task_init_static(&keyboard_task_desc, true);
           	keyboard_task.sp->r0.as_cvoidptr = drv_info;
           	break;

        case DCR_OPEN:
        	param->res = RES_OK;
			break;

	    case DCR_CANCEL:
	    	param->svc_list_cancel(drv_data->waiting);
	    	break;


	    case DCR_PARAMS:
            //keycode sent from thread
            {
				HANDLE hnd = drv_data->waiting;

				if( hnd )
				{
					// send the code to the waiting
					*hnd->dst.as_byteptr++ = (unsigned int)param;
					drv_data->waiting = hnd->next;
					svc_HND_SET_STATUS(hnd, RES_SIG_OK);
				} else
				{
					// store the code in the key_buf
					drv_data->key_buf[drv_data->key_in] = (unsigned int)param;
					drv_data->key_in = (drv_data->key_in+1) & DRV_KEYBUF_SIZE;
				}
            }
    }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void KEY2_DSR(KEY2_DRIVER_INFO* drv_info, HANDLE hnd)
{
	KEY2_DRIVER_DATA* drv_data = drv_info->drv_data;

	if (drv_data->key_in == drv_data->key_out)
	{
		hnd->next = drv_data->waiting;
		hnd->res = RES_BUSY;
		drv_data->waiting = hnd;
	}
	else
	{
		*hnd->dst.as_byteptr++ = drv_data->key_buf[drv_data->key_out];
		drv_data->key_out = (drv_data->key_out + 1) & DRV_KEYBUF_SIZE;
		hnd->len--;
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}
