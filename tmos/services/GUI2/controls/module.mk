##########################################################################
#
#	Make include for GUI2/controls
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

local_cpp_src-y	+= gbutton.cpp gfloating_button.cpp
local_cpp_src-y	+= gedit.cpp gtext.cpp gfloating_text.cpp
local_cpp_src-y	+= gmenu.cpp gradio.cpp glistbox.cpp
local_cpp_src-y	+= gclockview.cpp gdatetime.cpp

local_h_src-y	+= gbutton.h gfloating_button.h
local_h_src-y	+= gedit.h gtext.h gfloating_text.h
local_h_src-y	+= gmenu.h gradio.h glistbox.h
local_h_src-y	+= gclockview.h gdatetime.h

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


#add current directory to include path
inc_dirs += $(subdirectory)

#submodules
local_modules-y :=  auxiliary

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
