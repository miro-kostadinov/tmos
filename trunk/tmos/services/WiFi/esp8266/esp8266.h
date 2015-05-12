/*
 * esp8266.h
 *
 *  Created on: 05.05.2015
 *      Author: Stanly
 */

#ifndef ESP8266_H_
#define ESP8266_H_

#include <tmos.h>
#include <wifi_core.h>
#include <csocket.h>

/** esp8266 maximum number of sockets
 */
#ifndef WIFI_ESP8266_MAX_SOCKETS
#define WIFI_ESP8266_MAX_SOCKETS	 4
#endif

#define MODULE_SLEEP_TOUT 100

#ifndef WIFI_LOCAL_PORT_RANGE_START
#define WIFI_LOCAL_PORT_RANGE_START 4096
#define WIFI_LOCAL_PORT_RANGE_END   0x7fff
#endif

struct esp8266_module: public wifi_module_type
{
    unsigned int used_sockets;
    unsigned int wifi_tout;
    unsigned int sleep_tout;
    HANDLE		 waiting_open;
    CSocket*	 alloc_sockets[WIFI_ESP8266_MAX_SOCKETS];
    unsigned int received_size[WIFI_ESP8266_MAX_SOCKETS];
#if USE_WIFI_LISTEN
    unsigned short listen_ports[WIFI_ESP8266_MAX_SOCKETS];
    unsigned short accept_id[WIFI_ESP8266_MAX_SOCKETS];
    static unsigned short local_port;
#endif
	esp8266_module(const WIFI_DRIVER_INFO* pinfo):wifi_module_type(pinfo)
		{
				used_sockets = 0;
				wifi_tout = 0;
				waiting_open = NULL;
				sleep_tout = 0;
				for(int i=0; i<WIFI_ESP8266_MAX_SOCKETS; i++)
				{
					alloc_sockets[i] = NULL;
					received_size[i] = 0;
#if USE_WIFI_LISTEN
					listen_ports[i] = 0;
					accept_id[i] = WIFI_ESP8266_MAX_SOCKETS;
#endif
				}
		};


    virtual RES_CODE wifi_drv_pwron(bool lowlevel = false);
    virtual RES_CODE wifi_drv_off();
    virtual NET_CODE wifi_reset(bool force);
    NET_CODE wifi_drv_level();

    NET_CODE wifi_esp8266_init_net(CSocket * sock);
    NET_CODE wifi_esp8266_socket_open(CSocket* sock);
    void wifi_esp8266_socket_close(unsigned int sid);
    void wifi_driver_socket_close(unsigned int sid, unsigned int reason);

    virtual RES_CODE process_read(CSocket* sock);
    virtual RES_CODE process_write(CSocket* sock);
    virtual RES_CODE wifi_sock_open(CSocket* sock);
    virtual RES_CODE wifi_sock_connect_adr(CSocket* sock);
    virtual RES_CODE wifi_sock_connect_url(CSocket* sock);
    virtual RES_CODE wifi_sock_close(CSocket* sock);
    virtual RES_CODE wifi_gethostbyname(CSocket* sock);
#if USE_WIFI_LISTEN
    virtual RES_CODE wifi_sock_bind_adr(CSocket* sock);
    virtual RES_CODE wifi_sock_bind_url(CSocket* sock);
    virtual RES_CODE wifi_sock_listen(CSocket* sock);
    virtual RES_CODE wifi_sock_accept(CSocket* sock);
    virtual RES_CODE wifi_sock_addr(CSocket* sock);
#endif
    virtual int wifi_notification(const char* row);
//    virtual void wifi_process_tout();
    virtual void wifi_cancelation(bool all);

    bool unacknowledged_data(unsigned int sock_id, unsigned int& size);
    unsigned int get_socket_state(unsigned int sock_id);
    unsigned int get_socket_err(unsigned int sock_id);
    unsigned int socket_send_size(unsigned int sock_id, unsigned int& writen);



};





#endif /* ESP8266_H_ */
