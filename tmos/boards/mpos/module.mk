#-------------------------------------------------------------------------------
#
#	Config file for tmos/boards/mpos
#	
#   www.thumbos.com
#
#-------------------------------------------------------------------------------

OUT_NAME	?= mpos
OUT_DIR		?= out/$(OUT_NAME)/

#  optimisation level  can be [0, 1, 2, 3, s].
OPT			?= -O0

# Build library/binary/hex. can be [y, n]
BUILD_LIB	?= y
BUILD_BIN	?= n
BUILD_HEX	?= n

# Hardware selection
CFG_CORE 	?= arm7tdmi
CFG_FAMILY	?= at91
CFG_SERIES	?= sam7x
CFG_MCU 	?= SAM7XC256


# Debugging format.
DEBUG 		?= dwarf-2


#===============	Project settings		===============#


#===============	TMOS settings		===============#

# memcmp() memset() memclr() memcpy() memmove() strcpy() strncpy() strlen()
# strchr() strrchr() strpbrk() islower() isupper() strcmp() strncmp() strcasecmp()
# strncasecmp() toupper() tolower() XNUM_TO_DIGIT()
USE_TMOS_STRING	?=y

# TMOS_TL_V1
USE_TMOS_TL_V1	?=y
USE_CSTRING		?=y

# tmos stdlib -> sys_malloc() sys_realloc() svc_free()
USE_TMOS_STDLIB	?=y

# class CTime
USE_TIME		?=y

# sprintf() scanf() 
USE_TMOS_STDIO	?=y

#CalculateCRC32()
USE_CRC32		?=y

# class CCache
USE_CACHE		?=n

# SHA-1 class
USE_SHA_1		?=n

# GUI
USE_GUI			?=n

# GPIO output expander (shift register)
USE_GPIO_EXPANDER ?=y


##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_c_src-y 	+= #swi_tab.c
 
local_h_src-y 	+= #brd_cfg.h brd_cpp.h swi_tab.h trace.h

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

