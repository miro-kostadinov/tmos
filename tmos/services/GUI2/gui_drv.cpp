//////////////////////////////////////////////////////////////////////////
//
//			GUI Driver
//
//	Responsible for:
//			- interfacing between the GUI system and GUI clients
//  waiting list is the Zorder for the GUI objects
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include <gui_drv.h>
#include <stdgui.h>
#include <lcd.h>
#include <gwindow.h>
#include <gbutton.h>
#include <gtext.h>

#if GUI_DISPLAYS > 1
#include <lcd_multiplex.h>
#endif


//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------


WEAK_C GWindow* gui_test_main ()
{
	GWindow* win;
	win = new GWindow(1, RECT_T (0, 0, 132, 64), 0x3, GO_FLG_DEFAULT|GO_FLG_SELECTED);
	win->addChild(new GText (1, RECT_T (0,0,132,30), "A600C", NULL, GO_FLG_DEFAULT, SS_DEFAULT, &FNT10x21));
	return win;
}

WEAK_C void init_main_menu(void)
{

}

extern TASK_STRU gui_task;

__attribute__ ((weak)) void splash_screen(LCD_MODULE* lcd)
{
//	int pos;
//
//	lcd->set_font(&FNT10x21);
//	pos = (lcd->size_y - 2* lcd->font->vspacing)/2;
//	if(pos > 0)
//	{
//		lcd->set_xy_all(pos-7, TA_CENTER);
//		lcd->color = PIX_RED;
//		lcd->draw_text("TMOS");
//		lcd->color = PIX_BLUE;
//		lcd->draw_text("DEMO");
//	} else
//	{
//		pos = (lcd->size_y - lcd->font->vspacing)/2;
//		lcd->set_xy_all(pos, TA_CENTER);
//		lcd->color = PIX_RED;
//		lcd->draw_text("TMOS");
//		lcd->draw_hline(0, lcd->size_x-1, 0);
//		lcd->draw_hline(0, lcd->size_x-1, lcd->size_y-1);
//		lcd->draw_vline(0, lcd->size_y-1, 0);
//		lcd->draw_vline(0, lcd->size_y-1, lcd->size_x-1);
//	}
}

WEAK_C int detect_displays(GUI_DRIVER_INFO* drv_info)
{
	return (int)drv_info->lcd[0];
}


//*----------------------------------------------------------------------------
//*			GUI Task
//*----------------------------------------------------------------------------
//*  this is the GUI driver helper task
//*----------------------------------------------------------------------------
msgQueue<MAX_MESSAGES> GQueue;

unsigned int ms_since(unsigned int time);

void gui_thread(GUI_DRIVER_INFO* drv_info)
{
    unsigned int res;
    GWindow* desktop;		//main_dlg;
    GWindow* tmp;
    CHandle key_hnd;
    CHandle gui_hnd;
    GMessage msg;
    unsigned int idle_time = CURRENT_TIME;
#if GUI_DEBUG
    unsigned int t0;
#endif

    //prevent these signals not to be used from task handles
    ALLOCATE_SIGNAL(SIG_GUI_TASK);

    //wait for static constructors (lcd object)
    while(!detect_displays(drv_info))
    	tsk_sleep(10);

    // start desktop
	while( !(desktop = gui_test_main()) )
	{
		tsk_sleep(10);
	}
	//desktop->nextObj = NULL;
#if GUI_DISPLAYS > 1
	LCD_MULT mult;
	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		mult.lcd[i] = drv_info->lcd[i];
	}
	mult.addChild(desktop);
#else
	desktop->parent = drv_info->lcd[0];
#endif


	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		LCD_MODULE* lcd;
		if( (lcd=drv_info->lcd[i]) )
		{
			lcd->children = desktop;
			#if GUI_DISPLAYS > 1
				desktop->displays |= (1<<i);
			#endif
			lcd->lcd_init(splash_screen);
		}
	}
