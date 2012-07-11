/*
 * services.cpp
 *
 *  Created on: Jul 11, 2012
 *      Author: stanly
 */

#include <tmos.h>


void TRACE_BUF(const void* buf, unsigned int len)
{
	const char* str = (const char*)buf;

	while(len--)
	{
		if(*str < ' ')
		{
			TRACE("{%02X}", *str);
		} else
		{
			TRACE_CHAR(*str);
		}
		str++;
	}
}

void TRACE_BUF(const void* buf, unsigned int len, unsigned int color)
{
	TRACE1("\e[");
	if(color <100)
	{
		if(color>9)
		{
			TRACE_CHAR('0'+ (color/10));
			color %= 10;
		}
		TRACE_CHAR('0'+ color);
	}
	TRACE_CHAR('m');
	TRACE_BUF(buf, len);
	TRACE1("\e[m");
}

void TRACE_BUF(const void* buf, unsigned int len, unsigned int c1, unsigned int c2)
{
	TRACE1("\e[");
	if(c1 <100)
	{
		if(c1>9)
		{
			TRACE_CHAR('0'+ (c1/10));
			c1 %= 10;
		}
		TRACE_CHAR('0'+ c1);
		if(c2 <100)
		{
			TRACE_CHAR(';');
			if(c2>9)
			{
				TRACE_CHAR('0'+ (c2/10));
				c2 %= 10;
			}
			TRACE_CHAR('0'+ c2);
		}
	}
	TRACE_CHAR('m');
	TRACE_BUF(buf, len);
	TRACE1("\e[m");
}
