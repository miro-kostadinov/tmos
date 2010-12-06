/*
 * tmos_stdlib.cpp
 *
 *  Created on: 10.11.2010
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include "tmos_stdlib.h"
#include "tmos_string.h"
#include <trace.h>

/*-----------------------------------------------------------
 * 			Debug Dinamic Memory
 *----------------------------------------------------------*/

void* operator new(size_t size)
{
    return tsk_malloc(size);
}

void* operator new[](size_t size)
{
    return tsk_malloc(size);
}

void operator delete(void *p)
{
	tsk_free(p);
}

void operator delete[](void *p)
{
	tsk_free(p);
}



#if TRACE_MEMORY_LEVEL >= TRACE_DEFAULT_LEVEL
unsigned int allocated_size;
unsigned int allocated_count;
void* tsk_malloc(unsigned int size)
{
	register unsigned int lr asm("r14");
	void* ptr;
	int buf[2];
	ptr = usr_malloc(size);
	buf[1] = 0;
	*buf = *((int*)((void*)CURRENT_TASK->name));
	if(ptr)
	{
		allocated_size += dyn_sizeof(ptr);
		allocated_count++;
		TRACE_MEMORY("\r\n+%x[%s %x<%d,%d]", ptr, buf, lr, allocated_count, allocated_size);
	} else
	{
		TRACE("\r\nERROR:[%s+%d]", buf, size);

	}
	return ptr;
}

void* tsk_malloc_clear(unsigned int size)
{
	register unsigned int _lr asm("r14");
	unsigned int lr = _lr;
	int buf[2];
	void * ptr;

	ptr = usr_malloc(size);
	buf[1] = 0;
	*buf = *((int*)((void*)CURRENT_TASK->name));
	if(ptr)
	{
		allocated_size += dyn_sizeof(ptr);
		allocated_count++;
		TRACE_MEMORY("\r\n+%x[%s %x<%d,%d]", ptr, buf, lr, allocated_count, allocated_size);
	} else
	{
		TRACE("\r\nERROR:[%s+%d]", buf, size);

	}
	if(ptr)
		memclr(ptr, size);
	return ptr;
}


void  tsk_free(void* ptr)
{
	register unsigned int lr asm("r14");
	int buf[2];

	if(ptr)
	{
		if((char*)ptr > &end)
		{
			allocated_size -= dyn_sizeof(ptr);
			allocated_count--;
		}
		usr_free(ptr);
		if((char*)ptr > &end)
		{
			buf[1] = 0;
			*buf = *((int*)((void*)CURRENT_TASK->name));
			TRACE_MEMORY("\r\n-%x[%s %x >%d,%d]", ptr, buf, lr, allocated_count, allocated_size);
			buf[0] = CURRENT_TASK->time;
			tsk_sleep(10);
			CURRENT_TASK->time = buf[0];
		}
	}
}
void* tsk_realloc(void* ptr, unsigned int size)
{
	register unsigned int _lr asm("r14");
	int buf[2];
	unsigned int lr = _lr;

	buf[1] = 0;
	*buf = *((int*)((void*)CURRENT_TASK->name));
	if(ptr)
	{
		if((char*)ptr > &end)
		{
			allocated_size -= dyn_sizeof(ptr);
			allocated_count--;
			TRACE_MEMORY("\r\n-%x[%x<]", ptr, lr);
		}
	}
	ptr = usr_realloc(ptr, size);
	if(ptr)
	{
		allocated_size += dyn_sizeof(ptr);
		allocated_count++;
	} else
	{
		if(size)
		{
			TRACE("\r\nERROR:[%s=%d]", buf, size);
		} else
		{
			TRACE_MEMORY("\r\n+%x[%s %x=%d,%d]", ptr, buf, lr, allocated_count, allocated_size);
		}
	}
	return ptr;
}

#else
void* tsk_malloc_clear(unsigned int size)
{
	void * ptr;
	ptr = tsk_malloc(size);
	if(ptr)
		memclr(ptr, size);
	return ptr;
}

#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 					 helper functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
unsigned int atoi(const char *ptr )
{
	unsigned int res=0;
	if(ptr)
		tmos_sscanf(ptr, "%d", &res);
	return res;
}

unsigned int ahextoi(const char *ptr )
{
	unsigned int res=0;
	tmos_sscanf(ptr, "%x", &res);
	return res;
}

