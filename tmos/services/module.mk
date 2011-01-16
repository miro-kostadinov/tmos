##########################################################################
#
#	Make include for services
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

local_as_src-$(USE_AES_TMOS)+= aes_tmos.S

local_cpp_src-$(USE_SHA_1) 	+= sha_1.cpp

local_h_src-$(USE_SHA_1) 	+= sha_1.h
local_h_src-$(USE_AES_TMOS)	+= aes_tmos.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#submodules
local_modules-y := 

local_modules-$(USE_POLARSSL)	+= PolarSSL

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
