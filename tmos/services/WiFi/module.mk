##########################################################################
#
#	Make include for services/WiFi
#	
#   Platform: MPOS rev.2.3
#
#
##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_cpp_src-y	+= wifi_drv.cpp wifi_core.cpp

local_h_src-y 	+= wifi_drv.h wifi_opt.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-$(USB_WIFI_ESP8266) += esp8266

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
