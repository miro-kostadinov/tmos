##########################################################################
#
#	Make include for drivers
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

ifeq ($(CFG_CORE), cortex-m0)
local_cpp_src-y	+= systick_drv.cpp
local_h_src-y 	+= systick_drv.h
endif
ifeq ($(CFG_CORE), cortex-m3)
local_cpp_src-y	+= systick_drv.cpp
local_h_src-y 	+= systick_drv.h
endif
ifeq ($(CFG_CORE), cortex-m4)
local_cpp_src-y	+= systick_drv.cpp
local_h_src-y 	+= systick_drv.h
endif

USE_GUIX := $(call use_if_or,$(USE_GUI)$(USE_GUI2))

local_cpp_src-$(USE_GUIX) 	+= key_drv.cpp 
local_h_src-$(USE_GUIX)		+= key_drv.h 


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
