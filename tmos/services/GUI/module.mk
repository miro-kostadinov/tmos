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
local_as_src-y	:=
local_c_src-y 	:=
local_cpp_src-y	:=
local_h_src-y 	:=

local_as_src-y	+= rend_fun_ti.S

local_cpp_src-y	+= lcd_EM6125.cpp fonts.cpp gui_drv.cpp stdgui.cpp lcd.cpp
local_cpp_src-y	+= lcd_SPFD5414D.cpp lcd_ST7565S.cpp

local_h_src-y 	+= gui_drv.h lcd.h fonts.h stdgui.h lcd_EM6125.h lcd_SPFD5414D.h
local_h_src-y 	+= lcd_ST7565S.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory)