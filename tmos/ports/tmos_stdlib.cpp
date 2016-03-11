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
    return (tsk_malloc(size));
}


void* operator new[](size_t size)
{
    return (tsk_malloc(size));
}

void operator delete(void *p)
{
	tsk_free(p);
}

void operator delete[](void *p)
{
	tsk_free(p);
}

extern "C" char end;

#if TRACE_MEMORY_LEVEL >= TRACE_DEFAULT_LEVEL

void trace_sleep(void)
{
	unsigned int task_time = CURRENT_TASK->time;
	tsk_sleep(2);
	CURRENT_TASK->time = task_time;
}

void mm_info1( void )
{
	unsigned short* ptr = (unsigned short*)(void*)&end;

	__disable_irq();
	TRACE("\r\n==================================\r\n %08X %04X %04X  %04X ", ptr, ptr[0], ptr[1]>>1, ptr[2]);

	do
	{
		ptr += ptr[0] <<1;
		TRACE("\r\n %08X %04d %04d ", ptr+2, ptr[0]*4, (ptr[1]>>1)*4);
		if(!(ptr[1]&1))
		{
			TRACE1("              ");
		}
		TRACE(" %04X %04X", ptr[2], ptr[3]);
	}while(ptr[0]);
	__enable_irq();
}

int is_dynamic_mem(void* ptr)
{
	unsigned short* pool = (unsigned short*)(void*)&end;
	unsigned int adr = (unsigned int) ptr;
	int res = 0;
	int mem_size;

	if ((adr > (unsigned int) pool) && (adr < (0x20018000)))
	{
		adr -= 4;

		__disable_irq();
		mem_size = 0;
		do
		{
			pool += pool[0] << 1;
			if (adr == (unsigned int) pool)
			{
				// the address is here
				if (!(pool[1] & 1))
				{
					// the address is not free chunk
					res = 1;
				}
				else
				{
					TRACELN("ERROR: Pointer(0x%04X) is already free!", ptr);
				}
			}
			if (!(pool[1] & 1))
			{
//				TRACELN("%x   %d", pool+2, pool[0] <<2);
				mem_size += pool[0] << 2;
			}
		} while (pool[0]);

		if (mem_size != (PMAIN_TASK->aloc_mem<<2))
		{
			TRACELN("ERROR: bad mem size %08x (%d):(%d) %d", ptr, mem_size, (PMAIN_TASK->aloc_mem<<2), PMAIN_TASK->aloc_ptrs);
			__enable_irq();
			mm_info1();
			tsk_sleep(10);
			__disable_irq();
		}
		__enable_irq();
//		mm_info1();
	}
	if (ptr && !res)
		TRACELN("ERROR: bad mem ptr %08x", ptr);
	return (res);
}


void* tsk_malloc(unsigned int size)
{
	register unsigned int lr asm("r14");
	void* ptr;
	int buf[2];

//	tsk_sleep(1);
	ptr = CURRENT_TASK->name;
	*buf = *((int*)ptr);
	buf[1] = 0;

	ptr = usr_malloc(size);
	if(ptr)
	{
		TRACE_MEMORY("\r\n+%x[%s %x<%d,%d]", ptr, buf, lr, PMAIN_TASK->aloc_ptrs, (PMAIN_TASK->aloc_mem<<2));
	} else
	{
		TRACE("\r\nERROR:[%s+%d]", buf, size);

	}
	trace_sleep();
	is_dynamic_mem(ptr);
	return ptr;
}

void* tsk_malloc_clear(unsigned int size)
{
	register unsigned int _lr asm("r14");
	unsigned int lr = _lr;
	int buf[2];
	void * ptr;

	ptr = CURRENT_TASK->name;
	*buf = *((int*)ptr);
	buf[1] = 0;

	ptr = usr_malloc(size);
	if(ptr)
	{
		TRACE_MEMORY("\r\n+%x[%s %x<%d,%d]", ptr, buf, lr, PMAIN_TASK->aloc_ptrs, (PMAIN_TASK->aloc_mem<<2));
	} else
	{
		TRACE("\r\nERROR:[%s+%d]", buf, size);

	}
	trace_sleep();
	if(ptr)
		memclr(ptr, size);
	is_dynamic_mem(ptr);
	return ptr;
}

extern char end;

void  tsk_free(void* ptr)
{
	register unsigned int lr asm("r14");
	int buf[2];

//	tsk_sleep(1);
	is_dynamic_mem(ptr);
	if(ptr)
	{
		usr_free(ptr);
		if((char*)ptr > &end)
		{
			void* ptr1;

			ptr1 = CURRENT_TASK->name;
			*buf = *((int*)ptr1);
			buf[1] = 0;
			TRACE_MEMORY("\r\n-%x[%s %x>%d,%d]", ptr, buf, lr, PMAIN_TASK->aloc_ptrs, (PMAIN_TASK->aloc_mem<<2));
			trace_sleep();
		}
	}
}
void* tsk_realloc(void* ptr, unsigned int size)
{
	register unsigned int _lr asm("r14");
	int buf[2];
	unsigned int lr = _lr;
	void* ptr1;

//	tsk_sleep(1);

	is_dynamic_mem(ptr);
	ptr1 = CURRENT_TASK->name;
	*buf = *((int*)ptr1);
	buf[1] = 0;

	if(ptr)
	{
		if((char*)ptr > &end)
		{
			TRACE_MEMORY("\r\n-%x[%x<]", ptr, lr);
		}
	}
	ptr = usr_realloc(ptr, size);
	if(ptr)
	{
		TRACE_MEMORY("\r\n+%x[%s %x<%d,%d]", ptr, buf, lr, PMAIN_TASK->aloc_ptrs, (PMAIN_TASK->aloc_mem<<2));
	} else
	{
		if(size)
		{
			TRACE("\r\nERROR:[%s=%d]", buf, size);
		} else
		{
			TRACE_MEMORY("\r\n+%x[%s %x=%d,%d]", ptr, buf, lr, PMAIN_TASK->aloc_ptrs, (PMAIN_TASK->aloc_mem<<2));
		}
	}
	trace_sleep();
	return ptr;
}

#else
void* tsk_malloc_clear(unsigned int size)
{
	void * ptr;
	ptr = tsk_malloc(size);
	if(ptr)
		memclr(ptr, size);
	return (ptr);
}

#endif


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 					 helper functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int atoi(const char *ptr )
{
	int res=0;
	if(ptr)
		tmos_sscanf(ptr, "%d", &res);
	return (res);
}

#if USE_TMOS_LLONG
long long atoll(const char *ptr)
{
	long long res=0;
	if(ptr)
		tmos_sscanf(ptr, "%ld", &res);
	return (res);
}
#endif

unsigned int ahextoi(const char *ptr )
{
	unsigned int res=0;
	tmos_sscanf(ptr, "%x", &res);
	return (res);
}

unsigned int find_in_list(const char* str, STR_LIST sl, unsigned int* dwRead)
{
	unsigned int pos;
	unsigned int index=1;


	while(*sl)
	{
		pos=0;
		while(1)
		{
			if(!sl[pos])
			{
				if(dwRead)
					*dwRead += pos;
				return (index);
			}
			if(sl[pos] != str[pos])
				break;
			pos++;
		}
		while(sl[pos])
			pos++;
		sl+= pos+1;
		index++;
	}
	return (0);
}

const char* szlist_at(STR_LIST sl, unsigned int pos)
{
	while(*sl && pos--)
	{
		while(*sl++);
	}
	return sl;
}
