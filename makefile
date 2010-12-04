#-------------------------------------------------------------------------------
#
#	makefile for TMOS
#	
# 	Copyright (C) 2008-2010  Informa, www.thumbos.com
#
#	This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation.
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#   http://www.gnu.org/licenses/
#
#-------------------------------------------------------------------------------
# Usage:
#
#	make            ; build debug & release
#   make debug		; build a debug version
#   make release    ; build a release version
#	make clean		; clean the project
#
#-------------------------------------------------------------------------------
# Changelog:
#		2008 		- Miroslav Kostadinov - TMOS version		
#		22-10-2009 	- Ilko Iliev - PEEDI version 
#		28-10-2009 	- Miroslav Kostadinov - redesign  
#-------------------------------------------------------------------------------


#===============	1.C global variables		===============#
# Initialize them here as simple variables.

modules 		:=  

prebuild 		:=
postbuild 		:=

as_sources 		:=
as_a_sources 	:=
as_ai_sources 	:=
as_t_sources 	:=
as_ti_sources 	:=

c_sources 		:=
c_a_sources 	:=
c_ai_sources 	:=
c_t_sources 	:=
c_ti_sources 	:=

cpp_sources 	:=
cpp_a_sources 	:=
cpp_ai_sources 	:=
cpp_t_sources 	:=
cpp_ti_sources 	:=

h_sources 		:=

libraries 		:=
cdefines 		:=
adefines 		:=
inc_dirs 		:=
lib_dirs 		:=

as_objects 	 	+= $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_sources))
as_a_objects 	+= $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_a_sources))
as_ai_objects 	+= $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_ai_sources))
as_t_objects 	+= $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_t_sources))
as_ti_objects 	+= $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_ti_sources))
all_as_objects	+= $(as_objects) $(as_a_objects) $(as_ai_objects) $(as_t_objects) $(as_ti_objects)

c_objects 		+=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_sources))
c_a_objects 	+=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_a_sources))
c_ai_objects	+=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_ai_sources))
c_t_objects		+=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_t_sources))
c_ti_objects	+=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_ti_sources))
all_c_objects	+=$(c_a_objects) $(c_ai_objects) $(c_t_objects) $(c_ti_objects) $(c_objects)

cpp_objects 	+=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_sources))
cpp_a_objects 	+=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_a_sources))
cpp_ai_objects 	+=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_ai_sources))
cpp_t_objects	+=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_t_sources))
cpp_ti_objects	+=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_ti_sources))
all_cpp_objects +=$(cpp_a_objects) $(cpp_ai_objects) $(cpp_t_objects) $(cpp_ti_objects) $(cpp_objects)

h_objects 		+=$(addprefix $(OUT_DIR)include/,$(notdir $(h_sources)))


objects  		=$(all_as_objects) $(all_c_objects) $(all_cpp_objects)
listings		=$(subst .o,.lst,$(objects))
dependencies	=$(wildcard $(addsuffix .dep,$(objects)))

#===============	1.D TOOLCHAIN COMMANDS  	===============#
#TOOLCHAIN_PREFIX	= arm-none-eabi-
TOOLCHAIN_PREFIX	= arm-elf-

CC		= $(TOOLCHAIN_PREFIX)gcc
AS		= $(TOOLCHAIN_PREFIX)as
AR		= $(TOOLCHAIN_PREFIX)ar
LD		= $(TOOLCHAIN_PREFIX)ld
CPP		= $(TOOLCHAIN_PREFIX)g++
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
NM      = $(TOOLCHAIN_PREFIX)nm
SIZE   	= $(TOOLCHAIN_PREFIX)size
HEXSIZE = $(SIZE) --target=ihex $(PROJECT).hex
ELFSIZE = $(SIZE) -B -x -t $(PROJECT).elf
LIBSIZE = $(SIZE) -B -x -t $(LIBNAME).a
REMOVE  = rm -rf
REMOVEDIR = rm -rf

#===============	1.E TOOLCHAIN FLAGS  	    ===============#
# Flags for general use of gcc (asembling, compiling, linking)
GCCFLAGS +=-mcpu=$(CFG_CORE) -mthumb
GCCFLAGS +=-g -g3
GCCFLAGS +=$(OPT) -fomit-frame-pointer -fverbose-asm
GCCFLAGS +=-ffunction-sections -fdata-sections
GCCFLAGS +=-fno-dwarf2-cfi-asm

CFLAGS +=$(addprefix -D,$(cdefines))
CFLAGS +=$(addprefix -I,$(inc_dirs))
CFLAGS +=-Wall -Wcast-align -Wimplicit 
CFLAGS +=-Wpointer-arith -Wswitch
CFLAGS +=-Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS +=-Wa,-adhlns=$(subst $(suffix $@),.lst,$@)

