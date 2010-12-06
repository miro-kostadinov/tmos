//==========================================================================
// This file is part of TMOS Operating System.
// Copyright (C) 2007-2010 Informa Ltd.
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
// Date:          2008-05-01
// Description:   Common defines for TMOS
//
// NOTE: This file can be included in both C and assembler sources
//========================================================================
#ifndef	PORT_INC_H_
#define	PORT_INC_H_

#include <tmos_cfg.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 task structure offsets
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define tsk_sp			0x00
#define tsk_r4			0x04
#define tsk_r5			0x08
#define tsk_r6			0x0C
#define tsk_r7			0x10
#define tsk_r8			0x14
#define tsk_r9			0x18

#define tsk_aloc_sig	(0x1C + 0x00)
#define tsk_state		(0x1C + 0x02)
#define tsk_pty			(0x1C + 0x03)
#define tsk_signal		0x20
#define tsk_renext		0x24
#define tsk_reprev		0x28
#define tsk_time		0x2C
#define tsk_tnext		0x30
#define tsk_tprev		0x34
#define tsk_tlist		0x38
#define tsk_name		0x3c

//	offsets in stacked context structure
//#define tsk_tprev		0x34
#define tsk_sp_r0		0x00
#define tsk_sp_r1		0x04
#define tsk_sp_r2		0x08
#define tsk_sp_r3		0x0C
#define tsk_sp_r12		0x10
#define tsk_sp_lr		0x14
#define tsk_sp_pc		0x18
#define tsk_sp_psr		0x1c

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 driver structure offsets
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// offsets in DRV_INFO structure
//#define DRV_INFO_INDX		0x00
#define DRV_INFO_OP0		0x00
#define DRV_INFO_OP4		0x04
#define DRV_INFO_ISR		0x08
#define DRV_INFO_DCR		0x0C
#define DRV_INFO_DSR		0x10



//----- Driver Priority  --------------------//
#define DRV_ISR_PRIORITY_0		0
#define DRV_ISR_PRIORITY_1		1
#define DRV_ISR_PRIORITY_2		2
#define DRV_ISR_PRIORITY_3		3
#define DRV_ISR_PRIORITY_4		4
#define DRV_ISR_PRIORITY_5		6
#define DRV_ISR_PRIORITY_6		6
#define DRV_ISR_PRIORITY_7		7

#define DRV_HW_ID_NOHW			0xFF



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 handle structure offsets
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define HND_DRV_INDX  		(0x00 + 0x00)
#define HND_SIGNAL  		(0x00 + 0x01)
#define HND_MODES			(0x00 + 0x02)
#define HND_NEXT			0x04
#define HND_CLIENT			0x08
#define HND_LEN				(0x0C + 0x00)
#define HND_CMD				(0x0C + 0x02)
#define HND_RES 			(0x0C + 0x03)
#define HND_DST				0x10
#define HND_SRC				0x14
#define HND_MODE			0x18


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 states & flags
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#define TSKSTATE			0x1
#define TSKSTATE_SUSPEND	0x00
#define TSKSTATE_READY		0x80
#define TSKSTATE_TIMED		0x01
#define TSKSTATE_SIGNAL		0x02
#define TSKSTATE_SIGTIME	0x03
#define TSKSTATE_BLOCK		0x04
#define TSKSTATE_BLOCKTIME	0x05


#define FLAG_READ			0x01
#define FLAG_WRITE			0x02
#define FLAG_COMMAND		0x04
#define FLAG_LOCK			0x08
#define FLAG_CANCEL			0x10
#define FLAG_SWAP			(FLAG_READ |FLAG_WRITE)


#define CMD_READ_WRITE		(FLAG_READ | FLAG_WRITE )
#define CMD_READ			(FLAG_READ)
#define CMD_WRITE			(FLAG_WRITE)
#define CMD_COMMAND			(FLAG_COMMAND)
#define CMD_LOCK			(FLAG_LOCK)
#define CMD_BRIDGE			(FLAG_COMMAND|FLAG_READ|FLAG_WRITE)

#define FLG_OK				0x01
#define FLG_EOF				0x02
#define FLG_MEMORY			0x04
#define FLG_DATA			0x08
#define FLG_ERROR			0x10
#define FLG_SIGNALED		0x20
#define FLG_BUSY			0x40
#define FLG_CLOSED			0x80


#define RES_IDLE			0
#define RES_SIG_IDLE		(FLG_SIGNALED)
#define RES_SIG_CANCEL		(FLG_SIGNALED)
#define	RES_OK				FLG_OK
#define RES_SIG_OK			(FLG_SIGNALED | FLG_OK)
#define RES_ERROR			FLG_ERROR
#define	RES_BUSY			FLG_BUSY
#define RES_SIG_ERROR		(FLG_SIGNALED | FLG_ERROR)
#define	RES_CLOSED			FLG_CLOSED

#define RES_OUT_OF_MEMORY   (FLG_ERROR | FLG_MEMORY)
#define RES_EOF				(FLG_OK|FLG_EOF)
#define RES_FATAL			(FLG_DATA|FLG_ERROR)
#define RES_INVALID_DATA	(FLG_DATA)


#define SIGNAL_ANY			0xFF


#endif /*	PORT_INC_H_	*/
