##########################################################################
#
#	Make include for hardware/stm32
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

local_h_src-y 	+= fam_cpp.h fam_inc.h gpio_stm32.h exti_stm32.h spi_stm32.h
local_h_src-y 	+= otg_hs_stm32.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y :=  $(CFG_SERIES)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