# flags only for C
CONLYFLAGS +=-Wnested-externs -funsigned-char
CONLYFLAGS +=-Wstrict-prototypes
CONLYFLAGS +=-std=gnu99

# flags only for C++ (arm-elf-g++)
CPPFLAGS =-fno-rtti -fno-exceptions -fno-use-cxa-atexit
CPPFLAGS +=-std=c++0x

# Assembler flags.
ASFLAGS += $(addprefix -D ,$(adefines))
ASFLAGS += $(addprefix -I,$(inc_dirs))
ASFLAGS += -Wa,-adhlns=$(OUT_DIR)$(<:.S=_S.lst),--gstabs
ASFLAGS += -x assembler-with-cpp

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF $@.dep

# Linker flags.
LDFLAGS += -nostartfiles -nostdlib
LDFLAGS += -Wl,-Map=$(PROJECT).map,--cref,--gc-sections
LDFLAGS += $(addprefix -L,$(lib_dirs))
LDFLAGS += -T$(LDSCRIPT)

# AR flags
ARFLAGS := rvcs

#START_TIME   := $(shell timestamp.bat)
#CURRENT_TIME =  $(shell timestamp.bat $(START_TIME))

###########################  includes  ############################################
# default goal is "all" - set it before include files
all:

#-------------------------------------------------------------------------------
#						subdirectory()
#						--------------
#-------------------------------------------------------------------------------
define subdirectory
	$(patsubst %module.mk,%, \
		$(word \
		$(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
endef

#-------------------------------------------------------------------------------
#						changepath()
#						------------
# $(call changepath, local-source)
#-------------------------------------------------------------------------------
define changepath
	$(addprefix $(subdirectory),$(1))
endef


#-------------------------------------------------------------------------------
# include all modules
#-------------------------------------------------------------------------------
include module.mk

ifneq "$(MAKECMDGOALS)" "clean"
_MKDIRS := $(shell mkdir $(OUT_DIR) 2>/dev/null)
_MKDIRS := $(shell mkdir $(OUT_DIR)/include 2>/dev/null)
_MKDIRS := $(shell for d in $(modules); \
	do \
		mkdir -p $(OUT_DIR)$$d; \
	done)
endif

# Project name used for the output files:
PROJECT = $(OUT_DIR)$(OUT_NAME)
LIBNAME = $(OUT_DIR)lib$(OUT_NAME)

###########################   rules    ############################################

.PHONY: begin end all clean sizebefore sizeafter build elf hex bin lib lss sym clean \
		clean_list $(modules)/module.mk makefile headers

# Default target.
all: begin sizebefore $(prebuild) build $(postbuild) sizeafter end

# Target: clean project.
clean: begin clean_list end

# build target
ifeq ($(BUILD_HEX),y)
build: elf hex lss sym
IMGEXT=hex
endif

ifeq ($(BUILD_BIN),y)
build: elf bin lss sym 
IMGEXT=bin
endif
 
ifeq ($(BUILD_LIB),y)
build: lib headers
IMGEXT=a
endif

ifeq ($(IMGEXT),) 
$(error "Nothing to build. Set BUILD_LIB, BUILD_HEX or BUILD_BIN")
endif

lib: $(LIBNAME).a
bin: $(PROJECT).bin
hex: $(PROJECT).hex
elf: $(PROJECT).elf
lss: $(PROJECT).lss 
sym: $(PROJECT).sym
headers: $(h_objects)


#make a
%.a: $(objects)
	@echo
	@echo Archiving: $@
	$(AR) $(ARFLAGS) $@ $(objects) 

#make ELF
%.elf:  $(objects) 
	@echo
	@echo Linking: $@
	$(CC) -mthumb -mthumb-interwork $(GCCFLAGS) $(objects) --output $@ $(LDFLAGS)

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	@echo Creating load file for Flash: $@
	$(OBJCOPY) -O -hex $< $@

# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo
	@echo Creating load file for Flash: $@
	$(OBJCOPY) -O binary $< $@
	cp $@ $(TFTP_PATH)$(@F)

	
# Create extended listing file from ELF output file.
# testing: option -C
%.lss: %.elf
	@echo
	@echo Creating Extended Listing: $@
#	$(OBJDUMP) -h -S -C $< > $@
	$(OBJDUMP) -h -d -g -e -C -w $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo Creating Symbol Table: $@
	$(NM) -n $< > $@
	
clean_list :
	@echo
	@echo Cleaning project:
	$(REMOVE) $(OUT_DIR)

begin:
	@echo 
	@echo "-------- begin (make $(MAKECMDGOALS)) --------"
	@$(AS) --version
	@$(LD) --version
	@$(CC) --version

end:
	@echo "--------  end  --------"
#	@echo "elapsed time: $(CURRENT_TIME)" 
			
# Display size of file.
sizebefore:
	@if [ -f $(PROJECT).elf ]; then echo; echo Size before:; $(ELFSIZE); echo; fi
	@if [ -f $(PROJECT).hex ]; then echo; echo Size before:; $(HEXSIZE); echo; fi
	@if [ -f $(LIBNAME).a ]; then echo; echo Size before:; $(LIBSIZE); echo; fi

sizeafter:
	@if [ -f $(PROJECT).elf ]; then echo; echo Size after:; $(ELFSIZE); echo; fi
	@if [ -f $(PROJECT).hex ]; then echo; echo Size before:; $(HEXSIZE); echo; fi
	@if [ -f $(LIBNAME).a ]; then echo; echo Size before:; $(LIBSIZE); echo; fi

# ======= Assemble: create object files from assembler source files. ARM/Thumb
$(as_objects) : $(OUT_DIR)%_S.o :  %.S
	@echo
	@echo Assembling: $<
	$(CC) -c -mthumb $(GCCFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
#	$(OBJDUMP) -h $@

$(as_a_objects) : $(OUT_DIR)%_S.o :  %.S
	@echo
	@echo Assembling: $<
	$(CC) -c $(GCCFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
#	$(OBJDUMP) -h $@

$(as_ai_objects) : $(OUT_DIR)%_S.o :  %.S
	@echo
	@echo Assembling: $<
	$(CC) -c -mthumb-interwork $(GCCFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
#	$(OBJDUMP) -h $@

$(as_t_objects) : $(OUT_DIR)%_S.o :  %.S
	@echo
	@echo Assembling: $<
	$(CC) -c -mthumb $(GCCFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
#	$(OBJDUMP) -h $@

$(as_ti_objects) : $(OUT_DIR)%_S.o :  %.S
	@echo
	@echo Assembling: $<
	$(CC) -c -mthumb -mthumb-interwork $(GCCFLAGS) $(ASFLAGS) $(GENDEPFLAGS) $< -o $@
#	$(OBJDUMP) -h $@

# ====-== Compile: create object files from C source files. ARM/Thumb
$(c_objects) : $(OUT_DIR)%_c.o : %.c
	@echo
	@echo Compiling C: $<
	$(CC) -c -mthumb $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(c_a_objects) : $(OUT_DIR)%_c.o : %.c
	@echo
	@echo Compiling C: $<
	$(CC) -c $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(c_ai_objects) : $(OUT_DIR)%_c.o : %.c
	@echo
	@echo Compiling C: $<
	$(CC) -c -mthumb-interwork $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS) $< -o $@ 
#	$(OBJDUMP) -h $@

$(c_t_objects) : $(OUT_DIR)%_c.o : %.c
	@echo
	@echo Compiling C: $<
	$(CC) -c -mthumb $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(c_ti_objects) : $(OUT_DIR)%_c.o : %.c
	@echo
	@echo Compiling C: $<
	$(CC) -c -mthumb -mthumb-interwork $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS) $< -o $@ 
#	$(OBJDUMP) -h $@



# ====-== Compile: create object files from CPP source files. ARM/Thumb
$(cpp_objects) : $(OUT_DIR)%_cpp.o : %.cpp
	@echo
	@echo Compiling C++: $<
	$(CPP) -c -mthumb $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(cpp_a_objects) : $(OUT_DIR)%_cpp.o : %.cpp
	@echo
	@echo Compiling C++: $<
	$(CPP) -c $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(cpp_ai_objects) : $(OUT_DIR)%_cpp.o : %.cpp
	@echo
	@echo Compiling C++: $<
	$(CPP) -c -mthumb-interwork $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS) $< -o $@ 
#	$(OBJDUMP) -h $@

$(cpp_t_objects) : $(OUT_DIR)%_cpp.o : %.cpp
	@echo
	@echo Compiling C++: $<
	$(CPP) -c -mthumb $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS)  $< -o $@ 
#	$(OBJDUMP) -h $@

$(cpp_ti_objects) : $(OUT_DIR)%_cpp.o : %.cpp
	@echo
	@echo Compiling C++: $<
	$(CPP) -c -mthumb -mthumb-interwork $(GCCFLAGS) $(CFLAGS) $(CPPFLAGS) $(GENDEPFLAGS) $< -o $@ 
#	$(OBJDUMP) -h $@

#copy headers
$(h_objects) : $(OUT_DIR)include/%.h :  %.h
	@echo
	@echo Copying headers: 
	cp $< $@

vpath %.h $(dir $(h_sources))


# Don't try to MAKE the following extentions:
%.dep %.mk %.c %.h %.S %.cpp:
	@echo TMOS make will ignore this target $@


# Include the dependency files.
ifneq "$(MAKECMDGOALS)" "clean"
-include $(dependencies)
endif
