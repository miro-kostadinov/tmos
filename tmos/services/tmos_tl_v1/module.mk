##########################################################################
#
#	Make include for services/tmos_tl_v1
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

local_cpp_src-y	+= tmos_time.cpp   

local_cpp_src-$(USE_CSTRING)	+= str_list.cpp

local_h_src-y 	+= memory.h mqueue.h tmos_time.h 

local_h_src-$(USE_CSTRING)		+= str_list.h

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
