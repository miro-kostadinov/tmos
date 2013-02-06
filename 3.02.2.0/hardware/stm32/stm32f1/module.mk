##########################################################################
#
#	Make include for hardware/stm32/stm32f1
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

local_cpp_src-y	+= gpio_f1.cpp rcc_f1.cpp usart_f1.cpp dma_f1.cpp
 
local_h_src-y 	+= series_inc.h series_cpp.h gpio_f1.h rcc_f1.h flash_f1.h
local_h_src-y 	+= dbgmcu_f1.h usart_f1.h dma_f1.h

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
