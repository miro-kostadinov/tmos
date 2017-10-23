/*
 * csocket.h
 *
 *  Created on: Jan 31, 2012
 *      Author: Miroslav Kostadinov
 */

#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <tmos.h>
#include "url.h"

/// Socket commands
/// must be implemented from each socket compatible driver
#define SOCK_CMD_OPEN			(( 1 <<4)+CMD_COMMAND)
#define SOCK_CMD_BIND_ADR		(( 2 <<4)+CMD_COMMAND)
#define SOCK_CMD_BIND_URL		(( 3 <<4)+CMD_COMMAND)
#define SOCK_CMD_LISTEN			(( 4 <<4)+CMD_COMMAND)
#define SOCK_CMD_CONNECT_ADR	(( 5 <<4)+CMD_COMMAND)
#define SOCK_CMD_CONNECT_URL	(( 6 <<4)+CMD_COMMAND)
#define SOCK_CMD_ACCEPT			(( 7 <<4)+CMD_COMMAND)
#define SOCK_CMD_CLOSE			(( 8 <<4)+CMD_COMMAND)
#define SOCK_CMD_GET_HOST		(( 9 <<4)+CMD_COMMAND)
#define SOCK_CMD_GET_ADDR		((10 <<4)+CMD_COMMAND)
#define SOCK_CMD_DISCONNECT		((11 <<4)+CMD_COMMAND)

#define SOCKET_CLOSED		0
#define SOCKET_OPEN			1
#define SOCKET_CONECTED		(2 + SOCKET_OPEN)
#define SOCKET_WRITE_FLAG	4
#define SOCKET_LISTEN	    (8 + SOCKET_OPEN)

#define SOCKET_ID_INVALID	0xffff

#define IP_SOCKET_TCP			0
#define IP_SOCKET_UDP			1
#define BT_SOCKET				2
#define REDIRECTED_SOCKET		4

struct sock_mode_t
{
	DRIVER_INDEX	driver;			//!< driver to be used for this socket
	unsigned char 	interface;		//!< driver interface index (e.g. apn index)
	unsigned short 	port;			//!< optional port (e.g. for UDP)
	unsigned char	sock_type;		//!< IP_SOCKET_TCP or IP_SOCKET_UDP
};

struct redir_sock_mode_t
{
	sock_mode_t mode;
	const sock_mode_t*	redir_mode;
};

/**
 * CSocket class
 *
 */
struct CSocket: public CHandle
{
	unsigned char sock_state;
	unsigned char file_mode;
	unsigned short sock_id;

	CSocket() :
			CHandle(),
			sock_state(SOCKET_CLOSED),
			file_mode(0),
			sock_id(SOCKET_ID_INVALID)
		{};
	~CSocket() { close(); };

	NET_CODE open(const sock_mode_t* smode);
	RES_CODE bind(unsigned int ip_adr, unsigned int port);
	RES_CODE bind(const char* url);
	RES_CODE listen(int backlog);
	NET_CODE connect(const char* ip_adr, unsigned int port);
	NET_CODE connect(const char* ip_adr, unsigned int port, unsigned int timeout);
	NET_CODE connect(const CURL& link);
	NET_CODE connect(const char* url);
	RES_CODE disconnect();
	NET_CODE accept(CSocket* new_sock, unsigned int timeout = 1024*1024);
	RES_CODE gethostbyname(CSTRING& ip_adr, const char* url);
	RES_CODE get_addr(unsigned int& ip_adr, unsigned short& port, int local);
	RES_CODE get_addr(unsigned int& ip_adr, unsigned short& port, int local, unsigned int timeout);
	RES_CODE close();
};

CSocket* socket(int domain, int type, int protocol);

#endif /* CSOCKET_H_ */
