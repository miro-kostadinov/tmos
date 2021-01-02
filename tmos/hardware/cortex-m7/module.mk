################################################################################
#
#	Make include for hardware/cortex-m7
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

local_cpp_src-y	+= nvic_cm7.cpp

local_h_src-y 	+= core_map.h cdbg_cm7.h itm_cm7.h mpu_cm7.h nvic_cm7.h scb_cm7.h 
local_h_src-y 	+= systick_cm7.h core_instr.h tpiu_cm7.h

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
