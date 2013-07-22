//////////////////////////////////////////////////////////////////////////
//
//			GUI Driver
//
//	Responsible for:
//			- interfacing between the GUI system and GUI clients
//  waiting list is the Zorder for the GUI objects
//////////////////////////////////////////////////////////////////////////

#include <tmos.h>
#include "gui_drv.h"
#include <stdgui.h>
#include <lcd.h>

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------

WEAK_C RES_CODE maindlg_cb(WINDOW obj, unsigned int param, unsigned int msg)
{
	return (0);
}


WEAK_C void init_main_menu(void)
{

}

extern TASK_STRU gui_task;

WEAK_C RES_CODE splashdlg_cb(WINDOW obj, LCD_MODULE* lcd, unsigned int msg)
{
	int pos;

	lcd->set_font(&FNT10x21);
	pos = (lcd->size_y - 2* lcd->font->vspacing)/2;
	if(pos > 0)
	{
		lcd->set_xy_all(pos-7, ALL_CENTER);
		lcd->color = PIX_RED;
		lcd->draw_text("TMOS");
		lcd->color = PIX_BLUE;
		lcd->draw_text("DEMO");
	} else
	{
		pos = (lcd->size_y - lcd->font->vspacing)/2;
		lcd->set_xy_all(pos, ALL_CENTER);
		lcd->color = PIX_RED;
		lcd->draw_text("TMOS");
		lcd->draw_hline(0, lcd->size_x-1, 0);
		lcd->draw_hline(0, lcd->size_x-1, lcd->size_y-1);
		lcd->draw_vline(0, lcd->size_y-1, 0);
		lcd->draw_vline(0, lcd->size_y-1, lcd->size_x-1);
	}
	return (0);
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

void gui_thread(GUI_DRIVER_INFO* drv_info)
{
    unsigned int res, tmp, redraw;
    WINDOW win;
    WINDOW top;
    WINDOW desktop;		//main_dlg;
    CHandle key_hnd;
    CHandle gui_hnd;

    //prevent these signals not to be used from task handles
    ALLOCATE_SIGNAL(SIG_GUI_TASK);

    //wait for static constructors (lcd object)
    while(!detect_displays(drv_info))
    	tsk_sleep(10);

    // start desktop
	while( !(desktop = tsk_new_window(maindlg_cb)) )
	{
		tsk_sleep(10);
	}
	desktop->next = NULL;
	top = desktop;
	desktop->rect.x1 = drv_info->lcd[0]->size_x;
	desktop->rect.y1 = drv_info->lcd[0]->size_y;

	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		if(drv_info->lcd[i])
		{
			#if GUI_DISPLAYS > 1
				desktop->displays |= (1<<i);
			#endif
			drv_info->lcd[i]->lcd_init((GUI_CB)splashdlg_cb);
		}
	}
	tsk_sleep(3000);


	init_main_menu();


    // start key handle
    key_hnd.tsk_safe_open(KEY_DRV_INDX, 0);
    key_hnd.src.as_int = 0;
    key_hnd.tsk_start_read(&key_hnd.src.as_int, 1);


    // start gui handle
	gui_hnd.tsk_safe_open(GUI_DRV_INDX, 0);			//mode = 1 - control handle
	gui_hnd.tsk_start_read(NULL, 0);

    for(;;)
    {
        res = tsk_wait_signal(-1u, 1000 - (CURRENT_TIME %1000));
    	redraw = res>>8;;

		if(!res)
			redraw = 0xFF;

        // 1) get waiting objects
		if(res & gui_hnd.signal)
        {
			drv_info->lcd[0]->backlight_signal();
        	gui_hnd.res &= ~FLG_SIGNALED;
	        win = (WINDOW)gui_hnd.dst.as_voidptr;
			if(win)
			{
                top->next = win;
                do
                {
                	top = (WINDOW)top->next;
                	top->rect.x1 = desktop->rect.x1;
                	top->rect.y1 = desktop->rect.y1;
                	top->callback(NULL , WM_INIT);
					#if GUI_DISPLAYS > 1
                		redraw |= top->displays;
					#endif
                } while( top->next );
			}
			gui_hnd.tsk_start_read(NULL, 0);
			#if GUI_DISPLAYS == 1
				redraw |= 1;
			#endif
        }


        // 2) check keyboard
        if(res &  key_hnd.signal)
        {
			drv_info->lcd[0]->backlight_signal();
        	key_hnd.res &= ~FLG_SIGNALED;
            //send to top
			tmp = top->callback(key_hnd.src.as_int , WM_KEY);
			if(tmp & FLG_BUSY)	//FLG_BUSY returned to redraw
			{
				tmp ^= FLG_BUSY;
				#if GUI_DISPLAYS > 1
					redraw |= top->displays;
				#else
					redraw |= 1;
				#endif
			}
			top->mode1 = tmp;
            key_hnd.tsk_start_read(&key_hnd.src.as_int, 1);
        }

        // 3)  command loop
        top = NULL;
        win = desktop;
        do
        {
          	// check for pending commands
            if(win->mode0 & FLG_OK)
            {
	    		locked_clr_byte(&win->mode0, FLG_OK);
	            tmp = win->callback(win->dst.as_int, WM_USER);
				if(tmp & FLG_BUSY)	//FLG_BUSY returned to redraw
				{
					tmp ^= FLG_BUSY;
					#if GUI_DISPLAYS > 1
						redraw |= win->displays;
					#else
						redraw |= 1;
					#endif
				}
	            win->mode1 = tmp;

            }

            //check for complete (close object)
            if(top && ((win->mode0 | win->mode1) & FLG_SIGNALED) )
            {
            	top->next = win->next;
				#if GUI_DISPLAYS > 1
					redraw |= win->displays;
				#else
					redraw |= 1;
				#endif
                usr_HND_SET_STATUS(win, win->mode1 | FLG_SIGNALED);
                win = (WINDOW)top->next;
            }else
            {
                top = win;
                win = (WINDOW)win->next;
            }

        } while (win );

        // 3) draw loop
    	for(int i=0; i<GUI_DISPLAYS; i++)
    	{
    		if( (redraw & (1<<i)) && drv_info->lcd[i])
    		{
    			drv_info->lcd[i]->redraw_screen(desktop);
    		}
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
	WINDOW win = (WINDOW)param;

	drv_data = drv_info->drv_data;
	switch(reason)
    {
        case DCR_RESET:
        	display_init(drv_info);
            break;

	    case DCR_OPEN:
	    	if(win->mode.as_int)
	    	{
	    		// WINDOW objects
				win->rect.x0 = 0;
				win->rect.y0 = 0;
//				win->rect.x1 = drv_info->lcd->size_x;
//				win->rect.y1 = drv_info->lcd->size_y;
	    	}
	    	win->res = RES_OK;
	    	break;

	    case DCR_HANDLE:
	    	if(win->mode.as_int)
	    	{
	    		locked_set_byte(&win->mode0, FLG_OK);
	    		svc_send_signal(&gui_task, SIG_GUI_TASK);
	    	}
	    	break;

	    case DCR_CANCEL:
	    	if(win->mode.as_int)
	    	{
	    		// this is WINDOW handle...
	    		locked_set_byte(&win->mode0, FLG_SIGNALED);
		    	win->svc_list_cancel(drv_data->waiting);
	    	} else
	    	{
	    		// not a WINDOW
	    		if(win == drv_data->helper)
	    		{
					//the helper task is waiting for object...
					drv_data->helper = NULL;
					win->dst.as_voidptr = NULL;
					svc_HND_SET_STATUS(win, RES_SIG_OK);
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
		hnd->mode1 = 0;
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
