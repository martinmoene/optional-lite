@setlocal
@set std=%1
@if not "%std%"=="" set std=-std:%std%
clang-cl -m32 -W3 -EHsc %std% -Doptional_CONFIG_SELECT_OPTIONAL=optional_OPTIONAL_NONSTD -I../include/nonstd optional-main.t.cpp optional.t.cpp && optional-main.t.exe
@endlocal

