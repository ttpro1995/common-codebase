@path D:\compiler\arm-uclibc\bin
objdump -h -S %1.elf > %1.lss
nm -n -S %1.elf > %1.sym
objcopy -O binary %1.elf %1.bin
