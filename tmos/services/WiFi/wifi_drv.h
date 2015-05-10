/*
 * wifi_drv.h
 *
 */

#ifndef WIFI_DRV_H_
#define WIFI_DRV_H_
#include <tmos.h>
#include <wifi_opt.h>
#include <wifi_core.h>




#define WIFI_TRANSFER_INDICATOR		1
#define WIFI_STATE_INDICATOR			2


//*----------------------------------------------------------------------------
//*			WiFi driver data
//*----------------------------------------------------------------------------

// WIFI_FLAG_XXXX
#define WIFI_FLAG_ON			0x01		//!< WiFi powered on
#define WIFI_FLAG_REGISTERED	0x08		//!< WiFi registered in the network
#define WIFI_FLAG_SHUTDOWN		0x80

// WIFI_STATUS_XXXX
#define WIFI_STATE_OFF			0x00

//-----------------			WiFi error codes				 	 -----------------//
#define NET_ERR_WIFI				0x200	//!< WiFi communication general error
#define NET_ERR_WIFI_ON				0x201	//!< Failed to power on the wifi module
#define NET_ERR_WIFI_SHUTDOWN		0x202	//!< Failed the wifi module in shutdown mode
#define NET_ERR_WIFI_OFF			0x203	//!< Failed to power off the wifi module
#define NET_ERR_WIFI_REGISTER		0x206	//!< Failed to register
#define NET_ERR_WIFI_REJECTED		0x207	//!<
#define NET_ERR_WIFI_NET_NAME		0x208	//!< Failed to get network name




struct WIFI_DRIVER_DATA
{
    unsigned char		wifi_clients;
    unsigned char		wifi_flags_ok;
    unsigned char		wifi_flags_bad;

    HANDLE				waiting;
    wifi_module_type* 	wifi_module;		//!< Pointer to the detected wifi module (or NULL)

	CHandle				helper_hnd;		//!< Handle used between the helper task and the driver
    unsigned int		turnoff_time;
    unsigned short int	signal_level; 		// radio channel level
    unsigned int		wifi_error;

#if WIFI_NETWORK_NAME
    CSTRING 			network_name;
#endif
};

struct WIFI_DRIVER_INFO
{

	//--- required for all drivers -----///
	DRIVER_INFO_Type 	info;				//!< standard driver info, required from the OS


	//--- fields specific for this (WiFi) driver -----///
    WIFI_DRIVER_DATA*	drv_data;			//!< pointer to the driver data (RAM) structure

	DRIVER_INDEX		iface_driver_index;	//!< Index of the interface driver
	const void*			iface_mode_stru;	//!< mode for the interface handle
};


void WIFI_DCR(WIFI_DRIVER_INFO * drv_info, unsigned int reason, HANDLE param);
void WIFI_DSR(WIFI_DRIVER_INFO * drv_info, HANDLE hnd);


#endif /* WIFI_DRV_H_ */
