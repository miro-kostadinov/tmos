##########################################################################
#
#	Make include for hardware/stm32/stm32h7
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

local_as_src-y	+= crc_h7.S 

local_cpp_src-y	+= gpio_h7.cpp rcc_h7.cpp usart_h7.cpp dma_h7.cpp rtc_h7.cpp
local_cpp_src-y	+= flash_h7.cpp 
local_cpp_src-$(USE_NET) += eth_h7.cpp eth_phy_h7.cpp
 
local_h_src-y 	+= series_inc.h series_cpp.h gpio_h7.h rcc_h7.h flash_h7.h
local_h_src-y 	+= dbgmcu_h7.h usart_h7.h dma_h7.h pwr_h7.h rtc_h7.h adc_h7.h
local_h_src-y 	+= tim_h7.h crc_h7.h dac_h7.h eth_h7.h
local_h_src-$(USE_NET) 	+= eth_phy_h7.h

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
