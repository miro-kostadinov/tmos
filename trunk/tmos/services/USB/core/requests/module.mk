##########################################################################
#
#	Make include for services/USB/core/requests
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

local_cpp_src-y	+= 

local_h_src-y 	+= usb_requests.h usb_std_reqs.h  
local_h_src-$(USB_ENABLE_PRN) += usb_prn_reqs.h
local_h_src-$(USB_ENABLE_HID) += usb_hid_reqs.h
local_h_src-$(USB_ENABLE_CDC) += usb_cdc_reqs.h

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
