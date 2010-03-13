###############################################################################
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
###############################################################################


# get all lib file name in LIB_DIR, and add USR_LIBS
ALL_TARGET_LIBS := $(wildcard $(LIB_DIR)/*.a)
ALL_LINK_LIBS   := $(ALL_TARGET_LIBS) $(USR_LIBS)
TMP_VAL1        := $(notdir $(basename $(ALL_LINK_LIBS)))
ALL_LIBS        := $(sort $(subst lib,,$(TMP_VAL1)))

###############################################################################

ifeq (big,$(TARGET_ENDIAN))
  LD_FLAGS_ENDIAN = -EB
else
ifeq (little,$(TARGET_ENDIAN))
  LD_FLAGS_ENDIAN = -EL
endif
endif

# flags for linker
LD_FLAGS := -e _start -Ttext $(TARGET_TEXTBASE)                               \
            $(LD_FLAGS_ENDIAN) --gc-sections                                  \
            --start-group                                                     \
              $(LIB_DIR)/start.o                                              \
		      $(addprefix -l,$(ALL_LIBS))                                     \
            --end-group                                                       \
            -L$(LIB_DIR)                                                      \
			$(addprefix -L,$(USR_LIB_DIR))                                    \
    	    -Map $(IMAGE_DIR)/$(PROJECT_NAME).map                             \
		    -o $(IMAGE_DIR)/$(PROJECT_NAME).elf

###############################################################################

.PHONY : all clean build_start build_end

#******************************************************************************

all : build_start                                                             \
      $(addprefix $(IMAGE_DIR)/$(PROJECT_NAME).,$(ALL_TARGETS))               \
	  build_end

#******************************************************************************

clean :
	-$(RM) -r $(IMAGE_DIR)

#******************************************************************************

%.map %.elf : $(LIB_DIR)/start.o $(ALL_TARGET_LIBS)
	$(LD) $(LD_FLAGS)

#******************************************************************************

%.bin : %.elf
	$(OBJCOPY) -O binary $< $@

#******************************************************************************

%.lss : %.elf
	$(OBJDUMP) -h -S $< > $@

#******************************************************************************

%.sym : %.elf
	$(NM) -n $< > $@

#******************************************************************************

%.lzma : %.bin
	lzma e $< $@

#******************************************************************************

ifeq (loader,$(PROJECT_NAME))
%.sig : %.bin
else
%.sig : %.lzma
endif
	ImageSig $(PROJECT_DIR)/$(PROJECT_NAME)/inc/image_cfg.h $< $@

#******************************************************************************

build_start :
	-@$(MKDIR) $(IMAGE_DIR)

build_end : $(IMAGE_DIR)/$(PROJECT_NAME).elf
	@$(SIZE) -A -x $<

###############################################################################
