/*
 * gbg_opt.cpp
 *
 *  Created on: Apr 18, 2023
 *      Author: bratkov
 */

#include <stdgui.h>

#if GUI_DEBUG_MESSAGES || GUI_DEBUG || GUI_DEBUG_INVALIDATE
STR_LIST obj_type_str =
		SZ(OBJ_OBJECT) SZ(OBJ_CONTAINER) SZ(OBJ_BUTTON) SZ(OBJ_EDIT) SZ(OBJ_VKB)
		SZ(OBJ_FBUTTON) SZ(OBJ_FTEXT) SZ(OBJ_LISTBOX) SZ(OBJ_MENU)
		SZ(OBJ_RADIO) SZ(OBJ_TEXT) SZ(OBJ_WINDOW) SZ(OBJ_DIALOG)
		SZ(OBJ_MESSAGEBOX) SZ(OBJ_DOWAIT) SZ(OBJ_DISPLAY)
		SZ(OBJ_DISPLAY_MULTIPLEXER)
		SZ(OBJ_CPU_USAGE);

void gui_trace_sleep()
{
	uint32_t mytime = CURRENT_TASK->time;
	tsk_sleep(5);
	CURRENT_TASK->time = mytime;
}
#endif

#if GUI_DEBUG_MESSAGES
STR_LIST wm_dbg_str = SZ(WM_DELETED) SZ(WM_QUIT) SZ(WN_DESTROY) SZ(WM_CLOSE)
		SZ(WM_COMMAND) SZ(WM_CHANGE) SZ(WM_IDLE) SZ(WM_SET_FLAGS)
		SZ(WM_CLR_FLAGS) SZ(WM_SETFOCUS) SZ(WM_KILLFOCUS) SZ(WM_TIMER) SZ(WM_INIT) SZ(WM_DRAW) SZ(WM_KEY);

void GDebug_trace_message(const GMessage& msg, bool push)
{
	if(msg.code == WM_IDLE)
		return;
	if(push)
		TRACELN1("\e[1;93m< "); // yellow
	else
		TRACELN1("\e[1;92m> ");// green

	TRACE("%X[%d]%s: %s,%d", msg.dst, msg.dst->id,
			szlist_at(wm_dbg_str, msg.code),
			szlist_at(obj_type_str, msg.dst->get_object_type()),
			msg.param);
	if(msg.code == WM_DRAW)
	{
		if(msg.lparam)
			RECT_T(msg.lparam).dump();
		else
			msg.dst->rect.dump();
	}
	else
		TRACE(" %lX ", msg.lparam);
	TRACE1("\e[m");
	gui_trace_sleep();
}
#endif

#if GUI_DEBUG_INVALIDATE
static unsigned int gui_trace_level;

static void trace_line(const char*line, const RECT_T& rect, GObject* obj)
{
	TRACE1("\r\n* ");
	for(unsigned int i=0; i<gui_trace_level; i++)
	{
		TRACE1(" ");
	}
	TRACE1(line);
	if(obj)
	{
		TRACE_CHAR(':');
		TRACE1(szlist_at(obj_type_str, obj->get_object_type()));
	}
	rect.dump();
	gui_trace_sleep();
}

void trace_enter(const char*line, const RECT_T& rect, GObject* obj)
{
	trace_line(line, rect, obj);
	gui_trace_level++;
}

void trace_leave(const char*line, const RECT_T& rect, GObject* obj)
{
	if(gui_trace_level)
		gui_trace_level--;
	trace_line(line, rect, obj);
}
#endif

