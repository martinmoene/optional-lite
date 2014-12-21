@echo off

for %%s in ( c++98 c++03 c++11 c++1y ) do (

   mingw32-make clean test STD=%%s
)
