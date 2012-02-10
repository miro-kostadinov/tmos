/*
 * CSocket.h
 *
 *  Created on: Jan 31, 2012
 *      Author: Miroslav Kostadinov
 */

#ifndef CSOCKET_H_
#define CSOCKET_H_

#include <tmos.h>

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

/**
 * CSocket class
 *
 */
struct CSocket: public CHandle
{
	RES_CODE bind(unsigned int ip_adr, unsigned int port);
	RES_CODE bind(const char* url);
	RES_CODE listen(int backlog);
	RES_CODE connect(unsigned int ip_adr, unsigned int port);
	RES_CODE connect(const char* url);
	CSocket* accept(unsigned int timeout = 1024*1024);

	RES_CODE close();
};

CSocket* socket(int domain, int type, int protocol);

#endif /* CSOCKET_H_ */
