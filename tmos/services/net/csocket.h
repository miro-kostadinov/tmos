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

#define SOCKET_CLOSED		0
#define SOCKET_OPEN			1
#define SOCKET_CONECTED		3

#define SOCKET_ID_INVALID	0xffff

struct sock_mode
{
	DRIVER_INDEX	driver;
};

struct apn_stru
{
	CSTRING name;
	CSTRING user;
	CSTRING pass;
};

struct sock_gprs_mode: public sock_mode
{
	apn_stru* apn;
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

	NET_CODE open(const sock_mode* smode);
	RES_CODE bind(unsigned int ip_adr, unsigned int port);
	RES_CODE bind(const char* url);
	RES_CODE listen(int backlog);
	NET_CODE connect(const char* ip_adr, unsigned int port);
	NET_CODE connect(CURL& link);
	NET_CODE connect(const char* url);
	CSocket* accept(unsigned int timeout = 1024*1024);
	RES_CODE gethostbyname(CSTRING& ip_adr, const char* url);
	RES_CODE close();
};

CSocket* socket(int domain, int type, int protocol);

#endif /* CSOCKET_H_ */
