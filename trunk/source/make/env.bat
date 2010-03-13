:: #############################################################################
::     Get Project from User to Build
:: #############################################################################
@cls

:input_project
@echo.
@echo Select one target to build:
@echo     1 : bootloader
@echo     2 : kernel
@echo     3 : exit

:: #############################################################################
@set /p PROJECT=Project:

:: #############################################################################
@if "%PROJECT%" == "1" @(
    set PROJECT_NAME=loader
    goto next_stage
)  else if "%PROJECT%" == "2" @(
    set PROJECT_NAME=kernel
    goto next_stage
) else if "%PROJECT%" == "3" @(
    exit
) else @(
    echo.
	echo Invalid input!
    goto input_project
)

:next_stage
:: #############################################################################
::     Do Other Settings
:: #############################################################################
@echo off

::  set system command search path
set TOP_PATH=%CD%
set TOOLS_PATH=%TOP_PATH%\tools

call %TOP_PATH%\make\board_cfg.bat

:: add compiler search path
set INC_GCC_PATH=%TOOLS_PATH%\cross_compiler\%CPU_ARCH%\include
if "%TARGET_ENDIAN%" == "big" (
  set LIB_GCC_PATH=%TOOLS_PATH%\cross_compiler\%CPU_ARCH%\lib\be
) else (
  set LIB_GCC_PATH=%TOOLS_PATH%\cross_compiler\%CPU_ARCH%\lib\le
)
path %TOOLS_PATH%\cross_compiler\%CPU_ARCH%\bin


:: add base tools search path
path %TOOLS_PATH%\bin;%path%

:: set other directory, used by makefile
set PROJECT_DIR=%TOP_PATH:\=/%
set TOOLS_DIR=%TOOLS_PATH:\=/%
set CFG_DIR=%PROJECT_DIR%/make
set LIB_GCC_DIR=%LIB_GCC_PATH:\=/%
set INC_GCC_DIR=%INC_GCC_PATH:\=/%

:: change command prompt to project name
set PROMPT=%PROJECT_NAME%$G

:: #############################################################################
::     Ready to Enter the corresponding target directory
:: #############################################################################
cd %TOP_PATH%\%PROJECT_NAME%
cls
