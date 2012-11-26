##########################################################################
#
#	Make include for cc500/targets/default
#	
#
##########################################################################


OUT_NAME	?= cc500_def
OUT_DIR		?= out/$(OUT_NAME)/

#  optimisation level  can be [0, 1, 2, 3, s].
OPT			?= -O0

# Build library/binary/hex. can be [y, n]
BUILD_LIB	?= n
BUILD_BIN	?= y
BUILD_HEX	?= n

# GUI
USE_GUI			?=y


# Hardware selection
CFG_CORE 	?= cortex-m3
CFG_FAMILY	?= lm3s
CFG_MCU 	?= LM3S5B91_C1P


# Debugging format.
DEBUG 		?= dwarf-2
ifdef ComSpec
TFTP_PATH	?= \\tftproot\\
MPS_PATH	?= \\tftproot\\
endif

#====================   Targets settings	   =========================#

# linker script file 
LDSCRIPT	:= $(subdirectory)$(CFG_MCU).ld

#====================     TMOS settings	      =========================#

TMOS_PORT	?= cc500
TMOS_PATH	?=../tmos/out/$(TMOS_PORT)/

LDFLAGS += -Wl,-L$(TMOS_PATH) -Wl,-l$(TMOS_PORT) 
TMOS_LIB := $(TMOS_PATH)lib$(TMOS_PORT).a
libraries += $(TMOS_LIB)
$(TMOS_LIB):

inc_dirs += $(TMOS_PATH)include
 
##########################################################################

# local sources
local_as_src-y	:=
local_c_src-y 	:= 
local_cpp_src-y	:=   

local_cpp_src-y	+= drivers.cpp usb_setup.cpp

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


