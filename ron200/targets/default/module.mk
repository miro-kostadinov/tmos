##########################################################################
#
#	Make include for RON200
#	
#
#
##########################################################################


OUT_NAME	?= ron200
OUT_DIR		?= out/

#  optimisation level  can be [0, 1, 2, 3, s].
OPT			?= -O0

# Build library/binary/hex. can be [y, n]
BUILD_LIB	?= n
BUILD_BIN	?= y
BUILD_HEX	?= n

# Hardware selection
CFG_CORE 	?= cortex-m3
CFG_FAMILY	?= lm3s
CFG_MCU 	?= ATSAM3S4B


# Debugging format.
DEBUG 		?= dwarf-2
TFTP_PATH	:=\\tftproot\\

#===============	Project settings		===============#

# Select Target CPU


# Select Target board
#BOARD_TYPE  = HW_VER_10
BOARD_TYPE  ?= HW_VER_11

#====================   Targets settings	   =========================#

# linker script file 
LDSCRIPT	:= $(subdirectory)$(CFG_MCU).ld

#====================     TMOS settings	      =========================#

TMOS_PATH	?=../tmos/out/ron200/
TMOS_PORT	?= ron200

LDFLAGS += -Wl,-L$(TMOS_PATH) -Wl,-l$(TMOS_PORT) 

%.elf: $(TMOS_PATH)lib$(TMOS_PORT).a

##########################################################################

# local sources
local_as_src-y	:=
 
local_c_src-y 	:= 

local_cpp_src-y	:=   


#updating global variables
as_sources 	+= $(call changepath,$(local_as_src-y))
c_sources  	+= $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))

#add current directory to include path
inc_dirs += $(subdirectory) 
inc_dirs += $(TMOS_PATH)include

#submodules
local_modules-y := 

sub_modules := $(call changepath,$(local_modules-y))
modules += $(sub_modules)

include $(addsuffix /module.mk,$(sub_modules))


# Include the TMOS sources now!
#include $(TMOS_PATH)/module.mk

