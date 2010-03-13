:: #############################################################################
::     Board configuration
:: #############################################################################

set CPU_TYPE=at91rm9200
set OS_TYPE=ucos-ii

:: set target endian
set TARGET_ENDIAN=little

:: The real code start address
if "%CPU_TYPE%" == "at91rm9200" (
  set CPU_ARCH=arm
  set LOADER_TEXTBASE=0x20F00000
  set KERNEL_TEXTBASE=0x20000000
) else if "%CPU_TYPE%" == "s3c44b0" (
  set CPU_ARCH=arm
  LOADER_TEXTBASE=0x0CF00000
  KERNEL_TEXTBASE=0x0C000000
) else (
  echo.
  echo Unknown CPU Type, Press any key to exit!
  pause>nul
  exit
)
