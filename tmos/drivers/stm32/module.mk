##########################################################################
#
#	Make include for drivers/stm32
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

local_cpp_src-y	+= exti_drv.cpp usart_drv.cpp spi_drv.cpp dma_drv.cpp
local_cpp_src-y	+= tim_drv.cpp sdio_drv.cpp dac_drv.cpp

local_h_src-y 	+= tmos_drivers.h exti_drv.h usart_drv.h spi_drv.h dma_drv.h
local_h_src-y 	+= tim_drv.h sdio_drv.h dac_drv.h

#conditional drivers
	use_adc1 := n
	use_adc2 := n
	use_rtc2 := n
ifeq "$(CFG_SERIES)" "stm32f2"
	use_adc2 := y
	use_rtc2 := $(USE_TIME)
else
ifeq "$(CFG_SERIES)" "stm32f4"
	use_adc2 := y
	use_rtc2 := $(USE_TIME)
else
	use_adc1 := y
endif
endif

local_cpp_src-$(use_adc2)	+= adc2_drv.cpp
local_h_src-$(use_adc2) 	+= adc2_drv.h

local_cpp_src-$(use_adc1)	+= adc1_drv.cpp
local_h_src-$(use_adc1) 	+= adc1_drv.h

local_cpp_src-$(use_rtc2)	+= rtc2_drv.cpp 
local_h_src-$(use_rtc2) 	+= rtc2_drv.h 

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
