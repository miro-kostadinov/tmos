##########################################################################
#
#	Make include for hardware
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

local_h_src-y 	+= hardware_cfg.h hardware_inc.h hardware_cpp.h 

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
ifeq ($(CFG_CORE), cortex-m4)
hw_core := cortex-m3
else
hw_core := $(CFG_CORE)
endif

local_modules-y := $(CFG_FAMILY) $(hw_core)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
