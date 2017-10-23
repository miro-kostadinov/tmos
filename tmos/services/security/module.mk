##########################################################################
#
#	Make include for services/security
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


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#submodules
local_modules-y := 
ifeq ($(USE_SECURITY_TLS), y)
local_modules-y += crypto tls
else
local_modules-$(USE_SECURITY_CRYPTO) += crypto
endif

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
