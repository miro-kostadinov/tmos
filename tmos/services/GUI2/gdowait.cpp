/*
 * gdowait.cpp
 *
 *  Created on: 22.10.2014
 *      Author: Stanly
 */

#include <gdowait.h>
#include <lcd_multiplex.h>

GWait* GWait::dowait_win =nullptr;
int32_t GWait::dowait_cnt = 0;

POINT_T PolarToDevXY(int deg, int r, LCD_MODULE* lcd);

unsigned int GWait::initialize (GMessage& msg)
{
#if GUI_DISPLAYS > 1
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];
#else
	LCD_MODULE* lcd = (LCD_MODULE *)msg.lparam;
#endif

	client_rect = rect = lcd->rect;
	flags = GO_FLG_TRANSPARENT;
	last_state = 0;
	new_state = 0x3;
	displays = 1;  // use main display
	R = 10;
	POINT_T r(R,R);
	DPtoLP(r);
	R = (r.x > r.y)?r.x:r.y;
	// reserve one text row
	rect.y0 += FNT5x7.vspacing + FNT5x7.vdistance;
	base.x = rect.x0 + rect.width()/2;
	base.y = rect.y0 + rect.height()/2;
	client_rect  = RECT_T(base.x - R, base.y -R, base.x +R, base.y +R);
	SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
	dowait_win->owners = new GWaitOwner(parent->focus);
	return 0;
}


unsigned int GWait::process_default (GMessage& msg)
{
	if(msg.code == WM_TIMER)
	{
		if(msg.param == ID_BUSY_CLOCK)
		{
			SetTimer(ID_BUSY_CLOCK, 100);
			last_state = new_state;
			new_state <<= 1;
			if(new_state & 0x100)
				new_state ^= 0x101;
			last_state &= ~new_state;
			if(!(flags & GO_FLG_SHOW))
			{
				flags |= GO_FLG_SHOW;
				GWaitOwner *tmp = GWait::dowait_win->owners;
				while(tmp)
				{
					if(tmp->owner->is_available())
						tmp->flags = flags;
					else
						tmp->flags &=~GO_FLG_SHOW;
					tmp = tmp->next;
				}
			}
			invalidate(this, client_rect);
		}
	}
	return 0;
}

void GWait::draw_this(LCD_MODULE* lcd)
{
	uint8_t mask=1;
	if(R > 5)
	{
		POINT_T p;//, r(R-2,R-2);
		for(int i=0; i < 8; i++, mask <<=1)
		{
			p = PolarToDevXY(i*45, R-4, lcd);
			p += base;
			if(last_state & mask)
			{
				lcd->set_color(PIX_BLACK);
				fill_circle(p, 2);
				lcd->set_color(PIX_WHITE);
				draw_circle(p, 2);
				continue;
			}
			if(new_state & mask)
				fill_circle(p, 2);
			else
				draw_circle(p, 2);
		}
	}
}


void GWait::add_owner(void)
{
	GWaitOwner* ptr = owners;
	if(!owners)
	{
		owners = new GWaitOwner(parent->focus);
	}
	else
	{
		while(ptr)
		{
			if(ptr->owner == parent->focus)
				break;
			if(!ptr->next)
			{
				ptr->next = new GWaitOwner(parent->focus);
				break;
			}
			ptr = ptr->next;
		}
	}
	SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
}

void GWait::hide(void)
{
	GWait::dowait_win->KillTimer(ID_BUSY_CLOCK);
	if( GWait::dowait_win->flags & GO_FLG_SHOW)
	{
		GWait::dowait_win->flags &= ~GO_FLG_SHOW;
		GWindow* win = (GWindow *)(GWait::dowait_win->parent->children);
		while(win)
		{
			if((win->flags & GO_FLG_SHOW)
					&& (win->displays & GWait::dowait_win->displays)	)
				send_message(WM_DRAW, 0, GWait::dowait_win->rect.as_int, win);
			win = (GWindow *)win->nextObj;
		}
	}
}

void GWait::GUIDoWait(GUI_SYS_COMMANDS do_wait_cmd)
{
	switch(do_wait_cmd)
	{
	case gui_sys_cmd_DoWait_Begin :
		dowait_cnt++;
		break;
	case gui_sys_cmd_DoWait_End :
		dowait_cnt--;
		break;
	case gui_sys_cmd_DoWait_Restore :
		break;
	default:
		return;
	}
	if(dowait_cnt > 0)
	{
		if((do_wait_cmd !=  gui_sys_cmd_DoWait_End ) && dowait_cnt == 1 && nullptr == dowait_win)
		{
			dowait_win = new GWait();
			if(dowait_win)
			{
				dowait_win->nextObj =Gdesktop->parent->focus->nextObj;
				Gdesktop->parent->focus->nextObj = dowait_win;								//adds the new item to the Z list
				dowait_win->parent = Gdesktop->parent;										//LCD
#if GUI_DISPLAYS > 1
				GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MULT *)(Gdesktop->parent))->lcd, dowait_win));
#else
				GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MODULE *)(Gdesktop->parent)), dowait_win));
