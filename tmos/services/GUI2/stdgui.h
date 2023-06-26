#ifndef STDGUI_H
#define STDGUI_H

#include <tmos.h>
#include <gdbg_opt.h>
#include <mqueue.h>

#ifndef GUI_LANGUAGES
#define GUI_LANGUAGES	2
#endif

#ifndef GUI_DISPLAYS
#define GUI_DISPLAYS	1
#endif


#ifndef GUI_DEFAULT_DISP
#define GUI_DEFAULT_DISP	0xFF
#endif


#ifndef GO_SCROLL_WIDTH
#define GO_SCROLL_WIDTH		5
#endif

#ifndef GO_OBJ_FRAME_WIDTH
#define GO_OBJ_FRAME_WIDTH	3
#endif

#ifndef GO_OBJ_FRAME_HEIGHT
#define GO_OBJ_FRAME_HEIGHT	1
#endif

// sin(x)/cos(x) and other math functions
#ifndef USE_GUI_MATH
#define USE_GUI_MATH 1
#endif

#define KEYBOARD_WITH_ARROWS		(((KEY_DRV_COUNT * KEY_RD_COUNT) + KEY_FIX_COUNT) <= 6)
// object messages queue
#ifndef MAX_MESSAGES
#define MAX_MESSAGES 10
#endif

enum WM_MESSAGE:unsigned int
{
	WM_DELETED =0,
	WM_QUIT,
	WN_DESTROY,
	WM_CLOSE,
	WM_COMMAND,
	WM_CHANGE,
	WM_IDLE,
	WM_SET_FLAGS,
	WM_CLR_FLAGS,
	WM_SETFOCUS,
	WM_KILLFOCUS,
	WM_TIMER,
	WM_INIT,
	WM_DRAW,
	WM_SYSCTRL,			// the controls specific action
	WM_SYSCTRL_SET,		// setting the controls flags (align)
	WM_SYSCTRL_CLR,		// clearing the controls flags (align)
	WM_KEY,
	WM_USER				// after this message follow user defined commands
};

typedef unsigned long long LPARAM;
typedef unsigned int  WPARAM;

struct GObject;

struct GMessage
{
	WM_MESSAGE code;
	WPARAM param;
	LPARAM lparam;
	GObject* dst;

	GMessage () : code(WM_QUIT), param(0), lparam(0), dst(nullptr)
	{;}
	GMessage (WM_MESSAGE code_, WPARAM wparam_, LPARAM lparam_, GObject* destination_):
		code (code_), param (wparam_), lparam (lparam_), dst (destination_)
	{;}
	GMessage (WM_MESSAGE code_, WPARAM wparam_, GObject* destination_):
		code (code_), param (wparam_), lparam (0L), dst (destination_)
	{;}

	GMessage (GMessage&& arg) = delete;
	GMessage (GMessage& arg) = delete;

	GMessage& operator= (const GMessage& msg) = default;
};

template< const int size>
struct msgQueue : mqueue<GMessage, size>
{
	msgQueue(): mqueue<GMessage, size>()
		{;}

	bool del_msg_for(GObject* owner);
#if GUI_DEBUG_MESSAGES
	bool pop(GMessage& msg);
	bool push(const GMessage& msg);
#endif
};

template<const int size>
bool msgQueue<size>::del_msg_for(GObject* msg_owner)
{
	unsigned short indx = this->out;
	bool res = false;

	while(this->in != indx)
	{
		if(this->items[indx].dst == msg_owner)
		{
			GUI_DEBUG_MESSAGES_DELITED();
			this->items[indx].code = WM_DELETED;
			res = true;
		}
		if(++indx == size)
			indx=0;
	}
	return res;
}

#if GUI_DEBUG_MESSAGES
template<const int size>
bool msgQueue<size>::pop(GMessage& msg)
{
	if(mqueue<GMessage, size>::pop(msg))
	{
		GDebug_trace_message(msg, false);
		return true;
	}
	return false;
}

template<const int size>
bool msgQueue<size>::push(const GMessage& msg)
{
	if(mqueue<GMessage, size>::push(msg))
	{
		GDebug_trace_message(msg);
		return true;
	}
	return false;
}
#endif

extern msgQueue<MAX_MESSAGES> GQueue;

void processes_all_messages(void);
void send_message(WM_MESSAGE wm, unsigned int param, unsigned long long lparam, GObject* dst);
void send_message(GMessage& msg, GObject* dst);


