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
#include <gdowait.h>

#if GUI_DISPLAYS > 1
#include <lcd_multiplex.h>
#endif


//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------


WEAK_C GWindow* gui_desktop ()
{
	GWindow* win;
	win = new GWindow(1, RECT_T (0, 0, 132, 64), 0x3, GO_FLG_DEFAULT|GO_FLG_SELECTED);
	win->addChild(new GText (1, RECT_T (0,0,132,30), "A600C", nullptr, GO_FLG_DEFAULT, SS_DEFAULT, &FNT10x21));
	return win;
}

WEAK_C void init_main_menu(void)
{

}

WEAK_C unsigned int scan_code_to_key(unsigned int scan_cod)
{
	return 0;
}
//extern TASK_STRU gui_task;

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

static void GUIExecute_sys_cmd(HANDLE app_handle)
{
	if(app_handle)
	{
		GUI_SYS_COMMANDS  sys_cmd =(GUI_SYS_COMMANDS)( app_handle->src.as_int);
		void *param = app_handle->dst.as_voidptr;
		switch(sys_cmd)
		{
		case gui_sys_cmd_DoWait_Begin :
		case gui_sys_cmd_DoWait_End :
		case gui_sys_cmd_DoWait_Restore:
    		GWait::GUIDoWait(sys_cmd);
			break;
		case gui_sys_cmd_CPU_usage:
			CPU_Usage::GUI_CPU_Usage(sys_cmd, (bool)param);
			break;
		default :
			break;
		}
		usr_HND_SET_STATUS(app_handle, RES_SIG_OK);
	}
}

//*----------------------------------------------------------------------------
//*			GUI Task
//*----------------------------------------------------------------------------
//*  this is the GUI driver helper task
//*----------------------------------------------------------------------------


void gui_thread(GUI_DRIVER_INFO* drv_info)
{
    unsigned int res, sig;
    GWindow* tmp;
    CHandle key_hnd;
    CHandle gui_hnd;
    GMessage msg;
    unsigned int idle_time = CURRENT_TIME;

    //prevent these signals not to be used from task handles
    ALLOCATE_SIGNAL(SIG_GUI_TASK);
    drv_info->drv_data->helper = &gui_hnd;

    //wait for static constructors (lcd object)
    while(!detect_displays(drv_info))
    	tsk_sleep(10);

    // start desktop
	while( !(Gdesktop = gui_desktop()) )
	{
		tsk_sleep(10);
	}
	//desktop->nextObj = nullptr;
#if GUI_DISPLAYS > 1
	LCD_MULT mult;
	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		mult.displays |= (1<<i);
		mult.lcd[i] = drv_info->lcd[i];
	}
	mult.addChild(Gdesktop);
#else
	Gdesktop->parent = drv_info->lcd[0];
