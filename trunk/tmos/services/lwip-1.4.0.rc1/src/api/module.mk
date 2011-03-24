##########################################################################
#
#	Make include for lwIP/src/api
#	
#   Platform: MPOS rev.2.3
#
#   www.thumbos.com
#
##########################################################################

# local sources
local_as_src	:=
local_c_src 	:=
local_cpp_src  	:=
local_h_src-y 	:=

local_c_src 	+= api_msg.c err.c netbuf.c netdb.c netifapi.c api_lib.c  
local_c_src 	+= sockets.c tcpip.c

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src))
c_sources  	+= $(call changepath,$(local_c_src))
cpp_sources += $(call changepath,$(local_cpp_src))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
#inc_dirs += $(subdirectory)

#submodules
local_modules-y := 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
