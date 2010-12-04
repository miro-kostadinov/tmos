/**************************************************************************//**
 * @ingroup	 boards_ron200
 * @file
 * @brief    SWI TABLE
 *
 */


#include <tmos.h>

void sys_kernel_init( void);
void sys_sleep(void);
void sys_sleep_rel(void);
void sys_sleep_until(void);
void sys_test_signal(void);
void sys_get_signal(void);
void sys_wait_signal(void);
void sys_resume_wait_signal(void);
void sys_drv_service_wait(void);
void sys_malloc(void);
void sys_realloc(void);

//void svc_HND_SET_STATUS(HANDLE hnd, RES_CODE result);
//RES_CODE drv_hcontrol(HANDLE hnd, unsigned int reason);
//RES_CODE drv_service(HANDLE hnd);
void svc_HND_SET_STATUS(void);
void drv_hcontrol(void);
void drv_service(void);
void svc_drv_service(void);
void svc_drv_hcontrol(void);

//initialization using Designated Initializers (not supported in C++)

SWI_FUNC const SWI_TABLE[] __attribute__ ((section (".SWIHandler")))  =
{
	sys_kernel_init,				//SWI 0 must be kernel init

#ifdef usr_task_suspend_swi
	[usr_task_suspend_swi] 			= (SWI_FUNC)svc_task_suspend,
#endif

#ifdef usr_send_signal_swi
	[usr_send_signal_swi] 			= (SWI_FUNC)svc_send_signal,
#endif

#ifdef usr_task_schedule_swi
	[usr_task_schedule_swi] 		= (SWI_FUNC)svc_task_schedule,
#endif

#ifdef tsk_sleep_swi
	[tsk_sleep_swi]					= sys_sleep,
#endif

#ifdef tsk_sleep_rel_swi
	[tsk_sleep_rel_swi]				= sys_sleep_rel,
#endif

#ifdef tsk_sleep_until_swi
	[tsk_sleep_until_swi]			= sys_sleep_until,
#endif

#ifdef tsk_try_signal_swi
	[tsk_try_signal_swi]			= sys_test_signal,
#endif

#ifdef tsk_get_signal_swi
	[tsk_get_signal_swi]			= sys_get_signal,
#endif

#ifdef tsk_wait_signal_swi
	[tsk_wait_signal_swi]			= sys_wait_signal,
#endif

#ifdef tsk_resume_wait_signal_swi
	[tsk_resume_wait_signal_swi]	= sys_resume_wait_signal,
#endif

#ifdef usr_drv_hcontrol_swi
	[usr_drv_hcontrol_swi]			= svc_drv_hcontrol,
#endif

#ifdef usr_drv_icontrol_swi
	[usr_drv_icontrol_swi]			= (SWI_FUNC)svc_drv_icontrol,
#endif

#ifdef tsk_start_handle_swi
	[tsk_start_handle_swi]			= svc_drv_service,
#endif

#ifdef usr_HND_SET_STATUS_swi
	[usr_HND_SET_STATUS_swi]		= svc_HND_SET_STATUS,
#endif

#ifdef tsk_start_and_wait_swi
	[tsk_start_and_wait_swi]		= sys_drv_service_wait,
#endif

#ifdef malloc_swi
	[malloc_swi]					= sys_malloc,
#endif

#ifdef free_swi
	[free_swi]						= (SWI_FUNC)svc_free,
#endif

#ifdef realloc_swi
	[realloc_swi]					= sys_realloc,
#endif

	NULL

};
