//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2008 Informa Ltd.
//
// TMOS is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 3 or (at your option) any later version.
//
// TMOS is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
//==========================================================================
//
// Author(s):     Miroslav Kostadinov
// Date:          2008-12-07
// Description:   TMOS trace macros
//
//
//========================================================================
#ifndef TRACE_H_
#define TRACE_H_


#define TRACE_DISABLED			0
#define TRACE_PEEDI				1 //enable trace over JTAG DCC/ SWO using PEEDI format
#define TRACE_OPENOCD			2 //enable trace over JTAG DCC using OpenOCD format
#define TRACE_DBGU				3 //enable trace over DBGU uart

//select one from the above
#define TRACE_IS				TRACE_PEEDI

//--- stack size required to use trace with parameters
#define TRACE_SIZE		128



//*----------------------------------------------------------------------------
//*			Low level TRACE functions
//*----------------------------------------------------------------------------

#if TRACE_IS == TRACE_DISABLED
# define TRACE(...)
# define TRACELN(...)
# define TRACE1(str)
# define TRACELN1(str)
#elif TRACE_IS == TRACE_PEEDI

# define TRACE(...)								\
	do											\
	{											\
		char tr_buf[80];						\
		tmos_sprintf(tr_buf, __VA_ARGS__);		\
		usr_trace_str(tr_buf);					\
	}while(0)
# define TRACELN(str,...) TRACE("\r\n" str, ##__VA_ARGS__)
# define TRACE1(str) usr_trace_str(str)
# define TRACELN1(str) usr_trace_str("\r\n" str)

#endif


#if TRACE_IS == TRACE_DISABLED
#define ASSERT(expr)
#else
#define ASSERT(expr)	 						\
	do {                                      	\
		 if(!(expr))                        	\
		 {                                  	\
			 TRACELN("ASSERT %s at %d",__FILE__, __LINE__); \
		 }                                  	\
    } while (0)
#endif

//*----------------------------------------------------------------------------
//*			TRACE levels
//*----------------------------------------------------------------------------

#define TRACE_LEVEL_NONE                0
#define TRACE_LEVEL_INFO                1
#define TRACE_LEVEL_WARNING             2
#define TRACE_LEVEL_ERROR  	            3
#define TRACE_LEVEL_FATAL               4
#define TRACE_LEVEL_DEBUG               5
#define TRACE_LEVEL_TRACE               6

#define TRACE_DEFAULT_LEVEL          	TRACE_LEVEL_TRACE
#define TRACE_GSM_LEVEL					TRACE_LEVEL_DEBUG
#define TRACE_USB_LEVEL					TRACE_LEVEL_DEBUG
#define TRACE_WML_LEVEL					TRACE_LEVEL_DEBUG
#define TRACE_MEMORY_LEVEL				TRACE_LEVEL_DEBUG
#define TRACE_FOCUS_LEVEL				TRACE_LEVEL_DEBUG
#define TRACE_FILE_SYSTEM_LEVEL			TRACE_LEVEL_DEBUG
#define TRACE_PRN_LEVEL					TRACE_LEVEL_DEBUG
#define TRACE_ECR_LEVEL					TRACE_LEVEL_DEBUG
// trace functions with levels:

#define TRACE_LEVEL(level, ...) do{ if (level >= TRACE_DEFAULT_LEVEL) TRACE(__VA_ARGS__); }while(0)
#define TRACE1_LEVEL(level, str) do{ if (level >= TRACE_DEFAULT_LEVEL) TRACE1(str); }while(0)

#if TRACE_DEFAULT_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACE_DEBUG(...)		TRACE(__VA_ARGS__)
#	define TRACE1_DEBUG(str)	TRACE1(str)
#else
#	define TRACE_DEBUG(...)
#	define TRACE1_DEBUG(str)
#endif

#if TRACE_DEFAULT_LEVEL >= TRACE_LEVEL_FATAL
#	define TRACE_FATAL(...)		TRACE(__VA_ARGS__)
#	define TRACE1_FATAL(str)	TRACE1(str)
#else
#	define TRACE_FATAL(...)
#	define TRACE1_FATAL(str)
#endif

#if TRACE_DEFAULT_LEVEL >= TRACE_LEVEL_ERROR
#	define TRACE_ERROR(...)			TRACE(__VA_ARGS__)
#	define TRACE1_ERROR(str)		TRACE1(str)
#   define TRACELN_ERROR(str,...) 	TRACE("\r\n" str, ##__VA_ARGS__)
#   define TRACELN1_ERROR(str) 		TRACE1("\r\n" str)
#else
#	define TRACE_ERROR(...)
#	define TRACE1_ERROR(str)
#endif

#if TRACE_DEFAULT_LEVEL >= TRACE_LEVEL_WARNING
#	define TRACE_WARNING(...)	TRACE(__VA_ARGS__)
#	define TRACE1_WARNING(str)	TRACE1(str)
#else
#	define TRACE_WARNING(...)
#	define TRACE1_WARNING(str)
#endif

#if TRACE_DEFAULT_LEVEL >= TRACE_LEVEL_INFO
#	define TRACE_INFO(...)		TRACE(__VA_ARGS__)
#	define TRACE1_INFO(str)		TRACE1(str)
#else
#	define TRACE_INFO(...)
#	define TRACE1_INFO(str)
#endif

//*----------------------------------------------------------------------------
//*			USB
//*----------------------------------------------------------------------------
#define TRACE_USB(...) 			TRACE_LEVEL(TRACE_USB_LEVEL, __VA_ARGS__)
#define TRACE1_USB(str)			TRACE1_LEVEL(TRACE_USB_LEVEL, str)

#if TRACE_USB_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACE_USB_DEBUG(...)		TRACE_DEBUG(__VA_ARGS__)
#	define TRACE1_USB_DEBUG(str)	TRACE1_DEBUG(str)
#else
#	define TRACE_USB_DEBUG(...)
#	define TRACE1_USB_DEBUG(str)
#endif

#if TRACE_USB_LEVEL >= TRACE_LEVEL_WARNING
#	define TRACE_USB_WARNING(...)		TRACE_WARNING(__VA_ARGS__)
#	define TRACE1_USB_WARNING(str)	TRACE1_WARNING(str)
#else
#	define TRACE_USB_WARNING(...)
#	define TRACE1_USB_WARNING(str)
#endif


//*----------------------------------------------------------------------------
//*			DINAMIC MEMORY
//*----------------------------------------------------------------------------
#define TRACE_MEMORY(...) 			TRACE_LEVEL(TRACE_MEMORY_LEVEL, __VA_ARGS__)
#define TRACE1_MEMORY(str)			TRACE1_LEVEL(TRACE_MEMORY_LEVEL, str)

#if TRACE_MEMORY_LEVEL >= TRACE_LEVEL_DEBUG
#	define TRACE_MEMORY_DEBUG(...)		TRACE_DEBUG(__VA_ARGS__)
#	define TRACE1_MEMORY_DEBUG(str)	TRACE1_DEBUG(str)
#else
#	define TRACE_MEMORY_DEBUG(...)
#	define TRACE1_MEMORY_DEBUG(str)
#endif

#if TRACE_MEMORY_LEVEL >= TRACE_LEVEL_WARNING
#	define TRACE_MEMORY_WARNING(...)		TRACE_WARNING(__VA_ARGS__)
#	define TRACE1_MEMORY_WARNING(str)	TRACE1_WARNING(str)
#else
#	define TRACE_MEMORY_WARNING(...)
#	define TRACE1_MEMORY_WARNING(str)
#endif

#endif /* TRACE_H_ */
