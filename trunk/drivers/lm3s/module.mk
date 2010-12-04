##########################################################################
#
#	Make include for drivers/lm3s
#	
#   Platform: MPOS rev.3.0	
#
#
##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:=   clock_drv.cpp gpio_drv.cpp spi_drv.cpp uart_driver.cpp


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))


#add current directory to include path
inc_dirs += $(subdirectory) 

#submodules
local_modules-y :=  

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
