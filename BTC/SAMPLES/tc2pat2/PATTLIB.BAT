echo off
echo  
echo This batch file patches TLIB.EXE to fix a problem where line
echo information is lost when creating a library.
echo  
echo Syntax:  PATTLIB [TLIB-Directory]
echo Example: PATTLIB C:\TC\
echo  
echo If no parameter is specified for TLIB's directory,  TLIB.EXE in 
echo the current directory is patched.  Note: If you specify a pathname,
echo the last '\' is required.
echo  
echo Hit any key to patch TLIB, ^C to exit
echo  
pause
PATCH P2TL2.DIF %1TLIB.EXE