#ifndef GUI_IDLE_MSG_PERIOD
#define GUI_IDLE_MSG_PERIOD	15000 //!< period (in ms ) during which the message is sent WM_IDLE
#endif

// base object types
enum object_type:unsigned char
{
	OBJECT_OBJECT=0,
	OBJECT_CONTAINER,
	OBJECT_BUTTON,
	OBJECT_EDIT,
	OBJECT_VKB,
	OBJECT_FBUTTON,
	OBJECT_FTEXT,
	OBJECT_LISTBOX,
	OBJECT_MENU,
	OBJECT_RADIO,
	OBJECT_TEXT,
// window objects
	OBJECT_WINDOW,
	OBJECT_DIALOG,
	OBJECT_MESSAGEBOX,
	OBJECT_DOWAIT,
	OBJECT_DISPLAY,
	OBJECT_DISPLAY_MULTIPLEXER,
	OBJECT_CPU_USAGE
};

// base object flags
#define GO_FLG_BORDER		0x01
#define GO_FLG_SELECTED		0x02
#define GO_FLG_CHECKED		0x04
#define GO_FLG_ENABLED		0x08
#define GO_FLG_HSCROLL		0x10
#define GO_FLG_VSCROLL		0x20
#define GO_FLG_SHOW			0x40
#define GO_FLG_TRANSPARENT	0x80

#define GO_FLG_DEFAULT		GO_FLG_ENABLED|GO_FLG_SHOW
typedef unsigned char GFlags;

// reserved command/return id
#define GO_EXIT				0x00
#define GO_IDOK				0x01
#define GO_IDCANCEL			0x02
#define GO_IDRETRY			0x04
#define GO_IDYES			0x05
#define GO_IDNO				0x06

typedef unsigned char GId;

// client handle states
#define GUI_HND_OPEN	1
#define GUI_HND_ATTACH	2
#define GUI_HND_DETACH	3
#define GUI_HND_UNUSED	0

/*
 *  text styles,  drawing and input control modes
 */
// ---------------------------------------------  text alignment
// align mode  bit[0-3]
// horizontal bit[0-1]
#define TA_LEFT			0x00
#define TA_CENTER		0x01
#define TA_RIGHT 		0x02
#define TA_HORIZONTAL	0x03
// vertical  bit[2-3]
#define TA_TOP			(0x00<<2)
#define TA_MIDDLE		(0x01<<2)
#define TA_BOTTOM		(0x02<<2)
#define TA_VERTICAL		(0x03<<2)

// control styles

//-----------------------------------------------  static text
// align bit[0 - 3]
#define SS_RIGHT		TA_RIGHT
#define SS_CENTER    	TA_CENTER
#define SS_LEFT    		TA_LEFT
#define SS_TOP    		TA_TOP
#define SS_MIDDLE    	TA_MIDDLE
#define SS_BOTTOM  		TA_BOTTOM
// control draw  mode  bit[4,5]
#define SS_WORDWRAP 	(1<<4)
#define SS_AUTO_SCROLL	(1<<5)

#define SS_DEFAULT		(SS_WORDWRAP|SS_CENTER|SS_MIDDLE)


//-----------------------------------------------  edit control
// align bit[0 - 3]
#define ES_RIGHT		TA_RIGHT
#define ES_CENTER    	TA_CENTER
#define ES_LEFT    		TA_LEFT
#define ES_TOP    		TA_TOP
#define ES_MIDDLE    	TA_MIDDLE
#define ES_BOTTOM  		TA_BOTTOM
// control draw  mode  bit[4,5]
#define ES_MULTILINE	SS_WORDWRAP
#define ES_AUTO_SCROLL	SS_AUTO_SCROLL
// additional control styles for edit box
// bit[6 - 11]
#define ES_PASSWORD				(1<<6)
#define ES_WANTRETURN   		(1<<7)
#define ES_HIDE_CURSOR			(1<<8)
#define ES_LEAD_ZERO			(1<<9)
#define ES_NUMERIC				(1<<10)
#define ES_USE_VIRTUAL_KEYBOARD	(1<<11)  //!< set when used small keyboard (only arrows and OK/CANCEL)

#define ES_DEFAULT		(ES_CENTER|ES_MIDDLE|ES_MULTILINE|ES_AUTO_SCROLL)

//-----------------------------------------------   button control

