##########################################################################
#
#	Make include for lwIP/src/core
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

local_c_src 	+= def.c dhcp.c dns.c init.c mem.c memp.c netif.c pbuf.c raw.c 
local_c_src 	+= stats.c sys.c tcp_in.c tcp_out.c tcp.c timers.c udp.c

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src))
c_sources  	+= $(call changepath,$(local_c_src))
cpp_sources += $(call changepath,$(local_cpp_src))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
#inc_dirs += $(subdirectory)

#submodules
local_modules-y := ipv4 snmp

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
