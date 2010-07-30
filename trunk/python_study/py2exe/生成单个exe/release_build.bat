@echo off

set CURRENT_DIR=%CD%
set PYTHON_DIR=D:\compiler\python

path %PYTHON_DIR%;%path%

python %PYTHON_DIR%\py2exe_setup.py %CURRENT_DIR%\main.py --console

