@echo off

if "%COMPILER_BASE%"=="" (
  set COMPILER_BASE=D:\compiler
)

:: Set cross Compiler
set CROSS_COMPILER=arm-none-eabi-
path %COMPILER_BASE%\yagarto\bin

path %path%;%COMPILER_BASE%\msys\bin;%COMPILER_BASE%\MinGW\bin

start
