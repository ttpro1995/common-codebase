
How to compile u-boot under MinGW (Windows XP SP3):

*) Download u-boot:
     ftp://ftp.denx.de/pub/u-boot/u-boot-2011.09.tar.bz2
*) Download Msys:
     http://downloads.sourceforge.net/mingw/MSYS-1.0.11.exe
*) Download Codelite with MinGW:
     http://sourceforge.net/projects/codelite/files/Releases/codelite-3.0/codelite-3.0.0.5041-mingw4.4.1.exe/download
*) Download Yagarto:
     http://sourceforge.net/projects/yagarto/files/YAGARTO%20for%20Windows/20110429/yagarto-bu-2.21_gcc-4.6.0-c-c%2B%2B_nl-1.19.0_gdb-7.2_eabi_20110429.exe/download
*) Install:
     Msys    : install to D:\compiler\msys\
     Codelite: only install "GCC 4.4.1 (MinGW) full", and install to D:\compiler\MinGW\
     Yagarto : install to D:\_compiler\yagarto\
*) Decompression u-boot to D:\project\u-boot\
*) Copy env.bat/Makefile/mkconfig to D:\project\u-boot\
*) Double click env.bat
*) Enter "make at91rm9200ek_config"
*) Enter "make"
*) Wait for a while, then you will get u-boot.bin under D:\project\u-boot\

--- Panda.Xiong (yaxi1984@gmail.com)
--- 2011/09/30
