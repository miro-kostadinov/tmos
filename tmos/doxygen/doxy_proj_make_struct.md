# TMOS project structure {#doxy_proj_make_struct}


## Important rules:
* **Avoid spaces in the filenames or project directories**
* **Remember that the tools using directory and filenames are case sensitive**
* **Avoid names starting with "." unless you know what you are doing**

### Directory structure
At the top level we have a work directory. We mention it because Eclipse requires
a workspace directory. In the work directory you can have one or more project
directories.

The project directory is the most important because all paths are relative to this path.
No source files should be placed in the project directory, only the project makefile.
The source tree should be organized in subdirectories which we call modules.


Example:
~~~~~~~~~~~~~~~{.}
C:\WORK                 - lets assume this is our work directory
C:\WORK\MYPROJ          - project directory
C:\WORK\MYPROJ\TMOS     - tmos module
C:\WORK\MYPROJ\APP      - application module
~~~~~~~~~~~~~~~



### Make files
The project makefile located in the project directory contains the general rules how to
compile or clean the whole project. However it does not list any particular source file.
Instead there is a list of the top level software modules. Each top level module is
actually a subdirectory and it is assumed that each module contains module make file
-"module.mk". There is code in the project makefile that includes inside all module
make files.

The module-make files enlist the source files and eventually any sub-modules. Note
that this is not a recursive make process and the modules are not built one by one -
instead all make files are combined together and single dependency tree is build for
the whole project. So you don't have to worry if you have common sources used in
several modules or if changes in one module requires recompilation of another. You
don't have to worry about this - Make is an expert system and it works fine when
using such a non-recursive process.

It is possible that you may have to use existing sources which are designed to use
recursive make. There is no problem - you can still call recursively Make to build
such modules. Just make sure they are compiled with compatible options so they can
be linked together with the rest of the project. Of course it is preferable to avoid the
recursive process and to try to adapt all the sources to a single make project.


### Project or Subproject, module or submodule

TMOS contains almost everything required for a sample application, except the 
configuration and the application code of course. You can place your code inside
the TMOS project or you can create your own project.

In the first scenario there will be only a single project. Everything will be
compiled in a signle pass and the output will be hex binary files. No subprojects here.

In the second scenario TMOS more likely will be compiled separately as a library,
which you can link to your project.
   