##########################################################################
#
#	Make include for services/security/tls
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

local_cpp_src-y	+= tls_cipher_suites.cpp tls_common.cpp tls.cpp tls_drv.cpp 
local_cpp_src-y	+= tls_misc.cpp tls_record.cpp tls_client.cpp tls_server.cpp 

local_h_src-y 	+= tls_cache.h tls_cipher_suites.h tls.h tls_drv.h tls_misc.h 
local_h_src-y 	+= tls_opt.h tls_record.h 

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
