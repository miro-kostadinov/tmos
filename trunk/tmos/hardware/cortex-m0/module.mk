################################################################################
#
#	Make include for hardware/cortex-m0
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

local_cpp_src-y	+= nvic_cm0.cpp

local_h_src-y 	+= core_map.h cdbg_cm0.h mpu_cm0.h nvic_cm0.h scb_cm0.h systick_cm0.h
local_h_src-y 	+= core_instr.h

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
