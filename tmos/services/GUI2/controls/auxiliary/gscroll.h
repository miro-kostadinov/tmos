/*
 * gscroll.h
 *
 *  Created on: 07.10.2013
 *      Author: stanly
 */

#ifndef GSCROLL_H_
#define GSCROLL_H_

#include <stdgui.h>

struct LCD_MODULE;

struct GScroll
{
	struct{
	unsigned short	pos;
	unsigned short	page;
	unsigned int	ratio;
	GObject*		object;
	}__attribute__((packed));

	GScroll(GObject* object_t) : pos(0), page(0), ratio(0), object(object_t)
		{}

	bool ShowScroll(GFlags sb, bool visible);
	void draw_scroll(LCD_MODULE* lcd);
	void SetScrollPos(GFlags sb, unsigned short p, bool redraw = false);
	void SetScrollRange(GFlags sb, unsigned int p);
};

#endif /* GSCROLL_H_ */
