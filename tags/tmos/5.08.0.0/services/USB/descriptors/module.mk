##########################################################################
#
#	Make include for services/USB/core/descriptors
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

local_cpp_src-y	+= usb_descriptors.cpp

local_h_src-y 	+= usb_descriptors.h usb_std_dtors.h  
local_h_src-$(USB_ENABLE_PRN) += usb_prn_dtors.h
local_h_src-$(USB_ENABLE_HID) += usb_hid_dtors.h
local_h_src-$(USB_ENABLE_CDC) += usb_cdc_dtors.h

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
