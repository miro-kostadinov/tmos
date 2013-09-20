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
#include <mqueue.h>
#include <gwindow.h>
#include <gbutton.h>
#include <gtext.h>

#if GUI_DISPLAYS > 1
#include <lcd_multiplex.h>
#endif


#define ID_YES_BUTTON 0x01
#define ID_NO_BUTTON 0x02

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------

WEAK_C RES_CODE maindlg_cb(WINDOW obj, unsigned int param, unsigned int msg)
{
	return (0);
}

GWindow* gui_test_main ()
{
	GWindow* win;
	win = new GWindow(1, RECT_T (0, 0, 120, 31), 0xFF);
	win->addChild(new GButton(12, RECT_T(50,19,100,31), ID_NO_BUTTON, "Nooooo"));
	win->addChild(new GButton(11, RECT_T (0,19,49,31), ID_YES_BUTTON,  "Yes"));
	win->addChild(new GText (10, RECT_T (0,0,210,18), "Choose one of the buttons bellow!!!"));
	return win;
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
mqueue<GMessage, MAX_MESSAGES> GQueue;


void gui_thread(GUI_DRIVER_INFO* drv_info)
{
    unsigned int res;
    GWindow* desktop;		//main_dlg;
    GWindow* tmp;
    CHandle key_hnd;
    CHandle gui_hnd;
    GMessage msg;


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


	desktop->get_focus();
	for(int i=0; i<GUI_DISPLAYS; i++)
	{
		if(drv_info->lcd[i])
		{
			drv_info->lcd[i]->children = desktop;
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
	for (tmp = desktop; tmp; tmp = (GWindow*)tmp->nextObj)
		tmp->invalidate(tmp, tmp->rect);
	GQueue.push(GMessage(WM_DRAW,0, 0L, desktop->children));
	for (;;)
	{
		res = tsk_wait_signal(-1u, 100 - (CURRENT_TIME %100));

		if(res & gui_hnd.signal)												//checks for new items
        {
			drv_info->lcd[0]->backlight_signal();
        	gui_hnd.res &= ~FLG_SIGNALED;

        	GWindow* win = (GWindow*)((HANDLE)gui_hnd.dst.as_voidptr)->mode.as_voidptr;
        	desktop->parent->focus->nextObj = win;								//adds the new item to the Z list
        	win->parent = desktop->parent;
        	win->get_focus();
        	GQueue.push(GMessage(WM_INIT, 0, 0L, win));							//send WM_INIT to the new window
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
			if (msg.dst)
				msg.dst->message(msg);
			else
				desktop->parent->message(msg);
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
//				win->rect.x0 = 0;
//				win->rect.y0 = 0;
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