//	tsk_sleep(3000);


	init_main_menu();


    // start key handle
    key_hnd.tsk_safe_open(KEY_DRV_INDX, 0);
    key_hnd.src.as_int = 0;
    key_hnd.tsk_start_read(&key_hnd.src.as_int, 1);


    // start gui handle
	gui_hnd.tsk_safe_open(GUI_DRV_INDX, 0);			//mode = 1 - control handle
	gui_hnd.tsk_start_read(NULL, 0);

	desktop->initialize(msg);
	desktop->invalidate(desktop, desktop->rect);
	while(GQueue.pop(msg))
	{
		;
	}
	for (;;)
	{
		res = tsk_wait_signal(-1u, 100 - (CURRENT_TIME %100));
        if(!process_timers() && !res )	//checks for elapsed timers
        {
        	if( IDLE_PERIOD > ms_since(idle_time))
        			continue;
        }
        idle_time = CURRENT_TIME;

		if(res & SIG_GUI_TASK)
		{
			for (tmp = (GWindow*)desktop->nextObj; tmp; tmp = (GWindow*)tmp->nextObj)
			{
				if(tmp->hnd.mode0)
					usr_HND_SET_STATUS(&tmp->hnd, FLG_SIGNALED);
			}
			res &=~SIG_GUI_TASK;
			if(!res)
				continue;
		}
		if(res & gui_hnd.signal)												//checks for new items
        {
			drv_info->lcd[0]->backlight_signal();
        	gui_hnd.res &= ~FLG_SIGNALED;

        	GWindow* win = (GWindow*)((HANDLE)gui_hnd.dst.as_voidptr)->mode.as_voidptr;
        	RES_CODE hnd_ret = RES_SIG_ERROR;
			if(win->hnd.cmd & FLAG_WRITE)
			{
				if(win->hnd.mode1 == GUI_HND_ATTACH)
				{
					msg = *(GMessage *)(((HANDLE)gui_hnd.dst.as_voidptr))->src.as_voidptr;
					GQueue.push(msg);
					if(msg.code == WN_DESTROY)
					{
						win->hnd.mode1 = GUI_HND_DETACH;
						hnd_ret = RES_IDLE;
					}
					else
						hnd_ret = RES_SIG_OK;
				}
				if(win->hnd.mode1 == GUI_HND_OPEN)
				{
					desktop->parent->focus->nextObj = win;								//adds the new item to the Z list
					win->parent = desktop->parent;
					GQueue.push(GMessage(WM_INIT, 0, (long long)drv_info->lcd, win));							//send WM_INIT to the new window
					win->hnd.mode1 = GUI_HND_ATTACH;
					hnd_ret = RES_SIG_OK;										//signals the window thread
				}
			}
			else
			{
				if(win->hnd.cmd & FLAG_READ && win->hnd.mode1 != GUI_HND_OPEN)
				{
					if(!win->Queue.empty())
						hnd_ret = RES_SIG_OK;
					else
						hnd_ret = RES_IDLE;
				}
			}
			if(hnd_ret & FLG_SIGNALED)
				usr_HND_SET_STATUS(&win->hnd, hnd_ret);
			gui_hnd.tsk_start_read(NULL, 0);
        }
        if(res &  key_hnd.signal)												//checks for pressed buttons
        {
			drv_info->lcd[0]->backlight_signal();
        	key_hnd.res &= ~FLG_SIGNALED;
        	GQueue.push(GMessage(WM_KEY, key_hnd.src.as_int, 0L, desktop->parent->focus));
            key_hnd.tsk_start_read(&key_hnd.src.as_int, 1);
        }

        if (GQueue.empty())														//if the queue is empty sends WM_IDLE to everyone
		{
			for (tmp = desktop; tmp; tmp = (GWindow*)tmp->nextObj)
				GQueue.push(GMessage(WM_IDLE, 0, 0L, tmp));
		}
		while (GQueue.pop(msg))													//processes all messages in the queue
		{
#if GUI_DEBUG
			t0 = CURRENT_TIME;
			TRACELN1("\e[4;1;32m");
			TRACE("%X[%d] ( %s 0x%X/%d\e[m", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param, msg.param);
			if(WM_DRAW)
			{
				if(msg.lparam)
					RECT_T(msg.lparam).dump();
				else
					msg.dst->rect.dump();
			}
			else
			{
				TRACE("%ld ", msg.lparam);
			}

			TRACE1("\e[4;1;32m)\e[m");
#endif
			if(msg.code == WM_DELETED)
				continue;
			if (msg.dst)
				msg.dst->message(msg);
			else
				desktop->parent->message(msg);    // lcd message
#if GUI_DEBUG
			TRACE("\e[4;1;32m %d ms\e[m", ms_since(t0));
#endif
		}
	}
}

