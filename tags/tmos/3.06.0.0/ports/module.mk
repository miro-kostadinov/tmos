##########################################################################
#
#	Make include for ports
#	
#   Platform: MPOS rev.3.0	
#
#
##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:= 
local_cpp_src-y	:=   
local_h_src-y 	:=

local_as_src-$(USE_CRC16)		+= crc16.S

local_c_src-$(USE_CRC32)		+= crc32.c

local_cpp_src-$(USE_CACHE)		+= cache.cpp

local_h_src-y 	+= tmos.h tmos_cfg.h tmos_inc.h tmos_types.h
 
local_h_src-$(USE_CACHE)		+= cache.h
local_h_src-$(USE_CRC32)		+= crc32.h
local_h_src-$(USE_CRC16)		+= crc16.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y := $(CFG_CORE)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
