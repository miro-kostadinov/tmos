# How makefile works (step by step) {#doxy_proj_make_how}

Let's assume that Make is called from the command shell with "make" or "make all".

So we jump to the rule for "all". The first rule for all is:
~~~~~~~~~~~~~~~{.}
# default goal is "all" - set it before include files
all:
~~~~~~~~~~~~~~~

We can ignore this because it makes "all" the default target and nothing more. 
The real rule is:
~~~~~~~~~~~~~~~{.}
all: begin sizebefore $(prebuild) build $(postbuild) sizeafter end
~~~~~~~~~~~~~~~

This rule says that in order to make "all" we have to make firs "begin" and so forth. The
variables $(prebuild) and $(postbuild) are empty by default so usually they are replaced
with empty string so we can ignore them now. Note if you ever want to add actions
before or after the real build you can modify these variables in any of the module files.
The targets "begin", "end", "sizebefore", "sizeafter" are displaying messages to make the
console output pretty so we ignore them as well.

The real stuff is in the "build" so we look for rules about it. Actually there two different
rules for "build" and they are conditional:

~~~~~~~~~~~~~~~{.}
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
~~~~~~~~~~~~~~~

Depending on the value of $(BUILD_HEX), $(BUILD_BIN) and $(BUILD_LIB) variables 
we can have different outputs and we have prerequisities like "hex" or
"bin" . In both cases we have first "elf".

Let's find the rule for "elf":
~~~~~~~~~~~~~~~{.}
elf: $(PROJECT).elf
~~~~~~~~~~~~~~~
The $(PROJECT) variable is actually a constant that holds the output filenames for the
project without the extension. 
~~~~~~~~~~~~~~~{.}
PROJECT = $(OUT_DIR)$(OUT_NAME)
~~~~~~~~~~~~~~~

When we replace everything  we will see that "elf" requires something like
"out/board_name.elf". This prerequisite is actually a filename. Note that all targets we
stepped into so far were .PHONY targets (not real files).

Obviously, we need to create this elf file but Make is not that smart so it will
search for a rule involving this name. And it will find the following rule:
~~~~~~~~~~~~~~~{.}
#make ELF
%.elf:  $(objects) $(libraries) 
    @echo
    @echo Linking: $@
    $(CC) -mthumb-interwork $(GCCFLAGS) $(objects) --output $@ $(LDFLAGS)
~~~~~~~~~~~~~~~

This rule says that to create some "elf" file we need to check some objects and
libraries first and then we have some commands that eventually should make it. 
Where the objects come from?

~~~~~~~~~~~~~~~{.}
objects         =$(all_as_objects) $(all_c_objects) $(all_cpp_objects)
~~~~~~~~~~~~~~~

Well, it seems that "objects" is a set of other objects and we look for them we will see:

~~~~~~~~~~~~~~~{.}
as_objects      += $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_sources))
as_a_objects    += $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_a_sources))
as_ai_objects   += $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_ai_sources))
as_t_objects    += $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_t_sources))
as_ti_objects   += $(patsubst %.S,$(OUT_DIR)%_S.o,$(as_ti_sources))
all_as_objects  += $(as_objects) $(as_a_objects) $(as_ai_objects) $(as_t_objects) $(as_ti_objects)

c_objects       +=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_sources))
c_a_objects     +=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_a_sources))
c_ai_objects    +=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_ai_sources))
c_t_objects     +=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_t_sources))
c_ti_objects    +=$(patsubst %.c,$(OUT_DIR)%_c.o,$(c_ti_sources))
all_c_objects   +=$(c_a_objects) $(c_ai_objects) $(c_t_objects) $(c_ti_objects) $(c_objects)

cpp_objects     +=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_sources))
cpp_a_objects   +=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_a_sources))
cpp_ai_objects  +=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_ai_sources))
cpp_t_objects   +=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_t_sources))
cpp_ti_objects  +=$(patsubst %.cpp,$(OUT_DIR)%_cpp.o,$(cpp_ti_sources))
all_cpp_objects +=$(cpp_a_objects) $(cpp_ai_objects) $(cpp_t_objects) $(cpp_ti_objects) $(cpp_objects)
~~~~~~~~~~~~~~~


Each of the variables here is set to the result of a pattern substitution, where from some
"sources" we change the extension and the path. For example the "c_sources" will be
scanned with the pattern "%.c" and the matches will be substituted with
"$(OUT_DIR)%_c.o". So if we have "app/main.c" it will become "out/board_name/app/main_c.o".
The idea is obvious - we create a list with all object files that will be created from various
source files. But where we specify the sources? In the beginning of the make file we have
only this:
~~~~~~~~~~~~~~~{.}
as_sources      :=
as_a_sources    :=
as_ai_sources   :=
as_t_sources    :=
as_ti_sources   :=

c_sources       :=
c_a_sources     :=
c_ai_sources    :=
c_t_sources     :=
c_ti_sources    :=

cpp_sources     :=
cpp_a_sources   :=
cpp_ai_sources  :=
cpp_t_sources   :=
cpp_ti_sources  :=
~~~~~~~~~~~~~~~

No source files ?!

Yes, no sources in the makefile. For the sources we need to look in the modules. For
example (hardware/module.mk):

~~~~~~~~~~~~~~~{.}
# local sources
local_as_src-y  :=
local_c_src-y   :=
local_cpp_src-y :=
local_h_src-y   :=

local_h_src-y   += hardware_cfg.h hardware_inc.h hardware_cpp.h 

