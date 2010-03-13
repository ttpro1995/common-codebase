################################################################################
#
# (C) Copyright 2007
#    Panda Xiong, yaxi1984@gmail.com
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#
# History:
#    2007.03.27    Panda Xiong       Create
#
################################################################################

# add current directory to source and header file search path
USR_INC  += $(CURRENT_DIR)
SRC_DIR  += $(CURRENT_DIR)

# if no ASM object file assigned,
# it will auto scan .S source files, to get the corresponding object files.
ifeq (0,$(words $(A_OBJS)))
  ALL_A_SRCS := $(foreach dir,$(SRC_DIR),$(notdir $(wildcard $(dir)/*.S)))
  A_SRCS     := $(filter-out $(EXCLUDE_SRCS),$(ALL_A_SRCS))
  A_OBJS     := $(filter-out $(START_OBJ),$(patsubst %.S,%.o,$(filter %.S,$(A_SRCS))))
endif

# if no c/cpp/C object file assigned,
# it will auto scan c/cpp/C source files, to get the corresponding object files.
ifeq (0,$(words $(C_OBJS)))
  ALL_C_SRCS := $(foreach dir,$(SRC_DIR),$(notdir $(wildcard $(dir)/*.c)))    \
                $(foreach dir,$(SRC_DIR),$(notdir $(wildcard $(dir)/*.cpp)))  \
                $(foreach dir,$(SRC_DIR),$(notdir $(wildcard $(dir)/*.C)))
  C_SRCS     := $(filter-out $(EXCLUDE_SRCS),$(ALL_C_SRCS))
  C_OBJS     := $(patsubst %.c,%.o,$(filter %.c,$(C_SRCS)))                   \
                $(patsubst %.cpp,%.o,$(filter %.cpp,$(C_SRCS)))               \
			    $(patsubst %.C,%.o,$(filter %.C,$(C_SRCS)))
endif

ALL_OBJS_TMP := $(START_OBJ) $(A_OBJS) $(C_OBJS)
ALL_DEPS_TMP := $(ALL_OBJS_TMP:.o=.d)

LIB_TMP       := $(addprefix $(LIB_DIR)/,$(LIB))
LIB           := $(LIB_TMP)
ALL_OBJS      := $(addprefix $(OBJ_DIR)/,$(ALL_OBJS_TMP))
ALL_DEPS      := $(addprefix $(OBJ_DIR)/,$(ALL_DEPS_TMP))

################################################################################

# file searching path
vpath %.h   $(CURRENT_DIR) $(CURRENT_DIR)/inc $(CURRENT_DIR)/h
vpath %.hpp $(CURRENT_DIR) $(CURRENT_DIR)/inc $(CURRENT_DIR)/h
vpath %.c   $(CURRENT_DIR) $(SRC_DIR)
vpath %.C   $(CURRENT_DIR) $(SRC_DIR)
vpath %.cpp $(CURRENT_DIR) $(SRC_DIR)
vpath %.S   $(CURRENT_DIR) $(SRC_DIR)
vpath %.d   $(CURRENT_DIR) $(OBJ_DIR)
vpath %.o   $(CURRENT_DIR) $(OBJ_DIR)

################################################################################

# add all components to build
ALL_COMPONENTS    =

ifneq (0,$(words $(ALL_DEPS)))
  ALL_COMPONENTS += $(ALL_DEPS)
endif

ifneq (0,$(words $(ALL_OBJS)))
  ALL_COMPONENTS += $(ALL_OBJS)
endif

ifneq (0,$(words $(ALL_OBJS)))
 ifneq (0,$(words $(LIB)))
  ALL_COMPONENTS += $(LIB)
 endif
endif

#===============================================================================

.PHONY : all clean build_start build_end

#===============================================================================

all : build_start $(ALL_COMPONENTS) build_end
ifneq (0,$(words $(SUB_DIR)))
	@set -e;                                          \
	for i in $(SUB_DIR);                              \
	do                                                \
		$(MAKE) -f $(MAKEFILE_NAME) -C $$i $@;        \
	done
endif

#===============================================================================

clean :
ifneq (0,$(words $(ALL_OBJS)))
	-$(RM) -r $(OBJ_DIR)
endif
ifneq (0,$(words $(START_OBJ)))
	-$(RM) $(LIB_DIR)/start.o
endif
ifneq (0,$(words $(LIB)))
	-$(RM) $(LIB)
endif
ifneq (0,$(words $(SUB_DIR)))
	@set -e;                                          \
	for i in $(SUB_DIR);                              \
	do                                                \
		$(MAKE) -f $(MAKEFILE_NAME) -C $$i $@;        \
	done
endif

################################################################################

# update final start object file
$(LIB_DIR)/start.o : $(OBJ_DIR)/$(START_OBJ)
	$(CP) $< $@

ifneq (0,$(words $(START_OBJ)))
  FINAL_START_OBJ = $(LIB_DIR)/start.o
else
  FINAL_START_OBJ =
endif

#===============================================================================

build_start :
	-@

build_end : $(FINAL_START_OBJ)
	-@

#===============================================================================

# create the lib file
$(LIB) : $(filter-out $(OBJ_DIR)/$(START_OBJ),$(ALL_OBJS))
ifneq (0,$(words $(LIB)))
	-@$(MKDIR) $(LIB_DIR)
	$(AR) -crv $@ $^
endif

#===============================================================================

# auto create the depend file
$(OBJ_DIR)/%.d : %.S
	-@$(MKDIR) $(OBJ_DIR)
	@$(ECHO) Dependencies for $(notdir $<) ...
	@$(CC) -M $(AFLAGS) $< | sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d : ,g' > $@

$(OBJ_DIR)/%.d : %.c
	-@$(MKDIR) $(OBJ_DIR)
	@$(ECHO) Dependencies for $(notdir $<) ...
	@$(CC) -M $(CFLAGS) $< | sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d : ,g' > $@

$(OBJ_DIR)/%.d : %.C
	-@$(MKDIR) $(OBJ_DIR)
	@$(ECHO) Dependencies for $(notdir $<) ...
	@$(CXX) -M $(CFLAGS) $< | sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d : ,g' > $@

$(OBJ_DIR)/%.d : %.cpp
	-@$(MKDIR) $(OBJ_DIR)
	@$(ECHO) Dependencies for $(notdir $<) ...
	@$(CXX) -M $(CFLAGS) $< | sed 's,\($*\)\.o[ :]*,$(OBJ_DIR)/\1.o $(OBJ_DIR)/\1.d : ,g' > $@

#===============================================================================

# told compiler to use these rule to compile source files
$(OBJ_DIR)/%.o : %.S
	$(CC) -c $(AFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.C
	$(CXX) -c $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) -c $(CFLAGS) $< -o $@

################################################################################

# include depend files
ifneq (0,$(words $(ALL_DEPS)))
 ifneq (clean,$(MAKECMDGOALS))
  sinclude $(ALL_DEPS)
 endif
endif
