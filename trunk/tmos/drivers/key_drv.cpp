//////////////////////////////////////////////////////////////////////////
//
//			Keyboard Driver
//
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include "key_drv.h"
#include <tmos_drivers.h>

#ifndef KEY_DRV_COUNT
	#define KEY_DRV_COUNT	0
#endif

#ifndef KEY_RD_COUNT
	#define KEY_RD_COUNT 	0
	#ifndef KEY_FIX_COUNT
		#ifndef KEY_COUNT
			#error  Undefined key count !
		#endif
		#define KEY_FIX_COUNT	KEY_COUNT
	#endif
#endif

#ifndef KEY_FIX_COUNT
	#define KEY_FIX_COUNT	0
#endif

#ifndef KEY_RD_ACTIV_LEVEL
	#define KEY_RD_ACTIV_LEVEL 0
#endif

#ifndef KEY_POLLING_TIME
	#define KEY_POLLING_TIME	10
#endif

#ifndef KEY_DEBOUNCE_TIME
	#define KEY_DEBOUNCE_TIME 	50
#endif

#ifndef KEY_REPEAT_TIME
	#define KEY_REPEAT_TIME 	300
#endif

WEAK_C void on_key(unsigned int key)
{

}
static void key_post(unsigned int key)
{
	//TRACELN("[%2X]%d", key, (key&0x3f));
    usr_drv_icontrol(KEY_DRV_INDX, DCR_PARAMS, (void *)key);
    on_key(key);
}

#include "key_drv.h"

template <const unsigned int size>
struct key_rd_data
{
	unsigned int 	btns_pressed;
	unsigned char 	button_ticks[size];

	key_rd_data(): btns_pressed(0)
	{
		memclr(button_ticks,size);
	}
	bool key_rd_scan(unsigned int drv_ndx, pio_set new_keys[], const unsigned int key_desc[] );
};

template <const unsigned int size>
bool key_rd_data<size>::key_rd_scan(unsigned int drv_ndx, pio_set new_keys[], const unsigned int key_desc[] )
{
	unsigned int mask = 1;
	bool something_pressed = false;
	for(unsigned int i=0; i<size; i++)
	{
		if(
				((key_desc[i] & PD_ACTIVE_HIGH) && new_keys[i]) ||
				(!(key_desc[i] & PD_ACTIVE_HIGH) && !new_keys[i])
			)
		{//button pressed
			something_pressed =true;
			button_ticks[i]++;
			if(mask & btns_pressed)
			{ //button hold?
				if (button_ticks[i] > (KEY_REPEAT_TIME/KEY_POLLING_TIME))
				{
					button_ticks[i] = 0;
					key_post(KEY_REPEAT_CODE+i+drv_ndx);
				}
			} else
			if (button_ticks[i] > (KEY_DEBOUNCE_TIME/KEY_POLLING_TIME))
			{
				button_ticks[i] = 0;
				btns_pressed |= mask;
				key_post(KEY_DOWN_CODE +i +drv_ndx);
			}

		} else
		{
			button_ticks[i] = 0;
			if (mask & btns_pressed)
			{
				btns_pressed ^= mask;
				key_post(KEY_UP_CODE+i+drv_ndx);
			}
		}

		mask <<=1;
	}
	return something_pressed;
}

void key_drv_thread()
{
#if KEY_DRV_COUNT
	unsigned int drv_ndx;
	key_rd_data<KEY_RD_COUNT> rd[KEY_DRV_COUNT];
	PIO_Cfg_List(KEY_DRV_PINS);
#endif

#if KEY_FIX_COUNT
	key_rd_data<KEY_FIX_COUNT> fix_rd;
#endif

	bool something_pressed;
	pio_set new_keys[KEY_RD_COUNT+KEY_FIX_COUNT];
	CHandle keyrd_hnd;

	keyrd_hnd.tsk_safe_open(GPIO_IRQn, KEY_RD_PINS);

    for(;;)
    {
#if KEY_DRV_COUNT
		for(drv_ndx = 0; drv_ndx < KEY_DRV_COUNT; drv_ndx++)
			PIO_Deassert(KEY_DRV_PINS[drv_ndx]);
#endif
        // scan keyboard every 5ms while some key is pressed
        do
        {
            something_pressed = 0;
#if KEY_FIX_COUNT
        	keyrd_hnd.tsk_read(new_keys, sizeof(new_keys));
			something_pressed |= fix_rd.key_rd_scan( 0, &new_keys[KEY_RD_COUNT], &KEY_RD_PINS[KEY_RD_COUNT]);
#endif
#if KEY_DRV_COUNT
            drv_ndx = 0;
            for(drv_ndx=0; drv_ndx < KEY_DRV_COUNT; drv_ndx++)
            {
           		PIO_Assert(KEY_DRV_PINS[drv_ndx]);
            	keyrd_hnd.tsk_read(new_keys, sizeof(new_keys));
    			something_pressed |= rd[drv_ndx].key_rd_scan(
    					(drv_ndx * KEY_RD_COUNT)+KEY_FIX_COUNT, new_keys, KEY_RD_PINS);
           		PIO_Deassert(KEY_DRV_PINS[drv_ndx]);
            }
#endif
            tsk_sleep(KEY_POLLING_TIME-4);
        } while(something_pressed);

		//wait for any key...
#if KEY_DRV_COUNT
		for(drv_ndx = 0; drv_ndx < KEY_DRV_COUNT; drv_ndx++)
			PIO_Assert(KEY_DRV_PINS[drv_ndx]);
#endif
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
#if KEY_REPEAT_CODE > 0xFF
						*hnd->dst.as_shortptr++ = (unsigned int)param;
#else
						*hnd->dst.as_byteptr++ = (unsigned int)param;
#endif
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
#if KEY_REPEAT_CODE > 0xFF
				*hnd->dst.as_shortptr++ = drv_data->key_buf[drv_data->key_out];
#else
				*hnd->dst.as_byteptr++ = drv_data->key_buf[drv_data->key_out];
#endif
				hnd->len --;
				drv_data->key_out = (drv_data->key_out+1)& DRV_KEYBUF_SIZE;
			}
		}
		svc_HND_SET_STATUS(hnd, RES_SIG_OK);
	}
}
