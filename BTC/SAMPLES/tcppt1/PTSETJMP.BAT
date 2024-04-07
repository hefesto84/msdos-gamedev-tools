echo off
if not exist SETJMP.OBS goto ERROR_NOOBJ

if not exist %1cs.lib goto NOSMALLLIB
echo Patching small library...
copy SETJMP.OBS SETJMP.OBJ
tlib %1cs.lib +- SETJMP.OBJ
del SETJMP.OBJ
if errorlevel 1 goto ERROR_NOTLIB
goto MEDIUM
:NOSMALLLIB
echo Cannot find CS.LIB to patch small library.

:MEDIUM
if not exist %1cm.lib goto NOMEDLIB
echo Patching medium library...
copy SETJMP.OBM SETJMP.OBJ
tlib %1cm.lib +- SETJMP.OBJ
del SETJMP.OBJ
goto COMPACT
:NOMEDLIB
echo Cannot find CM.LIB to patch medium library.

:COMPACT
if not exist %1cc.lib goto NOCOMPACTLIB
echo Patching compact library...
copy SETJMP.OBC SETJMP.OBJ
tlib %1cc.lib +- SETJMP.OBJ
del SETJMP.OBJ
goto LARGE
:NOCOMPACTLIB
echo Cannot find CC.LIB to patch compact library.

:LARGE
if not exist %1cl.lib goto NOLARGELIB
echo Patching large library...
copy SETJMP.OBL SETJMP.OBJ
tlib %1cl.lib +- SETJMP.OBJ
del SETJMP.OBJ
goto HUGE
:NOLARGELIB
echo Cannot find CL.LIB to patch large library.

:HUGE
if not exist %1ch.lib goto NOHUGELIB
echo Patching huge library...
copy SETJMP.OBH SETJMP.OBJ
tlib %1ch.lib +- SETJMP.OBJ
del SETJMP.OBJ
goto DONE
:NOHUGELIB
echo Cannot find CH.LIB to patch huge library.
goto DONE

:ERROR_NOOBJ
echo The patch object module SETJMP is not in the current directory.
echo Cannot patch libraries.
goto DONE

:ERROR_NOTLIB
echo A problem was encountered executing TLIB.  Either the Turbo Librarian
echo cannot be located along your DOS path, or an error occurred.  Consult
echo your Turbo C++ Users Guide for more information on why TLIB might not
echo be operating correctly.

:DONE
echo Done.
