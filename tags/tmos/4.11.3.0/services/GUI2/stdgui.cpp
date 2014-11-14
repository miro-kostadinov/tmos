/*
 * display.c
 *
 *  Created on: 2008-10-19
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include <stdgui.h>
#include <lcd.h>


unsigned int current_laguage;
#if GUI_DEBUG
STR_LIST wm_dbg_str = SZ(WM_DELETED) SZ(WM_QUIT) SZ(WN_DESTROY) SZ(WM_CLOSE)
		SZ(WM_COMMAND) SZ(WM_CHANGE) SZ(WM_IDLE) SZ(WM_SET_FLAGS)
		SZ(WM_CLR_FLAGS) SZ(WM_SETFOCUS) SZ(WM_KILLFOCUS) SZ(WM_TIMER) SZ(WM_INIT) SZ(WM_DRAW) SZ(WM_KEY);

void trace_message(const GMessage& msg)
{
	TRACELN1("\e[4;1;32m");
	if(msg.dst)
		TRACE("%X[%d] ( %s 0x%X/%d\e[m", msg.dst, msg.dst->id, szlist_at(wm_dbg_str, msg.code), msg.param, msg.param);
	else
		TRACE("%X[INVALID] ( %s 0x%X/%d\e[m", msg.dst, szlist_at(wm_dbg_str, msg.code), msg.param, msg.param);
	if(msg.code == WM_DRAW)
	{
		if(msg.lparam)
			RECT_T(msg.lparam).dump();
		else
			msg.dst->rect.dump();
	}
	else
		TRACE(" %lX ", msg.lparam);
	TRACE1("\e[4;1;32m)\e[m");
}
#endif

WEAK_C char TranslateKey( unsigned int key_code)
{
	return 0;
}

//*----------------------------------------------------------------------------
//*			Point
//*----------------------------------------------------------------------------
POINT_T& POINT_T::operator= (POINT_T p_t)
{
	if(this != &p_t)
		as_int = p_t.as_int;
	return *this;
}
bool POINT_T::operator== (POINT_T p_t) const
{
	return (as_int == p_t.as_int);
}
POINT_T POINT_T::operator+(const POINT_T& op) const
{
	POINT_T res(x + op.x, y + op.y);
	return res;
}

POINT_T& POINT_T::operator+=(const POINT_T& op)
{
	x += op.x;
	y += op.y;
	return *this;
}

POINT_T::operator bool() const
{
	return (as_int != 0);
}

//*----------------------------------------------------------------------------
//*			Rectangle
//*----------------------------------------------------------------------------
RECT_T::RECT_T (const POINT_T& p0_t, const POINT_T& p1_t)
{
	p0.as_int = p0_t.as_int; p1.as_int = p1_t.as_int;
}
RECT_T::RECT_T (const POINT_T& p0_t, const short int& xs, const short int& ys )
{
	p0.as_int = p0_t.as_int;
	x1 = p0.x + xs; y1 = p0.y + ys;
}


RECT_T& RECT_T::operator= (const RECT_T& rect_t)
{
	if(this != &rect_t)
		as_int = rect_t.as_int;
	return *this;
}

RECT_T& RECT_T::operator= (int val)
{
	x0=y0=x1=y1=val;
	return *this;
}
RECT_T& RECT_T::operator= (unsigned long long val)
{
	as_int = val;
	return *this;
}
bool RECT_T::operator== (int val) const
{
	return (x0==val && y0== val && x1==val && y1== val);
}
bool RECT_T::operator== (RECT_T rect_t) const
{
	return (as_int == rect_t.as_int);
}
short int RECT_T::width() const
{
	return (x1 - x0);
}
short int RECT_T::height()const
{
	return (y1 - y0);
}

RECT_T::operator bool() const
{
	return (as_int != 0);
}

void RECT_T::Inflate(int x, int y)
{
	x0 +=x; x1 -=x;
	y0 +=y; y1 -=y;
}

void RECT_T::Inflate(int l, int t, int r, int b)
{
	x0 += l; x1 -=r;
	y0 += t; y1 -=b;
}

void RECT_T::Deflate(int x, int y)
{
	x0 -=x; x1 +=x;
	y0 -=y; y1 +=y;
}

void RECT_T::Deflate(int l, int t, int r, int b)
{
	x0 -= l; x1 +=r;
	y0 -= t; y1 +=b;
}

void RECT_T::Offset(int x, int y)
{
	x0 += x; x1 += x;
	y0 += y; y1 += y;
}

bool RECT_T::normalize (const RECT_T& rect)
{
	return normalize (rect.x0, rect.y0, rect.x1, rect.y1);
}

bool RECT_T::normalize (short int x0_t, short int y0_t, short int x1_t, short int y1_t)
{
	if (x0 > x1_t)
	{
		*this = NULL;
		return false;
	}
	if (y0 > y1_t)
	{
		*this = NULL;
		return false;
	}
	if (x0 < x0_t)
		x0 = x0_t;
	if (y0 < y0_t)
		y0 = y0_t;
	if (x1 < x0)
	{
		*this = NULL;
		return false;
	}
	if (y1 < y0)
	{
		*this = NULL;
		return false;
	}
	if (x1 > x1_t)
		x1 = x1_t;
	if (y1 > y1_t)
		y1 = y1_t;
	return true;
}


