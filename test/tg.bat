@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -o optional-lite.t.exe -I../include/nonstd optional-lite.t.cpp optional.t.cpp && optional-lite.t.exe
@endlocal

