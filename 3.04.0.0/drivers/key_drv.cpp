//////////////////////////////////////////////////////////////////////////
//
//			Keyboard Driver
//
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include "key_drv.h"
#include <tmos_drivers.h>

static void key_post(unsigned int key)
{
	TRACELN("[%2X]", key);
    usr_drv_icontrol(KEY_DRV_INDX, DCR_PARAMS, (void *)key);
}

unsigned int volatile btns_pressed;

void key_drv_thread(void)
{
    unsigned int i, mask, something_pressed;
	unsigned char button_ticks[KEY_COUNT];
	pio_set new_keys[KEY_COUNT];
	CHandle keyrd_hnd;

	keyrd_hnd.tsk_safe_open(GPIO_IRQn, KEY_RD_PINS);

	for(i=0; i<KEY_COUNT; i++)
        button_ticks[i] = 0;
    btns_pressed = 0;

    for(;;)
    {

        // scan keyboard every 5ms while some key is pressed
        do
        {
            tsk_sleep(KEY_POLLING_TIME-4);
            keyrd_hnd.tsk_read(new_keys, sizeof(new_keys));
			mask = 1;
			something_pressed = 0;
			for(i=0; i<KEY_COUNT; i++)
			{
				if (!new_keys[i])
				{//button pressed
					something_pressed =1;
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
//						pop();
						key_post(KEY_DOWN_CODE+i);
					}

				} else
				{
					button_ticks[i] = 0;
					if (mask & btns_pressed)
					{
						btns_pressed ^= mask;
						key_post(KEY_UP_CODE+i);
					}
				}

				mask <<=1;
			}
        } while(something_pressed);

		//wait for any key...
        keyrd_hnd.tsk_read_locked(new_keys, sizeof(new_keys));

    }
}
TASK_DECLARE_STATIC(keyboard_task, "KEYT", key_drv_thread, 5, 50 + TRACE_SIZE);

//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void KEY_DCR(KEY_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param)
{
	KEY_DRIVER_DATA* drv_data;

	drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
            drv_data->key_in = 0;
            drv_data->key_out = 0;
           	usr_task_init_static(&keyboard_task_desc, true);
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
				#define hnd ((HANDLE)reason)

					if( (reason = (unsigned int)drv_data->waiting) )
					{
						// send the code to the waiting
						*hnd->dst.as_byteptr++ = (unsigned int)param;
                        if(!--hnd->len)
                        {
							drv_data->waiting = hnd->next;
							svc_HND_SET_STATUS(hnd, RES_SIG_OK);
                        }
					} else
					{
						// store the code in the key_buf
						drv_data->key_buf[drv_data->key_in] = (unsigned int)param;
						drv_data->key_in = (drv_data->key_in+1) & DRV_KEYBUF_SIZE;
					}
				#undef hnd
            }
            break;

    }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void KEY_DSR(KEY_DRIVER_INFO* drv_info, HANDLE hnd)
{
	KEY_DRIVER_DATA* drv_data = drv_info->drv_data;

	if(hnd->cmd & FLAG_READ)
	{
        while (hnd->len)
		{
			if( drv_data->key_in == drv_data->key_out )
			{
				hnd->next = drv_data->waiting;
				hnd->res = RES_BUSY;
				drv_data->waiting = hnd;
				return ;
			} else
			{
				*hnd->dst.as_byteptr++ = drv_data->key_buf[drv_data->key_out];
				drv_data->key_out = (drv_data->key_out+1)& DRV_KEYBUF_SIZE;
				hnd->len --;
			}
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}
