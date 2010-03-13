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


# define thumb code for ARM cpu
ifeq (arm,$(CPU_ARCH))
  ifeq (loader,$(PROJECT_NAME))
    USE_THUMB_CODE = 0
  else
  ifeq (kernel,$(PROJECT_NAME))
    USE_THUMB_CODE = 0
  endif
  endif
else
  USE_THUMB_CODE = 0
endif

#===============================================================================

# Cross compiler define
CROSS_COMPILE =
CC        = $(CROSS_COMPILE)gcc
CXX       = $(CROSS_COMPILE)g++
LD        = $(CROSS_COMPILE)ld
AR        = $(CROSS_COMPILE)ar
AS        = $(CROSS_COMPILE)as
RANLIB    = $(CROSS_COMPILE)ranlib
NM        = $(CROSS_COMPILE)nm
OBJCOPY   = $(CROSS_COMPILE)objcopy
OBJDUMP   = $(CROSS_COMPILE)objdump
STRIP     = $(CROSS_COMPILE)strip
SIZE      = $(CROSS_COMPILE)size

#===============================================================================

ifeq (,$(CPU_ARCH))
  $(error "Unknown CPU Type!")
endif

CPU_OPTS  := -g

ifeq (little,$(TARGET_ENDIAN))
  CPU_OPTS += -D__LITTLE_ENDIAN__
else
  CPU_OPTS += -D__BIG_ENDIAN__
endif

# for arm CPU
ifeq (arm,$(CPU_ARCH))
  ifeq (1,$(USE_THUMB_CODE))
    CPU_OPTS += -Os -s -mthumb -D__THUMB_C__
  else
    CPU_OPTS += -O2
  endif

  CPU_OPTS += -mthumb-interwork -m$(TARGET_ENDIAN)-endian
endif

# for mips CPU
ifeq (mips,$(CPU_ARCH))
  CPU_OPTS += -O2

  ifeq (little,$(TARGET_ENDIAN))
    CPU_OPTS += -mel -DMIPSEL
  else
    CPU_OPTS += -meb -DMIPSEB
  endif

  CPU_OPTS   += -mips2 -DCPU=MIPS32 -DSOFT_FLOAT
endif

CPU_OPTS += -D__$(CPU_ARCH)__ -msoft-float                                   \
		    -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef       \
		    -fsigned-char -funsigned-bitfields -fshort-enums

ifeq (loader,$(PROJECT_NAME))
  CPU_OPTS += -D__TARGET_LOADER__
  TARGET_TEXTBASE = $(LOADER_TEXTBASE)
else
ifeq (kernel,$(PROJECT_NAME))
  CPU_OPTS += -D__TARGET_KERNEL__
  TARGET_TEXTBASE = $(KERNEL_TEXTBASE)
endif
endif

#===============================================================================
