/*
 * handle.cpp
 *
 *  Created on: 2010-4-9
 *      Author: stanly
 */

#include <drivers.h>

//*----------------------------------------------------------------------------
//*			Unsafe (this=NULL) methods
//*----------------------------------------------------------------------------


//----------------------    ctors etc   ---------------------------//
/**
 * CHandle constructor
 */
CHandle::CHandle(void)
{
	if(this)
	{
		drv_index = INALID_DRV_INDX;
		res = RES_CLOSED;
	}
}

/**
 * CCHandle destructor
 *
 */
CHandle::~CHandle()
{
	close();
}


//----------------------    common      ---------------------------//
/**
 * Close the handle
 */
void CHandle::close()
{
	if(this)
	{
		if(res < RES_CLOSED)
		{
			if( client.drv_index > INALID_DRV_INDX && !(client.drv_index & 3))
			{
				//this is a task handle...

				//1. Cancel any operation in progress..
				tsk_cancel();

				//2. Close
		        tsk_hcontrol(DCR_CLOSE);

		        //3. Deallocate the signal
				CURRENT_TASK->aloc_sig &= ~signal;

				drv_index = INALID_DRV_INDX;
				res = RES_CLOSED;
			}
		}
	}
}

/**
 * Open
 * @param index
 * @param m
 * @return
 */
bool CHandle::tsk_open(DRIVER_INDEX index, const void * m)
{
	if(this)
	{
		unsigned int sig;

	    *(unsigned int *)(void*)&drv_index = index;
	    mode.as_voidptr = (void*)m;
	    client.task = CURRENT_TASK;

	    sig = CURRENT_TASK->aloc_sig;
	    sig = (sig+1) & ~sig;
	    sig &= 255;
	    if( (signal = sig) )
	    {
		 	res = RES_CLOSED;
		 	tsk_hcontrol(DCR_OPEN);

			if(res == RES_OK)
			{
		    	CURRENT_TASK->aloc_sig |= signal;
				return true;
			}
			TRACELN_ERROR("Handle open err INDEX:%u", drv_index);
	    } else
	    {
	    	TRACELN1_ERROR("\r\nNo free signals for task ");
	    	TRACE1_ERROR((char*)CURRENT_TASK->name);
	    }
		drv_index = INALID_DRV_INDX;
	} else
		TRACELN_ERROR("Memory err INDEX:%u", index);
	return false;
}

/**
 * try to open in endless loop
 * @param index
 * @param m
 */
void CHandle::tsk_safe_open(DRIVER_INDEX index, const void * m)
{
	while(!tsk_open(index, m))
	{
		tsk_sleep(10);
	}
}

//*----------------------------------------------------------------------------
//*			Safe (this!=NULL) methods
//*		Call these methods only after successful open()
//*----------------------------------------------------------------------------

//----------------------    common      ---------------------------//

//----------------------   task related ---------------------------//
/**
 * Cancel the current operation (if any)
 */
void CHandle::tsk_cancel()
{
	if (res & FLG_BUSY)
	{
        tsk_hcontrol(DCR_CANCEL);
        tsk_get_signal(signal);
        res &= ~FLG_SIGNALED;
	} else
		if(res & FLG_SIGNALED)
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
		}
}




/**
 * Start read operation
 * @param buf
 * @param l
 * @return
 */
bool CHandle::tsk_start_read(void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & FLG_SIGNALED)
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}
		if(res & FLG_BUSY)
		{
			tsk_cancel();
	        continue;
		}

		//handle is closed or with error
		return false;
	}
	//handle is idle and open
	len = l;
	set_res_cmd(CMD_READ);
	dst.as_voidptr = buf;
	tsk_start_handle();
	return true;
}

/**
 * Blocking read
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_read(void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_READ);
	dst.as_voidptr = buf;
	return tsk_start_and_wait();
}

/**
 * Time limited read
 * @param buf
 * @param l
 * @param time
 * @return
 */
RES_CODE CHandle::tsk_read(void * buf, unsigned int l, unsigned int time)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_READ);
	dst.as_voidptr = buf;
    tsk_start_handle();
    if(tsk_wait_signal(signal, time))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
   return res;
}

/**
 * Blocking locked read
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_read_locked(void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd((CMD_READ|FLAG_LOCK));
	dst.as_voidptr = buf;
	return tsk_start_and_wait();
}

/**
 * Time limited locked read
 * @param buf
 * @param l
 * @param time
 * @return
 */
RES_CODE CHandle::tsk_read_locked(void * buf, unsigned int l, unsigned int time)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd((CMD_READ|FLAG_LOCK));
	dst.as_voidptr = buf;
    tsk_start_handle();
    if(tsk_wait_signal(signal, time))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
    return res;
}

/**
 * Resume read operation
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_resume_read(void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_READ);
	dst.as_voidptr = buf;
    tsk_start_handle();
    if(tsk_resume_wait_signal(signal))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
    return res;
}

/**
 * Start write
 * @param buf
 * @param l
 * @return
 */
bool CHandle::tsk_start_write(const void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & FLG_SIGNALED)
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}
		if(res & FLG_BUSY)
		{
			tsk_cancel();
	        continue;
		}

		//handle is closed or with error
		return false;
	}
	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE);
	src.as_voidptr = (void*)buf;
	tsk_start_handle();
	return true;
}

