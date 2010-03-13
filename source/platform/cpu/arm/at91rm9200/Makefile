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

include $(CFG_DIR)/config.mk

#===============================================================================

LIB	      := libcpu.a

#===============================================================================

SRC_DIR   := $(CURRENT_DIR)/src

#===============================================================================

# User options and include path
USER_OPTS  =
USR_INC    =

#===============================================================================

# only need to add asm file here, the c file will auto be searched.
START_OBJ := $(PROJECT_NAME)_start.o
A_OBJS    := 
C_OBJS    :=

ifeq (loader,$(PROJECT_NAME))
  A_OBJS  += lowlevel_init.o
endif
ifeq (kernel,$(PROJECT_NAME))
  A_OBJS  += cpu_isr.o
endif

#===============================================================================

include $(CFG_DIR)/make_target.mk

################################################################################
