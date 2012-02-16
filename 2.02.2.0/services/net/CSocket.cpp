/*
 * CSocket.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include "CSocket.h"

/**
 * Associate a socket with a port and address.
 * @param ip_adr
 * @param port
 * @return RES_OK on success
 */
RES_CODE CSocket::bind(unsigned int ip_adr, unsigned int port)
{
	if(complete())
	{
		src.as_int = ip_adr;
		dst.as_int = port;
		set_res_cmd(SOCK_CMD_BIND_ADR);
		tsk_start_and_wait();
	}
	return (res);
}

/**
 * Associate a socket with an URL
 * @param url
 * @return RES_OK on success
 */
RES_CODE CSocket::bind(const char* url)
{
	if(complete())
	{
		src.as_charptr = (char*)url;
		set_res_cmd(SOCK_CMD_BIND_URL);
		tsk_start_and_wait();
	}
	return (res);
}

/**
 * 	Establish queue for connection requests.
 * @param backlog
 * @return
 */
RES_CODE CSocket::listen(int backlog)
{
	if(complete())
	{
		src.as_int = backlog;
		set_res_cmd(SOCK_CMD_LISTEN);
		tsk_start_and_wait();
	}
	return (res);
}

/**
 * Initiate a connection to a remote port and address.
 * @param ip_adr
 * @param port
 * @return
 */
RES_CODE CSocket::connect(unsigned int ip_adr, unsigned int port)
{
	if(complete())
	{
		src.as_int = ip_adr;
		dst.as_int = port;
		set_res_cmd(SOCK_CMD_BIND_ADR);
		tsk_start_and_wait();
	}
	return (res);
}

/**
 * Initiate a connection to a remote host.
 * @param url
 * @return
 */
RES_CODE CSocket::connect(const char* url)
{
	if(complete())
	{
		src.as_charptr = (char*) url;
		set_res_cmd(SOCK_CMD_BIND_URL);
		tsk_start_and_wait();
	}
	return (res);
}

/**
 * Accept a connection request.
 * @param timeout
 * @return
 */
CSocket* CSocket::accept(unsigned int timeout)
{
	if(complete())
	{
		dst.as_voidptr = NULL;
		set_res_cmd(SOCK_CMD_ACCEPT);
	    tsk_start_handle();
	    if(tsk_wait_signal(signal, timeout))
	    {
	        res &= ~FLG_SIGNALED;
	    }
	    else
	    	tsk_cancel();

        return (CSocket*)dst.as_voidptr;
	}
	return (NULL);
}

/**
 * 	“One-way” close of a socket
 * @return
 */
RES_CODE CSocket::close()
{
	if(complete())
	{
		set_res_cmd(SOCK_CMD_CLOSE);
		tsk_start_and_wait();
	}
	CHandle::close();
	return (res);
}


