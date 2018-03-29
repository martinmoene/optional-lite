@setlocal
@set std=%1
@if not "%std%"=="" set std=-std:%std%
cl -W3 -EHsc %std% -I../include/nonstd optional-main.t.cpp optional.t.cpp && optional-main.t.exe
@endlocal

