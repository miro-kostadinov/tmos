/*
 * gdowait.h
 *
 *  Created on: 22.10.2014
 *      Author: Stanly
 */

#ifndef GDOWAIT_H_
#define GDOWAIT_H_

#include <tmos.h>
#include <gwindow.h>

#define ID_BUSY_CLOCK 		1
#define ID_BUSY_SET_OWNER 	0x1111
#define BUSY_START_TIME		300

struct GWaitOwner
{
	GObject*			owner;
	GWaitOwner*			next;

	GWaitOwner(GObject* win) : owner(win), next(NULL)
	{
	}

};

struct GWait : GWindow
{
	uint16_t	 new_state, last_state;
	uint32_t R;
	POINT_T  base;
//private:
	static GWait*   	dowait_win;
	static int32_t		dowait_cnt;
	static void* 		dowait_locker;
	GWaitOwner*			owners;
public:
	GWait() : GWindow(),
	new_state(3), last_state(0), R(15), owners(NULL)
	{
	}

	virtual ~GWait()
	{
		while(owners)
		{
			GWaitOwner* tmp;
			tmp = owners;
			owners = owners->next;
			delete tmp;
		}
	}

static void DoWait(int code);

protected:
	virtual unsigned int initialize (GMessage& msg);
	virtual void draw_this(LCD_MODULE* lcd);
	virtual unsigned int process_default (GMessage& msg);
	virtual unsigned int process_command (GMessage& msg);
};


void BeginWait();
void RestoreWait();
void EndWait();


#endif /* GDOWAIT_H_ */
