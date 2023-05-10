/*
 * display.c
 *
 *  Created on: 2008-10-19
 *      Author: Miroslav Kostadinov
 */

#include <stdgui.h>

unsigned int current_laguage;

WEAK_C char TranslateKey( unsigned int key_code)
{
	return 0;
}

msgQueue<MAX_MESSAGES> GQueue;

void processes_all_messages(void)
{
	GMessage msg;
	while (GQueue.pop(msg))														//processes all messages in the queue
	{
#if GUI_DEBUG_MESSAGES
		unsigned int t0 = CURRENT_TIME;
#endif
		if(msg.code == WM_DELETED || !Gdesktop)
			continue;

		if (msg.dst == nullptr )
			msg.dst = Gdesktop->parent;	// lcd message

		if(GWait::dowait_win && msg.code == WN_DESTROY && msg.dst && msg.dst != GWait::dowait_win)
		{
			GWaitOwner** powner = &GWait::dowait_win->owners;
			GWaitOwner* owner;

			while((owner = *powner))
			{
				if(owner->owner == msg.dst)
				{
					*powner =  owner->next;
					delete owner;
					break;
				}
				powner =  &owner->next;
			}
		}

		if(!msg.dst->message(msg) && msg.code == WM_KEY)   						// send message to desktop
			Gdesktop->message(msg);

		if(GWait::dowait_win && msg.dst && msg.dst != GWait::dowait_win && GWait::dowait_cnt)
		{
			GWaitOwner *tmp = GWait::dowait_win->owners;
			if(msg.code == WM_INIT && msg.lparam)
			{
				// msg.lparam set only if window object
				while(tmp)
				{
					if(tmp->owner == msg.dst && msg.dst->is_available())
						break;
					tmp = tmp->next;
				}
				if(!tmp)
					GWait::dowait_win->hide();
			}
			if(msg.code == WN_DESTROY)
			{
				while(tmp)
				{
					if(tmp->owner == Gdesktop->parent->focus && (tmp->flags & GO_FLG_SHOW))//!(GWait::dowait_win->flags & GO_FLG_SHOW))
					{
						GWait::dowait_win->SetTimer(ID_BUSY_CLOCK, BUSY_START_TIME);
						break;
					}
					tmp = tmp->next;
				}
				if(!tmp)
					GWait::dowait_win->hide();
			}
		}
#if GUI_DEBUG_MESSAGES
		TRACELN("\e[1;92m executed for %d ms\e[m", ms_since(t0));
		gui_trace_sleep();
#endif
	}
}

void send_message(WM_MESSAGE wm_code,
		WPARAM wparam, LPARAM lparam, GObject* dst)
{
	while(!GQueue.push(GMessage(wm_code, wparam, lparam, dst)))
		processes_all_messages();
}

void send_message(GMessage& msg, GObject* dst)
{
	if(dst)
		msg.dst = dst;
	while(!GQueue.push(msg))
		processes_all_messages();
}

//*----------------------------------------------------------------------------
//*			sin(x) / cos(x) used for polar transformations
//*----------------------------------------------------------------------------
#if USE_GUI_MATH
static const int16_t sin_x10000_0_90[]=
{
    0,  174,  348,  523,  697,  871, 1045, 1218, 1391, 1564, 1736, 1908, 2079,
 2249, 2419, 2588, 2756, 2923, 3090, 3255, 3420, 3583, 3746, 3907, 4067, 4226,
 4383, 4539, 4694, 4848, 5000, 5150, 5299, 5446, 5591, 5735, 5877, 6018, 6156,
 6293, 6427, 6560, 6691, 6819, 6946, 7071, 7193, 7313, 7431, 7547, 7660, 7771,
 7880, 7986, 8090, 8191, 8290, 8386, 8480, 8571, 8660, 8746, 8829, 8910, 8987,
 9063, 9135, 9205, 9271, 9335, 9396, 9455, 9510, 9563, 9612, 9659, 9702, 9743,
 9781, 9816, 9848, 9876, 9902, 9925, 9945, 9961, 9975, 9986, 9993, 9998, 10000
};

int32_t sin_x10000(int deg)
{
	while(1)
	{

		switch(deg)
		{
		case 0 ... 90:
			return sin_x10000_0_90[deg];
		case 91 ... 180:
			return sin_x10000_0_90[180 -deg];
		case 181 ... 270:
			return  - sin_x10000_0_90[deg -180];
		case 271 ... 360:
			return  - sin_x10000_0_90[360 - deg];
		default:
			while( deg > 360 )
				deg -= 360;
			while( deg < -360 )
				deg += 360;
			if( deg < 0 )
				deg += 360;
			break;
		}
	}
	return (int16_t)0;
}

int32_t cos_x10000(int deg)
{
	return sin_x10000(90 - deg);
}
#endif

