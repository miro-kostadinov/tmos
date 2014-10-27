##########################################################################
#
#	Make include for boards
#	
#   Platform: MPOS rev.3.0	
#
#
##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:=   


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))

#submodules
ifeq ($(CFG_BOARD), )
ifeq ($(CFG_EVAL), )
else
local_modules-y := evals
endif
else
local_modules-y := $(CFG_BOARD)
endif

ifeq ($(local_modules-y), )
$(error No board is selected! Use make CFG_BOARD=xxx or CFG_EVAL=xxx)
endif

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))
