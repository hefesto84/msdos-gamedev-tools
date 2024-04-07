echo off
if not exist FINDFIRS.OBS goto ERROR_NOOBJ

if not exist %1cs.lib goto NOSMALLLIB
echo Patching small library...
copy FINDFIRS.OBS FINDFIRS.OBJ
tlib %1cs.lib +- FINDFIRS.OBJ
del FINDFIRS.OBJ
if errorlevel 1 goto ERROR_NOTLIB
goto MEDIUM
:NOSMALLLIB
echo Cannot find CS.LIB to patch small library.

:MEDIUM
if not exist %1cm.lib goto NOMEDLIB
echo Patching medium library...
copy FINDFIRS.OBM FINDFIRS.OBJ
tlib %1cm.lib +- FINDFIRS.OBJ
del FINDFIRS.OBJ
goto COMPACT
:NOMEDLIB
echo Cannot find CM.LIB to patch medium library.

:COMPACT
if not exist %1cc.lib goto NOCOMPACTLIB
echo Patching compact library...
copy FINDFIRS.OBC FINDFIRS.OBJ
tlib %1cc.lib +- FINDFIRS.OBJ
del FINDFIRS.OBJ
goto LARGE
:NOCOMPACTLIB
echo Cannot find CC.LIB to patch compact library.

:LARGE
if not exist %1cl.lib goto NOLARGELIB
echo Patching large library...
copy FINDFIRS.OBL FINDFIRS.OBJ
tlib %1cl.lib +- FINDFIRS.OBJ
del FINDFIRS.OBJ
goto HUGE
:NOLARGELIB
echo Cannot find CL.LIB to patch large library.

:HUGE
if not exist %1ch.lib goto NOHUGELIB
echo Patching huge library...
copy FINDFIRS.OBH FINDFIRS.OBJ
tlib %1ch.lib +- FINDFIRS.OBJ
del FINDFIRS.OBJ
goto DONE
:NOHUGELIB
echo Cannot find CH.LIB to patch huge library.
goto DONE

:ERROR_NOOBJ
echo The patch object module FINDFIRS is not in the current directory.
echo Cannot patch libraries.
goto DONE

:ERROR_NOTLIB
echo A problem was encountered executing TLIB.  Either the Turbo Librarian
echo cannot be located along your DOS path, or an error occurred.  Consult
echo your Turbo C++ Users Guide for more information on why TLIB might not
echo be operating correctly.

:DONE
echo Done.
