################################################################################
#
#	Make include for cortex-m3
#	
#   Platform: MPOS rev.3.0	
#
#	core/family/mcu
################################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_as_src-y	+= exceptions.S services.S 
local_as_src-$(USE_CRC16)		+= crc16.S
local_as_src-$(USE_TMOS_STRING)	+= tmos_string.S 
local_as_src-$(USE_TMOS_STDLIB)	+= tmos_stdlib.S 
local_as_src-$(USE_TMOS_STDIO)	+= tmos_stdio.S 
local_as_src-$(USE_TMOS_LLONG)	+= tmos_llong.S 
local_as_src-$(USE_AES_TMOS)    += aes_tmos.S

local_cpp_src-y	+= kernel.cpp handle.cpp services.cpp  

local_h_src-y 	+= port_cpp.h port_inc.h tmos_string.h tasks.h services.h
local_h_src-y 	+= tmos_swi.h handle.h tmos_stdio.h port_asm.h tmos_atomic.h
local_h_src-$(USE_TMOS_LLONG)	+= tmos_llong.h
local_h_src-$(USE_CRC16)		+= crc16.h
local_h_src-$(USE_AES_TMOS)	    += aes_tmos.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y := $(CFG_FAMILY)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
