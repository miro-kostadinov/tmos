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
/**************************************************************************//**
 * @ingroup ports
 * @{
 * @file     tmos.h
 * @brief    TMOS header
 * @version  V3.00
 * @date     05. May 2008
 * @author	 Miroslav Kostadinov
 *
 * @note
 * @b @{THIS HEADER FILE MUST BE INCLUDED IN **EVERY** C/C++ SOURCE @}
 *
 * 	There are 3 types of headers
 * 		1. xxx_cfg.h - configuration headers
 * 		2. xxx_inc.h - simple includes that can be used from C/assembler
 * 		3. xxx_cpp.h - C/C++ headers with C/C++ types
 *
 *  The project structure has the following levels:
 *
 * 		a. brd_xxx 	- board level
 * 		b. mcu_xxx	- mcu/chip level
 * 		c. fam_xxx	- family level
 * 		d. ser_xx	- Series level
 * 		e. cpu_xx	- CPU core (architecture) level
 *
 *
 ******************************************************************************/
#ifndef TMOS_H
#define TMOS_H

#include <tmos_inc.h>
#include "tmos_types.h"

#ifdef __cplusplus
#if USE_CSTRING
#include <str_list.h>
#endif
#endif


#include <brd_cpp.h>

#include <port_cpp.h>
#include <trace.h>



#endif
/** @} ingroup ports */
