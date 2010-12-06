##########################################################################
#
#	Make include for RON200 top level
#	
#   Platform: MPOS rev.3.0	
#
#
##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:=   


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))


#submodules
local_modules-y := targets app

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
