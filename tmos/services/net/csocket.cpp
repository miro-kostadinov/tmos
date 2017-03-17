/*
 * CSocket.cpp
 *
 *  Created on: Jan 31, 2012
 *      Author: Miroslav Kostadinov
 */

#include <tmos.h>
#include "csocket.h"

NET_CODE CSocket::open(const sock_mode_t* smode)
{
	if(!IS_NOT_NULL(this))
		return NET_ERR_OUT_OF_MEMORY;

	tsk_open(smode->driver, smode);
	if(RES_OK == res)
	{
		set_res_cmd(SOCK_CMD_OPEN);
		tsk_start_and_wait();
		if(res == RES_OK)
		{
			return NET_OK;
		} else
		{
			CHandle::close();
		}
	}
	else
		error = NET_ERR_HANDLE_OPEN;
	return error;
}

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
	NET_CODE result = NET_IDLE;
	if(complete())
	{
		CURL link;

		result = link.url_parse(url);
		if(result == RES_OK)
		{
			src.as_ccharptr = link.host.c_str();
			dst.as_int = link.port;
			set_res_cmd(SOCK_CMD_BIND_URL);
			tsk_start_and_wait();
			if(res == RES_OK)
				result = NET_OK;
			else
				result = error;
		}
	}
	return (result);
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
NET_CODE CSocket::connect(const char* ip_adr, unsigned int port)
{
	NET_CODE result = NET_IDLE;

	if(complete())
	{
		src.as_charptr = (char*) ip_adr;
		dst.as_int = port;
		set_res_cmd(SOCK_CMD_CONNECT_ADR);
		tsk_start_and_wait();
		if(res == RES_OK)
			result = NET_OK;
		else
			result = error;
	}
	return (result);
}

/**
 * Initiate a connection to a remote port and address with timeout
 * @param ip_adr
 * @param port
 * @param timeout
 * @return
 */
NET_CODE CSocket::connect(const char* ip_adr, unsigned int port, unsigned int timeout)
{
	NET_CODE result = NET_IDLE;

	if(complete())
	{
		src.as_charptr = (char*) ip_adr;
		dst.as_int = port;
		set_res_cmd(SOCK_CMD_CONNECT_ADR);
		if(timeout)
		{
		    tsk_start_handle();
		    if(tsk_wait_signal(signal, timeout))
		    {
		        res &= ~FLG_SIGNALED;
				if(res == RES_OK)
					result = NET_OK;
				else
					result = error;
		    }
		    else
		    	tsk_cancel();
		}
		else
		{
			tsk_start_and_wait();
			if(res == RES_OK)
				result = NET_OK;
			else
				result = error;
		}
	}
	return (result);
}

/**
 * Initiate a connection to a remote link.
 * @param link
 * @return
 */
NET_CODE CSocket::connect(const CURL& link)
{
	NET_CODE result = NET_IDLE;

	if(complete())
	{
		src.as_ccharptr = link.host.c_str();
		dst.as_int = link.port;
		set_res_cmd(SOCK_CMD_CONNECT_URL);
		tsk_start_and_wait();
		if(res == RES_OK)
			result = NET_OK;
		else
			result = error;
	}
	return (result);
}

/**
 * Initiate a connection to a remote url.
 * @param url
 * @return
 */
NET_CODE CSocket::connect(const char* url)
{
	NET_CODE result = NET_IDLE;

	if(complete())
	{
		CURL link;

		result = link.url_parse(url);
		if(result == RES_OK)
		{
			src.as_ccharptr = link.host.c_str();
			dst.as_int = link.port;
			set_res_cmd(SOCK_CMD_CONNECT_URL);
			tsk_start_and_wait();
			if(res == RES_OK)
				result = NET_OK;
			else
				result = error;
		}
	}
	return (result);
}

/**
 * Disconnect a connection from a remote url.
 * @return
 */
NET_CODE CSocket::disconnect(void)
{
	NET_CODE result = NET_IDLE;

	if(complete())
	{
		set_res_cmd(SOCK_CMD_DISCONNECT);
		tsk_start_and_wait();
		if(res == RES_OK)
			result = NET_OK;
		else
			result = error;
	}
	return (result);
}

/**
 * Accept a connection request.
 * @param timeout
 * @return
 */
NET_CODE CSocket::accept(CSocket* new_sock, unsigned int timeout)
{
	if(complete())
	{
		dst.as_voidptr = new_sock;
		set_res_cmd(SOCK_CMD_ACCEPT);
	    tsk_start_handle();
	    if(tsk_wait_signal(signal, timeout))
	    {
	        res &= ~FLG_SIGNALED;
	    }
	    else
	    	tsk_cancel();

	}
	return (res);
}

/**
 * 	“One-way” close of a socket
 * @return
 */
RES_CODE CSocket::close()
{
	if(res < RES_CLOSED)
	{
		if(complete())
		{
			set_res_cmd(SOCK_CMD_CLOSE);
			tsk_start_and_wait();
		}
		CHandle::close();
	}
	return (res);
}

RES_CODE CSocket::gethostbyname(CSTRING& ip_adr, const char* url)
{
	if(complete())
	{
		src.as_charptr = (char*) url;
		dst.as_voidptr = &ip_adr;
		set_res_cmd(SOCK_CMD_GET_HOST);
		tsk_start_and_wait();
	}
	return (res);
}

RES_CODE CSocket::get_addr(unsigned int& ip_adr, unsigned short& port, int local)
{
	if(complete())
	{
		src.as_intptr = &ip_adr;
		dst.as_shortptr = &port;
		len = local;
		set_res_cmd(SOCK_CMD_GET_ADDR);
		tsk_start_and_wait();
	}
	return (res);
}
