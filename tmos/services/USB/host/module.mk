##########################################################################
#
#	Make include for services/USB/core/host
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

local_cpp_src-y	+= usb_host.cpp
local_cpp_src-$(USB_ENABLE_PRN) += usbh_prn.cpp
local_cpp_src-$(USB_ENABLE_HID) += usbh_hid.cpp
local_cpp_src-$(USB_ENABLE_MSC) += usbh_msc.cpp
local_cpp_src-$(USB_ENABLE_VID) += usbh_vid-cdc.cpp

local_h_src-y 	+= usb_host.h   
local_h_src-$(USB_ENABLE_PRN) += usbh_prn.h
local_h_src-$(USB_ENABLE_HID) += usbh_hid.h
local_h_src-$(USB_ENABLE_MSC) += usbh_msc.h
local_h_src-$(USB_ENABLE_VID) += usbh_vid-cdc.h

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
