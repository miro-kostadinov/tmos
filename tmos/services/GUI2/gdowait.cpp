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
void * GWait::dowait_locker = nullptr;

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
#if GUI_DISPLAYS > 1
	displays = 1;
#endif
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
#if GUI_DISPLAYS > 1
					&& (win->displays & GWait::dowait_win->displays)
#endif
					)
				send_message(WM_DRAW, 0, GWait::dowait_win->rect.as_int, win);
			win = (GWindow *)win->nextObj;
		}
	}
}

void GWait::GUIDoWait(int code)
{
	dowait_cnt += code;
	if(dowait_cnt > 0)
	{
		if(code > 0 && dowait_cnt == 1 && nullptr == dowait_win)
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
				if(code == 1) // begin wait
					dowait_win->add_owner();
				if(code == 0) // restore wait
					dowait_win->SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
			}
		}
	}
	else
	{
		if(dowait_win)
		{
			GMessage msg;
			dowait_win->process_destroy(msg);
			delete dowait_win;
			dowait_win = nullptr;
		}
		dowait_cnt = 0; // prevent underflow
	}
	dowait_locker = nullptr;
}

void DoWait(int code)
{
	CHandle hnd;
	if(hnd.tsk_open(GUI_DRV_INDX, &hnd))
	{
		hnd.tsk_command((void *)code, 0);
	}
}

void BeginWait()
{
	DoWait(1);
}

void RestoreWait()
{
	DoWait(0);
}

void EndWait()
{
	DoWait(-1);
}


