/*
 * services.h
 *
 *  Created on: 2010-3-16
 *      Author: Miroslav Kostadinov
 */

#ifndef SERVICES_H_
#define SERVICES_H_

#ifdef __cplusplus
 extern "C" {
#endif



void svc_send_signal(Task * task, unsigned int signal);
void svc_task_schedule(Task* volatile task);

void svc_task_suspend(Task *task);
void svc_drv_icontrol(DRIVER_INDEX drv_index, unsigned int reason, void * param);

void usr_trace_str(const char *buf);
void usr_trace_char(unsigned c);
void TRACE_BUF(const void* buf, unsigned int len);

int  locked_inc_byte(volatile unsigned char* Addend);
int  locked_add_byte(volatile unsigned char* Addend, unsigned int);
int  locked_dec_byte(volatile unsigned char* Addend);
int  locked_inc_short(volatile short* Addend);
int  locked_add_short(volatile short* Addend, int);
int  locked_dec_short(volatile short* Addend);
int  locked_inc_int(volatile int* Addend);
int  locked_add_int(volatile int* Addend, int);
int  locked_dec_int(volatile int* Addend);
int  locked_set_byte(volatile unsigned char* var, unsigned int flags);
int  locked_clr_byte(volatile unsigned char* var, unsigned int flags);
unsigned int  locked_set_int(volatile unsigned int* var, unsigned int flags);
unsigned int  locked_clr_int(volatile unsigned int* var, unsigned int flags);
void* locked_set_if_null(void* pointer, void* value);

static inline unsigned int locked_get_reg(volatile unsigned int* addr)
{
	unsigned int result;

	asm volatile ("ldrex %0, [%1]" : "=r" (result) : "r" (addr) );
	return (result);
}

static inline unsigned int locked_set_reg(volatile unsigned int* addr, unsigned int val)
{
	unsigned int result;

	asm volatile ("strex %0, %2, [%1]" : "=r" (result) : "r" (addr), "r" (val) );
	return (result);
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
void TRACE_BUF(const void* buf, unsigned int len, unsigned int color);
void TRACE_BUF(const void* buf, unsigned int len, unsigned int c1, unsigned int c2);
void TRACE_TEXT(const void* buf, unsigned int len);
void TRACE_TEXT(const void* buf, unsigned int len, unsigned int color);
void TRACE_COLOR(unsigned int color);
void TRACE_COLOR_END();

struct lock_t
{
private:
	uint32_t lock_locker;
	uint32_t lock_cnt;

public:
	lock_t(): lock_locker(0), lock_cnt(0)
	{;}

	void lock()
	{
		while(locked_set_if_null(&lock_locker, CURRENT_TASK))
		{
			if(lock_locker == (uint32_t)CURRENT_TASK)
				break;
			tsk_sleep(1);
		}
		lock_cnt++;
	}

	bool try_lock()
	{
		if(locked_set_if_null(&lock_locker, CURRENT_TASK))
		{
			if(lock_locker != (uint32_t)CURRENT_TASK)
				return false;
		}
		lock_cnt++;
		return true;
	}

	void unlock()
	{
		if(lock_locker == (uint32_t)CURRENT_TASK)
		{
			if(lock_cnt)
				lock_cnt--;
			if(!lock_cnt)
				lock_locker = 0;
		}
	}

	uint32_t is_locked()
	{
		return lock_locker;
	}

	uint32_t get_lock_cnt()
	{
		return lock_cnt;
	}


};

#endif

#define TC_BRIGHT				1
#define TC_FAINT				2
#define TC_Italic				3
#define TC_Underline 			4
#define TC_Blink_Slow 			5
#define TC_Blink_Rapid 			6
#define TC_negative 			7
#define TC_Conceal 				8
#define TC_Crossed 				9
#define TC_font0				10
#define TC_font1				11
#define TC_font2				12
#define TC_font3				13
#define TC_font4				14
#define TC_font5				15
#define TC_font6				16
#define TC_font7				17
#define TC_font8				18
#define TC_font9				19
#define TC_Fraktur 				20
#define TC_Bright_off 			21
#define TC_Normal 				22
#define TC_italic_Fraktur_off 	23
#define TC_Underline_off 		24
#define TC_Blink_off 			25
#define TC_POSITIVE 			27
#define TC_CONCEAL_OFF 			28
#define TC_CROSSED_OFF 			29
#define TC_TXT_BLACK	 		30
#define TC_TXT_RED		 		31
#define TC_TXT_GREEN	 		32
#define TC_TXT_YELLOW	 		33
#define TC_TXT_BLUE		 		34
#define TC_TXT_MAGENTA	 		35
#define TC_TXT_CYAN		 		36
#define TC_TXT_WHITE	 		37
#define TC_TXT_XTERM 			38	//Set xterm-256 text color[dubious – discuss]	next arguments are 5;x where x is color index (0..255)
#define TC_TXT_DEFAULT 			39
#define TC_BG_BLACK	 			40
#define TC_BG_RED		 		41
#define TC_BG_GREEN	 			42
#define TC_BG_YELLOW	 		43
#define TC_BG_BLUE		 		44
#define TC_BG_MAGENTA	 		45
#define TC_BG_CYAN		 		46
#define TC_BG_WHITE	 			47
#define TC_BG_XTERM  			48	//Set xterm-256 background color	next arguments are 5;x where x is color index (0..255)
#define TC_BG_DEFAULT 			49
#define TC_FRAMED 				51
#define TC_ENCIRCLED 			52
#define TC_OVERLINED 			53
#define TC_FRAMED_ENCIRCLED_OFF 54
#define TC_OVERLINE_OFF 		55

#endif /* SERVICES_H_ */