#endif


	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		LCD_MODULE* lcd;
		if( (lcd=drv_info->lcd[i]) )
		{
			lcd->children = Gdesktop;
			Gdesktop->displays |= (1<<i);
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
	gui_hnd.tsk_start_read(nullptr, 0);

	Gdesktop->initialize(msg);
	Gdesktop->invalidate(Gdesktop, Gdesktop->rect);
	while(GQueue.pop(msg))
	{
		;
	}
	sig = 0;
	for (;;)
	{
		res = tsk_wait_signal(-1u, 100 - (CURRENT_TIME %100));
        if(!process_timers() && !res )	//checks for elapsed timers
        {
        	if( GUI_IDLE_MSG_PERIOD > ms_since(idle_time))
        			continue;
            if (GQueue.empty())														//if the queue is empty sends WM_IDLE to everyone
    		{
    			for (tmp = Gdesktop; tmp; tmp = (GWindow*)tmp->nextObj)
    				GQueue.push(GMessage(WM_IDLE, 0, 0L, tmp));
    		}
       }
        sig |= res & 0xFF;

        idle_time = CURRENT_TIME;

		if(sig & gui_hnd.signal)												//checks for new items
        {
			sig &= ~gui_hnd.signal;
        	gui_hnd.res &= ~FLG_SIGNALED;
        	if(gui_hnd.dst.as_int)
        	{
            	if(((HANDLE)gui_hnd.dst.as_voidptr)->cmd & FLAG_COMMAND)
            	{
            		// GUI SYSTEM commands
            		GUIExecute_sys_cmd((HANDLE)gui_hnd.dst.as_voidptr);
            	}
            	else
            	{
                	GWindow* win = (GWindow*)((HANDLE)gui_hnd.dst.as_voidptr)->mode.as_voidptr;
                	RES_CODE hnd_ret = RES_SIG_ERROR;
        			if(win->hnd.cmd & FLAG_WRITE)
        			{
        				if(win->hnd.mode1 == GUI_HND_ATTACH)
        				{
        					msg = *(GMessage *)(((HANDLE)gui_hnd.dst.as_voidptr))->src.as_voidptr;
#if GUI_DEBUG_MESSAGES
        					TRACELN1("\e[1;34m HND MSG\e[m "); // blue
#endif
        					GQueue.push(msg);
        					if(msg.code == WN_DESTROY)
        						win->hnd.mode1 = GUI_HND_DETACH;
        					hnd_ret = RES_SIG_OK;
        				}
        				if(win->hnd.mode1 == GUI_HND_OPEN)
        				{
        					drv_info->lcd[0]->backlight_signal();
        					win->nextObj =Gdesktop->parent->focus->nextObj;
        					Gdesktop->parent->focus->nextObj = win;								//adds the new item to the Z list
        					win->parent = Gdesktop->parent;										//LCD
#if GUI_DEBUG_MESSAGES
        					TRACELN1("\e[1;34m HND ATTACH\e[m "); // blue
#endif
        					GQueue.push(GMessage(WM_INIT, 0, (long long)drv_info->lcd, win));	//send WM_INIT to the new window
        					win->hnd.mode1 = GUI_HND_ATTACH;
        					hnd_ret = RES_SIG_OK;												//signals the window thread
        				}
        			}
        			else
        			{
        				if((win->hnd.cmd & FLAG_READ) && win->hnd.mode1 != GUI_HND_OPEN)
        				{
        					if(!win->Queue.empty())
        						hnd_ret = RES_SIG_OK;
        					else
        					{
        						hnd_ret = RES_IDLE;
        						win->hnd.res = FLG_BUSY | FLG_OK;
        					}
        				}
        			}
        			if(hnd_ret & FLG_SIGNALED)
        				usr_HND_SET_STATUS(&win->hnd, hnd_ret);
            	}
        	}
			gui_hnd.tsk_start_read(nullptr, 0);
        }

		if(sig & SIG_GUI_TASK)
		{
			for (tmp = (GWindow*)Gdesktop->nextObj; tmp; tmp = (GWindow*)tmp->nextObj)
			{
				if(tmp->hnd.mode0 && tmp->hnd.res == (FLG_BUSY | FLG_OK) )
				{
					GUI_TRACELN1("\e[4;1;96m"); // bright Cyan
					GUI_TRACE("%X[%d] CANCEL\e[m", tmp, tmp->id);
					usr_HND_SET_STATUS(&tmp->hnd, FLG_SIGNALED);
				}
			}
			sig &=~SIG_GUI_TASK;
			if(!sig)
				continue;
		}

        if(sig &  key_hnd.signal)												//checks for pressed buttons
        {
			drv_info->lcd[0]->backlight_signal();
			if(Gdesktop->parent->focus && (Gdesktop->parent->focus->flags & GO_FLG_SELECTED))
			{
				key_hnd.res &= ~FLG_SIGNALED;
				GQueue.push(GMessage(WM_KEY, scan_code_to_key(key_hnd.src.as_int), 0L, Gdesktop->parent->focus));
				key_hnd.tsk_start_read(&key_hnd.src.as_int, 1);
				sig &= ~key_hnd.signal;
			}
        }

        processes_all_messages();
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
	    	if(param->mode.as_int)		// mode - pointer to Window
	    	{
	    		param->mode1 = GUI_HND_OPEN;
	    	}
	    	param->res = RES_OK;
	    	break;

	    case DCR_HANDLE:
	    	if(param != drv_data->helper)
	    	{
	    		locked_set_byte(&param->mode0, FLG_OK);
	    		svc_send_signal(&gui_task, SIG_GUI_TASK);
	    	}
	    	break;

	    case DCR_CANCEL:
	    	if(param != drv_data->helper)
	    	{
	    		// this is WINDOW handle...
	    		if(!param->svc_list_cancel(drv_data->waiting))
	    		{
		    		locked_set_byte(&param->mode0, FLG_SIGNALED);
		    		svc_send_signal(&gui_task, SIG_GUI_TASK);
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
	HANDLE tmp;
	GUI_DRIVER_DATA* drv_data= drv_info->drv_data;

	hnd->res = RES_BUSY;
	if(hnd == drv_data->helper)
	{
		// Request from the helper task...
		tmp = drv_data->waiting;		//  first waiting handle
		if( tmp )
		{
			//return one handle at a time
			drv_data->waiting = tmp->next;
			hnd->dst.as_voidptr = tmp;
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		}
	} else
	{
		// this is a client handle...
		hnd->mode0 = 0;
		tmp = drv_data->helper;
		if( tmp->res == RES_BUSY )
		{
			//the helper task is waiting for object...
			tmp->dst.as_voidptr = hnd;
			svc_HND_SET_STATUS(tmp, RES_SIG_OK);
		} else
		{
        	hnd->list_add(drv_data->waiting);
		}
	}
}



//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------

// No interrupts...
