@echo off & setlocal
set std=%1
if "%std%"=="" set std=c++14

set flags=-Wpedantic -Wno-padded -Wno-missing-noreturn
set   gpp=g++

%gpp% -std=%std% -O2 -Wall -Wextra %flags% -o optional-main.t.exe -I../include/nonstd optional-main.t.cpp optional.t.cpp && optional-main.t.exe
endlocal
