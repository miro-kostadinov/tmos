##########################################################################
#
#	Make include for cortex-m3/cmsis
#	
#   Platform: MPOS rev.3.0	
#
##########################################################################

# local sources
local_as_src-y	:=  
 
local_c_src-y 	:= 

local_cpp_src-y	:= mpu_cm3.cpp nvic_cm3.cpp scb_cm3.cpp systick_cm3.cpp   

local_h_src-y 	:= cmsis_cpp.h cmsis_inc.h core_cmInstr.h core_cmFunc.h itm_cm3.h 
local_h_src-y 	+= mpu_cm3.h nvic_cm3.h nvic_inc.h scb_cm3.h systick_cm3.h  
 
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
