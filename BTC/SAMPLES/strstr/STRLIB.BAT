echo off
rem Both the .LIB and .OBJ files to be referenced by this proc must reside
rem in the same directory and this proc must be started from that directory.
cls
if %1. == . goto needp
if not exist strstr%1.obj goto needp
echo strlib: now updating strstr.obj in c%1.lib using strstr%1.obj
ren strstr%1.obj strstr.obj
lib c%1 -+STRSTR.OBJ ; > NUL
ren strstr.obj strstr%1.obj
echo strlib: update of c%1.lib successfully completed
goto okex
:needp
echo strlib: you must specify memory model type as: s, c, m, l or h
:okex

