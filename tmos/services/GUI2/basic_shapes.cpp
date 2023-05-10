/*
 * dimensions.cpp
 *
 *  Created on: May 4, 2023
 *      Author: bratkov
 */

#include <basic_shapes.h>

//*----------------------------------------------------------------------------
//*			Point
//*----------------------------------------------------------------------------
inline bool POINT_T::operator== (const POINT_T& p_t) const
{
	return (as_int == p_t.as_int);
}
POINT_T POINT_T::operator+(const POINT_T& op)
{
	return POINT_T(x + op.x, y + op.y);
}

POINT_T& POINT_T::operator+=(const POINT_T& op)
{
	x += op.x;
	y += op.y;
	return *this;
}

inline POINT_T::operator bool() const
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

bool RECT_T::operator== (const RECT_T& rect_t) const
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




