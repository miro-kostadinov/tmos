#-------------------------------------------------------------------------------
#
#	Config file for cc100
#	
#   www.thumbos.com
#
#-------------------------------------------------------------------------------

OUT_NAME	?= cc300
OUT_DIR		?= out/$(OUT_NAME)/

#  optimisation level  can be [0, 1, 2, 3, s].
OPT			?= -Os

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


#=========================   Project settings       ===========================#

# Select Target CPU


# Select Target board


#========================    TMOS settings		===============================#

# memcmp() memset() memclr() memcpy() memmove() strcpy() strncpy() strlen()
# strchr() strrchr() strpbrk() islower() isupper() strcmp() strncmp() strcasecmp()
# strncasecmp() toupper() tolower() XNUM_TO_DIGIT()
USE_TMOS_STRING	?=y

#CSTRING class
USE_CSTRING		?=y

# tmos stdlib -> sys_malloc() sys_realloc() svc_free()
USE_TMOS_STDLIB	?=y

# class CTime
USE_TIME		?=y

# sprintf() scanf() 
USE_TMOS_STDIO	?=y

# use tmos long long operations
USE_TMOS_LLONG	?=y

#CalculateCRC32()
USE_CRC16		?=y
USE_CRC32		?=y

# class CCache
USE_CACHE		?=y

# SHA-1 class
USE_SHA_1		?=y

# base 32 
USE_BASE32		?=y

# GUI
USE_GUI			?=y

# GUI
USE_USB			?=y

# AES TMOS
USE_AES_TMOS	?=y

# GPIO output expander (shift register)
USE_GPIO_EXPANDER ?=y

#net
USE_NET			?=y


################################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_c_src-y 	+= swi_tab.c
 
local_h_src-y 	+= brd_cfg.h brd_cpp.h swi_tab.h trace.h

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

#========================     Versioning          =============================#
DEP_PROJECTS := 
TARGET_VER_FILE := $(subdirectory)versions.h 

ifneq "$(MAKECMDGOALS)" "clean"
ifneq "$(MAKECMDGOALS)" "all"
include boards/release.mk
endif
endif
