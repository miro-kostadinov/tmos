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


WEAK_C char TranslateKey( unsigned int key_code)
{
	return 0;
}


//*----------------------------------------------------------------------------
//*			sin(x) / cos(x) used for polar transformations
//*----------------------------------------------------------------------------

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


//*----------------------------------------------------------------------------
//*			Point
//*----------------------------------------------------------------------------
//POINT_T& POINT_T::operator= (POINT_T& p_t)
//{
//	if(this != &p_t)
//		as_int = p_t.as_int;
//	return *this;
//}
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


//RECT_T& RECT_T::operator= (const RECT_T& rect_t)
//{
//	if(this != &rect_t)
//		as_int = rect_t.as_int;
//	return *this;
//}

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


