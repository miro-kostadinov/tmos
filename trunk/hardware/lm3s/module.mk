################################################################################
#
#	Make include for cortex-m3/lm3s
#	
#   Platform: MPOS rev.3.0	
#
#
################################################################################

# local sources
local_as_src-y	:=  
 
local_c_src-y 	:= 

local_cpp_src-y	:= adc_lm3s.cpp gpio_lm3s.cpp ssi_lm3s.cpp sysctl_lm3s.cpp 
local_cpp_src-y	+= uart_lm3s.cpp timer_lm3s.cpp

local_h_src-y 	:= fam_cpp.h fam_inc.h memmap_inc.h sysctl_lm3s.h uart_lm3s.h 
local_h_src-y 	+= gpio_lm3s.h udma_lm3s.h ssi_lm3s.h adc_lm3s.h gpio_inc.h 
local_h_src-y 	+= ssi_inc.h uart_inc.h sysctl_inc.h types_lm3s.h timer_inc.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y :=  $(CFG_MCU)

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
