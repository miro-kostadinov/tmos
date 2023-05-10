/*
 * gdowait.h
 *
 *  Created on: 22.10.2014
 *      Author: Stanly
 */

#ifndef GDOWAIT_H_
#define GDOWAIT_H_

#include <stdgui.h>

enum GUI_SYS_COMMANDS
{
// wait
	gui_sys_cmd_DoWait_Begin =0 ,
	gui_sys_cmd_DoWait_Restore,
	gui_sys_cmd_DoWait_End,
// cpu usage
	gui_sys_cmd_CPU_usage,
//...
	gui_sys_cmd_INVALID
};

#define ID_BUSY_CLOCK 		1
#define ID_BUSY_SET_OWNER 	0x1111
#define BUSY_START_TIME		300

struct GWaitOwner
{
	GObject*			owner;
	GWaitOwner*			next;
	GFlags				flags;

	GWaitOwner(GObject* win) : owner(win), next(nullptr), flags(0)
	{
	}

};

struct GWait : GContainer    //GWindow
{
	uint16_t	 new_state, last_state;
	uint32_t R;
	POINT_T  base;
	POINT_T *circles_pos;
//private:
	static GWait*   	dowait_win;
	static int32_t		dowait_cnt;
	GWaitOwner*			owners;
public:
	GWait() : GContainer(), //GWindow(),
	new_state(3)
	, last_state(0)
	, R(15)
	, circles_pos(nullptr)
	, owners(nullptr)
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
		if(circles_pos)
		{
			delete circles_pos;
			circles_pos = nullptr;
		}
	}

	GUI_GET_OBJECT_TYPE(OBJECT_DOWAIT);

	static void GUIDoWait(GUI_SYS_COMMANDS do_wait_cmd);
	static void hide(void);
protected:
	unsigned int initialize (GMessage& msg) override;
	void draw_this(LCD_MODULE* lcd) override;
	unsigned int process_default (GMessage& msg) override;
	virtual void add_owner(void);
};

/*
 * ==========================================================
 * 	CPU_usage
 * ==========================================================
 */
#define CPU_USAGE_WIN_HEIGHT			4
#define CPU_USAGE_TIMER_ID				1
#define CPU_USAGE_TIMER_RELOAD			200
#define CPU_USAGE_TIMER_PEAK_ID			2
#define CPU_USAGE_TIMER_PEAK_RELOAD		1600
#define CPU_USAGE_AVERAGE_COUNT			4

extern volatile unsigned int cpu_usage __attribute__ ((deprecated ("Use: volatile unsigned int cpu_usage_percentx100 and cpu_usage_flags")));


struct CPU_Usage : GContainer     // GWindow
{

	unsigned int cpu_peak;
	unsigned int cpu_usage_pos;
	unsigned int cpu_usage_cnt;
	unsigned int cpu_average;
	LCD_MODULE*	main_lcd;
	CPU_Usage() :
		GContainer ()
	{
		cpu_peak = 0;
		cpu_average=cpu_usage_pos=cpu_peak;
		cpu_usage_cnt=0;
		main_lcd = nullptr;
	}

	GUI_GET_OBJECT_TYPE(OBJECT_CPU_USAGE);

	static CPU_Usage* CPU_Usage_win;
	static void GUI_CPU_Usage(GUI_SYS_COMMANDS gui_sys_cmd, bool show);
	virtual void invalidate(GObject* object, RECT_T area) override;
protected:
	unsigned int initialize (GMessage& msg) override;
	void draw_this(LCD_MODULE* lcd) override;
	unsigned int process_default (GMessage& msg) override;

};


// application commands
void BeginWait();
void RestoreWait();
void EndWait();

void CPU_usage(bool show);

#endif /* GDOWAIT_H_ */
