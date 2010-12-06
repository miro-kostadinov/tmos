#include <tmos.h>


int main(void);
TASK_DECLARE_STATIC(main_task, "MAIN", (void (*)(void))main, 0, 20+TRACE_SIZE);



int main(void)
{


	for(;;)
	{
		//SCB->SysCtlDeepSleep();
		//__WFI();
	}
}
