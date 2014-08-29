################################################################################
#
#	Make include for hardware/tm4c
#	
#   Platform: MPOS rev.3.0	
#
#
################################################################################

# local sources
local_as_src-y	:=  
local_c_src-y 	:= 
local_cpp_src-y	:=
local_h_src-y 	:=

local_cpp_src-y	+=  sysctl_tm4c.cpp gpio_tm4c.cpp

local_h_src-y 	+=  fam_cpp.h fam_inc.h sysctl_tm4c.h gpio_tm4c.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y :=  $(CFG_MCU)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
