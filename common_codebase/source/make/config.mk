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

# set makefile name
MAKEFILE_NAME = Makefile

#===============================================================================

EMPTY         :=
SPACE         := $(empty) $(empty)
DEAL_DRIVE     = $(strip $(subst /,$(SPACE),$(subst :/,/,$(subst \,/,$(1)))))
GET_ONLY_DRIVE = $(word 1,$(call DEAL_DRIVE,$(1)))
GET_ONLY_DIR   = $(subst $(SPACE),/,$(SPACE)$(wordlist 2,$(words $(call DEAL_DRIVE,$(1))),$(call DEAL_DRIVE,$(1))))

CURRENT_DRIVE    := $(call GET_ONLY_DRIVE,$(shell pwd))
CURRENT_REAL_DIR := $(addprefix $(CURRENT_DRIVE):,$(call GET_ONLY_DIR,$(shell pwd)))

# final target base directory
TARGET_BASE_DIR := $(PROJECT_DIR)/build/$(PROJECT_NAME)

LIB_DIR     := $(TARGET_BASE_DIR)/lib
IMAGE_DIR   := $(TARGET_BASE_DIR)/image

ifeq (1,0)

# current directory
CURRENT_DIR := .

# all objects path
OBJ_DIR     := $(CURRENT_DIR)/o

else

# current directory
CURRENT_DIR := $(CURRENT_REAL_DIR)

# all objects path
OBJ_DIR     := $(TARGET_BASE_DIR)/obj$(subst $(PROJECT_DIR),,$(CURRENT_REAL_DIR))

endif

#===============================================================================

include $(PROJECT_DIR)/make/target.mk

#===============================================================================

ECHO    = echo
CP      = cp
RM      = rm -f
MKDIR	= mkdir -p
MAKE    = make

#===============================================================================

# USER_OPTS : user options and include path
#USER_OPTS  =
#USR_INC    =

# module independence include path, this will send to all sub-directories
#MODULE_INC =

# image options
IMAGE_OPTS = -D__LOADER_TEXT_BASE__=$(LOADER_TEXTBASE)                \
             -D__KERNEL_TEXT_BASE__=$(KERNEL_TEXTBASE)

# basic include path
BASIC_INC  = $(PROJECT_DIR)/common                                    \
             $(PROJECT_DIR)/app                                       \
             $(PROJECT_DIR)/platform/cpu/$(CPU_ARCH)/$(CPU_TYPE)      \
			 $(PROJECT_DIR)/platform/cpu/$(CPU_ARCH)/lib              \
			 $(PROJECT_DIR)/platform/fs                               \
			 $(PROJECT_DIR)/platform/driver                           \
			 $(PROJECT_DIR)/platform                                  \
			 $(PROJECT_DIR)/include                                   \
			 $(PROJECT_DIR)/$(PROJECT_NAME)                           \
			 $(INC_GCC_DIR)

# all include path
ALL_INC	= $(BASIC_INC) $(USR_INC) $(MODULE_INC)

#===============================================================================

# ASM options used by gcc
AFLAGS  = $(CPU_OPTS) $(IMAGE_OPTS) $(USER_OPTS) $(addprefix -I,$(sort $(ALL_INC)))

# options used by gcc
CFLAGS	= $(CPU_OPTS) $(IMAGE_OPTS) $(USER_OPTS) $(addprefix -I,$(sort $(ALL_INC)))

#===============================================================================
