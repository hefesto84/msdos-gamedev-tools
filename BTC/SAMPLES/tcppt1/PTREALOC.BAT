echo off
if not exist FARHEAP.OBS goto ERROR_NOOBJ
if not exist NEARHEAP.OBS goto ERROR_NOOBJ

if not exist %1cs.lib goto NOSMALLLIB
echo Patching small library...
copy FARHEAP.OBS FARHEAP.OBJ
tlib %1cs.lib +- FARHEAP.OBJ
del FARHEAP.OBJ
copy NEARHEAP.OBS NEARHEAP.OBJ
tlib %1cs.lib +- NEARHEAP.OBJ
del NEARHEAP.OBJ
if errorlevel 1 goto ERROR_NOTLIB
goto MEDIUM
:NOSMALLLIB
echo Cannot find CS.LIB to patch small library.

:MEDIUM
if not exist %1cm.lib goto NOMEDLIB
echo Patching medium library...
copy FARHEAP.OBM FARHEAP.OBJ
tlib %1cm.lib +- FARHEAP.OBJ
del FARHEAP.OBJ
copy NEARHEAP.OBM NEARHEAP.OBJ
tlib %1cm.lib +- NEARHEAP.OBJ
del NEARHEAP.OBJ
goto COMPACT
:NOMEDLIB
echo Cannot find CM.LIB to patch medium library.

:COMPACT
if not exist %1cc.lib goto NOCOMPACTLIB
echo Patching compact library...
copy FARHEAP.OBC FARHEAP.OBJ
tlib %1cc.lib +- FARHEAP.OBJ
del FARHEAP.OBJ
copy NEARHEAP.OBC NEARHEAP.OBJ
tlib %1cc.lib +- NEARHEAP.OBJ
del NEARHEAP.OBJ
goto LARGE
:NOCOMPACTLIB
echo Cannot find CC.LIB to patch compact library.

:LARGE
if not exist %1cl.lib goto NOLARGELIB
echo Patching large library...
copy FARHEAP.OBL FARHEAP.OBJ
tlib %1cl.lib +- FARHEAP.OBJ
del FARHEAP.OBJ
copy NEARHEAP.OBL NEARHEAP.OBJ
tlib %1cl.lib +- NEARHEAP.OBJ
del NEARHEAP.OBJ
goto HUGE
:NOLARGELIB
echo Cannot find CL.LIB to patch large library.

:HUGE
if not exist %1ch.lib goto NOHUGELIB
echo Patching huge library...
copy FARHEAP.OBH FARHEAP.OBJ
tlib %1ch.lib +- FARHEAP.OBJ
del FARHEAP.OBJ
copy NEARHEAP.OBH NEARHEAP.OBJ
tlib %1ch.lib +- NEARHEAP.OBJ
del NEARHEAP.OBJ
goto DONE
:NOHUGELIB
echo Cannot find CH.LIB to patch huge library.
goto DONE

:ERROR_NOOBJ
echo The patch object module FARHEAP or NEARHEAP is not in the current
echo directory.  Cannot patch libraries.
goto DONE

:ERROR_NOTLIB
echo A problem was encountered executing TLIB.  Either the Turbo Librarian
echo cannot be located along your DOS path, or an error occurred.  Consult
echo your Turbo C++ Users Guide for more information on why TLIB might not
echo be operating correctly.

:DONE
echo Done.
