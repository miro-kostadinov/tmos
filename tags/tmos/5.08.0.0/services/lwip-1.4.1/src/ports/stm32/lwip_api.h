/*
 * lwip_api.h
 *
 *  Created on: 29.04.2011
 *      Author: Miroslav Kostadinov
 */

#ifndef LWIP_API_H_
#define LWIP_API_H_

#include <tmos.h>
#include <csocket.h>

struct lwip_mode_t
{
	sock_mode_t mode;
	uint8_t	 lwip_priority;
};

#define LWIP_CMD_TCP_NEW		(( 0 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_BIND		(( 1 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_LISTEN		(( 2 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_ACCEPT		(( 3 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_CONNECT	(( 4 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_CLOSE		(( 5 <<4)+CMD_COMMAND)
#define LWIP_CMD_TCP_DNS		(( 6 <<4)+CMD_COMMAND)


#ifndef LWIP_DRV_MAX_API_QUEUE
#define LWIP_DRV_MAX_API_QUEUE 	3
#endif

//handle state flags
#define TCPHS_OP_PCB			0x01	// client->mode is valid pcb
#define TCPHS_OP_WRITING		0x02	// Write pending
#define TCPHS_OP_READING		0x04	// Read pending
#define TCPHS_OP_CONNECTING		0x08	// Connect pending
#define TCPHS_OP_ACCEPTING		0x10	// Connect pending

#define TCPHS_OP_BOND			0x20	// Connection bond
#define TCPHS_OP_LISTEN			0x40	// Connection in listen state
#define TCPHS_OP_ESTABLISHED	0x80	// Connection is established (connected or accepted)

// handle states
#define TCPHS_UNKNOWN		0										//can new
#define TCPHS_NEW			(TCPHS_OP_PCB)							//can bind connect
#define TCPHS_CONECTING		(TCPHS_NEW | TCPHS_OP_CONNECTING)
#define TCPHS_BIND			(TCPHS_NEW | TCPHS_OP_BOND)				//can listen
#define TCPHS_LISTEN		(TCPHS_NEW | TCPHS_OP_LISTEN)			//can accept
#define TCPHS_ESTABLISHED	(TCPHS_NEW | TCPHS_OP_ESTABLISHED)		//can read write
#define TCPHS_WRITING		(TCPHS_ESTABLISHED | TCPHS_OP_WRITING)
#define TCPHS_READING		(TCPHS_ESTABLISHED | TCPHS_OP_READING)

#define TCPHS_CANCELABLE	(TCPHS_OP_READING | TCPHS_OP_ACCEPTING)
#define TCPHS_ALLPENDING	(TCPHS_OP_READING | TCPHS_OP_ACCEPTING | TCPHS_OP_WRITING | TCPHS_OP_CONNECTING)

#define LWIP_SHUT_WR		1
#define LWIP_SHUT_RD		2
#define LWIP_SHUT_RDWR		3




#endif /* LWIP_API_H_ */