#updating global variables
as_sources  += $(call changepath,$(local_as_src-y))
c_sources   += $(call changepath,$(local_c_src-y))
cpp_sources += $(call changepath,$(local_cpp_src-y))
h_sources   += $(call changepath,$(local_h_src-y))
~~~~~~~~~~~~~~~

In each module we have "local" variables. Note that these are simple variables so we can
use the same variable names in many modules without conflicts. So in every module we
create a temporary simple variable for each source type and then we just enlist all local source
files from this type. In the example actualy there are only few headers, but the
idea is the same.

Why we have different types of sources? 

Well, some sources are in assembler, some in C or C++.
Also, for some reasons under GCC you can't mix ARM and Thumb code in one source
file. To make the things even more complicated, you can't mix inter-working with non-
inter-working stuff. That's why we end up with many types of sources...

You create each list with the filenames only, but actually the relative path from the
project directory is required. But you don't have to write paths - there is a function
changepath() that does the magic for us.

You can see the function code in the project makefile. This a really simple function 
it adds $(subdirectory) to any word in the list supplied. Btw the subdirectory is a
variable that holds the relative path to the current module and can be
used whenever you need the current path in some module file.

It may look too complicated, but you don't have to know all this. All you need to know is
how to add or remove source files in the project. This can be done by listing them in the
corresponding "local source" variable.

From there the local variable is added to the corresponding global "source variable".
From the sources a single list of "objects" is collected and as we saw this list will be
required for:
~~~~~~~~~~~~~~~{.}
%.elf:  $(objects) $(libraries) 
~~~~~~~~~~~~~~~

OK, now we know that in order to build the elf file, Make will search for a list of object
files. If there are such files everything will be fine, but in a clean project these objects
should be created first. To do so, Make will search for rules and there are such rules for
every type of source. For example this is the rule for the c_objects:
~~~~~~~~~~~~~~~{.}
# ====-== Compile: create object files from C source files. ARM/Thumb
$(c_objects) : $(OUT_DIR)%_c.o : %.c
    @echo
    @echo Compiling C: $<
    $(CC) -c $(GCCFLAGS) $(CFLAGS) $(CONLYFLAGS) $(GENDEPFLAGS)  $< -o $@ 
~~~~~~~~~~~~~~~

The rule says that the object file depends on the source file and if the object is missing or
outdated, the commands after the rule should be executed.

The first two commands are just for the display. The $(CC) in the last command is the
command that invokes GCC and we have some flags telling the compiler what to do.

Then we have "$<" which is expanded with the first prerequisite and in our case this is
the source file name. So GCC will be called to compile the source file and to produce
output file (-o} with the same name as the target of this rule ( $@).

In fact the above commands expand to something like this:

~~~~~~~~~~~~~~~{.}
Compiling C: ports/crc32.c
arm-none-eabi-gcc -c -mcpu=cortex-m3 -mthumb -g -g3 -O0 -fomit-frame-pointer -fverbose-asm -ffunction-sections -fdata-sections -fno-dwarf2-cfi-asm  -Iboards/rpr250/ -Ihardware/ -Ihardware/stm32/ -Ihardware/stm32/stm32f2/ -Ihardware/stm32/stm32f2/STM32F205ZC/ -Ihardware/cortex-m3/ -Iports/ -Iports/cortex-m3/ -Idrivers/ -Idrivers/stm32/ -Iservices/USB/ -Iservices/USB/ports/stm32/ -Iservices/tmos_tl_v2/ -Wall -Wcast-align   -Wpointer-arith -Wswitch -Wredundant-decls -Wreturn-type -Wshadow -Wunused -Wa,-adhlns=out/rpr250/ports/crc32_c.lst -Wnested-externs -funsigned-char -Wstrict-prototypes -std=gnu99 -MD -MP -MF out/rpr250/ports/crc32_c.o.dep  ports/crc32.c -o out/rpr250/ports/crc32_c.o 

~~~~~~~~~~~~~~~

### Few words about the commands (binutils)

There are many binutils - different compilers, assemblers, linkers, library tools and so on.
For a specific platform they usually have some prefix - for example "arm-elf-" or "arm-none-eabi-" 
are prefixes for the ARM platform. So the arm assembler will be "arm-elf-as" or "arm-none-eabi-as".

Note however that we do not call directly the assembler or the linker. Instead we always call
arm-xxx-gcc. The reason for this is that GCC is smart enough and can call several tools
when this is required. For example when we call GCC for an assembler source it first
calls the preprocessor then with the result calls the assembler and it call also create
dependency file. So one GCC call replaces several other calls.

### Flags
GCC has many flags. For details on a specific flag/option check the corresponding
manual. Here we will consider only that there are several variables to collect various
falgs:

* GCCFLAGS - this variable is set with generic flags that we pass to *any* call to GCC.

* CFLAGS - these are GCC flags when it is called to compile C/C++ sources. There are
separate variables for specific C-only or C++-only flags.

* ASFLAGS - Flags used when GCC is called for assembling. The "-x assembler-with-
cpp" instructs GCC that the input language is assembly and preprocessor must be used.

* LDFLAGS - GCC flags for linking.

* GENDEPFLAGS - flags to generate dependency files.

### More about dependencies

In the assembler or C/C++ you can use #include. This means that the source file will
depend on some h-files. Of course if you change the header file the source must be
recompiled. One way to achieve this is to add the h-files as prerequisites in the rules. But
this is not quite practical.

Fortunately, GCC can generate the required rules automatically. With the flags "-MD -
MP -MF $@.dep" we instruct GCC to do so and save these rules in a separate file.
In the very end of the project file we include these dependency files.
