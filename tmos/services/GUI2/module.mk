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

local_cpp_src-y	+= lcd_EM6125.cpp gui_drv.cpp stdgui.cpp lcd.cpp lcd_SPFD5414D.cpp
local_cpp_src-y	+= lcd_ST7565S.cpp lcd_ST7735R.cpp lcd_ST7565S_RS.cpp
local_cpp_src-y += gwindow.cpp gobject.cpp gcontainer.cpp gbutton.cpp gtext.cpp
local_cpp_src-y += gdialog.cpp gradio.cpp gedit.cpp lcd_multiplex.cpp gfloating_text.cpp
local_cpp_src-y += gfloating_button.cpp gmenu.cpp gscroll.cpp gmsgbox.cpp
local_cpp_src-y += glistbox.cpp message.cpp gdowait.cpp lcd_SPFD5414D_UART.cpp
local_cpp_src-y += lcd_ST7735R_UART.cpp
local_cpp_src-y += gedit_vkb.cpp

local_h_src-y 	+= gui_drv.h lcd.h stdgui.h lcd_EM6125.h lcd_SPFD5414D.h lcd_ST7735R.h
local_h_src-y 	+= lcd_ST7565S.h gwindow.h gobject.h gcontainer.h gbutton.h gtext.h message.h lcd_ST7565S_RS.h
local_h_src-y 	+= gdialog.h gradio.h gedit.h lcd_multiplex.h gfloating_text.h
local_h_src-y 	+= gfloating_button.h gmenu.h gscroll.h gmsgbox.h glistbox.h gdowait.h
local_h_src-y 	+= lcd_ST7565S_cmd.h lcd_SPFD5414D_UART.h lcd_ST7735R_UART.h
local_h_src-y 	+= gedit_vkb.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory)

#submodules
local_modules-y := fonts 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
