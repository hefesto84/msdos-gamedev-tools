      ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
      ³                                                               ³
      ³                     C M A T H for  C / C++                    ³
      ³                                                               ³
      ³                     Shareware Version 3                       ³
      ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

Contents
    1. Introduction
    2. System requirements
    3. Installation
    4. Running the example
    5. Documentation
    6. What's New?
    7. Copyright
    8. Registered Version

1. Introduction
---------------
Didn't you already feel angry about the slow, inaccurate, or even failing
complex-number  functions of your complex class library? Or did you perhaps
regret that complex numbers are offered only by C++, but not by C? Or that
no functions in polar coordinates were available?

CMATH is a comprehensive library for complex-number arithmetis and
mathematics, both in cartesian and in polar coordinates.
All functions may alternatively be called from classic C with
type-specific function names (like cf_sin, cd_exp, pe_sqrt),
or from C++ with overloaded function names and operators.

Superior speed, accuracy and safety are achieved through the implementation
in Assembly language (as opposed to the C++ code of other complex class
libraries).

Each of the three floating-point accuracies (float, double, and extended)
is given its own, optimized version of each function.


2. System requirements
----------------------
This version of CMATH is designed for IBM AT, PS/2 or compatibles,
equipped at least with a 80386 CPU and a 80387 coprocessor
(Visual C++ version: at least 486DX).

Depending on your choice when ordering or downloading, you need
- Borland C++ (Turbo C++), version 3.0 or higher, memory
  models Win32, Windows-LARGE, or  DOS-LARGE
- or Microsoft Visual C++, version 5.0 or higher, "single-thread debug"
  or "multi-thread debug"

Registered version for Borland C/C++:
all memory models; separate libraries optimized for four degrees of
back-compatibility from Pentium III down to 286 with or without coprocessor.

Registered version for Microsoft Visual C++:
all PC models using the "single-thread", "multi-thread", or
"multi-thread DLL" run-time libraries, both for debug and release, in
versions for Pentium III+ and for 486DX/Pentium+.


3. Installation
---------------
Please run the installation program, INSTALL.EXE.
INSTALL.EXE moves all CMATH files into their correct subdirectory
and starts the clock for the trial period.
You may change the default directory structure by hand when the
installation is complete.
To install CMATH on Windows NT, 2000, XP etc., you need to log in
with administrator privileges.

After you completed the installation, you must set the library search path
and the include-file search path according to your CMATH directory choice:
Say, your CMATH directory is C:\CMATH. Then, your search paths are:
C:\CMATH\LIB          for the libraries,
C:\CMATH\INCLUDE      for the include-files.

Add these paths to the standard settings in the menu
"Extras / Options / Directories" in MS Visual C++ or 
"Options / Project / Directories" of Borland C++ and BC++ Builder
(remember: in Borland C++, a semicolon is used to separate entries in
these fields.)

For MS Visual C++, you also have to include the Windows API import library:
In the menu   Project / Settings / Linker / Object and Library modules,
you have to add    user32.lib   (if it is not yet there).
Otherwise you would get the linker error
LNK2001: Unresolved external symbol __imp__MessageBoxA@16??


4. Running the example
----------------------
Check your installation by compiling and running CDEMO.CPP. Follow the
instructions in the header of that file.
If you get compiler errors like "Cannot open include file ...", or a
linker error "Unresolved external", you probably did not set the library
and include paths correctly, or you forgot to include the respective
library into your project.


5. Documentation
----------------
The full CMATH documentation is to be found in the file CMATH.HTM
to be read with a browser like Netscape or IE.


6. What's New?
--------------
a) In the functions for data-type down-conversion (cdtocf etc.),
   OVERFLOW errors do no longer lead to an error message, but are
   silently treated by setting the result to the maximum value possible
   (with the correct sign).

b) a few minor improvements of accuracy

Version 3:
The classes "polar" of float, double, and extended precision are introduced
along with a whole range of functions and operators in polar coordinates.
As a consequence, the member function "polar" of the cartesian complex
classes had to be replaced by "magargtoc".



7. Copyright
------------
The copyright owner of this product as a whole and of all its constituent
parts is
         OptiCode 
         Dr. Martin Sander Software Development
         Steinachstr. 9A
         D-69198 Schriesheim
         Germany
         e-mail: MartinSander@optivec.com
         http://www.optivec.com

This Shareware version of CMATH is freely distributable in unchanged form.
For the distribution of applications created using CMATH, you need the
registered version. The detailed licence conditions are described in
chapter 1.2 of the file CMATH.HTM.


8. Registered Version
---------------------
If you like CMATH and decide to use it, please be fair and register.
The registered version of CMATH

-  supports all memory models of Windows95/98/NT, 3.x, and DOS of Borland C++,
   or all debug and release targets of Visual C++, respectively.

-  has individually optimized libraries for each degree of processor
   backward-compatibility (Borland C++ only):
      Pentium III+
      486DX/Pentium+
      386+ (387 coprocessor required)
      286+ (no coprocessor required).

-  comes with printed documentation.

-  entitles you to two years of free updates (by download from our web site).

-  costs USD 60 or EUR 59 for the commercial edition,
         USD 39 or EUR 39 for the educational edition,
   and can be ordered by e-mail from the author or through the
   following secure-server web-sites:

   SWREG:
   http://www.swreg.org/soft_shop/47/shopscr2.html

   ShareIt:
   CMATH for Borland C++:
       http://www.shareit.com/programs/101353.htm
   CMATH for Microsoft Visual C++:
       http://www.shareit.com/programs/103422.htm

See chapter 1.3 of CMATH.HTM for details about ordering.


    * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Copyright (C) OptiCode - Dr. Martin Sander Software Dev. 1996-2002

