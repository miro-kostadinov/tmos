##########################################################################
#
#	Make include for services/tmos_tl_v3
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


local_h_src-y 	+= ttl_config.h ttl_atomic.h ttl_iterator.h ttl_cpp_type_traits.h 
local_h_src-y 	+= ttl_initializer_list.h ttl_type_traits.h ttl_move.h ttl_algo.h 
local_h_src-y 	+= ttl_function.h ttl_string.h ttl_tree.h ttl_array.h ttl_memory.h
local_h_src-y 	+= ttl_vector.h ttl_pair.h ttl_pvector.h ttl_stack.h

local_cpp_src-y	+= ttl_config.cpp  ttl_tree.cpp  

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
