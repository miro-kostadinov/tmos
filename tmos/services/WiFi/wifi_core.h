/*
 * core_wifi.h
 *
 */

#ifndef CORE_WIFI_H_
#define CORE_WIFI_H_

#include <tmos.h>
#include <wifi_opt.h>
#include <csocket.h>


struct WIFI_DRIVER_INFO;

struct wifi_AP_t
{
	CSTRING name;		// APN name
	CSTRING user;		// APN user login name
	CSTRING pass;		// APN user pass
	uint8_t indx;		// APN index 0=default

	wifi_AP_t():
		indx(0) {};
};


// ----------        AT Commands API	--------------------------------------//
typedef unsigned int WIFI_CMD_STATE;

#define WIFI_CMD_STATE_IDLE		0x00
#define WIFI_CMD_STATE_STARTED	0x01	//!< something has been received
#define WIFI_CMD_STATE_UNK		0x02	//!< Unknown row received
#define WIFI_CMD_STATE_HND		0x04	//!< exit wifi_send_cmd() on special character
#define WIFI_CMD_STATE_ROW_STOP	0x08	//!< exit wifi_send_cmd() on first answer row
#define WIFI_CMD_STATE_RETURNED	0x10	//!< one or more answer rows recognized
#define WIFI_CMD_STATE_OK		0x20	//!< OK answer received
#define WIFI_CMD_STATE_CMES		0x40	//!< CMEE or CMES answer received
#define WIFI_CMD_STATE_FATAL	0x80	//!< failed to send command
#define WIFI_CMD_STATE_CRLF		0x100	//!< CRLF received first
#define WIFI_CMD_STATE_CRLFOK	0x200	//!< CRLF OK CRLF received


#define WIFI_CMD_STATE_RETURN	(WIFI_CMD_STATE_RETURNED | WIFI_CMD_STATE_HND)
#define WIFI_CMD_STATE_ROK		(WIFI_CMD_STATE_RETURNED | WIFI_CMD_STATE_OK)
#define WIFI_CMD_SEND_STATUS	(WIFI_CMD_STATE_ROK | WIFI_CMD_STATE_CMES | WIFI_CMD_STATE_FATAL)

#define WIFI_ON_REGISTER_CMDS		1


struct wifi_module_type
{
	const WIFI_DRIVER_INFO* drv_info;

    unsigned short row_start;
    unsigned short row_end;
    unsigned short cmd_state;  // WIFI_CMD_STATE_XXX
    unsigned char received_ch;
    unsigned char wifi_watchdog_cnt;
    bool		  in_command;

    CHandle rcv_hnd;
    CHandle snd_hnd;
#if WIFI_FLOW_CONTROL
    bool 		  stop_rcv;
    unsigned int  mem_alloc_size;
    HANDLE	 	 pending_read_data;
#endif
    unsigned int  wifi_pin_rts;
    char buf[WIFI_BUF_SIZE];

//----------------- constructor/destructor -------------------------------------
    wifi_module_type
    	(
    		const WIFI_DRIVER_INFO* pinfo
    		,unsigned int PIN_RTS
    	)
    	:drv_info(pinfo)
    	, row_start(0)
    	, row_end(0)
    	, cmd_state(0)
    	, received_ch(0)
    	, wifi_watchdog_cnt(0)
    	, in_command(false)
#if WIFI_FLOW_CONTROL
    	, stop_rcv(false)
    	, mem_alloc_size(0),
    	pending_read_data(nullptr)
#endif
    	, wifi_pin_rts(PIN_RTS)
    {
    }

    virtual ~wifi_module_type()
    {
    	//module_off();
    }

//---------------- start virtual methods (members if any) ----------------------
    virtual RES_CODE wifi_drv_pwron(bool lowlevel = false)=0;
    virtual RES_CODE wifi_drv_off()=0;
    virtual NET_CODE wifi_reset(bool force, wifi_module_type** driver_module)=0;
    virtual NET_CODE wifi_drv_level()=0;

    virtual void process_input(unsigned int signals, const char* cmd,
    		const char* hnd_start=nullptr);
    virtual int  wifi_notification(const char* row);
    virtual void wifi_notificatoin_response()
    {;}
    virtual void wifi_process_tout();
    virtual void wifi_cancelation(bool all_station, bool all_softAP);
    virtual bool wifi_data_received(const char* row)
    {
    	return false;
    }

    virtual WIFI_CMD_STATE wifi_process_row(const char *cmd);
    virtual WIFI_CMD_STATE wifi_send_cmd(const char *cmd, unsigned int time);
    virtual RES_CODE process_cmd(HANDLE client);

    virtual RES_CODE process_read(CSocket* sock) =0;
    virtual RES_CODE process_write(CSocket* sock) =0;
    virtual RES_CODE wifi_sock_open(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_connect_adr(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_connect_url(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_disconect(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_close(CSocket* sock)=0;
    virtual RES_CODE wifi_gethostbyname(CSocket* sock)=0;
#if USE_WIFI_LISTEN
    virtual RES_CODE wifi_sock_bind_adr(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_bind_url(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_listen(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_accept(CSocket* sock)=0;
    virtual RES_CODE wifi_sock_addr(CSocket* sock)=0;
#endif
    virtual NET_CODE wifi_get_network_name(CSTRING& name);
    virtual RES_CODE module_upgrade(HANDLE hnd)
    	{return NET_ERR_PHY_NOT_READY;}
//---------------- end virtual methods (members if any) ------------------------

    char* get_str_cmd(const char *cmd, unsigned int time);
    char* get_str_prm(char *row, unsigned int param);
    NET_CODE wifi_drv_on();

    friend RES_CODE wifi_drv_off(wifi_module_type *module, HANDLE hnd);
    friend void wifi_thread(WIFI_DRIVER_INFO* drv_info);
protected:
    static bool cmd_match(const char* cmd, const char* row);
    static bool cmd_submatch(const char* cmd, const char* row);

    NET_CODE wifi_check_reg();
    void wifi_sleep(unsigned int time);
    RES_CODE wifi_error(int err);
    NET_CODE wifi_net_error(int err);
    void hnd_error(HANDLE hnd) const;
};

// type definition for the commands
typedef RES_CODE (*WIFI_CBF)(wifi_module_type *module, HANDLE hnd);

#define WIFI_DRV_ON_CMD 	(( 0xD <<4)+CMD_COMMAND)
#define WIFI_DRV_UPGRADE 	(( 0xE <<4)+CMD_COMMAND)
#define WIFI_DRV_OFF_CMD 	(( 0xF <<4)+CMD_COMMAND)

extern "C" NET_CODE wifi_on_deregister(wifi_module_type* mod);

extern "C" NET_CODE wifi_on_init_station(wifi_module_type* mod, CSocket* sock, wifi_AP_t* apn);
extern "C" NET_CODE wifi_on_get_AP(wifi_module_type* mod, CSocket* sock, wifi_AP_t* apn);
extern "C" void wifi_on_disconnect(wifi_module_type* mod);
extern "C" void wifi_on_blink_transfer(wifi_module_type* mod, int reason);


#endif /* CORE_WIFI_H_ */
