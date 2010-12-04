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

int  locked_inc_byte(volatile unsigned char* Addend);
int  locked_add_byte(volatile unsigned char* Addend, unsigned int);
int  locked_dec_byte(volatile unsigned char* Addend);
int  locked_inc_short(volatile short* Addend);
int  locked_add_short(volatile short* Addend, int);
int  locked_dec_short(volatile short* Addend);
int  locked_add_int(volatile int* Addend, int);
int  locked_dec_int(volatile int* Addend);
int  locked_set_byte(volatile unsigned char* var, unsigned int flags);
int  locked_clr_byte(volatile unsigned char* var, unsigned int flags);

#ifdef __cplusplus
}
#endif

 #endif /* SERVICES_H_ */
