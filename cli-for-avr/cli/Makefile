
include ../makefile.ini

LIB      := libcli.a

COBJS    := cli.o
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
