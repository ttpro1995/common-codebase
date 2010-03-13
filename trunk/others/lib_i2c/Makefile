
ALL_C_SRC   = lib_i2c.c drv_i2c.c drv_i2c_lpt.c drv_i2c_usb.c
TARGET_NAME = lib_i2c
TARGET_DIR  = .\build

.PHONY: all

all:
	-mkdir $(TARGET_DIR)
	gcc -c -I./inc $(addprefix ./src/,$(ALL_C_SRC)) -o $(TARGET_DIR)/$(TARGET_NAME).o
	gcc -shared -fPIC -o $(TARGET_DIR)/$(TARGET_NAME).dll $(TARGET_DIR)/$(TARGET_NAME).o -Wl,--out-implib,$(TARGET_DIR)/$(TARGET_NAME).a
	cp -f $(TARGET_DIR)/*.dll $(windir)/system32

