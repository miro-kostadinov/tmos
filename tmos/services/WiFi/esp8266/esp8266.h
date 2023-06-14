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
    bool closed_sockets[WIFI_ESP8266_MAX_SOCKETS];
#if USE_WIFI_LISTEN
    CSocket*	 listen_socket;
    unsigned short listen_port;
    unsigned short accept_id[WIFI_ESP8266_MAX_SOCKETS];
    static unsigned short local_port;
    RES_CODE wifi_close_listen(CSocket* sock, unsigned int reason);
#endif
    wifi_notify_t notify_state;
    unsigned int  wifi_pin_pwr;
    unsigned int  wifi_pin_boot;

    esp8266_module(const WIFI_DRIVER_INFO* pinfo, unsigned int pwr_pin =0,
    		unsigned int rst_pin=0, unsigned int boot_pin =0)
    	:wifi_module_type(pinfo, rst_pin)
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
					closed_sockets[i] = true;
				}
				wifi_pin_pwr = pwr_pin;
				wifi_pin_boot = boot_pin;
		};

	RES_CODE wifi_echo_off(bool lowlevel, uint32_t indx);
    RES_CODE wifi_drv_pwron(bool lowlevel = false) override;
    RES_CODE wifi_drv_off() override;
    NET_CODE wifi_reset(bool force, wifi_module_type** drv_module) override;
    NET_CODE wifi_drv_level() override;
#if USE_DEPRECATED_AT_CMD
    RES_CODE wifi_receive_check(char sym);
#endif
    bool wifi_data_received(const char* row) override;

    NET_CODE wifi_esp8266_init_net(CSocket * sock);
    NET_CODE wifi_esp8266_socket_open(CSocket* sock);
    void wifi_esp8266_socket_close(unsigned int sid);
    void wifi_driver_socket_close(unsigned int sid, unsigned int reason);

    RES_CODE process_cmd(HANDLE client) override;
    RES_CODE process_read(CSocket* sock) override;
    RES_CODE process_write(CSocket* sock) override;
    RES_CODE wifi_sock_open(CSocket* sock) override;
    RES_CODE wifi_sock_connect_adr(CSocket* sock) override;
    RES_CODE wifi_sock_connect_url(CSocket* sock) override;
    RES_CODE wifi_sock_disconect(CSocket* sock) override;
    RES_CODE wifi_sock_close(CSocket* sock) override;
    RES_CODE wifi_gethostbyname(CSocket* sock) override;
#if USE_WIFI_LISTEN
    void 	module_accepted_socket(unsigned int sock_id);
    RES_CODE wifi_sock_bind_adr(CSocket* sock) override;
    RES_CODE wifi_sock_bind_url(CSocket* sock) override;
    RES_CODE wifi_sock_listen(CSocket* sock) override;
    RES_CODE wifi_sock_accept(CSocket* sock) override;
    RES_CODE wifi_sock_addr(CSocket* sock) override;
#endif
    int wifi_notification(const char* row) override;
    void wifi_notificatoin_response() override;
    void wifi_process_tout() override;
    void wifi_cancelation(bool all_station, bool all_softAP) override;

    NET_CODE wifi_get_current_net_ssid(CSTRING& ssid) override;
    unsigned int get_socket_state(unsigned int sock_id);

#if USE_WIFI_ESP8266 < 3 // version 3.0
    bool is_data_received(unsigned char sock_state);
#endif
    RES_CODE module_upgrade(HANDLE hnd) override;

};

bool wifi_get_param(const char*row, CSTRING& param, unsigned int num);
bool wifi_get_param(const char*row, unsigned int& param, unsigned int num);
bool wifi_get_param(const char*row, int& param, unsigned int num);
uint16_t wifi_rssi_to_level(const int32_t rssi );



#endif /* ESP8266_H_ */
