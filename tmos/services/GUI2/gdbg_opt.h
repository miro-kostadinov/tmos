/*
 * gdbg_opt.h
 *
 *  Created on: Apr 18, 2023
 *      Author: bratkov
 */

#ifndef GDBG_OPT_H_
#define GDBG_OPT_H_

#include <tmos.h>

#ifndef GUI_DEBUG
#define GUI_DEBUG			0
#endif

#ifndef GUI_DEBUG_MESSAGES
#define GUI_DEBUG_MESSAGES  0
#endif


#ifndef GUI_DEBUG_WIN
#define GUI_DEBUG_WIN		0
#endif

#ifndef GUI_DEBUG_INVALIDATE
#define GUI_DEBUG_INVALIDATE	0
#endif

#if !GUI_DEBUG_INVALIDATE
#ifndef 	INVALIDATE_INFO
#define 	INVALIDATE_INFO			0
#endif	//INVALIDATE_INFO
#else // !GUI_DEBUG_INVALIDATE
// do not change the following lines
#ifdef 		INVALIDATE_INFO
#undef		INVALIDATE_INFO
#define 	INVALIDATE_INFO			0
#endif	// INVALIDATE_INFO
#endif // !GUI_DEBUG_INVALIDATE

#if GUI_DEBUG
#	define GUI_TRACE_CHAR(ch)	TRACE_CHAR(ch)
#	define GUI_TRACE(...)		TRACE(__VA_ARGS__)
#	define GUI_TRACE1(str)		TRACE1(str)
#   define GUI_TRACELN(str,...) 	TRACE("\r\n" str, ##__VA_ARGS__)
#   define GUI_TRACELN1(str) 		TRACE1("\r\n" str)
#else
#	define GUI_TRACE_CHAR(ch)
#	define GUI_TRACE(...)
#	define GUI_TRACE1(str)
#   define GUI_TRACELN(str,...)
#   define GUI_TRACELN1(str)
#endif

#if GUI_DEBUG_WIN
#	define GUI_WIN_TRACE_CHAR(ch)	TRACE_CHAR(ch)
#	define GUI_WIN_TRACE(...)		TRACE(__VA_ARGS__)
#	define GUI_WIN_TRACE1(str)		TRACE1(str)
#   define GUI_WIN_TRACELN(str,...) 	TRACE("\r\n" str, ##__VA_ARGS__)
#   define GUI_WIN_TRACELN1(str) 		TRACE1("\r\n" str)
#else
#	define GUI_WIN_TRACE_CHAR(ch)
#	define GUI_WIN_TRACE(...)
#	define GUI_WIN_TRACE1(str)
#   define GUI_WIN_TRACELN(str,...)
#   define GUI_WIN_TRACELN1(str)
#endif

#if GUI_DEBUG_MESSAGES || GUI_DEBUG || GUI_DEBUG_INVALIDATE
#define GUI_GET_OBJECT_TYPE(x)	virtual object_type get_object_type(void)	\
										{									\
											return x;						\
										}
extern STR_LIST obj_type_str;
extern void gui_trace_sleep();

#else
#define gui_trace_sleep()
#define GUI_GET_OBJECT_TYPE(X)
#endif

#if GUI_DEBUG_INVALIDATE
struct RECT_T;
struct GObject;
extern  void trace_enter(const char*line, const RECT_T& rect,  GObject* ob);
extern void trace_leave(const char*line, const RECT_T& rect, GObject* ob);

#define INVALIDATE_ENTER_FUNCTION(x, obj)trace_enter(__FUNCTION__,  x, obj);
#define INVALIDATE_LEAVE_FUNCTION(x, obj) trace_leave(__FUNCTION__,  x, obj);

#else

#define INVALIDATE_ENTER_FUNCTION(x, obj)
#define INVALIDATE_LEAVE_FUNCTION(x, obj)

#endif

#if INVALIDATE_INFO
#define INVALIDATE_TRACELN(str,...) 	TRACE("\r\n" str, ##__VA_ARGS__)
#else
#define INVALIDATE_TRACELN(str,...)
#endif

#if GUI_DEBUG_MESSAGES
struct GMessage;
extern STR_LIST wm_dbg_str;
void GDebug_trace_message(const GMessage& msg, bool push= true);

#define GUI_DEBUG_MESSAGES_DELITED() TRACELN("\e[1;33m %X[%d] ( %s 0x%X ) deleted!\e[m", \
		this->items[indx].dst, this->items[indx].dst->id,	\
		szlist_at(wm_dbg_str, this->items[indx].code), this->items[indx].param)

#else
#define GUI_DEBUG_MESSAGES_DELITED()
#endif

#if TRACE_IS == TRACE_DISABLED
#define GUI_ASSERT(expr)
#define GUI_ASSERT_MSG(expr, msg)
#else
#define GUI_ASSERT(expr)	 						\
	do {                                      	\
		 if(!(expr))                        	\
		 {                                  	\
			 TRACELN("ASSERT %s at %d in %s",__FILE__, __LINE__, __FUNCTION__); \
		 }                                  	\
    } while (0)
#define GUI_ASSERT_MSG(expr, msg)	 						\
	do {                                      	\
		 if(!(expr))                        	\
		 {                                  	\
			 TRACELN("ASSERT %s at %d in %s ",__FILE__, __LINE__, __FUNCTION__); \
			 TRACE1(msg);						\
		 }                                  	\
    } while (0)
#endif




#endif /* GDBG_OPT_H_ */
