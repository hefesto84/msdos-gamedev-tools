Turbo C 2.0 patches
-------------------

This archive contains the batch files listed below which fix five
problems in Turbo C 2.0.  Also contained in this archive is a
utility called PATCH.COM which is used to apply the patches.

For instructions on applying each of the five patches, simply type
the name of the batch file for the patch at the DOS prompt followed
by a carriage return.

PATPRNTF.BAT - fixes the problem with the %.0f format specifier by
using TLIB.EXE to install new .OBJ files into your Turbo C
2.0 .LIB files.

PATTLIB.BAT - uses PATCH.COM to patch TLIB.EXE to eliminate the
problem in which TLIB strips out line number information from .LIB
modules.

PATTC.BAT - uses PATCH.COM to patch TC.EXE so that __TURBOC__
has the value 0x0200 and so that TC does not overwrite the INT
60H vector.

PATSTAT.BAT - fixes the problem with stat in the tiny model by
using TLIB.EXE to install a new .OBJ file into your Turbo C
2.0 .LIB file.

PATFLOAT.BAT - fixes "floating point formats not linked" problem.

