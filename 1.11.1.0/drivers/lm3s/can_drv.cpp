/*
 * can_drv.cpp
 *
 *  Created on: 06.03.2011
 *      Author: Miroslav Kostadinov
 */


#include <tmos.h>
#include <can_drv.h>

//*----------------------------------------------------------------------------
//*			Portable
//*----------------------------------------------------------------------------
#define CAN_TASK_STACK_SIZE 100

void can_thread(CAN_INFO drv_info)
{
	CHandle helper;

	helper.tsk_safe_open(drv_info->info.drv_index, NULL);


}
//*----------------------------------------------------------------------------
//*			DCR function
//*----------------------------------------------------------------------------
void CAN_DCR(CAN_INFO drv_info, unsigned int reason, HANDLE param)
{
	CAN_DRIVER_DATA*  drv_data;

	drv_data = drv_info->drv_data;

	switch(reason)
    {
        case DCR_RESET:
        	//Initialize the driver here
            Task* task;

            task = usr_task_create_dynamic("CANT", (TASK_FUNCTION) can_thread, 90,
				CAN_TASK_STACK_SIZE);
			if (task)
			{
				svc_task_schedule(task);
				task->sp->r0.as_cvoidptr = drv_info;
			}
            break;

	    case DCR_OPEN:
	    	param->res = RES_OK;
	    	break;


	    case DCR_CANCEL:
	    	if(param->mode.as_int)
	    	{
		    	param->svc_list_cancel(drv_data->waiting);
	    	} else
	    	{
		    	param->svc_list_cancel(drv_data->helper);
	    	}
	    	break;

    }
}

//*----------------------------------------------------------------------------
//*			DSR function
//*----------------------------------------------------------------------------
void CAN_DSR(CAN_INFO drv_info, HANDLE hnd)
{
	CAN_DRIVER_DATA* drv_data = drv_info->drv_data;
	HANDLE tmp;

	if(hnd->mode.as_int)
	{
		// this is a CAN handle...
		hnd->next = NULL;
		if( (tmp=drv_data->helper) )
		{
			//the helper task is waiting for object...
			hnd->res = RES_BUSY;
			drv_data->helper = NULL;
			tmp->dst.as_voidptr = hnd;
			svc_HND_SET_STATUS(tmp, RES_SIG_OK);
		} else
		{
			//queue the client handle while the helper task is busy
			hnd->list_add(drv_data->waiting);
		}
	} else
	{
		// this must be the helper
		if( (tmp=drv_data->waiting) )
		{
			drv_data->waiting = tmp->next;
			hnd->dst.as_voidptr = tmp;
			svc_HND_SET_STATUS(hnd, RES_SIG_OK);
		} else
		{
			hnd->res = RES_BUSY;
			drv_data->helper = hnd;
		}
	}
}

//*----------------------------------------------------------------------------
//*			ISR function
//*----------------------------------------------------------------------------
void CAN_ISR(CAN_INFO drv_info )
{

}
