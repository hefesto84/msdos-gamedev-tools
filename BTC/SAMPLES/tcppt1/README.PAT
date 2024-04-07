Programmer's Platform
=====================
To patch the Turbo C++ integrated environment run the batch file
    PATTC.BAT  <path to TC.EXE>

* If you are in the Turbo C++ bin directory, you may omit the path to TC.EXE
* If you supply a path, you must also state the name of the file to patch,
* in this case TC.EXE, ie.   PATTC C:\TC\BIN\TC.EXE

This patch file (TCIDE.PAT) corrects the following problems:

[1]  Growing .PRJ files
Project files will grow to enormous sizes in certain situations.  If your
project files seem extraordinarily large, apply the patches and then
reconstruct the project file.  If you don't reconstruct the .PRJ file,
the problem may still exist.  If you allow the project files to
continually grow, you may encounter a variety of strange behavior from
the environment, including possible compiler hangs.

[2]  CGA Border Color
When exiting the Turbo C++ programmer's platform, the border color of
CGA video cards gets set to BLUE.


Run Time Libraries
==================
To patch the Turbo C++ Libraries:

The Run Time Libraries are patched by running batch files which invoke
the Turbo Librarian to replace the existing objects in the libraries
with the new.

Batch files are supplied which correct the following problems:
( The batch files will accept the path to the TC++ libraries as an    )
( optional argument.  If you wish to place the path to the libraries  )
( on the command line, be sure to include a trailing backslash '\'    )
( character.                                                          )

[1]  FINDFIRST / FINDNEXT    -  PTFNDFST.BAT
The findfirst()/findnext() functions load _errno with the incorrect
value when no more files are located.

[2]  SETJMP                  -  PTSETJMP.BAT
The setjmp function invalidates the di register.

[3]  REALLOC                 -  PTREALOC.BAT
The realloc function allows the allocation of more than available memory
in the small data models.

[4]  HEAPFILLFREE            -  PTFILFRE.BAT
Under certain conditions farheapfillfree and nearheapfillfree would
corrupt memory.

[5]  WILDARGS
The WILDARGS.OBJ shipped with Turbo C++ 1.0 does not operate in the
large data models properly.  This can be patched simply by copying
the WILDARGS.OBJ in this patch file to your Turbo C++ library directory.

Overlays
========
Using _ovrbuffer to set a new overlay pool size does not work in the
huge memory model.  The OVERLAY.LIB shipped in this patch file corrects
this problem.  Simply copy the new library to your Turbo C++ library
directory.

Header files
============
The DOS.H header file shipped with Turbo C++ did not include macro
versions of inportb() and outportb().  To replace the original with
a version of DOS which contains this information simply copy the DOS.H
file from this patch file to your Turbo C++ include directory.

