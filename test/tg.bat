@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -o optional-main.t.exe -I../include/nonstd optional-main.t.cpp optional.t.cpp && optional-main.t.exe
@endlocal