TASK_DECLARE_STATIC(gui_task, "GUIT", (void (*)(void))gui_thread, 4, 350 + TRACE_SIZE);

// must be called at boot to start the display functionality
static void display_init(GUI_DRIVER_INFO* drv_info)
{
	usr_task_init_static(&gui_task_desc, true);
	gui_task.sp->r0.as_voidptr = drv_info;
}



//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void GUI_DCR(GUI_DRIVER_INFO* drv_info, unsigned int reason, HANDLE param)
{
	GUI_DRIVER_DATA*  drv_data;

	drv_data = drv_info->drv_data;
	switch(reason)
    {
        case DCR_RESET:
        	display_init(drv_info);
            break;

	    case DCR_OPEN:
	    	if(param->mode.as_int)
	    	{
	    		param->mode1 = GUI_HND_OPEN;
	    	}
	    	param->res = RES_OK;
	    	break;

	    case DCR_HANDLE:
	    	if(param->mode.as_int)
	    	{
	    		locked_set_byte(&param->mode0, FLG_OK);
	    		svc_send_signal(&gui_task, SIG_GUI_TASK);
	    	}
	    	break;

	    case DCR_CANCEL:
	    	if(param->mode.as_int)
	    	{
	    		// this is WINDOW handle...
	    		locked_set_byte(&param->mode0, FLG_SIGNALED);
	    		if(!param->svc_list_cancel(drv_data->waiting))
	    		{
		    		svc_send_signal(&gui_task, SIG_GUI_TASK);
	    		}
	    	} else
	    	{
	    		// not a WINDOW
	    		if(param == drv_data->helper)
	    		{
					//the helper task is waiting for object...
					drv_data->helper = NULL;
					param->dst.as_voidptr = NULL;
					svc_HND_SET_STATUS(param, RES_SIG_OK);
	    		}
	    	}
	    	break;

    }
}


//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void GUI_DSR(GUI_DRIVER_INFO* drv_info, HANDLE hnd)
{
	GUI_DRIVER_DATA* drv_data = drv_info->drv_data;
	HANDLE tmp;

	if(hnd->mode.as_int)
	{
		// this is GUI_OBJECT handle...
		hnd->res = RES_BUSY;
		hnd->mode0 = 0;
		if( (tmp=drv_data->helper) )
		{
			//the helper task is waiting for object...
			hnd->next = NULL;
			drv_data->helper = NULL;
			tmp->dst.as_voidptr = hnd;
			svc_HND_SET_STATUS(tmp, RES_SIG_OK);
		} else
		{
			//queue the WINDOW object while helper task is busy
			hnd->next = drv_data->waiting;
			drv_data->waiting = hnd;
		}
	} else
	{
		// not a WINDOW object
		// this should be the helper task reading the waiting WINDOW objects
		if( (tmp=drv_data->waiting) )
		{
			drv_data->waiting = NULL;
			hnd->dst.as_voidptr = tmp;
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		} else
		{
			hnd->res = RES_BUSY;
			drv_data->helper = hnd;
		}
	}
}



//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------

// No interrupts...
