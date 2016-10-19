##########################################################################
#
#	Make include for GUI2/fonts/cp1250
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

local_cpp_src-y	+= FNT5x7-CP1250.cpp 
local_cpp_src-y	+= FNT7x9-CP1250.cpp 
local_cpp_src-y	+= FNT10x12-CP1250.cpp 
local_cpp_src-y	+= FNT10x14-CP1250.cpp 
local_cpp_src-y	+= FNT10x21-CP1250.cpp 

#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources  	+= $(call changepath,$(local_h_src-y))


