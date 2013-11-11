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

#define GUI_DEBUG			1
// object messages
enum WM_MESSAGE:unsigned int
{
	WM_QUIT=0,
	WN_DESTROY,
	WM_CLOSE,
	WM_COMMAND,
	WM_CHANGE,
	WM_IDLE,
	WM_SET_FLAGS,
	WM_CLR_FLAGS,
	WM_INIT,
	WM_DRAW,
	WM_KEY
};

extern STR_LIST wm_dbg_str;

#define MAX_MESSAGES 10



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
#define GO_IDRETRY			0x03
#define GO_IDYES			GO_IDOK
#define GO_IDNO				GO_IDCANCEL

typedef unsigned char GId;

// client handle states
#define GUI_HND_OPEN	1
#define GUI_HND_ATTACH	2
#define GUI_HND_DETACH	3
#define GUI_HND_UNUSED	0


#define TA_LEFT			0x00
#define TA_CENTER		0x01
#define TA_RIGHT 		0x02
#define TA_HORIZONTAL	0x03

#define TA_TOP			(0x00<<2)
#define TA_MIDDLE		(0x01<<2)
#define TA_BOTTOM		(0x02<<2)
#define TA_VERTICAL		(0x03<<2)

extern unsigned int current_laguage;

extern "C" char TranslateKey( unsigned int key_code);


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

	RECT_T& operator<<=(const POINT_T& pt);

#if GUI_DEBUG
	void dump()
	{
		TRACE("[%d, %d, %d, %d]", x0, x1, y0, y1);
	}
#else
	void dump() {};
#endif
};

#endif
