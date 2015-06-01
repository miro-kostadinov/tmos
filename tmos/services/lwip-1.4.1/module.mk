##########################################################################
#
#	Make include for lwIP
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


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


# Special processing for LWIP headers (keep their relative path 
#e.g. #include "lwop/opt.h"
h_objects	+= $(addprefix $(OUT_DIR)include/,$(lwip_h_src))


#add current directory to include path
#inc_dirs += $(subdirectory)

#submodules
local_modules-y :=	src 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
