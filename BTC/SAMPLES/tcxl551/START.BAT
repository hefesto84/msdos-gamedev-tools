echo off
cls
: START.BAT (S)
echo .                                                                        .
echo *=====[ The TesSeRact(TM) CXL User Interface Development System ]======*
echo  Copyright (c) 1987-1990, Innovative Data Concepts. All Rights Reserved
echo .                                                                        .
: initial batch file for setting up the directories
echo .
echo Installation Part 2
echo .
:
echo y >yes
echo Unpacking Header Files ...
TCXL_INC.EXE /x <yes
del TCXL_INC.EXE >nul
:
echo Unpacking Libraries ...
TCXL_LIB.EXE /x <yes
del TCXL_LIB.EXE >nul
:
echo Unpacking Tools ...
for %%z in (SWP BIN UTI CNV BAT) do TCXL_%%z /x <yes
for %%z in (SWP BIN UTI CNV BAT) do del TCXL_%%z.EXE >NUL
:
echo Unpacking Demonstration ...
TCXLDEMO /x <yes
del TCXLDEMO.EXE >NUL
:
echo Do You Wish To Unpack Documentation?
getyn
if errorlevel 1 goto DOCYES
GOTO DOCNO
:DOCYES
TCXL_DOC.EXE /x <yes
del TCXL_DOC.EXE >nul
goto TELLENV
:
:DOCNO
echo Documentation Is In TCXL_DOC.EXE
echo To Unpack, Give The Command TCXL_DOC.EXE /x
echo A Subdirectory TCXL\DOC will then be created and filled
:
:TELLENV
del yes >NUL
echo .
echo Source Code Is In TCXL_xxx.EXE Where The xxx is one of
echo MNU, ENT, HLP, SEL, WIN, DSP, INP, STR, VID, KEY, MOU or HDW
echo .
echo .
echo To compile or build libraries, you will need to set environment 
echo variables with the command SET VAR=VALUE
echo .
echo Compiler        TC/TC++     MSC 5.1       MSC 6.0   ZORTECH
echo                          QC 2.0 or less    QC 2.5
echo  SET TCXL_C=      T            5             6         Z
echo .
echo Assembler       TASM     MASM 5.1
echo  SET TCXL_A=      T           M
echo .
echo Installation Done
:ND
echo .                                                                      .
echo *=====[ The TesSeRact(TM) CXL User Interface Development System ]======*
echo  Copyright (c) 1987-1990, Innovative Data Concepts. All Rights Reserved
echo .                                                                      .
echo Done
