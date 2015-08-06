/*
 * wifi_opt.h
 *
 */

#ifndef WIFI_OPT_H_
#define WIFI_OPT_H_

#include <brd_cfg.h>
#include <trace.h>


/*******************************************************************************
 * WIFI driver parameters
 ******************************************************************************/

/** Write timeout in ms for the wifi uart **/
#ifndef WIFI_WRITE_TOT
#define WIFI_WRITE_TOT 8192
#endif

/** Write timeout in ms for the wifi uart **/
#ifndef GPRS_READ_TOT
#define GPRS_READ_TOT 2048
#endif

/** Send retries (or seconds) for a gprs packet  **/
#ifndef GPRS_SEND_RETRIES
#define GPRS_SEND_RETRIES 50
#endif

/** Size of the receive buffer. It must be big enough to hold one row plus the
 * command response. If SMS is enabled one row is 160*2 + some overhead...
 */
#ifndef WIFI_BUF_SIZE
#define WIFI_BUF_SIZE	300
#endif

/** WIFI Network Name
 * If enabled, drv_data.opertor_name is defined and updated **/
#ifndef WIFI_GET_NETWORK_NAME
#define	WIFI_GET_NETWORK_NAME	1
#endif



#ifndef WIFI_AT_WATCHDOG
#define	WIFI_AT_WATCHDOG	0
#endif

#ifndef WIFI_WDT_PERIOD
#define WIFI_WDT_PERIOD	50 // 1 min
#endif

#ifndef TRACE_WIFI_LEVEL
#define TRACE_WIFI_LEVEL	TRACE_LEVEL_NONE
#endif

#ifndef TRACE_GPRS_LEVEL
#define TRACE_GPRS_LEVEL	TRACE_LEVEL_NONE
#endif

#ifndef USE_GPRS_LISTEN
#define USE_GPRS_LISTEN		0
#endif
//*----------------------------------------------------------------------------
//*			WIFI TRACE
//*----------------------------------------------------------------------------

#define TRACE_CHAR_WIFI(ch) 		TRACE_CHAR_LEVEL(TRACE_WIFI_LEVEL, ch)
#define TRACE_WIFI(...) 			TRACE_LEVEL(TRACE_WIFI_LEVEL, __VA_ARGS__)
#define TRACE1_WIFI(str)			TRACE1_LEVEL(TRACE_WIFI_LEVEL, str)

#if TRACE_WIFI_LEVEL >= TRACE_LEVEL_INFO
#	define TRACE_WIFI_INFO_CHAR(ch)	TRACE_CHAR(ch)
#	define TRACE_WIFI_INFO(...)		TRACE(__VA_ARGS__)
#	define TRACE1_WIFI_INFO(str)		TRACE1(str)
#else
#	define TRACE_WIFI_INFO_CHAR(ch)
#	define TRACE_WIFI_INFO(...)
#	define TRACE1_WIFI_INFO(str)
#endif

#if TRACE_WIFI_LEVEL >= TRACE_LEVEL_ERROR
#	define TRACE_WIFI_ERROR_CHAR(ch)	TRACE_CHAR_ERROR(ch)
#	define TRACE_WIFI_ERROR(...)		TRACE_ERROR(__VA_ARGS__)
#	define TRACE1_WIFI_ERROR(str)	TRACE1_ERROR(str)
#else
#	define TRACE_WIFI_ERROR_CHAR(ch)
#	define TRACE_WIFI_ERROR(...)
#	define TRACE1_WIFI_ERROR(str)
#endif

#if TRACE_WIFI_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACE_CHAR_WIFI_DEBUG(ch)	TRACE_CHAR_DEBUG(ch)
#	define TRACE_WIFI_DEBUG(...)		TRACE_DEBUG(__VA_ARGS__)
#	define TRACE1_WIFI_DEBUG(str)		TRACE1_DEBUG(str)
#else
#	define TRACE_CHAR_WIFI_DEBUG(ch)
#	define TRACE_WIFI_DEBUG(...)
#	define TRACE1_WIFI_DEBUG(str)
#endif


#endif /* WIFI_OPT_H_ */