#define GB_TYPE_NONE		0
#define GB_TYPE_RADIO		1
#define GB_TYPE_CHECK		2

// icon index
enum guistd_icons_index:uint8_t
{
	GICON_CIRCLE,
	GICON_CHECKED_CIRCLE,
	GICON_SQUARE,
	GICON_CHECKED_SQUARE,
	GICON_SQUARE_SQUARE,
	GICON_UP,
	GICON_DOWN,
	GICON_LEFT,
	GICON_RIGHT
};

//-----------------------------------------------   list box
// additional control styles for list box
// bit 12
#define LBS_DROPDOWN	(1<<12)
#define LBS_DEFAULT		(SS_WORDWRAP|SS_LEFT|SS_MIDDLE)

//-----------------------------------------------   message box

#define ID_MB_DLG		0xF0
#define ID_MB_TITLE		0xF1
#define ID_MB_TEXT_BOX	0xF2

// buttons text index
#define IDS_OK			0
#define IDS_RETRY		1
#define IDS_YES			2
#define IDS_NO			3
#define IDS_CANCEL		4

// buttons flags
#define MBF_OK			(1<<0)
#define MBF_RETRY		(1<<1)
#define MBF_YES			(1<<2)
#define MBF_NO			(1<<3)
#define MBF_CANCEL		(1<<4)
#define MBF_LAST_BTN	(1<<5)
// control flags
#define MBF_EDIT		(1<<5)
#define MBF_CLR			(1<<6)

// control fields
#define MBF_INPUT_TYPE(type) 	(((type)&0xF)<<8)
#define MBF_EDIT_FLAGS(flags) 	((flags)<<16)
#define MBF_BEEP_TYPE(num)		(((num)&0xF)<<12)

#define GET_MBF_INPUT_TYPE(mb_type)	((key_mode)(((mb_type)>>8)&0xF))
#define GET_MBF_EDIT_FLAGS(mb_type)	(((mb_type)>>16)&0xFFFF)
#define GET_MBF_BEEP_TYPE(mb_type)	(((mb_type)>>12)&0xF)

#define MB_OK			MBF_OK
#define MB_OKCANCEL 	(MBF_OK|MBF_CANCEL)
#define	MB_RETRYCANCEL  (MBF_RETRY|MBF_CANCEL)
#define MB_YESNO		(MBF_YES|MBF_NO)
#define MB_YESNOCANCEL  (MBF_YES|MBF_NO|MBF_CANCEL)

#define MB_BEEP(code)	MBF_BEEP_TYPE(code)
/*
 * Message(Edit)Box style format
 * ------------------------------------------------------------------------------
 *  31   30   29   28   27   26   25   24   23   22   21   20   19   18   17   16
 *
 *  15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
 * |   beep type      | BG/EN/bg/en/123   | ctrl flags   |   Buttons             |
 *  nc   nc   nc   nc   nc   nc   nc   nc   nc   CLR  EDI  CANC  NO  YES  RETRY OK
 */

extern unsigned int current_laguage;

extern "C" char TranslateKey( unsigned int key_code);

enum GUI_KEYS : unsigned char
{
	KEY_0 = 1,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_ENTER,
	KEY_CANCEL,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_ESC,
	KEY_POINT,
	KEY_SHIFT,
	KEY_X,
	KEY_USER_DEFINED
};

#if USE_GUI_MATH
// math utility
int32_t sin_x10000(int deg);
int32_t cos_x10000(int deg);
#endif

// GUI base
#include <base_colors.h>
#include <basic_shapes.h>
#include <fonts.h>
#include <gobject.h>
#include <gcontainer.h>
#include <gui_drv.h>
// GUI lcds
#include <lcd.h>
#include <lcd_multiplex.h>
// GUI api_windows
#include <gwindow.h>
#include <gdialog.h>
#include <gmsgbox.h>
// GUI controls
#include <gtext.h>
#include <gmenu.h>
#include <gbutton.h>
#include <gfloating_text.h>
#include <gradio.h>
#include <gclockview.h>
#include <gfloating_button.h>
#include <gedit.h>
#include <glistbox.h>
#include <gdatetime.h>
// CUI auxiliary
#include <gscroll.h>
#include <gdowait.h>
#include <gedit_vkb.h>

// DEPRECATED
struct  __attribute__ ((deprecated ("replace with GDateTime")))GTimeEdit;
struct __attribute__ ((deprecated ("replace with GClockView"))) GClock;

#endif

