##########################################################################
#
#	Make include for lwIP/src/include
#	
#   Platform: MPOS rev.2.3
#
#   www.thumbos.com
#
##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

lwip_h_src 	+= lwip/opt.h lwip/debug.h lwip/arch.h lwip/netif.h lwip/err.h 
lwip_h_src 	+= lwip/ip_addr.h lwip/def.h lwip/pbuf.h


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory)

#submodules
local_modules-y := ipv4 ipv6 lwip netif

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
