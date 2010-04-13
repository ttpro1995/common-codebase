
CC = gcc
TARGET_NAME  = cli_test

CLI_KERNEL_SRC   = ../kernel
CLI_PORTING_SRC  = ../porting
ALL_INC          = . $(CLI_KERNEL_SRC) $(CLI_PORTING_SRC)

CLI_KERNEL_FILE  = cli_api.c cli_cmd.c cli_history.c cli_io.c cli_par.c cli_reg.c
CLI_PORTING_FILE = cli_vt.c

C_FLAGS +=  -g -O2                                                             \
			                                                                   \
		    -ansi -fsigned-char -funsigned-bitfields                           \
            -Wall -Wpointer-arith -Wstrict-prototypes -Winline -Wundef         \
			-Wsign-compare -Wuninitialized                                     \
            -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-qual             \
			-Wcast-align -Waggregate-return -Wpacked -Wwrite-strings           \
			-Werror                                                            \
			                                                                   \
			$(addprefix $(CLI_KERNEL_SRC)/,$(CLI_KERNEL_FILE))                 \
		    $(addprefix $(CLI_PORTING_SRC)/,$(CLI_PORTING_FILE))               \
		    $(addprefix -I,$(ALL_INC))

.PHONY: all

all:
	$(CC) $(C_FLAGS) -o $(TARGET_NAME).exe $(TARGET_NAME).c		

