@setlocal
@set std=%1
@if not "%std%"=="" set std=-std:%std%
cl -W3 -EHsc %std% -Doptional_EXPERIMENTAL_FUNCTIONAL_EXTENSTIONS -I../include/nonstd optional-lite.t.cpp optional.t.cpp && optional-lite.t.exe
@endlocal

