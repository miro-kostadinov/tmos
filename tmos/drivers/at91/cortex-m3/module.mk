##########################################################################
#
#	Make include for drivers/at91/cortex-m3
#	
#   Platform: MPOS rev.3.0	
#
#
##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:= supc_drv.cpp rstc_drv.cpp rtc_drv.cpp rtt_drv.cpp pmc_drv.cpp
local_cpp_src-y	+= eefc_drv.cpp wdt_drv.cpp systick_drv.cpp

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))


#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y := $(CFG_SERIES)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
