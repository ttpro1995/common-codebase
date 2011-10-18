@echo off

:: ===========================================================================
::  These lines can be changed to fit product usage.
:: ===========================================================================

:: Define python install directory
set PYTHON_DIR=%COMPILER_BASE%\python

:: Define python entry file name at here
set ENTRY_FILE=main.py

:: Define generated exe file type
::   --> console : The generated exe file with Console.
::   --> windows : The generated exe file without Console.
set EXE_FILE_TYPE=console



:: ===========================================================================
::  These lines are forbidden to be changed, while porting to another product.
:: ===========================================================================

path %PYTHON_DIR%;%path%
python %PYTHON_DIR%\py2exe_setup.py %ENTRY_FILE% --%EXE_FILE_TYPE%

