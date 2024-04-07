echo off
if not exist FHEAPCHK.OBS goto ERROR_NOOBJ
if not exist NHEAPCHK.OBS goto ERROR_NOOBJ

if not exist %1cs.lib goto NOSMALLLIB
echo Patching small library...
copy FHEAPCHK.OBS FHEAPCHK.OBJ
tlib %1cs.lib +- FHEAPCHK.OBJ
del FHEAPCHK.OBJ
copy NHEAPCHK.OBS NHEAPCHK.OBJ
tlib %1cs.lib +- NHEAPCHK.OBJ
del NHEAPCHK.OBJ
if errorlevel 1 goto ERROR_NOTLIB
goto MEDIUM
:NOSMALLLIB
echo Cannot find CS.LIB to patch small library.

:MEDIUM
if not exist %1cm.lib goto NOMEDLIB
echo Patching medium library...
copy FHEAPCHK.OBM FHEAPCHK.OBJ
tlib %1cm.lib +- FHEAPCHK.OBJ
del FHEAPCHK.OBJ
copy NHEAPCHK.OBM NHEAPCHK.OBJ
tlib %1cm.lib +- NHEAPCHK.OBJ
del NHEAPCHK.OBJ
goto COMPACT
:NOMEDLIB
echo Cannot find CM.LIB to patch medium library.

:COMPACT
if not exist %1cc.lib goto NOCOMPACTLIB
echo Patching compact library...
copy FHEAPCHK.OBC FHEAPCHK.OBJ
tlib %1cc.lib +- FHEAPCHK.OBJ
del FHEAPCHK.OBJ
copy NHEAPCHK.OBC NHEAPCHK.OBJ
tlib %1cc.lib +- NHEAPCHK.OBJ
del NHEAPCHK.OBJ
goto LARGE
:NOCOMPACTLIB
echo Cannot find CC.LIB to patch compact library.

:LARGE
if not exist %1cl.lib goto NOLARGELIB
echo Patching large library...
copy FHEAPCHK.OBL FHEAPCHK.OBJ
tlib %1cl.lib +- FHEAPCHK.OBJ
del FHEAPCHK.OBJ
copy NHEAPCHK.OBL NHEAPCHK.OBJ
tlib %1cl.lib +- NHEAPCHK.OBJ
del NHEAPCHK.OBJ
goto HUGE
:NOLARGELIB
echo Cannot find CL.LIB to patch large library.

:HUGE
if not exist %1ch.lib goto NOHUGELIB
echo Patching huge library...
copy FHEAPCHK.OBH FHEAPCHK.OBJ
tlib %1ch.lib +- FHEAPCHK.OBJ
del FHEAPCHK.OBJ
copy NHEAPCHK.OBH NHEAPCHK.OBJ
tlib %1ch.lib +- NHEAPCHK.OBJ
del NHEAPCHK.OBJ
goto DONE
:NOHUGELIB
echo Cannot find CH.LIB to patch huge library.
goto DONE

:ERROR_NOOBJ
echo The patch object module FHEAPCHK or NHEAPCHK is not in the current
echo directory.  Cannot patch libraries.
goto DONE

:ERROR_NOTLIB
echo A problem was encountered executing TLIB.  Either the Turbo Librarian
echo cannot be located along your DOS path, or an error occurred.  Consult
echo your Turbo C++ Users Guide for more information on why TLIB might not
echo be operating correctly.

:DONE
echo Done.
