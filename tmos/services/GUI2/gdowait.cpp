/*
 * gdowait.cpp
 *
 *  Created on: 22.10.2014
 *      Author: Stanly
 */

#include <gdowait.h>
#include <lcd.h>

GWait* GWait::dowait_win =NULL;
int32_t GWait::dowait_cnt = 0;
void * GWait::dowait_locker = NULL;

POINT_T get_position(int deg, int r);

//WEAK void hide_dowait()
//{
//	if(GWait::dowait_win)
//	{
//		GQueue.push(GMessage(WM_CLR_FLAGS, GO_FLG_SHOW, 0, GWait::dowait_win));
//	}
//}

unsigned int GWait::initialize (GMessage& msg)
{
	LCD_MODULE* lcd = ((LCD_MODULE **)msg.lparam)[0];

	client_rect = rect = lcd->rect;
	flags = GO_FLG_TRANSPARENT;
	base.x = rect.x0 + rect.width()/2;
	base.y = rect.y0 + rect.height()/2;
	last_state = 0;
	new_state = 0x3;
	displays = 1;
	R = 12;
	client_rect = rect = RECT_T(base.x - R, base.y -R, base.x +R, base.y +R);
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
			flags |= GO_FLG_SHOW;
			invalidate(this, rect);
		}
	}
	return 0;
}

void GWait::draw_this(LCD_MODULE* lcd)
{
	uint8_t mask=1;
	if(R > 5)
	{
		POINT_T p;
		for(int i=0; i < 8; i++, mask <<=1)
		{
			p = get_position(i*45, R-4) + base;
			if(last_state & mask)
			{
				lcd->color = PIX_BLACK;
				fill_circle(p, 2);
				lcd->color = PIX_WHITE;
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


unsigned int GWait::process_command(GMessage& msg)
{
	if(msg.param == ID_BUSY_SET_OWNER)
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
		notify_message(WM_COMMAND, ID_BUSY_SET_OWNER, this);
		return 1;
	}
	return 0;
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
			if((win->flags & GO_FLG_SHOW) && (win->displays & GWait::dowait_win->displays))
				send_message(WM_DRAW, 0, GWait::dowait_win->rect.as_int, win);
			win = (GWindow *)win->nextObj;
		}
	}
}

void GWait::DoWait(int code)
{
	while(locked_set_if_null(&dowait_locker, CURRENT_TASK))
		tsk_sleep(10);

	dowait_cnt += code;
	if(dowait_cnt > 0)
	{
		if(code > 0 && dowait_cnt == 1 && NULL == dowait_win)
		{
			dowait_win = new GWait();
			if(dowait_win)
			{
				if(!dowait_win->Create())
				{
					delete dowait_win;
					dowait_win = NULL;
				}
			}
			if(!dowait_win)
				dowait_cnt = 0;
		}
		else
		{
			if(dowait_win)
			{
				if(code == 1) // begin wait
				{
					dowait_win->PostMessage(WM_COMMAND, ID_BUSY_SET_OWNER, dowait_win);
					GMessage msg;
					while(dowait_win->GetMessage(msg, 10))
					{
						if(msg.code == WM_COMMAND && msg.param == ID_BUSY_SET_OWNER)
							break;
					}
				}
				if(code == 0) // restore wait
					dowait_win->PostMessage(WM_TIMER, ID_BUSY_CLOCK, dowait_win);
			}
		}
	}
	else
	{
		if(dowait_win)
		{
			dowait_win->Destroy();
			delete dowait_win;
			dowait_win = NULL;
		}
		dowait_cnt = 0; // prevent underflow
	}
	dowait_locker = NULL;
}

void BeginWait()
{
	GWait::DoWait(1);
}

void RestoreWait()
{
	GWait::DoWait(0);
}

void EndWait()
{
	GWait::DoWait(-1);
}


