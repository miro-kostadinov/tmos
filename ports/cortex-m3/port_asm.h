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
// Description:   Assembly definitions. Include this file in beginning  of
//					every assembler file.
//
//==========================================================================

#ifndef CPU_ASM_H_
#define CPU_ASM_H_

.syntax unified		// Put the assembler into the correct configuration.
.thumb

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 Global CPU registers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define REG_MTASK		r10
#define REG_CTASK		r11


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 		 Assembler functions
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define __ALIGN_SHORT 	.align 2
#define __ALIGN_LONG 	.align 2

#define ENTRY(name) 	\
  .global name; 		\
  __ALIGN_LONG; 		\
  name:

#define END(name) 		\
  .size name, .-name

#define FUNC(name) 		\
.type   name, %function; \
.global name; 			\
.thumb_func;			\
.func name;				\
__ALIGN_SHORT; 			\
name:

#define ENDF(name)		\
.type name, STT_FUNC; 	\
.size name, . - name;	\
.endfunc

#endif /* CPU_ASM_H_ */
