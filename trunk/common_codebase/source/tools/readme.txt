This base tools are for minimum compiler tools.

 |
 |-- bin
 |     |-- cygwinb19.dll --+   these three files are get from Marvell SDK 
 |     |-- mkdir.exe     --|--   Pss-DX-1.1.6.
 |     |-- sh.exe        --+   it's based on Cygwin.
 |     |
 |     |-- echo --+   these two files are getted it from
 |     |          |--   MSYS-1.0.11-2004.04.30-1.exe,
 |     |-- pwd  --+   to fix a bug of sh.exe, when using a complex makefile.
 |     |          
 |     |-- make.exe --+   these two files are get from
 |     |              |--   arm-2007q3-53-arm-none-eabi.exe.
 |     |-- rm.exe   --+   these two files are individal of Cygwin and MinGW.
 |
 |-- yagarto : this is a free arm-elf-gcc 4.2.1 compiler, and it's only
                 contains the minimum compiler, for c.
			   if you want to get the full version,
			     please re-download it from yagarto.
			   note : the lib is only for hard-float, and don't support 
			            thumb-interwork.
				      if you want an soft-float lib, please try 
					    arm-2007q3-53-arm-none-eabi.exe, but there is not a 
                        big-endian supported lib for it.
