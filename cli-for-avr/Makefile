
sinclude ./makefile.ini

#===============================================================================

.PHONY : all clean distclean program init_check $(SUB_DIRS) $(IMAGE_DIR)

################################################################################

all : init_check
	@set -e;                                          \
	for i in $(SUB_DIRS);                             \
	do                                                \
		$(MAKE) -C $$i $@;                            \
	done
	$(MAKE) -C $(IMAGE_DIR) $@

#===============================================================================

clean distclean : init_check
	@for i in $(SUB_DIRS);                            \
	do                                                \
		$(MAKE) -C $$i $@;                            \
	done
	$(MAKE) -C $(IMAGE_DIR) $@

#===============================================================================

program :
	cd $(IMAGE_DIR) && $(MAKE) $@

#===============================================================================

init_check :
	@if [ ! -f makefile.ini ]; then                   \
		echo "You should config your project first!"; \
		exit 2;                                       \
	fi

################################################################################
