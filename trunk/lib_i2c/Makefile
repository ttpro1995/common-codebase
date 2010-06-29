
ALL_C_SRC   = lib_i2c.c drv_i2c.c drv_i2c_lpt.c drv_i2c_usb.c
TARGET_NAME = lib_i2c
TARGET_DIR  = .\build

INPOUT32_C_SRC = inpout32drv.c
INPOUT32_C_DIR = .\inpout32

.PHONY: all

all:
	-mkdir $(TARGET_DIR)
	gcc -c -I./inc $(addprefix ./src/,$(ALL_C_SRC)) $(addprefix $(INPOUT32_C_DIR)\,$(INPOUT32_C_SRC)) -o $(TARGET_DIR)/$(TARGET_NAME).o
	gcc -shared -fPIC -o $(TARGET_DIR)/$(TARGET_NAME).dll $(TARGET_DIR)/$(TARGET_NAME).o -Wl,--out-implib,$(TARGET_DIR)/$(TARGET_NAME).a

