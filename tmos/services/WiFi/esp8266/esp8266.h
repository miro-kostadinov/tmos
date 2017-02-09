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
#define WIFI_ESP8266_MAX_SOCKETS	 5
#endif

#define MODULE_SLEEP_TOUT 100

#ifndef WIFI_LOCAL_PORT_RANGE_START
#define WIFI_LOCAL_PORT_RANGE_START 4096
#define WIFI_LOCAL_PORT_RANGE_END   0x7fff
#endif

#define WIFI_QUIT_AP			"+CWQAP"
#define WFI_MUX					"+CIPMUX"
#define WIFI_GET_LOCAL_IP		"+CIFSR"
#define WIFI_LIST_AVAILABLE_AP	"+CWLAP"
#define WIFI_DISCONNECT			"+CIPCLOSE"
#define WIFI_CONNECT			"+CIPSTART"
#define WIFI_SEND_DATA			"+CIPSEND"
#define WIFI_SERVER				"+CIPSERVER"
#define WIFI_SRVER_TIMEOUT		"+CIPSTO"
#define WIFI_CONNECTION_STAT	"+CIPSTATUS"

#if USE_DEPRECATED_AT_CMD
#define WIFI_BAUD				"+CIOBAUD"
#define WIFI_SET_AP_IP			"+CIPAP"
#define WIFI_MODE				"+CWMODE"
#define WIFI_CFG_AP				"+CWSAP"
#define WIFI_JOIN_TO_AP			"+CWJAP"
	#define WIFI_CONNECTION_TYPE_INDX		5
#else
#define WIFI_BAUD				"+UART_CUR"
#define WIFI_SET_AP_IP			"+CIPAP_CUR"
#define WIFI_MODE				"+CWMODE_CUR"
#define WIFI_CFG_AP				"+CWSAP_CUR"
#define WIFI_JOIN_TO_AP			"+CWJAP_CUR"
	#define WIFI_CONNECTION_TYPE_INDX		6
#define WIFI_SET_LIST_OPTION	"+CWLAPOPT"
#endif

enum wifi_notify_t:uint8_t
{
		WIFI_NOTIFY_IDLE=0,
				WIFI_NOTIFY_LINK,
				WIFI_NOTIFY_LINKED,
				WIFI_NOTIFY_UNLINK
};

struct esp8266_module: public wifi_module_type
{
    unsigned int used_sockets;
    unsigned int wifi_tout;
    unsigned int sleep_tout;
    HANDLE		 waiting_open;
    CSTRING		 connected_network_name;
    CSocket*	 pending_connection;
    CSocket*	 alloc_sockets[WIFI_ESP8266_MAX_SOCKETS];

    unsigned int received_size[WIFI_ESP8266_MAX_SOCKETS];
    char* 		 received_data[WIFI_ESP8266_MAX_SOCKETS];
#if USE_WIFI_LISTEN
    CSocket*	 listen_socket;
    unsigned short listen_port;
    unsigned short accept_id[WIFI_ESP8266_MAX_SOCKETS];
    static unsigned short local_port;
    RES_CODE wifi_close_listen(CSocket* sock, unsigned int reason);
#endif
    wifi_notify_t notify_state;
    unsigned int  wifi_pin_pwr;

    esp8266_module(const WIFI_DRIVER_INFO* pinfo, unsigned int pwr_pin =0, unsigned int rst_pin=0 )
    	:wifi_module_type
    	 	    (
    	 	    		pinfo
    			#if WIFI_FLOW_CONTROL
    	 	    		, rst_pin
    			#endif
    			)
		{
				used_sockets = 0;
				wifi_tout = 0;
				waiting_open = nullptr;
				sleep_tout = 0;
				pending_connection = nullptr;
#if USE_WIFI_LISTEN
				listen_socket = nullptr;
				listen_port = 0;
#endif
				notify_state = WIFI_NOTIFY_IDLE;
				for(int i=0; i<WIFI_ESP8266_MAX_SOCKETS; i++)
				{
					alloc_sockets[i] = nullptr;
					received_data[i] = nullptr;
					received_size[i] = 0;
#if USE_WIFI_LISTEN
					accept_id[i] = WIFI_ESP8266_MAX_SOCKETS;
#endif
				}
				wifi_pin_pwr = pwr_pin;
		};

	RES_CODE wifi_echo_off(bool lowlevel, uint32_t indx);
    virtual RES_CODE wifi_drv_pwron(bool lowlevel = false);
    virtual RES_CODE wifi_drv_off();
    virtual NET_CODE wifi_reset(bool force);
    NET_CODE wifi_drv_level();
    RES_CODE wifi_receive_check(char sym);
    virtual bool wifi_data_received(const char* row);

    NET_CODE wifi_esp8266_init_net(CSocket * sock);
    NET_CODE wifi_esp8266_socket_open(CSocket* sock);
    void wifi_esp8266_socket_close(unsigned int sid);
    void wifi_driver_socket_close(unsigned int sid, unsigned int reason);

    virtual RES_CODE process_cmd(HANDLE client);
    virtual RES_CODE process_read(CSocket* sock);
    virtual RES_CODE process_write(CSocket* sock);
    virtual RES_CODE wifi_sock_open(CSocket* sock);
    virtual RES_CODE wifi_sock_connect_adr(CSocket* sock);
    virtual RES_CODE wifi_sock_connect_url(CSocket* sock);
    virtual RES_CODE wifi_sock_disconect(CSocket* sock);
    virtual RES_CODE wifi_sock_close(CSocket* sock);
    virtual RES_CODE wifi_gethostbyname(CSocket* sock);
#if USE_WIFI_LISTEN
    void 	module_accepted_socket(unsigned int sock_id);
    virtual RES_CODE wifi_sock_bind_adr(CSocket* sock);
    virtual RES_CODE wifi_sock_bind_url(CSocket* sock);
    virtual RES_CODE wifi_sock_listen(CSocket* sock);
    virtual RES_CODE wifi_sock_accept(CSocket* sock);
    virtual RES_CODE wifi_sock_addr(CSocket* sock);
#endif
    virtual int wifi_notification(const char* row);
    virtual void wifi_notificatoin_response();
    virtual void wifi_process_tout();
    virtual void wifi_cancelation(bool all_station, bool all_softAP);

    NET_CODE wifi_get_network_name(CSTRING& name);

    bool unacknowledged_data(unsigned int sock_id, unsigned int& size);
    unsigned int get_socket_state(unsigned int sock_id);
    unsigned int get_socket_err(unsigned int sock_id);
    unsigned int socket_send_size(unsigned int sock_id, unsigned int& writen);

    bool is_data_received(unsigned char sock_state);
};

bool wifi_get_param(const char*row, CSTRING& param, unsigned int num);
bool wifi_get_param(const char*row, unsigned int& param, unsigned int num);
bool wifi_get_param(const char*row, int& param, unsigned int num);



#endif /* ESP8266_H_ */
