/*
 * dimensions.h
 *
 *  Created on: May 4, 2023
 *      Author: bratkov
 */

#ifndef BASIC_SHAPES_H_
#define BASIC_SHAPES_H_

#include <tmos.h>

struct POINT_T
{
	union{
		struct {short int x, y;};
		int as_int;
	}__attribute__((packed));

	POINT_T() : as_int(0) {;}
	POINT_T(const short int& x_t, const short int& y_t): x(x_t), y(y_t) {;}
	POINT_T(int p): as_int(p) {;}

	POINT_T& operator= (const POINT_T& p_t) = default;
	inline bool operator== (const POINT_T& p_t) const;
	POINT_T operator+(const POINT_T& op);
	POINT_T& operator+=(const POINT_T& op);

	inline operator bool() const;
};

struct point_t
{
	union{
	struct {short int x, y;};
	int as_int;
	}__attribute__((packed));
	operator POINT_T() const
	{
		return POINT_T (as_int);
	}
};

union rec_t{
	struct {short int x0, y0, x1, y1;};
	unsigned long long as_int;
}__attribute__((packed));

typedef unsigned long long LPARAM;

struct RECT_T
{
	union{
		struct {short int x0, y0, x1, y1;};
		LPARAM as_int;
		struct {
			point_t p0;
			point_t p1;
		};
	}__attribute__((packed));

	RECT_T ()
		: as_int(0) {;}
	RECT_T (const unsigned long long& val)
		: as_int(val) {;}
	RECT_T (const short int& x0_t, const short int& y0_t, const short int& x1_t, const short int& y1_t)
		: x0(x0_t),  y0(y0_t), x1(x1_t), y1(y1_t) {;}
	RECT_T (const POINT_T& p0_t, const POINT_T& p1_t);
	RECT_T (const POINT_T& p0_t, const short int& xs, const short int& ys );

	inline operator LPARAM() const
		{ return as_int;}

    bool normalize (const RECT_T& rect);
    bool normalize (short int x0_t, short int y0_t, short int x1_t, short int y1_t);

    RECT_T& operator= (const RECT_T& rect_t) = default;
    RECT_T& operator= (int val);
    RECT_T& operator= (unsigned long long val);
    bool operator== (int val) const;
	bool operator== (const RECT_T& rect_t) const;
	short int width() const;
	short int height()const;
	inline operator bool() const
		{ return (as_int != 0LL); }
	void Inflate(int x, int y);
	void Inflate(int l, int t, int r, int b);
	void Deflate(int x, int y);
	void Deflate(int l, int t, int r, int b);
	void Offset(int x, int y);

#if GUI_DEBUG || GUI_DEBUG_INVALIDATE
	void dump() const
	{
		TRACE(" [x(%d, %d), y(%d, %d)]", x0, x1, y0, y1);
	}
#else
	inline void dump() const {;}
#endif
};

#endif /* BASIC_SHAPES_H_ */
