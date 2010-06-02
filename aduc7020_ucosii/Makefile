
PROJECT_NAME = aduc7020_ucosii

ALL_SRC_DIR = . 														\
			  ./lib														\
			  ./inc														\
			  ./ucos_ii/porting											\
			  ./ucos_ii/source                     			 			\
			  ./startup

CFLAGS = -g -O2 -Wall -W  												\
		 -mcpu=arm7tdmi -mapcs-frame -mthumb-interwork -mlittle-endian	\
		 $(addprefix -I,$(ALL_SRC_DIR))

ALL_OBJS = startup.o os_cpu_a.o main.o ucos_ii.o os_cpu_c.o

# file searching path
vpath %.S   $(ALL_SRC_DIR)
vpath %.c   $(ALL_SRC_DIR)
vpath %.o   $(ALL_SRC_DIR)


all: $(PROJECT_NAME).elf

clean:
	rm -f $(ALL_OBJS) $(wildcard $(PROJECT_NAME).*)

%.o : %.S
	gcc -c $(CFLAGS) $< -o $@

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

$(PROJECT_NAME).elf : $(ALL_OBJS)
	gcc -nostartfiles -T aduc7020.ld $(CFLAGS) $^ 	\
		-Wl,-Map=$(PROJECT_NAME).map -o $@
	objdump -h -S $@ > $(PROJECT_NAME).lss
	nm -n -S $@ > $(PROJECT_NAME).sym
	objcopy -O binary $@ $(PROJECT_NAME).bin
	@size -A -x $@

