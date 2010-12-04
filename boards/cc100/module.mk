#-------------------------------------------------------------------------------
#
#	Config file for cc100
#	
#   www.thumbos.com
#
#-------------------------------------------------------------------------------

OUT_NAME	?= cc100
OUT_DIR		?= out/$(OUT_NAME)/

#  optimisation level  can be [0, 1, 2, 3, s].
OPT			?= -O0

# Build library/binary/hex. can be [y, n]
BUILD_LIB	?= y
BUILD_BIN	?= n
BUILD_HEX	?= n

# Hardware selection
CFG_CORE 	?= cortex-m3
CFG_FAMILY	?= lm3s
CFG_MCU 	?= LM3S5B91_C1


# Debugging format.
DEBUG 		?= dwarf-2


#===============	Project settings		===============#

# Select Target CPU


# Select Target board
#BOARD_TYPE  = HW_VER_10
BOARD_TYPE  ?= HW_VER_11


#===============	TMOS settings		===============#

# memcmp() memset() memclr() memcpy() memmove() strcpy() strncpy() strlen()
# strchr() strrchr() strpbrk() islower() isupper() strcmp() strncmp() strcasecmp()
# strncasecmp() toupper() tolower() XNUM_TO_DIGIT()
USE_TMOS_STRING	?=y

#CSTRING class
USE_CSTRING		?=y

# tmos stdlib -> sys_malloc() sys_realloc() svc_free()
USE_TMOS_STDLIB	?=y

# class CTime
USE_TIME		?=n

# sprintf() scanf() 
USE_TMOS_STDIO	?=y




#CalculateCRC32()
USE_CRC32		?=n

# class CCache
USE_CACHE		?=n

# classes obj_item obj_pair obj_list
USE_OBJ_LIST	?=n



##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:= drivers.cpp  

local_h_src-y 	:= brd_cfg.h brd_cpp.h swi_tab.h trace.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y := 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))

