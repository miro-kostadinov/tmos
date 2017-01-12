
#===============	            PARAMS                      	===============#
SVN			 ?= svn
CVS			 ?= cvs
COUNTER_FILE ?= versions.h 
TARGET_VER_FILE ?= targets/$(CFG_TARGET)/versions.h 

#===============	            MACROS                      	===============#
#repository command
REP := $(if $(wildcard CVS),$(CVS),$(SVN))

#-------------------------------------------------------------------------------
#						FILE_ITEM()
#						------------
# Reads item from a file
# $(call FILE_ITEM, file, item)
define FILE_ITEM
$(lastword $(shell if test -f $1;then cat $1 |grep $2;fi))
endef
#-------------------------------------------------------------------------------
#						REP_VER()
#						------------
# Reads the last revision from a repository path
# $(call REP_VER, rep_path)
define REP_VER
$(lastword $(shell $(REP) info $(REP_ROOT)$(1)|grep "Last Changed Rev:"))
endef

#-------------------------------------------------------------------------------
#						COMPUTE()
#						------------
# $(call COMPUTE, expression)
define COMPUTE
$(shell echo $$(($(1))))
endef

#-------------------------------------------------------------------------------
#						COMPARE()
#						------------
# $(call COMPARE, a, b)
define COMPARE
$(shell if test "$1" = "$2" ;then echo tr;fi )
endef

#-------------------------------------------------------------------------------
#						DIFRENT()
#						------------
# $(call DIFRENT, a, b)
define DIFRENT
$(shell if test "$1" != "$2" ;then echo $3;fi )
endef

#-------------------------------------------------------------------------------
#						CHECK_TAG()
#						------------
# $(call CHECK_TAG, projetc)
define CHECK_TAG
$(call DIFRENT,$(call GET_VERSION,$1),$(call FILE_ITEM,$(TARGET_VER_FILE),$1),$1)
endef


#-------------------------------------------------------------------------------
#						GET_VERSION()
#						------------
# $(call GET_VERSION, project)
define GET_VERSION
$(call FILE_ITEM,$1/$(COUNTER_FILE),VERSION)
endef

#-------------------------------------------------------------------------------
#						PRINT_VERSIONS()
#						------------
# $(call PRINT_VERSIONS)
define PRINT_VERSIONS
$(shell echo TIME $(TIME) > $(COUNTER_FILE);\
echo BUILD $(BUILD) >> $(COUNTER_FILE);\
echo INTERMEDIATE $(INTERMEDIATE) >> $(COUNTER_FILE);\
echo VERSION $(VERSION).$(INTERMEDIATE) >> $(COUNTER_FILE))
endef

#-------------------------------------------------------------------------------
#						PRINT_TARG_VER()
#						------------
# $(call PRINT_TARG_VER)
define PRINT_TARG_VER
$(shell echo DEV_VER $(DEV_VERSION) > $(TARGET_VER_FILE);)
endef

#-------------------------------------------------------------------------------
#						PRINT_TARG_VER2()
#						------------
# $(call PRINT_TARG_VER2)
define PRINT_TARG_VER2
$(foreach i,$(DEP_PROJECTS),$(shell echo $(i) $(call GET_VERSION,$(i))>> $(TARGET_VER_FILE)))
endef

REP_INFO := $(if $(filter all,$(MAKECMDGOALS))$(filter clean,$(MAKECMDGOALS)),,$(shell $(REP) info >$(OUT_DIR)/svn.info))
REP_ROOT := $(lastword $(shell grep "Repository Root:" $(OUT_DIR)/svn.info))
REP_URL := $(lastword $(shell grep "URL:" $(OUT_DIR)/svn.info))
REP_FOLDER := $(lastword $(subst /, ,$(REP_URL)))

TAG_REQUESTED := $(filter tag,$(MAKECMDGOALS))$(filter release,$(MAKECMDGOALS))
TAG_THIS := $(if $(filter all,$(MAKECMDGOALS))$(filter clean,$(MAKECMDGOALS)),\
,$(shell if test $(call REP_VER,/trunk/$(REP_FOLDER)) -gt $(call REP_VER,/tags/$(REP_FOLDER)); then echo do; fi))
TAG_DEP_PROJ :=$(strip $(foreach i,$(DEP_PROJECTS),$(call CHECK_TAG,$(i))))
DO_TAG :=$(if $(TAG_REQUESTED),$(if $(TAG_THIS)$(TAG_DEP_PROJ),do,),$(if $(filter test-release,$(MAKECMDGOALS)),y,))

