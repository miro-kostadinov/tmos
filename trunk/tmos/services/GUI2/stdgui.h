#ifndef STDGUI_H
#define STDGUI_H

#include <tmos.h>

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
#define GO_OBJ_FRAME_HEIGHT	3
#endif

#define GUI_DEBUG			0
// object messages
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
	WM_KEY
};

extern STR_LIST wm_dbg_str;

#define MAX_MESSAGES 10

#define IDLE_PERIOD			15000 //!< period (in ms ) during which the message is sent WM_IDLE

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

// align mode
#define TA_LEFT			0x00
#define TA_CENTER		0x01
#define TA_RIGHT 		0x02
#define TA_HORIZONTAL	0x03

#define TA_TOP			(0x00<<2)
#define TA_MIDDLE		(0x01<<2)
#define TA_BOTTOM		(0x02<<2)
#define TA_VERTICAL		(0x03<<2)

// control styles

// static text
#define SS_RIGHT		TA_RIGHT
#define SS_CENTER    	TA_CENTER
#define SS_LEFT    		TA_LEFT
#define SS_TOP    		TA_TOP
#define SS_MIDDLE    	TA_MIDDLE
#define SS_BOTTOM  		TA_BOTTOM
#define SS_WORDWRAP 	(1<<8)

#define SS_DEFAULT		(SS_WORDWRAP|SS_CENTER|SS_MIDDLE)


// edit control
#define ES_RIGHT		TA_RIGHT
#define ES_CENTER    	TA_CENTER
#define ES_LEFT    		TA_LEFT
#define ES_TOP    		TA_TOP
#define ES_MIDDLE    	TA_MIDDLE
#define ES_BOTTOM  		TA_BOTTOM
#define ES_MULTILINE	SS_WORDWRAP
#define ES_PASSWORD		(1<<9)
#define ES_WANTRETURN   (1<<10)
#define ES_HIDE_CURSOR	(1<<11)
#define ES_AUTO_SCROLL	(1<<12)
#define ES_LEAD_ZERO	(1<<14)

#define ES_DEFAULT		(ES_CENTER|ES_MULTILINE|ES_AUTO_SCROLL)

// list box
#define LBS_DROPDOWN	(1<<13)
#define LBS_DEFAULT		(SS_WORDWRAP|SS_LEFT|SS_MIDDLE)

extern unsigned int current_laguage;

extern "C" char TranslateKey( unsigned int key_code);

// icon index
#define GICON_CIRCLE			0
#define GICON_CHECKED_CIRCLE	1
#define GICON_SQUARE			2
#define GICON_CHECKED_SQUARE	3
#define GICON_SQUARE_SQUARE		4
#define GICON_UP				5
#define GICON_DOWN				6
#define GICON_LEFT				7
#define GICON_RIGHT				8


struct POINT_T
{
	union{
		struct {short int x, y;};
		int as_int;
	};
	POINT_T() : as_int(0) {}
	POINT_T(const short int& x_t, const short int& y_t): x(x_t), y(y_t) {}
	POINT_T(int p): as_int(p) {}

	POINT_T& operator= (POINT_T p_t);
	bool operator== (POINT_T p_t) const;
	POINT_T operator+(const POINT_T& op) const;
	POINT_T& operator+=(const POINT_T& op);

	operator bool() const;
};

struct point_t
{
	union{
	struct {short int x, y;};
	int as_int;
	};
	operator POINT_T() const
	{
		return POINT_T (as_int);
	}
};

union rec_t{
	struct {short int x0, y0, x1, y1;};
	unsigned long long as_int;
};

struct RECT_T
{
	union{
		struct {short int x0, y0, x1, y1;};
		unsigned long long as_int;
		struct {
			point_t p0;
			point_t p1;
		};
	};

	RECT_T ()
		: as_int(0) {;}
	RECT_T (const unsigned long long& val)
		: as_int(val) {;}
	RECT_T (const short int& x0_t, const short int& y0_t, const short int& x1_t, const short int& y1_t)
		: x0(x0_t),  y0(y0_t), x1(x1_t), y1(y1_t) {;}
	RECT_T (const POINT_T& p0_t, const POINT_T& p1_t);
	RECT_T (const POINT_T& p0_t, const short int& xs, const short int& ys );

    bool normalize (const RECT_T& rect_t);
    bool normalize (short int x0_t, short int y0_t, short int x1_t, short int y1_t);

    RECT_T& operator= (const RECT_T& rect_t);
    RECT_T& operator= (int val);
    RECT_T& operator= (unsigned long long val);
    bool operator== (int val) const;
	bool operator== (RECT_T rect_t) const;
	short int width() const;
	short int height()const;
	operator bool() const;
	void Inflate(int x, int y);
	void Inflate(int l, int t, int r, int b);
	void Deflate(int x, int y);
	void Deflate(int l, int t, int r, int b);
	void Offset(int x, int y);

#if GUI_DEBUG
	void dump()
	{
		TRACE("[x(%d, %d), y(%d, %d)]", x0, x1, y0, y1);
	}
#else
	void dump() {};
#endif
};

RES_CODE msg_error(CSTRING& msg, int err_code);
RES_CODE msg_error(const char *msg, int err_code);

#endif
