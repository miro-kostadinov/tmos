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

void TRACE_COLOR(unsigned int color)
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
}

void TRACE_COLOR_END()
{
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

void TRACE_TEXT(const void* buf, unsigned int len)
{
	const char* str = (const char*)buf;

	while(len--)
	{
		if(*str < ' ' && *str != '\t' && *str != '\r' && *str != '\n')
		{
			TRACE("{%02X}", *str);
		} else
		{
			TRACE_CHAR(*str);
		}
		str++;
	}
}

void TRACE_TEXT(const void* buf, unsigned int len, unsigned int color)
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
	TRACE_TEXT(buf, len);
	TRACE1("\e[m");
}

void drv_enable_isr(DRIVER_INFO drv_info)
{
	__IO uint32_t* reg;
	uint32_t pos;

	//set priority
	reg = &NVIC->NVIC_IPR[drv_info->drv_index >> 2];
	pos = ((drv_info->drv_index & 3) << 3);
	reg[0] = (reg[0] & ~(0xFF << pos)) | (drv_info->isr_priority << pos);

    //enable
    NVIC->NVIC_ISER = 1 << drv_info->drv_index;
}