#endif
			}
			if(!dowait_win)
				dowait_cnt = 0;
		}
		else
		{
			if(dowait_win)
			{
				if(do_wait_cmd  == gui_sys_cmd_DoWait_Begin) // begin wait
					dowait_win->add_owner();
				if(do_wait_cmd  == gui_sys_cmd_DoWait_Restore) // restore wait
					dowait_win->SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
			}
		}
	}
	else
	{
		if(dowait_win)
		{
			dowait_win->close();
			delete dowait_win;
			dowait_win = nullptr;
		}
		dowait_cnt = 0; // prevent underflow
	}
}

static void DoGUI_sys_cmd(int code, int param=0)
{
	CHandle hnd;
	if(hnd.tsk_open(GUI_DRV_INDX, &hnd))
	{
		hnd.tsk_command((void *)code, (void *)param);
	}
}

void BeginWait()
{
	DoGUI_sys_cmd(gui_sys_cmd_DoWait_Begin);
}

void RestoreWait()
{
	DoGUI_sys_cmd(gui_sys_cmd_DoWait_Restore);
}

void EndWait()
{
	DoGUI_sys_cmd(gui_sys_cmd_DoWait_End);
}

/*
 * ==========================================================
 * 	CPU_usage
 * ==========================================================
 */

CPU_Usage* CPU_Usage::CPU_Usage_win = nullptr;
void CPU_Usage::GUI_CPU_Usage(GUI_SYS_COMMANDS gui_sys_cmd, bool show)
{
	if(show)
	{
		if(CPU_Usage_win)
			return;
		CPU_Usage_win = new CPU_Usage();
		if(CPU_Usage_win)
		{
			CPU_Usage_win->nextObj =Gdesktop->parent->focus->nextObj;
			Gdesktop->parent->focus->nextObj = CPU_Usage_win;					//adds the new item to the Z list
			CPU_Usage_win->parent = Gdesktop->parent;							//LCD
#if GUI_DISPLAYS > 1
			GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MULT *)(Gdesktop->parent))->lcd, CPU_Usage_win));
#else
			GQueue.push(GMessage(WM_INIT, 0, (long long)((LCD_MODULE *)(Gdesktop->parent)), CPU_Usage_win));
#endif
		}
	}
	else
	{
		if(!CPU_Usage_win)
			return;
		// 1. hide and disable this window, but it is still visible on the display, so the display needs to be refreshed
		CPU_Usage_win->close(); //
		GContainer* LCD = CPU_Usage_win->parent;
		// 2. replace the displays used by the LCD (if more than one) with the one used by CPU_Usage
		GFlags bkp_displays = LCD->displays;
		LCD->displays = CPU_Usage_win->displays;
		LCD->invalidate(LCD,  CPU_Usage_win->rect); // refresh all windows
		// 3. restore LCD displays
		LCD->displays = bkp_displays;
		delete CPU_Usage_win;
		CPU_Usage_win = nullptr;
	}
}


unsigned int CPU_Usage::initialize(GMessage& msg)
{
#if GUI_DISPLAYS > 1
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];
#else
	LCD_MODULE* lcd = (LCD_MODULE *)msg.lparam;
#endif
	displays = 1; // use main display

	rect = lcd->rect;
	rect.Inflate(0, rect.height() -CPU_USAGE_WIN_HEIGHT , 0,  0 );
	client_rect = rect;
	flags = GO_FLG_SHOW;
	SetTimer(CPU_USAGE_TIMER_ID, CPU_USAGE_TIMER_RELOAD);

	return 1;
}

unsigned int CPU_Usage::process_default (GMessage& msg)
{
	if(msg.code == WM_TIMER)
	{
		if( msg.param == CPU_USAGE_TIMER_ID)
		{
			uint32_t usage = cpu_usage/100;
			if(!cpu_usage_ready)
			{
				TRACELN1("!!!! 100% usage");
			}
			cpu_usage_ready =0;
			bool update = false;
			if(cpu_peak < usage)
			{
				cpu_usage_pos = cpu_peak = usage;
				SetTimer(CPU_USAGE_TIMER_PEAK_ID, CPU_USAGE_TIMER_PEAK_RELOAD);
				update = true;
			}
			cpu_average += usage;
			if(++cpu_usage_cnt >= CPU_USAGE_AVERAGE_COUNT)
			{
				if(!update)
					cpu_usage_pos = cpu_average/4;
				cpu_usage_cnt = cpu_average =0;
				if(!IsActiveTimer(CPU_USAGE_TIMER_PEAK_ID))
					cpu_peak = cpu_usage_pos;
				update = true;
			}
			if(update)
				invalidate(this, rect);
			SetTimer(CPU_USAGE_TIMER_ID, CPU_USAGE_TIMER_RELOAD);
		}
		if( msg.param == CPU_USAGE_TIMER_PEAK_ID)
		{
			cpu_peak = cpu_usage_pos;
			invalidate(this, rect);
		}
		return 1;
	}
	return 0;
}

void CPU_Usage::draw_this(LCD_MODULE* lcd)
{
	draw_hline(2, cpu_usage_pos +2, rect.y1 - 3);
	lcd->set_font(&FNT5x7);
	set_xy_all(lcd, ((rect.y1 -rect.y0) >> 1) - (lcd->font->height >> 1), TA_CENTER);
	lcd->pos_x = cpu_peak +2;
	lcd->draw_icon(GICON_SQUARE);
	//draw_bitmap(cpu_peak +2, rect.y0, src, width, rows)
}

void CPU_usage(bool show)
{
	DoGUI_sys_cmd(gui_sys_cmd_CPU_usage, show);
}