/**
 * Blocking write
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_write(const void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE);
	src.as_cvoidptr = buf;
	return tsk_start_and_wait();
}

/**
 * Time limited write
 * @param buf
 * @param l
 * @param time
 * @return
 */
RES_CODE CHandle::tsk_write(const void * buf, unsigned int l, unsigned int time)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE);
	src.as_voidptr = (void*)buf;
    tsk_start_handle();
    if(tsk_wait_signal(signal, time))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
    return res;
}

/**
 * Blocking locked write
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_write_locked(const void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE|FLAG_LOCK);
	src.as_voidptr = (void*)buf;
	return tsk_start_and_wait();
}

/**
 * Time limited locked write
 * @param buf
 * @param l
 * @param time
 * @return
 */
RES_CODE CHandle::tsk_write_locked(const void * buf, unsigned int l, unsigned int time)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE|FLAG_LOCK);
	src.as_voidptr = (void*)buf;
    tsk_start_handle();
    if(tsk_wait_signal(signal, time))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
    return res;
}

/**
 * Resume write operation
 * @param buf
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_resume_write(const void * buf, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_WRITE);
	src.as_voidptr = (void*)buf;
    tsk_start_handle();
    if(tsk_resume_wait_signal(signal))
        res &= ~FLG_SIGNALED;
    else
    	tsk_cancel();
    return res;
}

/**
 * Read & write (swap) operation
 * @param d
 * @param s
 * @param l
 * @return
 */
RES_CODE CHandle::tsk_read_write(void *d, const void *s, unsigned int l)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	len = l;
	set_res_cmd(CMD_READ_WRITE);
	dst.as_voidptr = d;
	src.as_voidptr = (void*)s;
	return tsk_start_and_wait();
}

/**
 * Blocking command
 * @param c
 * @param ptr
 * @return
 */
RES_CODE CHandle::tsk_command(void * c, void *ptr)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	set_res_cmd(CMD_COMMAND);
	dst.as_voidptr = ptr;
	src.as_voidptr = c;
	return tsk_start_and_wait();
}

/**
 * Locked command
 * @param c
 * @param ptr
 * @return
 */
RES_CODE CHandle::tsk_command_locked(void * c, void *ptr)
{
	while (res >= FLG_SIGNALED)
	{
		if(res & (FLG_SIGNALED | FLG_BUSY) )
		{
			tsk_get_signal(signal);
	        res &= ~FLG_SIGNALED;
	        continue;
		}

		//handle is closed or with error
		return res;
	}

	//handle is idle and open
	set_res_cmd(CMD_COMMAND|FLAG_LOCK);
	dst.as_voidptr = ptr;
	src.as_voidptr = c;
	return tsk_start_and_wait();
}

/**
 * Driver helper function
 * @param base
 * @return
 */
bool CHandle::svc_list_cancel(HANDLE& base)
{
	if(list_remove(base))
	{
		svc_HND_SET_STATUS(this, RES_SIG_IDLE);
		return true;
	}
	return false;
/*
	if(base && base == this )
	{
		base = next;
		svc_HND_SET_STATUS(this, RES_SIG_IDLE);
		return true;
	}
	HANDLE ptr = base;
	while(ptr && ptr->next)
	{
		if(ptr->next == this )
		{
			ptr->next = next;
			svc_HND_SET_STATUS(this, RES_SIG_IDLE);
			return true;
		}
		ptr = ptr->next;
	}
	return false;
*/
}

/**
 * Driver helper function
 * @param base
 * @return
 */
bool CHandle::list_remove(HANDLE& base)
{
	if(base && base == this )
	{
		base = next;
		return true;
	}
	HANDLE ptr = base;
	while(ptr && ptr->next)
	{
		if(ptr->next == this )
		{
			ptr->next = next;
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

/**
 * Driver helper function
 * @param base
 */
void CHandle::list_add(HANDLE& base)
{
	HANDLE ptr = base;

	next = 0;
	res = RES_BUSY;
	if(!ptr )
	{
		base = this;
	}
	else
	{
		while(ptr->next)
			ptr=ptr->next;
		ptr->next = this;
	}
}

/**
 * Driver helper function
 */
void CHandle::usr_hnd_signal()
{
    if(client.drv_index > INALID_DRV_INDX)
    {
    	if(client.drv_index & 3)
    	{
    		//the client is a callback
    		client.callback(this, res);
    	} else
    	{
    		//the client is a task
    		usr_send_signal(client.task, signal);
    	}
    } else
    {
        if(client.drv_index < INALID_DRV_INDX)
        	usr_drv_icontrol((DRIVER_INDEX)client.drv_index, signal, this);
    }
}



//*----------------------------------------------------------------------------
//*			Helper functions
//*----------------------------------------------------------------------------
HANDLE tsk_open_new_hnd(DRIVER_INDEX index, const void * mode )
{
	HANDLE hnd;

	if( (hnd = new CHandle()) )
	{
		if(!hnd->tsk_open(index, mode))
		{
			delete hnd;
			hnd = NULL;
		}
	}

	return hnd;
}

HANDLE tsk_safe_open_new_hnd(DRIVER_INDEX index, const void * mode )
{
	CHandle *hnd;

	while(1)
	{
		if( (hnd = new CHandle()) )
		{
			if(hnd->tsk_open(index, mode))
				return hnd;

			delete hnd;
			tsk_sleep(10);
		}
	}
}



