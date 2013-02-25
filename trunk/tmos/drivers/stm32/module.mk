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

local_h_src-y 	+= tmos_drivers.h exti_drv.h usart_drv.h spi_drv.h dma_drv.h

#conditional drivers
ifeq "$(CFG_SERIES)" "stm32f2"
local_cpp_src-y	+= rtc2_drv.cpp
local_h_src-y 	+= rtc2_drv.h
endif

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
