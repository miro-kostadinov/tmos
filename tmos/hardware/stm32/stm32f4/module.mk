##########################################################################
#
#	Make include for hardware/stm32/stm32f4
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

local_as_src-y	+= crc_f4.S 

local_cpp_src-y	+= gpio_f4.cpp rcc_f4.cpp usart_f4.cpp dma_f4.cpp rtc_f4.cpp
local_cpp_src-y	+= flash_f4.cpp 
local_cpp_src-$(USE_NET) += eth_f4.cpp eth_phy_f4.cpp
 
local_h_src-y 	+= series_inc.h series_cpp.h gpio_f4.h rcc_f4.h flash_f4.h
local_h_src-y 	+= dbgmcu_f4.h usart_f4.h dma_f4.h pwr_f4.h rtc_f4.h adc_f4.h
local_h_src-y 	+= tim_f4.h crc_f4.h dac_f4.h eth_f4.h
local_h_src-$(USE_NET) 	+= eth_phy_f4.h

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