F_TIME :=$(call FILE_ITEM,$(COUNTER_FILE),TIME)
F_BUILD :=$(call FILE_ITEM,$(COUNTER_FILE),BUILD)
F_VERSION :=$(call FILE_ITEM,$(COUNTER_FILE),VERSION)
F_INTERMEDIATE :=$(call FILE_ITEM,$(COUNTER_FILE),INTERMEDIATE)

C_TIME :=$(call COMPUTE, $(shell date +'%y')%10)$(shell date +'.%m')

ifeq ($(DO_TAG), do)
#new release
TIME := $(C_TIME)
BUILD := $(if $(call COMPARE,$(F_TIME),$(C_TIME)),$(call COMPUTE,$(F_BUILD)+1),0)
INTERMEDIATE := 0
else
#not a release 
TIME := $(if $(F_TIME),$(F_TIME),$(C_TIME))
BUILD := $(if $(F_BUILD),$(F_BUILD),0)
INTERMEDIATE := $(if $(filter test-release,$(MAKECMDGOALS)),$(call COMPUTE,$(F_INTERMEDIATE)+1),$(F_INTERMEDIATE))
endif

VERSION := $(TIME).$(BUILD)
REP_VERSION := $(VERSION).$(INTERMEDIATE)
LOG := ver $(REP_VERSION) $(OUT_NAME)

# generate version string
COUNTER_FILES := $(COUNTER_FILE) $(if $(DEP_PROJECTS),$(TARGET_VER_FILE),)
DO_PACKAGE :=

ifneq ($(BUILD_LIB),y)
DEV_VERSION :=$(call FILE_ITEM,$(TARGET_VER_FILE),DEV_VER)
objects += $(OUT_DIR)version.op

DO_PACKAGE := package

$(OUT_DIR)version.op: $(COUNTER_FILES) 
	@echo
	@echo Writing version object file
	@echo -n $(DEV_VERSION)$(VERSION)$(if $(INTERMEDIATE),.$(INTERMEDIATE),)> $(OUT_DIR)version.bin
	$(OBJCOPY) -B arm -I binary -O elf32-littlearm --rename-section .data=.version $(OUT_DIR)version.bin $@ 

$(TARGET_VER_FILE):
	@if ! test -f $(TARGET_VER_FILE); then \
	echo ERROR: missing target version file: $(TARGET_VER_FILE); \
	exit 1; fi
endif


#===============	            rules	                    	===============#
.PHONY: release force-release sync package tag trace test-release check_counters



# Release
release: sync check_counters all $(DO_PACKAGE) tag  

force-release: check_counters all $(DO_PACKAGE)   

test-release: sync check_counters all $(DO_PACKAGE) tag

#sync
sync: $(COUNTER_FILES)
	@echo
	@echo Updating from the repository...
	$(REP) update
	@if $(REP) status | grep "M"; then echo Synchronize first!; exit 1 ; fi
	
$(COUNTER_FILE):
	@if ! test -f $(COUNTER_FILE); then echo creating file: $(COUNTER_FILE); \
	echo "$(PRINT_VERSIONS)";\
	svn add $(COUNTER_FILE) ; fi
	
check_counters: 	
	@if [ -n "$(DO_TAG)" ] ; then \
	echo ;echo Updating version files: $(COUNTER_FILES); \
	echo "$(PRINT_VERSIONS)";\
	echo "$(PRINT_TARG_VER)";\
	echo "$(PRINT_TARG_VER2)"; fi
	
package:
	@echo
	@echo Packaging....
	@echo fw_pkg $(PROJECT)
	
	
tag: $(COUNTER_FILES)
	@echo
	@echo  Checking $(REP_FOLDER) repository ... 
	if [ -n "$(DO_TAG)" ] ; then \
	echo Tagging: $(F_VERSION) "->" $(REP_VERSION); \
	echo "$(PRINT_VERSIONS)";\
	echo "$(PRINT_TARG_VER)";\
	echo "$(PRINT_TARG_VER2)";\
	svn commit -m "$(LOG)" $(COUNTER_FILES); \
	svn copy -m "$(LOG)" . $(REP_ROOT)/tags/$(REP_FOLDER)/$(REP_VERSION); fi

postbuild += trace
prebuild += $(COUNTER_FILES)

trace:
	@echo 
	@echo Project: $(REP_FOLDER) version: $(DEV_VERSION)$(VERSION).$(INTERMEDIATE) 
	