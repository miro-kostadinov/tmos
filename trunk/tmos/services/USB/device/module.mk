##########################################################################
#
#	Make include for services/USB/core/device
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

local_cpp_src-y	+= usb_device.cpp
local_cpp_src-$(USB_ENABLE_CDC) += cdc_data_inf.cpp cdc_acm_inf.cpp

local_h_src-y 	+= usb_device.h usb_interface.h  
local_h_src-$(USB_ENABLE_CDC) += cdc_data_inf.h cdc_acm_inf.h

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
