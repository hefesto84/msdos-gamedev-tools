echo off
echo  
echo This batch file will install the replacement .OBJ module to fix
echo the stat() function in the tiny memory model.
echo  
echo Syntax:  PATSTAT [LIB-directory]
echo Example: PATSTAT C:\TC\LIB\
echo  
echo If you specify a path name, the last '\' is required.  If no
echo path name is specified, the .LIB files in the current 
echo directory are used.  
echo  
echo TLIB.EXE must be in the current directory or on your DOS exec path.
echo  
echo Hit any key to patch stat, ^C to exit
echo  
pause

if not exist %1*.lib goto HELP

copy stat.obj tzset.obj
tlib /E /0 %1cs -+tzset.obj
if errorlevel 1 goto ERROR
del tzset.obj

if exist %1*.bak del %1*.bak
goto DONE

:ERROR
echo ERROR adding module to library
echo installation terminated
goto DONE

:HELP
echo  
echo ERROR: invalid path specification

:DONE
