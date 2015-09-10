##########################################################################
#
#	Make include for hardware/stm32/stm32f0
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

local_cpp_src-y	+= gpio_f0.cpp rcc_f0.cpp usart_f0.cpp
 
local_h_src-y 	+= series_inc.h series_cpp.h gpio_f0.h rcc_f0.h flash_f0.h 
local_h_src-y 	+= dbgmcu_f0.h usart_f0.h dma_f0.h pwr_f0.h rtc_f0.h adc_f0.h
local_h_src-y 	+= tim_f0.h crc_f0.h dac_f0.h

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
