
include ../../makefile.ini

vpath %.c ./$(CPU)/src ./common/src
vpath %.S ./$(CPU)/src ./common/src
vpath %.h ./$(CPU)/inc ./common/inc

LIB	     := lib$(ARCH).a

COBJS    := cpu_delay.o interrupt.o sys_init.o uart.o
AOBJS    :=

ALL_OBJS := $(AOBJS) $(COBJS)
ALL_DEPS := $(ALL_OBJS:.o=.d)
ALL_SRC  := $(AOBJS:.o=.S) $(COBJS:.o=.c)

.PHONY : all clean distclean

all : $(ALL_DEPS) $(ALL_OBJS) $(LIB)

clean :
	-$(RM) ./o/*.o $(LIB)

distclean : clean
	-$(RM) -r ./o

$(LIB): $(addprefix ./o/,$(ALL_OBJS))
	$(AR) -crv $@ $^

sinclude $(addprefix ./o/,$(ALL_DEPS))
