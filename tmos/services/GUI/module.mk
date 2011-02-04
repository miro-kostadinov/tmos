##########################################################################
#
#	Make include for GUI
#	
#   Platform: MPOS rev.2.3
#
#   www.thumbos.com
#
##########################################################################

# local sources
local_as_src	:=
local_c_src 	:=
local_cpp_src  	:=
local_h_src-y 	:=

local_as_src	+= rend_fun_ti.S

local_cpp_src  	+= lcd_EM6125.cpp fonts.cpp gui_drv.cpp stdgui.cpp lcd.cpp
local_cpp_src  	+= lcd_SPFD5414D.cpp

local_h_src-y 	+= gui_drv.h lcd.h fonts.h stdgui.h lcd_EM6125.h lcd_SPFD5414D.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src))
c_sources  	+= $(call changepath,$(local_c_src))
cpp_sources += $(call changepath,$(local_cpp_src))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory)