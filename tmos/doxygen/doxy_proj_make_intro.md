# Introduction to Make {#doxy_proj_make_intro}

[TOC]

Most of the commercial compilers have wizards and project managers to simplify 
the project management. The management process is usually very user friendly.

The developer can see all the project options in a GUI, to change them by mouse 
clicks, to see all the source files etc.

Although this approach looks quite sexy, there are some problems. The project 
options are usually fixed and they differ from vendor to vendor. It is very 
difficult if not impossible to have single project working with two different 
compilers or two different targets.

From other side the native GNU projects usually are based on a standard makefile 
which allows one project to be compiled in different platforms for different 
targets. Although the MAKE is very powerful tool, it requires certain knowledge 
on how to write makefiles. For complex projects writing and supporting the make 
process can be quite time consuming task. Therefore for some projects like eCos 
for example there are special configuration tools which can generate 
automatically different makefiles depending on the user requirements.

At this time there is no configuration tool for TMOS, but we have developed 
general concept how to organize projects that include TMOS. This concept however
is independent from the operating system itself and can be used for any custom 
project.


## Concept ideas
In the first versions of Make it was assumed that all the source files are stored 
in a single directory. Nowadays many projects become big and it is not convenient
to have thousands of source files and object files in a single directory. 
Especially if you have user code and operating system. To separate the parts of 
the project it is more appropriate to have different folders for the different 
parts or modules.

Probably for historical reasons many projects use so called *recursive make* in 
which Make is called from the root directory and it calls itself recursively for 
any subdirectory. 

There is little bit less popular approach called **nonrecursive make** in which the 
different modules are processed all together not separately.

The difference between recursive and nonrecursive make are described in this paper:
http://aegis.sourceforge.net/auug97.pdf

The recursive make has no advantages only drawbacks and that's why the nonrecursive
approach was chosen for TMOS.

The operating system has several modules and the user can add more modules. All 
modules and sub-modules are placed as separate subdirectories in the project 
directory. The project directory should contain no source just subdirectories and 
the makefile.

The makefile contains all general rules required for the compilation, linking etc
as well as a list of the top level modules. Then, each module should provide an 
include file to specify its source files and sub-modules if any.

The idea is that the project makefile is complex but usually should not be modified. 
The developers should deal only with very simple include files.


## The Make
Make is a GNU tool which can be executed from the command prompt with or without
parameters. Make searches for a makefile in order to execute all the magical tasks 
inside. In fact it is something like a script language which tells when and which
tool to be used and for what purpose.

The most important terms are:
* variables
* macros and functions
* rules
* commands

### Variables
Variables can hold any text. In many cases the text strings are considered as list 
of words separated by whitespaces.

The variables can be used in expression with the following notation: **$(variable_name)**

Make can use all the environment variables.


There two general type of variables - *simple* and *recursive*.


Simple variable is created with the assignment operator ":=". For example:

~~~~~~~~~~~~~~~{.}
my_name := $(name)
~~~~~~~~~~~~~~~

In this case the variable my_name is simple, because it is assigned with the value 
of the right-side expression. So if you know the current value of the variable 
"name" you will know the value of the variable "my_name". Simple isn't it?

Macros and functions
You may have noticed that the recursive variables are something like macros.
Recursive variables are defined like this:
~~~~~~~~~~~~~~~{makefile}
my_name = $(name)
~~~~~~~~~~~~~~~

*What is the difference?*

Well, in the recursive case the right-side expression is not calculated immediately.
Instead, the Make remembers the whole expression as it is. Latter, if the value of
"my_name" is needed then the expression gets evaluated. At the time when a recursive
variable is first assigned you can't say what the value really is. It could change 
many times during the processing of the makefile is something in the right-side
gets changed. So the things aren't that simple ;-)


### Macros and functions

You may have noticed that the recursive variables are something like macros.


Another way to define a macros is:
~~~~~~~~~~~~~~~{.}
    define macro_name
        something e.g. command
        something else
        ...
    endef
~~~~~~~~~~~~~~~

There is just one difference between a recursive variable and macro which is the 
fact that the macro expression can be written on multiple lines. This is important 
when you need to remember a sequence of commands because each command needs to 
start on a new line.

The programmers will say that macros are useless without parameters. You can have
parameters. In the body you can use $1, $2, ... and to pass the parameters you can use:

~~~~~~~~~~~~~~~{.}
$(call macro_name[, param1...])
~~~~~~~~~~~~~~~

Now about the functions. Make has system function which can be used like this:
~~~~~~~~~~~~~~~{.}
$(function-name arg1[,arg2...])
~~~~~~~~~~~~~~~

For example:
~~~~~~~~~~~~~~~{.}
        listings = $(subst .o,.lst,$(objects))
~~~~~~~~~~~~~~~


The subst function replaces all occurrences of the first parameter with the second
parameter in the expression given by the third one. So in this case if $(objects) 
is "file1.o file2.o file3.o" the result will be a list of listing files.

**Note:** *one space is required after the function name, any other space will be
considered as part of the parameters and will lead to undesired results. 
The parameters should be separated only with commas and no whitespaces.*

We will not consider here the build-in Make functions - refer to Make manual for details.


### Rules
You call Make for some good reason, and usually you specify the reason in the command
line like this:
~~~~~~~~~~~~~~~{.}
make clean
~~~~~~~~~~~~~~~

or
~~~~~~~~~~~~~~~{.}
make all
~~~~~~~~~~~~~~~

Where "clean" and "all" are said to be targets and you want these targets to be 
done. The Make alone has no idea how to clean your project or how to build it. 
That's why the makefile is required to give the rules how one or another target 
to be made. 

You can describe the rules in the following format:
~~~~~~~~~~~~~~~{.}
Target: prerequisity1 prerequisity2 [...]
    Command1
    Command2
    [...]
~~~~~~~~~~~~~~~
    
    
This rule says that the Target must be updated by executing the listed commands
whenever the prerequisites are updated. The rule also says that if necessary the 
listed perquisites must be updated before updating the target.

Btw the rule does not have to be written at once. It can be like this:
~~~~~~~~~~~~~~~{.}
Target: prerequisity1
[other rules or expressions]
Target: prerequisity2
    Command1
    Command2
~~~~~~~~~~~~~~~

**Note:** *The first rule written in the makefile is considered as default target, 
e.g. calling Make with no parameters will update it. That's why usually in the 
beginning you can see "all:" with no prerequisites.*

**Note:** *The tabulation is important*

The rules can be decomposed in pair-wise dependencies where the left side depends
on the right side. The prerequisities on the right side can depend on other
prerequisity. Make determens how to build everything by building a directed 
acyclic graph (DAG) from all the rules. In the next step Make examens each item 
in the DAG (usually from left-to-right and from bottom-to-top) and if the item 
is outdated it will execute the commands required to update.

Usually, the DAG items are files unless they are declared as ".PHONY".

### Pattern rules

You can use patterns to write rules. For example:
~~~~~~~~~~~~~~~{.}
# Create final output file (.bin) from ELF output file.
%.bin: %.elf
    @echo
    @echo Creating load file for Flash: $@
    $(OBJCOPY) -O $(FORMAT) $< $@
~~~~~~~~~~~~~~~
        
This rule says that any target with ".bin" extention depends on the same file 
with ".elf" extention. With other words to create a bin file we need the corresponding
elf first. And in order to create the bin file, the Make should call the commands 
after the rule.

In the pattern rules we can use some automatic variables. Here is short info:
* $@    - the name representing the target (e.g. image.bin)
* $<    - represents the first prerequisite
* $?    - list of all prerequisites that are newer than the target, separated by spaces
* $^    - list of all prerequisites, separated by spaces. The duplicates are removed.
* $+    - list of all prerequisites
* $*    - represents the stem of that matched to the %-part of the rule


### Multiple targets/rules
It was mentioned already that you can write multiple rules for one target. After the
processing all the prediquistes are merged together and a single rule for that target is
constructed. In this case however there could be only one set of commands for the target.

It is also possible to multiple targets in a rule. This is equivalent to write many identical
rules, each one with one target.

If you have a pattern rule for multiple targets you can use also static pattern rules.
These are pattern rules that can be applied only to a certain list of targets. The list is
written first and separated with colon from the rest of the rule. For example the following
rule applies only to the files listed in the $(cobjects) variable:
~~~~~~~~~~~~~~~{.}
$(cobjects) : $(BIN_DIR)%.o : %.c
    @echo
    @echo Compiling C: $<
~~~~~~~~~~~~~~~

### Commands
In the rows after a rule you can write shell commands. The tabulation is important
because it tells Make which line contains rule and wich command.




