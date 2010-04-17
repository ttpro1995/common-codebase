
include ../makefile.ini

################################################################################

# All objects to link
ALL_OBJS := $(foreach dir,$(SUB_DIRS),$(wildcard $(PROJECT_DIR)/$(dir)/o/*.o))

#===============================================================================

.PHONY : all clean distclean program show_size

################################################################################


################################################################################
#======= avrdodo flags  =======
# -p : CPU type
# -P : Program port
# -c : Programmer type
#==============================
AVRDUDE_FLAGS         := -p $(CPU) -P lpt1 -c stk200
AVRDUDE_WRITE_FLASH   := -U flash:w:$(PROJECT).hex
AVRDUDE_WRITE_EEPROM  := -U eeprom:w:$(PROJECT).eep

################################################################################


################################################################################

all : clean $(PROJECT).elf $(PROJECT).map $(PROJECT).hex $(PROJECT).eep  \
	  $(PROJECT).lss $(PROJECT).sym show_size

#===============================================================================

clean distclean :
	-$(RM) $(PROJECT).*

#===============================================================================

program :
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

#===============================================================================

$(PROJECT).elf $(PROJECT).map :
	$(CC) $(CFLAGS) $(ALL_OBJS) -lm -Wl,-Map=$(PROJECT).map,--cref -o $(PROJECT).elf

$(PROJECT).hex : $(PROJECT).elf
	$(OBJCOPY) -O ihex -R .eeprom $(PROJECT).elf $(PROJECT).hex

$(PROJECT).eep : $(PROJECT).elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load"      \
		--change-section-lma .eeprom=0 -O ihex $(PROJECT).elf $(PROJECT).eep

$(PROJECT).lss : $(PROJECT).elf
	$(OBJDUMP) -h -S $(PROJECT).elf > $(PROJECT).lss

$(PROJECT).sym : $(PROJECT).elf
	$(NM) -n $(PROJECT).elf > $(PROJECT).sym

show_size : $(PROJECT).elf
	@echo
	@$(SIZE) -A $(PROJECT).elf
	@$(SIZE) -C --mcu=$(CPU) $(PROJECT).elf

################################################################################
